#include "tcp_client.h"
#include "addr_info.h"
#include "socket_option.h"
#include "event_loop.h"

using namespace green_turtle;
using namespace green_turtle::net;

//class TcpClient : public BufferedSocket
//{
// public:
//  TcpClient(const std::string& ip,unsigned short port);
//  ~TcpClient();
// protected:
//  int OnError();
// private:
//};

TcpClient::TcpClient(const std::string& ip, unsigned short port)
  : BufferedSocket(SocketOption::NewFD(), AddrInfo(ip.c_str(), port))
{
}

TcpClient::~TcpClient()
{
}

int TcpClient::OnError()
{
  this->event_loop()->Ternimal();
  return kOK;
}
