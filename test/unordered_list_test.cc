#include "../collections/unordered_list.h"
#include "../collections/hash_table.h"
#include <stdint.h>
#include <iostream>

using namespace green_turtle::collections;

struct for_each_list
{
  bool operator ()(uint32_t &v)
  {
    std::cout << v << std::endl;
    return true;
  }
};
struct for_each_hash_table
{
  bool operator ()(std::pair<const int, int> &v)
  {
    std::cout << "key:" << v.first 
        << ", value:" << v.second << std::endl;
    return true;
  }
};

int main()
{
  std::cout << "unordered_list test" << std::endl;

  unordered_list<uint32_t> test_container;
  test_container.set_deleted((uint32_t)-1);
  test_container.insert(100);
  test_container.insert(1212);
  for_each_list e;
  test_container.for_each(e);

  std::cout << "hash_table test" << std::endl;

  hash_map<int,int> table;

  table.insert(1,1);
  table.insert(2,2);
  
  for_each_hash_table e1;

  table.for_each(e1);
  
  return 0;
}
