#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <fcntl.h>
#include <string.h> // for memset
#include "linux_socket_api.h"

NativeHandle SocketApi::CreateNonBlockingStreamSocket()
{
  // SOCK_NONBLOCK since linux 2.6.27
  return socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
}

bool SocketApi::BindAndListen(NativeHandle h, const char *addr,
                              int16_t port)
{
  sockaddr_in addrin;
  memset(&addrin, 0, sizeof(addrin));
  addrin.sin_family = AF_INET;
  addrin.sin_port = htons(port);
  if(addr != NULL)
  {
    addrin.sin_addr.s_addr = inet_addr(addr);
  }
  else
  {
    addrin.sin_addr.s_addr = INADDR_ANY;
  }

  int ret = bind(h, (sockaddr *)&addrin, sizeof(addrin));
  if(ret == 0)
  {
    ret = listen(h, SOMAXCONN);
  }
  return ret;
}

NativeHandle SocketApi::Accept(NativeHandle h)
{
  sockaddr_in addrin;
  memset(&addrin, 0, sizeof(addrin));
  socklen_t len = sizeof(addrin);

  return accept4(h, (sockaddr *)&addrin, &len, 0);
}

bool SocketApi::Connect(NativeHandle h, const char *addr, int16_t port)
{
  sockaddr_in addrin;
  memset(&addrin, 0, sizeof(addrin));
  addrin.sin_family = AF_INET;
  addrin.sin_port = htons(port);
  addrin.sin_addr.s_addr = inet_addr(addr);

  return connect(h, (sockaddr *)&addrin, sizeof(addrin)) == 0;
}

ssize_t SocketApi::Receive(NativeHandle h, int8_t *buf, size_t len)
{
  return recv(h, buf, len, 0);
}

ssize_t SocketApi::Send(NativeHandle h, int8_t *buf, size_t len)
{
  return send(h, buf, len, 0);
}

bool SocketApi::SetNoDelay(NativeHandle h)
{
  int flag = 1;
  return setsockopt(h, IPPROTO_TCP, TCP_NODELAY,
      &flag, sizeof(flag)) == 0;
}

bool SocketApi::SetNonBlock(NativeHandle h)
{
  long flag = fcntl(h, F_GETFL);
  flag |= O_NONBLOCK;
  return fcntl(h, F_SETFL, flag) == 0;
}

bool SocketApi::ShutdownBoth(NativeHandle h)
{
  return shutdown(h, SHUT_RDWR) == 0;
}

void SocketApi::Close(NativeHandle h)
{
  close(h);
}

