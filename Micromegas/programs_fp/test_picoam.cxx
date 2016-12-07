
#include "picoam.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
//#include <time.h>

using namespace std;

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
  std::stringstream param3;
  param3 << argv[2]; //Frequenza di acquisizione
  string name;
  param3 >> name;
  std::stringstream param4;
  param4 << argv[2]; //Frequenza di acquisizione
  string side;
  param4 >> side;
  
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

        time_t now = time(0);
        tm *ltm = localtime(&now);
       
        ostringstream outname;
        ostringstream date;
            
        date << 1900+ ltm->tm_year;
        if (1+ltm->tm_mon < 10 ) date << "0" << 1+ltm->tm_mon << ltm->tm_mday ;
        else date << 1+ltm->tm_mon << ltm->tm_mday;
        cout << date.str() << endl;
        int32_t min = ltm->tm_min + ltm->tm_hour*60 + 10000; // aggiunto minuto
        outname  << "/home/atlas/Micromegas/M0Data/test_elettrici/picoamp/Results/"<< name << "_Picoameter_" << side  << date.str() << ".txt";
    
    
  myfile.open (outname.str(),std::ios::app); 
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

  myfile.close();

  return 0;
 }
