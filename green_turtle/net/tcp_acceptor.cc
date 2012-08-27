#include <assert.h>
#include "addr_info.h"
#include "socket_option.h"
#include "tcp_acceptor.h"
#include "event_loop.h"

using namespace green_turtle;
using namespace green_turtle::net;

TcpAcceptor::TcpAcceptor(const AddrInfo& addr, int rev_buf, int snd_buf)
  :EventHandler(SocketOption::NewFD())
    ,addr_( new AddrInfo )
{
  *this->addr_ = addr;
  SocketOption::SetNoBlock(this->fd());
  SocketOption::SetNoDelay(this->fd());
  SocketOption::SetRecvBuffer(this->fd(), rev_buf);
  SocketOption::SetSendBuffer(this->fd(), snd_buf);

  this->set_events(kEventReadable);
}

TcpAcceptor::~TcpAcceptor()
{
  delete addr_;
  addr_ = nullptr;
  SocketOption::DestoryFD(this->fd());
}

bool TcpAcceptor::Listen()
{
  int ret = SocketOption::Listen(this->fd()
                                 , addr_->sockaddr()
                                 , addr_->sockaddr_len());
  return ret != -1;
}

int TcpAcceptor::Accept()
{
  struct sockaddr_in addr;
  int new_fd = SocketOption::Accept(this->fd(), &addr);
  if(new_fd >= 0)
  {
    SocketOption::SetNoBlock(new_fd);
  }

  AddrInfo info(addr);
  (void)info.sockaddr();

  //TODO:egmkang
  //TcpTask
  return new_fd;
}

int TcpAcceptor::OnRead()
{
  int new_fd = Accept();
  if(new_fd == -1)
    return new_fd;
  //TODO:egmkang
  //Add new task to poller
  //this->event_loop()->AddEventHandler(<#green_turtle::net::EventHandler *pEventHandler#>);
  return kOK;
}

int TcpAcceptor::OnWrite()
{
  return kOK;
}

int TcpAcceptor::OnError()
{
  this->event_loop()->Ternimal();
  //TODO:egmkang
  //error processer
  return kOK;
}
