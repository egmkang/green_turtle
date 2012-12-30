#include "buffered_socket.h"
#include "addr_info.h"
#include "socket_option.h"
#include "event_loop.h"

using namespace green_turtle;
using namespace green_turtle::net;
  
static inline unsigned int nextpow2(unsigned int x)
{
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x+1;
}

BufferedSocket::BufferedSocket(int fd,const AddrInfo& addr, int rcv_window, int snd_window)
  : EventHandler(fd)
    , addr_(addr)
    , cache_line_size_(nextpow2(SocketOption::GetSendBuffer(fd)))
    , rcv_buffer_(nullptr)
    , write_lock_()
{
  if(rcv_window)
  {
    SocketOption::SetRecvBuffer(fd, rcv_window);
  }
  rcv_buffer_ = new CacheLine(nextpow2(SocketOption::GetRecvBuffer(fd)));
  if(snd_window)
  {
    SocketOption::SetSendBuffer(fd, snd_window);
    const_cast<size_t&>(cache_line_size_) = snd_window;
  }
}

BufferedSocket::~BufferedSocket()
{
  for(auto p : snd_queue_)
  {
    delete p;
  }
  snd_queue_.clear();
  delete rcv_buffer_;
}
const AddrInfo& BufferedSocket::addr() const
{
  return this->addr_;
}

int BufferedSocket::OnRead()
{
  int nread = SocketOption::Read(fd(), rcv_buffer_->BeginWrite(), rcv_buffer_->WritableLength());
  if(nread < 0)
    return kErr;
  if(nread)
  {
    rcv_buffer_->HasWritten(nread);
    Decoding(*rcv_buffer_);
    rcv_buffer_->Retrieve();
  }
  return kOK;
}

BufferedSocket::CacheLine* BufferedSocket::GetNewCacheLine()
{
  CacheLine *cache = nullptr;
  if(!snd_queue_.empty()) cache = snd_queue_.back();
  if(!cache || !cache->WritableLength())
  {
    cache = new CacheLine(cache_line_size_);
    snd_queue_.push_back(cache);
  }
  return cache;
}

bool BufferedSocket::HasData() const
{
  if(snd_messages_.size() ||
     snd_queue_.size() > 1) return true;
  if(snd_queue_.size() == 1 &&
     snd_queue_.front()->ReadableLength())
    return true;
  return false;
}

int BufferedSocket::OnWrite()
{
  if(!HasData())
    return kOK;
  std::deque<SharedMessage> tmp_messages;
  {
    std::lock_guard<std::mutex> lock(this->write_lock_);
    tmp_messages.swap(this->snd_messages_);
  }
  for(const auto& message: tmp_messages)
  {
    const char   *data = (char*)message->data();
    unsigned int  len = message->length();
    size_t sent = 0;
    while(len - sent)
    {
      CacheLine *cache = GetNewCacheLine();
      sent += cache->Append(data + sent, std::min(len - sent, cache->WritableLength()));
    }
  }

  while(!snd_queue_.empty())
  {
    CacheLine *cache = snd_queue_.front();
    int send_size = SocketOption::Write(fd(), cache->BeginRead(), cache->ReadableLength());
    if(send_size < 0)   return kErr;
    else if(!send_size) return kOK;
    cache->HasRead(send_size);
    if(!cache->ReadableLength())
    {
      if(snd_queue_.size() == 1)
      {
        cache->Retrieve();
        break;
      }
      delete cache;
      snd_queue_.pop_front();
    }
    else
    {
      break;
    }
  }
  return kOK;
}

int BufferedSocket::OnError()
{
  this->event_loop()->RemoveEventHandler(this);
  this->DeleteSelf();
  return -1;
}

void BufferedSocket::SendMessage(std::shared_ptr<Message> &&data)
{
  std::lock_guard<std::mutex> lock(this->write_lock_);
  this->snd_messages_.emplace_back(data);
}

void BufferedSocket::SendMessage(std::shared_ptr<Message>& data)
{
  std::lock_guard<std::mutex> lock(this->write_lock_);
  this->snd_messages_.push_back(data);
}
