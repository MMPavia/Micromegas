
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



  // relevant channels to read 
  uint16_t laser(15);
  do
   {

    std::cout << "triggering " << std::endl;
    myboard->digitalOutput(dchan[0], true);
    std::cout << "set DIGITAL channel: [" << dchan[0] << "] true" << std::endl;
    usleep(10000);
    // reset digital output
    myboard->digitalOutput(dchan[0], false);
    std::cout << "set DIGITAL channel: [" << dchan[0] << "] false" << std::endl;
    usleep(10000);

    std::cout << "reading " << std::endl;
    double vlaser = myboard->getPhyVal(laser);
    std::cout << "laser value " << vlaser << std::endl;
    usleep(500000);
   }
  while(1);

  arduinoX::cleanup();
  return 0;
 }
