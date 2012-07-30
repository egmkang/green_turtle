#ifndef SOCKET_API_
#define SOCKET_API_

#ifdef __linux
  #include "linux_socket_api.h"
#else
  #error "not support platform"
#endif

#endif // SOCKET_API_

