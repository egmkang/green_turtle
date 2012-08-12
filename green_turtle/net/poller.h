#ifndef __POLLER__
#define __POLLER__
#include <assert.h>
#include <vector>
#include <noncopyable.h>

namespace green_turtle{
namespace net{

class EventHandler;

class Poller : green_turtle::NonCopyable
{
 public:
  Poller(int init_size);
  virtual ~Poller();
 public:
  virtual void AddEventHandler(EventHandler *pEventHandler)       = 0;
  virtual void RemoveEventHandler(EventHandler *pEventHandler)    = 0;
  virtual void PollOnce(int timeout,std::vector<EventHandler*>& fired_handlers) = 0;
 protected:
  void SetEventHandler(int fd,EventHandler *handler);
  inline EventHandler* GetEventHandler(int fd)
  {
    assert(fd >= (int)event_handlers_.size());
    return event_handlers_[fd];
  }
 public:
  static Poller* CreatePoller(int expected_size);
 private:
  std::vector<EventHandler*> event_handlers_;
};

}
}

#endif
