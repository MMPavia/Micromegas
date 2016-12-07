
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>

#include "keithley.h"

void keithley::readline( double& val, double& time, double& stat )
 {
  char buf[1000];   
  std::string cmd = "READ?\r\n";
  write (m_fd, cmd.c_str(), strlen(cmd.c_str()));
  int n = read (m_fd, buf, 1000);  // read up to 1000 characters if ready to read
  while ((buf[n-1] != 10) && (buf[n-1] != 127) && (n < 1000))
   {
     int m = read (m_fd, buf+n, 1000-n);
     n += m;
   }
  // do some cleanup
  char* isXON = strchr( buf, 17);
  if (isXON != NULL)
   {
     *isXON = '\0';                          // set line end at XON pos.
   }
  else
   {
     char* isCR = strchr( buf, 13);
     if (isCR != NULL) *isCR = '\0';         // set line end at CR pos.
     else {}                                 // ? broken string to be handled
   }
  char* ptr = buf;
  if (buf[0] == 19) ptr++;                   // skip XOFF
  char* cm = strchr(ptr, ',');
  while (cm != NULL)                         // replace commas with blanks
   {
     *cm = ' ';
     cm = strchr(ptr, ',');
   }
  std::stringstream sskth(ptr);
  sskth >> val >> time >> stat;

  return;
 }
