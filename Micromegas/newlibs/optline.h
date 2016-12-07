#ifndef _OPTLINE_H_
#define _OPTLINE_H_

#include "myrs232.h"

class optline : rs232
 {
  public:
	optline( std::string tty ) : rs232( rs232type::OPTLINE, tty ) {}
	~optline() {}
	double readline();
 };

#endif // _OPTLINE_H_
