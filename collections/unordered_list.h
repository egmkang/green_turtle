//Copyright 2012, egmkang wang.
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
//     * Neither the name of Google Inc. nor the names of its
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
// Author: egmkang@gmail.com (egmkang wang)

#ifndef __UNORDERED_LIST__
#define __UNORDERED_LIST__
#include <vector>
#include <cstddef>

namespace green_turtle{ namespace collections{

template<class T>
class unordered_list
{
  unordered_list():
      list_(NULL)
      ,deleted_()
      ,size_(0)
      ,bound_(0)
      ,capability_(32)
  {
    list_ = new T[capability_]();
  }
  ~unordered_list()
  {
    delete[] list_;
  }
  //TODO:
  //impl the copy construtor
  unordered_list(const unordered_list& other)
  {}
  unordered_list& operator = (const unordered_list& other)
  {
    return *this;
  }
  size_t insert(const T& value)
  {
    //TODO:
    //if full then increase the capability
    //else insert the value into the back or one slot that deleted
    return 0;
  }
  void erase(size_t index)
  {
    //TODO:
    //if the slot is deleted,ignore
    //else set the value is deleted 
    //and push the index into the container deleted_index_
  }

  inline size_t size() const { return size_; }
  inline bool empty() const { return size_ == 0; }
 private:
  void increase_capability()
  {
    T *t_ = new T[capability_*2]();
    for(size_t idx = 0; idx < capability_; idx += 4)
    {
      t_[idx+0] = list_[idx+0];
      t_[idx+1] = list_[idx+1];
      t_[idx+2] = list_[idx+2];
      t_[idx+3] = list_[idx+3];
    }
    std::swap(t_,list_);
    delete[] t_;
    capability_ *= 2;
  }
 private:
  T		      *list_;
  T         deleted_;
  size_t    size_;
  size_t    bound_;
  size_t    capability_;
  std::vector<size_t> deleted_index_;
};

};//end namespace collections 
};//end namespace green_turtle

#endif//__UNORDERED_LIST__
