
#include <unistd.h>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "chauvinarnoux.h"

void chauvinarnoux::loop()
 {
  char buf[10000];
  int j(0);
  while (1)
   {
    usleep(1000);
/*
    int n = read (m_fd, buf, 10000);  // read up to 10000 characters if ready to read
    if (n)
     {
      for (int i=0; i<n; i++)
       {
        std::cout << buf[i];
       }
     }
 */
    char* p = fgets(buf, 10000, m_fp);
    if (p != NULL)
     {
      int l = strlen(buf);
      buf[l-2] = '\0';
      char* yes = strstr(buf, "Insulation Resistance:");
      if (yes != NULL) std::cout << "got: " << l << " bytes: [" << buf << ']' << std::endl;
     }
   }
 }
