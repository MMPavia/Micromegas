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

#define error_message(F,E) printf(F, E)
#define path "/home/atlas/Micromegas/M0Data/mapping/"
#define PI 3.14159265

using namespace std;


// *** MAIN ***

int main (int argc, char** argv)
{

        fp_mod::fp_tty("/dev/ttyUSB0");

        fp_ai_100* ailaser1 = fp_ai_100::create(1);
        fp_ai_100* ailaser2 = fp_ai_100::create(2);
        fp_ai_100* ailaser3 = fp_ai_100::create(3);
        fp_ai_100* ailaser4 = fp_ai_100::create(4);

	fp_di_XXX* dimot1 = fp_di_XXX::create(5);


	fp_do_401* do_led1 = fp_do_401::create(6);
	fp_do_401* do_led2 = fp_do_401::create(7);
	fp_do_401* do_led3 = fp_do_401::create(8);


	fp_do_401::cleanup();
	fp_ai_100::cleanup();
	fp_di_XXX::cleanup();

	return 0;
 }
