
#include <unistd.h>
#include <sched.h>
#include <cstdlib>
#include <iostream>

#include "arduinoZ.h"
#include "mutex_lock.h"

arduinoZ* arduinoZ::m_arduino_inst(NULL);

inline bool f_exists( const char* name )
 {
  struct stat buffer;
  return (stat (name, &buffer) == 0);
 }

std::string pipefromprog("/tmp/atlas/pipefromprog");
std::string pipetoprog("/tmp/atlas/pipetoprog");

ssize_t pipe_write( const void *buf, size_t howmany )
 {
  if (!f_exists(pipefromprog.c_str())) return 0;
  int fd = open(pipefromprog.c_str(), O_WRONLY | O_NOCTTY | O_SYNC);
  ssize_t nc = write (fd, buf, howmany);
  close(fd);
  return nc;
 }

static int m_getval()
 {
  if (!f_exists(pipetoprog.c_str())) return 0;
  int fd = open(pipetoprog.c_str(), O_RDONLY | O_NOCTTY | O_SYNC);
  char buf[100];
  int n = read (fd, buf, 100);
  while (buf[n-1] != 'Z')
   {
    int m = read (fd, buf+n, 100);
    n += m;
   }
  buf[n-1] = '\0';
  close(fd);
  return atoi( buf );
 }

void arduinoZ::digitalSetup( int nchans, int* chans, arduinoDIO* way )
 {
  mutex_lock my_lock;

  char* message = new char[2*nchans+2];
  message[0] = 'a';
  for ( int i=0; i<nchans; i++)
    {
      message[2*i+1] = chans[i];
      message[2*i+2] = (way[i] == arduinoDIO::INPUT);
    }
  message[2*nchans+1] = 0xff;
  pipe_write (message, 2*nchans+2);
  sched_yield();
  delete message;
 }

int arduinoZ::analogInput( int chan )
 {
  mutex_lock my_lock;

  char c[2];
  c[0] = 'b';
  c[1] = chan;
  pipe_write (c, 2);
  sched_yield();
  return m_getval();
 }

bool arduinoZ::digitalInput( int chan )
 {
  mutex_lock my_lock;

  char c[2];
  c[0] = 'e';
  c[1] = chan;
  pipe_write (c, 2);
  sched_yield();
  return m_getval();
 }

void arduinoZ::digitalOutput( int chan, bool val )
 {
  mutex_lock my_lock;

  char c[3];
  c[0] = 'c';
  c[1] = chan;
  c[2] = val;
  pipe_write (c, 3);
  sched_yield();
 }

void arduinoZ::pulseOutput( int chan )
 {
  mutex_lock my_lock;

  char c[2];
  c[0] = 'd';
  c[1] = chan;
  pipe_write (c, 2);
  sched_yield();
 }

void arduinoZ::setPhysScale( uint16_t chan, double RFS, double RZS, double EFS, double EZS)
 {
  m_RFS[chan] = RFS; m_RZS[chan] = RZS;
  m_EFS[chan] = EFS; m_EZS[chan] = EZS;
  if (RFS != RZS)
   {
    m_slope[chan] = (EFS-EZS)/(RFS-RZS);
    m_intcpt[chan] = (EZS*RFS-EFS*RZS)/(RFS-RZS);
   }
  m_RFS[chan] = RFS; m_RZS[chan] = RZS;
 }

arduinoZ::arduinoZ()
 {
  std::string lockname = "thisisthearduinoserver";

  std::string servername = "/home/atlas/Micromegas/bin/arduinoserver";
  boost::interprocess::named_mutex my_mutex(boost::interprocess::open_or_create, lockname.c_str());
  if (!my_mutex.try_lock())
   {
    printf("server <%s> running\n", lockname.c_str());
   }
  else
   {
    my_mutex.unlock();
    printf("server <%s> NOT running\n", lockname.c_str());
    printf("going to restart it\n");
    std::string sst = servername + " &";
    system(sst.c_str());
   }

 }

arduinoZ* arduinoZ::create()
 {
  if (m_arduino_inst == NULL) m_arduino_inst = new arduinoZ();
  return m_arduino_inst;
 }

void arduinoZ::cleanup()
 {
  if (m_arduino_inst != NULL) delete m_arduino_inst;
  m_arduino_inst = NULL;
 }

