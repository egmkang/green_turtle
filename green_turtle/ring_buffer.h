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

#ifndef __RING_BUFFER__
#define __RING_BUFFER__
#include <assert.h>
#include <cstring>
#include <algorithm>
#include <cstddef>
#include <noncopyable.h>

namespace green_turtle{

namespace details{
inline unsigned int nextpow2(unsigned int x)
{
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x+1;
}
}

template<class T>
class RingBuffer : NonCopyable{
 public:
  RingBuffer(size_t buffer_size_);
  ~RingBuffer();

  size_t  Read(T* dest,size_t count);
  size_t  Write(const T* src,size_t count);
  void    Reset();
  void    SkipRead(int offset);
  void    SkipWrite(int offset);
  T*      GetBegin() const;
  T*      GetEnd() const;
  size_t  GetSize() const;
  size_t  GetTailSpace() const;
  size_t  GetCapacity() const;
 private:
  size_t          write_;
  size_t          read_;
  size_t          index_mark_;
  T               *array_;
};
template<class T>
RingBuffer<T>::RingBuffer(size_t buffer_size_):
  write_(0)
  ,read_(0)
  ,index_mark_(0)
{
  assert(buffer_size_);
  buffer_size_ = details::nextpow2(buffer_size_);
  index_mark_ = buffer_size_ - 1;
  array_ = new  T[buffer_size_];
}
template<class T>
RingBuffer<T>::~RingBuffer()
{
  delete[] array_;
}
template<class T>
size_t  RingBuffer<T>::Read(T* dest,size_t count)
{
  assert(dest && count);
  assert(read_ <= write_);
  size_t size_      = GetSize();
  if(count > size_)
    count = size_;

  size_t from = read_ & index_mark_;
  size_t to   = (read_ + count) & index_mark_;
  if(from <= to)
  {
    std::memcpy(dest, &array_[from], count * sizeof(T));
  }
  else
  {
    size_t first_read_ = GetCapacity() - from;
    std::memcpy(dest, &array_[from], first_read_ * sizeof(T));
    std::memcpy(dest+first_read_, &array_[0], to * sizeof(T));
  }
  read_ += count;
  return count;
}
template<class T>
size_t  RingBuffer<T>::Write(const T* src,size_t count)
{
  assert(src && count);
  assert(read_ <= write_);
  size_t space_ = GetCapacity() - GetSize();
  if(count > space_)
    count = space_;
  size_t from = write_ & index_mark_;
  size_t to   = (write_ + count) & index_mark_;
  if(from <= to)
  {
    std::memcpy(&array_[from],src,count * sizeof(T));
  }
  else
  {
    size_t first_write_ = index_mark_ + 1 - from;
    std::memcpy(&array_[from], src, first_write_ * sizeof(T));
    std::memcpy(&array_[0], src + first_write_, to * sizeof(T));
  }
  write_ += count;
  return count;
}
template<class T>
void    RingBuffer<T>::Reset()
{
  const size_t size_ = GetSize();
  //0--------read--------write------end
  if((read_ & index_mark_) <= (write_ & index_mark_))
  {
    std::memmove(&array_[0], GetBegin(), size_ * sizeof(T));
  }
  else
  //0--------write-------read-------end
  {
    T array_tmp[write_];
    std::memset(array_tmp,0,sizeof(array_tmp));
    std::memcpy(&array_tmp[0],&array_[0],sizeof(array_tmp));
    size_t read_index = index_mark_ + 1 - (read_ & index_mark_);
    std::memcpy(&array_[0],GetBegin(), read_index * sizeof(T));
    std::memcpy(&array_[read_index],&array_tmp[0],sizeof(array_tmp));
  }
  write_ = size_;
  read_ = 0;
}
template<class T>
void    RingBuffer<T>::SkipRead(int offset)
{
  assert( read_ + offset > 0 && read_ + offset <= write_);
  read_ += offset;
}
template<class T>
void    RingBuffer<T>::SkipWrite(int offset)
{
  assert(write_ + offset > 0 && write_ + offset >= read_);
  write_ += offset;
}
template<class T>
T*      RingBuffer<T>::GetBegin() const
{
  return & array_[ read_ & index_mark_ ];
}
template<class T>
T*      RingBuffer<T>::GetEnd() const
{
  return & array_[ write_ & index_mark_ ];
}
template<class T>
size_t  RingBuffer<T>::GetSize() const
{
  assert(read_ <= write_);
  return write_ - read_;
}
template<class T>
size_t  RingBuffer<T>::GetTailSpace() const
{
  return index_mark_ - (write_ & index_mark_);
}
template<class T>
size_t  RingBuffer<T>::GetCapacity() const
{
  return index_mark_ + 1;
}

}
#endif
