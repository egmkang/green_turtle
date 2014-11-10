#include "tcp_client.h"
#include "addr_info.h"
#include "socket_option.h"
#include "event_loop.h"
#include "logger.h"

using namespace green_turtle;
using namespace green_turtle::net;

TcpClient::TcpClient(const std::string& ip, unsigned short port)
    : BufferedSocket(SocketOption::NewFD(), AddrInfo(ip.c_str(), port)) {}

TcpClient::~TcpClient() {}

int TcpClient::Connect() {
  int ret = SocketOption::Connect(this->fd(), this->addr().sockaddr(),
                                  this->addr().sockaddr_len());
  DEBUG_LOG(Logger::Default())("Connect To ip:", this->addr().addr_str, ", port:", this->addr().addr_port, ", ret:", ret);
  if (!ret) {
    SocketOption::SetNoBlock(this->fd());
  }
  return ret;
}

int TcpClient::OnError() {
  ERROR_LOG(Logger::Default())("TcpClient(", this->addr().addr_str, ':', this->addr().addr_port, ") Error:", errno);
  this->event_loop()->Ternimal();
  return kOK;
}
