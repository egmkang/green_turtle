#ifndef __POLLER__
#define __POLLER__
#include <vector>
#include <noncopyable.h>

namespace green_turtle{
namespace net{

class EventHandler;

class Poller : green_turtle::NonCopyable
{
 public:
  Poller();
  virtual ~Poller();
 public:
  virtual void AddEventHanler(EventHandler *pEventHandler)      = 0;
  virtual void RemoveEventHandler(EventHandler *pEventHandler)  = 0;
  virtual void PollOnce(int timeout,std::vector<EventHandler*>& fired_handlers) = 0;
 public:
  static Poller* CreatePoller(int expected_size);
 private:
  std::vector<EventHandler*> event_handlers_;
};

}
}

#endif
