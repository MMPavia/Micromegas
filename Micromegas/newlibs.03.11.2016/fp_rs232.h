#ifndef _FP_RS232_H_
#define _FP_RS232_H_

#include "myrs232.h"

class fp_rs232 : rs232
 {
  public:
	fp_rs232( std::string tty ) : rs232( rs232type::FIELDPOINT, tty ) {}
	~fp_rs232() {}
	int fd() { return m_fd; }
	FILE* fp() { return m_fp; }
       
 };

#endif // _FP_RS232_H_
