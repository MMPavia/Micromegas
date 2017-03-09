
#include <cstdio>
#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void sig_handler( int sig )
 {
  void* array[64];
  size_t bsize = backtrace(array, 64);
  fprintf(stderr, "Error: signal %d\n", sig);
  backtrace_symbols_fd(array, bsize, STDERR_FILENO);
  exit(1);
 }

class myha
 {
  public:
   myha()
   {
    signal(SIGSEGV, sig_handler );
    fprintf(stderr, "Installed handler for signal %d\n", SIGSEGV);
   }
 };

myha mm;
