#include <gtest/gtest.h>
#include <buffer.h>
#include <strings.h>
#include <string>

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
  auto size = buffer.Append(&c, sizeof(c));
  EXPECT_EQ(1ul, size);
  EXPECT_EQ(0ul, buffer.WritableLength());
  EXPECT_EQ(1025ul, buffer.ReadableLength());
}

TEST(Buffer, ReadWrite)
{
  Buffer buffer(16);
  auto begin_ptr = buffer.BeginRead();
  auto str = "0123456789";
  auto write_size = buffer.Append(str, strlen(str));
  EXPECT_EQ(strlen(str), write_size);
  EXPECT_EQ(strlen(str), buffer.ReadableLength());
  EXPECT_EQ(strlen(str), size_t(buffer.BeginWrite() - begin_ptr));
  std::string s(buffer.BeginRead(), buffer.BeginWrite());
  EXPECT_EQ(s, str);
  buffer.HasRead(s.size());

  auto size = buffer.Append(str, strlen(str));
  EXPECT_EQ(size, buffer.Capacity() - strlen(str));
  s.assign(std::string(buffer.BeginRead(), buffer.BeginWrite()));
  EXPECT_EQ(std::string(str, str + size), s);
  EXPECT_EQ(begin_ptr + buffer.Capacity(), buffer.BeginWrite());
}

TEST(Buffer, Retrieve)
{
  Buffer buffer(16);
  auto begin_ptr = buffer.BeginRead();

  buffer.Retrieve();
  EXPECT_EQ(begin_ptr, buffer.BeginRead());
  EXPECT_EQ(begin_ptr, buffer.BeginWrite());

  auto str = "0123456789";
  buffer.Append(str, strlen(str));
  buffer.HasRead(strlen(str));
  EXPECT_NE(begin_ptr, buffer.BeginRead());
  EXPECT_EQ(buffer.BeginRead(), buffer.BeginWrite());

  buffer.Retrieve();
  EXPECT_EQ(begin_ptr, buffer.BeginRead());
  EXPECT_EQ(begin_ptr, buffer.BeginWrite());

  buffer.Append(str, strlen(str));
  buffer.Retrieve();
  EXPECT_EQ(strlen(str), buffer.ReadableLength());
  EXPECT_EQ(str, std::string(buffer.BeginRead(), buffer.BeginRead()+buffer.ReadableLength()));

}
