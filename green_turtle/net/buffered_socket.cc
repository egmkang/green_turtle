#include "buffered_socket.h"
#include <lock.h>
#include <ring_buffer.h>
#include "addr_info.h"
#include "socket_option.h"

using namespace green_turtle;
using namespace green_turtle::net;

BufferedSocket::BufferedSocket(int fd,const AddrInfo& addr) 
  : EventHandler(fd)
    , addr_(new AddrInfo(addr))
    , cache_line_size_(SocketOption::GetRecvBuffer(fd)/4)
    , rcv_buffer_(new CacheLine(SocketOption::GetSendBuffer(fd)))
    , write_lock_(new Mutex())
{

}

BufferedSocket::~BufferedSocket()
{
  delete addr_;
  for(auto p : snd_queue_)
  {
    delete p;
  }
  snd_queue_.clear();
  delete write_lock_;
}

int BufferedSocket::OnRead()
{
  rcv_buffer_->Reset();
  int nread = SocketOption::Read(fd(), rcv_buffer_->GetEnd(), rcv_buffer_->GetTailSpace());
  if(nread < 0)
    return kErr;
  rcv_buffer_->SkipWrite(nread);
  //TODO:egmkang
  //process data
  return kOK;
}

int BufferedSocket::OnWrite()
{
  LockGuard<Mutex> lock(*this->write_lock_);
  int ret = 0;
  while(ret > 0)
  {
    while(!snd_queue_.empty())
    {
      CacheLine *cache = snd_queue_.front();
      int send_size = SocketOption::Write(fd(), cache->GetBegin(), cache->GetSize());
      if(send_size < 0)
        return kErr;
      if(!send_size)
        return kOK;
      cache->SkipRead(send_size);
      if(!cache->GetSize())
      {
        delete cache;
        snd_queue_.pop_front();
      }
    }
  }
  return kOK;
}

int BufferedSocket::OnError()
{
  //TODO:egmkang
  //remove this from event loop
  //and tell someone to dispose this
  return -1;
}

void BufferedSocket::SendMessage(const void *data, size_t len)
{
  LockGuard<Mutex> lock(*this->write_lock_);
  size_t sent = 0;
  while(len)
  {
    CacheLine *cache = snd_queue_.back();
    if(!cache || !cache->GetTailSpace())
    {
      cache = new CacheLine(cache_line_size_);
      snd_queue_.push_back(cache);
    }
    sent += cache->Write(reinterpret_cast<const unsigned char*>(data) + sent, len - sent);
  }
}
