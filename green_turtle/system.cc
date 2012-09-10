#include <sys/time.h>
#include <unistd.h>
#include "system.h"

using namespace green_turtle;

static timeval  time_val_;
static uint64_t time_millisecond_;
static tm       time_tm_;

void System::UpdateTime()
{
  gettimeofday(&time_val_,NULL);
  time_millisecond_ = time_val_.tv_sec * 1000 + time_val_.tv_usec / 1000;
  localtime_r(&time_val_.tv_sec,&time_tm_);
}

time_t System::GetSeconds()
{
  return time_val_.tv_sec;
}

uint64_t System::GetMilliSeconds()
{
  return time_millisecond_;
}

const tm& System::GetTime()
{
  return time_tm_;
}

int System::GetSecondsDiffDays(time_t s1, time_t s2)
{
  tm tm_1_;
  tm tm_2_;
  localtime_r(&s1,&tm_1_);
  localtime_r(&s2,&tm_2_);
  tm_1_.tm_sec = tm_1_.tm_min = tm_1_.tm_hour = 0;
  tm_2_.tm_sec = tm_2_.tm_min = tm_2_.tm_hour = 0;
  s1 = mktime(&tm_1_);
  s2 = mktime(&tm_2_);
  return (s2 - s1) / (3600 * 24);
}

void System::Yield(uint64_t milliSeconds)
{
  ::usleep(milliSeconds);
}

