#include "process.h"
#include "config.h"
#include <stdlib.h>

PCB* create_process(int pid, int ppid, int arrival_time) {
    PCB* p = malloc(sizeof(PCB));

    p->pid = pid;
    p->ppid = ppid;
    p->priority = 0; // Every new process enters at high priority
    p->status = READY;

    p->service_time = SERVICE_MIN + rand() % (SERVICE_MAX - SERVICE_MIN + 1);
    p->time_executed = 0;

    p->arrival_time = arrival_time;
    p->arrived = 0; // Not yet entered the system

    // Decide whether this process will perform I/O
    if (rand() % 100 < IO_CHANCE) {
        // Randomly pick the I/O type (1 = disk, 2 = tape, 3 = printer)
        p->io_type = (IOType)(1 + rand() % 3);
        // Define after how many CPU ticks it requests I/O
        p->io_instant = 1 + rand() % p->service_time;
    } else {
        p->io_type = NO_IO;
        p->io_instant = -1;        // Never requests I/O
    }

    p->remaining_io_time = 0;

    return p;
}

const char* io_name(IOType type) {
    switch (type) {
        case DISK:      return "Disk";
        case TAPE:       return "Tape";
        case PRINTER: return "Printer";
        default:         return "None";
    }
}