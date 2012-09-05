#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <thread>
#include <message_queue.h>

#define MAX_TIMES       10000000

green_turtle::MessageQueue<int*> queue;

static long empty_times = 0;
static long full_times = 0;

static int write_ = 0;
static int read_ = 0;

void producer()
{
  for(int i = 0; i < MAX_TIMES; ++i)
  {
    int *ptr = new int[rand()%200 + 10];
    int value = write_++;
    ptr[0] = value;
    while(!queue.Push(ptr))
    {
      ++full_times;
      usleep(0);
    }
  }
}

int main()
{
  struct timeval tv1, tv2;

  gettimeofday(&tv1, NULL);

  std::thread thrd(&producer);

  for(int i = 0; i < MAX_TIMES;)
  {
    int *ptr = 0;
    while(!queue.Pop(ptr))
    {
      ++empty_times;
      usleep(0);
    }
    int value = read_++;
    if(ptr[0] != value)
      assert(false);
    delete[] ptr;
    ++i;
  }
  thrd.join();

  gettimeofday(&tv2, NULL);

  long long  t1 = tv1.tv_sec*1000000 + tv1.tv_usec;
  long long  t2 = tv2.tv_sec*1000000 + tv2.tv_usec;
  printf("Result - %lld\n", (t2 - t1)/1000);
  printf("EmptyTimes : %ld, FullTimes : %ld\n",empty_times, full_times);
  return 0;
}
