#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

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
float LinuxParser::MemoryUtilization() {
  float memTotal, memFree, memAvailable, buffers, cached;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream.ignore(256, ':');
    linestream >> memTotal;
    std::getline(stream, line);
    linestream.str(line);
    linestream.ignore(256, ':');
    linestream >> memFree;
    std::getline(stream, line);
    linestream.str(line);
    linestream.ignore(256, ':');
    linestream >> memAvailable;
    std::getline(stream, line);
    linestream.str(line);
    linestream.ignore(256, ':');
    linestream >> buffers;
    std::getline(stream, line);
    linestream.str(line);
    linestream.ignore(256, ':');
    linestream >> cached;
  }
  return (memTotal - memFree - buffers - cached) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  double uptimeSec = 0.0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptimeSec;
  }
  return static_cast<long>(uptimeSec);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string cpu;
    linestream >> cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    return user + nice + system + idle + iowait + irq + softirq + steal;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]] ) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string token;
    for (int i = 0; i < 13; i++) {
      linestream >> token;
    }
    long utime, stime;
    linestream >> utime >> stime;
    return utime + stime;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string cpu;
    linestream >> cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    return user + nice + system + irq + softirq + steal;
  }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string cpu;
    linestream >> cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    return idle;
  }
  return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.compare(0, 3, "cpu") == 0) {
        cpu_utilization.push_back(line);
      }
    }
  }
  return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int numProcess = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    string line;
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      std::string field;
      int value;
      linestream >> field >> value;
      if (field == "processes") {
        numProcess = value;
        break;
      }
    }
  }
  return numProcess;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int processRunning = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    string line;
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      std::string field;
      int value;
      linestream >> field >> value;
      if (field == "procs_running") {
        processRunning = value;
        break;
      }
    }
  }
  return processRunning;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]] ) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]] ) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]] ) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]] ) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]] ) { return 0; }
