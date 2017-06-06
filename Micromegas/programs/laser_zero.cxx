
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <sstream>
#include "arduinoX.h"


int ndigital = 1;
//7 for relee
int dchan[3] = {51,4, 53};//7 for rele' (53 for batch, 51 for zeroing)
	// channels 4, 7, 8, 12 are connected to 4 relays
arduinoDIO dway[4] = { arduinoDIO::OUTPUT,arduinoDIO::OUTPUT, arduinoDIO::OUTPUT, arduinoDIO::OUTPUT};

/*
        void digitalSetup( int nchans, int* chans, arduinoDIO* way );
        void digitalOutput( int chan, bool val );
        int analogInput( int chan );
        bool digitalInput( int chan );
        void pulseOutput( int chan );
*/ 

int main (int argc, char** argv)
 { 

  arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
  usleep(100000);

  // define INPUT and OUTPUT digital channels
  // ndigital = 8, first 4 channels as INPUT, last 4 channels as OUTPUT 
  // 		channels defined in dchan, I/O mode in dway
  myboard->digitalSetup( ndigital, dchan, dway );
  usleep(100000);

  ////////////
  	uint16_t mod(0);
	uint16_t chan(0);
	double RFS, RZS, EFS, EZS;
	std::string name, unit, descr;
	std::string line;

  std::fstream f("/home/atlas/Micromegas/inputfile/arduino_analog.txt", std::fstream::in);
    while (getline(f,line))
    {
      std::stringstream s(line);
        s >> name;
        if (name == "#") continue;
        s >> mod >> chan >> RFS >> RZS >> EFS >> EZS >> unit >> descr;

	myboard->setPhysScale( chan, RFS, RZS, EFS, EZS); ;  //fp_ai_100
    }
    ////////////////



   uint16_t laser(15);
  //delay
   myboard->digitalOutput(dchan[1],1);
   myboard->delayedPulse(dchan[0], 500, 10000); // delay 5 ms, duration 20 ms

   myboard->delayedPulse(dchan[2], 5, 20);
     usleep(200000);//200ms

    
    double vlaser = myboard->getPhyVal(laser);
    std::cout << "laser value now " << vlaser << std::endl;
    usleep(500000);
   myboard->digitalOutput(dchan[1],0);

  arduinoX::cleanup();
  return 0;
 }


