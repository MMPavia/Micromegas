#ifndef _KEITHLEY_H_
#define _KEITHLEY_H_

#include <unistd.h>
#include "myrs232.h"

class keithley : rs232
 {
  public:
	keithley( std::string tty ) : rs232( rs232type::KEITHLEY, tty ) {}
	~keithley() {}
	ssize_t writeline(const void* cmd, size_t cmdl)
	 { return write (m_fd, cmd, cmdl); }
	void readline(double &T_mis, double &dt, int &dec, double &val);
 };

#endif // _KEITHLEY_H_
