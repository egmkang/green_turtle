#ifndef __TCP_ACCEPTOR__
#define __TCP_ACCEPTOR__
#include <vector>
#include "event_handler.h"

namespace green_turtle{
namespace net{

enum
{
  kAcceptorWindowRecvSize   = 128*1024,
  kAcceptorWindowSendSize   = 128*1024,
};

struct AddrInfo;

class TcpAcceptor : public EventHandler
{
  public:
    TcpAcceptor(const AddrInfo& addr, int rev_buf = kAcceptorWindowRecvSize, int snd_buf = kAcceptorWindowSendSize);
    virtual ~TcpAcceptor();
  public:
    bool Listen();
  protected:
    virtual int OnRead();
    virtual int OnWrite();
    virtual int OnError();
    virtual void OnAddedIntoEventLoop(EventLoop *loop);
    virtual EventHandler* CreateNewHandler(const AddrInfo& info, int fd) = 0; //EventHandler factory
  private:
    int Accept(AddrInfo& info);
    AddrInfo                *addr_;
    std::vector<EventLoop*> loops_;
    int                     idx_; //load balance,not thread safe,so what!
};

}
}

#endif

