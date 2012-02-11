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
#include "../util/hash_function.h"
namespace green_turtle{namespace collections{

//hash_table with linear probing
template<class Key,class T,class Hash = util::hash<Key>,class KeyEqual = std::equal_to<Key> >
class hash_map
{
 public:
  typedef Key                     key_type;
  typedef T                       mapped_type;
  typedef std::pair<const Key,T>  value_type;
  typedef size_t                  size_type;
  typedef Hash                    hash_fn;
  typedef KeyEqual                equal_fn;

  //TODO:egmlang
  //a lot function to impl
  //find,operator [],erase,insert
  //increase/decrease capability
  //for_each,etc
 private:
  value_type* find_positon(const key_type& key)
  {
    size_type hash_value_ = hasher_(key);
    size_type mark_ = capability_ - 1;
    size_type begin_ = hash_value_ & mark_;
    size_type times_ = 0;
    value_type *first_deleted_ = NULL;
    while(true)
    {
      if(is_key_deleted(buckets_[begin_]) && !first_deleted_)
        first_deleted_ = &buckets_[begin_];
      else if(is_key_empty(buckets_[begin_]))
        return first_deleted_;
      else if(equaler_(key,buckets_[begin_]))
        return &buckets_[begin_];

      begin_ = (++begin_) &  mark_;
      assert(times_++ <= capability_);

    }
    return NULL;
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
