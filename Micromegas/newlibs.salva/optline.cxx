
#include <unistd.h>
#include <cstdlib>

#include "optline.h"

double optline::readline()
 {
  write (m_fd, "Z\r\n", 3);       // send 3 character greeting
  usleep(100000);
  char buf[100];
  int n = read (m_fd, buf, 100);  // read up to 100 characters if ready to read
  while (buf[n-1] != 10)
   {
    int m = read (m_fd, buf+n, 100);
    n += m;
   }
  buf[n-1] = '\0';
  if (buf[n-2] == 13) buf[n-2] = '\0';
  if (buf[0] == 'Z') buf[0] = ' ';
  return strtod( buf, NULL);
 }
