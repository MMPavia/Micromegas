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

#include "fp_ai_100.h"
#include "fp_do_401.h"
#include "fp_di_XXX.h"
#include "gauge.h"
#include "optline.h"
#include "motors.h"

#include "my_pipe.h"

#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M0Data/mapping/"
#define PI 3.14159265


using namespace std;

// parametri da utilizzare: ./map_trap -w 2160 1120 -x 80 20 -y 80 20 -t 3 10



 

double misura(  optline* optl )
 {

	double ol = optl->readline();
	return ol;
 }



// *** MAIN ***

int main (int argc, char** argv)
{
	
	optline myoptl("/dev/ttyUSB3");

	
	std::cout << misura(&myoptl) << std::endl;

	return 0;
 }
