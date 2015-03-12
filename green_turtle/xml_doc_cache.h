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

#ifndef __XML_DOC_CACHE_H__
#define __XML_DOC_CACHE_H__
#include <pugixml.hpp>
#include <singleton.h>
#include <unordered_map>
#include <memory>
#include <functional>
#include <assert.h>
#include <system.h>

namespace green_turtle {

struct XmlDocInfo {
  std::string file_name;
  std::shared_ptr<pugi::xml_document> shared_doc;
  uint64_t modify_time;
  std::function<bool(void)> parse_file;
};

class XmlDocCache : public Singleton<XmlDocCache> {
 public:
  typedef std::pair<bool, pugi::xml_parse_status> parse_result;
  typedef pugi::xml_node node_type;

 public:
  parse_result LoadFile(const std::string& file_name, std::function<bool(void)> parse_file) {
    if (container_.find(file_name) != container_.end()) return std::make_pair(true, pugi::status_ok);

    std::shared_ptr<pugi::xml_document> doc = std::make_shared<pugi::xml_document>();
    pugi::xml_parse_result result = doc->load_file(file_name.c_str());
    if (result.status == pugi::status_ok) {
      XmlDocInfo info = {file_name, doc, System::GetFileModifyTime(file_name), parse_file};
      container_[file_name] = info;
      if (!parse_file()) {
        container_.erase(file_name);
        return std::make_pair(false, pugi::status_ok);
      }
    }
    return std::make_pair(true, result.status);
  }

  parse_result ReloadFile(bool need_abort = false) {
    for (container_type::iterator iter = container_.begin();
         iter != container_.end(); ++iter) {
      const std::string& file_name = iter->first;
      XmlDocInfo& doc_info = iter->second;
      uint64_t file_modify_time = System::GetFileModifyTime(file_name);
      if (file_modify_time != doc_info.modify_time) {
        std::shared_ptr<pugi::xml_document> doc =
            std::make_shared<pugi::xml_document>();
        pugi::xml_parse_result result = doc->load_file(file_name.c_str());
        if (result.status == pugi::status_ok) {
          doc_info.shared_doc = doc;

          if (!doc_info.parse_file() && need_abort) {
            return std::make_pair(false, result.status);
          }
        }
      }
    }
    return std::make_pair(true, pugi::status_ok);
  }

  template <typename T>
  bool ForEach(const std::string& file_name, const char* query, T callback) const {
    assert(query);
    container_type::const_iterator file_iter = container_.find(file_name);
    if (file_iter == container_.end()) return false;

    pugi::xpath_node_set nodes = file_iter->second.shared_doc->select_nodes(query);
    for (pugi::xpath_node_set::const_iterator iter = nodes.begin();
         iter != nodes.end(); ++iter) {
      node_type node = (*iter).node();
      if (!callback(node)) return false;
    }
    return true;
  }

  template <typename T>
  bool First(const std::string& file_name, const char* query, T callback) const {
    assert(query);
    container_type::const_iterator file_iter = container_.find(file_name);
    if (file_iter == container_.end()) return false;

    pugi::xpath_node_set nodes = file_iter->second.shared_doc->select_nodes(query);
    pugi::xpath_node_set::const_iterator iter = nodes.begin();
    if (iter != nodes.end()) {
      node_type node = (*iter).node();
      if (!callback(node)) return false;
    }
    return true;
  }

  void Clear() { container_.clear(); }
  void Clear(const std::string& file_name) { container_.erase(file_name); }

 private:
  typedef std::unordered_map<std::string, XmlDocInfo> container_type;

  container_type container_;
};
}

#endif
