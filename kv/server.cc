#include <net/tcp_acceptor.h>
#include <net/event_loop.h>
#include <net/buffered_socket.h>
#include <stdlib.h>
#include <signal.h>
#include <atomic>
#include <system.h>
#include <logger.h>
#include "protocol.h"
#include "message_loop.h"
#include <blocking_queue.h>
#include <iostream>
#include <sophia.h>

using namespace green_turtle;
using namespace green_turtle::net;

Logger& logger = Logger::InitDefaultLogger("./log.txt", nullptr);

class IoTask : public BufferedSocket {
 public:
  IoTask(int fd, const AddrInfo& addr) : BufferedSocket(fd, addr) {}
  ~IoTask() { DEBUG_LOG(logger)("IoTask will be disposed, 0x", HexPtr(this)); }

 protected:
  virtual void Decoding(Buffer& data) {
    while (true) {
      int32_t size = data.ReadableLength();
      if (size >= int32_t(sizeof(MessageHead))) {
        int32_t msg_len = *reinterpret_cast<int32_t*>(data.BeginRead());
        if (size >= msg_len) {
          uint8_t* msg = new uint8_t[msg_len];
          memcpy(msg, data.BeginRead(), msg_len);
          MessageLoopPool::Instance()
              .GetMessageLoop(reinterpret_cast<MessageHead*>(msg)->affinity)
              .PushMessage(
                  std::static_pointer_cast<green_turtle::net::BufferedSocket>(
                      this->shared_from_this()),
                  std::unique_ptr<uint8_t[]>(msg));
          data.HasRead(msg_len);
        } else {
          break;
        }
      }
    }
  }
};

extern void benchmark();
extern int32_t kThreadCount;
extern int32_t kRandomWriteCount;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  signal(SIGPIPE, SIG_IGN);

  kThreadCount = 4;
  kRandomWriteCount = 1250000;

  int64_t begin = green_turtle::System::GetMilliSeconds();
  benchmark();
  int64_t end = green_turtle::System::GetMilliSeconds();
  std::cout << "ThreadCount:" << kThreadCount << std::endl
    << "RecordCount:" << kThreadCount * kRandomWriteCount << std::endl
    << "QPS:" << kThreadCount * kRandomWriteCount / (end - begin) * 1000 << std::endl;

  MessageLoopPool::Instance().Init();

  std::shared_ptr<TcpAcceptor> acceptor = std::make_shared<TcpAcceptor>(
      "0.0.0.0", 10001, [](int fd, const AddrInfo& addr) {
        return std::make_shared<IoTask>(fd, addr);
      });
  acceptor->SetWindowSize(128 * 1024, 128 * 1024);
  bool result = acceptor->Listen();
  assert(result);
  (void)result;

  EventLoop server(1024);
  server.AddAcceptor(acceptor.get());
  server.SetThreadCount(3);
  server.Run();
  return 0;
}

