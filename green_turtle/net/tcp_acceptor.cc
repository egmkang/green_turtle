#include <algorithm>
#include <assert.h>
#include "addr_info.h"
#include "socket_option.h"
#include "tcp_acceptor.h"
#include "event_loop.h"
#include "buffered_socket.h"
#include "event_handler_factory.h"

using namespace green_turtle;
using namespace green_turtle::net;

TcpAcceptor::TcpAcceptor(const char *ip, unsigned short port, int rev_buf, int snd_buf)
  :EventHandler(SocketOption::NewFD())
    ,addr_( new AddrInfo )
    ,idx_(0)
{
  *this->addr_ = AddrInfo(ip, port);
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
  AddrInfo info;
  int new_fd = Accept(info);
  if(new_fd <= 0)
    return new_fd;
  EventHandler *new_handler = CreateNewHandler(new_fd, info);
  new_handler->set_events(kEventReadable | kEventWriteable);

  this->loops_[idx_++]->AddEventHandler(new_handler);
  if(idx_ >= (int)this->loops_.size())
    idx_ = 0;

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

void TcpAcceptor::OnAddedIntoEventLoop(EventLoop *loop)
{
  auto iter = std::find(loops_.begin(), loops_.end(), loop);
  if(iter == loops_.end())
    loops_.push_back(loop);
}

EventHandler* TcpAcceptor::CreateNewHandler(int fd, const AddrInfo& info)
{
  return EventHandlerFactory::Instance().NewEventHandler(this, fd, info);
}
