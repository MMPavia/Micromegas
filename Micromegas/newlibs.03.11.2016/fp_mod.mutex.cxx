
#define _USLEEP_ 10

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>

#include "fp_mod.h"
#include "mutex_lock.h"

#define error_message(F,E) printf(F, E)

using namespace std;

#define fp_write(fd, cstr) do { size_t sz = strlen(cstr); write (fd, cstr, sz); } while(0)

int32_t line_decode ( char* line )
 {
  size_t slen = strlen(line);
  if (slen < 9) return -1;
  char* code;
  line ++; // skip first char
  slen --; // skip first char
  line[slen-2] = 0; // remove last 2 chars
  slen -= 2;
  char cbuf[16];
  memset( cbuf, 0, 16 );
  memcpy( cbuf, line, 2 );
  uint16_t nmod = strtol( cbuf, NULL, 16 );
  line += 2;
  slen -= 2;
  for (size_t i=0; i<nmod; i++)
   {
    memcpy( cbuf, line, 4 );
    uint16_t mid = strtol( cbuf, NULL, 16 );
    line += 4;
    slen -= 4;
    cout << " module[" << i << "] : " << hex << mid << dec << endl;
    if (slen < 0) return -2;
   }
  return nmod;
 }

std::string fp_mod::m_tty("");
uint16_t fp_mod::m_modnr(0);
fp_rs232* fp_mod::m_fprs232(NULL);
int fp_mod::m_fd(-1);
FILE* fp_mod::m_fp(NULL);

fp_mod::fp_mod( uint16_t mod ) : m_id(mod), m_buffer((char*)malloc(65536)), m_size(65536)
 {
  if (m_fprs232 == NULL)
   {
    m_fprs232 = new fp_rs232( m_tty );
    m_fd = m_fprs232->fd();
    m_fp = m_fprs232->fp();
   }
  usleep(_USLEEP_);
  m_modnr ++;
  std::cout << "instance " << m_modnr << " mod # " << mod << std::endl;
 }

fp_mod::~fp_mod()
 {
  free(m_buffer);
  m_modnr --;
  if (m_modnr == 0)
   {
    if (m_fprs232 != NULL) delete m_fprs232;
   }
 }

uint32_t fp_mod::fp_moduleID()
 {

  mutex_lock my_lock;

  std::stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!A??." << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_moduleID " << m_buffer << std::endl;
  usleep(_USLEEP_);
  if (m_buffer[read-1] == 13)
   {
    m_buffer[read-1] = 0;
    read --;
   }
  if (read != 7) return uint16_t(-1);
  char cod[5] = {};
  memcpy(cod, m_buffer+1, 4);
  return strtoul(cod, NULL, 16);
 }

int32_t fp_mod::fp_allIDs()
 {
  if (m_id != 0) return 0;

  mutex_lock my_lock;

  std::string cmd(">00!B??.\n");
  fp_write (m_fd, cmd.c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_allIDs " << m_buffer << std::endl;
  usleep(_USLEEP_);
  if (m_buffer[read-1] == 13)
   {
    m_buffer[read-1] = 0;
    read --;
   }
  return line_decode(m_buffer);
 }

char srange[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
uint32_t fp_mod::fp_range( uint16_t chan, uint16_t range )
 {
  if (range > 255) return 0xffff;

  mutex_lock my_lock;

  uint16_t pos_mk = 1 << chan;
  std::stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!D"
     <<  std::setw(4) << pos_mk << "00001" << srange[range>>4] << srange[range&0xf] << "??." << std::endl;
std::cout << chan << " " << cmd.str() << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
  usleep(_USLEEP_);
  return 0;
 }

uint32_t fp_mod::fp_range( uint16_t chan )
 {

  mutex_lock my_lock;

  uint16_t pos_mk = 1 << chan;
  std::stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!E" <<  std::setw(4) << pos_mk << "00001??." << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
  usleep(_USLEEP_);
  if (m_buffer[read-1] == 13)
   {
    m_buffer[read-1] = 0;
    read --;
   }
  if (read != 5) return uint16_t(-1);
  char cod[3] = {};
  memcpy(cod, m_buffer+1, 2);
  return strtoul(cod, NULL, 16);
 }

uint16_t fp_mod::fp_read16( uint16_t chan )
 {

  mutex_lock my_lock;

  uint16_t pos_mk = 1 << chan;
  std::stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!F" <<  std::setw(4) << pos_mk << "??." << std::endl;
  uint32_t x(0);
  for (uint16_t i=0; i<4; i++)
   {
    fp_write (m_fd, cmd.str().c_str());
    ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_read16 (chan) " << m_buffer << std::endl;
    if (m_buffer[read-1] == 13)
     {
      m_buffer[read-1] = 0;
      read --;
     }
    if (read != 7) return uint16_t(-1);
    char cod[5] = {};
    memcpy(cod, m_buffer+1, 4);
    x += strtoul(cod, NULL, 16);
   }
  x >>= 2;
  return uint16_t(x);
 }

uint16_t fp_mod::fp_discrete()
 {

  mutex_lock my_lock;

  stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!J??." << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_getDI () " << m_buffer << std::endl;
  usleep(_USLEEP_);
  if (m_buffer[read-1] == 13)
   {
    m_buffer[read-1] = 0;
    read --;
   }
  if (read != 7) return uint16_t(-1);
  char cod[5] = {};
  memcpy(cod, m_buffer+1, 4);
  return strtoul(cod, NULL, 16);
 }

uint16_t fp_mod::fp_discrete( uint16_t chan, bool onoff )
 {

  mutex_lock my_lock;

  uint16_t pos_mk = 1 << chan;
  uint16_t val = (onoff) ? 1 << chan : 0;
  stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!L" <<  std::setw(4) << pos_mk << std::setw(4) << val << "??." << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_setDO (chan, onoff) " << m_buffer << std::endl;
  usleep(_USLEEP_);
  return 0;
 }

uint32_t fp_mod::fp_status()
 {

  mutex_lock my_lock;

  std::stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!N??." << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_status " << m_buffer << std::endl;
  usleep(_USLEEP_);
  if (m_buffer[read-1] == 13)
   {
    m_buffer[read-1] = 0;
    read --;
   }
  if (read != 4) return uint16_t(-1);
  return uint16_t(m_buffer[1]-'0');
 }

uint32_t fp_mod::fp_status( uint16_t chan )
 {

  mutex_lock my_lock;

  uint16_t pos_mk = 1 << chan;
  std::stringstream cmd;
  cmd << '>' << std::setfill('0') << std::setw(2) << std::hex << m_id << "!O" <<  std::setw(4) << pos_mk << "??." << std::endl;
// std::cout << __LINE__ << " fp_mod::fp_status (chan) " << cmd.str() << std::endl;
  fp_write (m_fd, cmd.str().c_str());
  usleep(_USLEEP_);
  ssize_t read = getline(&m_buffer, &m_size, m_fp);
// std::cout << __LINE__ << " fp_mod::fp_status (chan) " << m_buffer << std::endl;
  usleep(_USLEEP_);
  if (m_buffer[read-1] == 13)
   {
    m_buffer[read-1] = 0;
    read --;
   }
// std::cout << " fp_mod::fp_status( " << chan << " ) read : " << read << " " << m_buffer << endl;
  if (read != 4) return uint16_t(-1);
  uint16_t cod = m_buffer[1];
  return (cod != '?') ? uint16_t(cod-'0') : uint16_t(-2);
 }

void fp_mod::fp_tty( std::string tty )
 {
  m_tty = tty;
 }
