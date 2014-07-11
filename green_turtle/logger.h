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

#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <stdarg.h>
#include <string>
#include <mutex>
#include <memory>
#include <string>

#define __SHORT_FILE__  strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

namespace green_turtle{

class LogFile;

class Logger
{
 public:
  Logger(const char* file_name, const char *link_name);
  ~Logger();

  void ChangeLoggerFile(const char*new_file);

  void Debug(const char *pattern, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
  void Fatal(const char *pattern, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
  void Error(const char *pattern, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
  void Warn (const char *pattern, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
  void Info (const char *pattern, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
  void Trace(const char *pattern, ...) __attribute__ ((__format__ (__printf__, 2, 3)));

  void VDebug(const std::string& prefix, const char *pattern, va_list ap) __attribute__ ((__format__ (__printf__, 3, 0)));
  void VFatal(const std::string& prefix, const char *pattern, va_list ap) __attribute__ ((__format__ (__printf__, 3, 0)));
  void VError(const std::string& prefix, const char *pattern, va_list ap) __attribute__ ((__format__ (__printf__, 3, 0)));
  void VWarn (const std::string& prefix, const char *pattern, va_list ap) __attribute__ ((__format__ (__printf__, 3, 0)));
  void VInfo (const std::string& prefix, const char *pattern, va_list ap) __attribute__ ((__format__ (__printf__, 3, 0)));
  void VTrace(const std::string& prefix, const char *pattern, va_list ap) __attribute__ ((__format__ (__printf__, 3, 0)));

  void Flush();
  size_t GetWrittenSize() const { return size_; }
 private:
  void FormatMessage(int level, const char *pattern, va_list ap, const std::string& prefix);
  void LogMessage(char *str, size_t len);
  void CreateLink();
 private:
  std::mutex  lock_;
  std::unique_ptr<LogFile> file_;
  std::unique_ptr<LogFile> backup_file_;
  long long size_;

  std::string file_name_;
  std::string link_name_;
};

}

#endif
