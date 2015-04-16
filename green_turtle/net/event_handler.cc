#include "event_handler.h"
#include "socket_option.h"
#include "socket_config.h"
#include "conn_manager.h"
#include "system.h"
#include <logger.h>

using namespace green_turtle;
using namespace green_turtle::net;

EventHandler::EventHandler(int fd)
    : fd_(fd),
      events_(kEventNone),
      revents_(kEventNone),
      poll_idx_(-1),
      last_active_time_(System::GetMilliSeconds()),
      event_loop_(nullptr) {}

EventHandler::~EventHandler() { this->Close(); }

void EventHandler::AddToConnManager() {
  ConnManager::Instance().AddConn(this->shared_from_this());
}

void EventHandler::HandleEvent() {
  int ret = kOK;
  if (this->events_ & this->revents_ & kEventReadable) {
    last_active_time_ = System::GetMilliSeconds();
    ret = OnRead();
  }
  if (ret == kErr) {
    OnError();
    return;
  }
  if (this->events_ & this->revents_ & kEventWriteable) {
    ret = OnWrite();
  }
  if (ret == kErr) {
    ConnManager::Instance().RemoveConn(this->shared_from_this());
    OnError();
    Shutdown();
    return;
  }
  this->revents_ = kEventNone;
}

void EventHandler::SetWindowSize(int recv_size, int send_size) {
  SocketOption::SetRecvBuffer(this->fd(), recv_size);
  SocketOption::SetSendBuffer(this->fd(), send_size);
}

void EventHandler::Shutdown() {
  OnWrite();
  SocketOption::ShutDown(this->fd());
}

void EventHandler::Close() { SocketOption::DestoryFD(fd()); }

bool EventHandler::is_timeout() const {
  return System::GetMilliSeconds() - this->last_active_time() >=
         SocketConfig::kSocketRecvTimeOut;
}
