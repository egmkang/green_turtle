#include <net/tcp_client.h>
#include <net/event_loop.h>
#include <message_queue.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace green_turtle;
using namespace green_turtle::net;

static int send_times_ = 0;

static char * NewEchoString()
{
  char * str = (char*)malloc(100);
  snprintf(str, 100, "this is echo string sent by client, %d times",send_times_);
  send_times_++;
  return str;
}

class EchoTcpClient : public TcpClient
{
 public:
  EchoTcpClient(const std::string& ip, unsigned short port) : TcpClient(ip, port){}
 protected:
  virtual void ProcessInputData(CacheLine& data)
  {
    size_t size = data.GetSize();
    std::cout << std::string((char*)data.GetBegin(),(char*)data.GetEnd()) << std::endl;
    if(size)
    {
      data.SkipRead(size);
      char *str = NewEchoString();
      this->SendMessage(str, strlen(str));
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
    this->event_loop()->Ternimal();
  }
};


int main()
{
  EchoTcpClient client("127.0.0.1", 10001);
  int errorCode = client.Connect();
  assert(!errorCode);
  client.set_events(kEventReadable | kEventWriteable);

  char *str = NewEchoString();
  client.SendMessage(str, strlen(str));

  EventLoop loop(1024);
  loop.AddEventHandler(&client);

  loop.Loop();
  return 0;
}
