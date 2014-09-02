#include <format.h>
#include <iostream>

int main()
{
  char buffer[1000] = {0};
  green_turtle::Format(buffer, sizeof(buffer), 1, "\n", "2u", "\n",
                       std::string("3ul"), "\n", uint64_t(4), int64_t(5678),
                       "\n", 2.144534535);
  std::cout << buffer << std::endl;

  green_turtle::Format(buffer, sizeof(buffer));
  std::cout << buffer << std::endl;

  std::cout << green_turtle::Format(buffer, 9, "123456789") << std::endl;
  std::cout << green_turtle::Format(buffer, 8, "123456789") << std::endl;


  return 0;
}
