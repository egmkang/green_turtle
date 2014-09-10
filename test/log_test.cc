#include <logger.h>
#include <atomic>
#include <thread>
#include <stdlib.h>
#include <system.h>
#include <iostream>
#include <system.h>

const int kThreadCount = 1;
const long kMaxLogCount = 1000000;

int main() {
  green_turtle::Logger logger("test.txt.1", "test.txt");
  std::atomic_long count(0);
  std::thread *threads[kThreadCount];

  size_t begin = green_turtle::System::GetMilliSeconds();

  for (int i = 0; i < kThreadCount; ++i) {
    threads[i] = new std::thread([&]() {
      int32_t tid = green_turtle::System::GetThreadID();
      while (true) {
        DEBUG_LOG(logger)("thread ", tid, " write log, random_num ", rand());
        if (++count > kMaxLogCount) {
          break;
        }
      }
    });
  }

  while (count < kMaxLogCount) {
    green_turtle::System::Yield(20);
    logger.Flush();
  }

  for (int i = 0; i < kThreadCount; ++i) {
    threads[i]->join();
    delete threads[i];
  }

  time_t end = green_turtle::System::GetMilliSeconds();
  std::cout << int(double(kMaxLogCount) / (end - begin) * 1000) << std::endl;

  return 0;
}
