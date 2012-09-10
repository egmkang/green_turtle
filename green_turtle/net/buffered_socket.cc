#include "buffered_socket.h"
#include <ring_buffer.h>
#include "addr_info.h"
#include "socket_option.h"
#include "event_loop.h"

using namespace green_turtle;
using namespace green_turtle::net;

BufferedSocket::BufferedSocket(int fd,const AddrInfo& addr) 
  : EventHandler(fd)
    , addr_(new AddrInfo(addr))
    , cache_line_size_(SocketOption::GetRecvBuffer(fd)/4)
    , rcv_buffer_(new CacheLine(SocketOption::GetSendBuffer(fd)))
    , write_lock_()
{
}

BufferedSocket::~BufferedSocket()
{
  for(auto p : snd_queue_)
  {
    delete p;
  }
  snd_queue_.clear();
}
const AddrInfo& BufferedSocket::addr() const
{
  return *this->addr_;
}

int BufferedSocket::OnRead()
{
  rcv_buffer_->Reset();
  int nread = SocketOption::Read(fd(), rcv_buffer_->GetEnd(), rcv_buffer_->GetTailSpace());
  if(nread < 0)
    return kErr;
  rcv_buffer_->SkipWrite(nread);
  ProcessInputData(*rcv_buffer_);
  return kOK;
}

BufferedSocket::CacheLine* BufferedSocket::GetNewCacheLine()
{
  CacheLine *cache = snd_queue_.back();
  if(!cache || !cache->GetTailSpace())
  {
    cache = new CacheLine(cache_line_size_);
    snd_queue_.push_back(cache);
  }
  return cache;
}

int BufferedSocket::OnWrite()
{
  std::deque<RawData> send_raw_message_queue;
  {
    std::lock_guard<std::mutex> lock(this->write_lock_);
    send_raw_message_queue.swap(this->snd_raw_data_queue);
  }
  //use process here
  for(auto pair : send_raw_message_queue)
  {
    const void    *data = pair.first;
    unsigned int  len = pair.second;
    this->ProcessOutputMessage(data, len);
    //size_t sent = 0;
    //while(len)
    //{
    //  CacheLine *cache = GetNewCacheLine();
    //  sent += cache->Write(reinterpret_cast<const unsigned char*>(data) + sent, len - sent);
    //}
  }

  int ret = 0;
  while(ret > 0)
  {
    while(!snd_queue_.empty())
    {
      CacheLine *cache = snd_queue_.front();
      int send_size = SocketOption::Write(fd(), cache->GetBegin(), cache->GetSize());
      if(send_size < 0)   return kErr;
      else if(!send_size) return kOK;
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
  this->event_loop()->RemoveEventHandler(this); 
  this->ProcessDeleteSelf();
  return -1;
}

void BufferedSocket::SendMessage(const void *data, size_t len)
{
  std::lock_guard<std::mutex> lock(this->write_lock_);
  this->snd_raw_data_queue.push_back({data,len});
}
