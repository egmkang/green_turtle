#include <algorithm>
#include "poll_poller.h"
#include "event_handler.h"
#include "socket_config.h"

using namespace green_turtle;
using namespace green_turtle::net;

PollPoller::PollPoller()
    : Poller(SocketConfig::kInitEventSize), pollfds_(), polling_(false) {}

PollPoller::~PollPoller() {}

void PollPoller::AddEventHandler(EventHandler *event_handler) {
  assert(!polling_);
  this->SetEventHandler(event_handler->fd(), event_handler);

  struct pollfd pfd = {0, 0, 0};

  if (event_handler->events() & kEventReadable) pfd.events |= POLLIN;
  if (event_handler->events() & kEventWriteable) pfd.events |= POLLOUT;

  pfd.fd = event_handler->fd();
  pollfds_.push_back(pfd);

  event_handler->set_index(pollfds_.size() - 1);
}

void PollPoller::RemoveEventHandler(EventHandler *event_handler) {
  assert(!polling_);
  this->SetEventHandler(event_handler->fd(), nullptr);
  int idx = event_handler->index();
  if (idx < 0) return;
  if (idx != (int)(pollfds_.size() - 1)) {
    std::iter_swap(pollfds_.begin() + idx, --pollfds_.end());
    GetEventHandler(pollfds_[idx].fd)->set_index(idx);
  }
  pollfds_.pop_back();
  event_handler->set_index(-1);
}

void PollPoller::PollOnce(int timeout,
                          std::vector<EventHandler *> &fired_handler) {
  polling_ = true;
  int num = ::poll(&pollfds_[0], pollfds_.size(), timeout);
  for (const auto &e : pollfds_) {
    if (e.revents) {
      EventHandler *handle = GetEventHandler(e.fd);
      if (!handle) continue;

      int mask = kEventNone;
      if (e.revents & POLLIN) mask |= kEventReadable;
      if (e.revents & POLLOUT) mask |= kEventWriteable;
      if (e.revents & POLLERR) mask |= kEventWriteable;
      if (e.revents & POLLHUP) mask |= kEventWriteable;
      handle->set_revents(mask);

      fired_handler.push_back(handle);
      --num;
      if (!num) break;
    }
  }
  polling_ = false;
}

