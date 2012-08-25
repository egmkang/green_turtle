#ifndef __SOCKET_OPTION__
#define __SOCKET_OPTION__
struct sockaddr;

namespace green_turtle{
namespace net{

struct AddrInfo;

class SocketOption{
  public:
   static int   NewFD();
   static int   DestoryFD(int fd);
   static int   SetNoDelay(int fd);
   static int   SetNoBlock(int fd);
   static int   SetSendBuffer(int fd, int size);
   static int   SetRecvBuffer(int fd, int size);
   static int   Listen(int fd, struct sockaddr* addr, int len);
   static int   Accept(int fd, AddrInfo& info);
};
}
}
#endif
