#include <unordered_list.h>
#include <stdint.h>
#include <iostream>

using namespace green_turtle;

struct for_each_list
{
  bool operator ()(uint32_t &v,size_t idx)
  {
    std::cout << v << ", " << idx << std::endl;
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
  test_container.insert(1);
  test_container.insert(3);
  test_container.insert(4);
  test_container.insert(5);
  test_container.insert(6);
  test_container.insert(7);
  test_container.insert(8);
  test_container.insert(9);

  for_each_list e;
  test_container.for_each(e);

  std::cout << std::endl;

  test_container.erase(0);
  test_container.insert(9);

  test_container.for_each(e);

  test_container.clear();
  test_container.insert(1);
  test_container.insert(2);
  test_container.insert(3);
  test_container.erase(2);
  test_container.insert(3);

  test_container.for_each(e);

  std::cout << "hash_table test" << std::endl;

  return 0;
}
