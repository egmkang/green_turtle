#include <net/tcp_client.h>
#include <net/event_loop.h>
#include <message_queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <atomic>
#include <memory>
#include <iostream>

using namespace green_turtle;
using namespace green_turtle::net;

static std::atomic<long> recv_message_count_(0);

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

static char * NewEchoString(int& send_times_)
{
  char * str = (char*)malloc(100);
  snprintf(str, 100, "this is echo string sent by client, %d times",send_times_);
  send_times_++;
  return str;
}

class EchoTcpClient : public TcpClient
{
 public:
  EchoTcpClient(const std::string& ip, unsigned short port) : TcpClient(ip, port, 16*1024, 16*1024){}
 protected:
  virtual void ProcessInputData(CacheLine& data)
  {
    size_t size = data.GetSize();
    std::string str((char*)data.GetBegin(), (char*)data.GetEnd());
    long count = recv_message_count_.load(std::memory_order_acquire);
    if(count % 1024 == 0)
    {
      std::cout << "=============================================" << std::endl << str << std::endl;
    }
    if(size)
    {
      data.SkipRead(size);
      char *str = NewEchoString(this->send_times_);
      std::shared_ptr<Message> message(new EchoMessage(str));
      this->SendMessage(message);
      free(str);
      recv_message_count_.store(count + 1, std::memory_order_release);
    }
    if(this->send_times_ > 10000)
    {
      this->event_loop()->RemoveHandlerLater(this);
    }
  }

  virtual void ProcessDeleteSelf()
  {
    delete this;
  }
 private:
  int send_times_ = 0;
};

#define CLIENT_NUM    400
int main()
{
  EventLoop loop(CLIENT_NUM);

  for(int i = 0; i < CLIENT_NUM; ++i)
  {
    EchoTcpClient *client = new EchoTcpClient("127.0.0.1", 10001);
    int errorCode = client->Connect();
    assert(!errorCode);
    client->set_events(kEventReadable | kEventWriteable);

    int num = 0;
    char *str = NewEchoString(num);
    std::shared_ptr<Message> message(new EchoMessage(str));
    client->SendMessage(message);
    free(str);
    loop.AddEventHandler(client);
  }


  loop.Loop();
  return 0;
}
