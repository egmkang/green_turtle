#include <format.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <system.h>

using namespace green_turtle;
using green_turtle::Format;

const int kMaxCount = 1000000;

void benchmark() {
  char array[1024];
  int32_t tid = green_turtle::System::GetThreadID();

  time_t begin, end;

  begin = green_turtle::System::GetMilliSeconds();
  for(int i = 0; i < kMaxCount; ++i)
  {
    snprintf(array, sizeof(array), "thread id:%d write log, random_num:%d\n", tid, rand());
  }
  end = green_turtle::System::GetMilliSeconds();

  std::cout << "snprintf:" << end - begin << "ms" << std::endl;

  begin = green_turtle::System::GetMilliSeconds();
  for(int i = 0; i < kMaxCount; ++i)
  {
    green_turtle::Format(array, sizeof(array), "thread id:", tid, " write log, random_num:", rand());
  }
  end = green_turtle::System::GetMilliSeconds();

  std::cout << "Format:" << end - begin << "ms" << std::endl;
}

int main() {
  srand(time(NULL));
  char buffer[1000] = {0};
  green_turtle::Format(buffer, sizeof(buffer), 1, "\n", "2u", "\n",
                       std::string("3ul"), "\n", uint64_t(4), int64_t(5678),
                       "\n", 2.144534535);
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, sizeof(buffer));
  std::cout << buffer << std::endl;

  std::cout << green_turtle::Format(buffer, 9, "123456789") << std::endl;
  std::cout << green_turtle::Format(buffer, 8, "123456789") << std::endl;

  const char array[] = {"123456789"};
  StringSlice slice(array);
  std::string str(array);

  green_turtle::Format(buffer, 9, Hex(0x1234, 4));
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, 9, Hex(0x1234ul, 4));
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, 30, Hex(std::string("1234"), 4));
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, 30, Hex("5678", 4));
  std::cout << buffer << std::endl;

  str = "9101";
  green_turtle::Format(buffer, 30, Hex(str, 4));
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, 30, Hex(slice, 4));
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, 30, Hex(StringSlice(array), 4));
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, 30, green_turtle::Align(kAlignUpper | kAlignCenter, "abCdefgh", 20, '*'));
  std::cout << buffer << std::endl;


  benchmark();

  return 0;
}
