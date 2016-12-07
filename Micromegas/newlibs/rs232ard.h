#ifndef _RS232_ARD_H_
#define _RS232_ARD_H_

#include <string>

class rs232ard
 {
  protected:
   rs232ard ( std::string tty ) : m_tty(tty) {}
   ~rs232ard() {}
   std::string& ard_tty() { return m_tty; }
   void rs232ard_init();
  private:
   std::string m_tty;
 };

#endif // _RS232_ARD_H_
