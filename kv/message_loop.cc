#include "message_loop.h"
#include "protocol.h"
#include <logger.h>

using namespace green_turtle::net;
extern green_turtle::Logger* logger;

static inline void __Append(std::shared_ptr<MessageBuffer>& buffer) {
  (void)buffer;
}

template <typename T, typename... Tn>
static inline void __Append(std::shared_ptr<MessageBuffer>& buffer, T&& t,
                            Tn&&... tn) {
  buffer->Append(t);
  __Append(buffer, std::forward<Tn>(tn)...);
}

template <typename... T>
static inline void SendMessage(BufferedSocketPtr& handler, MessageHead& head,
                               T&&... v) {
  std::shared_ptr<MessageBuffer> buffer = std::make_shared<MessageBuffer>();
  MemberPtr<MessageHead> head_ptr = buffer->Append<MessageHead>();
  *head_ptr = head;
  __Append(buffer, std::forward<T>(v)...);
  head_ptr->length = buffer->length();
  handler->SendMessage(buffer);
}

static inline void SendError(BufferedSocketPtr& handler, MessageHead& head,
                             int8_t error) {
  head.opcode = PROTOCOL_CMD_ERROR;
  SendMessage(handler, head, error);
}

void MessageLoop::PushMessage(
    BufferedSocketPtr&& handler,
    std::unique_ptr<uint8_t[]>&& msg) {
  Request req = {std::move(handler), std::move(msg)};
  queue_.Push(std::move(req));
}

void MessageLoop::Loop() {
  while (this->terminal_) {
    this->queue_.Pop(this->process_queue_);
    for (auto& pair : this->process_queue_) {
      this->DispatchMessage(pair.handler, pair.msg);
      pair.reset();
    }
    this->process_queue_.clear();
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
    BufferedSocketPtr& handler,
    std::unique_ptr<uint8_t[]>& msg) {
  (void)handler;
  MessageHead* req_head = reinterpret_cast<MessageHead*>(msg.get());
  uint8_t* data = reinterpret_cast<uint8_t*>(msg.get()) + sizeof(*req_head);
  (void)data;

  MessageHead head;
  head.magic = PROTOCOL_RESPONSE;
  head.opcode = req_head->opcode;
  head.affinity = 0;
  head.seq = req_head->seq;

  switch (req_head->opcode) {
    case PROTOCOL_CMD_VERSION: {
      std::string version(VERSION);
      SendMessage(handler, head, int32_t(version.length()), version);
    } break;
    case PROTOCOL_CMD_NOOP: {
      SendMessage(handler, head);
    }
    break;
    case PROTOCOL_CMD_GET: {
      std::string key = Decode<std::string>(&data);
      (void)key;
      int8_t error = PROTOCOL_ERR_OK;
      std::string result;


      if (error) {
        SendError(handler, head, error);
      } else {
        SendMessage(handler, head, result);
      }
    } break;
    case PROTOCOL_CMD_SET: {
      std::string key = Decode<std::string>(&data);
      std::string value = Decode<std::string>(&data);
      (void)key;
      (void)value;

      SendMessage(handler, head);
    }
    break;
    case PROTOCOL_CMD_DEL: {
      std::string key = Decode<std::string>(&data);
      (void)key;
      SendMessage(handler, head);
    } break;
    case PROTOCOL_CMD_APPEND: {
      std::string key = Decode<std::string>(&data);
      std::string append = Decode<std::string>(&data);
      (void)key;
      (void)append;
      std::string value;
      value.append(append);

      SendMessage(handler, head);
    } break;
    case PROTOCOL_CMD_INC: {
      std::string key = Decode<std::string>(&data);
      (void)key;
      std::string value;
      //snprintf
      SendMessage(handler, head, value);
    } break;
    case PROTOCOL_CMD_DEC: {
      std::string key = Decode<std::string>(&data);
      (void)key;
      std::string value;
      //snprintf
      SendMessage(handler, head, value);
    } break;
    case PROTOCOL_CMD_FLUSH: {
      SendMessage(handler, head);
    } break;
    default:
      ERROR_LOG(logger)("%s MSG:%02x not found", __PRETTY_FUNCTION__,
                        req_head->opcode);
      break;
  }
}

