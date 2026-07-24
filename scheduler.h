#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"

// Groups the whole system state into a struct
typedef struct {
    Queue high; // Ready queue - high priority
    Queue low;  // Ready queue - low priority
    Queue io_disk;    // Blocked, waiting for disk
    Queue io_tape;    // Blocked, waiting for tape
    Queue io_printer; // Blocked, waiting for printer

    int clock; // Current tick
    int finished_processes;
    int total_processes;
} Scheduler;

void scheduler_init(Scheduler* s, int total_processes);
void simulate(Scheduler* s, PCB* all[], int n);

#endif