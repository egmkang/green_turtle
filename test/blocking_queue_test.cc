#include <blocking_queue.h>
#include <assert.h>

using namespace green_turtle;

int main()
{
  BlockingQueue<int> queue;

  int v;
  assert(!queue.Pop(v));
  assert(queue.Push(v));
  int v1;
  assert(queue.Pop(v1));
  assert(v1 == v);

  return 0;
}
