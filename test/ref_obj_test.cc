#include "../util/ref_object.h"
#include <iostream>

using namespace green_turtle::util;

class ObjA : public RefObject
{
 public:
  int value;
};


int main()
{
  ObjA *pObjA = new ObjA;
  pObjA->value = 100;
  RefPtr<ObjA> ptr(pObjA);

  ObjA *p = ptr.Get();
  if(p)
  {
    std::cout << p->value << std::endl;
  }
  delete pObjA;

  p = ptr.Get();
  if(p)
  {
    std::cout << p->value << std::endl;
  }
  else
  {
    std::cout << "p is nil" << std::endl;
  }


  return 0;
}
