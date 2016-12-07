
#include <unistd.h>
#include <sched.h>
#include <cstdlib>
#include <iostream>

#include "arduino2.h"

static int m_getval( int fd )
 {
  char buf[100];
  int n = read (fd, buf, 1);
  while (buf[n-1] != 10)
   {
    int m = read (fd, buf+n, 1);
    n += m;
   }
  buf[n-1] = '\0';
  return atoi( buf );
 }

void arduino2::digitalSetup( int nchans, int* chans, arduinoDIO* way )
 {
  char* message = new char[2*nchans+2];
  message[0] = 'a';
  for ( int i=0; i<nchans; i++)
    {
      message[2*i+1] = chans[i];
      message[2*i+2] = (way[i] == arduinoDIO::INPUT);
    }
  message[2*nchans+1] = 0xff;
  write (m_fd, message, 2*nchans+2);
  sched_yield();
  delete message;
 }

int arduino2::analogInput( int chan )
 {
  char c[2];
  c[0] = 'b';
  c[1] = chan;
  write (m_fd, c, 2);
  sched_yield();
  return m_getval(m_fd);
 }

bool arduino2::digitalInput( int chan )
 {
  char c[2];
  c[0] = 'e';
  c[1] = chan;
  write (m_fd, c, 2);
  sched_yield();
  return m_getval(m_fd);
	 }

void arduino2::digitalOutput( int chan, bool val )
 {
  char c[3];
  c[0] = 'c';
  c[1] = chan;
  c[2] = val;
  write (m_fd, c, 3);
  sched_yield();
 }

void arduino2::pulseOutput( int chan )
 {
  char c[2];
  c[0] = 'd';
  c[1] = chan;
  write (m_fd, c, 2);
  sched_yield();
 }

