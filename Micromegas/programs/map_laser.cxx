//mapping using the blu laser
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
std::cout << " qui sono AAA " << std::endl;
  arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
std::cout << " qui sono BBB " << std::endl;
  usleep(1000000);

  // calibrating the channels
  uint16_t mod(0);
  uint16_t chan(0);
  double RFS, RZS, EFS, EZS;
  std::string name, unit, descr;
  std::string line;

  std::fstream f("/home/atlas/Micromegas/inputfile/arduino_analog.txt", std::fstream::in);

std::cout << " qui sono " << std::endl;

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
	//remove the following lines to remove the external trigger
    std::cout << "triggering " << std::endl;
    myboard->digitalOutput(dchan[0], true);
    std::cout << "set DIGITAL channel: [" << dchan[0] << "] true" << std::endl;
    usleep(10000);
    // reset digital output
    myboard->digitalOutput(dchan[0], false);
    std::cout << "set DIGITAL channel: [" << dchan[0] << "] false" << std::endl;
    usleep(10000);
    
    std::cout << "reading " << std::endl;
    int rawcounts = myboard->analogInput(laser);
    double mvolts = myboard->getVal(laser);
    double vlaser = myboard->getPhyVal(laser);
    std::cout << "laser value (mm) " << vlaser << std::endl;
    std::cout << "raw counts "<<rawcounts<<std::endl;
    std::cout << "mvolts " << mvolts << std::endl;
    usleep(500000);
   }
  while(1);

  arduinoX::cleanup();
  return 0;
 }
