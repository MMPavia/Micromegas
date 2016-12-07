
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

class mutex_lock
 {
  public:
   mutex_lock();
   ~mutex_lock();
   static void remove_lock(void);
  private:
   boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock;
   static bool is_mine;
 };

