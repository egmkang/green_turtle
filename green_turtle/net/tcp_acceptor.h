#ifndef TCP_ACCEPTOR_
#define TCP_ACCEPTOR_

#include "socket_base.h"
class TcpSocket;

class TcpAcceptor : public SocketBase
{
  public:
    TcpAcceptor();
    virtual ~TcpAcceptor();
  public:
    // if addr is NULL, port on all interfaces will be listened 
    bool BindAndListen(const char *addr, int16_t port);
    TcpSocket *Accept();
  public:
    // virtual int OnReadUntilBlock() {}
    // virtual int OnWriteUntilBlock() {}
    // virtual int OnRead() {}
    // virtual int OnWrite() {}
    // virtual int OnError() {}
    // virtual int OnHangUp() {}
    // virtual int OnReadHangUp() {}
};

#endif

