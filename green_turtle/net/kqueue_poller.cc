#include <assert.h>
#include <poll.h>
#include <unistd.h>
#include "kqueue_poller.h"
#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

static const int kInitKqueueSize = 16;

KqueuePoller::KqueuePoller():
    Poller(kInitKqueueSize)
    ,kqfd_(::kqueue())
    ,events_(16)
{
  assert(kqfd_ != -1);
}

KqueuePoller::~KqueuePoller()
{
  ::close(kqfd_);
}

void KqueuePoller::AddEventHandler(EventHandler *event_handler)
{
  this->SetEventHandler(event_handler->fd(), event_handler);
  if(event_handler->fd() >= this->events_.size())
  {
    events_.resize(events_.size() * 2);
  }

  struct kevent ke;

  if(event_handler->events() & kEventReadable)
  {
    EV_SET(&ke, event_handler->fd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if(kevent(kqfd_, &ke, 1, NULL, 0, NULL) == -1)
    {
      RemoveEventHandler(event_handler);
    }
  }
  if(event_handler->events() & kEventWriteable)
  {
    EV_SET(&ke, event_handler->fd(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if(kevent(kqfd_, &ke, 1, NULL, 0, NULL) == -1)
    {
      RemoveEventHandler(event_handler);
    }
  }
}

void KqueuePoller::RemoveEventHandler(EventHandler *event_handler)
{
  this->SetEventHandler(event_handler->fd(), nullptr);

  struct kevent ke;
  if(event_handler->events() & kEventReadable)
  {
    EV_SET(&ke, event_handler->fd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kqfd_, &ke, 1, NULL, 0, NULL);
  }
  if(event_handler->events() & kEventWriteable)
  {
    EV_SET(&ke, event_handler->fd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(kqfd_, &ke, 1, NULL, 0, NULL);
  }
}

void KqueuePoller::PollOnce(int timeout,std::vector<EventHandler*>& fired_handler)
{
  int num = ::kevent(kqfd_, NULL, 0, &*events_.begin(), (int)events_.size(), NULL);
  for(int idx = 0; idx < num; ++idx)
  {
    const auto& e = events_[idx];
    EventHandler *handle = this->GetEventHandler(e.ident);
    assert(handle);

    int mark = kEventNone;
    if(e.filter & EVFILT_READ)  mark |= kEventReadable;
    if(e.filter & EVFILT_WRITE) mark |= kEventWriteable;
    handle->set_revents(mark);

    fired_handler.push_back(handle);
  }
}

