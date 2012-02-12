#ifndef EPOLL_REACTOR_
#define EPOLL_REACTOR_

#include <vector>

struct epoll_event;
class EventHandler;

// TODO: yyweii
#undef EPOLL_EDGE_TRIGGERED

class Reactor
{
  public:
    Reactor();
    virtual ~Reactor();
    bool CreateFd();
    void DestroyFd();
  private:
    Reactor(const Reactor &);
    Reactor &operator=(const Reactor &);
  public:
    bool RegisterEventHandler(EventHandler *h);
    bool UnregisterEventHandler(EventHandler *h);
    // timeout
    void RunOnce(int timeout);
  private:
    int epfd_;
    std::vector<epoll_event> events_;
};

#endif

