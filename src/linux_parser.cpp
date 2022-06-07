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
    vector<float> memuse;
    string delim = " ";
    std::ifstream memfile;
    string filename = "/proc/meminfo";
    memfile.open(filename);

	for (int i = 0; i<4; i++)
    {
      getline(memfile, line);	
      int pos = 0;

      while ((pos = (int)line.find(delim)) != (int)string::npos) 
      {
        token.push_back(line.substr(0, pos));    
        line.erase(0, pos + delim.length());    
      }
      memuse.push_back(stof(token.back())); // store memory use data for each type of mem
	}
    
    return (memuse[0]-memuse[1])/memuse[0]; 
}

// TODO: Read and return the system uptime
int LinuxParser::UpTime() 
{
    string line;
    vector<string> token;
    string delim = " ";
    std::ifstream uptimefile;
    //open the processor status file
    uptimefile.open("/proc/uptime");
    getline(uptimefile, line);	//capture first line only

    boost::algorithm::split(token, line, boost::is_any_of(" "));
    return stoi(token.front())+stoi(token.back()); 
}
int LinuxParser::ProcUpTime(int pid)
  {
    string line;
    vector<string> token;
    string delim = " ";
    std::ifstream uptimefile;
    string procstat = "/proc/"+to_string(pid)+"/stat";
  
    //open the processor status file
    uptimefile.open(procstat);
    getline(uptimefile, line);	//capture first line only
    boost::algorithm::split(token, line, boost::is_any_of(" "));
                 
    return stoi(token[21]);  
    
    
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid)
{
    string line;
    vector<string> token;
    string delim = " ";
    std::ifstream procfile;

    //open the processor status file
    procfile.open("/proc/"+to_string(pid)+"/stat");
    getline(procfile, line);	//capture first line only
    int pos = 0;

    while ((pos = (int)line.find(delim)) != (int)string::npos)
    {
      token.push_back(line.substr(0, pos));    
      line.erase(0, pos + delim.length());    
    }

    token.erase(token.begin(),token.begin()+13);
    token.erase(token.end()-29,token.end());
    return token; //test
  }


// TODO: Read and return CPU utilization
vector<string> LinuxParser::Utilization() 
{
  string line;
  vector<string> token;
  string delim = " ";
  std::ifstream procfile;
  
  //open the processor status file
  procfile.open("/proc/stat");
  if(!procfile.is_open()){std::cout<<"Unable to open /proc/stat"<<std::endl;}
  
  
  getline(procfile, line);
  int pos = 0;
 
  while ((pos = (int)line.find(delim)) != (int)string::npos) {
    token.push_back(line.substr(0, pos));    
    line.erase(0, pos + delim.length());    
  }
  
  token.erase(token.begin()); //remove the 'cpu' field
  token.erase(token.begin()); //remove the blank line field
  token.pop_back();

  return token; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  string line;
  vector<string> token;
  string delim = " ";
  string target = "processes";
  std::ifstream procfile;
  
  
  string filename = "/proc/stat";
  procfile.open(filename);
  int pos =0;
  
  while (!procfile.eof())
  {    
    getline(procfile, line);
    
    if (line.find(target) != string::npos)
    {
       while ((pos = (int)line.find(delim)) != (int)string::npos) 
          {token.push_back(line.replace(line.begin(),line.begin()+10, ""));}
      
    }
  }
 return stoi(token.front()); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{  string line;
  vector<string> token;
  string delim = " ";
  string target = "procs_running";
  std::ifstream procfile;
  
  
  string filename = "/proc/stat";
  procfile.open(filename);
  int pos =0;
  
  while (!procfile.eof())
  {    
    getline(procfile, line);
    
    if (line.find(target) != string::npos)
    {
       
       while ((pos = (int)line.find(delim)) != (int)string::npos) 
          {token.push_back(line.replace(line.begin(),line.begin()+14, ""));}
      
    }
  }
 return stoi(token.front());
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  string line,filename;
  std::ifstream cmdlinefile;
  
  //open the processor status file
  filename = "/proc/" + to_string(pid) + "/cmdline";
  cmdlinefile.open(filename);
  if(!cmdlinefile.is_open()){std::cout<<"Unable to open /proc/stat"<<std::endl;}  
  
  getline(cmdlinefile, line);
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
  string target = "VmSize:";
  std::ifstream statfile;
  
  int pos =0;
  string filename = "/proc/" + to_string(pid) + "/status";
  statfile.open(filename);
  if(!statfile){std::cout<<"Status file not found."<<std::endl;}
  
  while (!statfile.eof())
  {
    getline(statfile, line);
    
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
  std::ifstream statfile;
  int pos =0;
  string filename = "/proc/" + to_string(pid) + "/status";
  statfile.open(filename);
  if(!statfile){std::cout<<"Status file not found."<<std::endl;}
  while (!statfile.eof())
  {
    getline(statfile, line);
    
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
  std::ifstream etcfile;
  int pos =0;
  string filename = "/etc/passwd";
  etcfile.open(filename);
  if(!etcfile){std::cout<<"Status file not found."<<std::endl;}
  while (!etcfile.eof())
  {
    getline(etcfile, line);
    
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
 
  return token[0]; //user 
 
}