#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include "socket_option.h"
#include "addr_info.h"

using namespace green_turtle;
using namespace green_turtle::net;

int SocketOption::NewFD()
{
  return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
int SocketOption::DestoryFD(int fd)
{
  return ::close(fd);
}
int SocketOption::SetNoDelay(int fd)
{
  int yes = 1, ret = 0;
  ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
#ifdef DEBUG
  assert(ret != -1);
#endif
  return ret;
}
int SocketOption::SetNoBlock(int fd)
{
  int flags = 0;
  int ret = 0;
  flags = fcntl(fd, F_GETFL);
#ifdef DEBUG
  assert(flags != -1);
#endif
  if(flags == -1)
    return flags;
  ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#ifdef DEBUG
  assert(ret != -1);
#endif
  return ret;
}
int SocketOption::GetSendBuffer(int fd)
{
  socklen_t buff_szie = 0;
  int optname = 0;
  int ret = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optname , &buff_szie);
  assert(ret != -1);
  assert(!buff_szie);
  return buff_szie > 0 ? buff_szie : 0;
}

int SocketOption::GetRecvBuffer(int fd)
{
  socklen_t buff_szie = 0;
  int optname = 0;
  int ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optname, &buff_szie);
  assert(ret != -1);
  assert(!buff_szie);
  return buff_szie > 0 ? buff_szie : 0;
}

int SocketOption::SetSendBuffer(int fd, int size)
{
  int ret = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
#ifdef DEBUG
  assert(ret != -1);
#endif
  return ret;
}
int SocketOption::SetRecvBuffer(int fd, int size)
{
  int ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
#ifdef DEBUG
  assert(ret != -1);
#endif
  return ret;
}
int SocketOption::Listen(int fd, const struct sockaddr* addr, int len)
{
  int ret = ::bind(fd, addr, len);
  if(ret == -1)
  {
    ::close(fd);
    return ret;
  }
  ret = ::listen(fd, 511);
  if(ret == -1)
  {
    ::close(fd);
  }
  return ret;
}
int SocketOption::Accept(int fd, struct sockaddr_in* addr)
{
  socklen_t salen = sizeof(*addr);
  int accept_fd = ::accept(fd, reinterpret_cast<struct sockaddr*>(addr), &salen);
  if(accept_fd == -1 && errno == EAGAIN)
    accept_fd = 0;
  return accept_fd;
}
int SocketOption::Write(int fd, const void *data, size_t len)
{
  int nwrite = ::write(fd, data, len);
  if(nwrite == -1 && errno == EAGAIN) nwrite = 0;
  return nwrite;
}

int SocketOption::Read(int fd, void *data, const size_t len)
{
  int nread = ::read(fd, data, len);
  if(nread == -1 && errno == EAGAIN) nread = 0;
  return nread;
}

int SocketOption::Connect(int fd, const sockaddr *addr, int len)
{
  int ret = ::connect(fd, addr, len);
  if(ret == EINPROGRESS) ret = 0;
  return ret;
}
