#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#include <iostream>

#define error_message(F,E) printf(F, E)

static int set_interface_attribs (int fd, int speed, int chsz, char parity, int nstopb)
 {
/* Set up the control structure */
 struct termios toptions;
 
 /* Get currently set options for the tty */
 tcgetattr(fd, &toptions);
 
/* Set custom options */
 
/* 9600 baud */
 cfsetispeed(&toptions, B115200);
 cfsetospeed(&toptions, B115200);
 /* 8 bits, no parity, 1 stop bit */
 toptions.c_cflag &= ~PARENB;
 toptions.c_cflag &= ~CSTOPB;
 toptions.c_cflag &= ~CSIZE;
 toptions.c_cflag |= CS8;
 /* no hardware flow control */
 toptions.c_cflag &= ~CRTSCTS;
 /* enable receiver, ignore status lines */
 toptions.c_cflag |= CREAD | CLOCAL;
 /* disable input/output flow control, disable restart chars */
 toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
 /* disable canonical input, disable echo,
 *  disable visually erase chars,
 *   disable terminal-generated signals */
 toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 /* disable output processing */
 toptions.c_oflag &= ~OPOST;
 
/* wait for 18 characters to come in before read returns */
/* WARNING! THIS CAUSES THE read() TO BLOCK UNTIL ALL */
/* CHARACTERS HAVE COME IN! */
 toptions.c_cc[VMIN] = 19;
 /* no minimum time to wait before read returns */
 toptions.c_cc[VTIME] = 0;
 

/* commit the options */
 tcsetattr(fd, TCSANOW, &toptions);
 
/* Wait for the Arduino to reset */
 usleep(1000*1000);
 /* Flush anything already in the serial buffer */
 tcflush(fd, TCIFLUSH);

 usleep(1000*1000);
 }

int m_fd(0);
FILE* m_fp(0);

void openTTY()
 {
/* Open the file descriptor in non-blocking mode */
  int fd = open ("/dev/ttyUSB6", O_RDWR | O_NOCTTY );
  if (fd < 0)
   {
    printf ("error %d opening /dev/ttyUSB6: %s", errno, strerror (errno));
    throw fd;
   }
  m_fd = fd;
  m_fp = fdopen(fd, "r");
  set_interface_attribs (fd, B115200, CS8, 'N', 1);    // 9600 baud, 8-bit chars, NO parity, 1 stop bit
  usleep(10000);
  char c(-1);
  write (m_fd, &c, 1);
  usleep(10000);
 }

void closeTTY()
 {
  if (m_fp) fclose(m_fp);
  if (m_fd) close(m_fd);
 }
 
int ndigital = 4;
int dchan[4] = { 0, 1, 2, 3 };

enum class arduinoDIO { INPUT=1, OUTPUT=0 };

arduinoDIO dway[4] = { arduinoDIO::OUTPUT, arduinoDIO::OUTPUT,
                  arduinoDIO::INPUT, arduinoDIO::INPUT };

int writeLine( char* line, size_t sz )
 {
  return write (m_fd, line, sz);
 }

int main (int argc, char** argv)
 {

  openTTY();

  usleep(10000);
  char message[64];
  message[0] = 'a';
  message[1] = 0;
  message[2] = 0;
  message[3] = 1;
  message[4] = 0;
  message[5] = 2;
  message[6] = 0;
  message[7] = 3;
  message[8] = 0;
  message[9] = 4;
  message[10] = 0;
  message[11] = 5;
  message[12] = 0;
  message[13] = 6;
  message[14] = 0;
  message[15] = 7;
  message[16] = 0;
  message[17] = 8;
  message[18] = 1;
  message[19] = 9;
  message[20] = 1;
  message[21] = 10;
  message[22] = 1;
  message[23] = 11;
  message[24] = 1;
  message[25] = 12;
  message[26] = 1;
  message[27] = 13;
  message[28] = 1;
  message[29] = 14;
  message[30] = 1;
  message[31] = 15;
  message[32] = 1;
  message[33] = 255;
  message[34] = '\0';
  // sprintf(message, "edcba\r\n");
  writeLine( message, 34);
  usleep(100000);
if(0) {
  // std::cout << " arduino read " << std::endl;
  char buf[10];
  int n = read (m_fd, buf, 1);  // read up to 100 characters if ready to read
  // std::cout << n << " arduino sent " << int(buf[n-1]) << " bytes" << std::endl;
  while (buf[n-1] != 10)
   {
    int m = read (m_fd, buf+n, 1);
    n += m;
  // std::cout << n << " arduino sent " << int(buf[n-1]) << " bytes" << std::endl;
   }
  buf[n-1] = '\0';
  if (buf[n-2] == 13) buf[n-2] = '\0';
  if (buf[0] == 'Z') buf[0] = ' ';
  std::cout << " arduino got: " << atoi(buf) << std::endl;
 }

  closeTTY();
  return 0;
 }
