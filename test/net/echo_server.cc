#include <net/tcp_acceptor.h>
#include <net/tcp_server.h>
#include <net/buffered_socket.h>
#include <net/event_handler_factory.h>
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

class EchoMessage : public Message
{
 public:
  EchoMessage(const char *data)
      :data_(data)
  {
  }
  EchoMessage(const char *data, size_t len)
      :data_(data, data+len)
  {
  }
  void* data() const { return (void*)&data_[0]; }
  size_t length() const { return data_.length(); }
 private:
  std::string data_;
};

class EchoTask : public BufferedSocket
{
 public:
  EchoTask(int fd,const AddrInfo& addr):BufferedSocket(fd, addr){}
 protected:
  virtual void ProcessInputData(CacheLine& data)
  {
    size_t size = data.GetSize();
    if(size)
    {
      ++message_count;
      message_size += data.GetSize();
      std::shared_ptr<Message> message(new EchoMessage((char*)data.GetBegin(), data.GetSize()));
      data.SkipRead(data.GetSize());
      this->SendMessage(message);
    }
  }
  virtual void ProcessDeleteSelf()
  {
    printf("EchoTask will be disposed, %p\n", this);
    delete this;
  }
};

EventHandler* NewEventHanlder(int fd, const AddrInfo& addr)
{
  return new EchoTask(fd, addr);
}

int main()
{
  System::UpdateTime();

  ::last_update_time_ = System::GetMilliSeconds();
  signal(SIGPIPE, SIG_IGN);

  TcpAcceptor acceptor("127.0.0.1", 10001, 16*1024, 16*1024);
  bool result = acceptor.Listen();
  assert(result);

  PrintMessageCount timer;

  EventHandlerFactory::Instance().RegisterDefault(&NewEventHanlder);
  TcpServer server(1024);
  server.AddAcceptor(&acceptor);
  server.SetThreadCount(2);
  server.ScheduleTimer(&timer, 2000);
  server.Run();
  return 0;
}

