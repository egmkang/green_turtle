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
#include <stdint.h>
#include <atomic>
#include <noncopyable.h>

namespace green_turtle{

namespace internal {

struct AtomicIndex {
  AtomicIndex(uint64_t init = 0) : value_(init) {}
  inline uint64_t load(std::memory_order memory_order)
  {
    return value_.load(memory_order);
  }
  inline void store(uint64_t value, std::memory_order memory_order)
  {
    value_.store(value, memory_order);
  }

  std::atomic<uint64_t> value_;
};

struct VolatileIndex
{
  VolatileIndex(uint64_t init = 0) : value_(init) {}
  inline uint64_t load(std::memory_order memory_order)
  {
    (void)memory_order;
    return value_;
  }
  inline void store(uint64_t value, std::memory_order memory_order)
  {
    (void)memory_order;
    value_ = value;
  }

  volatile uint64_t value_;
};
}

struct PaddedAtomicIndex : public internal::AtomicIndex {
  uint64_t padding_values_[7];
};

struct PaddedVolatileIndex : public internal::VolatileIndex {
  uint64_t padding_values_[7];
};

//support POD data only
//1:1 MessageQueue
//N:1 equals N*(1:1)
template<class T, class Counter = PaddedAtomicIndex>
class MessageQueue : NonCopyable
{
 public:
  typedef T value_type;
 public:
  MessageQueue(uint64_t size = 128*1024)
    : read_idx_()
    , write_idx_()
    , size_(size)
    , mask_(size - 1)
    , array_(new T[size]())
  {
    assert(size >= 2);
    assert(!(size_ & (size_ -1)) && "size must be 2^n!");
    assert(array_);
  }

  ~MessageQueue()
  {
    delete[] array_;
  }

  bool Push(const value_type& v)
  {
    const auto current_write_ = write_idx_.load(std::memory_order_relaxed);
    const auto current_read_  = read_idx_.load(std::memory_order_acquire);

    if(current_write_ - current_read_ == mask_)
      return false;

    array_[current_write_ & mask_] = v;
    write_idx_.store(current_write_ + 1, std::memory_order_release);

    return true;
  }

  bool Pop(value_type& v)
  {
    const auto current_write_ = write_idx_.load(std::memory_order_acquire);
    const auto current_read_  = read_idx_.load(std::memory_order_relaxed);
    if(current_read_ == current_write_)
      return false;

    v = array_[current_read_ & mask_];
    read_idx_.store(current_read_ + 1, std::memory_order_release);
    return true;
  }
  uint64_t Size() const
  {
    const auto current_read_  = read_idx_.load(std::memory_order_relaxed);
    const auto current_write_ = write_idx_.load(std::memory_order_relaxed);
    return (current_write_ - current_read_);
  }
  uint64_t Capacity() const
  {
    return size_;
  }
private:
  Counter         read_idx_;
  Counter         write_idx_;
  const uint64_t  size_;
  const uint64_t  mask_;
  value_type      *array_;
};

}
#endif
