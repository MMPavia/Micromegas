#define _USLEEP_ 10000

#include <unistd.h>
#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <math.h>
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

int ndigital = 6;
int dchan[6] = { 4, 38, 39, 40, 41, 42 };
arduinoDIO dway[6] = { arduinoDIO::OUTPUT, arduinoDIO::INPUT,
                       arduinoDIO::INPUT, arduinoDIO::INPUT,
                       arduinoDIO::INPUT, arduinoDIO::INPUT };

using namespace std;

// parametri da utilizzare: ./map_trap -w xdim ydim  -s xstep  vx vy


motors* motp(NULL);

void next ( const char* gx, arduinoX* myboard )
{
    motp->mot_write(gx);
    motp->mot_read();
    uint16_t mstati(0);
    uint16_t mstat(0);
    cout << "starting next command " << gx << endl;
   
    mstat = 0;
    for (int i=1; i<6; i++){
        mstati = myboard->digitalInput(dchan[i]);
        mstat += mstati;
    }
    cout << "in next():   mstat " << mstat << endl;
    usleep(10000);
    //while (mstat > 0);
    //usleep(2000000);
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

void misura( int64_t x, int64_t y,  optline* optl, arduinoX* myboard, uint16_t laser , uint16_t t_ch1, uint16_t t_ch2, FILE* logf )
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

    fprintf( logf, "  %f \t %f \t %f \t %f \t %f \t %f \n", ola.at(0), ola.at(1), ola.at(2), vlaser, t_tab, t_amb  );
    fflush( logf );
    // in the print on screen temporarely left x-y info from steps
    // printf( "%f \t %f \t %f \t", dtime(&T1,&T2), dtime(&T2,&T3), dtime(&T3,&T4));
    printf( "  %f \t %f \t %f \t %f \t %f \t %f \n", ola.at(0), ola.at(1),  ola.at(2), vlaser, t_tab, t_amb  );
    fflush( stdout );
}



// *** MAIN ***

