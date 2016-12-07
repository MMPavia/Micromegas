
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace boost::interprocess;
#define ARDUINO_NAMED_MUTEX "arduino_named_mutex"

int main (int argc, char** argv )
 {
  named_mutex::remove(ARDUINO_NAMED_MUTEX);
  return 0;
 }
