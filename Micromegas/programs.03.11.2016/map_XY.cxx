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
#include "gauge.h"
#include "optline.h"
#include "motors.h"

#include "my_pipe.h"

std::string pipe_name("/tmp/atlas/mapXYpipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {0};
my_pipe p(pipe_name.c_str(), cmd);



#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M0Data/PreserieFrame/"

using namespace std;

uint16_t imotlist[] { 231, 232, 233, 234, 235, 236 };
string smotlist[] { "231", "232", "233", "234", "235", "236" };

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
  //      std::cout << "s" << std::endl;
//	std::cout << myboard->digitalInput(chan) << std::endl;
	mstati = myboard->digitalInput(chan);
	mstat += mstati;
    }   
    usleep(10000);
  }
  while (mstat > 0);
  usleep(2000000);
 }

void misura( int64_t x, int64_t y, gauge* tast, optline* optl, arduinoX* myboard, uint16_t t_ch1, uint16_t t_ch2, FILE* logf )
 {
	double t_tab = myboard->getPhyVal(t_ch1);
	double t_amb = myboard->getPhyVal(t_ch2);
	double  utast = tast->readval();
	double ol = optl->readline();
	fprintf( logf, " %d \t %d \t %f \t %f \t %f \t %f  \n", x, y, ol, utast, t_tab, t_amb  );
	fflush( logf );
	printf( " %d \t %d \t %f \t %f \t %f \t %f  \n", x, y, ol, utast, t_tab, t_amb  );
	fflush( stdout );
 }

int main (int argc, char** argv)
{
	uint8_t mask(0);
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2), tmot(6);
	uint64_t xmm(0), tum(0);
	float_t ymm1(0), ymm2(0);
	uint64_t xsteps(0), ysteps1(0), ysteps2(0), tsteps(0);
	uint64_t xspeed(5), yspeed(5), tspeed(60);
	bool xposdir(true), yposdir1(true),yposdir2(true), tposdir(true);
	uint64_t xcoded(0), ycoded(0), ycoded2(0), tcoded(0);
	int32_t xdim(0); 
	uint64_t ydim(0);
	int32_t ntotx(0), ntoty(0);

	arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");

        gauge tast("/dev/ttyUSB2");		// modificato, invertito 2 e 3
	optline myoptl("/dev/ttyUSB3");

	//fp_do_401* do_led = fp_do_401::create(8);
	//do_led->fp_discrete(2, true);

	uint16_t mod(0);
	uint16_t chan(0);
	double RFS, RZS, EFS, EZS;
	std::string name, unit, descr;
	std::string line;
	
	std::fstream f("/home/atlas/Micromegas/inputfile/arduino_analog.txt", std::fstream::in);
	while (getline(f,line))
	 {
	  stringstream s(line);
	  s >> name;
	  if (name == "#") continue;
	  s >> mod >> chan >> RFS >> RZS >> EFS >> EZS >> unit >> descr;
	  cout << "XXXXX name " << name << " mod " << mod << " chan " << chan << " RFS "
               << RFS << " RZS " << RZS << " EFS " << EFS << " EZS " << EZS
               << " unit " << unit << " descr " << descr << endl;
	  myboard->setPhysScale( chan, RFS, RZS, EFS, EZS);
	 }
	//fp_di_XXX* dimot = fp_di_XXX::create(5);

	xcoded = xmot*100000*100*100;
	tcoded = tmot*100000*100*100;
	ycoded = ymot*100000*100*100;
	ycoded2 =ymot*100000*100*100;

	if (argc < 4)  return __LINE__;
	-- argc; ++ argv;

	do
         {
	  if ((strcasecmp(*argv,"w")==0) || (strcasecmp(*argv,"-w")==0)) {
	    if (argc < 3) return __LINE__;

	    -- argc; ++ argv;
	    xdim = strtol(*argv, NULL, 10);

	//    ydim = strtol(*argv, NULL, 10);
 	//    ysteps2=ydim*25;

	    -- argc; ++ argv;
	    ntoty = strtol(*argv, NULL, 10);

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
	    int64_t mm1 = strtol(*argv, NULL, 10);
	    yposdir1 = (mm1 > 0);
	    if (!yposdir1) mm1 = -mm1;
	    ymm1 = mm1;
	    ysteps1 = uint64_t(ymm1*25);

	    -- argc; ++ argv;
	    float_t mm2 = strtof(*argv, NULL);
	    yposdir2 = (mm2 > 0);
	    if (!yposdir2) mm2 = -mm2;
	    ymm2 = mm2;
	    ysteps2 = uint64_t(ymm2*25);
	    
	    -- argc; ++ argv;
	    yspeed = strtol(*argv, NULL, 10);

	    ycoded = ((ymot * 100000 + ysteps1)*100 + yspeed)*100 + ramp;
	    ycoded2 = ((ymot * 100000 + ysteps2)*100 + yspeed)*100 + ramp;
	    
            cout << ycoded << " " << mm1  << " " << ymm1  << " " << ysteps1 << endl;
            cout << ycoded2 << " " << mm2  << " " << ymm2  << " " << ysteps2 << endl;

	     mask |= 2;

	  } else if ((strcasecmp(*argv,"t")==0) || (strcasecmp(*argv,"-t")==0)) {
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    char* wherecomma = index(*argv, ',');
	    if (wherecomma != NULL) *wherecomma = '.';
	    double mm = strtod(*argv, NULL);
	    tposdir = (mm > 0);
	    if (!tposdir) mm = -mm;
	    tum = uint64_t(mm*1000+0.5);
	    // gg: modificato passo per asse z 2/5 -> 1/5
	    tsteps = (tum*1)/5;

	    -- argc; ++ argv;
	    tspeed = strtol(*argv, NULL, 10);

	    tcoded = ((tmot * 100000 + tsteps)*100 + tspeed)*100 + ramp;
	    mask |= 4;

	  } else  return __LINE__;
	  ++ argv;
	 }
	while (-- argc);


