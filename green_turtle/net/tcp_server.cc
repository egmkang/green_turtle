#include <assert.h>
#include <functional>
#include "tcp_server.h"
#include "event_loop.h"
#include "tcp_acceptor.h"
#include "tcp_client.h"
#include "timer.h"
#include "timer_queue.h"
#include <systime.h>

using namespace green_turtle;
using namespace green_turtle::net;

TcpServer::TcpServer(int expected_size) : 
    timer_queue_(new TimerQueue(2048,16))
    ,is_terminal_(false)
    ,thread_count_(1)
    ,expected_size_(expected_size)
{
}

TcpServer::~TcpServer()
{
  for(auto thrd : this->threads_)
  {
    delete thrd;
  }
  for(auto loop : this->loops_)
  {
    delete loop;
  }
}

void TcpServer::AddAcceptor(TcpAcceptor *acceptor)
{
  this->handler_mark_.push_back({acceptor,-1});
  acceptor->set_events(kEventReadable);
}

void TcpServer::AddClient(TcpClient *client, int slot)
{
  assert(slot >= 0 && slot < 32);
  this->handler_mark_.push_back({client,1 << slot});
  client->set_events(kEventReadable | kEventWriteable);
}

void TcpServer::SetThreadCount(int count)
{
  assert(count >= 1);
  this->thread_count_ = count;
}

void TcpServer::Terminal()
{
  is_terminal_ = true;
  for(auto loop : this->loops_)
  {
    loop->Ternimal();
  }
}

void TcpServer::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,int64_t time_delay)
{
  this->timer_queue_->ScheduleTimer(timer_ptr, timer_interval, time_delay);
}

void TcpServer::CancelTimer(Timer *timer_ptr)
{
  this->timer_queue_->CancelTimer(timer_ptr);
}

void TcpServer::Run()
{
  int event_handler_size_per_thread = (expected_size_ + thread_count_ - 1)   / thread_count_;
  for(int idx = 0; idx < thread_count_; ++idx)
  {
    EventLoop *loop = new EventLoop(event_handler_size_per_thread);
    assert(loop);
    this->loops_.push_back(loop);
  }
  for(auto& pair : this->handler_mark_)
  {
    for(int shift = 0; shift < thread_count_; ++shift)
    {
      if(pair.second & (1 << shift))
      {
        this->loops_[shift]->AddEventHandler(pair.first);
      }
    }
  }
  for(int i = 0; i < thread_count_; ++i)
  {
    std::thread *thrd = new std::thread(std::bind(&EventLoop::Loop,this->loops_[i]));
    this->threads_.push_back(thrd);
  }
  while(!is_terminal_)
  {
    SysTime::Update();
    size_t message_begin = SysTime::GetMilliSeconds();

    //process message
    //or something

    SysTime::Update();
    size_t timer_begin = SysTime::GetMilliSeconds();
    this->timer_queue_->Update(timer_begin);

    SysTime::Update();
    size_t process_end = SysTime::GetMilliSeconds();
    //time cost
    size_t cost = process_end - message_begin;
    if(cost)
    {
    }
  }
  for(auto& thrd : this->threads_)
  {
    thrd->join();
  }
}
