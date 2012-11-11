#include <system.h>
#include <stdint.h>
#include <iostream>

using namespace green_turtle;

int main()
{
  System::UpdateTime();
  std::cout << System::GetSeconds() << std::endl;
  std::cout << System::GetMilliSeconds() << std::endl;
  return 0;
}
