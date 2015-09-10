#ifndef __MESSAGE_LOOP_H__
#define __MESSAGE_LOOP_H__
#include "protocol.h"
#include <noncopyable.h>
#include <blocking_queue.h>
#include <net/buffered_socket.h>
#include <memory>
#include <singleton.h>
#include <vector>

struct Request {
  std::shared_ptr<green_turtle::net::BufferedSocket> handler;
  std::unique_ptr<uint8_t[]> msg;
  void reset() {
    handler.reset();
    msg.reset();
  }
};

class MessageLoop : green_turtle::NonCopyable {
 public:
  MessageLoop() : terminal_(false) {}

  void PushMessage(
      std::shared_ptr<green_turtle::net::BufferedSocket>&& handler,
      std::unique_ptr<uint8_t[]>&& msg);

  void Loop();
  template <typename... T>
  static void Send(std::shared_ptr<green_turtle::net::BufferedSocket>& handler,
                   MessageHead& head, const T&... args) {
    //TODO:egmkang
  }

  void DispatchMessage(
      std::shared_ptr<green_turtle::net::BufferedSocket>& handler,
      std::unique_ptr<uint8_t[]>& msg);

  void terminal() { this->terminal_ = true; }

 private:
  green_turtle::BlockingQueue<Request> queue_;
  bool terminal_;
};

class MessageLoopPool : public green_turtle::Singleton<MessageLoopPool> {
 public:
  void Init(int32_t loop_count = 4);

  size_t size() const { return loops_.size(); }

  MessageLoop& GetMessageLoop(int32_t index) {
    return *loops_[index % loops_.size()];
  }

 private:
  std::vector<std::unique_ptr<MessageLoop>> loops_;
};

#endif
