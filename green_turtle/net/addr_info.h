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

#ifndef __ADDR_INFO__
#define __ADDR_INFO__
#include <assert.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace green_turtle{
namespace net{

struct AddrInfo
{
  AddrInfo()
  {
    addr_ = {0};
    addr_port_ = 0;
  }
  AddrInfo(const char *ip, const unsigned short port) : addr_str_(ip), addr_port_(port)
  {
    assert(ip);
    addr_.sin_family      = AF_INET;
    addr_.sin_port        = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip);
  }
  AddrInfo(const sockaddr_in& addr_) : addr_(addr_)
  {
    addr_port_ = ::ntohs(addr_.sin_port);
    addr_str_ = ::inet_ntoa(addr_.sin_addr);
  }
  std::string     addr_str_;
  unsigned short  addr_port_;
  sockaddr_in     addr_;
  struct sockaddr* sockaddr()
  {
    return static_cast<struct sockaddr*>(static_cast<void*>(&addr_));
  }
  int sockaddr_len()
  {
    return sizeof(addr_);
  }
};

}
}
#endif
