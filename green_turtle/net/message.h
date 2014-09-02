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
#include <memory>
namespace green_turtle {
namespace net {

class Message : green_turtle::NonCopyable,
                public std::enable_shared_from_this<Message> {
 public:
  virtual ~Message() {}
  virtual void* data() const = 0;
  virtual size_t length() const = 0;
  template <typename Y>
  std::shared_ptr<Y> cast() {
    return std::static_pointer_cast<Y>(this->shared_from_this());
  }
};

template <class T>
class MemberPtr {
 public:
  MemberPtr(Message* message, int offset) : offset_(offset) {
    message_ = message->shared_from_this();
  }
  T* get() const { return static_cast<T*>(message_->data()); }
  T* operator->() const { return get(); }
  T& operator*() const { return *get(); }

 private:
  std::shared_ptr<Message> message_;
  const int offset_;
};

// default Message impl using std::string
class MessageBuffer : public Message {
 public:
  enum {
    kDefaultInitSize = 64
  };

  MessageBuffer(int init_size = kDefaultInitSize) {
    buffer_.reserve(init_size);
  }

  template <typename T>
  MemberPtr<T> Append() {
    return MemberPtr<T>(this, MakeSpace(sizeof(T)));
  }
  template <typename T>
  void Append(const T& v) {
    int offset = MakeSpace(sizeof(v));
    T* ptr = static_cast<T*>(const_cast<char*>(this->buffer_.c_str() + offset));
    *ptr = v;
  }

  void Append(const char* str) { buffer_.append(str); }
  void Append(const void* data, int size) {
    buffer_.append(static_cast<const char*>(data), size);
  }
  virtual void* data() const {
    return static_cast<void*>(const_cast<char*>(this->buffer_.c_str()));
  }
  virtual size_t length() const { return this->buffer_.size(); }

 private:
  int MakeSpace(int size) {
    int offset = buffer_.size();
    buffer_.resize(buffer_.size() + size);
    return offset;
  }

 private:
  std::string buffer_;
};
}
}

#endif
