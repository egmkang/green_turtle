#include <net/tcp_client.h>
#include <net/event_loop.h>
#include <net/timer_queue.h>
#include <net/timer.h>
#include <message_queue.h>
#include <constructor_in_place.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <atomic>
#include <memory>
#include <iostream>
#include <random>
#include "simple_message.h"
#include "broadcast_message.h"

using namespace green_turtle;
using namespace green_turtle::net;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> dis(1, 100);
static std::uniform_int_distribution<int> next_timer(100, 400);

static long recv_message_count[3] = {0};

static void RandMessage(TcpClient *pClient);

class BroadCastClient : public TcpClient, public Timer
{
 public:
  BroadCastClient(const std::string& ip, unsigned short port) : TcpClient(ip, port, 16*1024, 16*1024){}
 protected:
  virtual void Decoding(CacheLine& data)
  {
    Command *pCommand = (Command*)(data.GetBegin());
    if(data.GetSize() >= pCommand->len)
    {
      ++recv_message_count[pCommand->type];
      data.SkipRead(pCommand->len);
    }
  }

  virtual void OnTimeOut(uint64_t current_time)
  {
    (void)current_time;
    this->event_loop()->ScheduleTimer(this, next_timer(gen));
    RandMessage(this);
  }

  virtual void DeleteSelf()
  {
    delete this;
  }
 private:
  int send_times_ = 0;
};

static void RandMessage(TcpClient *pClient)
{
  std::shared_ptr<Message> message;
  char raw_data[1024];
  int rand = dis(gen);
  if(rand < 50)
  {
    EchoCommand *pEchoCmd = (EchoCommand*)(raw_data);
    constructor(pEchoCmd);
    pEchoCmd->data_len = snprintf(pEchoCmd->data, 300, "this is a EchoMessage, random value %lu",
                                  gen());
    message = std::shared_ptr<Message>(new SimpleMessage(raw_data, pEchoCmd->Length()));
  }
  else
  {
    BroadCastCommand *pBroadCast = (BroadCastCommand*)(raw_data);
    constructor(pBroadCast);
    pBroadCast->data_len = snprintf(pBroadCast->data, 300, "this is a BroadCastMessage, random value %lu",
                                    gen());
    message = std::shared_ptr<Message>(new SimpleMessage(raw_data, pBroadCast->Length()));
  }
  pClient->SendMessage(message);
}

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

    loop.AddEventHandler(client);
    loop.ScheduleTimer(client, next_timer(gen));
  }
  loop.Loop();
  return 0;
}
