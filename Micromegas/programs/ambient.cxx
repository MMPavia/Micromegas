#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <cerrno>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

#include "arduinoX.h"

#include "my_pipe.h"

#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M05Data/ambient/"

std::string pipe_name("/tmp/atlas/ambientpipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {'\0'};
my_pipe p(pipe_name.c_str(), cmd);

#define SLEEP 60000000    // 1 min?

using namespace std;

string GetDate(tm *ltm)
{
        ostringstream date;

        date << 1900+ ltm->tm_year ;

	if (1+ltm->tm_mon < 10 ) date << "0" <<  1+ltm->tm_mon;
	else date << 1+ltm->tm_mon;

	if (ltm->tm_mday < 10 ) date <<  "0" << ltm->tm_mday;
	else date << ltm->tm_mday; 

	//cout << "date " <<  date.str() << endl; 
        return date.str();
}

string GetTime(tm *ltm)
{
        ostringstream time;

	if (ltm->tm_hour < 10 ) time <<  "0" << ltm->tm_hour;
	else time << ltm->tm_hour; 

	time << ":";

	if (ltm->tm_min < 10 ) time <<  "0" << ltm->tm_min;
	else time << ltm->tm_min; 

        time << ":";

	if (ltm->tm_sec < 10 ) time <<  "0" << ltm->tm_sec;
	else time << ltm->tm_sec; 


//	cout << "time " <<  time.str() << endl; 
	return time.str(); 

}

int main (int argc, char** argv)
{
	//arduinoX* myboard = arduinoX::create("/dev/ttyACM0");
	arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
 	// calibrating the channels
	uint16_t mod(0);
	uint16_t chan(0);
	double RFS, RZS, EFS, EZS;
	std::string name, unit, descr;
	std::string line;

	std::fstream f("/home/atlas/Micromegas/inputfile/arduino_analog.txt", std::fstream::in);

	while (getline(f,line)){

	  stringstream s(line);
	  s >> name;
	  if (name == "#") continue;
	  s >> mod >> chan >> RFS >> RZS >> EFS >> EZS >> unit >> descr;
	  cout << "XXXXX name " << name << " mod " << mod << " chan " << chan << " RFS "
               << RFS << " RZS " << RZS << " EFS " << EFS << " EZS " << EZS
               << " unit " << unit << " descr " << descr << endl;
	  myboard->setPhysScale( chan, RFS, RZS, EFS, EZS); ;  //fp_ai_100
	 }

	// relevant channels to read 
	uint16_t temp_up_sb(0);
	uint16_t temp_dw_sb(1);
	uint16_t press_line3_cb(2);
	uint16_t press_line4_cb(3);
       	uint16_t press_amb(4);
	uint16_t temp_dw_tb(5);
	uint16_t temp_up_tb(6);
	uint16_t temp_p1(7);
	uint16_t temp_p2(8);
	uint16_t temp_p3(9);
	uint16_t temp_amb_pc(10);
	uint16_t rh_amb_pc(11);
	uint16_t press_diff(12);
	uint16_t temp_amb_door(13);
	uint16_t rh_amb_door(14);
	uint16_t laser(15);

	// getting time info for file name
	time_t now = time(0);
	tm *ltm = localtime(&now);
        string mydate = GetDate(ltm); 


        #define _MICRO_SLEEP_ 100
        #define _LONG_SLEEP_ 1800

	printf ( " sleep times are %d sec %d usec\n", _LONG_SLEEP_, _MICRO_SLEEP_ );
        bool run(true);
        if (*cmd != '\0')
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
            printf("found string %s\n", cmd);
           }
          if (strcasecmp("quit", cmd) == 0) run = false;
          if (strcasecmp("kill", cmd) == 0) exit(1);
          *cmd = '\0';
         }


         FILE* logf;

	 // output data file
         ostringstream outname;
	 outname << path << "ambient_" << mydate <<".txt";
	 cout << "Saving to File " << outname.str() << endl; 
         logf = fopen (outname.str().c_str(),"a+");
         fprintf ( logf, "  time  \t temp_tab_dw  \t temp_tab_up  \t temp_pl1  \t temp_pl2  \t temp_pl3  \t temp_amb_pc  \t temp_amb_door  \t rh_amb_pc  \t rh_amb_door  \t press_amb  \t press_diff  \n");


	while(run)
         {

           time_t now2 = time(0);
           tm *tm2 = localtime(&now2);
           string checkday = GetDate(tm2);

           if (checkday != mydate) {
                             cout << "Changing date" << endl;
                             ostringstream outname2;
                             outname2 << path << "ambient_" << checkday <<".txt";
                             cout << "Saving to File " << outname2.str() << endl; 
                             fclose(logf);
                             logf = fopen (outname2.str().c_str(),"a+");
                             fprintf ( logf, "  time  \t temp_tab_dw  \t temp_tab_up  \t temp_pl1  \t temp_pl2  \t temp_pl3  \t temp_amb_pc  \t temp_amb_door  \t rh_amb_pc  \t rh_amb_door  \t press_amb  \t press_diff  \n");
                             mydate =checkday;

           }



           time_t t0 = time(NULL);
           time_t tlim = _LONG_SLEEP_ + t0;

	   cout << t0 << " " << tlim << endl;


	   cout << " reading " << endl; 
	   time_t timestamp = time(0);
	   tm *lts = localtime(&timestamp);
	   string mytime = GetTime(lts);
           string myldate = GetDate(lts);


          // if( strcmp ( myldate.c_str() , mydate.c_str()) !=  0) return 1; 

	   double p_amb = myboard->getPhyVal(press_amb);
           double t_tab_dw = myboard->getPhyVal(temp_dw_tb);
	   double t_tab_up = myboard->getPhyVal(temp_up_tb);
	   double t_pl1 = myboard->getPhyVal(temp_p1);
	   double t_pl2 = myboard->getPhyVal(temp_p2);
	   double t_pl3 = myboard->getPhyVal(temp_p3);
	   double t_amb_pc = myboard->getPhyVal(temp_amb_pc);
	   double r_amb_pc = myboard->getPhyVal(rh_amb_pc);
	   double p_diff = myboard->getPhyVal(press_diff);
	   double t_amb_door = myboard->getPhyVal(temp_amb_door);
	   double r_amb_door = myboard->getPhyVal(rh_amb_door);

           fprintf( logf, " %s \t %f \t  %f \t %f  \t %f  \t %f  \t %f  \t %f  \t %f  \t %f  \t %f  \t %f   \n", mytime.c_str(), t_tab_dw, t_tab_up, t_pl1, t_pl2, t_pl3, t_amb_pc, t_amb_door,  r_amb_pc,  r_amb_door, p_amb, p_diff );
	   fflush( logf );


           do
            {
             if (*cmd != '\0')
              {
               printf("found string %s\n", cmd);
               if (strcasecmp("pause", cmd) == 0)
                {
                 *cmd = '\0';
                 do
                  {
                   usleep(_MICRO_SLEEP_);
                  }
                 while (!( strcasecmp("resume", cmd) == 0 ||
                           strcasecmp("quit", cmd) == 0 ||
                           strcasecmp("kill", cmd) == 0 ));
                }
               if (strcasecmp("quit", cmd) == 0) goto QuitNow;
               if (strcasecmp("kill", cmd) == 0) exit(1);
               *cmd = '\0';
              }
             usleep(_MICRO_SLEEP_);
            }

           while (time(NULL) < tlim);  
           cout << time(NULL) << " " << tlim << endl; 
	}

  QuitNow:
        fclose(logf);

	arduinoX::cleanup();

	return 0;
 }

