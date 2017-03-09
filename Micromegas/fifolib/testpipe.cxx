
#include <string>
#include "my_pipe.h"

std::string pipe_name("/tmp/atlas/testAApipe");

volatile char buffer[1024] = {'\0'};
char cmd[1024] = {0};
my_pipe p(pipe_name.c_str(), cmd);

int main( int argc, char** argv )
 {
  return 0;
 }
