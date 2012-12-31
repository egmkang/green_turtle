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
// author: yywei

#ifndef __SOCKET_OPTION__
#define __SOCKET_OPTION__
#include <cstddef>
struct sockaddr;
struct iovec;

namespace green_turtle{
namespace net{

class SocketOption{
  public:
   static int   NewFD();
   static int   DestoryFD(int fd);
   static int   SetNoDelay(int fd);
   static int   SetNoBlock(int fd);
   static int   GetSendBuffer(int fd);
   static int   GetRecvBuffer(int fd);
   static int   SetSendBuffer(int fd, int size);
   static int   SetRecvBuffer(int fd, int size);
   static int   Listen(int fd, const struct sockaddr* addr, int len);
   static int   Accept(int fd, struct sockaddr_in* info);
   static int   Write(int fd, const void *data, size_t len);
   static int   Read(int fd, void *data, const size_t len);
   static int   Readv(int fd, iovec *iov, int count);
   static int   Connect(int fd, const struct sockaddr* addr, int len);
};
}
}
#endif
