#include <strings.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

#include "my_pipe.h"

char cmd[1024] = {'\0'};

int main( int argc, char** argv )
 {
  if ( argc != 2 )
   {
    printf("USAGE: %s <pipe name>\n", argv[0]);
    exit(1);
   }

  my_pipe p(argv[1], cmd);
  printf("pipe %s \n", argv[1]);
  while(1)
   {
    if (*cmd != '\0')
     {
      printf("found string %s\n", cmd);
      if (strcasecmp("pause", cmd) == 0)
       {
        *cmd = '\0';
        do 
         { 
          usleep(100);
         } 
        while (!( strcasecmp("resume", cmd) == 0 ||
                  strcasecmp("quit", cmd) == 0 ||
                  strcasecmp("kill", cmd) == 0 ));
       }
      if (strcasecmp("quit", cmd) == 0) break;
      if (strcasecmp("kill", cmd) == 0) exit(1);
      *cmd = '\0';
     }
    usleep(1000);
   }
  return 0;
 }
