//Copyright 2013, egmkang wang.
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
// Author: egmkang@gmail.com (egmkang wang)
#ifndef __ORDERED_LIST_H__
#define __ORDERED_LIST_H__
#include <vector>
#include <cstddef>
#include <utils.h>

namespace green_turtle{

template<typename T>
class ordered_list
{
  public:
    typedef size_t      iterator;

    ordered_list() : deleted_(), set_deleted_(false)
    {
        list_.reserve(16);
        free_list_.reserve(16);
    }
    iterator insert(const T& v)
    {
        size_t idx = 0;
        if(!free_list_.empty())
        {
            idx = free_list_.back();
            free_list_.pop_back();
            list_[idx] = v;
        }
        else
        {
            idx = list_.size();
            list_.push_back(v);
        }
        return idx;
    }
    void erase(iterator idx)
    {
        list_[idx] = deleted_;
        free_list_.push_back(idx);
    }
    template<typename Fn>
        void for_each(Fn fn)
        {
            for(iterator iter = 0; iter < list_.size(); ++iter)
            {
                if(list_[iter] == deleted_) continue;
                if(!fn(list_[iter], iter)) break;
            }
        }
    inline T get(size_t idx) const { return list_[idx] == deleted_ ? T() : list_[idx]; }
    inline bool is_deleted(size_t idx) const {return list_[idx] == deleted_; }
    inline void set_deleted(const T& v) { deleted_ = v; }
  private:
   std::vector<T>       list_;
   std::vector<size_t>  free_list_;
   T                    deleted_;
   bool                 set_deleted_;
};

}

#endif
