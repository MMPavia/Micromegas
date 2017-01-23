#ifndef _PICOAM_H_
#define _PICOAM_H_

#include <unistd.h>
#include "myrs232.h"

class rs232laser : rs232
 {
  public:
	rs232laser( std::string tty ) : rs232( rs232type::LASER, tty ) {}
	~rs232laser() {}
	ssize_t writeline(const void* cmd, size_t cmdl)
	 { return write (m_fd, cmd, cmdl); }
	int readlaser(std::string &val);
 };

#endif // _PICOAM_H_
