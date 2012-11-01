#include <buffer.h>
#include <assert.h>

using namespace green_turtle;

int main()
{
  Buffer buffer(1024);
  assert(buffer.ReadableLength() == 0);
  assert(buffer.WritableLength() == buffer.Capacity());

  const char *str = "test string";
  buffer.Append(str, strlen(str));
  assert(strcmp(buffer.BeginRead(), str) == 0);
  buffer.HasRead(strlen(str));
  assert(buffer.ReadableLength() == 0);
  buffer.Retrieve();
  assert(buffer.BeginRead() == buffer.BeginWrite());

  return 0;
}
