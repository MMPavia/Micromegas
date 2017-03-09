#define _USLEEP_ 10000

#include <unistd.h>
#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "arduinoX.h"
#include "optline3D.h"

#include "motors.h"

#include "my_pipe.h"

std::string pipe_name("/tmp/atlas/trapXYSpipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {0};
my_pipe p(pipe_name.c_str(), cmd);

#define error_message(F,E) printf(F, E)

using namespace std;

motors* motp(NULL);

void next ( const char* gx, arduinoX* myboard )
{
    motp->mot_write(gx);
    motp->mot_read();
    uint16_t mstati(0);
    uint16_t mstat(0);
    cout << "starting next command " << gx << endl;
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
}


int main (int argc, char** argv)
{
	uint8_t mask(0);
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2), smot(4);
	uint64_t xmm(0), ymm(0), sum(0);
	uint64_t xsteps(0), ysteps1(0), ysteps2(0), dys(0),
	        ssteps1(0), ssteps2(0), dss(0);
	uint64_t xspeed(5), yspeed(5), sspeed(60);
	bool xposdir(true), yposdir(true), sposdir(true);
	uint64_t xcoded(0), ycoded1(0), ycoded2(0), scoded1(0), scoded2(0);
	uint32_t nloops(0);


	arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
	motp = new motors("/dev/ttyUSB1");
        optline  optl("/dev/ttyUSB3");

        float length(0);

	xcoded = xmot*100000*100*100;
	scoded1 = smot*100000*100*100;
	scoded2 = smot*100000*100*100;
	ycoded1 = ymot*100000*100*100;
	ycoded2 = ymot*100000*100*100;

	if (argc < 4)  return __LINE__;
	-- argc; ++ argv;

	do
         {
	  if ((strcasecmp(*argv,"n")==0) || (strcasecmp(*argv,"-n")==0)) {
	    if (argc < 2) return __LINE__;

	    -- argc; ++ argv;
	    int32_t nc = strtol(*argv, NULL, 10);
	    if (nc > 0) nloops = (uint32_t)nc;

	  } else if ((strcasecmp(*argv,"x")==0) || (strcasecmp(*argv,"-x")==0)) {
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    int64_t mm = strtol(*argv, NULL, 10);
	    xposdir = (mm > 0);
	    if (!xposdir) mm = -mm;
	    xmm = uint64_t(mm);
	    xsteps = xmm*25;

	    -- argc; ++ argv;
	    xspeed = strtol(*argv, NULL, 10);

	    xcoded = ((xmot * 100000 + xsteps)*100 + xspeed)*100 + ramp;
	    mask |= 1;

	  } else if ((strcasecmp(*argv,"y")==0) || (strcasecmp(*argv,"-y")==0)) {
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    int64_t mm = strtol(*argv, NULL, 10);
	    yposdir = (mm > 0);
	    if (!yposdir) mm = -mm;
	    ymm = uint64_t(mm);
	    ysteps1 = ymm*25;

	    -- argc; ++ argv;
	    yspeed = strtol(*argv, NULL, 10);

	    ycoded1 = ((ymot * 100000 + ysteps1)*100 + yspeed)*100 + ramp;
	    mask |= 2;

	  } else if ((strcasecmp(*argv,"spint")==0) || (strcasecmp(*argv,"-s")==0)) {
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    char* wherecomma = index(*argv, ',');
	    if (wherecomma != NULL) *wherecomma = '.';
	    double mm = strtod(*argv, NULL);
	    sposdir = (mm > 0);
	    if (!sposdir) mm = -mm;
	    sum = uint64_t(mm*1000+0.5);
	    ssteps1 = (sum*16)/3;

	    -- argc; ++ argv;
	    sspeed = strtol(*argv, NULL, 10);

	    scoded1 = ((smot * 100000 + ssteps1)*100 + sspeed)*100 + ramp;
	    mask |= 4;

	  } else  return __LINE__;
	  ++ argv;
	 }
	while (-- argc);

	double ddy = 2*double(xmm)*tan(10.5*atan(1)/45);
	dys = uint64_t(ddy*25+0.5);
	if (ysteps1 > dys) ysteps2 = ysteps1 - dys;
	ycoded2 = ((ymot * 100000 + ysteps2)*100 + yspeed)*100 + ramp;

	double dsum = double(sum)*double(dys)/double(ysteps1);
	double dsst = double(ssteps1)*double(dys)/double(ysteps1);
	dss = uint64_t(dsst+0.5);
	if (ssteps1 > dss) ssteps2 = ssteps1 - dss;
	scoded2 = ((smot * 100000 + ssteps2)*100 + sspeed)*100 + ramp;

	cout << " dx(mm) dy(mm) ds(um) " << xmm << " " << ddy << " " << dsum
	     << " dx(st) dy(st) ds(st) " << xsteps << " " << dys << " " << dss
	     << endl;

	string rprog1 = "r238\r\n";
	string gprog1 = "g238\r\n";
	string mprog1_h = "m238-";
	string rprog2 = "r239\r\n";
	string gprog2 = "g239\r\n";
	string mprog2_h = "m239-";

	stringstream mxcmd1, mycmd1, mscmd1;
	stringstream mxcmd2, mycmd2, mscmd2;

	mxcmd1 << mprog1_h << setw(10) << xcoded << (xposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mycmd1 << mprog1_h << setw(10) << ycoded1 << (yposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mscmd1 << mprog1_h << setw(10) << scoded1 << (sposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mxcmd2 << mprog2_h << setw(10) << xcoded << (xposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mycmd2 << mprog2_h << setw(10) << ycoded2 << (yposdir ? '-' : '+')
	       << setw(1) << func << '\r' << endl;
	mscmd2 << mprog2_h << setw(10) << scoded2 << (sposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << xmot
	     << "\n passi [-s] " << xsteps
	     << "\n velocità [-v] " << xspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << xposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mxcmd1.str()
	     << " stringa2 " << mxcmd2.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << ymot
	     << "\n passi1 [-s] " << ysteps1
	     << "\n passi2 [-s] " << ysteps2
	     << "\n velocità [-v] " << yspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << yposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mycmd1.str()
	     << " stringa2 " << mycmd2.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << smot
	     << "\n passi1 [-s] " << ssteps1
	     << "\n passi2 [-s] " << ssteps2
	     << "\n velocità [-v] " << sspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << sposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mscmd1.str()
	     << " stringa2 " << mscmd2.str();
 if (1) cout << "-----------------------------------------------------"
	     << "\n # giri [-n] " << nloops
	     << endl;
 if (1) cout << "-----------------------------------------------------"
	     << endl;

	if (nloops == 0)
	 {
		cout << " *** NUMERO GIRI NON VALIDO (= " << nloops << ") *** " << endl;
			 return __LINE__;
		 }
		uint16_t nmots(0);
		if (mask & 1)
		 {
		  if ((xsteps > 99999) || (xsteps == 0))
		   {
			cout << " *** SPOSTAMENTO ASSE X NON VALIDO (= " << xsteps << ") *** " << endl;
			 return __LINE__;
		   }
		  if ((xspeed > 99) || (xspeed == 0))
		   {
			cout << " *** VELOCITA' ASSE X NON VALIDA (= " << xspeed << ") *** " << endl;
			 return __LINE__;
		   }
		  nmots ++;
		 }
		if (mask & 2)
		 {
		  if ((ysteps1 > 99999) || (ysteps1 == 0) || (ysteps1 < dys))
		   {
			cout << " *** SPOSTAMENTO ASSE Y NON VALIDO (= " << ysteps1 << ") *** " << endl;
			 return __LINE__;
		   }
		  if ((yspeed > 99) || (yspeed == 0))
		   {
			cout << " *** VELOCITA' ASSE Y NON VALIDA (= " << yspeed << ") *** " << endl;
			 return __LINE__;
		   }
		  nmots ++;
		 }
		if (mask & 4)
		 {
		  if ((ssteps1 > 99999) || (ssteps1 == 0) || (ssteps1 < dss))
		   {
			cout << " *** SPOSTAMENTO SPINTORE NON VALIDO (= " << ssteps1 << ") *** " << endl;
			 return __LINE__;
		   }
		  if ((sspeed > 99) || (sspeed == 0))
		   {
			cout << " *** VELOCITA' SPINTORE NON VALIDA (= " << sspeed << ") *** " << endl;
			 return __LINE__;
		   }
		  nmots ++;
		 }

		if (nmots < 2)
		   {
			cout << " *** UN SOLO MOTORE SELEZIONATO (= " << mask << ") *** " << endl;
			 return __LINE__;
		   }



	motp->mot_write(mxcmd1.str().c_str());
	motp->mot_read();
	motp->mot_write(mycmd1.str().c_str());
	motp->mot_read();
	motp->mot_write(mscmd1.str().c_str());
	motp->mot_read();
	motp->mot_write(rprog1.c_str());
	motp->mot_read();
	motp->mot_write(mxcmd2.str().c_str());
	motp->mot_read();
	motp->mot_write(mycmd2.str().c_str());
	motp->mot_read();
	motp->mot_write(mscmd2.str().c_str());
	motp->mot_read();
	motp->mot_write(rprog2.c_str());
	motp->mot_read();
	cout.flush();

	string prog1 = "g238\r\n";
	string prog2 = "g239\r\n";
	string ycmd1str = mycmd1.str();
	string ycmd2str = mycmd2.str();
	string scmd1str = mscmd1.str();
	string scmd2str = mscmd2.str();
	bool ispos(true);
	const char* progmv = gprog1.c_str();
	const char* ynxtprog = ycmd2str.c_str();
	const char* snxtprog = scmd2str.c_str();
	uint64_t ynxtsteps = ysteps2;
	uint64_t snxtsteps = ssteps2;
	cout << " ycmd1 " << ycmd1str;
	cout << " scmd1 " << scmd1str;

	std:vector<float>  ola; 

while (nloops --)
 {

                if (*cmd != '\0')	// **** PIPE ****
                 {
                  printf("found string %s\n", cmd);
                  if (strcasecmp("pause", cmd) == 0)
                   {
                    *cmd = '\0';
                    do 
                     { 
                      usleep(100);
                     } 
                    while (!( strcasecmp("resume", cmd) == 0 ||
                              strcasecmp("quit", cmd) == 0 ||
                              strcasecmp("kill", cmd) == 0 ));
                   }
                  if (strcasecmp("quit", cmd) == 0) goto QuitNow;
                  if (strcasecmp("kill", cmd) == 0) exit(1);
                  *cmd = '\0';
                 }

	motp->mot_write(progmv);
	motp->mot_read();
	cout << nloops << " progmv " << progmv;
	motp->mot_write(ynxtprog);
	motp->mot_read();
	motp->mot_write(snxtprog);
	motp->mot_read();
	cout << nloops << " ynxtprog " << ynxtprog;
	cout << nloops << " snxtprog " << snxtprog;
	cout.flush();
	ispos = not ispos;
	progmv = (ispos) ? gprog1.c_str() : gprog2.c_str();
	ynxtprog = (ispos) ? ycmd2str.c_str() : ycmd1str.c_str();
	snxtprog = (ispos) ? scmd2str.c_str() : scmd1str.c_str();
	if ((ynxtsteps > dys) && (snxtsteps > dss)){
	  char stemp[8];
	  ynxtsteps -= dys; 
	  sprintf(stemp, "%u", ymot*100000+ynxtsteps);
	  memcpy((void*)(ynxtprog+5), stemp, 6);
          cout << "length " << length << " mm " << endl;
          length +=snxtsteps*3/16000;
          cout << snxtsteps*3/16000 << " mm; tot " << length << " mm "  <<  endl;
          if(length > 100) return 0;
          snxtsteps -= dss; 
	  sprintf(stemp, "%u", smot*100000+snxtsteps);
	  memcpy((void*)(snxtprog+5), stemp, 6);
	 }
	else{
	  if (nloops > 1) nloops = 1;
	}
        next(progmv, myboard);
        ola = optl.readlineXYZ();
	cout << " x " << ola.at(0) << " y " << ola.at(1) << endl;
	//uint16_t mstat(0);
	//usleep(10000);
	//do
	// {
        //  mstat = a->fp_discrete();
	//  usleep(10000);
	// }
	//while (mstat & 0x3f);
 }
 


QuitNow:

  delete motp;

  return 0;
 }

