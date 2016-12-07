#ifndef _FP_MOD_H_
#define _FP_MOD_H_

#include <stdint.h>
#include <cstdio>
#include <string>
// #include <map>

#include "fp_rs232.h"

class fp_mod
 {
  public:
    uint32_t fp_moduleID();                               // !A
    int32_t fp_allIDs();                                  // !B
    uint32_t fp_range( uint16_t chan, uint16_t range );   // !D
    uint32_t fp_range( uint16_t chan );                   // !E
    uint16_t fp_read16( uint16_t chan );                  // !F
    virtual uint16_t fp_discrete();                               // !J
    virtual uint16_t fp_discrete( uint16_t chan, bool onoff );    // !L
    uint32_t fp_status();                                 // !N
    uint32_t fp_status( uint16_t chan );                  // !O

  protected:
    fp_mod( uint16_t mod );
    ~fp_mod();
  private:
    uint16_t m_id;
    char* m_buffer;
    size_t m_size;
    static std::string m_tty;
    static uint16_t m_modnr;
    static fp_rs232* m_fprs232;
    static int m_fd;
    static FILE* m_fp;
  public:
    static void fp_tty( std::string tty );
 };

#endif // _FP_MOD_H_
