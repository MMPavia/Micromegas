#include <csignal>
#include <iostream>

#include "mutex_lock.h"

static boost::interprocess::named_mutex
 fp_mutex(boost::interprocess::open_or_create, "fp_named_mutex");

// Exit properly on signal SIGINT
void sighandler(int sig)
 {
  std::cerr << "- caught signal " << sig << std::endl;
  mutex_lock::remove_lock();
  exit(1);
 }

class mySigH
 {
  public:
   mySigH(int sig)
    {
     signal(sig,sighandler);
     std::cerr << "- sighandler installed for signal " << sig << std::endl;
    }
 };
static mySigH mmm(SIGINT);

bool mutex_lock::is_mine(false);

void mutex_lock::remove_lock(void)
 {
  if (is_mine) fp_mutex.unlock();
  is_mine = false;
 }

mutex_lock::mutex_lock() : lock(fp_mutex)
 {
  is_mine = true;
 }

mutex_lock::~mutex_lock()
 {
  is_mine = false;
 }

