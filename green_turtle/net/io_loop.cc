#include <thread>
#include <assert.h>
#include "io_loop.h"
#include "event_handler.h"
#include "poller.h"
#include "socket_config.h"
#include <system.h>
#include <logger.h>

using namespace green_turtle;
using namespace green_turtle::net;

IoLoop::IoLoop(int expected_size)
    : terminal_(false), loop_index_(0), check_timeout_(System::GetSeconds()) {
  poller_.reset(Poller::CreatePoller(expected_size));
  assert(poller_);
}

IoLoop::~IoLoop() {
  this->fired_handler_.clear();
  this->changed_handler_.clear();
  this->poller_.reset(nullptr);
}

void IoLoop::AddEventHandler(EventHandler *pEventHandler) {
  poller_->AddEventHandler(pEventHandler);
  pEventHandler->set_event_loop(this);
}

void IoLoop::RemoveEventHandler(EventHandler *pEventHandler) {
  poller_->RemoveEventHandler(pEventHandler);
}

void IoLoop::AddHandlerLater(EventHandler *pEventHandler) {
  std::lock_guard<std::mutex> guard(this->mutex_);
  this->changed_handler_.push_back(
      std::make_pair(true, pEventHandler->shared_from_this()));
}
void IoLoop::RemoveHandlerLater(EventHandler *pEventHandler) {
  std::lock_guard<std::mutex> guard(this->mutex_);
  this->changed_handler_.push_back(
      std::make_pair(false, pEventHandler->shared_from_this()));
}

void IoLoop::Loop() {
  assert(poller_);
  while (!this->terminal_) {
    size_t begin_time = System::GetMilliSeconds();

    fired_handler_.clear();
    poller_->PollOnce(0, fired_handler_);
    for (auto handler : fired_handler_) {
      handler->HandleEvent();
    }

    if (System::GetSeconds() > this->check_timeout_) {
      this->check_timeout_ = System::GetSeconds();
      fired_handler_.clear();
      poller_->CheckEventHandlerTimeOut(fired_handler_);
      for (auto handler : fired_handler_) {
        this->RemoveHandlerLater(handler);
      }
    }

    size_t cost_time = System::GetMilliSeconds() - begin_time;
    if (cost_time < SocketConfig::kIoLoopFrameTime) {
      System::Yield(SocketConfig::kIoLoopFrameTime - cost_time);
    }

    std::vector<HandlerPair> temp_queue;
    {
      std::lock_guard<std::mutex> guard(this->mutex_);
      this->changed_handler_.swap(temp_queue);
    }
    for (auto &pair : temp_queue) {
      if (pair.first) {
        this->AddEventHandler(pair.second.get());
      } else {
        pair.second->OnError();
      }
    }

    if (!this->loop_index_)
      Logger::Default()->Flush();
  }
}
