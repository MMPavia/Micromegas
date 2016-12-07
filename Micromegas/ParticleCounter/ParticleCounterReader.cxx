#define _USLEEP_ 10000

#include <unistd.h>
#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include "particlecounter.h"




#define error_message(F,E) printf(F, E)

#define PI 3.14159265


using namespace std;



// *** MAIN ***

int main (int argc, char** argv)
{

	particlecounter myparcount("/dev/ttyUSB4");


	myparcount.connect();

	myparcount.run();

	sleep(120);
	


	std::stringstream test;
	test << myparcount.reading();
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, ' '))
	{
	   seglist.push_back(segment);
	}


	for (int i = 0; i < seglist.size(); i++)
		std::cout << seglist[i] << std::endl;

	

	std::stringstream file_name;
	file_name << "/home/atlas/Micromegas/ParticleCounter/outfile/";

	    time_t t = time(0);   // get time now
	    struct tm * now = localtime( & t );
	    file_name << (now->tm_year + 1900) << '-' 
         << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << '-' << std::setw(2) << std::setfill('0')
         <<  now->tm_mday << ".txt";

	     fstream appendFileToWorkWith;

     appendFileToWorkWith.open(file_name.str().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);


      // If file does not exist, Create new file
      if (!appendFileToWorkWith ) 
      {


        appendFileToWorkWith.open(file_name.str().c_str(),  fstream::in | fstream::out | fstream::trunc);
        appendFileToWorkWith <<"\n";
        appendFileToWorkWith.close();

       } 
      else   
      {    // use existing file
         cout<<"success "<<file_name <<" found. \n";


	appendFileToWorkWith << now -> tm_hour << "-" <<  now -> tm_min << "-" << now -> tm_sec <<   "\t" << seglist[5]  <<   "\t" << seglist[6] <<   "\t" << seglist[7] <<   "\t" << seglist[8] <<"\n";

         appendFileToWorkWith.close();
         cout<<"\n";

    }

   
	
 }
