#include <unistd.h>
#include <iostream>

#include "myrs232.h"

class arduinosrv : rs232
 {
  public:
    arduinosrv( std::string tty ) : rs232( rs232type::ARDUINO, tty ) {}
    ~arduinosrv() {}
    int sendreq(char* msg, int len);
    char* getval(int* v);
  private:
    char m_buf[1024];
 };

inline int arduinosrv::sendreq( char* msg, int len )
 {
  write (m_fd, msg, len);
  return 0; 
 }

inline int m_getval( int fd, char* buf )
 {
  int n = read (fd, buf, 100);
  while (buf[n-1] != '\n')
   {
    int m = read (fd, buf+n, 100);
    n += m;
   }
  buf[n-1] = '\0';
  return atoi( buf );
 }

inline char* arduinosrv::getval( int* v )
 {
  *v = m_getval( m_fd, m_buf );
  return m_buf;
 }

#include <sys/types.h>
#include <sys/stat.h>

inline bool f_exists( const char* name )
 {
  struct stat buffer;
  return (stat (name, &buffer) == 0);
 }

#include <csignal>
#include <memory>
#include <boost/interprocess/sync/named_mutex.hpp>

#include "bpipe.h"

static boost::interprocess::named_mutex* my_mutex(NULL);
static std::string lockname("");

bpipe* ptoprog(NULL);
bpipe* pfromprog(NULL);

void my_cleanup()
 {
  std::unique_ptr<bpipe> toprog (ptoprog);
  std::unique_ptr<bpipe> fromprog (pfromprog);

  std::cerr << "AAAA - cleaning up" << std::endl;
  if (my_mutex != NULL)
   {
    my_mutex->unlock();
    delete my_mutex;
    my_mutex = NULL;
   }
  if (lockname != "") boost::interprocess::named_mutex::remove(lockname.c_str());
  lockname = "";
 }

void sighandler(int sig)
 {
  std::cerr << "BBBB - caught signal " << sig << std::endl;
  exit(sig);
 }

int main( int argc, char** argv )
 {
  atexit(my_cleanup);
  signal(SIGINT, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGTERM, sighandler);

  lockname = "thisisthearduinoserver";

  if ((argc == 2) && (strcmp("-clean", argv[1]) == 0))
    boost::interprocess::named_mutex::remove(lockname.c_str());

  my_mutex = new boost::interprocess::named_mutex(boost::interprocess::open_or_create, lockname.c_str());
  if (!my_mutex->try_lock())
   {
    printf("process %s already running\n", lockname.c_str());
    throw 1;
   }

  arduinosrv sv("/dev/ttyUSB0");

  std::string inpipe("/tmp/atlas/pipefromprog");
  std::string outpipe("/tmp/atlas/pipetoprog");

  /* Create the FIFO if it does not exist */
  if (!f_exists(outpipe.c_str()))
   {
    umask(0);
    mkfifo(outpipe.c_str(), 0666);
   }

  char cmd[1024] = {'\0'};

  ptoprog = new bpipe(outpipe.c_str());
  pfromprog = new bpipe(inpipe.c_str(), cmd);

  char* data = cmd+1;
  while(1)
   {
    char* vstr;
    int val;
    int nby = cmd[0];
    if (nby != '\0')
     {
      if (strstr(data,"quit") == data) break;
      if (strstr(data,"kill") == data) exit(999);
 
      sv.sendreq( data, nby );
      if ((data[0] == 'b') || (data[0] == 'e'))    // analogIn or digitalIn
       {
        vstr = sv.getval(&val);
        ptoprog->to_bpipe(vstr, strlen(vstr));
       }
      *cmd = '\0';
     }
   }

  /*
   * cleanup is done by my_cleanup() function:
   * my_mutex->unlock();
   * delete my_mutex;
   * boost::interprocess::named_mutex::remove(lockname.c_str());
   */
  return 0;
 }
