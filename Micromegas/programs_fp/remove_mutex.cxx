
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace boost::interprocess;
#define FP_NAMED_MUTEX "fp_named_mutex"

int main (int argc, char** argv )
 {
  named_mutex::remove(FP_NAMED_MUTEX);
  return 0;
 }
