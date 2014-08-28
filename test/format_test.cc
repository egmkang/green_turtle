#include <format.h>
#include <iostream>

int main()
{
  char buffer[1000] = {0};
  green_turtle::format(buffer, sizeof(buffer), 1, "\n", "2u", "\n",
                       std::string("3ul"), "\n", uint64_t(4), int64_t(5678),
                       "\n", 2.144534535);
  std::cout << buffer << std::endl;
  return 0;
}
