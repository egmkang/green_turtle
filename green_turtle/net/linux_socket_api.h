#ifndef LINUX_SOCKET_API_
#define LINUX_SOCKET_API_

#include <stdint.h>
#include <stddef.h> // for size_t

typedef int NativeHandle;
static const int kInvalidHandle = -1;

class SocketApi
{
  public:
    static NativeHandle CreateNonBlockingStreamSocket();
  public:
    static bool BindAndListen(NativeHandle h, const char *addr,
	int16_t port);
    static NativeHandle Accept(NativeHandle h);
    static bool Connect(NativeHandle h, const char *addr, int16_t port);
  public:
    static int Receive(NativeHandle h, int8_t *buf, size_t len);
    static int Send(NativeHandle h, int8_t *buf, size_t len);

  public:
    static bool SetNoDelay(NativeHandle h);
    static bool SetNonBlock(NativeHandle h);
  public:
    static bool ShutdownBoth(NativeHandle h);
    static void Close(NativeHandle h);
};

#endif

