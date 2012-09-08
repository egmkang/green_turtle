#include <systime.h>
#include <stdint.h>
#include <iostream>

using namespace green_turtle;

int main()
{
  SysTime::Update();
  std::cout << SysTime::GetSeconds() << std::endl;
  std::cout << SysTime::GetMilliSeconds() << std::endl;
  return 0;
}
