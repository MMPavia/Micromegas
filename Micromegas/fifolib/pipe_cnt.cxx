#include <cstdlib>
#include <cstdio>

#include "my_pipe.h"

int main(int argc, char *argv[])
{

 if ( argc != 3 )
  {
   printf("USAGE: %s <pipe name> <command>\n", argv[0]);
   exit(1);
  }
 my_pipe p(argv[1]);
 p.to_pipe(argv[2]);

 return 0;
}

