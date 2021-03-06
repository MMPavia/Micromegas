
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include "arduinoX.h"

//batch start: pin 4 (first relay from top)
//laser zero: pin 7 (second relay from top)

int ndigital = 1;
// int dchan[4] = { 4, 7, 8, 12 };
	// channels 4, 7, 8, 12 are connected to 4 relays

//4 //pin for relee
int dchan[2] = {53,4};//4
arduinoDIO dway[4] = { arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::OUTPUT, arduinoDIO::OUTPUT };

int main (int argc, char** argv)
 {
  arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
  usleep(1000000);
  myboard->digitalSetup( ndigital, dchan, dway );
  usleep(100000);

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

  myboard->digitalOutput(dchan[1], 1);

  // relevant channels to read 
  uint16_t laser(15);
  do
   {

 // set 2 digital outputs
     //myboard->digitalOutput(dchan[0], true);
     //std::cout << "set DIGITAL channel: [" << dchan[0] << "] true" << std::endl;
     //usleep(1000000);
     //myboard->digitalOutput(dchan[0], false);
     //std::cout << "set DIGITAL channel: [" << dchan[1] << "] true" << std::endl;
     //usleep(1000000);
     myboard->delayedPulse(dchan[0], 5, 20);
// myboard->delayedPulse(dchan[0], 5, 5000000);
     usleep(200000);//200ms

    double vlaser = myboard->getPhyVal(laser);
    std::cout << "laser value now " << vlaser << std::endl;
    //usleep(500000);
    usleep(2000000);
   }
  while(1);

  arduinoX::cleanup();
  return 0;
 }
