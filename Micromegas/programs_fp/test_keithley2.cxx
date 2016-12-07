#include "keithley2.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <cstdio>
#include <iomanip>
#include <time.h>

using namespace std;

int main (int argc, char* argv[])
 {
  keithley mykeith("/dev/ttyUSB7");
  std::ofstream myfile;
  std::stringstream param1; //Durata acquisizione
  time_t t = time(0);
  
	cout << t << endl;

  param1 << argv[1];
  double T_mis;
		  param1 >> T_mis;
  std::stringstream param2; //Intervallo tra misure
  param2 << argv[2];
  double dt;
  param2 >> dt;
  std::stringstream param3; //Precisione della misura
  param3 << argv[3];
  int dec;
  param3 >> dec;
  std::stringstream param4;
  param4 << argv[4]; //Frequenza di acquisizione
  string name;
  param4 >> name;
 
  std::string cmd = "*RST\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:ZCH ON\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "VOLT:GUAR ON\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "FUNC 'CURR'\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "CURR:RANG 2E-9\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:ZCOR ON\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:ZCH OFF\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:TIME:RES\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);

  	time_t now = time(0);
        tm *ltm = localtime(&now);

        ostringstream outname;
        ostringstream date;

        date << 1900+ ltm->tm_year;
	if (1+ltm->tm_mon < 10 ) date << "0" << 1+ltm->tm_mon << ltm->tm_mday ;
        else date << 1+ltm->tm_mon << ltm->tm_mday;
        cout << date.str() << endl;
        int32_t min = ltm->tm_min + ltm->tm_hour*60 + 10000; // aggiunto minuto
        outname  << "/home/atlas/Micromegas/M0Data/test_elettrici/elettrometro/Results/" << name <<"_electrometer_"<< date.str() << ".txt";


  myfile.open (outname.str(),std::ios::app);
//  FILE* logf = fopen (outname.str().c_str(),"w+");

  double elapsed;
  while (elapsed<T_mis)
    {
      double r,val;
      mykeith.readline(T_mis, dt, dec, val);
      usleep(dt*1000000);
      std::cout << val << std::endl;//" nA" << std::endl;
      myfile << val << std::endl; //<< " nA\n";
      elapsed += dt;
    }
  myfile.close();
  return 0;
 }