//	ycoded2 = ((ymot * 100000 - ysteps2)*100 + yspeed)*100 + ramp;
//        cout << ycoded2 << endl; 

	cout << " dx(mm) dy(mm) ds(um) " << xmm << " " << ymm1 << " " << ymm2 << " " << tum
	     << endl;
	if (xmm) ntotx = (xdim / xmm) + 1;
//	if (ymm) ntoty = (ydim / ymm) + 1;
	cout << " xdim ydim ntotx ntoty " << xdim << " " << ydim << " "
             << ntotx << " " << ntoty << " " << endl;

	string rx1 = "r" + smotlist[0] + "\r\n";
	string gx1 = "g" + smotlist[0] + "\r\n";
	string mx1 = "m" + smotlist[0] + "-";
	string rx2 = "r" + smotlist[1] + "\r\n";
	string gx2 = "g" + smotlist[1] + "\r\n";
	string mx2 = "m" + smotlist[1] + "-";
	string ry1 = "r" + smotlist[2] + "\r\n";
	string gy1 = "g" + smotlist[2] + "\r\n";
	string my1 = "m" + smotlist[2] + "-";
	string rzup = "r" + smotlist[3] + "\r\n";
	string gzup = "g" + smotlist[3] + "\r\n";
	string mzup = "m" + smotlist[3] + "-";
	string rzdw = "r" + smotlist[4] + "\r\n";
	string gzdw = "g" + smotlist[4] + "\r\n";
	string mzdw = "m" + smotlist[4] + "-";
	string ry2 = "r" + smotlist[5] + "\r\n";
	string gy2 = "g" + smotlist[5] + "\r\n";
	string my2 = "m" + smotlist[5] + "-";

	stringstream mx1cmd, mx2cmd, my1cmd, my2cmd;
	stringstream mzupcmd, mzdwcmd;

	mx1cmd << mx1 << setw(10) << xcoded << (xposdir ? '+' : '-')
	      << setw(1) << func << '\r' << endl;
	mx2cmd << mx2 << setw(10) << xcoded << (xposdir ? '-' : '+')
	      << setw(1) << func << '\r' << endl;
	my1cmd << my1 << setw(10) << ycoded << (yposdir1 ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	my2cmd << my2 << setw(10) << ycoded2 << (yposdir2 ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mzupcmd << mzup << setw(10) << tcoded << (tposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mzdwcmd << mzdw << setw(10) << tcoded << (tposdir ? '-' : '+')
	       << setw(1) << func << '\r' << endl;
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << xmot
	     << "\n passi [-s] " << xsteps
	     << "\n velocità [-v] " << xspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << xposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mx1cmd.str()
	     << " stringa2 " << mx2cmd.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << ymot
	     << "\n passi [-s] " << ysteps1
	     << "\n velocità [-v] " << yspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << yposdir1
	     << "\n funzione [-f] " << func
	     << "\n stringa " << my1cmd.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << ymot
	     << "\n passi [-s] " << ysteps2
	     << "\n velocità [-v] " << yspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << yposdir2
	     << "\n funzione [-f] " << func
	     << "\n stringa " << my2cmd.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << tmot
	     << "\n passi [-s] " << tsteps
	     << "\n velocità [-v] " << tspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << tposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mzupcmd.str()
	     << " stringa2 " << mzdwcmd.str();
 if (1) cout << "-----------------------------------------------------"
	     << endl;

	if ((xdim == 0) || (xmm == 0) || (xdim % xmm))
	 {
		cout << " ***  MOVIMENTI ASSE X NON VALIDI (Range = " << xdim << ", Passo = " << xmm << ") *** " << endl;
		 return __LINE__;
	 }
	/* 
	if ((ydim == 0) || (ymm == 0) || (ydim % ymm))
	 {
		cout << " ***  MOVIMENTI ASSE Y NON VALIDI (Range = " << ydim << ", Passo = " << ymm << ") *** " << endl;
		 return __LINE__;
	 } */
	 
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
	  if ((ysteps1 > 99999)|| (ysteps2 > 99999) || (ysteps1 == 0) || (ysteps2 == 0)  )
	   {
		cout << " *** SPOSTAMENTO ASSE Y NON VALIDO (= " << ysteps1 << " " << ysteps2 << ") *** " << endl;
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
	  if ((tsteps > 99999) || (tsteps == 0))
	   {
		cout << " *** SPOSTAMENTO ASSE Z NON VALIDO (= " << tsteps << ") *** " << endl;
		 return __LINE__;
	   }
	  if ((tspeed > 99) || (tspeed == 0))
	   {
		cout << " *** VELOCITA' ASSE Z NON VALIDA (= " << tspeed << ") *** " << endl;
		 return __LINE__;
	   }
	  nmots ++;
	 }

	if (nmots < 2)
	   {
		cout << " *** UN SOLO MOTORE SELEZIONATO (= " << mask << ") *** " << endl;
		 return __LINE__;
	   }

       cout << "writing command to the motors " << endl;


	motp = new motors("/dev/ttyUSB1");

	for (size_t i=0; i<6; i++) motp->mot_reset(imotlist[i]);

	motp->mot_write(mx1cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rx1.c_str());
	motp->mot_read();
	motp->mot_write(mx2cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rx2.c_str());
	motp->mot_read();
	motp->mot_write(my1cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(ry1.c_str());
	motp->mot_read();
	motp->mot_write(my2cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(ry2.c_str());
	motp->mot_read();
	motp->mot_write(mzupcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rzup.c_str());
	motp->mot_read();
	motp->mot_write(mzdwcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rzdw.c_str());
	motp->mot_read();
	cout.flush();
	
	int64_t x(0);
	int64_t y(0);
	bool back(true);
	uint16_t laserch(2);
	uint16_t t_ch1(7);
	uint16_t t_ch2(9);

	time_t now = time(0);
        tm *ltm = localtime(&now);

        ostringstream outname;
        ostringstream date;
        date << 1900+ ltm->tm_year;
        if (1+ltm->tm_mon < 10 ) date << "0" << 1+ltm->tm_mon << ltm->tm_mday ;
        else date << 1+ltm->tm_mon << ltm->tm_mday;
        cout << date.str() << endl;
        int32_t min = ltm->tm_min + ltm->tm_hour*60 + 10000; // aggiunto minuto
        outname << path << "mapping_" << date.str() << min << ".txt";

        cout << " Ready to start " << endl;


	FILE* logf = fopen (outname.str().c_str(),"w+");
 
	for (uint32_t ny=0; ny < ntoty; ny++)
	 {
	  for (uint32_t nx=0; nx < ntotx; nx++)
	   {

                if (*cmd != '\0')	// ********************** PIPE ****************************************
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

		next( gzdw.c_str(), myboard );
		misura( x, y, &tast, &myoptl, myboard, t_ch1, t_ch2, logf );
		next( gzup.c_str(), myboard );
		if(nx!=ntotx-1){ 
		   next( back ? gx1.c_str() : gx2.c_str(), myboard );
		   x = x + (back ? xmm : -xmm);
	        }
          }
	  if(ny!=ntoty-1) next(back ? gy1.c_str() :gy2.c_str(), myboard );
	  back ? y = y + ymm1 : y=y + ymm2;
	  back = not back;
	 }


QuitNow:
	fclose(logf);

	//do_led->fp_discrete(2, false);

	delete motp;

	 arduinoX::cleanup();

	return 0;
 }

