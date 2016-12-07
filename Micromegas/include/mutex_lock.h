#ifndef _MUTEX_LOCK_H_
#define _MUTEX_LOCK_H_

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

class mutex_lock
 {
  public:
   mutex_lock();
   ~mutex_lock();
  private:
   boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock;
 };

#endif // _MUTEX_LOCK_H_
