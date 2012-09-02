#ifndef __tcp__server__
#define __tcp__server__
#include <vector>
#include <thread>
#include <noncopyable.h>

namespace green_turtle{
namespace net{

class EventHandler;
class EventLoop;
class TcpAcceptor;
class TcpClient;
class TimerQueue;
class Timer;

class TcpServer : green_turtle::NonCopyable {
 public:
  TcpServer(int expected_size);
  ~TcpServer();
 public:
  void AddAcceptor(TcpAcceptor *acceptor);
  void AddClient(TcpClient *client, int slot = 0);
  void SetThreadCount(int count);
  void Run();
  void Terminal();
 public:
  //register a timer,unit ms
  void ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,int64_t time_delay = 0);
  //unregister a timer
  void CancelTimer(Timer *timer_ptr);
 private:
  void InitEventLoop();
  void InitThreads();
 private:
  std::vector<EventLoop*>   loops_;
  std::vector<std::thread*> threads_;
  std::vector<std::pair<EventHandler*, int>>  handler_mark_;
  TimerQueue*  timer_queue_;
  bool  is_terminal_;
  int   thread_count_;
  int   expected_size_;
};
}
}
#endif
