
#include <string>
#include <iostream>
#include <unistd.h>
#include "motors.h"

int main (int argc, char** argv)
 {
  std::string cmd("\r\n");
  motors motp("/dev/ttyUSB1");
  motp.mot_write(cmd.c_str());
  usleep(1000);
  motp.mot_read();
  motp.mot_dump();

  return 0;
 }
