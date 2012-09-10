#ifndef __SINGLETON__
#define __SINGLETON__
#include <noncopyable.h>

namespace green_turtle{

//not thread safe
template<class T>
class Singleton : NonCopyable
{
 public:
  ~Singleton()
  {
    delete value_;
    value_ = nullptr;
  }
  static T& Instance()
  {
    if(!value_)
      value_ = new T();
    return *value_;
  }
 private:
  static T  *value_;

};
template<class T>
T* Singleton<T>::value_ = NULL;

}

#endif
