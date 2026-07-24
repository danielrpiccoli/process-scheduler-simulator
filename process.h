#ifndef PROCESS_H
#define PROCESS_H

#include "pcb.h"

// Creates and fills a PCB with (partially randomized) values
PCB* create_process(int pid, int ppid, int arrival_time);

// Human-readable name of the I/O type (for the logs)
const char* io_name(IOType type);

#endif