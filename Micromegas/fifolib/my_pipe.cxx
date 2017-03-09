#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <iostream>

#include "my_pipe.h"

static FILE* m_fp(NULL);

inline bool f_exists( const char* name )
 {
  struct stat buffer;   
  return (stat (name, &buffer) == 0); 
 }

void* from_pipe( void* argv )
 {
  int rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  if (rc != 0) printf ("pthread_setcancelstate error %d\n", rc);

  char* name = ((char**)argv)[0];
  char* cmd = ((char**)argv)[1];

  *cmd = '\0';
  while (!f_exists(name))
   {
    usleep(100);
   }
  std::cout << "start reading pipe " << name << std::endl;
  while(1)
   {
    m_fp = fopen(name, "r");
    fgets(cmd, 256, m_fp);
    fclose(m_fp);
    m_fp = NULL;
    std::cout << " got command " << cmd << std::endl;
    // if (strcasecmp("kill", cmd) == 0) exit(1); // if needed
   }

  return NULL;
 }

my_pipe::my_pipe( const char* name, char* cmd )
 {
  m_argv[0] = const_cast<char*>(name);
  m_argv[1] = const_cast<char*>(cmd);
  int rc = pthread_create(&m_tid, NULL, from_pipe, m_argv);
  if (rc != 0)
   {
    printf ("pthread_create error %d\n", rc);
    exit(1);
   }
  printf("thread %ld started\n", m_tid);

  /* Create the FIFO if it does not exist */
  if (!f_exists(name))
   {
    umask(0);
    mkfifo(name , 0666);
   }

 }

my_pipe::my_pipe( const char* name ) : m_tid(0)
 {
  m_pname = name;
  printf("my_pipe %s client\n", name);
 }

my_pipe::~my_pipe()
 {
  if (m_tid != 0)
   {
    int rc = pthread_cancel(m_tid);
    if (rc != 0) printf ("pthread_cancel error %d\n", rc);
    if (m_fp != NULL) fclose(m_fp);
    printf("thread %ld closed\n", m_tid);
   }
 }

void my_pipe::to_pipe( char* cmd )
 {
  printf("to_pipe %s\n", m_pname.c_str());
  while (!f_exists(m_pname.c_str()))
   {
    usleep(100);
   }
  FILE *fp;
  printf("to_pipe file %s exists\n", m_pname.c_str());
  if((fp = fopen(m_pname.c_str(), "w")) == NULL)
   {
    perror("fopen");
    exit(1);
   }
  printf("to_pipe writing command %s\n", cmd);
  fflush(stdout);
  fputs(cmd, fp);
  fclose(fp);
 }
