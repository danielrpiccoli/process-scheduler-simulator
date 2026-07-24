#ifndef PCB_H
#define PCB_H

// Possible states of a process
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} Status;

// I/O types
typedef enum {
    NO_IO,
    DISK,
    TAPE,
    PRINTER
} IOType;

typedef struct {
    int pid;
    int ppid;
    int priority; // 0 = high, 1 = low
    Status status;

    int service_time;   // Total CPU time the process needs
    int time_executed;  // How much CPU it has already consumed

    int arrival_time;  // Tick at which the process enters the system
    int arrived;       // 0 = not yet entered, 1 = already created

    IOType io_type;          // Type of I/O it will request
    int io_instant;          // After how many CPU ticks it requests I/O
    int remaining_io_time;   // Countdown while blocked
} PCB;

#endif