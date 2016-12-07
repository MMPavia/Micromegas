#include <pthread.h>
#include <cstdio>
#include <string>

class my_pipe
 {
  public:
   my_pipe( const char* name, char* cmd );
   my_pipe( const char* name );
   ~my_pipe();
   void to_pipe( char* cmd );
  private:
   pthread_t m_tid;
   std::string m_pname;
 };

