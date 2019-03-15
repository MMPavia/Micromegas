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

#include "arduinoX.h"
#include "optline3D.h"

#include "motors.h"

#include "my_pipe.h"

std::string pipe_name("/tmp/atlas/moregluepipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {0};
my_pipe p(pipe_name.c_str(), cmd);

#define error_message(F,E) printf(F, E)

using namespace std;

motors* motp(NULL);

void next ( string  gx, arduinoX* myboard )
{

    motp->mot_write(gx);
    motp->mot_read();
    uint16_t mstati(0);
    uint16_t mstat(0);
    cout << "starting next command " << gx << endl;
    do
    {
        mstat = 0;
        for (int chan=38; chan<43; chan++){
            mstati = myboard->digitalInput(chan);
            mstat += mstati;
        }
        usleep(10000);
    }
    while (mstat > 0);
    usleep(2000000);
}


int main (int argc, char** argv)
{

  cout << "starting program " << endl;
	arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");
	motp = new motors("/dev/ttyUSB1");
        optline  optl("/dev/ttyUSB3");

        float length(0);
	std:vector<float>  ola; 

	cout << "imotlist initialization " << endl;

	uint16_t imotlist[] { 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112 };
	string smotlist[] { "g101\r\n", "g112\r\n", "g103\r\n", "g104\r\n", "g112\r\n", "g105\r\n", "g106\r\n", "g112\r\n", "g107\r\n", "g108\r\n", "g112\r\n", "g109\r\n", "g110\r\n", "g102\r\n", "g111\r\n" };

	cout << "reseting motors " << endl;

	for (size_t i=0; i<12; i++) motp->mot_reset(imotlist[i]);

	 cout << "memorize programs " << endl;

        // frame bottom
	string m101_2 = "m101-2100602031+0\r\n"; 
	string m101_4 = "m101-4075331731-0\r\n"; 
	cout << m101_2 << " " << m101_4 << endl;
	motp->mot_write(m101_2); 
	motp->mot_read();
	motp->mot_write(m101_4); 
	motp->mot_read();
	motp->mot_write("r101\r\n");
	motp->mot_read();
        cout.flush();      


	// diagonale
        string m102_1 = "m102-1104252031-0\r\n";
        string m102_2 = "m102-2019920531+0\r\n";
	cout << m102_1 << " " << m102_2 << endl;
        motp->mot_write(m102_1.c_str());
        motp->mot_read();
        motp->mot_write(m102_2.c_str());
        motp->mot_read();
	motp->mot_write("r102\r\n");
	motp->mot_read();
        cout.flush();      

        string m112_1 = "m112-1100502031-0\r\n";
        string m112_2 = "m112-2019920531+0\r\n";
	cout << m112_1 << " " << m112_2 << endl;
        motp->mot_write(m112_1.c_str());
        motp->mot_read();
        motp->mot_write(m112_2.c_str());
        motp->mot_read();
	motp->mot_write("r112\r\n");
	motp->mot_read();
        cout.flush();      


	//traversa 4
        string m103_2 = "m103-2140382031-0\r\n";
        string m103_4 = "m103-4105101731-0\r\n";
        motp->mot_write(m103_2.c_str());
        motp->mot_read();
        motp->mot_write(m103_4.c_str());
        motp->mot_read();
	motp->mot_write("r103\r\n");
	motp->mot_read();
        cout.flush();      
        string m104_2 = "m104-2143442031+0\r\n";
        string m104_4 = "m104-4107391731-0\r\n";
        string m104_1 = "m104-1008256031-0\r\n";
        motp->mot_write(m104_2.c_str());
        motp->mot_read();
        motp->mot_write(m104_4.c_str());
        motp->mot_read();
        motp->mot_write(m104_1.c_str());
        motp->mot_read();
	motp->mot_write("r104\r\n");
	motp->mot_read();
        cout.flush();      

	// traversa 3
        string m105_2 = "m105-2180162031-0\r\n";
        string m105_4 = "m105-4134871731-0\r\n";
        motp->mot_write(m105_2.c_str());
        motp->mot_read();
        motp->mot_write(m105_4.c_str());
        motp->mot_read();
	motp->mot_write("r105\r\n");
	motp->mot_read();
        string m106_2 = "m106-2183222031+0\r\n";
        string m106_4 = "m106-4137161731-0\r\n";
        string m106_1 = "m106-1008256031-0\r\n";
        motp->mot_write(m106_2.c_str());
        motp->mot_read();
        motp->mot_write(m106_4.c_str());
        motp->mot_read();
        motp->mot_write(m106_1.c_str());
        motp->mot_read();
	motp->mot_write("r106\r\n");
	motp->mot_read();

	// traversa 2
	string m107_2 = "m107-2219942031-0\r\n";
        string m107_4 = "m107-4164641731-0\r\n";
        motp->mot_write(m107_2.c_str());
        motp->mot_read();
        motp->mot_write(m107_4.c_str());
        motp->mot_read();
	motp->mot_write("r107\r\n");
	motp->mot_read();
        string m108_2 = "m108-2222382031+0\r\n";
        string m108_4 = "m108-4159531731-0\r\n";
        string m108_1 = "m108-1008256031-0\r\n";
        motp->mot_write(m108_2.c_str());
        motp->mot_read();
        motp->mot_write(m108_4.c_str());
        motp->mot_read();
        motp->mot_write(m108_1.c_str());
        motp->mot_read();
	motp->mot_write("r108\r\n");
	motp->mot_read();

	// traversa 1
	string m109_2 = "m109-2259102031-0\r\n";
        string m109_4 = "m109-4185811731-0\r\n";
        motp->mot_write(m109_2.c_str());
        motp->mot_read();
        motp->mot_write(m109_4.c_str());
        motp->mot_read();
	motp->mot_write("r109\r\n");
	motp->mot_read();
        string m110_2 = "m110-2262162031+0\r\n";
        string m110_4 = "m110-4188001731-0\r\n";
        string m110_1 = "m110-1008256031-0\r\n";
        motp->mot_write(m110_2.c_str());
        motp->mot_read();
        motp->mot_write(m110_4.c_str());
        motp->mot_read();
        motp->mot_write(m110_1.c_str());
        motp->mot_read();
	motp->mot_write("r110\r\n");
	motp->mot_read();

        //frame
	string m111_2 = "m111-2301942031-0\r\n";
        string m111_4 = "m111-4216471731-0\r\n";
        motp->mot_write(m111_2.c_str());
        motp->mot_read();
        motp->mot_write(m111_4.c_str());
        motp->mot_read();
	motp->mot_write("r111\r\n");
	motp->mot_read();

        cout.flush();      

	for(size_t i=0; i<15; i++) {
	   cout << "command " << smotlist[i] << endl; 
           next(smotlist[i], myboard); 
           ola = optl.readlineXYZ();
	   cout << " x " << ola.at(0) << " y " << ola.at(1) << endl;


           if (*cmd != '\0')	// **** PIPE ****
           {
              printf("found string %s\n", cmd);
              if (strcasecmp("pause", cmd) == 0)
              {
                 *cmd = '\0';
                 do 
                 { 
                    usleep(100);
                 } 
                 while (!( strcasecmp("resume", cmd) == 0 ||
                           strcasecmp("quit", cmd) == 0 ||
                           strcasecmp("kill", cmd) == 0 ));
              }
              if (strcasecmp("quit", cmd) == 0) goto QuitNow;
              if (strcasecmp("kill", cmd) == 0) exit(1);
              *cmd = '\0';
            }

         }
 


QuitNow:

  delete motp;
  uint16_t mstati(0);
  uint16_t mstat(0);
  do
    {
        mstat = 0;
        for (int chan=38; chan<43; chan++){
            mstati = myboard->digitalInput(chan);
            mstat += mstati;
        }
        usleep(10000);
    }
    while (mstat > 0);
    usleep(2000000);



  return 0;
 }

