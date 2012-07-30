#include <net/timer.h>
#include <net/timer_queue.h>
#include <systime.h>
#include <iostream>
#include <unistd.h>

using namespace green_turtle::util;
using namespace green_turtle::network;

class TestTimer:public Timer
{
 public:
  TestTimer():begin_time_(0),count_(0)
  {
  }
 protected:
  void OnTimeOut(uint64_t current_time)
  {
    count_++;
    std::cout << this->GetInterval() 
        << "ms , count " << count_
        << " , Diff Time :  " << SysTime::GetMilliSeconds()-begin_time_ 
        << std::endl;

    begin_time_ = SysTime::GetMilliSeconds();
  }
 public:
  uint64_t begin_time_;
  uint64_t count_;
};

int main()
{
  SysTime::Update();
  uint64_t current_time = SysTime::GetMilliSeconds();

  TimerQueue queue(16,8);
  queue.Update(current_time);

  TestTimer *timer = new TestTimer;
  timer->begin_time_ = current_time;
  queue.ScheduleTimer(timer, 16);
  timer = new TestTimer;
  timer->begin_time_ = current_time;
  queue.ScheduleTimer(timer, 50);


  for(size_t idx = 0; idx < 100; ++idx)
  {
    SysTime::Update();
    uint64_t current_time = SysTime::GetMilliSeconds();
    queue.Update(current_time);
    ::usleep(4000);
  }
  uint64_t diff_time = SysTime::GetMilliSeconds() - current_time;

  std::cout << diff_time << std::endl;

  return 0;
}
