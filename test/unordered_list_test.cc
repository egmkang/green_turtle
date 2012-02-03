#include "../collections/unordered_list.h"
#include <stdint.h>
#include <iostream>

using green_turtle::collections::unordered_list;

struct for_each_list
{
  bool operator ()(uint32_t &v)
  {
    std::cout << v << std::endl;
    return true;
  }
};

int main()
{
  unordered_list<uint32_t> test_container;
  test_container.set_deleted((uint32_t)-1);
  test_container.insert(100);
  test_container.insert(1212);
  for_each_list e;
  test_container.for_each(e);
  return 0;
}
