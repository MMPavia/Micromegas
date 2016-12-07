#ifndef _ARDUINOSERIAL_H_
#define _ARDUINOSERIAL_H_

#include "myrs232.h"

class arduinoSerial : rs232
 {
  public:
	static arduinoSerial* create ( std::string tty );
	static void cleanup();

  protected:
	arduinoSerial( std::string tty ) : rs232( rs232type::ARDUINO, tty ) {}
	virtual ~arduinoSerial() {}

  public:
	static arduinoSerial* m_aserial_inst;
 };

#endif // _ARDUINOSERIAL_H_
