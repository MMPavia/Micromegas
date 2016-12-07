
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include "arduino2.h"

int ndigital = 8;
int dchan[8] = { 20, 21, 22, 23, 4, 7, 8, 12 };
arduinoDIO dway[8] = { arduinoDIO::INPUT, arduinoDIO::INPUT,
                  arduinoDIO::INPUT, arduinoDIO::INPUT,
                  arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::OUTPUT, arduinoDIO::OUTPUT };

/*
        void digitalSetup( int nchans, int* chans, arduinoDIO* way );
        void digitalOutput( int chan, bool val );
        int analogInput( int chan );
        bool digitalInput( int chan );
        void pulseOutput( int chan );
 */

int main (int argc, char** argv)
 {
  arduino2 myboard("/dev/ttyUSB0");
  usleep(20000);

  myboard.digitalSetup( ndigital, dchan, dway );

  bool v = myboard.digitalInput(dchan[0]);
  std::cout << "Read DIGITAL channel: [" << dchan[0] << "] returned: " << v << std::endl;
  usleep(10000);
  v = myboard.digitalInput(dchan[1]);
  std::cout << "Read DIGITAL channel: [" << dchan[1] << "] returned: " << v << std::endl;
  usleep(10000);
  v = myboard.digitalInput(dchan[2]);
  std::cout << "Read DIGITAL channel: [" << dchan[2] << "] returned: " << v << std::endl;
  usleep(10000);
  v = myboard.digitalInput(dchan[3]);
  std::cout << "Read DIGITAL channel: [" << dchan[3] << "] returned: " << v << std::endl;
  usleep(1000000);

  int val = myboard.analogInput(0);
  std::cout << "Read ANALOG channel: [" << 0 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard.analogInput(2);
  std::cout << "Read ANALOG channel: [" << 2 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard.analogInput(6);
  std::cout << "Read ANALOG channel: [" << 6 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard.analogInput(9);
  std::cout << "Read ANALOG channel: [" << 9 << "] returned: " << val << std::endl;
  usleep(1000000);
  
  myboard.pulseOutput(dchan[4]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[4] << "]" << std::endl;
  usleep(10000);
  myboard.pulseOutput(dchan[5]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[5] << "]" << std::endl;
  usleep(10000);
  myboard.pulseOutput(dchan[6]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[6] << "]" << std::endl;
  usleep(10000);
  myboard.pulseOutput(dchan[7]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[7] << "]" << std::endl;
  usleep(1000000);

  myboard.digitalOutput(dchan[4], true);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] true" << std::endl;
  usleep(10000);
  myboard.digitalOutput(dchan[5], true);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] true" << std::endl;
  usleep(10000);
  myboard.digitalOutput(dchan[6], true);
  std::cout << "set DIGITAL channel: [" << dchan[6] << "] true" << std::endl;
  usleep(10000);
  myboard.digitalOutput(dchan[7], true);
  std::cout << "set DIGITAL channel: [" << dchan[7] << "] true" << std::endl;
  usleep(1000000);

  myboard.digitalOutput(dchan[4], false);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] false" << std::endl;
  usleep(10000);
  myboard.digitalOutput(dchan[5], false);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] false" << std::endl;
  usleep(10000);
  myboard.digitalOutput(dchan[6], false);
  std::cout << "set DIGITAL channel: [" << dchan[6] << "] false" << std::endl;
  usleep(10000);
  myboard.digitalOutput(dchan[7], false);
  std::cout << "set DIGITAL channel: [" << dchan[7] << "] false" << std::endl;
  usleep(10000);

  return 0;
 }
