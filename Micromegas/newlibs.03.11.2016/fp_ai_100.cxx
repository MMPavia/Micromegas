
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

#include "fp_ai_100.h"

std::map < uint16_t, fp_ai_100* > fp_ai_100::mpoSssInstance;

fp_ai_100::fp_ai_100( uint16_t mod ) : fp_mod(mod)
 {
std::cout << "CTOR FP-AI-100 mod: " << mod << std::endl;
  for (uint16_t chan=0; chan<8; chan++)
   {
    m_RFS[chan] = m_RZS[chan] = m_EFS[chan] = m_EZS[chan] = 0;
    m_slope[chan] = m_intcpt[chan] = 0;
    uint32_t r = fp_range(chan);
    switch(ai100range(r))
     {
      case ai100range::R_0_24mA:
	m_vmin[chan] = 0;
	m_vmax[chan] = 24000;
	m_mV[chan] = false;
	break;
      case ai100range::R_3_24mA:
	m_vmin[chan] = 3500;
	m_vmax[chan] = 24000;
	m_mV[chan] = false;
	break;
      case ai100range::R_pm24mA:
	m_vmin[chan] = -24000;
	m_vmax[chan] = 24000;
	m_mV[chan] = false;
	break;
      case ai100range::R_pm6V:
	m_vmin[chan] = -6000;
	m_vmax[chan] = 6000;
	m_mV[chan] = true;
	break;
      case ai100range::R_0_6V:
	m_vmin[chan] = 0;
	m_vmax[chan] = 6000;
	m_mV[chan] = true;
	break;
      case ai100range::R_pm1200mV:
	m_vmin[chan] = -1200;
	m_vmax[chan] = 1200;
	m_mV[chan] = true;
	break;
      case ai100range::R_0_1200mV:
	m_vmin[chan] = 0;
	m_vmax[chan] = 1200;
	m_mV[chan] = true;
	break;
      case ai100range::R_0_18V:
	m_vmin[chan] = 0;
	m_vmax[chan] = 18000;
	m_mV[chan] = true;
	break;
      case ai100range::R_pm30V:
	m_vmin[chan] = -30000;
	m_vmax[chan] = 30000;
	m_mV[chan] = true;
	break;
      case ai100range::R_0_30V:
	m_vmin[chan] = 0;
	m_vmax[chan] = 30000;
	m_mV[chan] = true;
	break;
      case ai100range::R_pm15V:
	m_vmin[chan] = -15000;
	m_vmax[chan] = 15000;
	m_mV[chan] = true;
	break;
     }
    m_range[chan] = m_vmax[chan]-m_vmin[chan];
std::cout << " FP-AI-100 mod: " << mod << " chan: " << chan 
          << " range: " << m_vmin[chan] << "/" << m_vmax[chan]
          << " = " << m_range[chan] << (m_mV[chan] ? " (mV)" : " (uA)")
          << std::endl;
   }
 }

void fp_ai_100::setPhysScale( uint16_t chan, double RFS, double RZS, double EFS, double EZS)
 {
  m_RFS[chan] = RFS; m_RZS[chan] = RZS;
  m_EFS[chan] = EFS; m_EZS[chan] = EZS;
  if (RFS != RZS)
   {
    m_slope[chan] = (EFS-EZS)/(RFS-RZS);
    m_intcpt[chan] = (EZS*RFS-EFS*RZS)/(RFS-RZS);
   }
  m_RFS[chan] = RFS; m_RZS[chan] = RZS;
 }

fp_ai_100* fp_ai_100::create( uint16_t mod )
 {
  if (mpoSssInstance.count(mod) == 0) mpoSssInstance[mod] = new fp_ai_100( mod );
  return mpoSssInstance[mod];
 }

void fp_ai_100::cleanup()
 {
  for (std::map<uint16_t,fp_ai_100*>::iterator it=mpoSssInstance.begin(); it!=mpoSssInstance.end(); ++it)
   {
    std::cout << "fp_ai_100::cleanup deleting " << it->first << " => " << it->second << std::endl;
    delete it->second;
   }
 }
