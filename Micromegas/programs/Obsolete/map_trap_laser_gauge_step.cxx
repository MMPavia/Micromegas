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
#include <sys/time.h>

#include "arduinoX.h"
#include "gauge.h"
#include "optline3D.h"
#include "motors.h"

#include "my_pipe.h"

#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M05Data/mapping/"
#define PI 3.14159265

std::string pipe_name("/tmp/atlas/maptrappipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {'\0'};
my_pipe p(pipe_name.c_str(), cmd);

int ndigital = 1;
int dchan[1] = { 4 };
arduinoDIO dway[4] = { arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                       arduinoDIO::OUTPUT, arduinoDIO::OUTPUT };

using namespace std;

// parametri da utilizzare: ./map_trap -w 2160 1120 -x 80 20 -y 80 20 -t 3 10

uint16_t imotlist[] { 231, 232, 233, 234, 235, 236, 237 };
string smotlist[] { "231", "232", "233", "234", "235", "236", "237"};

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

struct timeval T1, T2, T3, T4;

double dtime( struct timeval * T1, struct timeval * T2)
 {
  return (T2->tv_sec-T1->tv_sec)*1000.0+(T2->tv_usec-T1->tv_usec)*0.001;
 }

void waitnext( struct timeval * T1, int msec )
 {
  struct timeval tnow;
  double elapsed;
  do
   {
    sched_yield();
    gettimeofday(&tnow, NULL);
    elapsed = dtime(T1,&tnow);
   }
  while ( elapsed < msec );
  return;
 }




 
/*
void misura( int64_t x, int64_t y, gauge* tast, optline* optl, arduinoX* myboard, uint16_t t_ch1, uint16_t t_ch2, FILE* logf )
{
    double t_tab = myboard->getPhyVal(t_ch1);
    double t_amb = myboard->getPhyVal(t_ch2);
    double  utast = tast->readval();
    double ol = optl->readlineZ();
    fprintf( logf, " %d \t %d \t %f \t %f \t %f \t %f  \n", x, y, ol, utast, t_tab, t_amb  );
    fflush( logf );
    printf( " %d \t %d \t %f \t %f \t %f \t %f  \n", x, y, ol, utast, t_tab, t_amb  );
    fflush( stdout );
}
*/


void misura( int64_t x, int64_t y,  optline* optl, arduinoX* myboard, uint16_t laser ,  gauge* tast, uint16_t t_ch1, uint16_t t_ch2, FILE* logf )
{
    double t_tab = myboard->getPhyVal(t_ch1);
    double t_amb = myboard->getPhyVal(t_ch2);

    gettimeofday(&T1, NULL);
    myboard->delayedPulse(dchan[0], 5, 20); // delay 5 ms, duration 20 ms
    // reading all the three channels of the optical line together 
    gettimeofday(&T2, NULL);
    std:vector<float>  ola= optl->readlineXYZ();
    usleep(200000);//200ms
    gettimeofday(&T3, NULL);
    double vlaser = myboard->getPhyVal(laser);
    gettimeofday(&T4, NULL);
    double  utast = tast->readval();


    fprintf( logf, "  %f \t %f \t %f \t %f \t %f \t %f \t %f \n", ola.at(0), ola.at(1), ola.at(2), vlaser, utast, t_tab, t_amb  );
    fflush( logf );
    // in the print on screen temporarely left x-y info from steps
    // printf( "%f \t %f \t %f \t", dtime(&T1,&T2), dtime(&T2,&T3), dtime(&T3,&T4));
    printf( "  %f \t %f \t %f \t %f \t %f \t %f \t %f \n", ola.at(0), ola.at(1),  ola.at(2), vlaser, utast, t_tab, t_amb  );
    fflush( stdout );
}

// *** MAIN ***

int main (int argc, char** argv)
{
	uint8_t mask(0);
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2), tmot(6);
	uint64_t xmm(0), ymm(0), tum(0);
	uint64_t xsteps(0), ysteps(0), tsteps(0);
	uint64_t xspeed(5), yspeed(5), tspeed(60);
	bool xposdir(true), yposdir(true), tposdir(true);
	uint64_t xcoded(0), ycoded(0), ycoded2(0), tcoded(0);
	int32_t xdim(0), ydim(0);
	int32_t ntotx(0), ntoty(0);

	//choose which USB port is connected to which device

        motp = new motors("/dev/ttyUSB1");

        arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
	gauge tast("/dev/ttyUSB2");
	optline myoptl("/dev/ttyUSB3");




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

	xcoded = xmot*100000*100*100;
	tcoded = tmot*100000*100*100;
	ycoded = ymot*100000*100*100;
	ycoded2 = ymot*100000*100*100;

	if (argc < 4)  return __LINE__;
	-- argc; ++ argv;

	do				// *** INIZIALIZZO BASE E ALTEZZA RETTANGOLO ***
         {
	  if ((strcasecmp(*argv,"w")==0) || (strcasecmp(*argv,"-w")==0)) {
	    if (argc < 2) return __LINE__;

	    -- argc; ++ argv;
	    xdim = strtol(*argv, NULL, 10);

	    -- argc; ++ argv;
	    ydim = strtol(*argv, NULL, 10);

	  } else if ((strcasecmp(*argv,"x")==0) || (strcasecmp(*argv,"-x")==0)) {	// *** INIZIALIZZO X ****************
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

	  } else if ((strcasecmp(*argv,"y")==0) || (strcasecmp(*argv,"-y")==0)) {	// *** INIZIALIZZO Y ****************
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
	    ycoded2 = ((ymot * 100000 + ysteps/2)*100 + yspeed)*100 + ramp;
	    mask |= 2;

	  } 
	  
  else if ((strcasecmp(*argv,"t")==0) || (strcasecmp(*argv,"-t")==0)) {	// *** INIZIALIZZO T ****************
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

	  }

 
	  else  return __LINE__;
	  ++ argv;
	 }
	while (-- argc);

	cout << " dx(mm) dy(mm) ds(um) " << xmm << " " << ymm << " " << tum
	     << endl;
	if (xmm) ntotx = (xdim / xmm) + 1;
	if (ymm) ntoty = (ydim / ymm) + 1;
	cout << " xdim ydim ntotx ntoty " << xdim << " " << ydim << " "
             << ntotx << " " << ntoty << " " << endl;

        // programma 231 per movimento 1 in y
	string ry1 = "r" + smotlist[0] + "\r\n";    // stringa "r231\r\n"
	string gy1 = "g" + smotlist[0] + "\r\n";    // stringa "g231\r\n"
	string my1 = "m" + smotlist[0] + "-";       // stringa "m231-"
        // programma 232 per movimento 2 in y
	string ry2 = "r" + smotlist[1] + "\r\n";    // stringa "r232\r\n"
	string gy2 = "g" + smotlist[1] + "\r\n";    // stringa "g232\r\n"
	string my2 = "m" + smotlist[1] + "-";       // stringa "m232-"
        // programma 233 per movimento in x
	string rx = "r" + smotlist[2] + "\r\n";     // stringa "r233\r\n"
	string gx = "g" + smotlist[2] + "\r\n";     // stringa "g233\r\n"
	string mx = "m" + smotlist[2] + "-";        // stringa "m233-"
        // programma 234 per movimento up in z
	string rzup = "r" + smotlist[3] + "\r\n";   // stringa "r234\r\n"
	string gzup = "g" + smotlist[3] + "\r\n";   // stringa "g234\r\n"
	string mzup = "m" + smotlist[3] + "-";      // stringa "m234-"
        // programma 235 per movimento down in z
	string rzdw = "r" + smotlist[4] + "\r\n";   // stringa "r235\r\n"
	string gzdw = "g" + smotlist[4] + "\r\n";   // stringa "g235\r\n"
	string mzdw = "m" + smotlist[4] + "-";      // stringa "m235-"
        // programma 236 per movimento 3 in y
	string ry3 = "r" + smotlist[5] + "\r\n";    // stringa "r236\r\n"
	string gy3 = "g" + smotlist[5] + "\r\n";    // stringa "g236\r\n"
	string my3 = "m" + smotlist[5] + "-";       // stringa "m236-"
        // programma 237 per movimento 4 in y
	string ry4 = "r" + smotlist[6] + "\r\n";    // stringa "r237\r\n"
	string gy4 = "g" + smotlist[6] + "\r\n";    // stringa "g237\r\n"
	string my4 = "m" + smotlist[6] + "-";       // stringa "m237-"

	stringstream my1cmd, my2cmd, my3cmd, my4cmd, mxcmd;
	stringstream mzupcmd, mzdwcmd;

	my1cmd << my1 << setw(10) << ycoded << (yposdir ? '+' : '-')
	      << setw(1) << func << '\r' << endl;
	my2cmd << my2 << setw(10) << ycoded << (yposdir ? '-' : '+')
	      << setw(1) << func << '\r' << endl;
	my3cmd << my3 << setw(10) << ycoded2 << (yposdir ? '+' : '-')
	      << setw(1) << func << '\r' << endl;
	my4cmd << my4 << setw(10) << ycoded2 << (yposdir ? '-' : '+')
	      << setw(1) << func << '\r' << endl;
	mxcmd << mx << setw(10) << xcoded << (xposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
	mzupcmd << mzup << setw(10) << tcoded << (tposdir ? '-' : '+')
	       << setw(1) << func << '\r' << endl;
	mzdwcmd << mzdw << setw(10) << tcoded << (tposdir ? '+' : '-')
	       << setw(1) << func << '\r' << endl;
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << xmot
	     << "\n passi [-s] " << xsteps
	     << "\n velocità [-v] " << xspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << xposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa " << mxcmd.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << ymot
	     << "\n passi [-s] " << ysteps
	     << "\n velocità [-v] " << yspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << yposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << my1cmd.str()
	     << "\n stringa2 " << my2cmd.str()
	     << "\n stringa3 " << my3cmd.str()
	     << "\n stringa4 " << my4cmd.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << tmot
	     << "\n passi [-s] " << tsteps
	     << "\n velocità [-v] " << tspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n direzione [-d] " << tposdir
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mzupcmd.str()
	     << "\n stringa2 " << mzdwcmd.str();
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

    
        cout << "before resetting all "<< endl;


	for (size_t i=0; i<8; i++) motp->mot_reset(imotlist[i]);

	cout << "reset done " << endl;

	motp->mot_write(  mxcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  rx.c_str());
	motp->mot_read();
	motp->mot_write(  mzupcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  rzup.c_str());
	motp->mot_read();
	motp->mot_write(  mzdwcmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  rzdw.c_str());
	motp->mot_read();
	motp->mot_write(  my1cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  ry1.c_str());
	motp->mot_read();
	motp->mot_write(  my2cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  ry2.c_str());
	motp->mot_read();
	motp->mot_write(  my3cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  ry3.c_str());
	motp->mot_read();
	motp->mot_write(  my4cmd.str().c_str());
	motp->mot_read();
	motp->mot_write(  ry4.c_str());
	motp->mot_read();

        cout << " writing to motors done " << endl;

	cout.flush();
	int64_t x(0);
	int64_t y(0);
	bool back(true);
	uint16_t laserch(2);
	uint16_t t_ch1(3);
	uint16_t t_ch2(5);
	uint16_t laser(15);



	time_t now = time(0);
        tm *ltm = localtime(&now);

        ostringstream outname;
        ostringstream filename;//Athina
        ostringstream date;
        date << 1900+ ltm->tm_year;
        if (1+ltm->tm_mon < 10 ) date << "0" << 1+ltm->tm_mon << ltm->tm_mday ;
        else date << 1+ltm->tm_mon << ltm->tm_mday;
        cout << date.str() << endl;
        int32_t min = ltm->tm_min + ltm->tm_hour*60 + 10000; // aggiunto minuto
        outname << path << "mapping_trap_" << date.str() << min << ".txt";//save raw file
	filename << path << "map_trap_filename.txt";//Athina

	int64_t Nstepin = 0;
	int64_t Nstepnext = 0;
	double deltay = ymm*tan(10.5*PI/180.0);
	double lin = ydim;  
	double lnext = 0;
	int ystepout = 0;	// questa variabile serve solo per scrive nel file di output

	cout << "starting " << ydim << " " << lin << " " << lnext << " " << ntotx << " " << ntoty << endl;    

	FILE* logf = fopen (outname.str().c_str(),"w+");
	FILE* saveFileName = fopen (filename.str().c_str(),"w+");//Athina
	fprintf( saveFileName, " %s", outname.str().c_str() );//Athina
	fflush( saveFileName );//Athina
	fclose(saveFileName );//Athina

	for (uint32_t nx=0; nx < ntotx; nx++)
	{
		//number of steps calculation
        	lnext = lin - 2*deltay;
		Nstepin = trunc( lin / ymm)+1;
		Nstepnext = trunc(lnext / ymm)+1; // lnext:reduction in length from 10.5 degrees for each line

		cout << nx << " " << lin << " " << lnext << " " << Nstepin  << " " << Nstepnext << endl; 

                for (uint32_t ny=0; ny < Nstepin; ny++)	{

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

		//actual measurement: move down on the z axis, measure, move up again
		next( gzdw.c_str(), myboard );
		misura( x, y, &myoptl, myboard, laser, &tast, t_ch1, t_ch2, logf );
		next( gzup.c_str(), myboard );

		//depending on where you are (number of steps defined above) you move to the right, left, forwards
		if(ny!=Nstepin-1){ 
		        next( back ? gy2.c_str() : gy1.c_str(), myboard );  /// giusto
		 	ystepout = ymm;
		        y = y + (back ? ystepout : -ystepout);
	           }
        	cout << ny << " done " << endl;         
		}


	        if(nx!=ntotx-1) next(  gx.c_str(), myboard);   

		if ( Nstepnext < Nstepin ) { 


               		next( back ? gy3.c_str() : gy4.c_str(), myboard );	 
		 	ystepout = ymm/2;
		        y = y + (back ? -ystepout : ystepout);
	  	} 

	       back = not back;

	       x = x + xmm;
	       lin = lnext;	
	 }

QuitNow:
	fclose(logf);



	delete motp;

    arduinoX::cleanup();


	return 0;
 }
