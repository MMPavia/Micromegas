#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "myrs232.h"

class gauge : rs232
 {
  public:
	gauge( std::string tty ) : rs232( rs232type::GAUGE, tty ) {}
	~gauge() {}
	double readval();
 };

#endif // _GAUGE_H_
