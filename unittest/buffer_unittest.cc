#include <gtest/gtest.h>
#include <buffer.h>

using namespace green_turtle;

TEST(Buffer, Empty)
{
  Buffer buffer(1024);
  EXPECT_EQ(0ul, buffer.ReadableLength());
  EXPECT_EQ(1024ul, buffer.WritableLength());
  EXPECT_EQ(buffer.BeginRead(), buffer.BeginWrite());
}

TEST(Buffer, Full)
{
  Buffer buffer(1024);
  char c = 'c';
  for(int i = 0; i < 1023; ++i)
  {
    buffer.Append(&c, sizeof(c));
  }
  EXPECT_EQ(1ul, buffer.WritableLength());

  buffer.Append(&c, sizeof(c));
  EXPECT_EQ(0ul, buffer.WritableLength());
  EXPECT_EQ(1024ul, buffer.ReadableLength());
}

TEST(Buffer, ReadWrite)
{
  Buffer buffer(1024);
}
