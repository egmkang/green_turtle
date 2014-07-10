#include "timer.h"
#include "timer_queue.h"
#include <assert.h>

using green_turtle::ordered_list;
using namespace green_turtle::net;

TimerQueue::TimerQueue(size_t slot_size,size_t interval):
    slot_size(slot_size)
    ,last_update_time_(0)
    ,interval_(interval)
    ,current_slot_(0)
    ,interval_exponent_(0)
    ,circle_time_(0)
{
  assert( ~(slot_size & (slot_size - 1)) && "Slot Size must be 2^n" );
  assert( ~(interval_ & (interval_ - 1)) && "Interval must be 2^n" );
  queues_ = new list_type[slot_size]();
  for(size_t idx = 0; idx < slot_size; ++idx)
    queues_[idx].set_deleted((Timer*)(void*)(-1l));
  for(uint64_t i = interval_; i != 1; i /= 2)
  {
    ++interval_exponent_;
  }
  circle_time_ = slot_size * interval_;
}

TimerQueue::~TimerQueue()
{
  delete[] queues_;
}

void TimerQueue::CancelTimer(Timer *timer_ptr)
{
  if(!timer_ptr || !timer_ptr->queue_)
    return;
  list_type   *queue = timer_ptr->queue_->queues_;
  size_t      slot = timer_ptr->iter_slot_;
  size_t      pos = timer_ptr->iter_pos_;

  if(slot < slot_size)
  {
    list_type& list_ = queue[slot];
    list_.erase(pos);
    timer_ptr->queue_ = NULL;
  }
}
void TimerQueue::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,int64_t time_delay)
{
  uint64_t  next_handle_time = time_delay + timer_interval + last_update_time_;

  if(timer_ptr->IsInQueue())
  {
    CancelTimer(timer_ptr);
  }

  size_t    slot_mask = slot_size - 1;
  size_t    to_slot = current_slot_
                      + ((time_delay + circle_time_ + timer_interval) >> interval_exponent_);

  to_slot   = to_slot & slot_mask;

  list_type& list_            = queues_[to_slot];
  timer_ptr->iter_pos_        = list_.insert(timer_ptr);
  timer_ptr->iter_slot_       = to_slot;
  timer_ptr->queue_           = this;
  timer_ptr->timer_interval_  = timer_interval;
  timer_ptr->next_handle_time_= next_handle_time;
}

void TimerQueue::Update(uint64_t current_time)
{
  if(!last_update_time_)
    last_update_time_ = current_time;

  uint64_t delta_time = current_time + this->interval_;
  size_t   slot_mask = slot_size - 1;
  while(last_update_time_ < delta_time)
  {
    list_type& list_ = queues_[current_slot_];

    current_slot_ = (current_slot_ + 1) & slot_mask;
    last_update_time_ += interval_;

    //lambda expression
    list_.for_each([&](Timer* timer,size_t iter) -> bool
    {
      if(timer)
      {
        while(timer && timer->GetNextHandleTime() <= last_update_time_)
        {
          timer->HandleTimeOut();
          timer = list_.get(iter);
        }
        if(timer)
        {
          int64_t  delay = timer->GetNextHandleTime() - (timer->GetInterval() + this->GetLastUpdateTime());
          this->ScheduleTimer(timer,timer->GetInterval(),delay);
        }
      }
      else
      {
        list_.erase(iter);
      }
      return true;
    });
  }
}
