#ifndef EVENT_HANDLER_
#define EVENT_HANDLER_

#include "socket_api.h"

class EventHandler
{
  public:
    virtual NativeHandle sockfd() const = 0;
    virtual int OnReadUntilBlock() = 0;
    virtual int OnWriteUntilBlock() = 0;
    virtual int OnRead() = 0;
    virtual int OnWrite() = 0;
    virtual int OnError() = 0;
    virtual int OnHangUp() = 0;
    virtual int OnReadHangUp() = 0;
};

#endif

