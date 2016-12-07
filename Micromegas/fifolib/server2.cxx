#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#define FIFO_FILE       "/tmp/atlas/temppipe"

static FILE *fp(NULL);

void* from_pipe( void* argv )
 {
  int rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  if (rc != 0) printf ("pthread_setcancelstate error %d\n", rc);\

  char* cmd = (char*)argv;

  /* Create the FIFO if it does not exist */
  umask(0);
  mknod(FIFO_FILE, S_IFIFO|0666, 0);

  while(1)
   {
    fp = fopen(FIFO_FILE, "r");
    fgets(cmd, 256, fp);     // is fgets a cancellation point ?
    printf("Received string: %s\n", cmd);
    fclose(fp);
    fp = NULL;
   }

  return NULL;
 }

class my_pipe
 {
  public:
   my_pipe( char* cmd );
   my_pipe() {}
   ~my_pipe();
   void to_pipe( char* cmd );
  private:
   pthread_t m_tid;
 };

my_pipe::my_pipe( char* cmd )
 {
  int rc = pthread_create(&m_tid, NULL, from_pipe, cmd);
  if (rc != 0) printf ("pthread_create error %d\n", rc);\
 }

my_pipe::~my_pipe()
 {
  int rc = pthread_cancel(m_tid);
  if (rc != 0) printf ("pthread_cancel error %d\n", rc);\
  if (fp != NULL) fclose(fp);
  printf("fp closed\n");
 }

void my_pipe::to_pipe( char* cmd )
 {
  FILE *fp;

  if((fp = fopen(FIFO_FILE, "w")) == NULL)
   {
    perror("fopen");
    exit(1);
   }
  fputs(cmd, fp);
  fclose(fp);
 }

char cmd[256];
my_pipe p(cmd);

int main( int argc, char** argv )
 {
  while(1)
   {
    if (strcasecmp(cmd,"quit")==0) break;
    if (strcasecmp(cmd,"kill")==0) exit(1);
   }
  return 0;
 }
