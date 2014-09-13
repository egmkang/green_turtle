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

#ifndef __SYSTEM_TIME_H__
#define __SYSTEM_TIME_H__
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <cstddef>
#include <stdint.h>
#include <algorithm>

namespace green_turtle {
class System {
 public:
  static time_t GetSeconds() {
    return time(NULL);
  }

  static uint64_t GetMilliSeconds() {
    uint64_t time_millisecond;
    timeval time_val;
    gettimeofday(&time_val, NULL);
    time_millisecond = time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
    return time_millisecond;
  }

  // first is seconds
  // second is milliseconds
  static std::pair<time_t, time_t> GetCurrentTime() {
    timeval time_val;
    gettimeofday(&time_val, NULL);
    return std::make_pair(time_val.tv_sec, time_val.tv_usec / 1000);
  }

  // local time
  static tm GetTime() {
    tm time_tm;
    time_t time_sec = GetCurrentTime().first;
    localtime_r(&time_sec, &time_tm);
    return time_tm;
  }

  // if s1 and s2 is the same day,than return 0
  // else return s2.days - s1.days
  // days are defined in local time
  static int GetSecondsDiffDays(time_t s1, time_t s2) {
    tm tm_1;
    tm tm_2;
    localtime_r(&s1, &tm_1);
    localtime_r(&s2, &tm_2);
    tm_1.tm_sec = tm_1.tm_min = tm_1.tm_hour = 0;
    tm_2.tm_sec = tm_2.tm_min = tm_2.tm_hour = 0;
    s1 = mktime(&tm_1);
    s2 = mktime(&tm_2);
    return (s2 - s1) / (3600 * 24);
  }

  // sleep for a while
  static void Yield(uint64_t milliSeconds) { ::usleep(milliSeconds * 1000); }

  // get thread id
  static int32_t GetThreadID() {
    static __thread int32_t tid = syscall(SYS_gettid);
    return tid;
  }
};
};

#endif
