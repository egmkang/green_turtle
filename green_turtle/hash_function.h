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

#ifndef __HASH_FUCTION__
#define __HASH_FUCTION__
#include <cstddef>
#include <string>
#include <stdint.h>
#include "SpookyV2.h"

//hash function impl
namespace green_turtle{

template<class T>
struct hash
{
  //size_t operator()(const T& value) const
  //{
  //  return 0;
  //}
};
#define TEMPLATE_RETURN_SELF(T) \
template<>                                \
struct hash<T>                            \
{                                         \
  size_t operator()(const T& c) const     \
  {                                       \
    return static_cast<size_t>(c);        \
  }                                       \
};
//signed char,unsigned char
//signed short,unsigned short
//signed int,unsigned int
//signed long,unsigned long
TEMPLATE_RETURN_SELF(signed char);
TEMPLATE_RETURN_SELF(unsigned char);
TEMPLATE_RETURN_SELF(signed short);
TEMPLATE_RETURN_SELF(unsigned short);
TEMPLATE_RETURN_SELF(signed int);
TEMPLATE_RETURN_SELF(unsigned int);
TEMPLATE_RETURN_SELF(signed long);
TEMPLATE_RETURN_SELF(unsigned long);
TEMPLATE_RETURN_SELF(signed long long);
TEMPLATE_RETURN_SELF(unsigned long long);

template<class T>
struct hash<T*>
{
  size_t operator()(const T* &p) const
  {
    return static_cast<size_t>(p);
  }
};
template<>
struct hash<float>
{
  size_t operator()(const float& f) const
  {
    return SpookyHash::Hash64(&f, sizeof(f), 0);
  }
};
template<>
struct hash<double>
{
  size_t operator()(const double& f) const
  {
    return SpookyHash::Hash64(&f, sizeof(f), 0);
  }
};
template<>
struct hash<std::string>
{
  size_t operator()(const std::string& s) const
  {
    return SpookyHash::Hash64(s.data(), s.length(), 0);
  }
};

}
#endif//__HASH_FUCTION__
