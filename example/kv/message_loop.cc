#include "message_loop.h"
#include <logger.h>

extern green_turtle::Logger& logger;

void MessageLoop::PushMessage(
    std::shared_ptr<green_turtle::net::BufferedSocket>&& handler,
    std::unique_ptr<uint8_t[]>&& msg) {
  Request req = {std::move(handler), std::move(msg)};
  queue_.Push(std::move(req));
}

void MessageLoop::Loop() {
  std::vector<Request> cache;
  while (this->terminal_) {
    this->queue_.Pop(cache);
    for (std::vector<Request>::iterator iter = cache.begin(); iter != cache.end();
         ++iter) {
      this->DispatchMessage(iter->handler, iter->msg);
      iter->reset();
    }
    cache.clear();
  }
}

void MessageLoopPool::Init(int32_t loop_count) {
  assert(loop_count > 0);
  if (!loops_.empty()) {
    assert(false);
  }
  for (int32_t i = 0; i < loop_count; ++i) {
    loops_.push_back(std::unique_ptr<MessageLoop>());
  }
}

void MessageLoop::DispatchMessage(
    std::shared_ptr<green_turtle::net::BufferedSocket>& handler,
    std::unique_ptr<uint8_t[]>& msg) {
  (void)handler;
  MessageHead* head = reinterpret_cast<MessageHead*>(msg.get());
  uint8_t* data = reinterpret_cast<uint8_t*>(msg.get()) + sizeof(*head);

  MessageHead response;
  response.magic = PROTOCOL_RESPONSE;
  response.opcode = head->opcode;
  response.affinity = 0;
  response.seq = head->seq;

  switch (head->opcode) {
    case PROTOCOL_CMD_VERSION: {
      std::string version(VERSION);
      ;
    } break;
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
      ERROR_LOG(logger)("%s MSG:%02x not found", __PRETTY_FUNCTION__,
                        head->opcode);
      break;
  }
}

