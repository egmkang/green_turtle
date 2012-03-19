#include "../util/ref_object.h"
#include <stdio.h>

using namespace green_turtle::util;

class ObjA : public RefObject
{
 public:
  int value;
};


int main()
{
  {
    ObjA *pObjA = new ObjA;
    pObjA->value = 100;
    RefPtr<ObjA> ptr(pObjA);

    ObjA *p = ptr.Get();
    if(p)
    {
      printf("%d\n",p->value);
    }
    delete pObjA;

    p = ptr.Get();
    if(p)
    {
      printf("%d\n",p->value);
    }
    else
    {
      printf("p is nil\n");
    }
  }


  return 0;
}
