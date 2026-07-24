#include "scheduler.h"
#include "config.h"
#include "process.h"
#include <stdio.h>

void scheduler_init(Scheduler* s, int total_processes) {
    queue_init(&s->high);
    queue_init(&s->low);
    queue_init(&s->io_disk);
    queue_init(&s->io_tape);
    queue_init(&s->io_printer);
    s->clock = 0;
    s->finished_processes = 0;
    s->total_processes = total_processes;
}

// Helper functions (static = visible only in this file)

// Places a process in the correct I/O queue and starts its duration counter
static void send_to_io(Scheduler* s, PCB* p) {
    p->status = BLOCKED;
    switch (p->io_type) {
        case DISK:
            p->remaining_io_time = DISK_DURATION;
            enqueue(&s->io_disk, p);
            break;
        case TAPE:
            p->remaining_io_time = TAPE_DURATION;
            enqueue(&s->io_tape, p);
            break;
        case PRINTER:
            p->remaining_io_time = PRINTER_DURATION;
            enqueue(&s->io_printer, p);
            break;
        default:
            break;
    }
    printf("[clock %2d] PID %d -> I/O (%s)\n",
           s->clock, p->pid, io_name(p->io_type));
}

/* Decrements the I/O time for a queue. Whoever finishes goes back to a ready queue.
   to_high = 1 -> goes back to HIGH
   to_high = 0 -> goes back to LOW */
static void update_io_queue(Scheduler* s, Queue* io_queue, int to_high) {
    int n = io_queue->size;
    for (int i = 0; i < n; i++) {
        PCB* p = dequeue(io_queue);
        p->remaining_io_time--;

        if (p->remaining_io_time <= 0) {
            // I/O completed. This process won't request I/O again
            p->io_type = NO_IO;
            p->io_instant = -1;
            p->status = READY;

            if (to_high) {
                enqueue(&s->high, p);
                printf("[clock %2d] PID %d <- I/O complete, goes to HIGH\n",
                       s->clock, p->pid);
            } else {
                enqueue(&s->low, p);
                printf("[clock %2d] PID %d <- I/O complete, goes to LOW\n",
                       s->clock, p->pid);
            }
        } else {
            // Not finished yet, put it back in the same queue
            enqueue(io_queue, p);
        }
    }
}

// Picks the next process to run. High has priority over low
static PCB* select_process(Scheduler* s) {
    if (!queue_empty(&s->high)) return dequeue(&s->high);
    if (!queue_empty(&s->low))  return dequeue(&s->low);
    return NULL;
}

void simulate(Scheduler* s, PCB* all[], int n) {
    printf("\n===== SIMULATION START =====\n\n");

    while (s->finished_processes < s->total_processes) {

        /* 1. Arrival: a process enters as soon as the clock reaches its arrival time.
           Uses the 'arrived' flag + '<=' to never skip a process whose
           arrival falls on a tick that the clock skipped because of the quantum. */
        for (int i = 0; i < n; i++) {
            if (!all[i]->arrived && all[i]->arrival_time <= s->clock) {
                all[i]->arrived = 1;
                enqueue(&s->high, all[i]);
                printf("[clock %2d] PID %d created -> HIGH queue\n",
                       s->clock, all[i]->pid);
            }
        }

        // 2. Update I/O (disk goes back to low. Tape and printer go back to high)
        update_io_queue(s, &s->io_disk, 0);    // 0 = low
        update_io_queue(s, &s->io_tape, 1);    // 1 = high
        update_io_queue(s, &s->io_printer, 1); // 1 = high

        // 3. Selects the process for the CPU
        PCB* current = select_process(s);

        if (current == NULL) {
            // CPU idle: no one ready, but someone is still in I/O
            s->clock++;
            continue;
        }

        // 4. Runs it respecting the quantum
        current->status = RUNNING;
        int quantum_used = 0;
        printf("[clock %2d] PID %d enters CPU (service %d/%d)\n",
               s->clock, current->pid, current->time_executed, current->service_time);

        while (quantum_used < QUANTUM) {
            current->time_executed++;
            quantum_used++;
            s->clock++;

            // 5a. Did it finish its service?
            if (current->time_executed >= current->service_time) {
                current->status = TERMINATED;
                s->finished_processes++;
                printf("[clock %2d] PID %d FINISHED\n", s->clock, current->pid);
                current = NULL;
                break;
            }

            // 5b. Time to request I/O?
            if (current->io_type != NO_IO &&
                current->time_executed == current->io_instant) {
                send_to_io(s, current);
                current = NULL;
                break;
            }
        }

        // 5c. Quantum ran out without finishing or requesting I/O -> Preemption
        if (current != NULL) {
            current->status = READY;
            enqueue(&s->low, current);
            printf("[clock %2d] PID %d was PREEMPTED -> LOW queue\n",
                   s->clock, current->pid);
        }
    }

    printf("\n===== SIMULATION END (final clock: %d) =====\n", s->clock);
}