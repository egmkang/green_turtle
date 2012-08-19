#include <assert.h>
#include "tcp_socket.h"
using namespace green_turtle;
using namespace green_turtle::net;

TcpSocket::TcpSocket()
{
}

TcpSocket::TcpSocket(int fd)
  : SocketBase(fd)
{
}

TcpSocket::~TcpSocket()
{
}

bool TcpSocket::Connect(const char *addr, int16_t port)
{
  assert(sockfd_ != kInvalidHandle);
  assert(NULL != addr);

  return SocketApi::Connect(sockfd_, addr, port);
} 

int TcpSocket::Receive(int8_t *buf, size_t len)
{
  assert(sockfd_ != kInvalidHandle);
  return SocketApi::Receive(sockfd_, buf, len);
}

int TcpSocket::Send(int8_t *buf, size_t len)
{
  assert(sockfd_ != kInvalidHandle);
  return SocketApi::Send(sockfd_, buf, len);
}

