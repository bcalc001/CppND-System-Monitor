#include <string>
#include <math.h>
#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) 
{ 
  
  int hh,mm,ss,remainder;
  std::string time;
  hh = floor(seconds/3600);
  remainder = seconds - hh*3600;
  mm = floor(remainder/60);
  remainder -= mm*60;
  ss = remainder;
 
  time = std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);
  
 
  return time; }