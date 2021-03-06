
#include <unistd.h>
#include <sched.h>
#include <cstdlib>
#include <iostream>

#include "arduinoX.h"
#include "mutex_lock.h"

arduinoX* arduinoX::m_arduino_inst(NULL);

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

void arduinoX::digitalSetup( int nchans, int* chans, arduinoDIO* way )
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
  write (m_fd, message, 2*nchans+2);
  sched_yield();
  delete message;
 }

int arduinoX::analogInput( int chan )
 {
  mutex_lock my_lock;

  char c[2];
  c[0] = 'b';
  c[1] = chan;
  write (m_fd, c, 2);
  sched_yield();
  return m_getval(m_fd);
 }

bool arduinoX::digitalInput( int chan )
 {
  mutex_lock my_lock;

  char c[2];
  c[0] = 'e';
  c[1] = chan;
  write (m_fd, c, 2);
  sched_yield();
  return m_getval(m_fd);
 }

void arduinoX::digitalOutput( int chan, bool val )
 {
  mutex_lock my_lock;

  char c[3];
  c[0] = 'c';
  c[1] = chan;
  c[2] = val;
  write (m_fd, c, 3);
  sched_yield();
 }

void arduinoX::pulseOutput( int chan )
 {
  mutex_lock my_lock;

  char c[2];
  c[0] = 'd';
  c[1] = chan;
  write (m_fd, c, 2);
  sched_yield();
 }

arduinoX* arduinoX::create( std::string tty )
 {
  if (m_arduino_inst == NULL) m_arduino_inst = new arduinoX( tty );
  return m_arduino_inst;
 }

void arduinoX::cleanup()
 {
  if (m_arduino_inst != NULL) delete m_arduino_inst;
  m_arduino_inst = NULL;
 }

void arduinoX::setPhysScale( uint16_t chan, double RFS, double RZS, double EFS, double EZS)
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

