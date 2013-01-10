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

#ifndef __NET_MESSAGE__
#define __NET_MESSAGE__
#include <noncopyable.h>
#include <cstddef>
#include <string>
namespace green_turtle{
namespace net{

class Message : green_turtle::NonCopyable {
 public:
  virtual ~Message() {}
  virtual void*   data()    const = 0;
  virtual size_t  length()  const = 0;
};

//default implation using std::string
class DefaultMessageBuffer : public Message
{
  enum{ kDefaultInitSize = 64 };
 public:
  DefaultMessageBuffer() : current_pos_(0)
  {
    buffer_.reserve(kDefaultInitSize);
  }

  template<typename T>
  T* CurrentPtr()
  {
    return ConvertToPtr<T>(current_pos_);
  }

  template<typename T>
  T* ConvertToPtr(int offset)
  {
    char *p = const_cast<char*>(buffer_.data() + offset);
    return static_cast<T*>(p);
  }

  template<typename T>
  T* Append()
  {
    MakeSpace(sizeof(T));
    T *ptr = ConvertToPtr<T>();
    current_pos_ += sizeof(T);
    return ptr;
  }

  size_t CurrentPos() const { return current_pos_; }
 public:
  virtual void* data() const
  {
    return static_cast<void*>(const_cast<char*>(buffer_.data()));
  }

  virtual size_t length() const
  {
    return buffer_.size();
  }
 private:
  void MakeSpace(int s) { buffer_.resize(buffer_.size() + s); }
 private:
  std::string buffer_;
  int current_pos_;
};

} //namespace net
} //namespace green_turtle

#endif
