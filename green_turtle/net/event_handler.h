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
// author: yyweii

#ifndef __EVENT_HANDLER__
#define __EVENT_HANDLER__
#include <vector>
#include <memory>
#include <noncopyable.h>

namespace green_turtle{
namespace net{

enum {
  kOK   = 0,
  kErr  = -1,
};

enum {
  kEventNone        = 0x0,
  kEventReadable    = 0x1,
  kEventWriteable   = 0x2,
};

class EventLoop;

class EventHandler : green_turtle::NonCopyable, public std::enable_shared_from_this<EventHandler>
{
 public:
  EventHandler(int fd);
  virtual ~EventHandler();
  int  fd() const { return fd_; }
  int  revents() const { return revents_; }
  void set_revents(int revents) { revents_ = revents; } 
  int  events() const { return events_;}
  void set_events(int events) { events_ = events; }
  int  index() const { return poll_idx_; } 
  void set_index(int idx) { poll_idx_ = idx; }
  void HandleEvent();
  EventLoop* event_loop() const { return event_loop_; }
  void set_event_loop(EventLoop *loop) { event_loop_ = loop; }
 public:
  void AddToConnManager();
  void SetWindowSize(int size);
  virtual void loop_balance(const std::vector<EventLoop*>& loops) { (void)loops;}
  virtual int OnError()           = 0;
  void Shutdown();
 protected:
  virtual int OnRead()            = 0;
  virtual int OnWrite()           = 0;
 private:
  int fd_;
  int events_;    //request events
  int revents_;   //returned events
  int poll_idx_;  //poll index,for fast remove
  EventLoop *event_loop_;
};

}
}
#endif

