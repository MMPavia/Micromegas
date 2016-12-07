#ifndef _FP_AI_100_H_
#define _FP_AI_100_H_

#include <stdint.h>
#include <map>

#include "fp_mod.h"

enum class ai100range
 { R_0_24mA=0, R_3_24mA=1, R_pm24mA=2, R_pm6V=5, R_0_6V=6, R_pm1200mV=7,
  R_0_1200mV=8, R_0_18V=0x0E, R_pm30V=0x0F, R_0_30V=0x11, R_pm15V=0x12 };

enum class ai100unit { uA=0, mV=1 };

class fp_ai_100 : public fp_mod
 {
  public:
   static fp_ai_100* create ( uint16_t mod );
   static void cleanup();
  protected:
   fp_ai_100( uint16_t mod );
   virtual ~fp_ai_100() {}

  public:
   inline void setRange( uint16_t chan, ai100range r ) { fp_range( chan, uint16_t(r)); }
   virtual uint16_t fp_discrete() { return 0xffff; } 
   virtual uint16_t fp_discrete( uint16_t chan, bool onoff ) { return 0xffff; }
   void setPhysScale( uint16_t chan, double RFS, double RZS, double EFS, double EZS);

   inline double getVal( uint16_t chan )
    {
     double v=double(fp_read16(chan))/65536.0;
     return v*m_range[chan]+m_vmin[chan];
    }
   inline double getPhyVal( uint16_t chan )
    {
     return m_slope[chan]*this->getVal(chan) + m_intcpt[chan];
    }
  private:
   int32_t m_vmin[8], m_vmax[8], m_range[8];
   bool m_mV[8];
   double m_RFS[8], m_RZS[8], m_EFS[8], m_EZS[8];
   double m_slope[8], m_intcpt[8];
   static std::map < uint16_t, fp_ai_100* > mpoSssInstance;

 };
#endif // _FP_AI_100_H_
