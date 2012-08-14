#ifndef __KQUEUE_POLLER__
#define __KQUEUE_POLLER__
#include <sys/event.h>
#include <sys/time.h>
#include "poller.h"

namespace green_turtle{
namespace net{

class EventHandler;

class KqueuePoller : public Poller
{
  public:
   KqueuePoller();
   virtual ~KqueuePoller();
   virtual void AddEventHandler(EventHandler *event_handler);
   virtual void RemoveEventHandler(EventHandler *event_handler);
   virtual void PollOnce(int timeout,std::vector<EventHandler*>& fired_handler);
  private:
   int kqfd_;
   std::vector<struct kevent> events_;
};

}
}

#endif
