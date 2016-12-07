#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

#include "bpipe.h"

static FILE* m_fp(NULL);
static int m_fd(-1);

inline bool f_exists( const char* name )
 {
  struct stat buffer;   
  return (stat (name, &buffer) == 0); 
 }

void* from_bpipe( void* argv )
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
  printf ("start reading bpipe %s\n", name);
  m_fd = open(name, O_RDONLY | O_NOCTTY | O_SYNC);
  while(1)
   {
    while (*cmd != '\0') usleep(1000);
    int n = read (m_fd, cmd+1, 256);
    if (n>0) *cmd = n;
   }

  close(m_fd);
  m_fd = -1;
  return NULL;
 }

bpipe::bpipe( const char* name, char* cmd )
 {
  m_pname = name;
  void* argv[2];
  argv[0] = const_cast<char*>(name);
  argv[1] = cmd;
  int rc = pthread_create(&m_tid, NULL, from_bpipe, argv);
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

bpipe::bpipe( const char* name ) : m_tid(0)
 {
  m_pname = name;
  printf("bpipe %s client\n", name);
 }

bpipe::~bpipe()
 {
  if (m_tid != 0)
   {
    int rc = pthread_cancel(m_tid);
    if (rc != 0) printf ("pthread_cancel error %d\n", rc);
    if (m_fp != NULL) fclose(m_fp);
    printf("thread %ld closed\n", m_tid);
   }
 }

void bpipe::to_bpipe( const void* buf, size_t count )
 {
  while (!f_exists(m_pname.c_str()))
   {
    usleep(100);
   }
  int fd;
  if((fd = open(m_pname.c_str(), O_WRONLY | O_NOCTTY | O_SYNC)) == -1)
   {
    perror("fopen");
    exit(1);
   }
  write(fd, buf, count);
  close(fd);
 }

