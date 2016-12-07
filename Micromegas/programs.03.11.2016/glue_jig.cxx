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
//	cout << "mstat: " << mstat << endl;
        usleep(10000);
    }
    while (mstat > 0);
    usleep(2000000);
}


int main (int argc, char** argv)
{
	uint64_t ramp(31), func(0);
	uint64_t xmot(1), ymot(2), smot(4);
	uint64_t xmm(33),  sum(0);
	uint64_t xsteps(0),ysteps(0),ssteps(0);
	uint64_t xspeed(60), yspeed(20), sspeed(17);
	uint64_t xcoded(0);
	bool ispos(true);
	const char* progmv;
	const char* progrd;

	//	, ysteps1(0), ysteps2(0), dys(0),
	//        ssteps1(0), ssteps2(0), dss(0);
//	bool xposdir(true), yposdir(true), sposdir(true);
//	uint64_t xcoded(0), ycoded1(0), ycoded2(0), scoded1(0), scoded2(0);

	int cart(0); 
        uint32_t type(0);
	std::vector<int> nloops; 
 	std::vector<float> ymm; 
	std::vector<uint64_t> ycoded; 
	std::vector<uint64_t> scoded; 
	std::vector<string> smotlist;
        std::vector<string> rprog1;
        std::vector<string> gprog1;
        std::vector<string> mprog1;
        std::vector<string> rprog2;
        std::vector<string> gprog2;
        std::vector<string> mprog2;
 


	//gg da scommentare
        arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
	motp = new motors("/dev/ttyUSB1");

        float length(0);

	// step in x is always the same
	xsteps = xmm*25;
	xcoded = ((xmot * 100000 + xsteps)*100 + xspeed)*100 + ramp;
	
	stringstream mxcmd1, mscmd1, mxcmd2, mscmd2, mycmd1, mycmd2;

	if(argc > 0) cart=atoi(argv[1]); 
	else return 0;
 
        cout << "cart " << cart << endl;
        if (cart==1){
	    type =5;  
	    nloops = {12,1,12,1,8}; 
	    //ymm = {923, 842.5, 762, 682, 600.6};
	    ymm = {880, 802.5, 722, 642, 560.6};
	    smotlist= {"400", "401", "402", "403", "404", "405","406","407", "408","409"}; 
	  } else  if (cart==2){
	    type =7;  
            nloops =  {4,1,13,1,2,1,5};
	    ymm = {560.6, 480, 400, 814.5, 1230, 734, 238};
	    smotlist= {"400", "401", "402", "403", "404","405","406","407","408","409","410", "411","412","413"}; 
	   // ispos = not ispos;
	}


	//sstep1 = (mm * 1000 +0.5)*16/3 = mm * 1000 * 16/3 + 0.5*16/3 = mm* 16000/3 + 2.7;
	//sstep1 = (ymm/1000)*3.3 * 8000/3 +2.7 = ymm *8*1.1 +2.7 = ymm*17.6 + 2.7 ;
	
	for(int t=0; t<type; t++){
	   cout << t << " " << nloops[t] << " " << ymm[t] << endl;
	   ssteps=ymm[t]*17.6 + 2.7; 
	   ysteps=ymm[t]*25; 
	   scoded.push_back(((smot * 100000 + ssteps)*100 + sspeed)*100 + ramp);
	   ycoded.push_back(((ymot * 100000 + ysteps)*100 + yspeed)*100 + ramp);
	   cout << ssteps << " " << scoded[t] << endl; 
	   cout << ysteps << " " << ycoded[t] << endl; 

	   rprog1.push_back("r"+smotlist[t]+"\r\n");           
	   gprog1.push_back( "g"+smotlist[t]+"\r\n");           
	   //mprog1.push_back( "m"+smotlist[t]+"-");
	   rprog2.push_back( "r"+smotlist[type+t]+"\r\n");           
	   gprog2.push_back( "g"+smotlist[type+t]+"\r\n");           
	   //mprog2.push_back( "m"+smotlist[type+t]+"-");
	}

	for(int t=0; t<type; t++){
	   mxcmd1 << "m" << smotlist[t] << "-" << setw(10) << xcoded << "+" << setw(1) << func << '\r' << endl;        
	   mscmd1 << "m" << smotlist[t] << "-" << setw(10) << scoded[t] << "-" << setw(1) << func << '\r' << endl;
	   mycmd1 << "m" << smotlist[t] << "-" << setw(10) << ycoded[t] << "-" << setw(1) << func << '\r' << endl;
	   mxcmd2 << "m" << smotlist[t+type] << "-"  << setw(10) << xcoded << "+"<< setw(1) << func << '\r' << endl;        
	   mscmd2 << "m" << smotlist[t+type] << "-"  << setw(10) << scoded[t] << "-" << setw(1) << func << '\r' << endl;
	   mycmd2 << "m" << smotlist[t+type] << "-"  << setw(10) << ycoded[t] << "+" << setw(1) << func << '\r' << endl;
	   cout << mxcmd1.str().c_str() << endl;
	   cout << mscmd1.str().c_str() << endl;
	   cout << mycmd1.str().c_str() << endl;
	   cout << mxcmd2.str().c_str() << endl;
	   cout << mscmd2.str().c_str() << endl;
	   cout << mycmd2.str().c_str() << endl;

	   cout << "writing to mem" << endl;
	   motp->mot_write(mxcmd1.str().c_str());
	   motp->mot_read();
	   motp->mot_write(mycmd1.str().c_str());
	   motp->mot_read();
	   motp->mot_write(mscmd1.str().c_str());
	   motp->mot_read();
	   motp->mot_write(rprog1[t]);
	   motp->mot_read();
	   motp->mot_write(mxcmd2.str().c_str());
	   motp->mot_read();
	   motp->mot_write(mycmd2.str().c_str());
	   motp->mot_read();
	   motp->mot_write(mscmd2.str().c_str());
	   motp->mot_read();
	   motp->mot_write(rprog2[t]);
	   motp->mot_read();
	   cout.flush();
	   cout << "finished write to mem" << endl;

	   mxcmd1.str(std::string());
	   mxcmd2.str(std::string());
	   mscmd1.str(std::string());
	   mscmd2.str(std::string());
	   mycmd1.str(std::string());
	   mycmd2.str(std::string());

	}

 	for(int t=0; t<type; t++){
	   cout << t << " " << nloops[t] << " " << ymm[t] << endl;
	   for (int i =0; i<nloops[t]; i++ ){

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

		progmv = (ispos) ? gprog1[t].c_str() : gprog2[t].c_str();
	//	progrd = (ispos) ? rprog1[t].c_str() : rprog2[t].c_str();
		cout << i << " "  << progmv << endl;  //"  << progrd << endl;
	//	motp->mot_write(progrd);
        //   	motp->mot_read();
	//      cout.flush();
        	next(progmv, myboard);
		cout << "program done"<< endl;
		ispos = not ispos;
	   }
	} 


QuitNow:

  delete motp;

  return 0;
 }

