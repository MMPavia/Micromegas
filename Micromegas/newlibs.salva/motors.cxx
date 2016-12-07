
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "motors.h"

void motors::mot_write ( std::string s )
 {
	const char* cp = s.c_str();
	size_t sz = strlen(cp);
	write (m_fd, cp, sz);
	usleep(400000);
 }

void motors::mot_read ()
 {
	int n = read (m_fd, m_buf, 65535);
	m_buf[n] = '\0';
	m_rdbuf.push_back( std::string(m_buf) );
 }

void motors::mot_dump ()
 {
	const char* pt;
	size_t n;
	while (m_rdbuf.size() > 0)
	 {
	  pt = (*m_rdbuf.begin()).c_str();
	  n = strlen(pt);
	  write(1, pt, n);
	  write(1, "\n", 1);
	  m_rdbuf.pop_front();
	 }
 }

void motors::mot_reset ( uint16_t prog )
 {
	for (uint64_t i=1; i<7; ++i)
	 {
	  std::stringstream zcmd;
	  zcmd << "m" << std::setfill('0') << std::setw(3) << prog << '-'
	       << std::setw(10) << i*100000*100*100 << "+0\r" << std::endl;
	  this->mot_write(zcmd.str());
	  this->mot_read();
	 }
	std::stringstream rcmd;
	rcmd << "r" << std::setfill('0') << std::setw(3) << prog << '\r' << std::endl;
	this->mot_write(rcmd.str());
	this->mot_read();
 }
