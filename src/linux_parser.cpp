#include "linux_parser.h"
#include "generic.h"

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
        if (key == prettyName) {
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

float LinuxParser::MemoryUtilization() {
  string path = kProcDirectory + kMeminfoFilename;
  float total = Util::findValueByKey<float>(memTotal, path);
  float free = Util::findValueByKey<float>(memFree, path);
  float buffer = Util::findValueByKey<float>(buffers, path);
  float cache = Util::findValueByKey<float>(cached, path);
  return (total - free - buffer - cache) / total;
}

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

long LinuxParser::ActiveJiffies(int pid) {
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

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.compare(0, 3, "cpu") == 0) {
        cpu_utilization.emplace_back(line);
      }
    }
  }
  return cpu_utilization;
}

int LinuxParser::TotalProcesses() {
  int numProcess = 0;
  string path = kProcDirectory + kStatFilename;
  numProcess = Util::findValueByKey<int>(processesFilter, path);
  return numProcess;
}

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

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
  }
  if (line.length() > 40)
    line = line.substr(0, 40) + "...";
  return line;
}

string LinuxParser::Ram(int pid) {
  string line, key, value;
  long ram = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == vmRSS) {
        ram = stol(value) / 1024;
        break;
      }
    }
  }
  return to_string(ram);
}

string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        break;
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string uid;
  string username;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      getline(linestream, username, ':');
      getline(linestream, key, ':');
      getline(linestream, uid, ':');
      if (uid == LinuxParser::Uid(pid)) {
        break;
      }
    }
  }
  return username;
}

long LinuxParser::UpTime(int pid) {
  string line;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    string ignore;
    for (int i = 0; i < 21; i++) {
      linestream >> ignore;
    }
    linestream >> uptime;
  }
  return LinuxParser::UpTime() - (uptime / sysconf(_SC_CLK_TCK));
}
