#include <gtest/gtest.h>
#include <flags.h>

using namespace green_turtle;

const char* argv[] =
{
  "unittest",
  "-d",
  "-port",
  "3306",
  "-h"
};


TEST(Flags, Check)
{
  CommandFlags flags(sizeof(argv)/sizeof(argv[0]), argv);
  EXPECT_EQ(flags.CheckFlag("d"), true);
  EXPECT_EQ(flags.CheckFlag("p"), false);
  EXPECT_EQ(flags.CheckFlag("h"), true);
  EXPECT_NE(flags.CheckFlag("H"), true);
}

TEST(Flags, GetFlag)
{
  CommandFlags flags(sizeof(argv)/sizeof(argv[0]), argv);
  EXPECT_EQ(flags.GetFlags("h"), "");
  EXPECT_EQ(flags.GetFlags("d"), "");
  EXPECT_EQ(flags.GetFlags("port"), "3306");
}
