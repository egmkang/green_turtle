#include <assert.h>
#include "socket_base.h"

SocketBase::SocketBase()
  : sockfd_(kInvalidHandle)
{
}

SocketBase::SocketBase(int fd)
  : sockfd_(fd)
{
}

SocketBase::~SocketBase()
{
  DestroyFd();
}

bool SocketBase::CreateFd()
{
  assert(sockfd_ == kInvalidHandle);
  sockfd_ = SocketApi::CreateNonBlockingStreamSocket();
  return sockfd_ != kInvalidHandle;
}

void SocketBase::DestroyFd()
{
  if(sockfd_ != kInvalidHandle)
  {
    SocketApi::Close(sockfd_);
    sockfd_ = kInvalidHandle;
  }
}

int SocketBase::sockfd() const
{
  return sockfd_;
}

bool SocketBase::SetNoDelay()
{
  assert(sockfd_ != kInvalidHandle);
  return SocketApi::SetNoDelay(sockfd_);
}

bool SocketBase::SetNonBlock()
{
  assert(sockfd_ != kInvalidHandle);
  return SocketApi::SetNonBlock(sockfd_);
}

