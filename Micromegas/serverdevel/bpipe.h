#include <pthread.h>
#include <cstddef>
#include <string>

class bpipe
 {
  public:
   bpipe( const char* name, char* cmd );
   bpipe( const char* name );
   ~bpipe();
   void to_bpipe( const void* buf, size_t count );
  private:
   pthread_t m_tid;
   std::string m_pname;
 };

