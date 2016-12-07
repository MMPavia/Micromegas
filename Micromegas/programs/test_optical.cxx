#include <unistd.h>
#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <sys/time.h>

#include "optline3D.h"
//#include "optline.h"

unsigned long t0(0);

int howmanyus()
 {
  struct timeval tm;
  gettimeofday(&tm, NULL);
  return (tm.tv_sec-t0)*1000000+tm.tv_usec;
 }

int main (int argc, char** argv)
 {
  optline myoptl("/dev/ttyUSB3");
  double olx, oly, olz;
  char* ola;
  t0 = time(NULL);
  unsigned int q = howmanyus();
  for (int i=0; i<10; i++)
   {
    ola = myoptl.readlineXYZ();
   }
  unsigned int w = howmanyus();
  std::cout << " ooooola " << ola << " time " << (w-q) << std::endl;
  usleep(200000);
  unsigned int s = howmanyus();
  for (int i=0; i<10; i++)
   {
    olx = myoptl.readlineX();
    oly = myoptl.readlineY();
    olz = myoptl.readlineZ();
   }
  unsigned int f = howmanyus();

  std::cout << "total time " << f-s <<  std::endl;
  std::cout << "optline reading " << olx << " " << oly << " " << olz <<  std::endl;
  std::cout << "optline reading " << ola <<  std::endl;

  return 0;
 }
