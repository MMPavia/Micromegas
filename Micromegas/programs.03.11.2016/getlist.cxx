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


#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M0Data/mapping/"
#define PI 3.14159265

using namespace std;


// *** MAIN ***

int main (int argc, char** argv)
{

    arduinoX* myboard = arduinoX::create("/dev/ttyUSB0");



    arduinoX::cleanup();


	return 0;
 }
