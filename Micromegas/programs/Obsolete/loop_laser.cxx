//mapping using the blu laser
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include "arduinoX.h"
#include <sys/time.h>

#define path "/home/atlas/Micromegas/M05Data/laser_test/"

int ndigital = 1;
// int dchan[4] = { 4, 7, 8, 12 };
	// channels 4, 7, 8, 12 are connected to 4 relays

int dchan[1] = { 4 };
arduinoDIO dway[4] = { arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::OUTPUT, arduinoDIO::OUTPUT };

using namespace std;


int main (int argc, char** argv)
 {
  arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
  usleep(1000000);

  // calibrating the channels
  uint16_t mod(0);
  uint16_t chan(0);
  double RFS, RZS, EFS, EZS;
  std::string name, unit, descr;
  std::string line;

  std::fstream f("/home/atlas/Micromegas/inputfile/arduino_analog.txt", std::fstream::in);

  while (getline(f,line)){

    std::stringstream s(line);
    s >> name;
    if (name == "#") continue; 
    s >> mod >> chan >> RFS >> RZS >> EFS >> EZS >> unit >> descr; 
    std::cout << "XXXXX name " << name << " mod " << mod << " chan " << chan << " RFS "
         << RFS << " RZS " << RZS << " EFS " << EFS << " EZS " << EZS
         << " unit " << unit << " descr " << descr << std::endl;
    myboard->setPhysScale( chan, RFS, RZS, EFS, EZS); ;  //fp_ai_100
  }

  myboard->digitalSetup( ndigital, dchan, dway );
  usleep(1000000);

  time_t now = time(0);
  tm *ltm = localtime(&now);


  ostringstream outname;
  ostringstream date;
  date << 1900+ ltm->tm_year;
  if (1+ltm->tm_mon < 10 ) date << "0" << 1+ltm->tm_mon << ltm->tm_mday ;
  else date << 1+ltm->tm_mon << ltm->tm_mday;
  cout << date.str() << endl;
  int32_t min = ltm->tm_min + ltm->tm_hour*60 + 10000; // aggiunto minuto
  outname << path << "laser_read_" << date.str() << min << ".txt";//save raw file




  FILE* logf = fopen (outname.str().c_str(),"w+");
 
  // relevant channels to read 
  uint16_t laser(15);
  int j=0;
   
  int rr = 50;  
 
  do
   {
    //remove the following lines to remove the external trigger
    std::cout << "triggering " << std::endl;
    myboard->digitalOutput(dchan[0], true);
    std::cout << "set DIGITAL channel: [" << dchan[0] << "] true" << std::endl;
    usleep(10000);
    // reset digital output
    myboard->digitalOutput(dchan[0], false);
    std::cout << "set DIGITAL channel: [" << dchan[0] << "] false" << std::endl;
    usleep(10000);
    
    double vlaser = myboard->getPhyVal(laser);
    std::cout << j  <<  " laser value (mm) " << vlaser << std::endl;
    usleep(500000);
    fprintf( logf, "  %f  \n",vlaser);
    fflush( logf );


    j++; 
   }
  while(j<rr);
 
   fclose(logf);


  arduinoX::cleanup();
  return 0;
 }
