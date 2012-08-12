#include <assert.h>
#include "event_loop.h"
#include "event_handler.h"
#include "poller.h"

using namespace green_turtle;
using namespace green_turtle::net;

EventLoop::EventLoop(int expected_size) : poller_(nullptr)
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
}
void EventLoop::RemoveEventHandler(EventHandler *pEventHandler)
{
  poller_->RemoveEventHandler(pEventHandler);
}
void EventLoop::Loop()
{
  assert(poller_);
  fired_handler_.clear();
  poller_->PollOnce(0,fired_handler_);
  for(EventHandler *handler : fired_handler_)
  {
    handler->HandleEvent();
  }
}
