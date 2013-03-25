#include "conn_manager.h"

using namespace green_turtle;
using namespace green_turtle::net;

void ConnManager::AddHandler(const HandlerPtr& ptr)
{
  int fd = ptr->fd();
  if(fd >= (int)this->handlers_.size())
    this->handlers_.resize(fd << 1);
  this->handlers_[fd] = ptr;
}

void ConnManager::ResetHandler(int fd)
{
  if(fd > 0 && fd < (int)this->handlers_.size())
    this->handlers_[fd].reset();
}

void ConnManager::AddConn(const HandlerPtr& ptr)
{
  std::lock_guard<std::mutex> guard(this->mutex_);
  this->add_handler_.push_back(ptr);
}

void ConnManager::RemoveConn(const HandlerPtr& ptr)
{
  std::lock_guard<std::mutex> guard(this->mutex_);
  this->remove_handler_.push_back(ptr);
}

void ConnManager::Update()
{
  HandlerSet add_set;
  HandlerSet remove_set;
  {
    std::lock_guard<std::mutex> guard(this->mutex_);
    for(const HandlerPtr& ptr : this->remove_handler_)
    {
      this->ResetHandler(ptr->fd());
    }
    for(const HandlerPtr& ptr : this->add_handler_)
    {
      this->AddHandler(ptr);
    }
    add_set = this->add_handler_;
    remove_set = this->remove_handler_;
    this->add_handler_.clear();
    this->remove_handler_.clear();
  }
  if(this->add_callback_ && add_set.size())
    this->add_callback_(add_set);
  if(this->remove_callback_ && remove_set.size())
    this->remove_callback_(remove_set);
}
