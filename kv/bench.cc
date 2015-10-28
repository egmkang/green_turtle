#include <sophia.h>
#include <vector>
#include <thread>
#include <random>
#include <stdint.h>

const int32_t key_dist = 5000000;
int32_t kThreadCount = 4;
int32_t kRandomWriteCount = 1000000;
void *env = nullptr;
std::random_device device;

void random_write() {
  void *db = sp_getobject(env, "db.test");
  sp_open(db);
  std::mt19937 mt(device());

  for (int32_t i = 0; i < kRandomWriteCount; ++i) {
    void * o = sp_object(db);
    int32_t key = mt() % key_dist;
    int32_t value = mt();
    static const char data[] =
        "0123456789qwertyuiooasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM!@#$%^&*"
        "()";
    sp_setstring(o, "key", &key, sizeof(key));
    sp_setstring(o, "value", &data, value % sizeof(data));
    int32_t result = sp_set(db, o);
    if (result == -1) abort();
  }

  sp_destroy(db);
}

void benchmark() {
  env = sp_env();
  sp_setstring(env, "sophia.path", "./", 0);
  sp_setstring(env, "db", "test", 0);
  sp_open(env);

  std::vector<std::thread*> threads;
  threads.resize(kThreadCount, nullptr);
  for (int32_t i = 0; i < kThreadCount; ++i) {
    threads[i] = new std::thread([]() { random_write(); });
  }

  for (int32_t i = 0; i < kThreadCount; ++i) {
    threads[i]->join();
    delete threads[i];
  }
}

