#include <assert.h>
#include <poll.h>
#include <unistd.h>
#include "epoll_poller.h"
#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

static const int kInitEpollSize = 16;

static int Epoll_Ctl(int epollfd,int operation,int fd,int events);

EpollPoller::EpollPoller():
    Poller(kInitEpollSize)
    ,epollfd_(::epoll_create(kInitEpollSize))
{
}

EpollPoller::~EpollPoller()
{
  ::close(epollfd_);
}

void EpollPoller::AddEventHandler(EventHandler *event_handler)
{
  this->SetEventHandler(event_handler->fd(), event_handler);
  if(event_handler->fd() >= this->events_.size())
  {
    events_.resize(events_.size() * 2);
  }
  if(Epoll_Ctl(epollfd_, EPOLL_CTL_ADD, event_handler->fd(), event_handler->GetEventMark()) < 0)
  {
    //TODO:egmkang
    this->SetEventHandler(event_handler->fd(), nullptr);
  }
}

void EpollPoller::RemoveEventHandler(EventHandler *event_handler)
{
  this->SetEventHandler(event_handler->fd(), nullptr);
  if(Epoll_Ctl(epollfd_, EPOLL_CTL_DEL, event_handler->fd(), event_handler->GetEventMark()) < 0)
  {
      //TODO:egmkang
  }
}

void EpollPoller::PollOnce(int timeout,std::vector<EventHandler*>& fired_handler)
{
  int num = ::epoll_wait(epollfd_, &*events_.begin(), (int)events_.size(), timeout);
  for(int idx = 0; idx < num; ++idx)
  {
    const struct epoll_event& e = events_[idx];
    EventHandler *handle = this->GetEventHandler(e.data.fd);
    assert(handle);

    int mark = kEventNone;
    if(e.events & EPOLLIN)  mark |= kEventReadable;
    if(e.events & EPOLLOUT) mark |= kEventWriteable;
    if(e.events & EPOLLERR) mark |= kEventWriteable;
    if(e.events & EPOLLHUP) mark |= kEventWriteable;
    handle->SetEvents(mark);

    fired_handler.push_back(handle);
  }
}

int Epoll_Ctl(int epollfd, int operation, int fd, int events)
{
  struct epoll_event event = {9};
  event.events = events;
  return ::epoll_ctl(epollfd, operation, fd, &event);
}
