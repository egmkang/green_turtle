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

#ifndef __SLICE_H__
#define __SLICE_H__
#include <string>
#include <cstddef>

namespace green_turtle {

template <typename T = char>
class Slice {
 public:
   typedef T value_type;
 public:
  Slice(const value_type* data, size_t length) : data_(data), length_(length) {}

  Slice(const std::basic_string<value_type>& str) : Slice(str.data(), str.size()) {}

  template <int N>
  Slice(const value_type (&array)[N])
      : Slice(array, N) {}

  template <typename Iter>
  Slice(Iter begin, Iter end)
      : Slice(&*begin, end - begin) {}

  Slice(const Slice& slice)
      : Slice(slice.data(), slice.length()) {}
  Slice(Slice&& slice)
      : Slice(slice.data(), slice.length()) {}

  ~Slice() {}

  const value_type& operator[](size_t pos) const {
    assert(pos < length_);
    return data_[pos];
  }

  const value_type* begin() const { return data_; }
  const value_type* end() const { return data_ + length_; }
  const value_type* data() const { return data_; }
  size_t length() const { return length_; }
  size_t size() const { return length(); }

 private:
  const value_type* data_;
  size_t length_;
};

typedef Slice<char> StringSlice;
}
#endif
