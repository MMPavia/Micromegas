
#include "rs232laser.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
//#include <time.h>

int main(int argc, char* argv[])
 {
  rs232laser mylaser("/dev/ttyS0");
  std::string cmd = "TG\r";
  std::cout << "cmd " << cmd.c_str() << '\n' << "length " << strlen(cmd.c_str()) << std::endl;
  mylaser.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10000);
  std::string val;
  mylaser.readlaser(val);
  std::cout << " GOT " << val << std::endl;
  return 0;
 }
