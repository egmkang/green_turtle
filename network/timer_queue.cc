#include "timer.h"
#include "timer_queue.h"
#include <assert.h>

using namespace green_turtle::network;

TimerQueue::TimerQueue(size_t slot_size,size_t interval):
    last_update_time_(0)
    ,interval_(interval)
    ,current_slot_(0)
{
  assert( ~(slot_size & (slot_size - 1)) && "Slot Size must be 2^n" );
  queues_.resize(slot_size);
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
  }
}
void TimerQueue::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,uint64_t time_delay)
{
  //TODO:egmkang
}
void TimerQueue::Update(uint64_t current_time)
{
  //TODO:egmkang
}
