
#include <iostream>
#include <boost/interprocess/sync/named_mutex.hpp>

#define ARDUINO_NAMED_MUTEX "arduino_named_mutex"

using namespace boost::interprocess;

static named_mutex arduino_mutex(open_or_create, ARDUINO_NAMED_MUTEX);

int main()
 {
  bool lk = arduino_mutex.try_lock();
  std::cout << " mutex '" ARDUINO_NAMED_MUTEX "' is" << (lk ? " NOT " : " ") << "locked by somebody else" << std::endl;
  std::cout << " mutex '" ARDUINO_NAMED_MUTEX "' is" << (lk ? " " : " NOT ") << "locked by me" << std::endl;
  bool removed(true);
  if (!lk)
   {
    removed = named_mutex::remove(ARDUINO_NAMED_MUTEX);
    std::cout << " mutex '" ARDUINO_NAMED_MUTEX "' was" << (removed ? " " : " NOT ") << "removed " << std::endl;
    if (!removed) std::cerr << "*** mutex '" ARDUINO_NAMED_MUTEX "' is still LOCKED ***" << std::endl;
   } else arduino_mutex.unlock();

  if (removed) std::cout << " NOW mutex '" ARDUINO_NAMED_MUTEX "' must be unlocked" << std::endl;
  return 0;
 }
