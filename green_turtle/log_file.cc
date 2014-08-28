#include <log_file.h>

using namespace green_turtle;

LogFile::LogFile(const std::string& file_name, int buffer_size):
  file_(fopen(file_name.c_str(), "a+"), green_turtle::kFileDeleter)
  , buffer_(new char[buffer_size])
  , file_name_(file_name)
  , offset_(0)
{
  if(file_)
  {
    offset_ = fseek(file_.get(), 0, SEEK_END);
    setvbuf(file_.get(), buffer_.get(), _IOFBF, buffer_size);
  }
}

int LogFile::Write(const char *str, int len)
{
  if(file_)
  {
    int size = fwrite(str, 1, len, file_.get());
    offset_ += size;
    return size;
  }
  return 0;
}


void LogFile::Flush()
{
  if(file_) fflush(file_.get());
}
