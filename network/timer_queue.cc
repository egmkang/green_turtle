#include "timer.h"
#include "timer_queue.h"
#include <assert.h>

using namespace green_turtle::network;

TimerQueue::TimerQueue(size_t slot_size,size_t interval):interval_(interval)
{
  assert( ~(slot_size & (slot_size - 1)) && "Slot Size must be 2^n" );
  queues_.resize(slot_size);
}
void TimerQueue::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,uint64_t time_delay){}
void TimerQueue::CancelTimer(Timer *timer_ptr){}
void TimerQueue::Update(uint64_t current_time){}
