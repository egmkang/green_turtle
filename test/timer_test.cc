#include "../network/timer.h"
#include "../network/timer_queue.h"
#include "../util/systime.h"
#include <iostream>
#include <unistd.h>

using namespace green_turtle::util;
using namespace green_turtle::network;

class TestTimer:public Timer
{
 protected:
  void OnTimeOut(uint64_t current_time)
  {
    std::cout 
        << "Internal : " << this->GetInterval()
        << "ms , Tick Time : " << current_time
        << " , Real Time : " << SysTime::GetMilliSeconds() 
        << std::endl;
  }
};

int main()
{
  SysTime::Update();
  uint64_t current_time = SysTime::GetMilliSeconds();

  TimerQueue queue(16,10);
  queue.Update(current_time);

  TestTimer *timer = new TestTimer;
  queue.ScheduleTimer(timer, 10);
  timer = new TestTimer;
  queue.ScheduleTimer(timer, 12);


  for(size_t idx = 0; idx < 100; ++idx)
  {
    SysTime::Update();
    uint64_t current_time = SysTime::GetMilliSeconds();
    queue.Update(current_time);
    ::usleep(4000);
    std::cout << "RealTime " << SysTime::GetMilliSeconds()
        << " * "<< idx << std::endl;
  }
  return 0;
}
