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
#include <stdlib.h>
#include <format.h>

#define __SHORT_FILE__ \
  strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

namespace green_turtle {

class LogFile;

enum {
  kLoggerLevel_None = -1,
  kLoggerLevel_Info = 0,
  kLoggerLevel_Debug = 1,
  kLoggerLevel_Trace = 2,
  kLoggerLevel_Warn = 3,
  kLoggerLevel_Error = 4,
  kLoggerLevel_Fatal = 5,
};

class Logger {
  enum {
    kLogEntryMaxLength = 4 * 1024,
  };

 public:
  Logger(const char *file_name, const char *link_name,
         int8_t log_level = kLoggerLevel_Debug);
  ~Logger();

  void ChangeLoggerFile(const char *new_file);

  void VDebug(const std::string &prefix, const char *pattern, va_list ap)
      __attribute__((__format__(__printf__, 3, 0)));
  void VFatal(const std::string &prefix, const char *pattern, va_list ap)
      __attribute__((__format__(__printf__, 3, 0)));
  void VError(const std::string &prefix, const char *pattern, va_list ap)
      __attribute__((__format__(__printf__, 3, 0)));
  void VWarn(const std::string &prefix, const char *pattern, va_list ap)
      __attribute__((__format__(__printf__, 3, 0)));
  void VInfo(const std::string &prefix, const char *pattern, va_list ap)
      __attribute__((__format__(__printf__, 3, 0)));
  void VTrace(const std::string &prefix, const char *pattern, va_list ap)
      __attribute__((__format__(__printf__, 3, 0)));

  template <typename... Tn>
  void Log(int8_t level, Tn &&... vn) {
    char array[kLogEntryMaxLength];
    int32_t header = GenerateLogHeader(array, level);
    int32_t length =
        Format(array + header, sizeof(array) - header, std::forward<Tn>(vn)...);
    assert(length > 0);
    if (length > 0) this->LogMessage(array, header + length);
  }

  template <typename... Tn>
  void LogDebug(Tn &&... vn) {
    Log(kLoggerLevel_Debug, std::forward<Tn>(vn)...);
  }

  template <typename... Tn>
  void LogInfo(Tn &&... vn) {
    Log(kLoggerLevel_Info, std::forward<Tn>(vn)...);
  }

  template <typename... Tn>
  void LogTrace(Tn &&... vn) {
    Log(kLoggerLevel_Trace, std::forward<Tn>(vn)...);
  }

  template <typename... Tn>
  void LogWarn(Tn &&... vn) {
    Log(kLoggerLevel_Warn, std::forward<Tn>(vn)...);
  }

  template <typename... Tn>
  void LogError(Tn &&... vn) {
    Log(kLoggerLevel_Error, std::forward<Tn>(vn)...);
  }

  template <typename... Tn>
  void LogFatal(Tn &&... vn) {
    Log(kLoggerLevel_Fatal, std::forward<Tn>(vn)...);
    this->Flush();
    abort();
  }

  template <typename... Tn>
  void Write(Tn &&... vn) {
    Log(kLoggerLevel_None, std::forward<Tn>(vn)...);
  }

  void Flush();
  size_t GetWrittenSize() const { return size_; }
  int8_t log_level() const { return log_level_; }
  void log_level(int8_t log_level) {
    assert(log_level >= kLoggerLevel_Info && log_level <= kLoggerLevel_Fatal);
    log_level_ = log_level;
  }

 public:
  //!!!
  //NOT THREAD SAFE
  //!!!
  static Logger *Default();
  static Logger *InitDefaultLogger(const char *file_name, const char *link_name,
                                   int8_t log_level = kLoggerLevel_Debug);

 private:
  void FormatMessage(int level, const char *pattern, va_list ap,
                     const std::string &prefix);
  void LogMessage(char *str, size_t len);
  void CreateLink();
  int32_t GenerateLogHeader(char *array, int8_t level);

 private:
  std::mutex lock_;
  std::unique_ptr<LogFile> file_;
  std::unique_ptr<LogFile> backup_file_;
  long long size_;
  int8_t log_level_;

  std::string file_name_;
  std::string link_name_;
};

}


#define INFO_LOG(LOGGER) if (LOGGER && LOGGER->log_level() <= green_turtle::kLoggerLevel_Info) LOGGER->LogInfo
#define DEBUG_LOG(LOGGER) if (LOGGER && LOGGER->log_level() <= green_turtle::kLoggerLevel_Debug) LOGGER->LogDebug
#define TRACE_LOG(LOGGER) if (LOGGER && LOGGER->log_level() <= green_turtle::kLoggerLevel_Trace) LOGGER->LogTrace
#define WARN_LOG(LOGGER) if (LOGGER && LOGGER->log_level() <= green_turtle::kLoggerLevel_Warn) LOGGER->LogWarn
#define ERROR_LOG(LOGGER) if (LOGGER && LOGGER->log_level() <= green_turtle::kLoggerLevel_Error) LOGGER->LogError
#define FATAL_LOG(LOGGER) if (LOGGER && LOGGER->log_level() <= green_turtle::kLoggerLevel_Fatal) LOGGER->LogFatal

#endif
