#ifndef SOCKET_BASE_
#define SOCKET_BASE_

#include "event_handler.h"

namespace green_turtle{
namespace net{

// SocketBase is the common base class for TcpSocket, TcpAcceptor.
// SocketBase is noncopyable and pure virtual class
class SocketBase : public EventHandler
{
  public:
    SocketBase();
    SocketBase(int fd);
    virtual ~SocketBase();
    bool CreateFd();
    void DestroyFd();
  private:
    SocketBase(const SocketBase &);
    SocketBase &operator=(const SocketBase &);
  public:
    virtual int sockfd() const;
  public:
    bool SetNoDelay();
    bool SetNonBlock();
  protected:
    int sockfd_;
};

}
}
#endif

