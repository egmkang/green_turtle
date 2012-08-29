#ifndef __SOCKET_OPTION__
#define __SOCKET_OPTION__
#include <cstddef>
struct sockaddr;

namespace green_turtle{
namespace net{

class SocketOption{
  public:
   static int   NewFD();
   static int   DestoryFD(int fd);
   static int   SetNoDelay(int fd);
   static int   SetNoBlock(int fd);
   static int   GetSendBuffer(int fd);
   static int   GetRecvBuffer(int fd);
   static int   SetSendBuffer(int fd, int size);
   static int   SetRecvBuffer(int fd, int size);
   static int   Listen(int fd, struct sockaddr* addr, int len);
   static int   Accept(int fd, struct sockaddr_in* info);
   static int   Write(int fd, const void *data, size_t len);
   static int   Read(int fd, void *data, const size_t len);
};
}
}
#endif
