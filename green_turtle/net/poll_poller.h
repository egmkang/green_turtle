#ifndef __POLL_POLLER__
#define __POLL_POLLER__
#include <poll.h>
#include <vector>
#include "poller.h"

namespace green_turtle{
namespace net{

class EventHandler;

class PollPoller : public Poller
{
 public:
  PollPoller();
  ~PollPoller();
 public:
   virtual void AddEventHandler(EventHandler *event_handler);
   virtual void RemoveEventHandler(EventHandler *event_handler);
   virtual void PollOnce(int timeout,std::vector<EventHandler*>& fired_handler);
 private:
   std::vector<struct pollfd> pollfds_;
   bool polling_;
};

}
}

#endif
