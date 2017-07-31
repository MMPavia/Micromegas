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

#include "arduinoX.h"
#include "motors.h"

#define error_message(F,E) printf(F, E)

#define _POS_DIR_ true
#define _NEG_DIR_ false


using namespace std;

motors* motp(NULL);

int ndigital = 1;
int dchan[5] = { 38, 39, 40, 41, 42};//4 for relee
arduinoDIO dway[5] = { arduinoDIO::INPUT, arduinoDIO::INPUT,
                       arduinoDIO::INPUT, arduinoDIO::INPUT, arduinoDIO::INPUT  };

int32_t main (int argc, char** argv)
{
	bool reset_prog(false);
	double mm(0);
	uint64_t prog(0), mot(0), steps(0), speed(0), ramp(0), func(0);
	bool dir(_POS_DIR_);

	arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
        usleep(1000000);
        myboard->digitalSetup( ndigital, dchan, dway );
        usleep(100000);


	-- argc; ++ argv;
	if (argc < 1) return __LINE__;

/*
 * PROG=001 MOT1 STEP=10000 SPEED=05 RAMP=31 DIR=- FUNC=3
 * PROG=002 MOT2 STEP=10000 SPEED=05 RAMP=31 DIR=+ FUNC=3
 * PROG=003 MOT6 STEP=10000 SPEED=10 RAMP=31 DIR=- FUNC=3
 * PROG=004 MOT5 STEP=10000 SPEED=30 RAMP=31 DIR=- FUNC=3
 * PROG=005 MOT4 STEP=10000 SPEED=60 RAMP=31 DIR=+ FUNC=3
 */
	if ((strcasecmp(*argv,"x")==0) || (strcasecmp(*argv,"-x")==0)) {
		mot = 1;
		dir = _NEG_DIR_;
	} else if ((strcasecmp(*argv,"y")==0) || (strcasecmp(*argv,"-y")==0)) {
		mot = 2;
		dir = _POS_DIR_;
	} else if ((strcasecmp(*argv,"z")==0) || (strcasecmp(*argv,"-z")==0)) {
		mot = 6;
		dir = _NEG_DIR_;
	} else if ((strcasecmp(*argv,"gluer")==0) || (strcasecmp(*argv,"-g")==0)) {
		mot = 5;
		dir = _NEG_DIR_;
	} else if ((strcasecmp(*argv,"spint")==0) || (strcasecmp(*argv,"-s")==0)) {
		mot = 4;
		dir = _POS_DIR_;
	} else return __LINE__;
	prog = 240+mot;

/*
	-- argc; ++ argv;
	uint64_t aspd = (argc) ? strtol(*argv, NULL, 10) : 999999;
	speed = (aspd<100) ? aspd : (mot<3) ? 5 : (mot==6) ? 10 : (mot==5) ? 30 : (mot==4) ? 60 : 0;
 */
	// 2.9.16 changed velocity mot 5 to 15 from 30
	speed = (mot<3) ? 10 : (mot==6) ? 15 : (mot==5) ? 15 : (mot==4) ? 90 : 0; 

	if ((speed > 99) || (speed == 0))
	 {
		cout << " *** VELOCITA' NON VALIDA (= " << speed << ") *** " << endl;
		return __LINE__;
	 }
 
	steps = 10000;
	ramp = 31;
	func = 3;

	stringstream mcmd;
	stringstream rcmd; 
	stringstream xcmd;

        rcmd << "r" << setfill('0') << setw(3) << prog << '\r' << endl;
        xcmd << "g" << setfill('0') << setw(3) << prog << '\r' << endl;
        uint64_t coded = ((mot * 100000 + steps)*100 + speed)*100 + ramp;
        mcmd << "m" << setfill('0') << setw(3) << prog << '-' << setw(10) << coded
	     << ((dir==_POS_DIR_) ? '+' : '-') << setw(1) << func << '\r' << endl;
 if (1) cout << "-----------------------------------------------------"
	     << "\n programma [-p] " << prog
	     << "\n motore [-m] " << mot
	     << "\n passi [-s] " << steps
	     << "\n velocità [-v] " << speed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << dir
	     << "\n funzione [-f] " << func
	     << "\n stringa " << mcmd.str()
	     << "-----------------------------------------------------"
	     << endl;

	motp = new motors("/dev/ttyUSB1");

	if (reset_prog) motp->mot_reset(prog);
	motp->mot_write(mcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(xcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rcmd.str().c_str());
	motp->mot_read();    
    
	//controlla che la macchina e' in uso
	uint16_t mstati(0);
	uint16_t mstat(0);
	

	do
	  {
	    mstat = 0;
	    for (int chan=38; chan<43; chan++){
            mstati = myboard->digitalInput(chan);
            mstat += mstati;
	    }
	    usleep(10000);
	  }
	while (mstat > 0);
	usleep(2000000);

	delete motp;

	return 0;
}
