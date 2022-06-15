#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

#include "linux_parser.h"
#include "processor.h"
#include <boost/algorithm/string.hpp>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{   string line;
    vector<string> token;
    vector<float> mem_use;
    string delim = " ";
    std::ifstream mem_file;
    string filename = "/proc/meminfo";
    mem_file.open(filename);

	for (int i = 0; i<4; i++)
    {
      getline(mem_file, line);	
      int pos = 0;

      while ((pos = (int)line.find(delim)) != (int)string::npos) 
      {
        token.push_back(line.substr(0, pos));    
        line.erase(0, pos + delim.length());    
      }
      mem_use.push_back(stof(token.back())); // store memory use data for each type of mem
	}
    mem_file.close();
    return (mem_use[0]-mem_use[1])/mem_use[0]; 
}

// TODO: Read and return the system uptime
int LinuxParser::UpTime() 
{
    string line;
    vector<string> token;
    string delim = " ";
    std::ifstream uptime_file;
    //open the processor status file
    uptime_file.open("/proc/uptime");
    getline(uptime_file, line);	//capture first line only

    boost::algorithm::split(token, line, boost::is_any_of(" "));
    uptime_file.close();
    return stoi(token.front())+stoi(token.back()); 
}
int LinuxParser::ProcUpTime(int pid)
  {
    string line;
    vector<string> token;
    string delim = " ";
    std::ifstream uptime_file;
    string proc_stat = "/proc/"+to_string(pid)+"/stat";
  
    //open the processor status file
    uptime_file.open(proc_stat);
    getline(uptime_file, line);	//capture first line only
    boost::algorithm::split(token, line, boost::is_any_of(" "));
    uptime_file.close();           
    return stoi(token[21]);  
    
    
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid)
{
    string line;
    vector<string> token;
    string delim = " ";
    std::ifstream proc_file;

    //open the processor status file
    proc_file.open("/proc/"+to_string(pid)+"/stat");
    getline(proc_file, line);	//capture first line only
    int pos = 0;

    while ((pos = (int)line.find(delim)) != (int)string::npos)
    {
      token.push_back(line.substr(0, pos));    
      line.erase(0, pos + delim.length());    
    }

    token.erase(token.begin(),token.begin()+13);
    token.erase(token.end()-29,token.end());
    proc_file.close();
    return token; //test
  }


// TODO: Read and return CPU utilization
vector<string> LinuxParser::Utilization() 
{
  string line;
  vector<string> token;
  string delim = " ";
  std::ifstream proc_file;
  
  //open the processor status file
  proc_file.open("/proc/stat");
  if(!proc_file.is_open()){std::cout<<"Unable to open /proc/stat"<<std::endl;}
  
  
  getline(proc_file, line);
  int pos = 0;
 
  while ((pos = (int)line.find(delim)) != (int)string::npos) {
    token.push_back(line.substr(0, pos));    
    line.erase(0, pos + delim.length());    
  }
  
  token.erase(token.begin()); //remove the 'cpu' field
  token.erase(token.begin()); //remove the blank line field
  token.pop_back();
  proc_file.close();
  return token; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  string line;
  vector<string> token;
  string delim = " ";
  string target = "processes";
  std::ifstream proc_file;
  
  
  string file_name = "/proc/stat";
  proc_file.open(file_name);
  int pos =0;
  
  while (!proc_file.eof())
  {    
    getline(proc_file, line);
    
    if (line.find(target) != string::npos)
    {
       while ((pos = (int)line.find(delim)) != (int)string::npos) 
          {token.push_back(line.replace(line.begin(),line.begin()+10, ""));}
      
    }
  }
 proc_file.close();
 return stoi(token.front()); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{  string line;
  vector<string> token;
  string delim = " ";
  string target = "procs_running";
  std::ifstream proc_file;
  
  
  string file_name = "/proc/stat";
  proc_file.open(file_name);
  int pos =0;
  
  while (!proc_file.eof())
  {    
    getline(proc_file, line);
    
    if (line.find(target) != string::npos)
    {
       
       while ((pos = (int)line.find(delim)) != (int)string::npos) 
          {token.push_back(line.replace(line.begin(),line.begin()+14, ""));}
      
    }
  }
 proc_file.close();
 return stoi(token.front());
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  string line,file_name;
  std::ifstream cmdline_file;
  
  //open the processor status file
  file_name = "/proc/" + to_string(pid) + "/cmdline";
  cmdline_file.open(file_name);
  if(!cmdline_file.is_open()){std::cout<<"Unable to open /proc/stat"<<std::endl;}  
  
  getline(cmdline_file, line);
  cmdline_file.close();
  return line; 
  
  
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  string line;
  char buffer[10];
  vector<string> token;
  string delim = " ";
  string target = "VmRSS:";	//Replaced "VmSize" iaw reviewer suggestions to obtain more accurate RAM usage
  std::ifstream stat_file;
  
  int pos =0;
  string file_name = "/proc/" + to_string(pid) + "/status";
  stat_file.open(file_name);
  if(!stat_file){std::cout<<"Status file not found."<<std::endl;}
  
  while (!stat_file.eof())
  {
    getline(stat_file, line);
    
    if (line.find(target) != string::npos)
    {
  
     while ((pos = (int)line.find(delim)) != (int)string::npos) 
        {
          token.push_back(line.substr(0, pos)); 
          line.erase(0, pos + delim.length());   
        }
      break;
    }
  }
  //Convert the Ram values into a string with two decimal places for output
 std::sprintf(buffer, "%.2f", stof(token.back())/1000.0);
 string output(buffer);
  stat_file.close();
 return output;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{

  string line;
  vector<string> token;
  string delim = "\t";
  string target = "Uid";
  std::ifstream stat_file;
  int pos =0;
  string file_name = "/proc/" + to_string(pid) + "/status";
  stat_file.open(file_name);
  if(!stat_file){std::cout<<"Status file not found."<<std::endl;}
  while (!stat_file.eof())
  {
    getline(stat_file, line);
    
    if (line.find(target) != string::npos)
    {
  
     while ((pos = (int)line.find(delim)) != (int)string::npos) 
        {
          token.push_back(line.substr(0, pos)); 
          line.erase(0, pos + delim.length());   
        }
      break;
    }
  
  }
  
 stat_file.close();
  return token[1]; //uid 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  string username;
  string uid = Uid(pid);
  string line;
  vector<string> token;
  string delim = ":";
  string target = "x:"+uid+":";
  std::ifstream etc_file;
  int pos =0;
  string file_name = "/etc/passwd";
  etc_file.open(file_name);
  if(!etc_file){std::cout<<"Status file not found."<<std::endl;}
  while (!etc_file.eof())
  {
    getline(etc_file, line);
    
    if (line.find(target) != string::npos)
    {
  
     while ((pos = (int)line.find(delim)) != (int)string::npos) 
        {
          token.push_back(line.substr(0, pos)); 
          line.erase(0, pos + delim.length());   
        }
      break;
    }
  
  }
 etc_file.close();
  return token[0]; //user 
 
}