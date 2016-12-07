#include "keithley2.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char* argv[])
 {
  keithley mykeith("/dev/ttyUSB6");
  std::ofstream myfile;
  std::stringstream param1; //Durata acquisizione
  param1 << argv[1];
  double T_mis;
  param1 >> T_mis;
  std::stringstream param2; //Intervallo tra misure
  param2 << argv[2];
  double dt;
  param2 >> dt;
  std::stringstream param3; //Precisione della misura
  param3 << argv[3];
  int dec;
  param3 >> dec;
 
  std::string cmd = "*RST\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:ZCH ON\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "VOLT:GUAR ON\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "FUNC 'CURR'\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "CURR:RANG 2E-9\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:ZCOR ON\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:ZCH OFF\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);
  cmd = "SYST:TIME:RES\r\n";
  mykeith.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(10);

  myfile.open ("test_keithley2.txt",std::ios::app);
  int elapsed;
  while (elapsed<T_mis)
    {
      double r,val;
      mykeith.readline(T_mis, dt, dec, val);
      usleep(dt*1000000);
      std::cout << val << " nA" << std::endl;
      myfile << val << " nA\n";
      elapsed += dt;
    }
  myfile.close();
  return 0;
 }
