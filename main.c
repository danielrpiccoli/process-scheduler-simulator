#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "process.h"
#include "scheduler.h"

int main() {
    srand(time(NULL));

    int n = MAX_PROCESSES;
    PCB* all[MAX_PROCESSES];

    // Creates the processes with arrival times spread out over time
    int arrival = 0;
    for (int i = 0; i < n; i++) {
        all[i] = create_process(i + 1, 1, arrival);
        arrival += rand() % 4;   // Next one arrives 0 to 3 ticks later
    }

    // Shows the process table before simulating
    printf("===== PROCESSES CREATED =====\n");
    for (int i = 0; i < n; i++) {
        printf("PID %d | arrival=%d | service=%d | io=%s | requests_io_after=%d\n",
               all[i]->pid, all[i]->arrival_time,
               all[i]->service_time, io_name(all[i]->io_type),
               all[i]->io_instant);
    }

    // Initializes and runs once
    Scheduler s;
    scheduler_init(&s, n);
    simulate(&s, all, n);

    // Frees memory
    for (int i = 0; i < n; i++) free(all[i]);

    return 0;
}