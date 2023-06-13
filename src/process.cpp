#include "process.h"
#include "linux_parser.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid; }

float Process::CpuUtilization() {
  long uptime = LinuxParser::UpTime();
  long total_time = LinuxParser::ActiveJiffies(pid);
  long seconds = uptime - (LinuxParser::UpTime(pid) / sysconf(_SC_CLK_TCK));
  utilization = (float)total_time / (float)sysconf(_SC_CLK_TCK) / (float)seconds;
  return utilization;
}

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& a) const {
  return utilization > a.utilization;
}