#ifndef _PARTICLECOUNTER_H_
#define _CHAUVINARNOUX_H_

#include "myrs232.h"

class chauvinarnoux : rs232
 {
  public:
	chauvinarnoux( std::string tty ) : rs232( rs232type::CHAUVINARNOUX, tty ) {}
	~chauvinarnoux() {}
	void loop();

  private:
};

#endif // _CHAUVINARNOUX_H_
