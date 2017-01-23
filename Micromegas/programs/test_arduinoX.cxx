
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include "arduinoX.h"

int ndigital = 8;
int dchan[8] = { 12, 7, 8, 4, 20, 21, 22, 23 };
	// channels 4, 7, 8, 12 are connected to 4 relays
arduinoDIO dway[8] = { arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::INPUT, arduinoDIO::INPUT,
                  arduinoDIO::INPUT, arduinoDIO::INPUT };

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

  // read the 4 digital inputs
  bool v = myboard->digitalInput(dchan[0]);
  std::cout << "Read DIGITAL channel: [" << dchan[0] << "] returned: " << v << std::endl;
  usleep(10000);
  v = myboard->digitalInput(dchan[1]);
  std::cout << "Read DIGITAL channel: [" << dchan[1] << "] returned: " << v << std::endl;
  usleep(10000);
  v = myboard->digitalInput(dchan[2]);
  std::cout << "Read DIGITAL channel: [" << dchan[2] << "] returned: " << v << std::endl;
  usleep(10000);
  v = myboard->digitalInput(dchan[3]);
  std::cout << "Read DIGITAL channel: [" << dchan[3] << "] returned: " << v << std::endl;
  usleep(1000000);

  // read 16 analog inputs
  int val = myboard->analogInput(0);
  std::cout << "Read ANALOG channel: [" << 0 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(1);
  std::cout << "Read ANALOG channel: [" << 1 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(2);
  std::cout << "Read ANALOG channel: [" << 2 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(3);
  std::cout << "Read ANALOG channel: [" << 3 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(4);
  std::cout << "Read ANALOG channel: [" << 4 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(5);
  std::cout << "Read ANALOG channel: [" << 5 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(6);
  std::cout << "Read ANALOG channel: [" << 6 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(7);
  std::cout << "Read ANALOG channel: [" << 7 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(8);
  std::cout << "Read ANALOG channel: [" << 8 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(9);
  std::cout << "Read ANALOG channel: [" << 9 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(10);
  std::cout << "Read ANALOG channel: [" << 10 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(11);
  std::cout << "Read ANALOG channel: [" << 11 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(12);
  std::cout << "Read ANALOG channel: [" << 12 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(13);
  std::cout << "Read ANALOG channel: [" << 13 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(14);
  std::cout << "Read ANALOG channel: [" << 14 << "] returned: " << val << std::endl;
  usleep(10000);
  val = myboard->analogInput(15);
  std::cout << "Read ANALOG channel: [" << 15 << "] returned: " << val << std::endl;
  usleep(1000000);
  
  // pulse the 4 digital outputs
  myboard->pulseOutput(dchan[4]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[4] << "]" << std::endl;
  usleep(10000);
  myboard->pulseOutput(dchan[5]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[5] << "]" << std::endl;
  usleep(10000);
  myboard->pulseOutput(dchan[6]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[6] << "]" << std::endl;
  usleep(10000);
  myboard->pulseOutput(dchan[7]);
  std::cout << "Pulsed DIGITAL channel: [" << dchan[7] << "]" << std::endl;
  usleep(1000000);

  // set digital output
  myboard->digitalOutput(dchan[4], true);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] true" << std::endl;
  usleep(1000000);
  // reset digital output
  myboard->digitalOutput(dchan[4], false);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] false" << std::endl;
  usleep(1000000);

  // set digital output
  myboard->digitalOutput(dchan[5], true);
  std::cout << "set DIGITAL channel: [" << dchan[5] << "] true" << std::endl;
  usleep(1000000);
  // reset digital output
  myboard->digitalOutput(dchan[5], false);
  std::cout << "set DIGITAL channel: [" << dchan[5] << "] false" << std::endl;
  usleep(1000000);

  // set digital output
  myboard->digitalOutput(dchan[6], true);
  std::cout << "set DIGITAL channel: [" << dchan[6] << "] true" << std::endl;
  usleep(1000000);
  // reset digital output
  myboard->digitalOutput(dchan[6], false);
  std::cout << "set DIGITAL channel: [" << dchan[6] << "] false" << std::endl;
  usleep(1000000);

  // set digital output
  myboard->digitalOutput(dchan[7], true);
  std::cout << "set DIGITAL channel: [" << dchan[7] << "] true" << std::endl;
  usleep(1000000);
  // reset digital output
  myboard->digitalOutput(dchan[7], false);
  std::cout << "set DIGITAL channel: [" << dchan[7] << "] false" << std::endl;
  usleep(1000000);

  // set 2 digital outputs
  myboard->digitalOutput(dchan[0], true);
  std::cout << "set DIGITAL channel: [" << dchan[0] << "] true" << std::endl;
  usleep(1000000);
  myboard->digitalOutput(dchan[1], true);
  std::cout << "set DIGITAL channel: [" << dchan[1] << "] true" << std::endl;
  usleep(1000000);
  // reset 2 digital outputs
  myboard->digitalOutput(dchan[0], false);
  std::cout << "set DIGITAL channel: [" << dchan[0] << "] false" << std::endl;
  usleep(1000000);
  myboard->digitalOutput(dchan[1], false);
  std::cout << "set DIGITAL channel: [" << dchan[1] << "] false" << std::endl;
  usleep(1000000);

  int chan = (argc > 1) ? atoi(argv[1]) : 13;

  int raw = myboard->analogInput(chan);
  double mV = myboard->phyVal(chan, raw);
  std::cout << " myboard->analogInput(" << chan << ") " << raw << " -> " << mV << " mV " << std::endl;

  // read 10 times the selected analog channel
  double sum(0), sumq(0);
  std::cout << " going to test analog input channel: " << chan << std::endl;
  for (int i=0; i<10; i++)
   {
  myboard->digitalOutput(dchan[4], true);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] true" << std::endl;
    usleep(10000);
  myboard->digitalOutput(dchan[4], false);
  std::cout << "set DIGITAL channel: [" << dchan[4] << "] false" << std::endl;

    usleep(1000000);
    double v = myboard->getVal(chan);
    std::cout << " myboard->analogInput(" << chan << ") " << myboard->analogInput(chan) << " -> " << v << " mV " << std::endl;
    sum += v;
    sumq += v*v;
   }
  sum/= 10.;
  sumq/= 10.;
  double std = sqrt(sumq - sum*sum);
  std::cout << chan << " " << sum << " " << std << std::endl;

  usleep(400000);

  // read digital channels from 37 to 44 (? signals from the motors ?)
  for (int chan=37; chan<45; chan++)
   {
    int v = myboard->digitalInput(chan);
    std::cout << " myboard->digitalInput(" << chan << ") " << v << std::endl;
    usleep(400000);
   }

  // pulse the 4 digital outputs
  myboard->delayedPulse(8,20,1);
  std::cout << "Delayed pulse on DIGITAL channel: [" << dchan[4] << "]" << std::endl;

  arduinoX::cleanup();
  return 0;
 }
uint16_t laser(15);


