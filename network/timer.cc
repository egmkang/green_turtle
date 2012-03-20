#include "timer.h"

using namespace green_turtle::network;
using namespace green_turtle::collections;

Timer::Timer()
  :queue_(NULL)
  ,iter_slot_(0)
  ,iter_pos_(0)
  ,timer_interval_(0)
  ,next_handle_time_(0)
{
}
Timer::~Timer()
{
  if(IsInQueue())
  {
    timer_slot_->erase(iter_pos_);
    timer_slot_ = NULL;
  }
}

void Timer::HandleTimeOut()
{
  next_handle_time_ += this->timer_interval_;
  this->OnTimeOut();
}

