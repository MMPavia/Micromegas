#ifndef _KEITHLEY_H_
#define _KEITHLEY_H_

#include <unistd.h>
#include "myrs232.h"

class keithley : rs232
 {
  public:
	keithley( std::string tty ) : rs232( rs232type::KEITHLEY, tty )
          { m_precision = 6; }
	~keithley() {}
	ssize_t  writecmd(const void* cmd, size_t cmdl)
          { return write (m_fd, cmd, cmdl); }
	void setprecision(int prc) { m_precision = prc; }
	void readline( double& val, double& time, double& stat );
  private:
        int m_precision;
 };

#endif // _KEITHLEY_H_
