
#include "arduinoSerial.h"

arduinoSerial* arduinoSerial::m_aserial_inst(NULL);

arduinoSerial* arduinoSerial::create( std::string tty )
 {
  if (m_aserial_inst == NULL) m_aserial_inst = new arduinoSerial( tty );
  return m_aserial_inst;
 }

void arduinoSerial::cleanup()
 {
  if (m_aserial_inst != NULL) delete m_aserial_inst;
  m_aserial_inst = NULL;
 }

