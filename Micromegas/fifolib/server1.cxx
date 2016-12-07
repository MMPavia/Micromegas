#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#define FIFO_FILE       "/tmp/atlas/temppipe"

static FILE *fp(NULL);

void* from_pipe( void* argv )
 {
  int rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  if (rc != 0) printf ("pthread_setcancelstate error %d\n", rc);\

  std::string* str = (std::string*)argv;
  char readbuf[256];

  /* Create the FIFO if it does not exist */
  umask(0);
  mknod(FIFO_FILE, S_IFIFO|0666, 0);

  while(1)
   {
    fp = fopen(FIFO_FILE, "r");
    fgets(readbuf, 256, fp);     // is fgets a cancellation point ?
    printf("Received string: %s\n", readbuf);
    *str = std::string(readbuf);
    printf("Received std::string: %s\n", str->c_str());
    fclose(fp);
    fp = NULL;
    if (*str == "quit") return NULL;
   }

  return NULL;
 }

class my_pipe
 {
  public:
   my_pipe( std::string& s);
   ~my_pipe();
  private:
   pthread_t m_tid;
 };

my_pipe::my_pipe( std::string& s )
 {
  int rc = pthread_create(&m_tid, NULL, from_pipe, &s);
  if (rc != 0) printf ("pthread_create error %d\n", rc);\
 }

my_pipe::~my_pipe()
 {
  int rc = pthread_cancel(m_tid);
  if (rc != 0) printf ("pthread_cancel error %d\n", rc);\
  if (fp != NULL) fclose(fp);
  printf("fp closed\n");
 }

std::string s("");
my_pipe p(s);

int main( int argc, char** argv )
 {
  while(1){ if (s == "quit") break; if (s == "kill") exit(1); }
  return 0;
 }
