#include <assert.h>
#include "addr_info.h"
#include "socket_option.h"
#include "tcp_acceptor.h"
#include "event_loop.h"
#include "buffered_socket.h"
#include "event_handler_factory.h"

using namespace green_turtle;
using namespace green_turtle::net;

TcpAcceptor::TcpAcceptor(const char *ip, unsigned short port)
  :EventHandler(SocketOption::NewFD())
    ,addr_( new AddrInfo )
    ,idx_(0)
{
  *this->addr_ = AddrInfo(ip, port);
  SocketOption::SetNoBlock(this->fd());
  SocketOption::SetNoDelay(this->fd());

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

int TcpAcceptor::Accept(AddrInfo& info)
{
  struct sockaddr_in addr;
  int new_fd = SocketOption::Accept(this->fd(), &addr);
  if(new_fd > 0)
  {
    SocketOption::SetNoBlock(new_fd);
  }

  AddrInfo fd_info(addr);
  info = fd_info;

  return new_fd;
}

int TcpAcceptor::OnRead()
{
  //int count = 0;
  AddrInfo info;
  while(true)
  {
    int new_fd = Accept(info);
    if(new_fd <= 0)
      return new_fd;
    EventHandler *new_handler = CreateNewHandler(new_fd, info);
    new_handler->set_events(kEventReadable | kEventWriteable);

    this->loops_[idx_++]->AddHandlerLater(new_handler);
    if(idx_ >= loops_.size()) idx_ = 0;

    //if(++count >= 10) break;
  }

  return kOK;
}

int TcpAcceptor::OnWrite()
{
  return kOK;
}

int TcpAcceptor::OnError()
{
  for(auto loop : loops_)
  {
    loop->Ternimal();
  }
  return kOK;
}
void TcpAcceptor::loop_balance(const std::vector<EventLoop *> &loops)
{
  this->loops_ = loops;
}

EventHandler* TcpAcceptor::CreateNewHandler(int fd, const AddrInfo& info)
{
  return EventHandlerFactory::Instance().NewEventHandler(this, fd, info);
}
