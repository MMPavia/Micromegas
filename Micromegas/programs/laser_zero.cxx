
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include "arduinoX.h"

int ndigital = 1;
int dchan[1] = {7};//7
	// channels 4, 7, 8, 12 are connected to 4 relays
arduinoDIO dway[1] = { arduinoDIO::OUTPUT};

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

  //delay
   myboard->delayedPulse(dchan[0], 500, 10000); // delay 5 ms, duration 20 ms

  arduinoX::cleanup();
  return 0;
 }


