
#include "mutex_lock.h"
#include "my_pipe.h"
#include "arduinoSerial.h"

int main( int argc, char** argv )
 {
  char cmd[1024] = {'\0'};
  arduinoSerial* myboard = arduinoSerial::create("/dev/ttyUSB0");
  my_pipe p_from(argv[1], cmd);
  my_pipe p_to(argv[2]);

  if (*cmd != '\0')
   {
    printf("found string %s\n", cmd);
    write(m_fd, cmd, strlen(cmd));
    *cmd = '\0';
   }

  return 0;
 }
