#ifndef __TCP_ACCEPTOR__
#define __TCP_ACCEPTOR__
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
    virtual int OnRead();
    virtual int OnWrite();
    virtual int OnError();
  public:
    bool Listen();
    int Accept();
  private:
    AddrInfo *addr_;
};

}
}

#endif

