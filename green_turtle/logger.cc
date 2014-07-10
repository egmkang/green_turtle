#include "logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <log_file.h>
#include <system.h>

using namespace green_turtle;

Logger::Logger(const char* filename, const char* link_name) :
    file_(new LogFile(filename))
    , size_(0)
    , file_name_(filename)
{
  if(link_name) link_name_ = link_name;
  assert(file_ && "open file error");
  size_ = file_->offset();
  CreateLink();
}

Logger::~Logger()
{
  file_->Flush();
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
    int result = symlink(link.c_str(), link_name_.c_str());
    (void)result;
  }
}

void Logger::ChangeLoggerFile(const char *new_file)
{
  if(file_name_ == new_file) return;
  std::unique_ptr<LogFile> file(new LogFile(new_file));
  if(file)
  {
    std::lock_guard<std::mutex> guard(lock_);
    this->file_->Flush();
    this->backup_file_ = std::move(this->file_);
    this->file_ = std::move(file);
    file_name_ = new_file;
    size_ = this->file_->offset();
    CreateLink();
  }
}

static char LOGGER_LEVEL[][9] =
{
  "[INFO ] ",
  "[DEBUG] ",
  "[TRACE] ",
  "[WARN ] ",
  "[ERROR] ",
  "[FATAL] "
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


__thread time_t t_time = 0;
__thread char t_time_str[8] = {0};

inline void Logger::FormatMessage(int level, const char *pattern, va_list ap, const char *prefix /*= NULL*/)
{
  int size = 0;
  char msg[kLoggerMessage_MaxSize + 1];
  int msglen = kLoggerMessage_MaxSize;

  time_t now = System::GetSeconds();
  if (now > t_time)
  {
    struct tm tm_now;
    localtime_r( &now, &tm_now );
    sprintf(t_time_str, "%02d:%02d:%02d",
        tm_now.tm_hour,
        tm_now.tm_min,
        tm_now.tm_sec);
    t_time = now;
  }
  *reinterpret_cast<uint64_t*>(msg) = *reinterpret_cast<uint64_t*>(t_time_str);
  msg[sizeof(t_time_str)] = ' ';
  size += sizeof(t_time_str) + 1;
  *reinterpret_cast<uint64_t*>(msg+size) = *reinterpret_cast<uint64_t*>(LOGGER_LEVEL[level]);
  size += sizeof(LOGGER_LEVEL[0]) - 1;

  if(prefix)
  {
    char *from = msg+size;
    size += (strncat(msg+size, prefix, msglen - size) - from);
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
  int write_size = file_->Write(str, len+1);
  size_ += write_size;
  assert(write_size == int(len)+1);
}

void Logger::Flush()
{
  if(file_) file_->Flush();
}
