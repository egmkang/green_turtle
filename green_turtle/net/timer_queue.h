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
// auhor: egmkang (egmkang@gmail.com)

#ifndef __TIMER_QUEUE_H__
#define __TIMER_QUEUE_H__
#include <stdint.h>
#include <vector>
#include <ordered_list.h>
#include <noncopyable.h>

namespace green_turtle{
namespace net{

class Timer;

//Hash Timer Queues
class TimerQueue : green_turtle::NonCopyable
{
 public:
  TimerQueue(size_t slot_size,size_t interval);
  ~TimerQueue();

  //register a timer,unit ms
  void ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,int64_t time_delay = 0);
  //unregister a timer
  void CancelTimer(Timer *timer_ptr);
  //timers loop
  void Update(uint64_t current_time);
  inline uint64_t GetLastUpdateTime() const { return last_update_time_; }
 private:
  typedef green_turtle::ordered_list<Timer*> list_type;

  list_type     *queues_;
  const size_t  slot_size;
  uint64_t      last_update_time_;
  const size_t  interval_;  //must be 2^n ms
  size_t        current_slot_;
  uint8_t       interval_exponent_;//2^interval_exponent_ = interval_
  uint64_t      circle_time_;
};

};
};

#endif
