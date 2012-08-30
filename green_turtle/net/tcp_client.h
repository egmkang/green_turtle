#ifndef __TCP__CLIENT__
#define __TCP__CLIENT__
#include <string>
#include "buffered_socket.h"

namespace green_turtle{
namespace net{

class TcpClient : public BufferedSocket
{
 public:
  TcpClient(const std::string& ip,unsigned short port);
  ~TcpClient();
  int Connect();
 protected:
  int OnError();
};

}
}
#endif
