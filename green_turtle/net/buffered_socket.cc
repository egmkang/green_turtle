#include "buffered_socket.h"
#include <mutex>
#include "addr_info.h"

using namespace green_turtle;
using namespace green_turtle::net;

//class BufferedSocket : public EventHandler
//{
// public:
//  typedef green_turtle::RingBuffer<unsigned char> CacheLine;
// public:
//  BufferedSocket(int fd,const AddrInfo& addr);
//  ~BufferedSocket();
//  void SendMessage(const void *data, size_t len);
// private:
//  AddrInfo                *addr_;
//  std::deque<CacheLine*>  snd_queue_;
//  CacheLine               *rcv_buffer_;
//  const size_t            cache_line_size_;
//  std::mutex              *write_lock_;
//};
