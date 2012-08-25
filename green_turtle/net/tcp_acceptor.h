#ifndef __TCP_ACCEPTOR__
#define __TCP_ACCEPTOR__
#include "addr_info.h"
#include "event_handler.h"

namespace green_turtle{
namespace net{

enum
{
  kWindowRecv   = 128*1024,
  kWindowSend   = 128*1024,
};

class TcpAcceptor : public EventHandler
{
  public:
    TcpAcceptor(const AddrInfo& addr, int rev_buf = kWindowRecv, int snd_buf = kWindowSend);
    virtual ~TcpAcceptor();
  public:
    virtual int OnRead();
    virtual int OnWrite();
    virtual int OnError();
  public:
    bool Listen();
    int Accept();
  private:
    AddrInfo addr_;
};

}
}

#endif

