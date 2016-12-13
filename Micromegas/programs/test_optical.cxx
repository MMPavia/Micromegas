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

using namespace std; 
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
  //char* ola;
  std::vector<float> ola;
  int n=10; 
  t0 = time(NULL);
  unsigned int q = howmanyus();
  for (int i=0; i<n; i++)
   {
    ola = myoptl.readlineXYZ();
   }
  unsigned int w = howmanyus();
  std::cout << " time for " << n << " A readings " << (w-q) << std::endl;
  /*
  std::string ola_s(ola);
  std::string::size_type pos;
  pos = ola_s.find("X");
  string myx= ola_s.substr(pos+2,9); 
  float fmyx= stof(myx); 
  cout << myx << " float "<< fmyx <<endl;  
 */
  cout << " x " << ola.at(0) << " y " << ola.at(1) <<" z " << ola.at(2) <<endl; 


  usleep(200000);
  unsigned int s = howmanyus();
  for (int i=0; i<10; i++)
   {
    olx = myoptl.readlineX();
    oly = myoptl.readlineY();
    olz = myoptl.readlineZ();
   }
  unsigned int f = howmanyus();

  std::cout << "total time for " << n << " xyz readings " << f-s <<  std::endl;
  std::cout << "optline X Y Z " << olx << " " << oly << " " << olz <<  std::endl;
//  std::cout << "optline A " << ola <<  std::endl;

  return 0;
 }
