
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <sstream>
#include "arduinoX.h"


int ndigital = 3;
//7 for relee
int dchan[3] = {51, 4, 53};// (53 for batch, 51 for zeroing)
	// channels 4, 7, 8, 12 are connected to 4 relays
arduinoDIO dway[4] = { arduinoDIO::OUTPUT,arduinoDIO::OUTPUT, arduinoDIO::OUTPUT, arduinoDIO::OUTPUT};

int main (int argc, char** argv)
 { 

  arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
  usleep(1000000);

  // define INPUT and OUTPUT digital channels
  // ndigital = 8, first 4 channels as INPUT, last 4 channels as OUTPUT 
  // 		channels defined in dchan, I/O mode in dway
  myboard->digitalSetup( ndigital, dchan, dway );
  usleep(100000);

  ////////////
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
   
    myboard->setPhysScale( chan, RFS, RZS, EFS, EZS); ;  //fp_ai_100

  }
    ///////////////

   uint16_t laser(15);
   myboard->digitalOutput(dchan[1],1);
   usleep(5000000);//200ms

   myboard->delayedPulse(dchan[2], 5, 20);
   usleep(500000);//200ms
   double vlaser = myboard->getPhyVal(laser);
   std::cout<<"laser before: "<<vlaser<<std::endl;

   myboard->delayedPulse(dchan[0], 500, 10000);

   usleep(200000);//200ms

   myboard->delayedPulse(dchan[2], 5, 20);
   usleep(500000);//200ms
   vlaser = myboard->getPhyVal(laser);
   std::cout<<"laser now: "<<vlaser<<std::endl;


   myboard->digitalOutput(dchan[1],0);


   arduinoX::cleanup();
   return 0;
 }


