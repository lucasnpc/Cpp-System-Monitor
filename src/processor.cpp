#include "processor.h"
#include <chrono>
#include <thread>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    prevIdle = LinuxParser::IdleJiffies();
    prevActive = LinuxParser::ActiveJiffies();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    currIdle = LinuxParser::IdleJiffies();
    currActive = LinuxParser::ActiveJiffies();

    total = currActive + currIdle - prevActive - prevIdle;
    active = currActive - prevActive;

    return static_cast<float>(active) / static_cast<float>(total);
}