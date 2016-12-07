#ifndef _FP_DI_XXX_H_
#define _FP_DI_XXX_H_

#include <stdint.h>
#include <map>

#include "fp_mod.h"

class fp_di_XXX : public fp_mod
 {
  public:
   static fp_di_XXX* create ( uint16_t mod );
   static void cleanup();
  protected:
   fp_di_XXX( uint16_t mod );
   virtual ~fp_di_XXX() {}
  private:
   static std::map < uint16_t, fp_di_XXX* > mpoSssInstance;

 };
#endif // _FP_DI_XXX_H_
