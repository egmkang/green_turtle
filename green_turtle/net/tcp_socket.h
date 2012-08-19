#ifndef TCP_SOCKET_
#define TCP_SOCKET_

#include "socket_api.h"
#include "socket_base.h"

namespace green_turtle{
namespace net{

class TcpSocket : public SocketBase
{
  public:
    TcpSocket();
    TcpSocket(int fd);
    virtual ~TcpSocket();
  public:
    bool Connect(const char *addr, int16_t port);
    int Receive(int8_t *buf, size_t len);
    int Send(int8_t *buf, size_t len);
  public:
    // virtual int OnReadUntilBlock() {}
    // virtual int OnWriteUntilBlock() {}
    // virtual int OnRead() {}
    // virtual int OnWrite() {}
    // virtual int OnError() {}
    // virtual int OnHangUp() {}
    // virtual int OnReadHangUp() {}
};

}
}

#endif

