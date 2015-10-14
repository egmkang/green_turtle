#include <mysqlcxx.h>
#include <system.h>
#include <iostream>
#include <stdint.h>
#include <atomic>
#include <thread>

using namespace std;
using namespace green_turtle;

const int kThreadCount = 16;
int64_t begin_time = 0;

void test(std::atomic_int& finished_thread) {
  MySqlConnection *conn = new MySqlConnection;
  conn->Connect("192.168.16.151", 3306, "root", "1q2w3e", "zhanjian");
  int32_t count = 0;
  for (int32_t i = 0; i < 10000; ++i) {
    std::shared_ptr<ResultSet> result = conn->ExecSelect("select 1;");
    if (result->IsValid()) {
      count += result->at(0).to_int32();
    }
  }
  ++finished_thread;
  if (finished_thread == kThreadCount) {
    int64_t end_time = System::GetMilliSeconds();
    cout << "ThreadCount:" << kThreadCount << endl;
    cout << "SQL Count:" << kThreadCount * 10000 << endl;
    cout << "Time:" << double(end_time - begin_time) / (kThreadCount * 10000)
         << "ms" << endl;
  }
}

int main() {
  //MySqlConnection conn;
  //conn.Connect("192.168.16.151", 3306, "root", "1q2w3e", "zhanjian");

  begin_time = System::GetMilliSeconds();
  std::thread* thread_pool[kThreadCount];
  std::atomic_int finished_thread(0);

  for (int i = 0; i < kThreadCount; ++i) {
    thread_pool[i] = new std::thread([&]() {
      test(finished_thread);
    });
  }

  for (int i = 0; i < kThreadCount; ++i) {
    thread_pool[i]->join();
    delete thread_pool[i];
  }
  return 0;
}
