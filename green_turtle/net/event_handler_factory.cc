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

EventHandler* EventHandlerFactory::NewEventHandler(TcpAcceptor *acceptor, const AddrInfo& addr, int fd)
{
  EventHandler *handler = nullptr;
  auto iter = map_.find(acceptor);
  if(iter != end(map_))
  {
    handler = iter->second(addr,fd);
  }
  else
  {
    assert(default_);
    handler = default_(addr, fd);
  }
  return handler;
}
