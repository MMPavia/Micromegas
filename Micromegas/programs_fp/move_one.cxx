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
	bool reset_prog(false);
	double mm(0);
	uint64_t prog(0), mot(0), steps(0), speed(0), ramp(0), func(0);
	bool posdir(true);

	-- argc; ++ argv;
	if (argc < 2) return -1;

	if ((strcasecmp(*argv,"r")==0) || (strcasecmp(*argv,"-r")==0)) {
		reset_prog = true;
		-- argc; ++ argv;
	}
	if (argc < 2) return -2;

	if ((strcasecmp(*argv,"x")==0) || (strcasecmp(*argv,"-x")==0)) {
		mot = 1;
	} else if ((strcasecmp(*argv,"y")==0) || (strcasecmp(*argv,"-y")==0)) {
		mot = 2;
	} else if ((strcasecmp(*argv,"z")==0) || (strcasecmp(*argv,"-z")==0)) {
		mot = 6;
	} else if ((strcasecmp(*argv,"gluer")==0) || (strcasecmp(*argv,"-g")==0)) {
		mot = 5;
	} else if ((strcasecmp(*argv,"spint")==0) || (strcasecmp(*argv,"-s")==0)) {
		mot = 4;
	} else return -3;
	prog = 240+mot;

	-- argc; ++ argv;
/*
	mm = strtol(*argv, NULL, 10);
	posdir = (mm > 0);
	if (!posdir) mm = -mm;
	steps = (mot<3) ? mm*25 : (mot!=4) ? mm*400 : (mm*16000)/3;
 */

	char* wherecomma = index(*argv, ',');
	if (wherecomma != NULL) *wherecomma = '.';
	mm = strtod(*argv, NULL);
	posdir = (mm > 0);
	if (!posdir) mm = -mm;
	uint64_t um = uint64_t(mm*1000+0.5);
	// gg: cambiato ratio um passi 3.8.2016 (era 2/5) e 16/3
	steps = (mot<3) ? um/40 : (mot!=4) ? um*1/5 : (um*8)/3;

	-- argc; ++ argv;
	uint64_t aspd = (argc) ? strtol(*argv, NULL, 10) : 999999;
	// gg> cambiata velocita' motore 5 da 30 a 10
	speed = (aspd<100) ? aspd : (mot<3) ? 5 : (mot==6) ? 10 : (mot==5) ? 10 : (mot==4) ? 60 : 0;

	if ((steps > 99999) || (steps == 0))
	 {
		cout << " *** SPOSTAMENTO NON VALIDO (= " << steps << ") *** " << endl;
		return -4;
	 }
	if ((speed > 99) || (speed == 0))
	 {
		cout << " *** VELOCITA' NON VALIDA (= " << speed << ") *** " << endl;
		return -5;
	 }
 
	ramp = 31;
	func = 0;

	stringstream rcmd;
	stringstream mcmd;
	stringstream xcmd;

	rcmd << "r" << setfill('0') << setw(3) << prog << '\r' << endl;
	xcmd << "g" << setfill('0') << setw(3) << prog << '\r' << endl;
	uint64_t coded = ((mot * 100000 + steps)*100 + speed)*100 + ramp;
	mcmd << "m" << setfill('0') << setw(3) << prog << '-' << setw(10) << coded
	     << (posdir ? '+' : '-') << setw(1) << func << '\r' << endl;
 if (1) cout << "-----------------------------------------------------"
	     << "\n programma [-p] " << prog
	     << "\n motore [-m] " << mot
	     << "\n passi [-s] " << steps
	     << "\n velocità [-v] " << speed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << posdir
	     << "\n funzione [-f] " << func
	     << "\n stringa " << mcmd.str()
	     << "-----------------------------------------------------"
	     << endl;

        motp = new motors("/dev/ttyUSB1");

	if (reset_prog) motp->mot_reset(prog);
	motp->mot_write( mcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(xcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rcmd.str().c_str());
	motp->mot_read();
	
	delete motp;
	
	return 0;
}
