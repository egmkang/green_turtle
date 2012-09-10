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
#include "event_handler.h"

namespace green_turtle{

template<class T>
class RingBuffer;

namespace net{

struct AddrInfo;

class BufferedSocket : public EventHandler
{
 public:
  typedef green_turtle::RingBuffer<unsigned char> CacheLine;
 public:
  BufferedSocket(int fd,const AddrInfo& addr);
  ~BufferedSocket();
  //data must in heap,cannot be an object on stack
  void SendMessage(const void *data, size_t len);
  const AddrInfo& addr() const;
  CacheLine* GetNewCacheLine();
 protected:
  virtual int OnRead();
  virtual int OnWrite();
  virtual int OnError();
  virtual void ProcessInputData(CacheLine& data) = 0;;
  virtual void ProcessOutputMessage(const void *data, unsigned int len) = 0;;
  virtual void ProcessDeleteSelf() = 0;
 private:
  typedef std::pair<const void*, unsigned int> RawData;
  std::unique_ptr<AddrInfo>   addr_;
  const size_t                cache_line_size_;
  std::deque<CacheLine*>      snd_queue_;
  std::deque<RawData>         snd_raw_data_queue;
  std::unique_ptr<CacheLine>  rcv_buffer_;
  std::mutex                  write_lock_;
};

}
}
#endif
