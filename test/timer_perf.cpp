#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>
#include <random>
#include <stdlib.h>
#include "pq_timer.h"
#include <net/timer_queue.h>
#include <net/timer.h>

#include <time.h>
#include <sys/time.h>

using namespace green_turtle;
using namespace green_turtle::net;

void hr_init(){}
uint64_t hr_time()
{
  struct timeval val;
  ::gettimeofday(&val, nullptr);
  return val.tv_sec * 1000 + val.tv_usec / 1000;
}

uint64_t now;
void do_tick_loop(uint64_t maxTime)
{
  for (now = 0; now < maxTime + 16; now += 16)
  {
    timer_update(now, 16);
  }
}

uint64_t counter = 0;
void tick_timer_callback(pq_time_tick_t* timer, uint64_t data)
{
  timer_destroy(timer);
  ++ counter;
  if (now < data || now > data + 16) {
    printf("Assertion failed: Timer is not expiring in this tick.\n");
  }
}

uint64_t startStamp;
void start_test()
{
  startStamp = hr_time();
}

void end_test(const char* name)
{
  printf("%s: %lf\n", name, (hr_time() - startStamp) / 1000.0f);
}

void normal_test(uint64_t maxTime, uint64_t timerCount)
{
  counter = 0;

  std::mt19937 eng;
  std::uniform_int_distribution<> gen(0, maxTime);
  for (uint64_t i = 0; i < timerCount; i++)
  {
    uint64_t val = gen(eng);
    timer_create(val, tick_timer_callback, val);
  }

  do_tick_loop(maxTime);

  if (counter < timerCount) {
    printf("Assertion failed: Not all callback invoked. %" PRIu64 "/%" PRIu64 "\n", counter, timerCount);
  }
}

struct MyTimeTick : public Timer
{
  void OnTimeOut(uint64_t current_time)
  {
    (void)current_time;
    ++counter;
    delete this;
  }
};

TimerQueue *queue;

void do_wheel_loop(uint64_t maxTime)
{
  for (now = 0; now <= maxTime + 16; now += 16)
  {
    queue->Update(now);
  }
}

void timer_wheel_test(uint64_t maxTime, uint64_t timerCount)
{
  counter = 0;
  queue = new TimerQueue(2<<13, 16);
  queue->Update(0);

  std::mt19937 eng;
  std::uniform_int_distribution<> gen(0, maxTime);
  for (uint64_t i = 0; i < timerCount; i++)
  {
    uint64_t val = gen(eng);
    MyTimeTick *ptick = new MyTimeTick;
    queue->ScheduleTimer(ptick, val);
  }

  do_wheel_loop(maxTime);

  if (counter < timerCount) {
    printf("Assertion failed: Not all callback invoked. %" PRIu64 "/%" PRIu64 "\n", counter, timerCount);
  }
}

int main()
{
  hr_init();
  start_test();
  normal_test(1000000L, 1000000L);
  end_test("normal_test(1000000L, 1000000L)");

  start_test();
  timer_wheel_test(1000000L, 1000000L);
  end_test("timer_wheel_test(1000000L, 1000000L)");

  return 0;
}
