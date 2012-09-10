#ifndef __EVENT_HANDLER_FACTORY__
#define __EVENT_HANDLER_FACTORY__
#include <map>
#include <functional>
#include <singleton.h>

namespace green_turtle{
namespace net{

class EventHandler;
class TcpAcceptor;
struct AddrInfo;

typedef std::function<EventHandler* (const AddrInfo&, int)> FactoryFunction;

class EventHandlerFactory : public green_turtle::Singleton<EventHandlerFactory>
{
 public:
  void Register(TcpAcceptor *acceptor,FactoryFunction func);
  void RegisterDefault(FactoryFunction func);
  EventHandler* NewEventHandler(TcpAcceptor *acceptor, const AddrInfo& addr, int fd);
 private:
  FactoryFunction default_;
  std::map<TcpAcceptor*, FactoryFunction> map_;
};

}
}

#endif
