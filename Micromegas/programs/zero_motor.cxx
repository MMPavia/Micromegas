#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "motors.h"

#define error_message(F,E) printf(F, E)

using namespace std;

motors* motp(NULL);

int main (int argc, char** argv)
{
	if (argc < 2) return -1;
	-- argc; ++ argv;

	uint64_t prog1(0), prog2(0);

	prog1 = strtoul(*argv, NULL, 10);
	-- argc; ++ argv;

	if (argc) prog2 = strtoul(*argv, NULL, 10);

	if ((prog2) && (prog2 < prog1)) return -2;

	if (prog2 == 0) prog2 = prog1;

	motp = new motors("/dev/ttyUSB1");
	do
	 {
	  for (uint64_t i=1; i<7; ++i)
	   {
	    stringstream zcmd;
	    zcmd << "m" << setfill('0') << setw(3) << prog1 << '-'
	         << setw(10) << i*100000*100*100 << "+0\r" << endl;
	    motp->mot_write( zcmd.str().c_str());
	    motp->mot_read();
	   }
	  stringstream rcmd;
	  rcmd << "r" << setfill('0') << setw(3) << prog1 << '\r' << endl;
	  motp->mot_write(rcmd.str().c_str());
	  motp->mot_read();
	  prog1 ++;
	 }
	while (prog1 <= prog2);
	
	delete motp;
	
	return 0;
}
