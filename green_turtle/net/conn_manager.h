//Copyright 2013, egmkang wang.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of green_turtle nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// author: egmkang (egmkang@gmail.com)
#ifndef __CONN_MANAGER_H__
#define __CONN_MANAGER_H__
#include "event_handler.h"
#include <singleton.h>
#include <mutex>
#include <vector>
#include <functional>

namespace green_turtle{
namespace net{

class ConnManager : public green_turtle::Singleton<ConnManager> 
{
 public:
  typedef std::shared_ptr<EventHandler> HandlerPtr;
  typedef std::vector<HandlerPtr>       HandlerSet;
  typedef std::function<void (HandlerSet&)> CallBack;

  ConnManager(int init_size = 2048)
  {
    this->handlers_.reserve(init_size);
  }

  void AddConn(const HandlerPtr& ptr);

  void RemoveConn(const HandlerPtr& ptr);

  void Update();

  void SetAddCallback(CallBack call_back) { this->add_callback_ = call_back; }
  void SetRemoveCallbacl(CallBack call_back) { this->remove_callback_ = call_back; }
 private:
  void AddHandler(const HandlerPtr& ptr);
  void ResetHandler(int fd);
 private:
  std::mutex  mutex_;
  HandlerSet  handlers_;
  HandlerSet  add_handler_;
  HandlerSet  remove_handler_;
  CallBack    add_callback_;
  CallBack    remove_callback_;
};

}
}

#endif
