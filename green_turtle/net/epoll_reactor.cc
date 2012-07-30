#include <sys/epoll.h>
#include <unistd.h> // for close
#include <assert.h>
#include "event_handler.h"
#include "epoll_reactor.h"

namespace
{
  const int kInvalidFd = -1;
  const int kInitEventsSize = 2000;
}

Reactor::Reactor()
  : epfd_(kInvalidFd)
{
  events_.resize(kInitEventsSize);
}

Reactor::~Reactor()
{
  DestroyFd();
}

bool Reactor::CreateFd()
{
  assert(epfd_ == kInvalidFd);
  epfd_ = epoll_create1(0);
  return epfd_ != kInvalidFd;
}

void Reactor::DestroyFd()
{
  if(epfd_ != kInvalidFd)
  {
    close(epfd_);
    epfd_ = kInvalidFd;
  }
}

bool Reactor::RegisterEventHandler(EventHandler *h)
{
  assert(epfd_ != kInvalidFd);
  assert(h != NULL);
  const int sock = h->sockfd();
  assert(sock != kInvalidFd);

  epoll_event ev;
  ev.events = EPOLLIN | EPOLLOUT;
  ev.data.ptr = h;
  return epoll_ctl(epfd_, EPOLL_CTL_ADD, sock, &ev) == 0;
}

bool Reactor::UnregisterEventHandler(EventHandler *h)
{
  assert(epfd_ != kInvalidFd);
  assert(h != NULL);
  const int sock = h->sockfd();
  assert(sock != kInvalidFd);

  return epoll_ctl(epfd_, EPOLL_CTL_DEL, sock, NULL) == 0;
}

void Reactor::RunOnce(int timeout)
{
  assert(epfd_ != kInvalidFd);

  int ready_size = epoll_pwait(epfd_, &events_[0], events_.size(),
      timeout, NULL);
  for(int i = 0; i < ready_size; ++i)
  {
    EventHandler *h = reinterpret_cast<EventHandler *>(events_[i].data.ptr);
    const uint32_t evs = events_[i].events;
    if(evs & EPOLLERR)
    {
      h->OnError();
    }
    else if(evs & EPOLLHUP)
    {
      h->OnHangUp();
    }
    else
    {
      if(evs & EPOLLIN)
      {
          const int ret = h->OnReadUntilBlock();
          (void)ret;
      }
      if(evs & EPOLLOUT)
      {
          const int ret = h->OnWriteUntilBlock();
          (void)ret;
      }
    }
  }
}

