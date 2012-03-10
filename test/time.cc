#include "../util/systime.h"
#include <stdint.h>
#include <iostream>

using namespace green_turtle::util;

int main()
{
  SysTime::update();
  std::cout << SysTime::getSeconds() << std::endl;
  std::cout << SysTime::getMilliSeconds() << std::endl;
  return 0;
}
