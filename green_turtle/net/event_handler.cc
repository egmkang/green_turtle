#include "event_handler.h"
#include "socket_option.h"

using namespace green_turtle;
using namespace green_turtle::net;

EventHandler::EventHandler(int fd) :
    fd_(fd)
    ,events_(kEventNone)
    ,revents_(kEventNone)
    ,poll_idx_(-1)
    ,event_loop_(nullptr)
{
}

EventHandler::~EventHandler()
{
  SocketOption::DestoryFD(fd());
}

void EventHandler::HandleEvent()
{
  int ret = kOK;
  if(this->events_ & this->revents_ & kEventReadable)
  {
    ret = OnRead();
  }
  if(ret == kErr)
  {
    OnError();
    return;
  }
  if(this->events_ & this->revents_ & kEventWriteable)
  {
    ret = OnWrite();
  }
  if(ret == kErr)
  {
    OnError();
    return;
  }
  this->revents_ = kEventNone;
}
