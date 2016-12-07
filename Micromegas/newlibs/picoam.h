#ifndef _PICOAM_H_
#define _PICOAM_H_

#include <unistd.h>
#include "myrs232.h"

class picoam : rs232
 {
  public:
	picoam( std::string tty ) : rs232( rs232type::PICOAM, tty ) {}
	~picoam() {}
	ssize_t writeline(const void* cmd, size_t cmdl)
	 { return write (m_fd, cmd, cmdl); }
	int readpico(std::string &val);
 };

#endif // _PICOAM_H_
