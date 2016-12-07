#ifndef _OPTLINE3D_H_
#define _OPTLINE3D_H_

#include "myrs232.h"

class optline : rs232
 {
  public:
	optline( std::string tty ) : rs232( rs232type::OPTLINE, tty ) {}
	~optline() {}
	char* readlineXYZ();
	double readlineX();
	double readlineY();
	double readlineZ();
 };

#endif // _OPTLINE_H_
