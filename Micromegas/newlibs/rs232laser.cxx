
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctype.h>

#include "rs232laser.h"

int rs232laser::readlaser(std::string &val)
 {
  char buf[100];
  int n = read (m_fd, buf, 100);
  buf[n] = '\0';
  std::cout << " got " << n << " chars " << buf << std::endl;

  val = std::string(buf);
  return 0;
 }
