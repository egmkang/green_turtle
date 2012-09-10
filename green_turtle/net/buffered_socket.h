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
  //data must in heap,cannot be an object on stack
  void SendMessage(const void *data, size_t len);
  const AddrInfo& addr() const;
  CacheLine* GetNewCacheLine();
 protected:
  virtual int OnRead();
  virtual int OnWrite();
  virtual int OnError();
  virtual void ProcessInputData(CacheLine& data) = 0;;
  virtual void ProcessOutputMessage(const void *data, unsigned int len) = 0;;
  virtual void ProcessDeleteSelf() = 0;
 private:
  typedef std::pair<const void*, unsigned int> RawData;
  std::unique_ptr<AddrInfo>   addr_;
  const size_t                cache_line_size_;
  std::deque<CacheLine*>      snd_queue_;
  std::deque<RawData>         snd_raw_data_queue;
  std::unique_ptr<CacheLine>  rcv_buffer_;
  std::mutex                  write_lock_;
};

}
}
#endif
