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

#ifndef __tcp__server__
#define __tcp__server__
#include <vector>
#include <thread>
#include <functional>
#include <noncopyable.h>
#include "timer.h"

namespace green_turtle{
namespace net{

class EventHandler;
class EventLoop;
class TcpAcceptor;
class TcpClient;
class TimerQueue;
class TcpServer;

class TcpServer : green_turtle::NonCopyable {
 public:
  //default using epoll or kequeue
  //if your want to use poll, please set this number small than 128
  TcpServer(int expected_size = 1024);
  ~TcpServer();
 public:
  void AddAcceptor(TcpAcceptor *acceptor);
  void AddClient(TcpClient *client);
  void SetThreadCount(int count);
  void Run();
  void Terminal();

 public:
  //register a timer,unit ms
  void ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,int64_t time_delay = 0);
  //unregister a timer
  void CancelTimer(Timer *timer_ptr);
  void SetLoopCallBack(std::function<void(void)> loop) { loop_once_ = loop; }
 private:
  void InitEventLoop();
  void InitThreads();
 private:
  typedef std::shared_ptr<EventHandler> SharedHandler;
  std::vector<EventLoop*>     loops_;
  std::vector<std::thread*>   threads_;
  std::vector<SharedHandler>  handlers_;
  std::unique_ptr<TimerQueue> timer_queue_;
  bool  is_terminal_;
  int   thread_count_;
  int   expected_size_;
  std::function<void(void)> loop_once_;
};

}
}
#endif
