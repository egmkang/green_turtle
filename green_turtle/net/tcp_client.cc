#include "tcp_client.h"
#include "addr_info.h"
#include "socket_option.h"
#include "event_loop.h"

using namespace green_turtle;
using namespace green_turtle::net;

TcpClient::TcpClient(const std::string& ip, unsigned short port, int recvWindowSize, int sendWindowSize)
  : BufferedSocket(SocketOption::NewFD(), AddrInfo(ip.c_str(), port), recvWindowSize, sendWindowSize)
{
}

TcpClient::~TcpClient()
{
}

int TcpClient::Connect()
{
  int ret = SocketOption::Connect(this->fd(), this->addr().sockaddr(), this->addr().sockaddr_len());
  if(!ret)
  {
    SocketOption::SetNoBlock(this->fd());
  }
  return ret;
}

int TcpClient::OnError()
{
  this->event_loop()->Ternimal();
  return kOK;
}
