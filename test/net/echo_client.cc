#include <net/tcp_client.h>
#include <net/event_loop.h>
#include <message_queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <atomic>

using namespace green_turtle;
using namespace green_turtle::net;

static std::atomic<long> recv_message_count_(0);

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
      printf("%p , %s\n", this, str.c_str());
    }
    if(size)
    {
      data.SkipRead(size);
      char *str = NewEchoString(this->send_times_);
      this->SendMessage(str, strlen(str));
      recv_message_count_.store(count + 1, std::memory_order_release);
    }
    if(this->send_times_ > 10000)
    {
      this->event_loop()->RemoveHandlerLater(this);
    }
  }

  virtual void ProcessOutputMessage(const void *data, unsigned int len)
  {
    size_t sent = 0;
    while(len - sent)
    {
      CacheLine *cache = GetNewCacheLine();
      sent += cache->Write(reinterpret_cast<const unsigned char*>(data) +
                           sent, len - sent);
    }
    free((void*)data);
  }

  virtual void ProcessDeleteSelf()
  {
    delete this;
    //this->event_loop()->Ternimal();
  }
 private:
  int send_times_ = 0;
};


int main()
{
  EventLoop loop(512);

  for(int i = 0; i < 512; ++i)
  {
    EchoTcpClient *client = new EchoTcpClient("127.0.0.1", 10001);
    int errorCode = client->Connect();
    assert(!errorCode);
    client->set_events(kEventReadable | kEventWriteable);

    int num = 0;
    char *str = NewEchoString(num);
    client->SendMessage(str, strlen(str));

    loop.AddEventHandler(client);
  }


  loop.Loop();
  return 0;
}
