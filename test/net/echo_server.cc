#include <net/tcp_acceptor.h>
#include <net/tcp_server.h>
#include <net/buffered_socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <atomic>
#include <system.h>
#include <iostream>

using namespace green_turtle;
using namespace green_turtle::net;

size_t              last_update_time_ = 0;
std::atomic<size_t> message_count(0);
size_t              last_update_message_count = 0;
std::atomic<size_t> message_size(0);
size_t              last_update_message_size = 0;

class PrintMessageCount : public Timer
{
 public:
  void OnTimeOut(uint64_t current_time)
  {
    (void)current_time;
    size_t time_stamp = System::GetMilliSeconds() - last_update_time_;
    size_t current_count = message_count.load(std::memory_order_relaxed);
    size_t current_size = message_size.load(std::memory_order_relaxed);
    size_t count = current_count - last_update_message_count;
    size_t size = current_size - last_update_message_size;
    std::cout << "Message Count : " << count
        << " , cost time : " << time_stamp << "ms , "
        "Tps : " << (float)count / time_stamp * 1000 <<
        " , " << (float)size / time_stamp * 1000/1024 << "kB/s"
        << std::endl;
    last_update_message_count = current_count;
    last_update_time_ = System::GetMilliSeconds();
    last_update_message_size = current_size;
  }
};

class EchoTask : public BufferedSocket
{
 public:
  EchoTask(int fd,const AddrInfo& addr):BufferedSocket(fd, addr){}
 protected:
  virtual void Decoding(Buffer& data)
  {
    size_t size = data.ReadableLength();
    if(size)
    {
      ++message_count;
      message_size += data.ReadableLength();
      std::shared_ptr<MessageBuffer> message = std::make_shared<MessageBuffer>();
      message->Append(data.BeginRead(), data.ReadableLength());
      data.HasRead(size);
      this->SendMessage(std::move(message));
    }
  }
  virtual void DeleteSelf()
  {
    printf("EchoTask will be disposed, %p\n", this);
    delete this;
  }
};

std::shared_ptr<EventHandler> NewEventHanlder(int fd, const AddrInfo& addr)
{
  return std::make_shared<EchoTask>(fd, addr);
}

int main(int argc, char** argv)
{
  ::last_update_time_ = System::GetMilliSeconds();
  signal(SIGPIPE, SIG_IGN);

  std::shared_ptr<TcpAcceptor> acceptor =
      std::make_shared<TcpAcceptor>("192.168.2.27", 10001,
          std::bind(&NewEventHanlder, std::placeholders::_1, std::placeholders::_2));
  acceptor->SetWindowSize(16*1024);
  bool result = acceptor->Listen();
  assert(result);
  (void)result;

  PrintMessageCount timer;

  TcpServer server(1024);
  server.AddAcceptor(acceptor.get());
  server.SetThreadCount(2);
  server.ScheduleTimer(&timer, 2000);
  server.Run();
  return 0;
}

