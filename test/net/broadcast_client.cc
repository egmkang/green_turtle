#include <net/tcp_client.h>
#include <net/event_loop.h>
#include <net/timer_queue.h>
#include <net/timer.h>
#include <message_queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <atomic>
#include <memory>
#include <iostream>
#include <random>
#include <system.h>
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
  BroadCastClient(const std::string& ip, unsigned short port) : TcpClient(ip, port){}
 protected:
  virtual void Decoding(Buffer& data)
  {
    while(true)
    {
      Command *pCommand = (Command*)(data.BeginRead());
      if(data.ReadableLength() > 4 && data.ReadableLength() >= pCommand->len)
      {
        assert(pCommand->len < 2000);
        assert(pCommand->type < 3);
        ++recv_message_count[pCommand->type];
        data.HasRead(pCommand->len);
      }
      else
      {
        break;
      }
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
};

static void RandMessage(TcpClient *pClient)
{
  std::shared_ptr<MessageBuffer> message;
  char raw_data[1024];
  int rand = dis(gen);
  if(rand < 50)
  {
    EchoCommand *pEchoCmd = (EchoCommand*)(raw_data);
    new (pEchoCmd) EchoCommand();
    pEchoCmd->data_len = snprintf(pEchoCmd->data, 300, "this is a EchoMessage, random value %u",
                                  (unsigned int)gen());
    pEchoCmd->len = pEchoCmd->Length();
    assert(pEchoCmd->len < 2000);
    assert(pEchoCmd->type < 3);
    message = std::make_shared<MessageBuffer>();
    message->Append(raw_data, pEchoCmd->Length());
  }
  else
  {
    BroadCastCommand *pBroadCast = (BroadCastCommand*)(raw_data);
    new (pBroadCast) BroadCastCommand();
    pBroadCast->data_len = snprintf(pBroadCast->data, 300, "this is a BroadCastMessage, random value %u",
                                    (unsigned int)gen());
    pBroadCast->len = pBroadCast->Length();
    assert(pBroadCast->len < 2000);
    assert(pBroadCast->type < 3);
    message = std::make_shared<MessageBuffer>();
    message->Append(raw_data, pBroadCast->Length());
  }
  pClient->SendMessage(std::move(message));
}

#define CLIENT_NUM    200
int main()
{
  EventLoop loop(CLIENT_NUM);

  for(int i = 0; i < CLIENT_NUM; ++i)
  {
    std::shared_ptr<BroadCastClient> client = std::make_shared<BroadCastClient>("192.168.89.56", 10001);
    int errorCode = client->Connect();
    assert(!errorCode);
    (void)errorCode;
    client->set_events(kEventReadable | kEventWriteable);

    loop.AddEventHandler(client.get());
    loop.ScheduleTimer(client.get(), next_timer(gen));
  }
  loop.Loop();
  return 0;
}
