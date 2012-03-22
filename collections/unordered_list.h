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
#include <deque>
#include <cstddef>
#include <assert.h>

namespace green_turtle{ namespace collections{

//fast insert/earse/for_each operations
//when inserted or deleted too many times,
//you'd better to rebuild it if the capability is large enough.
template<class T>
class unordered_list
{
 public:
  unordered_list():
      list_(NULL)
      ,deleted_()
      ,size_(0)
      ,bound_(0)
      ,capability_(8)
      ,setted_deleted_(false)
  {
    //must be 2^n
    assert( ! (capability_ & (capability_ -1)) );
    list_ = new T[capability_]();
  }
  ~unordered_list()
  {
    delete[] list_;
  }
  unordered_list(const unordered_list& other)
  {
    if(capability() < other.capability())
    {
      delete[] list_;
      list_ = new T[other.capability()]();
    }
    copy_from(other);
  }
  unordered_list& operator = (const unordered_list& other)
  {
    if(capability() < other.capability())
    {
      delete[] list_;
      list_ = new T[other.capability()]();
    }
    copy_from(other);
    return *this;
  }
  size_t insert(const T& value)
  {
    //if full then increase the capability
    //else insert the value into the back or one slot that deleted
    long long index = -1;
    if(full())
      increase_capability();
    while(!deleted_index_.empty())
    {
      index = deleted_index_.front();
      deleted_index_.pop_front();
      if(index < (long long)bound_)
        break;
      else
        index = 0;
    }
    if(index < 0)
    {
      index = bound_;
    }

    list_[index] = value;
    ++size_;
    if(index >= (long long)bound_) bound_ = index + 1;

    return index;
  }
  void erase(size_t index)
  {
    assert(setted_deleted_ && "you must set a delelted value before erase!");
    if(size() && index < bound_ && list_[index] != deleted_)
    {
      list_[index] = deleted_;
      deleted_index_.push_back(index);
      --size_;
      //consider to decrease the bound_
      while(bound_ && list_[bound_-1] == deleted_)
      {
        --bound_;
      }
    }
  }

  void swap(unordered_list &other)
  {
    std::swap(list_,other.list_);
    std::swap(size_,other.size_);
    std::swap(bound_,other.bound_);
    std::swap(capability_,other.capability_);
    std::swap(deleted_index_,other.deleted_index_);
  }

  template<class Fn>
  void for_each(Fn& f)
  {
    for(size_t idx = 0; idx < bound_ && idx < capability_; ++idx)
    {
      if(list_[idx] == deleted_) continue;
      if(!f(list_[idx])) break;
    }
  }
  inline void set_deleted(const T& v) 
  {
    if(!setted_deleted_)
    {
      deleted_ = v;
      setted_deleted_ = true;
    }
    else
    {
      assert(v == deleted_ && "you cannot changed deleted");
    }
  }

  void clear()
  {
    size_ = 0;
    bound_ = 0;
    deleted_index_.clear();
    T tmp = T();
    for(size_t idx = 0; idx < bound_; ++idx)
    {
      list_[idx] = tmp;
    }
  }
  inline const T& get(size_t idx) const {return list_[idx]; }
  inline size_t size() const { return size_; }
  inline size_t capability() const { return capability_; }
  inline bool empty() const { return size_ == 0; }
  inline bool full() const { return size_ == capability_; }
  inline bool is_deleted(size_t idx) const {return list_[idx] == deleted_; }
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
  void copy_from(const unordered_list &other)
  {
    clear();
    size_ = other.size_;
    bound_ = other.bound_;
    capability_ = other.capability_;
    deleted_ = other.deleted_;
    deleted_index_ = other.deleted_index_;

    for(size_t idx = 0; idx < capability_; idx += 4)
    {
      list_[idx+0] = other.list_[idx+0];
      list_[idx+1] = other.list_[idx+1];
      list_[idx+2] = other.list_[idx+2];
      list_[idx+3] = other.list_[idx+3];
    }
  }
 private:
  T         *list_;
  T         deleted_;
  size_t    size_;
  size_t    bound_;
  size_t    capability_;
  std::deque<size_t> deleted_index_;
  bool      setted_deleted_;
};

};//end namespace collections 
};//end namespace green_turtle

#endif//__UNORDERED_LIST__
