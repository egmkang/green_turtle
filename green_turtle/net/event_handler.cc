#include "event_handler.h"

using namespace green_turtle;
using namespace green_turtle::net;

void EventHandler::HandleEvent()
{
  if(this->mark_ & this->revents_ & kEventReadable)
  {
    OnRead();
  }
  if(this->mark_ & this->revents_ & kEventWriteable)
  {
    OnWrite();
  }
  this->revents_ = kEventNone;
}
