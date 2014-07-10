#include <net/timer.h>
#include <net/timer_queue.h>
#include <system.h>
#include <iostream>
#include <unistd.h>

using namespace green_turtle;
using namespace green_turtle::net;

class TestTimer:public Timer
{
 public:
  TestTimer():begin_time_(0),count_(0)
  {
  }
 protected:
  void OnTimeOut(uint64_t current_time)
  {
    (void)current_time;
    count_++;
    std::cout << this->GetInterval() 
        << "ms , count " << count_
        << " , Diff Time :  " << System::GetMilliSeconds()-begin_time_ 
        << std::endl;

    begin_time_ = System::GetMilliSeconds();
  }
 public:
  uint64_t begin_time_;
  uint64_t count_;
};

int main()
{
  uint64_t current_time = System::GetMilliSeconds();

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
    uint64_t current_time = System::GetMilliSeconds();
    queue.Update(current_time);
    ::usleep(4000);
  }
  uint64_t diff_time = System::GetMilliSeconds() - current_time;

  std::cout << diff_time << std::endl;

  return 0;
}
