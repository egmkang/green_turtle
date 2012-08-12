#ifndef LINUX_SOCKET_API_
#define LINUX_SOCKET_API_

#include <stdint.h>
#include <stddef.h> // for size_t

static const int kInvalidHandle = -1;

class SocketApi
{
  public:
    static int CreateNonBlockingStreamSocket();
  public:
    static bool BindAndListen(int h, const char *addr,
	int16_t port);
    static int Accept(int h);
    static bool Connect(int h, const char *addr, int16_t port);
  public:
    static int Receive(int h, int8_t *buf, size_t len);
    static int Send(int h, int8_t *buf, size_t len);

  public:
    static bool SetNoDelay(int h);
    static bool SetNonBlock(int h);
  public:
    static bool ShutdownBoth(int h);
    static void Close(int h);
};

#endif

