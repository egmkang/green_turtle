#include <gtest/gtest.h>
#include <ref_object.h>
#include <memory>

using namespace green_turtle;

struct ObjectA : RefObject
{
  ObjectA() : value(0) { }
  int value;
};

TEST(RefPtr, Set)
{
  RefPtr<ObjectA> ptr;
  EXPECT_EQ(nullptr, ptr);

  std::unique_ptr<ObjectA> p(new ObjectA);
  std::unique_ptr<ObjectA> p1(new ObjectA);
  p->value = 1000;
  p1->value = 100;

  ptr = p.get();
  EXPECT_EQ(1000, ptr->value);

  ptr = nullptr;
  EXPECT_EQ(nullptr, ptr);

  ptr = p1.get();
  EXPECT_EQ(100, ptr->value);
}

TEST(RefPtr, Const)
{
  std::unique_ptr<const ObjectA> p(new ObjectA);
  std::unique_ptr<ObjectA> p1(new ObjectA);

  RefPtr<const ObjectA> ptr;
  EXPECT_EQ(nullptr, ptr);

  ptr = p.get();
  EXPECT_EQ(0, ptr->value);

  ptr = nullptr;
  EXPECT_EQ(nullptr, ptr);

  ptr = p1.get();
  EXPECT_EQ(p1.get(), ptr);
}

TEST(RefPtr, Delete)
{
  RefPtr<ObjectA> ptr;
  ObjectA *p = new ObjectA;

  ptr = p;
  EXPECT_EQ(p, ptr);

  delete p; p = nullptr;

  EXPECT_EQ(p, ptr);

  p = new ObjectA;
  ptr = p;
  EXPECT_EQ(p, ptr);
  delete p;
}
TEST(RefPtr, Move)
{
  {
    RefPtr<ObjectA> ptr;
    ObjectA *p = new ObjectA;
    ptr = p;

    RefPtr<ObjectA> pMove(std::move(ptr));
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(pMove.Get(), p);
  }
  {
    RefPtr<ObjectA> ptr;
    ObjectA *p = new ObjectA;
    ptr = p;

    RefPtr<ObjectA> pMove;
    pMove = std::move(ptr);
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(pMove.Get(), p);
  }
}
