#include <net/tcp_acceptor.h>
#include <net/event_loop.h>
#include <net/buffered_socket.h>
#include <stdlib.h>
#include <signal.h>
#include <atomic>
#include <system.h>
#include <logger.h>
#include "protocol.h"
#include <blocking_queue.h>
#include <sophia.h>

using namespace green_turtle;
using namespace green_turtle::net;

Logger& logger = Logger::Default();

class IoTask : public BufferedSocket {
 public:
  IoTask(int fd, const AddrInfo& addr) : BufferedSocket(fd, addr) {}
  ~IoTask(){
    printf("IoTask will be disposed, %p\n", this);
  }
 protected:
  void ProcessOneMessage(MessageHead* msg) {
    uint8_t* data = reinterpret_cast<uint8_t*>(msg) + sizeof(*msg);
    (void)data;

    switch (msg->opcode) {
      case PROTOCOL_CMD_VERSION:
        break;
      case PROTOCOL_CMD_NOOP:
        break;
      case PROTOCOL_CMD_ERROR:
        break;
      case PROTOCOL_CMD_GET:
        break;
      case PROTOCOL_CMD_SET:
        break;
      case PROTOCOL_CMD_DEL:
        break;
      case PROTOCOL_CMD_APPEND:
        break;
      case PROTOCOL_CMD_INC:
        break;
      case PROTOCOL_CMD_DEC:
        break;
      case PROTOCOL_CMD_FLUSH:
        break;
      default:
        ERROR_LOG(logger)("%s MSG:%02x not found", __PRETTY_FUNCTION__, msg->opcode);
        break;
    }
  }

  virtual void Decoding(Buffer& data) {
    while (true) {
      int32_t size = data.ReadableLength();
      if (size >= int32_t(sizeof(MessageHead))) {
        int32_t msg_len = *reinterpret_cast<int32_t*>(data.BeginRead());
        if (size >= msg_len) {
          this->ProcessOneMessage(
              reinterpret_cast<MessageHead*>(data.BeginRead()));
          data.HasRead(msg_len);
        } else {
          break;
        }
      }
    }
  }
};

int main(int argc, char** argv) {
  signal(SIGPIPE, SIG_IGN);

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

