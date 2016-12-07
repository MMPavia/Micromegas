#ifndef _FP_DO_401_H_
#define _FP_DO_401_H_

#include <stdint.h>
#include <map>

#include "fp_mod.h"

class fp_do_401 : public fp_mod
 {
  public:
   static fp_do_401* create ( uint16_t mod );
   static void cleanup();
  protected:
   fp_do_401( uint16_t mod );
   virtual ~fp_do_401() {}

  public:

  private:
   static std::map < uint16_t, fp_do_401* > mpoSssInstance;

 };
#endif // _FP_DO_401_H_
