#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>
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
  assert(ret != -1);
  return ret;
}
int SocketOption::SetNoBlock(int fd)
{
  int flags = 0;
  int ret = 0;
  flags = fcntl(fd, F_GETFL);
  assert(flags != -1);
  if(flags == -1)
    return flags;
  ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  assert(ret != -1);
  return ret;
}
int SocketOption::GetSendBuffer(int fd)
{
  socklen_t buff_szie = sizeof(socklen_t);
  int optname = 0;
  int ret = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optname , &buff_szie);
  (void)ret;
  assert(ret != -1);
  assert(optname);
  return optname > 0 ? optname : 0;
}

int SocketOption::GetRecvBuffer(int fd)
{
  socklen_t buff_szie = sizeof(socklen_t);
  int optname = 0;
  int ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optname, &buff_szie);
  (void)ret;
  assert(ret != -1);
  assert(optname);
  return optname > 0 ? optname : 0;
}

int SocketOption::SetSendBuffer(int fd, int size)
{
  int ret = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
  (void)ret;
  assert(ret != -1);
  return ret;
}
int SocketOption::SetRecvBuffer(int fd, int size)
{
  int ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
  assert(ret != -1);
  return ret;
}
int SocketOption::Listen(int fd, const struct sockaddr* addr, int len)
{
  int optval = 1;
  ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  int ret = ::bind(fd, addr, len);
  int error_no = errno;
  (void)error_no;
  if(ret == -1)
  {
    ::close(fd);
    return ret;
  }
  ret = ::listen(fd, 511);
  error_no = errno;
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
  int error_no = errno;
  //from linux accept(2) man page
  if(accept_fd == -1 && 
     (error_no == EAGAIN ||
      error_no == ENETDOWN ||
      error_no == EPROTO ||
      error_no == ENOPROTOOPT ||
      error_no == EHOSTDOWN ||
#ifdef HAVE_EPOLL
      error_no == ENONET ||
#endif
      error_no == EHOSTUNREACH ||
      error_no == EOPNOTSUPP ||
      error_no == ENETUNREACH))
    accept_fd = 0;
  return accept_fd;
}
int SocketOption::Write(int fd, const void *data, size_t len)
{
  int nwrite = ::write(fd, data, len);
  int error_no = errno;
  if(nwrite == -1 && error_no == EAGAIN) nwrite = 0;
  return nwrite;
}

int SocketOption::Read(int fd, void *data, const size_t len)
{
  int nread = ::read(fd, data, len);
  int error_no = errno;
  (void)error_no;
  if(nread == -1 && errno == EAGAIN) nread = 0;
  return nread;
}

int SocketOption::Readv(int fd, iovec *iov, int count)
{
  int nread = ::read(fd, iov, count);
  int error_no = errno;
  (void)error_no;
  if(nread == -1 && errno == EAGAIN) nread = 0;
  return nread;
}

int SocketOption::Connect(int fd, const sockaddr *addr, int len)
{
  int ret = ::connect(fd, addr, len);
  if(ret == EINPROGRESS) ret = 0;
  return ret;
}
void  SocketOption::ShutDown(int fd)
{
  ::shutdown(fd, SHUT_RDWR);
}
