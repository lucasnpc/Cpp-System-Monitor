#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
    long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1, guest1, guestNice1;
    long user2, nice2, system2, idle2, iowait2, irq2, softirq2, steal2, guest2, guestNice2;
    long total1, total2;
public:
    float Utilization();  // TODO: See src/processor.cpp
};

#endif