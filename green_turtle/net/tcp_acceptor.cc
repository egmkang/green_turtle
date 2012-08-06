#include <assert.h>
#include "tcp_socket.h"
#include "tcp_acceptor.h"

TcpAcceptor::TcpAcceptor()
{
}

TcpAcceptor::~TcpAcceptor()
{
}

bool TcpAcceptor::BindAndListen(const char *addr, int16_t port)
{
  assert(sockfd_ != kInvalidHandle);
  return SocketApi::BindAndListen(sockfd_, addr, port);
}

TcpSocket *TcpAcceptor::Accept()
{
  assert(sockfd_ != kInvalidHandle);
  int h = SocketApi::Accept(sockfd_);
  TcpSocket *ret = NULL;
  if(h != kInvalidHandle)
  {
    ret = new TcpSocket(h);
  }
  return ret;
}

