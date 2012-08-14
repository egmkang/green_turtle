#include <algorithm>
#include "poll_poller.h"
#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

static const int kInitEventSize = 16;

PollPoller::PollPoller():
    Poller(kInitEventSize)
    ,pollfds_(kInitEventSize)
    ,polling_(false)
{
}

PollPoller::~PollPoller()
{
}

void PollPoller::AddEventHandler(EventHandler *event_handler)
{
  assert(!polling_);
  this->SetEventHandler(event_handler->fd(), event_handler);

  struct pollfd pfd = {0};
  pfd.events = event_handler->events();
  pfd.fd = event_handler->fd();
  pollfds_.push_back(pfd);

  event_handler->set_index(pollfds_.size() - 1);
}

void PollPoller::RemoveEventHandler(EventHandler *event_handler)
{
  assert(!polling_);
  this->SetEventHandler(event_handler->fd(), nullptr);
  int idx = event_handler->index();
  assert(idx > 0);
  if(idx != pollfds_.size() - 1)
  {
    std::iter_swap(pollfds_.begin() + idx, --pollfds_.end());
    GetEventHandler(pollfds_[idx].fd)->set_index(idx);
  }
  pollfds_.pop_back();
  event_handler->set_index(-1);
}

void PollPoller::PollOnce(int timeout,std::vector<EventHandler*>& fired_handler)
{
  polling_ = true;
  int num = ::poll(&*pollfds_.begin(), pollfds_.size(), 0);
  for(int i = 0; i < num; ++i)
  {
    const struct pollfd& e = pollfds_[i];

    EventHandler *handle = GetEventHandler(e.fd);
    int mark = kEventNone;
    if(e.events & POLLIN)   mark |= kEventReadable;
    if(e.events & POLLOUT)  mark |= kEventWriteable;
    if(e.events & POLLERR)  mark |= kEventWriteable;
    if(e.events & POLLHUP)  mark |= kEventWriteable;
    handle->set_revents(mark);

    fired_handler.push_back(handle);
  }
  polling_ = false;
}

