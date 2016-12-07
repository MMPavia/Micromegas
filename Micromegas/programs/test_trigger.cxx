
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include "arduinoX.h"

int ndigital = 1;
// int dchan[4] = { 4, 7, 8, 12 };
	// channels 4, 7, 8, 12 are connected to 4 relays

int dchan[1] = { 8 };
arduinoDIO dway[4] = { arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::OUTPUT, arduinoDIO::OUTPUT };

int main (int argc, char** argv)
 {
  arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
  usleep(1000000);

  // define INPUT and OUTPUT digital channels
  // ndigital = 8, first 4 channels as INPUT, last 4 channels as OUTPUT 
  // 		channels defined in dchan, I/O mode in dway
  myboard->digitalSetup( ndigital, dchan, dway );
  usleep(1000000);

  // set digital output
  myboard->digitalOutput(dchan[0], true);
  std::cout << "set DIGITAL channel: [" << dchan[0] << "] true" << std::endl;
  usleep(10000);
  // reset digital output
  myboard->digitalOutput(dchan[0], false);
  std::cout << "set DIGITAL channel: [" << dchan[0] << "] false" << std::endl;
  usleep(10000);

  arduinoX::cleanup();
  return 0;
 }
