#include <ref_object.h>
#include <stdio.h>
#include <assert.h>

using namespace green_turtle;

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
    RefPtr<ObjA> ptr = pObjA;

    RefPtr<const ObjA> pConst = (const ObjA*)(pObjA);
    RefPtr<const ObjA> pConst1 = pObjA;
    //Fail
    //const Ptr convert to non const Ptr,
    //RefPtr<ObjA> pConst2 = (const ObjA*)(pObjA);

    RefPtr<ObjA> null;
    null = pObjA;
    assert(null);
    printf("%d\n", null->value);

    if(ptr)
    {
      printf("%d\n",ptr->value);
    }
    delete pObjA;

    if(ptr)
    {
      printf("%d\n",ptr->value);
    }
    else
    {
      printf("p is nil\n");
    }
    RefPtr<ObjA> ptr1 = ptr;
    if(ptr1)
    {
      printf("%d\n",ptr1->value);
    }
    else
    {
      printf("ptr1 is nil\n");
    }
  }


  return 0;
}
