#include <csignal>
#include <iostream>

#include "mutex_lock.h"

volatile sig_atomic_t is_mine(0);

static boost::interprocess::named_mutex
 arduino_mutex(boost::interprocess::open_or_create, "arduino_named_mutex");

// Exit properly on signal SIGINT
void sighandler(int sig)
 {
  std::cerr << "- caught signal: " << sig << " - unlocking: " << is_mine << std::endl;
  if (is_mine) arduino_mutex.unlock();
  is_mine = 0;
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

mutex_lock::mutex_lock() : lock(arduino_mutex)
 {
  is_mine = 1;
 }

mutex_lock::~mutex_lock()
 {
  is_mine = 0;
 }

