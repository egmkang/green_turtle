//Copyright 2012, egmkang wang.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of green_turtle nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// author: egmkang (egmkang@gmail.com)

#ifndef __MESSAGE_QUEUE__
#define __MESSAGE_QUEUE__
#include <assert.h>
#include <cstddef>
#include <atomic>
#include <noncopyable.h>

namespace green_turtle{
namespace details{
struct AtomicIndex{
  AtomicIndex(size_t init = 0) : value_(init){}
  inline size_t load(std::memory_order memory_order)
  {
    return value_.load(memory_order);
  }
  inline void store(size_t value, std::memory_order memory_order)
  {
    value_.store(value, memory_order);
  }
  std::atomic<size_t> value_;
};

struct PaddedAtomicIndex : public AtomicIndex{
  size_t padding_values_[7];
};

struct VolatileIndex
{
  VolatileIndex(size_t init = 0) : value_(init){}
  inline size_t load(std::memory_order memory_order)
  {
    (void)memory_order;
    return value_;
  }
  inline void store(size_t value, std::memory_order memory_order)
  {
    (void)memory_order;
    value_ = value;
  }
  volatile size_t value_;
};

struct PaddedVolatileIndex : public VolatileIndex{
  size_t padding_values_[7];
};

}

//support POD data only
//1:1 MessageQueue
//N:1 equals N*(1:1)
template<class T>
class MessageQueue : NonCopyable
{
 public:
  typedef T value_type;
 public:
  MessageQueue(size_t size = 128*1024)
    : read_idx_()
    , write_idx_()
    , size_(size)
    , array_(new T[size]())
  {
    assert(size >= 2);
    assert(array_);
  }

  ~MessageQueue()
  {
    delete[] array_;
  }

  bool Push(const value_type& v)
  {
    size_t const current = write_idx_.load(std::memory_order_relaxed);
    size_t next = current + 1;
    if(next == size_)
    {
      next = 0;
    }

    if (next != read_idx_.load(std::memory_order_acquire))
    {
      array_[current] = v;
      write_idx_.store(next, std::memory_order_release);
      return true;
    }

    return false;
  }

  bool Pop(value_type& v)
  {
    size_t const current = read_idx_.load(std::memory_order_relaxed);
    if (current == write_idx_.load(std::memory_order_acquire))
    {
      return false;
    }

    size_t next = current + 1;
    if(next == size_)
    {
      next = 0;
    }
    v = array_[current];
    read_idx_.store(next, std::memory_order_release);
    return true;
  }
private:
  details::PaddedVolatileIndex  read_idx_;
  details::PaddedVolatileIndex  write_idx_;
  const size_t        size_;
  value_type          *array_;
};

}
#endif
