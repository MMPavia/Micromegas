#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "particlecounter.h"
#include <iostream>
#include <sstream>
std::string particlecounter::reading()
 {
  int i = write (m_fd, "A", 1);
  if (i<0)
	throw "Error";
      
  usleep(100000);
  char buf[100] = "";
  std::stringstream ss;

  while(1)
  {
    char buffer[100];
    ssize_t length = read(m_fd, &buffer, sizeof(buffer));
    if (length == -1)
    {
        printf("Error reading from serial port\n");
        break;
    }
    else if (length == 0)
    {
        printf("No more data\n");
        break;
    }
    else
    {
        buffer[length] = '\0';
        printf("%s", buffer);
	ss << buffer;
    }
  }


  return ss.str();


}

void particlecounter::connect()
 {
  int i = write (m_fd, "U", 1);       // start counting
  if (i<0)
	throw "Error";
  std::cout << m_fd << std::endl;
  std::cout << "  " <<std::endl;
  char buf[200];
  read(m_fd, buf, 200);
  usleep(100000);

	

 }

void particlecounter::run()
 {
  int i = write (m_fd, "d", 1);       // start counting	
  if (i<0)
	throw "Error";
 }

