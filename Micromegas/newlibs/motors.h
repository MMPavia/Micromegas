#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <stdint.h>
#include <list>

#include "myrs232.h"

class motors : rs232
 {
  public:
        motors( std::string tty ) : rs232( rs232type::MOTORS, tty ) {}
        ~motors() {}
        void mot_write( std::string s );
        void mot_read();
        void mot_reset( uint16_t prog );
	void mot_dump();
  private:
	char m_buf[65536];
	std::list<std::string> m_rdbuf;
 };

#endif // _MOTORS_H_
