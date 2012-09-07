#ifndef __BUFFERED_SOCKET__
#define __BUFFERED_SOCKET__
#include <cstddef>
#include <deque>
#include <mutex>
#include <memory>
#include "event_handler.h"

namespace green_turtle{

template<class T>
class RingBuffer;

namespace net{

struct AddrInfo;

class BufferedSocket : public EventHandler
{
 public:
  typedef green_turtle::RingBuffer<unsigned char> CacheLine;
 public:
  BufferedSocket(int fd,const AddrInfo& addr);
  ~BufferedSocket();
  void SendMessage(const void *data, size_t len);
  const AddrInfo& addr() const;
 protected:
  virtual int OnRead();
  virtual int OnWrite();
  virtual int OnError();
  virtual void ProcessData(CacheLine& data) = 0;;
 private:
  std::unique_ptr<AddrInfo>   addr_;
  const size_t                cache_line_size_;
  std::deque<CacheLine*>      snd_queue_;
  std::unique_ptr<CacheLine>  rcv_buffer_;
  std::mutex                  write_lock_;
};

}
}
#endif
