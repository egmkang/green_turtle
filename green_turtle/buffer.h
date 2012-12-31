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
// auhor: egmkang (egmkang@gmail.com)
#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <cassert>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <noncopyable.h>

namespace green_turtle{
/**
 * Simple Buffer support only Read/Write/Retrieve.
 * Not RingBuffer.
 */
class Buffer : NonCopyable
{
 public:
  /**
   * @param init_size The Buffer MaxSize, Capacity.
   */
  Buffer(int init_size)
      : array_(nullptr),
      write_(0),
      read_(0),
      size_(init_size)
  {
    array_ = static_cast<char*>(std::malloc(init_size));
  }
  ~Buffer()
  {
    std::free(array_);
  }
 public:
  /**
   * @param data Raw data will be appended into the buffer.
   * @param size Raw data's length.
   * @return appended size
   */
  size_t Append(const void *data, size_t size)
  {
    return Append(static_cast<const char*>(data),size);
  }
  /**
   * @param data Raw data will be appended into the buffer.
   * @param size Raw data's length.
   * @return appended size
   */
  size_t Append(const char *data, size_t size)
  {
    int need = size - WritableLength();
    if(need > 0)
    {
      array_ = static_cast<char*>(std::realloc(array_, this->size_ + need));
      assert(array_);
      this->size_ += need;
    }
    std::memcpy(array_ + write_, data, size);
    write_ += size;
    return size;
  }
  /**
   * the buffer's max size.
   * @return count in bytes
   */
  size_t Capacity() const { return size_; }
  /**
   * current max writable length.
   * @return count in bytes
   */
  size_t WritableLength() const { return size_ - write_; }
  /**
   * current max readable length.
   * @return count in bytes
   */
  size_t ReadableLength() const { return write_ - read_; }
  /**
   * current writable pointer.
   * @return current writable pointer.
   */
  char* BeginWrite() const { return array_ + write_; }
  /**
   * current reabable pointer.
   * @return current reabable pointer.
   */
  char* BeginRead() const { return array_ + read_; }
  /**
   * move the current writable pointer back
   * @param size count in bytes
   */
  void HasWritten(size_t size) { write_ += size; }
  /**
   * move the current readable pointer back
   * @param size count in bytes
   */
  void HasRead(size_t size) { read_ += size; }
  /**
   * try reset the writable/readable pointer.
   * don't clear data, just move all data toward to begin.
   * using HasRead to clear data.
   * @see HasRead
   */
  void Retrieve()
  {
    size_t len = ReadableLength();
    if(read_)
    {
      std::memmove(array_, BeginRead(), len);
    }
    read_ = 0;
    write_ = len;
  }
 private:
  char    *array_;
  size_t  write_;
  size_t  read_;
  size_t  size_;
};

}

#endif