int main (int argc, char** argv)
{
	uint8_t mask(0);
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2);
	uint64_t xmm(0), ymm(0);
	uint64_t xsteps(0), ysteps1(0),ysteps2(0), dys(0) ;
	uint64_t xspeed(5), yspeed(5);
	uint64_t xcoded(0), ycoded1(0), ycoded2(0);
	int32_t xdim(0), ydim(0);
	int32_t ntotx(0), ntoty(0);

	//choose which USB port is connected to which device

        motp = new motors("/dev/ttyUSB1");

        arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
        usleep(100000);
        myboard->digitalSetup( ndigital, dchan, dway );
        usleep(100000);

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
	ycoded1 = ymot*100000*100*100;
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
	    ysteps1 = ydim*25;

	  } else if ((strcasecmp(*argv,"s")==0) || (strcasecmp(*argv,"-s")==0)) {	// *** INIZIALIZZO X ****************
	    if (argc < 3)  return __LINE__;

	    -- argc; ++ argv;
	    int64_t mm = strtol(*argv, NULL, 10);
	    xmm = uint64_t(mm);
	    xsteps = xmm*25;

	    -- argc; ++ argv;
	    xspeed = strtol(*argv, NULL, 10);

	    xcoded = ((xmot * 100000 + xsteps)*100 + xspeed)*100 + ramp;
	    mask |= 1;
	    


	    -- argc; ++ argv;
	    yspeed = strtol(*argv, NULL, 10);
	    
	    ycoded1 = ((ymot * 100000 + ysteps1)*100 + yspeed)*100 + ramp;
	    mask |= 2;



	  }

	  else  return __LINE__;
	  ++ argv;
	 }
	while (-- argc);

	double ddy = 2*double(xmm)*tan(10.5*atan(1)/45);
	dys = uint64_t(ddy*25+0.5);
	if (ysteps1 > dys) ysteps2 = ysteps1 - dys;
	ycoded2 = ((ymot * 100000 + ysteps2)*100 + yspeed)*100 + ramp;

	cout << " dx(mm) dy(mm) ds(um) " << xmm << " " << ydim << " " << ddy   << endl;
	if (xmm) ntotx = (xdim / xmm) + 1;


	string rprog1 = "r200\r\n";
	string gprog1 = "g200\r\n";
	string mprog1_h = "m200-";
	string rprog2 = "r201\r\n";
	string gprog2 = "g201\r\n";
	string mprog2_h = "m201-";

	stringstream mxcmd1, mycmd1, mscmd1;
	stringstream mxcmd2, mycmd2, mscmd2;

	mxcmd1 << mprog1_h << setw(10) << xcoded << "+"  << setw(1) << func << '\r' << endl;
	mycmd1 << mprog1_h << setw(10) << ycoded1 << "-" << setw(1) << func << '\r' << endl;
	mxcmd2 << mprog2_h << setw(10) << xcoded << "+"  << setw(1) << func << '\r' << endl;
	mycmd2 << mprog2_h << setw(10) << ycoded2 << "+" << setw(1) << func << '\r' << endl;
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << xmot
	     << "\n passi [-s] " << xsteps
	     << "\n velocità [-v] " << xspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mxcmd1.str()
	     << " stringa2 " << mxcmd2.str();
 if (1) cout << "\n-----------------------------------------------------"
	     << "\n motore [-m] " << ymot
	     << "\n passi1 [-s] " << ysteps1
	     << "\n passi2 [-s] " << ysteps2
	     << "\n velocità [-v] " << yspeed
	     << "\n accelerazione [-r] " << ramp
	     << "\n funzione [-f] " << func
	     << "\n stringa1 " << mycmd1.str()
	     << " stringa2 " << mycmd2.str();



	if ((xdim == 0) || (xmm == 0) || (xdim % xmm))
	 {
		cout << " ***  MOVIMENTI ASSE X NON VALIDI (Range = " << xdim << ", Passo = " << xmm << ") *** " << endl;
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
	  if ((ysteps1 > 99999) || (ysteps1 == 0))
	   {
		cout << " *** SPOSTAMENTO ASSE Y NON VALIDO (= " << ysteps1 << ") *** " << endl;
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
	motp->mot_write(rprog1.c_str());
	motp->mot_read();
	motp->mot_write(mxcmd2.str().c_str());
	motp->mot_read();
	motp->mot_write(mycmd2.str().c_str());
	motp->mot_read();
	motp->mot_write(rprog2.c_str());
	motp->mot_read();
	cout.flush();

	string prog1 = "g200\r\n";
	string prog2 = "g201\r\n";
	string ycmd1str = mycmd1.str();
	string ycmd2str = mycmd2.str();
	bool ispos(false);
	const char* progmv = gprog1.c_str();
	const char* ynxtprog = ycmd2str.c_str();
	uint64_t ynxtsteps = ysteps2;
	cout << " ycmd1 " << ycmd1str;
	cout << " ycmd2 " << ycmd2str;


	int64_t x(0);
	int64_t y(0);
	uint16_t t_ch1(7);
	uint16_t t_ch2(10);
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



	// //int64_t Nstepin = 0;
	// //int64_t Nstepnext = 0;
	// double deltay = ymm*tan(10.5*PI/180.0);
	// double lin = ydim;  
	// double lnext = 0;
	// int ystepout = 0;	// questa variabile serve solo per scrive nel file di output

	// cout << "starting " << ydim << " " << lin << " " << lnext << " " << ntotx << " " << ntoty << endl;    

	FILE* logf = fopen (outname.str().c_str(),"w+");
	FILE* saveFileName = fopen (filename.str().c_str(),"w+");//Athina
	fprintf( saveFileName, " %s \n", outname.str().c_str() );//Athina
	fflush( saveFileName );//Athina
	fclose(saveFileName );//Athina

        struct timeval t0;
        int time_period_ms(1260);
	int nloops = xdim/xmm;
    	uint16_t mdigitali(0);
    	uint16_t mdigital(0);

	while (nloops --)
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

		//cout << nloops << " progmv " << progmv;
		motp->mot_write(ynxtprog);
		motp->mot_read();
		//cout << nloops << " ynxtprog " << ynxtprog;
		cout.flush();
		ispos = not ispos;
		cout << " ispos " << ispos << endl;
		progmv = (ispos) ? gprog1.c_str() : gprog2.c_str();
		ynxtprog = (ispos) ? ycmd2str.c_str() : ycmd1str.c_str();
		cout << "dys " << dys << " ynxtsteps " << ynxtsteps << endl;
		if (ynxtsteps > dys){
		  char stemp[8];
		  ynxtsteps -= dys; 
		  sprintf(stemp, "%u", ymot*100000+ynxtsteps);
		  memcpy((void*)(ynxtprog+5), stemp, 6);
		  cout << ynxtprog << endl;
		}
		else{
		  if (nloops > 1) nloops = 1;
		}
		
		float yRead, yRead0, yMax;
		yRead0 = myoptl.readlineY();
		cout << "yRead prima  next() " <<  yRead0 <<  endl;
		next(progmv, myboard);
		
		
		yMax = (ispos) ? yRead0 - ynxtsteps/25 : yRead0 + ynxtsteps/25;
		cout << yRead0 << " " << ynxtsteps << " passi  " << ynxtsteps/25 << " mm " << yMax << endl;  
		do {
		  gettimeofday(&t0,NULL);
		  //continue reading the laser and opt line
		  misura( x, y, &myoptl, myboard, laser, t_ch1, t_ch2, logf );
		  waitnext(&t0, time_period_ms);
		  yRead = myoptl.readlineY();
		  cout << "yRead dopo misura() " <<  yRead <<  endl;
		  // usleep(230000);//take measurements every 0.5s (* ROBERTO: 0.27 s are inside 'misura' *)		}
		  for (int chan=38; chan<43; chan++){
        		mdigitali = myboard->digitalInput(chan);
        		mdigital += mdigitali;
    		  }	
		  cout << " mdigital " << mdigital<< endl;
		  mdigital =0;
		  mdigitali =0;
		}
		//as long as it is smaller than the max distance
		while ((ispos) ? yRead >  yMax : yRead < yMax);
		usleep(500000);
	}

 QuitNow:
	fclose(logf);
	delete motp;
	arduinoX::cleanup();
	return 0;
 }
