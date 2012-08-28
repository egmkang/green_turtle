#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <assert.h>
#include <pthread.h>
#include <noncopyable.h>

namespace green_turtle{

//because clang complete do not support libc++....
struct Mutex : NonCopyable
{
  public:
   Mutex()
   {
     ::pthread_mutex_init(&this->m_, nullptr);
   }
   ~Mutex()
   {
     ::pthread_mutex_destroy(&this->m_);
   }
  public:
   void Lock()
   {
     ::pthread_mutex_lock(&this->m_);
   }
   void UnLock()
   {
     ::pthread_mutex_unlock(&this->m_);
   }
  private:
   pthread_mutex_t m_;
};

template<class lockable>
struct LockGuard
{
  public:
   explicit LockGuard(lockable& m) : m(m)
   {
     m.Lock();
   }
   ~LockGuard()
   {
     m.UnLock();
   }
  private:
   lockable& m;
};

}
#endif
