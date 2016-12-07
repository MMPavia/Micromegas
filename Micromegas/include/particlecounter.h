#ifndef _PARTICLECOUNTER_H_
#define _PARTICLECOUNTER_H_

#include "myrs232.h"

class particlecounter : rs232
 {
  public:
	particlecounter( std::string tty ) : rs232( rs232type::PARTICLECOUNTER, tty ) {}
	~particlecounter() {}
	std::string reading();
	void connect();
	void run();

  private:
	double big_part;
	double small_part;
		
 
};

#endif // _PARTICLECOUNTER_H_
