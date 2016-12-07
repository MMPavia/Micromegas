#ifndef _OPTLINE_H_
#define _OPTLINE_H_

#include "myrs232.h"

enum class arduinoDIO { INPUT=1, OUTPUT=0 };

class arduino : rs232
 {
  public:
	arduino( std::string tty ) : rs232( rs232type::ARDUINO, tty ) {}
	~arduino() {}
	int writeLine( char* line, size_t sz );
	int getLine( char* line, size_t sz );
 };

#endif // _OPTLINE_H_
