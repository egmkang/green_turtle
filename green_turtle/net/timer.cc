#include "timer.h"
#include "timer_queue.h"

using namespace green_turtle::net;

Timer::Timer()
    : queue_(NULL),
      iter_slot_(0),
      iter_pos_(0),
      timer_interval_(0),
      next_handle_time_(0) {}

Timer::~Timer() {
  if (IsInQueue()) {
    queue_->CancelTimer(this);
  }
}

void Timer::HandleTimeOut() {
  uint64_t current_time = next_handle_time_;
  next_handle_time_ += this->timer_interval_;
  this->OnTimeOut(current_time);
}

