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

#ifndef __POLLER__
#define __POLLER__
#include <assert.h>
#include <vector>
#include <memory>
#include <noncopyable.h>

#ifdef __linux__
#define HAVE_EPOLL 1
#endif
#if (defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6)) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#define HAVE_KQUEUE 1
#endif

namespace green_turtle{
namespace net{

class EventHandler;

class Poller : green_turtle::NonCopyable
{
 public:
  Poller(int init_size);
  virtual ~Poller();
 public:
  virtual void AddEventHandler(EventHandler *pEventHandler)       = 0;
  virtual void RemoveEventHandler(EventHandler *pEventHandler)    = 0;
  virtual void PollOnce(int timeout,std::vector<EventHandler*>& fired_handlers) = 0;
 protected:
  void SetEventHandler(int fd,EventHandler *handler);
  inline EventHandler* GetEventHandler(int fd)
  {
    assert(fd <= (int)event_handlers_.size());
    return event_handlers_[fd].get();
  }
 public:
  static Poller* CreatePoller(int expected_size);
 private:
  std::vector<std::shared_ptr<EventHandler>> event_handlers_;
};

}
}

#endif
