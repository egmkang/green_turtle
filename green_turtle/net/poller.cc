#include "poller.h"
#include "event_handler.h"
#include "poll_poller.h"

#ifdef HAVE_EPOLL
#include "epoll_poller.h"
#endif

#ifdef HAVE_KQUEUE
#include "kqueue_poller.h"
#endif

using namespace green_turtle;
using namespace green_turtle::net;

Poller::Poller(int init_size) : event_handlers_(init_size) {}

Poller::~Poller() { this->event_handlers_.clear(); }

void Poller::SetEventHandler(int fd, EventHandler *handler) {
  assert(fd > 0);
  if (fd >= (int)event_handlers_.size()) {
    event_handlers_.resize(event_handlers_.size() * 2);
  }
  assert(fd < (int)event_handlers_.size());
  event_handlers_[fd] = handler->shared_from_this();
}

Poller *Poller::CreatePoller(int expected_size) {
  Poller *poller = nullptr;
  if (expected_size > 128) {
#ifdef HAVE_EPOLL
    poller = new EpollPoller();
#endif
#ifdef HAVE_KQUEUE
    poller = new KqueuePoller();
#endif
  }
  if (!poller) {
    poller = new PollPoller();
  }
  return poller;
}
