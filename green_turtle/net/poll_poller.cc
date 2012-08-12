#include "poll_poller.h"
#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

static const int kInitEventSize = 16;

PollPoller::PollPoller():
    Poller(kInitEventSize)
    ,this->pollfds_(kInitEventSize)
{
}
PollPoller::~PollPoller()
{
}
void PollPoller::AddEventHandler(EventHandler *event_handler)
{
  this->SetEventHandler(event_handler->fd(), event_handler);
  pollfds_.push_back(event_handler->fd());
  //TODO:egmkang
  //check
}
void PollPoller::RemoveEventHandler(EventHandler *event_handler)
{
  this->SetEventHandler(event_handler->fd(), nullptr);
  //TODO:egmkang
  //remove  here
}
void PollPoller::PollOnce(int timeout,std::vector<EventHandler*>& fired_handler)
{
  //TODO:egmkang
  //
}
