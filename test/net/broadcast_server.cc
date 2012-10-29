#include <net/tcp_acceptor.h>
#include <net/tcp_server.h>
#include <net/buffered_socket.h>
#include <net/event_handler_factory.h>
#include <net/event_loop.h>
#include <message_queue.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <atomic>
#include <system.h>
#include <iostream>
#include <random>
#include <unordered_map>
#include <mutex>
#include "simple_message.h"
#include "broadcast_message.h"

using namespace green_turtle;
using namespace green_turtle::net;

class BroadCastTask;

#define THRD_COUNT    2

size_t              last_update_time_ = 0;
std::atomic<size_t> message_count(0);
size_t              last_update_message_count = 0;
std::atomic<size_t> message_size(0);
size_t              last_update_message_size = 0;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> dis(1, 100);
static std::mutex mutex_;
static std::deque<BroadCastTask*> delete_task_;
static std::unordered_map<int, BroadCastTask*> task_manager_;
static MessageQueue<std::pair<int, Command*>>  queue_[THRD_COUNT];

static int  SendMesageByPercent(int percent, const char *data, int size);
static void DeleteTask(BroadCastTask *pTask);
static void AddNewTask(BroadCastTask *pTask);
static void RemoveTask(BroadCastTask *pTask);
static BroadCastTask* GetTask(int fd);

static void PushMessage(int fd, Command *pCmd, int index)
{
  while(true)
  {
    std::pair<int, Command*> pair(fd, pCmd);
    if(!queue_[index].Push(pair))
    {
      System::Yield(0);
    }
  }
}
static std::pair<int, Command*> PopMessage(int index)
{
  std::pair<int, Command*> cmd(0, nullptr);
  queue_[index].Pop(cmd);
  return cmd;
}

class BroadCastTask : public BufferedSocket
{
 public:
  BroadCastTask(int fd,const AddrInfo& addr):BufferedSocket(fd, addr){}
 protected:
  virtual void Decoding(CacheLine& data)
  {
    while(true)
    {
      if(data.GetSize() > 4)
      {
        Command *pCommand = (Command*)(data.GetBegin());
        if(data.GetSize() >= pCommand->len)
        {
          ++message_count;
          char *raw_data = (char*)malloc(pCommand->len);
          memcpy(raw_data, data.GetBegin(), pCommand->len);
          PushMessage(this->fd(), (Command*)raw_data, this->event_loop()->LoopIndex());
        }
      }
    }
    message_size += data.GetSize();
    std::shared_ptr<Message> message(new SimpleMessage((char*)data.GetBegin(), data.GetSize()));
    data.SkipRead(data.GetSize());
    this->SendMessage(message);
  }
  virtual void DeleteSelf()
  {
    printf("BroadCastTask will be disposed, %p\n", this);
    DeleteTask(this);
  }
};

static void _MessageProc(TcpServer *pServer)
{
  (void)pServer;
  for(int idx = 0; idx < THRD_COUNT; ++idx)
  {
    while(true)
    {
      std::pair<int, Command*> pair = PopMessage(idx);
      Command *pCmd = pair.second;
      if(pCmd->type == kCommandType_Echo)
      {
        if(gen() <= 75)
        {
          SendMesageByPercent(75, (const char*)pCmd, pCmd->len);
        }
        else
        {
          BroadCastTask *pTask = GetTask(pair.first);
          std::shared_ptr<Message> message(new SimpleMessage((const char*)pCmd, pCmd->len));
          if(pTask) pTask->SendMessage(message);
        }
      }
      else if(pCmd->type == kCommandType_BroadCast)
      {
          SendMesageByPercent(100, (const char*)pCmd, pCmd->len);
      }
      delete pCmd;
    }
  }
  std::deque<BroadCastTask*> t;
  {
    std::lock_guard<std::mutex> guard(mutex_);
    std::swap(t, delete_task_);
  }
  for(auto pTask : t)
  {
    RemoveTask(pTask);
    delete pTask;
  }
}

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

static void DeleteTask(BroadCastTask *pTask)
{
  std::lock_guard<std::mutex> guard(mutex_);
  delete_task_.push_back(pTask);
}

static void AddNewTask(BroadCastTask *pTask)
{
  assert(task_manager_.find(pTask->fd()) != task_manager_.end());
  task_manager_[pTask->fd()] = pTask;
}

static void RemoveTask(BroadCastTask *pTask)
{
  task_manager_.erase(pTask->fd());
}
static BroadCastTask* GetTask(int fd)
{
  std::unordered_map<int, BroadCastTask*>::const_iterator iter = task_manager_.find(fd);
  if(iter != task_manager_.end())
    return iter->second;
  return nullptr;
}

static int SendMesageByPercent(int percent, const char *data, int len)
{
  int count = 0;
  Message *pMessage = new SimpleMessage(data, len);
  std::shared_ptr<Message> p(pMessage);
  for(auto task_pair : task_manager_)
  {
    bool is_send = false;
    if(percent < 100)
    {
      is_send = (dis(gen) <= percent);
    }
    else
    {
      is_send = true;
    }
    if(is_send)
    {
      task_pair.second->SendMessage(p);
      ++count;
    }
  }
  return count;
}

EventHandler* NewEventHanlder(int fd, const AddrInfo& addr)
{
  BroadCastTask *pTask = new BroadCastTask(fd, addr);
  AddNewTask(pTask);
  return pTask;
}

int main()
{
  System::UpdateTime();

  ::last_update_time_ = System::GetMilliSeconds();
  signal(SIGPIPE, SIG_IGN);

  TcpAcceptor acceptor("192.168.89.56", 10001, 32*1024, 32*1024);
  bool result = acceptor.Listen();
  assert(result);

  PrintMessageCount timer;

  EventHandlerFactory::Instance().RegisterDefault(&NewEventHanlder);
  TcpServer server(1024);
  server.AddAcceptor(&acceptor);
  server.SetThreadCount(2);
  server.ScheduleTimer(&timer, 2000);
  server.SetMessageProc(_MessageProc);
  server.Run();
  return 0;
}

