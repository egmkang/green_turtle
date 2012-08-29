#include "poller.h"
#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

Poller::Poller(int init_size):
    event_handlers_(init_size)
{
}
Poller::~Poller()
{
  for(auto handler : this->event_handlers_)
  {
    delete handler;
  }
}
void Poller::SetEventHandler(int fd, EventHandler *handler)
{
  assert(fd > 0);
  if(fd >= (int)event_handlers_.size())
  {
    event_handlers_.resize(event_handlers_.size() * 2);
  }
  assert(fd < (int)event_handlers_.size());
  event_handlers_[fd] = handler;
}
