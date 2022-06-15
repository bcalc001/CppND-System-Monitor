#include <string>
#include <math.h>
#include "format.h"
#include <sstream>
#include <iomanip>
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
std::ostringstream elapsed_time;
  elapsed_time << std::setw(2) << std::setfill('0') << std::to_string(hh) << ":" 
     << std::setw(2) << std::setfill('0') << std::to_string(mm) << ":"
     << std::setw(2) << std::setfill('0') << std::to_string(ss);
  return elapsed_time.str();  //time = std::setw(2)std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);
  
 
//  return time; 
}