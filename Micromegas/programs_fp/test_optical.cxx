
#include <iostream>
#include "optline.h"

int main (int argc, char** argv)
 {
  optline myoptl("/dev/ttyUSB3");
  double ol = myoptl.readline();
  std::cout << "optline reading " << ol << std::endl;

  return 0;
 }
