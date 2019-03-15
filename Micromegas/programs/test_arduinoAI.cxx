
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include "arduinoX.h"

/*
        void digitalSetup( int nchans, int* chans, arduinoDIO* way );
        void digitalOutput( int chan, bool val );
        int analogInput( int chan );
        bool digitalInput( int chan );
        void pulseOutput( int chan );
*/ 

int main (int argc, char** argv)
 {
  std::string rs232port("/dev/tty");

  if (argc == 1) return -1;
  rs232port = rs232port+argv[1];
//  arduinoX* myboard = arduinoX::create("/dev/ttyUSB3");
//  arduinoX* myboard = arduinoX::create("/dev/ttyACM1");
  arduinoX* myboard = arduinoX::create(rs232port.c_str());
  usleep(100000);

  // read 16 analog inputs
  for (int k=0; k<16; k++)
   {
    int val = myboard->analogInput(k);
    std::cout << "Read ANALOG channel: [" << k << "] returned: " << val << std::endl;
    usleep(10000);
   }

  arduinoX::cleanup();
  return 0;
 }

