#ifndef REACTOR_
#define REACTOR_

#ifdef __linux
  #include "epoll_reactor.h"
#else
  #error "not support platform"
#endif

#endif

