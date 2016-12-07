#ifndef _ARDUINO2_H_
#define _ARDUINO2_H_

#include "myrs232.h"

enum class arduinoDIO { INPUT=1, OUTPUT=0 };

class arduino2 : rs232
 {
  public:
	arduino2( std::string tty ) : rs232( rs232type::ARDUINO, tty ) {}
	~arduino2() {}
	void digitalSetup( int nchans, int* chans, arduinoDIO* way );
	void digitalOutput( int chan, bool val );
	int analogInput( int chan );
	bool digitalInput( int chan );
	void pulseOutput( int chan );
 };

#endif // _ARDUINO2_H_
