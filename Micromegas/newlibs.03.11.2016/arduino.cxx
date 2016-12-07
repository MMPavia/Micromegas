
#include <unistd.h>
#include <sched.h>
#include <cstdlib>
#include <iostream>

#include "arduino.h"

int arduino::writeLine( char* line, size_t sz )
 {
// std::cout << "arduino::writeLine (" << line << ", " << sz << ')' << std::endl;
  return write (m_fd, line, sz);
 }

int arduino::getLine( char* line, size_t sz )
 {
  // std::cout << "arduino::getLine max " << sz << std::endl;
  int n(0), m(0);
  do
   {
    m = read (m_fd, line, sz-n);  // read up to 100 characters if ready to read
    n += m;
   }
  while ((n < sz) && ((n == 0) || (m != 0)));
  // std::cout << "arduino::getLine got " << n << std::endl;
  return n;
 }
