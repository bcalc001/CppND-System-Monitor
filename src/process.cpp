#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() 
{ return this->pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
  
  vector<string> token = LinuxParser::CpuUtilization(Pid());
  
   int Total, utime, stime, cutime, cstime, starttime, seconds;
   float uptime, cpu_usage;
      
    starttime = std::stoi(token.back()); 
    uptime = LinuxParser::UpTime();
    utime = std::stoi(token[0]);
    stime = std::stoi(token[1]);
    cutime = std::stoi(token[2]);
    cstime = std::stoi(token[3]);
    Total = utime + stime + cutime + cstime;
    seconds = uptime - starttime/sysconf(_SC_CLK_TCK);
    cpu_usage = 100.00 * (Total/sysconf(_SC_CLK_TCK)/seconds);

    return cpu_usage;
 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram(){ return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() 
{
  
  this->username = LinuxParser::User(Pid());
  return this->username; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::ProcUpTime(Pid())/sysconf(_SC_CLK_TCK); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
//bool Process::operator < (Process const& a) const { return true; }  //Param: Process const& a