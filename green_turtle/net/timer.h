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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <cstddef>
#include <stdint.h>

namespace green_turtle{
namespace net{

class TimerQueue;

class Timer
{
 public:
  friend class TimerQueue;
  Timer();
  virtual ~Timer();

 public:
  void          HandleTimeOut();
  inline size_t GetNextHandleTime() const { return next_handle_time_; }
  inline bool   IsInQueue()         const {return queue_; }
  inline size_t GetInterval()       const {return timer_interval_; }
 protected:
  virtual void  OnTimeOut(uint64_t current_time) = 0;
 private:
  Timer(const Timer&) {}
  Timer& operator = (const Timer&) { return *this;}
  //iter data here
  TimerQueue  *queue_;
  size_t      iter_slot_;
  size_t      iter_pos_;
  size_t      timer_interval_;
  //next handle time, milliseconds
  size_t      next_handle_time_;
};

}
}
#endif
