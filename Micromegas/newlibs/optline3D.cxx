#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>

#include "optline3D.h"

static char abuf[256];
/*
char* optline::readlineXYZ()
 {
  write (m_fd, "A\r\n", 3);       // send 3 character greeting
  usleep(10000);
  int n = read (m_fd, abuf, 256);  // read up to 100 characters if ready to read
  while (abuf[n-1] != 10)
   {
    int m = read (m_fd, abuf+n, 256);
    n += m;
   }
  abuf[n-1] = '\0';
  if (abuf[n-2] == 13) abuf[n-2] = '\0';
  if (abuf[0] == 'A') abuf[0] = ' ';
  return abuf;
 }
*/

std::vector<float>  optline::readlineXYZ()
 {
  write (m_fd, "A\r\n", 3);       // send 3 character greeting
  usleep(10000);
  int n = read (m_fd, abuf, 256);  // read up to 100 characters if ready to read
  while (abuf[n-1] != 10)
   {
    int m = read (m_fd, abuf+n, 256);
    n += m;
   }
  abuf[n-1] = '\0';
  if (abuf[n-2] == 13) abuf[n-2] = '\0';
  if (abuf[0] == 'A') abuf[0] = ' ';
  std::string s_abuf(abuf); 
  std::vector<float> mycoord; 
  std::string::size_type pos;
  pos = s_abuf.find("X");
  float myc = stof(s_abuf.substr(pos+2,9));
  mycoord.push_back(myc);
  pos = s_abuf.find("Y");
  myc = stof(s_abuf.substr(pos+2,9));
  mycoord.push_back(myc);
  pos = s_abuf.find("Z");
  myc = stof(s_abuf.substr(pos+2,9));
  mycoord.push_back(myc);
  return mycoord;
 }


double optline::readlineX()
 {
  write (m_fd, "X\r\n", 3);       // send 3 character greeting
  usleep(10000);
  char buf[100];
  int n = read (m_fd, buf, 100);  // read up to 100 characters if ready to read
  while (buf[n-1] != 10)
   {
    int m = read (m_fd, buf+n, 100);
    n += m;
   }
  buf[n-1] = '\0';
  if (buf[n-2] == 13) buf[n-2] = '\0';
  if (buf[0] == 'X') buf[0] = ' ';
  return strtod( buf, NULL);
 }

double optline::readlineY()
 {
  write (m_fd, "Y\r\n", 3);       // send 3 character greeting
  usleep(10000);
  char buf[100];
  int n = read (m_fd, buf, 100);  // read up to 100 characters if ready to read
  while (buf[n-1] != 10)
   {
    int m = read (m_fd, buf+n, 100);
    n += m;
   }
  buf[n-1] = '\0';
  if (buf[n-2] == 13) buf[n-2] = '\0';
  if (buf[0] == 'Y') buf[0] = ' ';
  return strtod( buf, NULL);
 }
double optline::readlineZ()
 {
  write (m_fd, "Z\r\n", 3);       // send 3 character greeting
  usleep(10000);
  char buf[100];
  int n = read (m_fd, buf, 100);  // read up to 100 characters if ready to read
  while (buf[n-1] != 10)
   {
    int m = read (m_fd, buf+n, 100);
    n += m;
   }
  buf[n-1] = '\0';
  if (buf[n-2] == 13) buf[n-2] = '\0';
  if (buf[0] == 'Z') buf[0] = ' ';
  return strtod( buf, NULL);
 }
