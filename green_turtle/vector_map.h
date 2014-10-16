//Copyright 2014, egmkang wang.
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

#ifndef __VECTOR_MAP_H__
#define __VECTOR_MAP_H__
#include <vector>
#include <utility>
#include <algorithm>

namespace green_turtle {

template <typename Key, typename T, class Compare = std::less<Key>>
class VectorMap {
 public:
  typedef std::pair</*const*/ Key, T> value_type;
  typedef std::vector<value_type> container_type;
  typedef typename container_type::iterator iterator;
  typedef typename container_type::const_iterator const_iterator ;

 public:
  template <class... Args>
  void emplace(Args&&... arg) {
    value_type value(arg...);
    iterator iter = std::lower_bound(container_.begin(), container_.end(), value, compare_);
    if (iter != this->end() && equal_to(*iter, value)) {
      iter->second = std::move(value.second);
    } else {
      this->container_.insert(iter, std::move(value));
    }
  }

  const T& operator[](const Key& key) const {
    const_iterator iter = find(key);
    assert(iter != this->end());
    return iter->second;
  }

  iterator find(const Key& key) {
    typename container_type::iterator iter = std::lower_bound(container_.begin(), container_.end(), value_type(key, T()), compare_);
    value_type value(key, T());
    return iter != container_.end() && equal_to(*iter, value) ? iter : container_.end();
  }

  const_iterator find(const Key& key) const {
    typename container_type::const_iterator iter = std::lower_bound(container_.begin(), container_.end(), value_type(key, T()), compare_);
    value_type value(key, T());
    return iter != container_.end() && equal_to(*iter, value) ? iter : container_.end();
  }

  void erase(const Key& key) {
    iterator iter = this->find(key);
    value_type value(key, T());
    if (iter != container_.end() && equal_to(*iter, value)) this->container_.erase(iter);
  }

  iterator begin() {
    return this->container_.begin();
  }
  const_iterator begin() const {
    return this->container_.begin();
  }
  iterator end() {
    return this->container_.end();
  }
  const_iterator end() const {
    return this->container_.end();
  }

  void clear() { this->container_.clear(); }

  size_t size() const { return this->container_.size(); }


 private:
  bool equal_to(const value_type& a, const value_type& b) const {
    if (!Compare()(a.first, b.first) && !Compare()(b.first, a.first))
      return true;
    return false;
  }

  struct CompareValueType
      : public std::binary_function<value_type, value_type, bool> {
    bool operator()(const value_type& a, const value_type& b) const {
      return Compare()(a.first, b.first);
    }
  };

 private:
  container_type container_;
  CompareValueType compare_;
};
}
#endif
