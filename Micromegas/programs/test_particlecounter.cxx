
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string>
#include "particlecounter.h"

int main (int argc, char** argv)
 {
  particlecounter mypc("/dev/ttyUSB4");
  mypc.connect();
  mypc.run();
  for (int i=0; i<120; i++)
   {
    printf("%d..",120-i); fflush(stdout);
    sleep(1);
   }
  std::cout << std::endl;
  
  std::string rd=mypc.reading();
  std::cout << rd << std::endl;

  return 0;
 }
