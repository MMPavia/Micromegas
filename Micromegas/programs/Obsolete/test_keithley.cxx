#include "keithley.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char* argv[])
 {
  keithley mykeith("/dev/ttyUSB5");
  std::ofstream myfile;
  std::stringstream param1; //Durata acquisizione
  param1 << argv[1];
  double T_mis;
  param1 >> T_mis;
  std::stringstream param2; //Intervallo tra misure
  param2 << argv[2];
  double dt;
  param2 >> dt;
 
  std::string cmd = "*RST\r\n";
  mykeith.writecmd ( cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "SYST:ZCH ON\r\n";
  mykeith.writecmd ( cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "VOLT:GUAR ON\r\n";
  mykeith.writecmd ( cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "VOLT:RANG 2\r\n";
  mykeith.writecmd ( cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "SYST:ZCOR ON\r\n";
  mykeith.writecmd ( cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "SYST:ZCH OFF\r\n";
  mykeith.writecmd ( cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);

  myfile.open ("test_keithley.txt",std::ios::app);
  int elapsed;
  while (elapsed<(T_mis*60))
    {
      double val, time, stat;
      mykeith.readline(val, time, stat);
      usleep(dt*1000000);
      std::stringstream res_str;
      res_str << val << ", " << time << ", " << stat;
      std::cout << res_str.str() << std::endl;
      myfile << res_str.str() << " V\n";
      elapsed += dt;
    }
  myfile.close();
  return 0;
 }
