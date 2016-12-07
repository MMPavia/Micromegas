#include <strings.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

#include "my_pipe.h"

char cmd[1024] = {0};
my_pipe p(cmd);

int main( int argc, char** argv )
 {
  while(1)
   {
    if (*cmd != '\0')
     {
      printf("found string %s\n", cmd);
      if (strcasecmp("quit", cmd) == 0) break;
      if (strcasecmp("kill", cmd) == 0) exit(1);
      if (strcasecmp("pause", cmd) == 0)
       {
        *cmd = '\0';
        do 
         { 
          usleep(100);
         } 
        while (strcasecmp("resume", cmd) != 0);
       }
      *cmd = '\0';
     }
    usleep(100);
   }
  return 0;
 }
