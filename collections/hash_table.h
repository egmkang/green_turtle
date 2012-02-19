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
// auhor: egmkang (egmkang@gmail.com)

#ifndef __MY_HASH_TABLE__
#define __MY_HASH_TABLE__
#include <utility>
#include <functional>
#include <cstddef>
#include <stdlib.h>
#include "../util/hash_function.h"
#include "../util/constructor_in_place.h"
namespace green_turtle{namespace collections{

//hash_table with linear probing
template<class Key,
        class T,
        class Hash = util::hash<Key>,
        class KeyEqual = std::equal_to<Key> >
class hash_map
{
 public:
  typedef Key                     key_type;
  typedef T                       mapped_type;
  typedef std::pair<const Key,T>  value_type;
  typedef size_t                  size_type;
  typedef Hash                    hash_fn;
  typedef KeyEqual                equal_fn;

  hash_map(size_type capability = 32,key_type empty = key_type(),key_type deleted = key_type()):
    empty_key_(empty)
    ,deleted_key_(deleted)
    ,size_(0)
    ,capability_(capability)
    ,buckets_()
    ,hasher_()
    ,equaler_()
  {
    buckets_ = (value_type*) malloc(sizeof(value_type) * capability_);
    constructor_array<value_type,key_type,mapped_type>(buckets_,capability_,empty_key_,mapped_type());
  }
  ~hash_map()
  {
    delete[] buckets_;
  }
  hash_map(const hash_map& m,size_type capability = 32)
  {
    empty_key_ = m.empty_key_;
    deleted_key_ = m.deleted_key_;
    size_ = 0;
    capability_ = m.capability_;
    //to impl the increase and decrease method
    if(capability_ !=  capability && capability >= 32)
      capability_ = capability;
    hasher_ = m.hasher_;
    equaler_ = m.equaler_;

    buckets_ = (value_type*)malloc(sizeof(value_type)*capability_);
    constructor_array<value_type,key_type,mapped_type>(buckets_,capability_,empty_key_,mapped_type());

    copy_from(m);
  }
  hash_map& operator = (const hash_map& m)
  {
    empty_key_ = m.empty_key_;
    deleted_key_ = m.deleted_key_;
    size_ = 0;
    capability_ = m.capability_;
    hasher_ = m.hasher_;
    equaler_ = m.equaler_;
    delete[] buckets_;

    buckets_ = (value_type*)malloc(sizeof(value_type)*capability_);
    constructor_array<value_type,key_type,mapped_type>(buckets_,capability_,empty_key_,mapped_type());

    copy_from(m);
  }
  void swap(hash_map& m)
  {
    std::swap(empty_key_ , m.empty_key_);
    std::swap(deleted_key_ , m.deleted_key_);
    std::swap(size_ , m.size_);
    std::swap(capability_ , m.capability_);
    std::swap(hasher_ , m.hasher_);
    std::swap(equaler_ , m.equaler_);
    std::swap(buckets_ , m.buckets_);
  }
  
  value_type* find(const key_type& key)
  {
    if(is_key_empty(key) || is_key_deleted(key))
      return NULL;
    value_type *pair_ = find_positon(key);
    if(!pair_ || !equaler_(key,pair_->first))
      return NULL;
    return pair_;
  }

  value_type* insert(const key_type& key,const mapped_type& value)
  {
    if(is_key_deleted(key) || is_key_empty(key))
      return NULL;
    increase_capability();
    value_type *pair_ = find_positon(key);
    if(!value || !equaler_(key,pair_->first))
      return NULL;
    pair_->second = value;
    return pair_;
  }
  mapped_type& operator[](const key_type& key)
  {
    value_type *pair_ = find(key);
    if(!pair_)
    {
      pair_ = insert(key,mapped_type());
    }
    return pair_->second;
  }
  void erase(const key_type& key)
  {
    assert(empty_key_ != deleted_key_ && "you must set a deleted key value before delete it");
    value_type *pair = find(key);
    if(pair && equaler_(key,pair->first))
      set_key_deleted(pair);
    decrease_capability();
  }
  void erase(const value_type* value)
  {
    if(value) erase(value->first);
  }
  void clear()
  {
    if(empty()) return;
    for(size_t idx = 0; idx < capability_; ++idx)
    {
      buckets_[idx]->first = empty_key_;
      buckets_[idx]->second = mapped_type();
    }
    size_ = 0;
  }
  //bool (const value_type&);
  template<class Fn>
  void for_each(Fn& f)
  {
    if(empty()) return;
    for(size_t idx = 0; idx < capability_; ++idx)
    {
      if(is_key_deleted(buckets_[idx].first) ||
         is_key_empty(buckets_[idx].first))
        continue;
      if(!f(buckets_[idx]))
        break;
    }
  }

  inline void set_deleted_key(key_type k)
  {
    assert(empty_key_ != k);
    if(deleted_key_ != empty_key_)
      assert(deleted_key_ == k);
    deleted_key_ = k;
  }
  inline bool empty() const { return size_ == 0; }
  inline size_type size() const { return size_; }
  inline size_type capability() const { return capability_; }
 private:
  //return key equal position
  //or first deleted postion
  //or empty postion
  value_type* find_positon(const key_type& key)
  {
    size_type hash_pair_ = hasher_(key);
    size_type mark_ = capability_ - 1;
    size_type begin_ = hash_pair_ & mark_;
    size_type times_ = 0;
    value_type *first_deleted_ = NULL;
    while(true)
    {
      if(is_key_deleted(buckets_[begin_].first) && !first_deleted_)
        first_deleted_ = &buckets_[begin_];
      else if(is_key_empty(buckets_[begin_].first))
      {
        if(first_deleted_) return first_deleted_;
        return &buckets_[begin_];
      }
      else if(equaler_(key,buckets_[begin_].first))
        return &buckets_[begin_];

      begin_ = (++begin_) &  mark_;
      assert(times_++ <= capability_);
    }
    return NULL;
  }
  void copy_from(const hash_map& m)
  {
    if(m.empty()) return;
    for(size_t idx = 0; idx < capability_; ++idx)
    {
      if(is_key_deleted(buckets_[idx].first) ||
         is_key_empty(buckets_[idx].first))
        continue;
      insert(buckets_[idx].first,buckets_[idx].second);
    }
  }
  void increase_capability()
  {
    if(size_ > (capability_ >> 1))
    {
      hash_map _m(*this,capability_ >> 1);
      swap(_m);
    }
  }
  void decrease_capability()
  {
    if(size_ < (capability_ >> 2))
    {
      hash_map _m(*this,capability_ >> 2);
      swap(_m);
    }
  }
  void set_key_deleted(value_type& pair)
  {
      pair.first = deleted_key_;
      pair.second = mapped_type();
  }
  inline bool is_key_deleted(const key_type& key) const { return equaler_(key,deleted_key_); }
  inline bool is_key_empty(const key_type& key) const { return equaler_(key,empty_key_); }
 private:
  key_type    empty_key_;
  key_type    deleted_key_;
  size_type   size_;
  size_type   capability_;
  value_type  *buckets_;
  hash_fn     hasher_;
  equal_fn    equaler_;
};

};//end namepsace collections
};//end namespace green_turtle
#endif//__MY_HASH_TABLE__
