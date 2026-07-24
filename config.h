#ifndef CONFIG_H
#define CONFIG_H

// Simulator assumptions

#define MAX_PROCESSES   5   // Maximum number of processes created
#define QUANTUM         3   // Time slice, in ticks

// Range of service (CPU) time for each process
#define SERVICE_MIN     2
#define SERVICE_MAX     10

// Duration of each I/O type, in ticks
#define DISK_DURATION       4
#define TAPE_DURATION       3
#define PRINTER_DURATION    5

// Probability of a process requesting I/O
#define IO_CHANCE       60

#endif