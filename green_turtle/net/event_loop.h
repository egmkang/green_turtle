#ifndef __event_loop__
#define __event_loop__
#include <vector>
#include "event_handler.h"
#include <noncopyable.h>

namespace green_turtle{
namespace net{

class Poller;

class EventLoop : NonCopyable
{
 public:
  EventLoop(int expected_size);
  ~EventLoop();
  void Ternimal() { terminal_ = true; }
  void AddEventHandler(EventHandler *pEventHandler);
  void RemoveEventHandler(EventHandler *pEventHandler);
  void Loop();
  static void SetFrameTime(int milliSeconds);
 private:
  Poller  *poller_;
  bool    terminal_;
  std::vector<EventHandler*>  fired_handler_;
};

}
}
#endif
