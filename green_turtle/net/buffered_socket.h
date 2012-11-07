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

#ifndef __BUFFERED_SOCKET__
#define __BUFFERED_SOCKET__
#include <cstddef>
#include <deque>
#include <mutex>
#include <memory>
#include <buffer.h>
#include "addr_info.h"
#include "event_handler.h"
#include "message.h"

namespace green_turtle{
namespace net{

class BufferedSocket : public EventHandler
{
 public:
  typedef green_turtle::Buffer  CacheLine;
 public:
  BufferedSocket(int fd,const AddrInfo& addr, int rcv_window = 0, int snd_window = 0);
  ~BufferedSocket();
  void SendMessage(std::shared_ptr<Message>&& data);
  void SendMessage(std::shared_ptr<Message>& data);
  const AddrInfo& addr() const;
  CacheLine* GetNewCacheLine();
 protected:
  virtual int OnRead();
  virtual int OnWrite();
  virtual int OnError();
  virtual void Decoding(CacheLine& data) = 0;
  virtual void DeleteSelf() = 0;
 private:
  bool HasData() const;
 private:
  typedef std::shared_ptr<Message>  SharedMessage;
  AddrInfo                    addr_;
  const size_t                cache_line_size_;
  std::deque<CacheLine*>      snd_queue_;
  std::deque<SharedMessage>   snd_messages_;
  CacheLine                   *rcv_buffer_;
  std::mutex                  write_lock_;
};

}
}
#endif
