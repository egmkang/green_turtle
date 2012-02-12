#ifndef SOCKET_BASE_
#define SOCKET_BASE_

#include "socket_api.h"
#include "event_handler.h"

// SocketBase is the common base class for TcpSocket, TcpAcceptor.
// SocketBase is noncopyable and pure virtual class
class SocketBase : public EventHandler
{
  public:
    SocketBase();
    SocketBase(NativeHandle fd);
    virtual ~SocketBase();
    bool CreateFd();
    void DestroyFd();
  private:
    SocketBase(const SocketBase &);
    SocketBase &operator=(const SocketBase &);
  public:
    virtual NativeHandle sockfd() const;
  public:
    bool SetNoDelay();
    bool SetNonBlock();
  protected:
    NativeHandle sockfd_;
};

#endif

