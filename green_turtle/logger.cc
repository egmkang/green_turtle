#include "logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <log_file.h>
#include <system.h>
#include <stdio.h>

using namespace green_turtle;

static char kMilliSecondsString[1024][4] = {};

static inline void LazyInitMilliSecondsString() {
  if (kMilliSecondsString[0][0]) return;
  for (int32_t i = 0; i < 1024; ++i) {
    uint32_t base = i % 1000;
    kMilliSecondsString[i][3] = "0123456789"[base % 10];
    kMilliSecondsString[i][2] = "0123456789"[base / 10 % 10];
    kMilliSecondsString[i][1] = "0123456789"[base / 100 % 10];
    kMilliSecondsString[i][0] = '.';
  }
}

Logger::Logger(const char *filename, const char *link_name, int8_t log_level)
    : file_(new LogFile(filename)),
      size_(0),
      log_level_(log_level),
      file_name_(filename) {
  if (link_name) link_name_ = link_name;
  assert(file_ && "open file error");
  assert(log_level >= kLoggerLevel_Info && log_level <= kLoggerLevel_Fatal);
  size_ = file_->offset();
  CreateLink();
  LazyInitMilliSecondsString();
}

Logger::~Logger() { file_->Flush(); }

void Logger::CreateLink() {
  if (file_name_ != link_name_ && link_name_.size()) {
    remove(link_name_.c_str());
    std::string link = file_name_;
    std::string::size_type pos = link.find_last_of('/');
    if (pos != std::string::npos) {
      link = link.substr(pos + 1, link.size() - pos - 1);
    }
    int result = symlink(link.c_str(), link_name_.c_str());
    (void)result;
  }
}

void Logger::ChangeLoggerFile(const char *new_file) {
  if (file_name_ == new_file) return;
  std::unique_ptr<LogFile> file(new LogFile(new_file));
  if (file) {
    std::lock_guard<std::mutex> guard(lock_);
    this->file_->Flush();
    this->backup_file_ = std::move(this->file_);
    this->file_ = std::move(file);
    file_name_ = new_file;
    size_ = this->file_->offset();
    CreateLink();
  }
}

static char LOGGER_LEVEL[][9] = {"[INFO ] ", "[DEBUG] ", "[TRACE] ",
                                 "[WARN ] ", "[ERROR] ", "[FATAL] "};

const std::string kEmptyString = "";
__thread time_t t_time = 0;
__thread char t_time_str[9] = {0};

inline void Logger::FormatMessage(int level, const char *pattern, va_list ap,
                                  const std::string &prefix) {
  if (level < log_level()) return;

  int size = 0;
  char msg[kLogEntryMaxLength + 1];
  size = GenerateLogHeader(msg, level);

  if (!prefix.empty()) {
    memcpy(msg + size, prefix.c_str(), prefix.size());
    size += prefix.size();
  }
  size += vsnprintf(msg + size, sizeof(msg) - size - 2, pattern, ap);
  LogMessage(msg, size);
}

void Logger::VDebug(const std::string &prefix, const char *pattern,
                    va_list ap) {
  FormatMessage(kLoggerLevel_Debug, pattern, ap, prefix);
}

void Logger::VFatal(const std::string &prefix, const char *pattern,
                    va_list ap) {
  FormatMessage(kLoggerLevel_Fatal, pattern, ap, prefix);
  abort();
  this->Flush();
}

void Logger::VError(const std::string &prefix, const char *pattern,
                    va_list ap) {
  FormatMessage(kLoggerLevel_Error, pattern, ap, prefix);
}

void Logger::VWarn(const std::string &prefix, const char *pattern, va_list ap) {
  FormatMessage(kLoggerLevel_Warn, pattern, ap, prefix);
}

void Logger::VInfo(const std::string &prefix, const char *pattern, va_list ap) {
  FormatMessage(kLoggerLevel_Info, pattern, ap, prefix);
}

void Logger::VTrace(const std::string &prefix, const char *pattern,
                    va_list ap) {
  FormatMessage(kLoggerLevel_Trace, pattern, ap, prefix);
}

void Logger::LogMessage(char *str, size_t len) {
  str[len] = '\n';
  int write_size = file_->Write(str, len + 1);
  size_ += write_size;
  assert(write_size == int(len) + 1);
}

void Logger::Flush() {
  if (file_) file_->Flush();
}

int32_t Logger::GenerateLogHeader(char *array, int8_t level) {
  int size = 0;
  std::pair<time_t, time_t> current_time = System::GetCurrentTime();
  current_time.first = System::GetSeconds();
  if (current_time.first > t_time) {
    struct tm tm_now;
    localtime_r(&current_time.first, &tm_now);
    snprintf(t_time_str, sizeof(t_time_str), "%02d:%02d:%02d", tm_now.tm_hour, tm_now.tm_min,
            tm_now.tm_sec);
    t_time = current_time.first;
  }

  memcpy(array + size, t_time_str, sizeof(t_time_str) - 1);
  size += sizeof(t_time_str) - 1;

  memcpy(array + size, kMilliSecondsString[current_time.second % 1024], sizeof(uint32_t));
  size += sizeof(uint32_t);

  array[size] = ' ';
  size += 1;

  if (level >= kLoggerLevel_Info && level <= kLoggerLevel_Fatal) {
    memcpy(array + size, LOGGER_LEVEL[level], sizeof(LOGGER_LEVEL[0]));
    size += sizeof(LOGGER_LEVEL[0]) - 1;
  }

  return size;
}

static std::shared_ptr<Logger> DefaultLogger;

Logger *Logger::Default() {
  if (!DefaultLogger) DefaultLogger = std::make_shared<Logger>("./default_logger.log", nullptr, kLoggerLevel_Debug);
  return DefaultLogger.get();
}

Logger* Logger::InitDefaultLogger(const char *file_name, const char *link_name,
                       int8_t log_level/*= kLoggerLevel_Debug*/){
  DefaultLogger = std::make_shared<Logger>(file_name, link_name, log_level);
  return DefaultLogger.get();
}
