#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

void EventHandler::HandleEvent()
{
  int ret = kOK;
  if(this->mark_ & this->revents_ & kEventReadable)
  {
    ret = OnRead();
  }
  if(ret == kErr)
  {
    OnError();
    return;
  }
  if(this->mark_ & this->revents_ & kEventWriteable)
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
