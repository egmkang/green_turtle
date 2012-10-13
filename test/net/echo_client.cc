#include <net/tcp_client.h>
#include <net/event_loop.h>
#include <message_queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <atomic>
#include <memory>
#include <iostream>
#include "simple_message.h"

using namespace green_turtle;
using namespace green_turtle::net;

static std::atomic<long> recv_message_count_(0);

class BroadCastClient : public TcpClient
{
 public:
  BroadCastClient(const std::string& ip, unsigned short port) : TcpClient(ip, port, 16*1024, 16*1024){}
 protected:
  virtual void Decoding(CacheLine& data)
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
      //this->SendMessage(message);

      recv_message_count_.store(count + 1, std::memory_order_release);
    }
    if(this->send_times_ > 10000)
    {
      this->event_loop()->RemoveHandlerLater(this);
    }
  }

  virtual void DeleteSelf()
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
    BroadCastClient *client = new BroadCastClient("192.168.89.56", 10001);
    int errorCode = client->Connect();
    assert(!errorCode);
    client->set_events(kEventReadable | kEventWriteable);

    //TODO:egmkang

    loop.AddEventHandler(client);
  }


  loop.Loop();
  return 0;
}
