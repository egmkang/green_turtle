#include <gtest/gtest.h>
#include <vector_map.h>

using namespace green_turtle;


TEST(VectorMap, Emplace) {
  VectorMap<int, int> v;
  EXPECT_EQ(0ul, v.size());

  v.emplace(1, 2);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(1ul, v.size());

  v.emplace(100, 3);
  EXPECT_EQ(v[100], 3);
  EXPECT_EQ(2ul, v.size());

  v.emplace(1, 10);
  EXPECT_EQ(v[1], 10);
  EXPECT_EQ(2ul, v.size());

  //1,100
  v.emplace(0, 100);
  EXPECT_EQ(v[0], 100);
  EXPECT_EQ(v.begin()->first, 0);
  EXPECT_EQ((v.begin()+1)->first, 1);
  EXPECT_EQ((v.begin()+2)->first, 100);

  //0,1,100
  v.emplace(3, 20);
  EXPECT_EQ(v[3], 20);
  EXPECT_EQ(v.begin()->first, 0);
  EXPECT_EQ((v.begin()+1)->first, 1);
  EXPECT_EQ((v.begin()+2)->first, 3);
  EXPECT_EQ((v.begin()+3)->first, 100);

  //0,1,3,100
  v.emplace(1000, 101);
  EXPECT_EQ(v[1000], 101);
  EXPECT_EQ(v.begin()->first, 0);
  EXPECT_EQ((v.begin()+1)->first, 1);
  EXPECT_EQ((v.begin()+2)->first, 3);
  EXPECT_EQ((v.begin()+3)->first, 100);
  EXPECT_EQ((v.begin()+4)->first, 1000);
}

TEST(VectorMap, Clear) {
  VectorMap<int, int> v;
  EXPECT_EQ(v.begin(), v.end());

  v.emplace(1, 1);
  EXPECT_NE(v.begin(), v.end());

  v.clear();
  EXPECT_EQ(v.begin(), v.end());
  EXPECT_EQ(0ul, v.size());
}

TEST(VectorMap, Erase) {
  VectorMap<int, int> v;
  v.emplace(1, 2);
  v.emplace(2, 3);
  v.emplace(3, 4);

  v.erase(0);
  EXPECT_EQ(3ul, v.size());

  v.erase(1);
  EXPECT_EQ(2ul, v.size());
  EXPECT_EQ(v.begin()->first, 2);

  v.emplace(1, 2);
  v.erase(2);
  EXPECT_EQ(v.find(2), v.end());

  v.emplace(2, 3);
  v.erase(3);
  EXPECT_EQ((--v.end())->first, 2);
}

TEST(VectorMap, Get) {
  VectorMap<int, int> v;
  v.emplace(1, 2);
  v.emplace(2, 3);
  v.emplace(3, 4);

  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
}
