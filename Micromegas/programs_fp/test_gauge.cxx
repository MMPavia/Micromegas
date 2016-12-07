
#include <iostream>
#include "gauge.h"

int main (int argc, char** argv)
 {
  gauge mygauge("/dev/ttyUSB2");
  double gg = mygauge.readval();
  std::cout << "gauge reading " << gg << std::endl;

  return 0;
 }
