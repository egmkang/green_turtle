#include <thread>
#include <assert.h>
#include "event_loop.h"
#include "event_handler.h"
#include "poller.h"
#include <system.h>

using namespace green_turtle;
using namespace green_turtle::net;

static size_t FrameTime = 20;

EventLoop::EventLoop(int expected_size) :
    poller_(nullptr)
    ,terminal_(false)
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

void EventLoop::SetFrameTime(int milliSeconds)
{
  assert(milliSeconds > 10);
  FrameTime = milliSeconds;
}

void EventLoop::AddHandlerLater(EventHandler *pEventHandler)
{
  std::lock_guard<std::mutex> guard(this->add_mutex_);
  this->add_handler_.push_back(pEventHandler);
}
void EventLoop::RemoveHandlerLater(EventHandler *pEventHandler)
{
  std::lock_guard<std::mutex> guard(this->remove_mutex_);
  this->remove_handler_.push_back(pEventHandler);
}

void EventLoop::Loop()
{
  assert(poller_);
  while(!this->terminal_)
  {
    System::UpdateTime();
    size_t begin_time = System::GetMilliSeconds();

    fired_handler_.clear();
    poller_->PollOnce(0,fired_handler_);
    for(auto handler : fired_handler_)
    {
      handler->HandleEvent();
    }

    System::UpdateTime();
    size_t cost_time = System::GetMilliSeconds() - begin_time;
    if(cost_time < FrameTime)
    {
      System::Yield(FrameTime - cost_time);
    }

    std::deque<EventHandler*> temp_add_queue;
    {
      std::lock_guard<std::mutex> guard(this->remove_mutex_);
      this->add_handler_.swap(temp_add_queue);
    }
    for(auto handler : temp_add_queue)
    {
      this->AddEventHandler(handler);
    }

    std::deque<EventHandler*> temp_remove_queue;
    {
      std::lock_guard<std::mutex> guard(this->remove_mutex_);
      this->remove_handler_.swap(temp_remove_queue);
    }
    for(auto handler : temp_remove_queue)
    {
      handler->OnError();
    }
  }
  assert(false);
}
