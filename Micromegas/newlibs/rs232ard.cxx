
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#include "rs232ard.h"

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

void rs232ard::rs232ard_init()
 {
  int fd = open (m_tty.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0)
   {
    printf ("error %d opening %s: %s\n", errno, m_tty.c_str(), strerror (errno));
    throw fd;
   }
  set_interface_attribs (fd, B57600, CS8, 'N', 1);  // 57600 baud, 8-bit chars, NO parity, 1 stop bit
  close(fd);
 }
