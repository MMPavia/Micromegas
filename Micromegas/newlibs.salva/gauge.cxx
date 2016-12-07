
#include <unistd.h>
#include <cstdlib>

#include "gauge.h"

double gauge::readval()
 {
	write (m_fd, "@", 1);           // send 1 character greeting
	usleep(200000);
	char buf [100];
	int n = read (m_fd, buf, 100);  // read up to 100 characters if ready to read
	while (buf[n-1] != 13)
	 {
	  int m = read (m_fd, buf+n, 100);
	  n += m;
	 }
	buf[n-1] = '\0';
	return strtod( buf, NULL);
 }
