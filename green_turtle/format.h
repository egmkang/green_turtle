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

#ifndef __FORMAT_H__
#define __FORMAT_H__
#include <num2a.h>
#include <utility>
#include <tuple>
#include <string>
#include <slice.h>

namespace green_turtle {

template <typename T>
inline int32_t
ToString(T value, char *buffer, int32_t left);

#define FORMAT_VALUE(TYPE, FUNC)                                  \
template <>                                                       \
inline int32_t ToString(TYPE value, char *buffer, int32_t left) { \
  char array[64];                                                 \
  int32_t length = FUNC(value, array);                            \
  if (length <= left)                                             \
    memcpy(buffer, array, length);                                \
  else                                                            \
    buffer[0] = 0;                                                \
  return length;                                                  \
}

FORMAT_VALUE(unsigned char, u32toa_sse2);
FORMAT_VALUE(signed short, i32toa_sse2);
FORMAT_VALUE(unsigned short, u32toa_sse2);
FORMAT_VALUE(signed int, i32toa_sse2);
FORMAT_VALUE(unsigned int, u32toa_sse2);
FORMAT_VALUE(signed long, i64toa_sse2);
FORMAT_VALUE(unsigned long, u64toa_sse2);
FORMAT_VALUE(signed long long, i64toa_sse2);
FORMAT_VALUE(float, dtoa_milo);
FORMAT_VALUE(double, dtoa_milo);

template <>
inline int32_t ToString(signed char value, char *buffer, int32_t left) {
  if (left <= 0) return -1;
  *buffer = value;
  return 1;
}

template <>
inline int32_t ToString(StringSlice slice, char *buffer, int32_t left) {
  if (slice.size() > size_t(left)) return -1;
  memcpy(buffer, slice.data(), slice.size());
  return slice.size();
}

inline int32_t ToString(std::string& str, char *buffer, int32_t left) {
  return ToString(StringSlice(str), buffer, left);
}

template <>
inline int32_t ToString(const char *str, char *buffer, int32_t left) {
  return ToString(StringSlice(str, strlen(str)), buffer, left);
}

template <int N>
inline int32_t ToString(const char (&str)[N], char *buffer, int32_t left) {
  return ToString(StringSlice(str, N - 1), buffer, left);
}

inline int32_t __format(char *buffer, int32_t left, int32_t sum) { return sum; }

template <typename T, typename... Tn>
inline int32_t __format(char *buffer, int32_t left, int32_t sum, T &&v,
                        Tn &&... vn) {
  int32_t length = ToString(v, buffer, left);
  if (length > left) return -1;
  sum += length;
  left -= length;
  buffer += length;
  return __format(buffer, left, sum, std::forward<Tn>(vn)...);
}

template <typename... Tn>
int32_t format(char *input, int32_t max_length, Tn &&... vn) {
  return __format(input, max_length, 0, std::forward<Tn>(vn)...);
}
}
#endif
