#include <assert.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>
#include "epoll_poller.h"
#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

static const int kInitEpollSize = 16;

static int Epoll_Ctl(int epollfd,int operation,int fd,int events);

EpollPoller::EpollPoller():
    Poller(kInitEpollSize)
    ,epollfd_(::epoll_create(kInitEpollSize))
    ,events_(16)
{
}

EpollPoller::~EpollPoller()
{
  ::close(epollfd_);
}

void EpollPoller::AddEventHandler(EventHandler *event_handler)
{
  assert(!polling_);
  this->SetEventHandler(event_handler->fd(), event_handler);
  if(event_handler->fd() >= (int)this->events_.size())
  {
    events_.resize(events_.size() * 2);
  }
  if(Epoll_Ctl(epollfd_, EPOLL_CTL_ADD, event_handler->fd(), event_handler->events()) < 0)
  {
    this->SetEventHandler(event_handler->fd(), nullptr);
  }
}

void EpollPoller::RemoveEventHandler(EventHandler *event_handler)
{
  assert(!polling_);
  this->SetEventHandler(event_handler->fd(), nullptr);
#ifndef LAZY
  if(Epoll_Ctl(epollfd_, EPOLL_CTL_DEL, event_handler->fd(), event_handler->events()) < 0)
  {
  }
#endif
}

void EpollPoller::PollOnce(int timeout,std::vector<EventHandler*>& fired_handler)
{
  polling_ = true;
  int num = ::epoll_wait(epollfd_, &events_[0], (int)events_.size(), timeout);
  for(int idx = 0; idx < num; ++idx)
  {
    const struct epoll_event& e = events_[idx];
    EventHandler *handle = this->GetEventHandler(e.data.fd);
    if(!handle) continue;

    int mask = kEventNone;
    if(e.events & EPOLLIN)  mask |= kEventReadable;
    if(e.events & EPOLLOUT) mask |= kEventWriteable;
    if(e.events & EPOLLERR) mask |= kEventWriteable;
    if(e.events & EPOLLHUP) mask |= kEventWriteable;
    handle->set_revents(mask);

    fired_handler.push_back(handle);
  }
  polling_ = false;
}

int Epoll_Ctl(int epollfd, int operation, int fd, int events)
{
  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = fd;

  if(events & kEventReadable)   event.events |= EPOLLIN;
  if(events & kEventWriteable)  event.events |= EPOLLOUT;

  int ret = ::epoll_ctl(epollfd, operation, fd, &event);
  return ret;
}
