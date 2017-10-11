#include "chauvinarnoux.h"

int main (int argc, char** argv)
 {
  chauvinarnoux mydev("/dev/ttyUSB0");
  mydev.loop();

  return 0;
 }
