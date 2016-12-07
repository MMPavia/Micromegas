
#include <unistd.h>
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "keithley2.h"

void keithley::readline(double &T_mis, double &dt, int &dec, double &val)
 {
   if (dec>6)
    {
      std::cout << "Massimo 6 decimali" << std::endl;
      dec = 6;
    }
 
  char buf[1000];   
  std::string cmd = "READ?\r\n";
  write (m_fd, cmd.c_str(), strlen(cmd.c_str()));
  int n = read (m_fd, buf, 1000);

  while ((buf[n-1] != 10) && (buf[n-1] != 127) && (n < 1000))
   {
     int m = read (m_fd, buf+n, 1000-n);
     n += m;
   }
  
  char* ptr = buf;
  
  std::string ret = ptr;
  std::string strCut = ret.substr(1,13);
  std::stringstream sskth;
  sskth << strCut;
  double dblCut,rndCut;
  sskth >> dblCut;
  rndCut = dblCut/pow(10,-9);
  val = round(rndCut * pow(10,dec))/pow(10,dec);
  return;
 }                                                                                            
