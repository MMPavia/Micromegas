#ifndef _MYRS232_H_
#define _MYRS232_H_

#include <cstdio>
#include <string>

enum class rs232type { FIELDPOINT=1, MOTORS=2, OPTLINE=4, GAUGE=8, MITUTOYO=16, KEITHLEY=32, PARTICLECOUNTER=64, ARDUINO=128, PICOAM=256, LASER=512 };

class rs232
 {
  protected:
   rs232 ( rs232type type, std::string tty );
   ~rs232();
   int m_fd;
   FILE* m_fp;
 };

#endif // _MYRS232_H_
