#ifndef __EPOLL_POLLER__
#define __EPOLL_POLLER__
#include <sys/epoll.h>
#include "poller.h"

namespace green_turtle{
namespace net{

class EventHandler;

class EpollPoller : public Poller
{
  public:
   EpollPoller();
   virtual ~EpollPoller();
   virtual void AddEventHandler(EventHandler *event_handler);
   virtual void RemoveEventHandler(EventHandler *event_handler);
   virtual void PollOnce(int timeout,std::vector<EventHandler*>& fired_handler);
  private:
   int epollfd_;
   std::vector<struct epoll_event> events_;
};

}
}

#endif
