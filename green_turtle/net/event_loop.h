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

#ifndef __event_loop__
#define __event_loop__
#include <vector>
#include <thread>
#include <deque>
#include "event_handler.h"
#include <noncopyable.h>

namespace green_turtle{
namespace net{

class Poller;

class EventLoop : NonCopyable
{
 public:
  EventLoop(int expected_size);
  ~EventLoop();
  void Ternimal() { terminal_ = true; }
  void AddEventHandler(EventHandler *pEventHandler);
  void RemoveEventHandler(EventHandler *pEventHandler);
  void Loop();
  static void SetFrameTime(int milliSeconds);
 public:
  void AddHandlerLater(EventHandler *pEventHandler);
  void RemoveHandlerLater(EventHandler *pEventHandler);
 private:
  Poller  *poller_;
  bool    terminal_;
  std::vector<EventHandler*>  fired_handler_;

  std::mutex                  add_mutex_;
  std::deque<EventHandler*>   add_handler_;
  std::mutex                  remove_mutex_;
  std::deque<EventHandler*>   remove_handler_;
};

}
}
#endif
