
#include <iostream>

#include "fp_di_XXX.h"

std::map < uint16_t, fp_di_XXX* > fp_di_XXX::mpoSssInstance;

fp_di_XXX::fp_di_XXX( uint16_t mod ) : fp_mod(mod)
 {
std::cout << " CTOR FP-DI-XXX mod: " << mod << std::endl;
 }

fp_di_XXX* fp_di_XXX::create( uint16_t mod )
 {
  if (mpoSssInstance.count(mod) == 0) mpoSssInstance[mod] = new fp_di_XXX( mod );
  return mpoSssInstance[mod];
 }

void fp_di_XXX::cleanup()
 {
  for (std::map<uint16_t,fp_di_XXX*>::iterator it=mpoSssInstance.begin(); it!=mpoSssInstance.end(); ++it)
   {
    std::cout << "fp_di_XXX::cleanup deleting " << it->first << " => " << it->second << std::endl;
    delete it->second;
   }
 }
