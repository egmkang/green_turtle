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

#include <endian.h>

#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef LIKELY
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif

#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

template<class T>
T HostToNet(T v);

template<class T>
T NetToHost(T v);

#define CONVERT_BETWEEN_NET_HOST(T,TO_HOST,TO_NET)  \
    template<>                                      \
    inline T HostToNet<T>(T v)                      \
    {                                               \
      return TO_NET(v);                             \
    }                                               \
    template<>                                      \
    inline T NetToHost(T v)                         \
    {                                               \
      return TO_HOST(v);                            \
    }

CONVERT_BETWEEN_NET_HOST(short, be16toh, htobe16)
CONVERT_BETWEEN_NET_HOST(unsigned short, be16toh, htobe16)
CONVERT_BETWEEN_NET_HOST(int, be32toh, htobe32)
CONVERT_BETWEEN_NET_HOST(unsigned int, be32toh, htobe32)
CONVERT_BETWEEN_NET_HOST(long long, be64toh, htobe64)
CONVERT_BETWEEN_NET_HOST(unsigned long long, be64toh, htobe64)

#endif
