
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctype.h>

#include "picoam.h"

int picoam::readpico(std::string &val)
 {
  char buf[100];
  char mis[2];
  char rng[2];
  int n = read (m_fd, buf, 100);
/*std::cout << " got " << n << " bytes: [" << std::string(buf) << "]" << std::endl;
for (int i=0; i<n; i++)
 {
  std::cout << " - [" << i << "]: " << int(buf[i]);
 }
std::cout << std::endl;*/
  for (int i=0;i<96;i++)
    {
      int ibuf = buf[i];
      int ibuf1 = buf[i+1];
      int ibuf2 = buf[i+2];
      int ibuf3 = buf[i+3];
      int ibuf4 = buf[i+4];
      if (isxdigit(ibuf) && isxdigit(ibuf1) && isblank(ibuf2) && isxdigit(ibuf3) && isxdigit(ibuf4))
	{
	  mis[0] = ibuf;
	  mis[1] = ibuf1;
	  //mis[2] = ' ';
	  rng[0] = ibuf3;
	  rng[1] = ibuf4;
	}
    }
  /*while ((buf[n-1] != 10) && (buf[n-1] != 127) && (n < 100))
    {
      int m = read (m_fd, buf+n, 100-n);
      n += m;
      }*/
  std::string strMis = mis;
  std::string strRng = rng;
  std::stringstream streamMis;
  int decMis;
  streamMis << strMis;
  streamMis >> std::hex >> decMis;

  std::stringstream streamRng;
  int decRng;
  streamRng << strRng;
  streamRng >> std::hex >> decRng;

  //std::string range;
  
  /*if (decRng == 62 || decRng == 61 || decRng == 59)
    {
      range = " nA";
    }
  else
    {
      range = " uA";
    }*/
  
  std::stringstream ssMis;
  ssMis << decMis;
  std::string charMis;
  ssMis >> charMis;
  val = charMis;//+range;
  std::cout << val << std::endl;

  //std::stringstream sspico;
  //sspico << decMis;
   
  return 0;
 }
