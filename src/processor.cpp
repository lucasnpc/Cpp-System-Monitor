#include "processor.h"
#include <sstream>
#include <chrono>
#include <thread>

#include <string>
#include "linux_parser.h"

using LinuxParser::kProcDirectory;
using LinuxParser::kStatFilename;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float utilization = 0.0;
    std::ifstream stream(kProcDirectory + kStatFilename);
    string line, cpu1, cpu2;
    if (stream.is_open())
    {
        {
            std::ifstream stream("/proc/stat");
            std::string line;
            std::getline(stream, line);
            std::istringstream linestream(line);
            linestream >> cpu1 >> user1 >> nice1 >> system1 >> idle1 >> iowait1 >> irq1 >> softirq1 >> steal1 >> guest1 >> guestNice1;

            std::this_thread::sleep_for(std::chrono::seconds(1));

            stream.seekg(0);
            std::getline(stream, line);
            std::istringstream linestream2(line);
            linestream2 >> cpu2 >> user2 >> nice2 >> system2 >> idle2 >> iowait2 >> irq2 >> softirq2 >> steal2 >> guest2 >> guestNice2;

            float prevIdle = idle1 + iowait1;
            float idle = idle2 + iowait2;

            float prevNonIdle = user1 + nice1 + system1 + irq1 + softirq1 + steal1;
            float nonIdle = user2 + nice2 + system2 + irq2 + softirq2 + steal2;

            float prevTotal = prevIdle + prevNonIdle;
            float total = idle + nonIdle;

            float totald = total - prevTotal;
            float idled = idle - prevIdle;

            utilization = static_cast<float>((totald - idled) / totald);
        }
    }
    return utilization * 100;
}