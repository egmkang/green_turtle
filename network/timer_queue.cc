#include "timer.h"
#include "timer_queue.h"
#include <assert.h>

using namespace green_turtle::network;

TimerQueue::TimerQueue(size_t slot_size,size_t interval):
    last_update_time_(0)
    ,interval_(interval)
    ,current_slot_(0)
    ,interval_exponent_(0)
{
  assert( ~(slot_size & (slot_size - 1)) && "Slot Size must be 2^n" );
  assert( ~(interval_ & (interval_ - 1)) && "Interval must be 2^n" );
  queues_.resize(slot_size);
  for(uint64_t i = interval_; i != 0; i /= 2)
  {
    ++interval_exponent_;
  }
}
void TimerQueue::CancelTimer(Timer *timer_ptr)
{
  if(!timer_ptr || !timer_ptr->queue_)
    return;
  queue_type& queue = timer_ptr->queue_->queues_;
  size_t      slot = timer_ptr->iter_slot_;
  size_t      pos = timer_ptr->iter_pos_;

  if(slot < queue.size())
  {
    list_type& list_ = queue[slot];
    list_.erase(pos);
    timer_ptr->queue_ = NULL;
  }
}
void TimerQueue::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,uint64_t time_delay)
{
  if(timer_ptr->IsInQueue())
    CancelTimer(timer_ptr);

  uint64_t  next_time = time_delay + timer_interval + last_update_time_;
  size_t    to_slot = current_slot_ + ((next_time + interval_ - 1) >> interval_exponent_);
  to_slot = to_slot & (queues_.size() - 1);
  list_type& list_ = queues_[to_slot];
  timer_ptr->iter_pos_ = list_.insert(timer_ptr);
  timer_ptr->iter_slot_ = to_slot;
  timer_ptr->queue_ = this;
}
void TimerQueue::Update(uint64_t current_time)
{
  size_t from_slot = current_slot_;
  size_t to_slot   = from_slot;
  if(!last_update_time_) last_update_time_ = current_time;
  //TODO:egmkan
  //[from_slot,to_slot)
  //for each queue and timer
}
