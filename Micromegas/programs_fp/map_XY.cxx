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

#include "fp_ai_100.h"
#include "fp_do_401.h"
#include "fp_di_XXX.h"
#include "gauge.h"
#include "optline.h"
#include "motors.h"

#include "my_pipe.h"

std::string pipe_name("/tmp/atlas/mapXYpipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {0};
my_pipe p(pipe_name.c_str(), cmd);



#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M0Data/mapping/"

using namespace std;

uint16_t imotlist[] { 231, 232, 233, 234, 235 };
string smotlist[] { "231", "232", "233", "234", "235" };

motors* motp(NULL);

void next ( const char* gx, fp_di_XXX* di )
 {
  motp->mot_write(gx);
  motp->mot_read();
  uint16_t mstat(0);
  do
   {
    mstat = di->fp_discrete();
    usleep(10000);
   }
  while (mstat & 0x3f);
 }

void misura( int64_t x, int64_t y, gauge* tast, optline* optl, fp_ai_100* ai, uint16_t laserch, fp_ai_100* ait, uint16_t t_ch1, uint16_t t_ch2, FILE* logf )
 {
	double ls = ai->getPhyVal( laserch );
	double t_tab = ait->getPhyVal(t_ch1);
	double t_amb = ait->getPhyVal(t_ch2);
	double  utast = tast->readval();
	double ol = optl->readline();
	fprintf( logf, " %d \t %d \t %f \t %f \t %f \t %f \t %f  \n", x, y, ol, ls, utast, t_tab, t_amb  );
	fflush( logf );
	printf( " %d \t %d \t %f \t %f \t %f \t %f \t %f  \n", x, y, ol, ls, utast, t_tab, t_amb  );
	fflush( stdout );
 }

int main (int argc, char** argv)
{
	uint8_t mask(0);
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2), tmot(6);
	uint64_t xmm(0), ymm(0), tum(0);
	uint64_t xsteps(0), ysteps(0), tsteps(0);
	uint64_t xspeed(5), yspeed(5), tspeed(60);
	bool xposdir(true), yposdir(true), tposdir(true);
	uint64_t xcoded(0), ycoded(0), tcoded(0);
	int32_t xdim(0), ydim(0);
	int32_t ntotx(0), ntoty(0);

        fp_mod::fp_tty("/dev/ttyUSB0");
	gauge tast("/dev/ttyUSB2");		// modificato, invertito 2 e 3
	optline myoptl("/dev/ttyUSB3");

	fp_do_401* do_led = fp_do_401::create(8);
	do_led->fp_discrete(2, true);

	uint16_t mod(0);
	uint16_t chan(0);
	double RFS, RZS, EFS, EZS;
	std::string name, unit, descr;
	std::string line;
	std::vector <fp_ai_100*> b;
	std::fstream f("/home/atlas/Micromegas/inputfile/fpchannels_all.txt", std::fstream::in);
	while (getline(f,line))
	 {
	  stringstream s(line);
	  s >> name;
	  if (name == "#") continue;
	  s >> mod >> chan >> RFS >> RZS >> EFS >> EZS >> unit >> descr;
	  cout << "XXXXX name " << name << " mod " << mod << " chan " << chan << " RFS "
               << RFS << " RZS " << RZS << " EFS " << EFS << " EZS " << EZS
               << " unit " << unit << " descr " << descr << endl;
	  fp_ai_100* m = fp_ai_100::create(mod);
	  m->setPhysScale( chan, RFS, RZS, EFS, EZS);
	  b.push_back(m);
	 }
	fp_ai_100* ailaser = fp_ai_100::create(4);
	fp_ai_100* aitemp = fp_ai_100::create(3);
	fp_di_XXX* dimot = fp_di_XXX::create(5);

	xcoded = xmot*100000*100*100;
	tcoded = tmot*100000*100*100;
	ycoded = ymot*100000*100*100;

	if (argc < 4)  return __LINE__;
	-- argc; ++ argv;

	do
         {
	  if ((strcasecmp(*argv,"w")==0) || (strcasecmp(*argv,"-w")==0)) {
	    if (argc < 2) return __LINE__;

	    -- argc; ++ argv;
	    xdim = strtol(*argv, NULL, 10);

	    -- argc; ++ argv;
	    ydim = strtol(*argv, NULL, 10);

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
	    ysteps = ymm*25;

	    -- argc; ++ argv;
	    yspeed = strtol(*argv, NULL, 10);

	    ycoded = ((ymot * 100000 + ysteps)*100 + yspeed)*100 + ramp;
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
	    tsteps = (tum*2)/5;

	    -- argc; ++ argv;
	    tspeed = strtol(*argv, NULL, 10);

	    tcoded = ((tmot * 100000 + tsteps)*100 + tspeed)*100 + ramp;
	    mask |= 4;

	  } else  return __LINE__;
	  ++ argv;
	 }
	while (-- argc);

	cout << " dx(mm) dy(mm) ds(um) " << xmm << " " << ymm << " " << tum
	     << endl;
	if (xmm) ntotx = (xdim / xmm) + 1;
	if (ymm) ntoty = (ydim / ymm) + 1;
	cout << " xdim ydim ntotx ntoty " << xdim << " " << ydim << " "
             << ntotx << " " << ntoty << " " << endl;

	string rx1 = "r" + smotlist[0] + "\r\n";
	string gx1 = "g" + smotlist[0] + "\r\n";
	string mx1 = "m" + smotlist[0] + "-";
	string rx2 = "r" + smotlist[1] + "\r\n";
	string gx2 = "g" + smotlist[1] + "\r\n";
	string mx2 = "m" + smotlist[1] + "-";
	string ry = "r" + smotlist[2] + "\r\n";
	string gy = "g" + smotlist[2] + "\r\n";
	string my = "m" + smotlist[2] + "-";
	string rzup = "r" + smotlist[3] + "\r\n";
	string gzup = "g" + smotlist[3] + "\r\n";
	string mzup = "m" + smotlist[3] + "-";
	string rzdw = "r" + smotlist[4] + "\r\n";
	string gzdw = "g" + smotlist[4] + "\r\n";
	string mzdw = "m" + smotlist[4] + "-";

	stringstream mx1cmd, mx2cmd, mycmd;
	stringstream mzupcmd, mzdwcmd;

	mx1cmd << mx1 << setw(10) << xcoded << (xposdir ? '+' : '-')
	      << setw(1) << func << '\r' << endl;
	mx2cmd << mx2 << setw(10) << xcoded << (xposdir ? '-' : '+')
	      << setw(1) << func << '\r' << endl;
	mycmd << my << setw(10) << ycoded << (yposdir ? '+' : '-')
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
	     << "\n passi [-s] " << ysteps
	     << "\n velocità [-v] " << yspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << yposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa " << mycmd.str();
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
	 
	if ((ydim == 0) || (ymm == 0) || (ydim % ymm))
	 {
		cout << " ***  MOVIMENTI ASSE Y NON VALIDI (Range = " << ydim << ", Passo = " << ymm << ") *** " << endl;
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
	  if ((ysteps > 99999) || (ysteps == 0))
	   {
		cout << " *** SPOSTAMENTO ASSE Y NON VALIDO (= " << ysteps << ") *** " << endl;
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

        cout << hex << " id: " << dimot->fp_moduleID() << dec << endl;
        cout << hex << " all ids: " << dimot->fp_allIDs() << dec << endl;
        cout << hex << " status: " << dimot->fp_status() << dec << endl;
        cout << hex << " range(2): " << dimot->fp_range( 2 ) << dec << endl;
        cout << hex << " discrete: " << dimot->fp_discrete() << dec << endl;
        cout << hex << " status(5): " << dimot->fp_status( 5 ) << dec << endl;

	motp = new motors("/dev/ttyUSB1");

	for (size_t i=0; i<5; i++) motp->mot_reset(imotlist[i]);

	motp->mot_write(mx1cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rx1.c_str());
	motp->mot_read();
	motp->mot_write(mx2cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(rx2.c_str());
	motp->mot_read();
	motp->mot_write(mycmd.str().c_str());
	motp->mot_read();
	motp->mot_write(ry.c_str());
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
	uint16_t t_ch1(3);
	uint16_t t_ch2(5);

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

		//next( gzdw.c_str(), dimot );
		misura( x, y, &tast, &myoptl, ailaser, laserch, aitemp, t_ch1, t_ch2, logf );
		//next( gzup.c_str(), dimot );
		if(nx!=ntotx-1){ 
		   next( back ? gx1.c_str() : gx2.c_str(), dimot );
		   x = x + (back ? xmm : -xmm);
	        }
          }
	  if(ny!=ntoty-1) next( gy.c_str(), dimot );
	  back = not back;
	  y = y + ymm;
	 }

QuitNow:
	fclose(logf);

	do_led->fp_discrete(2, false);

	delete motp;

	fp_do_401::cleanup();
	fp_ai_100::cleanup();
	fp_di_XXX::cleanup();

	return 0;
 }

