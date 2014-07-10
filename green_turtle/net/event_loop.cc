#include <thread>
#include <assert.h>
#include "event_loop.h"
#include "event_handler.h"
#include "timer_queue.h"
#include "poller.h"
#include <system.h>

using namespace green_turtle;
using namespace green_turtle::net;

static size_t FrameTime = 20;

EventLoop::EventLoop(int expected_size) :
    poller_(nullptr)
    ,terminal_(false)
    ,loop_index_(0)
    ,timer_queue_(nullptr)
{
  poller_ = Poller::CreatePoller(expected_size);
  assert(poller_);
}

EventLoop::~EventLoop()
{
  delete poller_;
}

void EventLoop::AddEventHandler(EventHandler *pEventHandler)
{
  poller_->AddEventHandler(pEventHandler);
  pEventHandler->set_event_loop(this);
}

void EventLoop::RemoveEventHandler(EventHandler *pEventHandler)
{
  poller_->RemoveEventHandler(pEventHandler);
}

void EventLoop::LazyInitTimerQueue()
{
  if(!timer_queue_)
  {
    timer_queue_ = new TimerQueue(2048, 16);
    timer_queue_->Update(System::GetMilliSeconds());
  }
}

void EventLoop::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,int64_t time_delay )
{
  LazyInitTimerQueue();
  timer_queue_->ScheduleTimer(timer_ptr, timer_interval, time_delay);
}

void EventLoop::CancelTimer(Timer *timer_ptr)
{
  LazyInitTimerQueue();
  timer_queue_->CancelTimer(timer_ptr);
}

void EventLoop::SetFrameTime(int milliSeconds)
{
  assert(milliSeconds > 10);
  FrameTime = milliSeconds;
}

void EventLoop::AddHandlerLater(EventHandler *pEventHandler)
{
  std::lock_guard<std::mutex> guard(this->mutex_);
  this->changed_handler_.push_back(std::make_pair(true, pEventHandler->shared_from_this()));
}
void EventLoop::RemoveHandlerLater(EventHandler *pEventHandler)
{
  std::lock_guard<std::mutex> guard(this->mutex_);
  this->changed_handler_.push_back(std::make_pair(false, pEventHandler->shared_from_this()));
}

void EventLoop::Loop()
{
  assert(poller_);
  while(!this->terminal_)
  {
    size_t begin_time = System::GetMilliSeconds();

    if(timer_queue_)
      timer_queue_->Update(begin_time);

    fired_handler_.clear();
    poller_->PollOnce(0,fired_handler_);
    for(auto handler : fired_handler_)
    {
      handler->HandleEvent();
    }

    size_t cost_time = System::GetMilliSeconds() - begin_time;
    if(cost_time < FrameTime)
    {
      System::Yield(FrameTime - cost_time);
    }

    std::deque<HandlerPair> temp_queue;
    {
      std::lock_guard<std::mutex> guard(this->mutex_);
      this->changed_handler_.swap(temp_queue);
    }
    for(auto& pair : temp_queue)
    {
      if(pair.first)
      {
        this->AddEventHandler(pair.second.get());
      }
      else
      {
        pair.second->OnError();
      }
    }
  }
}
