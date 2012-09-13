#include <assert.h>
#include "event_handler_factory.h"

using namespace green_turtle;
using namespace green_turtle::net;

void EventHandlerFactory::Register(TcpAcceptor *acceptor,FactoryFunction func)
{
  if(!default_) default_ = func;
  map_[acceptor] = func;
}

void EventHandlerFactory::RegisterDefault(FactoryFunction func)
{
  this->default_ = func;
}

EventHandler* EventHandlerFactory::NewEventHandler(TcpAcceptor *acceptor,int fd, const AddrInfo& addr)
{
  EventHandler *handler = nullptr;
  auto iter = map_.find(acceptor);
  if(iter != end(map_))
  {
    handler = iter->second(fd, addr);
  }
  else
  {
    assert(default_);
    handler = default_(fd, addr);
  }
  return handler;
}
