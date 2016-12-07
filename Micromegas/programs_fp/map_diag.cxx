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

#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M0Data/mapping/"
#define PI 3.14159265

std::string pipe_name("/tmp/atlas/mapdiagpipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {'\0'};
my_pipe p(pipe_name.c_str(), cmd);

using namespace std;

// parametri da utilizzare: ./map_diag -l +-lato -s step vstep -t 3 10 -e (dx/sx)

uint16_t imotlist[] { 231, 232, 233, 234, 235, 236 };
string smotlist[] { "231", "232", "233", "234", "235", "236"};

motors* motp(NULL);

void next ( const char* gx, fp_di_XXX* di )
 {
  motp->mot_write(gx );
  motp->mot_read();
  uint16_t mstat(0);
  do{
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



// *** MAIN ***

int main (int argc, char** argv)
{
	uint8_t mask(0);
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2), tmot(6);
	double  xmm(0), ymm(0), lmm(0), smm(0), tum(0);
	//uint64_t xmm(0), ymm(0), lmm(0), smm(0), tum(0);
	uint64_t xsteps(0), ysteps(0), nsteps(0), tsteps(0);
	uint64_t sspeed(5), tspeed(60);
	bool ldir(true), yposdir(true), tposdir(true), edge(true);
	uint64_t xcoded(0), ycoded(0), ycoded2(0), tcoded(0);
	int32_t xdim(0), ydim(0);
	int32_t ntotx(0), ntoty(0);

        motp = new motors("/dev/ttyUSB1");

        fp_mod::fp_tty("/dev/ttyUSB0");
	gauge tast("/dev/ttyUSB2");
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

	if (argc < 5)  return __LINE__;
	-- argc; ++ argv;

	do			
         {
	  if ((strcasecmp(*argv,"l")==0) || (strcasecmp(*argv,"-l")==0)) {	// lunghezza lato, segno da direzione
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    int64_t mm = strtol(*argv, NULL, 10);
	    ldir = (mm > 0);
	    if (!ldir) mm = -mm;
	    lmm = uint64_t(mm);

            -- argc; ++ argv;
	    edge = strtol(*argv, NULL, 10);

	  } else if ((strcasecmp(*argv,"s")==0) || (strcasecmp(*argv,"-s")==0)) {	// passo e velocita'
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    int64_t mm = strtol(*argv, NULL, 10);
	    smm = uint64_t(mm);

	    -- argc; ++ argv;
	    sspeed = strtol(*argv, NULL, 10);


	  } else if ((strcasecmp(*argv,"t")==0) || (strcasecmp(*argv,"-t")==0)) {	// *** INIZIALIZZO T ****************
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

	  } else  return __LINE__;
	  ++ argv;
	 }
	while (-- argc);


	cout << "************************" <<endl;
	cout << "measure for "  << lmm << " mm along the " << (edge ? "left" : "right") << "side (looking from pc)";
	cout << "toward " << (ldir ? "PC" : "DOOR" ) << " with step of " << smm << " mm "; 
	cout << "************************" <<endl;
	
        if (smm) nsteps = lmm/smm;
	xmm = smm * cos(10.5*PI/180.0);	
	ymm = smm * sin(10.5*PI/180.0);	

	//xcoded = ((xmot * 100000 + xmm*25)*100 + sspeed)*100 + ramp;
	//ycoded = ((ymot * 100000 + ymm*25)*100 + sspeed)*100 + ramp;
	
	xcoded = ((xmot * 100000 + uint64_t(xmm*25))*100 + sspeed)*100 + ramp;
	ycoded = ((ymot * 100000 + uint64_t(ymm*25))*100 + sspeed)*100 + ramp;

 	cout << "smm " << smm << " xmm " << xmm << " ymm " << ymm << endl;
	cout << xcoded << endl;
	cout << ycoded << endl;
	 	

        // programma 231 per movimento up in z
	string rzup = "r" + smotlist[0] + "\r\n";   // stringa "r231\r\n"
	string gzup = "g" + smotlist[0] + "\r\n";   // stringa "g231\r\n"
	string mzup = "m" + smotlist[0] + "-";      // stringa "m231-"
        // programma 232 per movimento down in z
	string rzdw = "r" + smotlist[1] + "\r\n";   // stringa "r232\r\n"
	string gzdw = "g" + smotlist[1] + "\r\n";   // stringa "g232\r\n"
	string mzdw = "m" + smotlist[1] + "-";      // stringa "m232-"
        // programma 233 per movimento diag lato sx verso pc
	string rsxpc = "r" + smotlist[2] + "\r\n";    // stringa "r233\r\n"
	string gsxpc = "g" + smotlist[2] + "\r\n";    // stringa "g233\r\n"
	string msxpc = "m" + smotlist[2] + "-";       // stringa "m233-"
        // programma 234 per movimento diag lato sx verso door
	string rsxdo = "r" + smotlist[3] + "\r\n";    // stringa "r234\r\n"
	string gsxdo = "g" + smotlist[3] + "\r\n";    // stringa "g234\r\n"
	string msxdo = "m" + smotlist[3] + "-";       // stringa "m234-"
        // programma 235 per movimento diag lato dx verso pc
	string rdxpc = "r" + smotlist[4] + "\r\n";    // stringa "r235\r\n"
	string gdxpc = "g" + smotlist[4] + "\r\n";    // stringa "g235\r\n"
	string mdxpc = "m" + smotlist[4] + "-";       // stringa "m235-"
        // programma 236 per movimento diag lato dx verso door
	string rdxdo = "r" + smotlist[5] + "\r\n";    // stringa "r236\r\n"
	string gdxdo = "g" + smotlist[5] + "\r\n";    // stringa "g236\r\n"
	string mdxdo = "m" + smotlist[5] + "-";       // stringa "m236-"

	stringstream m1cmd, m2cmd, m3cmd, m4cmd, m5cmd, m6cmd, m7cmd, m8cmd;
	stringstream mzupcmd, mzdwcmd;

	m1cmd << msxpc << setw(10) << xcoded <<  '+'   << setw(1) << func << '\r' << endl;
	m2cmd << msxpc << setw(10) << ycoded <<  '-'   << setw(1) << func << '\r' << endl;

	m3cmd << msxdo << setw(10) << xcoded <<  '-'  << setw(1) << func << '\r' << endl;
	m4cmd << msxdo << setw(10) << ycoded <<  '+'  << setw(1) << func << '\r' << endl;

	m5cmd << mdxpc << setw(10) << xcoded <<  '+'   << setw(1) << func << '\r' << endl;
	m6cmd << mdxpc << setw(10) << ycoded <<  '+'   << setw(1) << func << '\r' << endl;

	m7cmd << mdxdo << setw(10) << xcoded <<  '-'  << setw(1) << func << '\r' << endl;
	m8cmd << mdxdo << setw(10) << ycoded <<  '-'  << setw(1) << func << '\r' << endl;

	mzupcmd << mzup << setw(10) << tcoded << (tposdir ? '-' : '+') << setw(1) << func << '\r' << endl;
	mzdwcmd << mzdw << setw(10) << tcoded << (tposdir ? '+' : '-') << setw(1) << func << '\r' << endl;


	cout <<  "m1cmd " << m1cmd.str() ;
	cout <<  "m2cmd " << m2cmd.str() ;
	cout <<  "m3cmd " << m3cmd.str() ;
	cout <<  "m4cmd " << m4cmd.str() ;
	cout <<  "m5cmd " << m5cmd.str() ;
	cout <<  "m6cmd " << m6cmd.str() ;
	cout <<  "m7cmd " << m7cmd.str() ;
	cout <<  "m8cmd " << m8cmd.str() ;
 	
     for (size_t i=0; i<7; i++) motp->mot_reset(imotlist[i]);

	motp->mot_write(  mzupcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  rzup.c_str());
	motp->mot_read();
	motp->mot_write(  mzdwcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  rzdw.c_str());
	motp->mot_read();
	motp->mot_write(  m1cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m2cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m3cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m4cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m5cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m6cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m7cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  m8cmd.str().c_str());
	motp->mot_read();

	cout << hex << " id: " << dimot->fp_moduleID() << dec << endl;
        cout << hex << " all ids: " << dimot->fp_allIDs() << dec << endl;
        cout << hex << " status: " << dimot->fp_status() << dec << endl;
        cout << hex << " range(2): " << dimot->fp_range( 2 ) << dec << endl;
        cout << hex << " discrete: " << dimot->fp_discrete() << dec << endl;
        cout << hex << " status(5): " << dimot->fp_status( 5 ) << dec << endl;


	cout.flush();
	int64_t x(0);
	int64_t y(0);
	bool back(true);
	uint16_t laserch(2);
	uint16_t t_ch1(3);
	uint16_t t_ch2(5);

	if(!ldir){
	   x = lmm * cos(10.5*PI/180.0);
	   y = lmm * sin(10.5*PI/180.0);
	}


	time_t now = time(0);
        tm *ltm = localtime(&now);

        ostringstream outname;
        ostringstream date;
        date << 1900+ ltm->tm_year;
        if (1+ltm->tm_mon < 10 ) date << "0" << 1+ltm->tm_mon << ltm->tm_mday ;
        else date << 1+ltm->tm_mon << ltm->tm_mday;
        cout << date.str() << endl;
        int32_t min = ltm->tm_min + ltm->tm_hour*60 + 10000; // aggiunto minuto
        outname << path << "mapping_trap_" << date.str() << min << ".txt";


	FILE* logf = fopen (outname.str().c_str(),"w+");

	for (uint32_t ns=0; ns < nsteps; ns++)
	{

           if (*cmd != '\0')	// *** PIPE ***
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

	   next( gzdw.c_str(), dimot );
	   misura( x, y, &tast, &myoptl, ailaser, laserch, aitemp, t_ch1, t_ch2, logf );
	   next( gzup.c_str(), dimot );
	
	   cout << "step " << ns << " di " << nsteps << endl;
	   if(ns == nsteps-1) continue; 

	   if(ldir && edge) {
		cout << "gsxpc " << " " << x << " " << y <<endl;
		next(  gsxpc.c_str(), dimot );   
	        x = x + xmm;
	        y = y + ymm;
	   }
	   else if(!ldir && edge) {
		cout << "gsxdo" << " " <<  x << " " << y << endl;
		next(  gsxdo.c_str(), dimot );   
		x = x - xmm;
	        y = y - ymm;
	   }
	   else if(ldir && !edge) {
		cout << "gdxpc" << " " <<  x << " " << y <<  endl;
		next(  gdxpc.c_str(), dimot );   
	        x = x + xmm;
	        y = y - ymm;
	   }
	   else if(!ldir && !edge) {
		cout << "gdxdo" << " " << x << " " << y << endl;
		next(  gdxdo.c_str(), dimot );   
		x = x - xmm;
	        y = y + ymm;
	   }
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
