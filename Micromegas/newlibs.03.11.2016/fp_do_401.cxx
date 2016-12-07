
#define _USLEEP_ 10000

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "fp_do_401.h"

std::map < uint16_t, fp_do_401* > fp_do_401::mpoSssInstance;

fp_do_401::fp_do_401( uint16_t mod ) : fp_mod(mod)
 {
std::cout << " CTOR FP-DO-401 mod: " << mod << std::endl;
 }

fp_do_401* fp_do_401::create( uint16_t mod )
 {
  if (mpoSssInstance.count(mod) == 0) mpoSssInstance[mod] = new fp_do_401( mod );
  return mpoSssInstance[mod];
 }

void fp_do_401::cleanup()
 {
  for (std::map<uint16_t,fp_do_401*>::iterator it=mpoSssInstance.begin(); it!=mpoSssInstance.end(); ++it)
   {
    std::cout << "fp_do_401::cleanup deleting " << it->first << " => " << it->second << std::endl;
    delete it->second;
   }
 }
