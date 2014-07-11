#include <ordered_list.h>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

using namespace green_turtle;

template<class T>
struct ForEach
{
  std::vector<T>& v;
  ForEach(std::vector<T>& v): v(v){}
  bool operator() (T& value, size_t )
  {
    v.push_back(value);
    return true;
  }
};

template<class T>
std::vector<T> ToVector(ordered_list<T>& list)
{
  std::vector<T> v;
  ForEach<T> for_each(v);
  list.for_each(for_each);
  return v;
}

TEST(UnOrderedList, Insert)
{
  std::vector<int> v = {1,23,2,4,5,6};
  ordered_list<int> list;
  EXPECT_EQ(ToVector(list), std::vector<int>());
  for(int i : v)
  {
    list.insert(i);
  }
  std::vector<int> copy = ToVector(list);
  EXPECT_EQ(copy, v);
}

TEST(UnOrderedList, Erase)
{
  std::vector<int> v = {1,23,2,4,5,6};
  ordered_list<int> list;
  list.set_deleted(-1);
  for(int i : v)
  {
    list.insert(i);
  }
  list.erase(1);
  EXPECT_EQ(list.is_deleted(1), true);
  EXPECT_EQ(ToVector(list), std::vector<int>({1,2,4,5,6}));
  list.erase(1);
  EXPECT_EQ(ToVector(list), std::vector<int>({1,2,4,5,6}));
  list.erase(5);
  EXPECT_EQ(ToVector(list), std::vector<int>({1,2,4,5}));
}
