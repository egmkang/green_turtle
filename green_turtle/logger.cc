#include "logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

Logger::Logger(const char* filename, const char* link_name) :
    fd_(0),
    size_(0),
    file_name_(filename),
    fd_backup_(0)
{
  if(link_name) link_name_ = link_name;
  fd_ = ::open(file_name_.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
  assert(fd_ && "open file error");
  off_t off = ::lseek(fd_, 0L, SEEK_END);
  size_ = off;
  CreateLink();
}

Logger::~Logger()
{
  //std::lock_guard<std::mutex> guard(lock_);
  //::close(fd_);
}

void Logger::CreateLink()
{
  if(file_name_ != link_name_ && link_name_.size())
  {
    remove(link_name_.c_str());
    std::string link = file_name_;
    std::string::size_type pos = link.find_last_of('/');
    if(pos != std::string::npos)
    {
      link = link.substr(pos + 1, link.size() - pos - 1);
    }
    symlink(link.c_str(), link_name_.c_str());
  }
}

void Logger::ChangeLoggerFile(const char *new_file)
{
  if(file_name_ == new_file) return;
  std::lock_guard<std::mutex> guard(lock_);
  if(fd_backup_) close(fd_backup_);
  fd_backup_ = fd_;
  fd_ = ::open(new_file, O_RDWR | O_CREAT | O_APPEND, 0644);
  assert(fd_ && "open file error");
  file_name_ = new_file;
  off_t off = ::lseek(fd_, 0L, SEEK_END);
  size_ = off;
  CreateLink();
}

static const char* LOGGER_LEVEL[] =
{
  "[INFO ]",
  "[DEBUG]",
  "[TRACE]",
  "[WARN ]",
  "[ERROR]",
  "[FATAL]"
};
enum
{
  kLoggerLevel_Info   =   0,
  kLoggerLevel_Debug  =   1,
  kLoggerLevel_Trace  =   2,
  kLoggerLevel_Warn   =   3,
  kLoggerLevel_Error  =   4,
  kLoggerLevel_Fatal  =   5,
};
enum
{
  kLoggerMessage_MaxSize  =   4096,
};
#define __FORMAT_MESSAGE__(level)       \
    va_list ap;                         \
    va_start(ap, pattern);              \
    FormatMessage(level, pattern, ap);  \
    va_end(ap);

inline void Logger::FormatMessage(int level, const char *pattern, va_list ap, const char *prefix /*= NULL*/)
{
  int size = 0;
  char msg[kLoggerMessage_MaxSize + 1];
  int msglen = kLoggerMessage_MaxSize;

  time_t now = time(NULL);
  struct tm tm_now;
  localtime_r( &now, &tm_now );
  size += snprintf(msg + size, msglen - size,
                   "%02d:%02d:%02d %s ",
                   tm_now.tm_hour,
                   tm_now.tm_min,
                   tm_now.tm_sec,
                   LOGGER_LEVEL[level]);
  if(prefix)
  {
    size += snprintf(msg + size, msglen - size, 
                     "%s ", prefix);
  }
  size += vsnprintf(msg + size, msglen-size-1, pattern, ap);
  LogMessage(msg, size);
}

void Logger::Debug(const char *pattern, ...)
{
  __FORMAT_MESSAGE__(kLoggerLevel_Debug);
}

void Logger::Fatal(const char *pattern, ...)
{
  __FORMAT_MESSAGE__(kLoggerLevel_Fatal);
}

void Logger::Error(const char *pattern, ...)
{
  __FORMAT_MESSAGE__(kLoggerLevel_Error);
}

void Logger::Warn (const char *pattern, ...)
{
  __FORMAT_MESSAGE__(kLoggerLevel_Warn);
}

void Logger::Info (const char *pattern, ...)
{
  __FORMAT_MESSAGE__(kLoggerLevel_Info);
}

void Logger::Trace(const char *pattern, ...)
{
  __FORMAT_MESSAGE__(kLoggerLevel_Trace);
}

void Logger::VDebug(const char *prefix, const char *pattern, va_list ap)
{
  FormatMessage(kLoggerLevel_Debug, pattern, ap, prefix);
}

void Logger::VFatal(const char *prefix, const char *pattern, va_list ap)
{
  FormatMessage(kLoggerLevel_Fatal, pattern, ap, prefix);
}

void Logger::VError(const char *prefix, const char *pattern, va_list ap)
{
  FormatMessage(kLoggerLevel_Error, pattern, ap, prefix);
}

void Logger::VWarn (const char *prefix, const char *pattern, va_list ap)
{
  FormatMessage(kLoggerLevel_Warn, pattern, ap, prefix);
}

void Logger::VInfo (const char *prefix, const char *pattern, va_list ap)
{
  FormatMessage(kLoggerLevel_Info, pattern, ap, prefix);
}

void Logger::VTrace(const char *prefix, const char *pattern, va_list ap)
{
  FormatMessage(kLoggerLevel_Trace, pattern, ap, prefix);
}

void Logger::LogMessage(char *str, size_t len)
{
  str[len] = '\n';
  ::write(fd_, str, len+1);
  size_ += len;
}
