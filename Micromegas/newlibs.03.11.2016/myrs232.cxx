
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "myrs232.h"

#define error_message(F,E) printf(F, E)

static int set_interface_attribs (int fd, int speed, int chsz, char parity, int nstopb)
 {

        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                error_message ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | chsz;
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
        
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	switch (parity)
         {
          case 'O':
            tty.c_cflag |= (PARENB | PARODD);
            break;
          case 'E':
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;
          case 'N':
          default:
            tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
            break;
         }
        (nstopb == 2) ?  tty.c_cflag |= CSTOPB : tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;
        
        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
        	error_message ("error %d from tcsetattr", errno);
        	return -1;
        }
        return 0;
 }

rs232::rs232 ( rs232type type, std::string tty )
 {
  int fd = open (tty.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0)
   {
    printf ("error %d opening %s: %s\n", errno, tty.c_str(), strerror (errno));
    throw fd;
   }
  m_fd = fd;
  m_fp = fdopen(fd, "r");
  switch (type)
   {
    case rs232type::FIELDPOINT:
	set_interface_attribs (fd, B57600, CS8, 'N', 1);  // 57600 baud, 8-bit chars, no parity, 1 stop bit
	break;
    case rs232type::MOTORS:
	set_interface_attribs (fd, B19200, CS8, 'N', 1);  // 19200 baud, 8-bit chars, no parity, 1 stop bit
	break;
    case rs232type::OPTLINE:
        set_interface_attribs (fd, B4800, CS7, 'E', 1);   // 4800 baud, 7-bit chars, even parity, 1 stop bit
	break;
    case rs232type::GAUGE:
        set_interface_attribs (fd, B9600, CS7, 'E', 2);   // 9600 baud, 7-bit chars, even parity, 2 stop bits
	break;
    case rs232type::MITUTOYO:
        set_interface_attribs (fd, B9600, CS8, 'O', 1);   // 9600 baud, 8-bit chars, odd parity, 1 stop bit
	break;
    case rs232type::KEITHLEY:
        set_interface_attribs (fd, B2400, CS8, 'N', 1);   // 2400 baud, 8-bit chars, NO parity, 1 stop bit
	break;
    case rs232type::PARTICLECOUNTER:
        set_interface_attribs (fd, B9600, CS8, 'N', 1);   // 9600 baud, 8-bit chars, NO parity, 1 stop bit
	break;
    case rs232type::ARDUINO:
        set_interface_attribs (fd, B57600, CS8, 'N', 1);  // 57600 baud, 8-bit chars, NO parity, 1 stop bit
        break;
    case rs232type::PICOAM:
        set_interface_attribs (fd, B38400, CS8, 'N', 1);    // 38400 baud, 8-bit chars, NO parity, 1 stop bit
        break;
   }
 }

rs232::~rs232()
 {
  fclose(m_fp);
  close(m_fd);
 }
