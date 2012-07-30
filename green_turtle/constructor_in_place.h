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

#ifndef __CONSTRUCTOR_IN_PLACE__
#define __CONSTRUCTOR_IN_PLACE__
namespace green_turtle{
template<class T>
void constructor(T* addr)
{
  new (addr) T();
}
template<class T,class P0>
void constructor(T* addr,P0 p0)
{
  new (addr) T(p0);
}
template<class T,class P0,class P1>
void constructor(T* addr,P0 p0,P1 p1)
{
  new (addr) T(p0,p1);
}
template<class T,class P0,class P1,class P2>
void constructor(T* addr,P0 p0,P1 p1,P2 p2)
{
  new (addr) T(p0,p1,p2);
}
template<class T,class P0,class P1,class P2,class P3>
void constructor(T* addr,P0 p0,P1 p1,P2 p2,P3 p3)
{
  new (addr) T(p0,p1,p2,p3);
}
template<class T,class P0,class P1,class P2,class P3,class P4>
void constructor(T* addr,P0 p0,P1 p1,P2 p2,P3 p3,P4 p4)
{
  new (addr) T(p0,p1,p2,p3,p4);
}
template<class T>
void constructor_array(T* addr,int size)
{
  T* addr_= (T*)addr;
  for(int idx = 0; idx < size; ++idx)
  {
    constructor<T>(addr_);
    addr_ += 1;
  }
}
template<class T,class P0>
void constructor_array(T* addr,int size,P0 p0)
{
  T* addr_= (T*)addr;
  for(int idx = 0; idx < size; ++idx)
  {
    constructor<T,P0>(addr_,p0);
    addr_ += 1;
  }
}
template<class T,class P0,class P1>
void constructor_array(T* addr,int size,P0 p0,P1 p1)
{
  T* addr_= (T*)addr;
  for(int idx = 0; idx < size; ++idx)
  {
    constructor<T,P0,P1>(addr_,p0,p1);
    addr_ += 1;
  }
}
template<class T,class P0,class P1,class P2>
void constructor_array(T* addr,int size,P0 p0,P1 p1,P2 p2)
{
  T* addr_= (T*)addr;
  for(int idx = 0; idx < size; ++idx)
  {
    constructor<T,P0,P1,P2>(addr_,p0,p1,p2);
    addr_ += 1;
  }
}
template<class T,class P0,class P1,class P2,class P3>
void constructor_array(T* addr,int size,P0 p0,P1 p1,P2 p2,P3 p3)
{
  T* addr_= (T*)addr;
  for(int idx = 0; idx < size; ++idx)
  {
    constructor<T,P0,P1,P2,P3>(addr_,p0,p1,p2,p3);
    addr_ += 1;
  }
}
template<class T,class P0,class P1,class P2,class P3,class P4>
void constructor_array(T* addr,int size,P0 p0,P1 p1,P2 p2,P3 p3,P4 p4)
{
  T* addr_= (T*)addr;
  for(int idx = 0; idx < size; ++idx)
  {
    constructor<T,P0,P1,P2,P3,P4>(addr_,p0,p1,p2,p3,p4);
    addr_ += 1;
  }
}
}

#endif//__CONSTRUCTOR_IN_PLACE__
