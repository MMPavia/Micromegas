
#include "picoam.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
//#include <time.h>

int main(int argc, char* argv[])
 {
  picoam mypico("/dev/ttyUSB5");
  std::stringstream param1;
  param1 << argv[1]; //Durata acquisizione
  int T_mis;
  param1 >> T_mis;
  std::stringstream param2;
  param2 << argv[2]; //Frequenza di acquisizione
  int dt;
  param2 >> dt;

  //std::cout << T_mis << ' ' << dt << std::endl;

  std::string cmd = "\rm\r";
  mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "\rm\r22\r"; // Abilita comunicazione seriale
  mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  cmd = "\rm\r12\r"; // Selezione del canale 1
  mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);

  switch (dt)
    {
    case(1):
      cmd = "\rm\r62\r";
      mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
      break;
    case(10):  
      cmd = "\rm\r63\r";
      mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
      break;
    case(60):  
      cmd = "\rm\r64\r";
      mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
      break;
    }
  
  std::ofstream myfile;
  int n = T_mis/dt;
  myfile.open ("test_picoam.txt", std::ios::app);
  for (int i=0; i<n; i++)
    {
      std::string val;
      mypico.readpico(val);
      sleep(dt);
      myfile << val << std::endl;
    }
  /*time_t start, now;
    start = time(NULL);
  
  int n;
  do
    {
      mypico.readpico(); // Lettura pico
      usleep(100);
      now = time(NULL);
      n++;
    }
    while (difftime(now, start)<T_mis);
    std::cout << n << std::endl;*/
 
  cmd = "\rm\r21\r"; // Disabilita comunicazione seriale
  mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));
  usleep(100);
  /*
  
  std::string dt_str;
  switch (dt)
    {
    case 1:
      dt_str = "1\r";
      dt = 0;
    case 2:
      dt_str = "2\r";
      dt = 1;
    case 3:
      dt_str = "3\r";
      dt = 10;
    case 4:
      dt_str = "4\r";
      dt = 60;
    }
 
  cmd = dt_str.c_str();
  mypico.writeline(cmd.c_str(), strlen(cmd.c_str()));

  cmd = "22\r";*/ // Abilita comunicazione seriale
  
  /*double elapsed;
  while (elapsed<T_mis)
  {
      mypico.readpico();
      /*elapsed += dt;
      }*/
  return 0;
 }
