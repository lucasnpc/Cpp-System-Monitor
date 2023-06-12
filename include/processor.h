#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
    long prevIdle, prevActive, currIdle, currActive, total, active;
public:
    float Utilization();  // TODO: See src/processor.cpp
};

#endif