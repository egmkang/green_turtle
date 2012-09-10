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

#ifndef __TCP_ACCEPTOR__
#define __TCP_ACCEPTOR__
#include <vector>
#include "event_handler.h"

namespace green_turtle{
namespace net{

enum
{
  kAcceptorWindowRecvSize   = 128*1024,
  kAcceptorWindowSendSize   = 128*1024,
};

struct AddrInfo;

class TcpAcceptor : public EventHandler
{
  public:
    TcpAcceptor(const AddrInfo& addr, int rev_buf = kAcceptorWindowRecvSize, int snd_buf = kAcceptorWindowSendSize);
    virtual ~TcpAcceptor();
  public:
    bool Listen();
  protected:
    virtual int OnRead();
    virtual int OnWrite();
    virtual int OnError();
    virtual void OnAddedIntoEventLoop(EventLoop *loop);
    EventHandler* CreateNewHandler(const AddrInfo& info, int fd);
  private:
    int Accept(AddrInfo& info);
    AddrInfo                *addr_;
    std::vector<EventLoop*> loops_;
    int                     idx_; //load balance,not thread safe,so what!
};

}
}

#endif

