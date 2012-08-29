#ifndef __BUFFERED_SOCKET__
#define __BUFFERED_SOCKET__
#include <cstddef>
#include <deque>
#include <mutex>
#include "event_handler.h"

namespace green_turtle{

struct Mutex;

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
 protected:
  int OnRead();
  int OnWrite();
  int OnError();
 private:
  AddrInfo                *addr_;
  const size_t            cache_line_size_;
  std::deque<CacheLine*>  snd_queue_;
  CacheLine               *rcv_buffer_;
  std::mutex              write_lock_;
};

}
}
#endif
