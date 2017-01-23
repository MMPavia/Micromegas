#include <iostream>

#include <boost/interprocess/sync/named_mutex.hpp>

static boost::interprocess::named_mutex
 arduino_mutex(boost::interprocess::open_or_create, "arduino_named_mutex");

int main()
 {
  bool lk = arduino_mutex.try_lock();
  std::cout << " mutex was" << (lk ? " " : " NOT ") << "locked" << std::endl;
  if (lk) arduino_mutex.unlock();
  boost::interprocess::named_mutex::remove("arduino_named_mutex");
  return 0;
 }
