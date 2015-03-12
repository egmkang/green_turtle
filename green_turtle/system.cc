#include "system.h"
#include <sys/types.h>
#include <sys/stat.h>

using namespace green_turtle;

uint64_t System::GetFileModifyTime(const std::string& file_name) {
  struct stat file_stat = {0};
  int32_t result = ::stat(file_name.c_str(), &file_stat);

  if (-1 == result) {
    file_stat.st_mtim.tv_sec = 0;
  }

  return file_stat.st_mtim.tv_sec;
}
