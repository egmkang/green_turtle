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
#include <utility>
#include <type_traits>
#include <string>
#include <ctype.h>
#include <num2a.h>
#include <slice.h>

namespace green_turtle {

enum {
  kAlignLeft = 1,
  kAlignRight = 2,
  kAlignCenter = 3,
  kAlignMask = 3,
  kAlignHex = 4,
  kAlignUpper = 8,
};

template <typename T>
struct AlignValue {
  const T &value;
  uint8_t width;
  int8_t pad;
  int8_t align;
};

template<typename T>
inline AlignValue<T> Align(int8_t align, T &&value, uint8_t width, int8_t pad = '0') {
  return AlignValue<T>{std::forward<T>(value), width, pad, align};
}

template <typename T>
inline AlignValue<T> Hex(T &&value, uint8_t width, int8_t pad = '0') {
  return Align<T>(kAlignRight, std::forward<T>(value), width, pad);
}

template <typename T>
inline AlignValue<T> UpperHex(T &&value, uint8_t width, int8_t pad = '0') {
  return Align(kAlignRight | kAlignUpper, std::forward<T>(value), width, pad);
}

template<typename T>
inline AlignValue<T> LeftAlign(T &&value, uint8_t width, int8_t pad = '0') {
  return Align(kAlignLeft, std::forward<T>(value), width, pad);
}

template<typename T>
inline AlignValue<T> RightAlign(T &&value, uint8_t width, int8_t pad = '0') {
  return Align(kAlignRight, std::forward<T>(value), width, pad);
}

template<typename T>
inline AlignValue<T> CenterAlign(T &&value, uint8_t width, int8_t pad = '0') {
  return Align(kAlignCenter, std::forward<T>(value), width, pad);
}

template <typename T>
inline int32_t ToString(T value, char *buffer, int32_t left);

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

inline int32_t ToString(const std::string& str, char *buffer, int32_t left) {
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

template <typename T>
inline int32_t ToHex(T v, char *buffer, int32_t left) {
  char array[33];
  int8_t length = 0;
  typename std::make_unsigned<T>::type value = v;

  for (int8_t index = sizeof(T) * 8 - 4; index >= 0; index -= 4) {
    uint8_t hex_value = value >> index;
    if (length || hex_value) {
      array[length++] = "0123456789abcdef"[hex_value];
    }
    value = value & ((1ull << index) - 1);
  }
  array[length] = '\0';
  if (left < length) return -1;
  memcpy(buffer, array, length);
  return length;
}

inline int32_t ToHex(const char *value, char *buffer, int32_t left) {
  return ToString(value, buffer, left);
}

inline int32_t ToHex(const std::string& value, char *buffer, int32_t left) {
  return ToString(value, buffer, left);
}

inline int32_t ToHex(StringSlice value, char *buffer, int32_t left) {
  return ToString(value, buffer, left);
}

template<int N>
inline int32_t ToHex(const char (&value)[N], char *buffer, int32_t left) {
  return ToString(value, buffer, left);
}

inline void ToUpper(char *buffer, int32_t len){
  const int32_t byte_4_len = len / 4;
  const int32_t left_len = len % 4;
  for (int32_t i = 0; i < byte_4_len; ++i) {
    uint32_t *d = reinterpret_cast<uint32_t*>(buffer) + i;
    uint32_t eax = *d;
    uint32_t ebx = (0x7f7f7f7fu & eax) + 0x05050505u;
    ebx = (0x7f7f7f7fu & ebx) + 0x1a1a1a1au;
    ebx = ((ebx & ~eax) >> 2) & 0x20202020u;
    *d = eax - ebx;
  }
  char *buffer_left = buffer + byte_4_len * 4;
  switch(left_len)
  {
    case 3: *buffer_left = toupper(*buffer_left); ++buffer_left;
    case 2: *buffer_left = toupper(*buffer_left); ++buffer_left;
    case 1: *buffer_left = toupper(*buffer_left); ++buffer_left;
    case 0: *buffer_left = '\0';
  }
}

template <typename T>
inline int32_t ToString(AlignValue<T> align, char *buffer, int32_t left) {
  int32_t length = ToHex(align.value, buffer, left);
  int32_t max_length = length < align.width ? align.width : length;
  int32_t space_length = max_length - length;
  if (length < 0 || left < max_length) return -1;
  if (align.align & kAlignUpper) {
    ToUpper(buffer, length);
  }
  if (space_length > 0) {
    if ((align.align & kAlignMask) == kAlignLeft) {
      memset(buffer + length, align.pad, space_length);
    } else if ((align.align & kAlignMask) == kAlignRight) {
      memmove(buffer + space_length, buffer, length);
      memset(buffer, align.pad, space_length);
    } else if ((align.align & kAlignMask) == kAlignCenter) {
      memmove(buffer + space_length / 2, buffer, length);
      memset(buffer, align.pad, space_length / 2);
      memset(buffer + length + space_length / 2, align.pad,
             max_length - length - space_length / 2);
    }
  }
  buffer[max_length] = '\0';
  return max_length;
}

inline int32_t __Format(char *buffer, int32_t left, int32_t sum) {
  buffer[0] = 0;
  return sum;
}

template <typename T, typename... Tn>
inline int32_t __Format(char *buffer, int32_t left, int32_t sum, T &&v,
                        Tn &&... vn) {
  int32_t length = ToString(v, buffer, left);
  if (length > left) return -1;
  sum += length;
  left -= length;
  buffer += length;
  return __Format(buffer, left, sum, std::forward<Tn>(vn)...);
}

template <typename... Tn>
int32_t Format(char *input, int32_t max_length, Tn &&... vn) {
  return __Format(input, max_length, 0, std::forward<Tn>(vn)...);
}
}
#endif
