# Process Scheduler — Round Robin with Feedback

Process scheduling simulator written in **C**, implementing the **Round Robin
(Circular) with Feedback** selection strategy.

Final project for the course **ICP246 — Computer Architecture and Operating
Systems** (UFRJ — Instituto de Computação, 2026-1).

---

## Overview

The simulator models the short-term process management of an operating system.
At each clock *tick*, it creates processes, has them compete for the CPU while
respecting a time slice (quantum), applies preemption, blocks processes that
request I/O, and returns them to the ready queues according to the type of
device used.

The strategy reproduces, in simplified form, the scheduling of **traditional
UNIX**: Feedback with multiple priority queues and Round Robin internally within
each queue.

### Key features

- **PCB (Process Control Block)** with identification (PID, PPID), state, and
  scheduling attributes.
- **Five queues:** two ready queues (high and low priority) and three I/O
  queues (disk, tape, and printer).
- **Quantum-based preemption:** processes that exhaust their time slice without
  finishing are demoted to the low-priority queue.
- **Feedback by I/O type:**
  - Disk → returns to the **low**-priority queue
  - Magnetic tape → returns to the **high**-priority queue
  - Printer → returns to the **high**-priority queue
- **Random service and I/O times** for each process.
- **Detailed log** of every event (creation, entering the CPU, preemption,
  blocking, I/O return, and termination).

---

## Project structure

| File | Responsibility |
|---|---|
| `config.h` | Simulation assumptions and numeric parameters |
| `pcb.h` | PCB structure and state/I/O-type enumerations |
| `queue.h` / `queue.c` | Generic circular queue (FIFO) structure |
| `process.h` / `process.c` | Process creation with randomized attributes |
| `scheduler.h` / `scheduler.c` | Core: Round Robin with Feedback simulation loop |
| `main.c` | Process setup and simulation launch |

---

## How to build and run

The project depends only on a standard C compiler (GCC or Clang) and the
language's standard library. There are no external dependencies.

### Linux / macOS

```bash
gcc -Wall main.c queue.c process.c scheduler.c -o scheduler
./scheduler
```

With Clang, just swap `gcc` for `clang`.

### Windows

With **MinGW** (GCC for Windows):

```bash
gcc -Wall main.c queue.c process.c scheduler.c -o scheduler.exe
scheduler.exe
```

With the **Microsoft compiler (MSVC)**, in a *Developer Command Prompt*:

```bash
cl main.c queue.c process.c scheduler.c /Fe:scheduler.exe
scheduler.exe
```

> Service and I/O times are randomized on every run (the generator is seeded
> with `srand(time(NULL))`), so the output varies between executions.

---

## Adjusting the assumptions

All simulation parameters are centralized in `config.h` and can be changed
before compiling:

```c
#define MAX_PROCESSES   10   /* maximum number of processes created  */
#define QUANTUM         3    /* time slice, in ticks                 */

#define SERVICE_MIN     5    /* minimum service time                 */
#define SERVICE_MAX     20   /* maximum service time                 */

#define DISK_DURATION       4
#define TAPE_DURATION       3
#define PRINTER_DURATION    5

#define IO_CHANCE       60   /* probability (%) of requesting I/O    */
```

For example, increasing `QUANTUM` makes the behavior approach FCFS; decreasing
it increases the frequency of preemptions.

---

## Sample output

```
===== PROCESSES CREATED =====
PID 1 | arrival=0 | service=6 | io=Tape | requests_io_after=3
PID 2 | arrival=3 | service=19 | io=Tape | requests_io_after=17
...

===== SIMULATION START =====

[clock  0] PID 1 created -> HIGH queue
[clock  0] PID 1 enters CPU (service 0/6)
[clock  3] PID 1 -> I/O (Tape)
[clock  3] PID 2 enters CPU (service 0/19)
[clock  6] PID 2 was PREEMPTED -> LOW queue
...
[clock  9] PID 1 <- I/O completed, goes to HIGH
...

===== SIMULATION END (final clock: NN) =====
```

Each line records the clock instant and the corresponding scheduling event.

---

## References
- BASTOS, Valeria M. *Sistemas Operacionais I — Unidade II: Processos.* UFRJ/IM/DCC.
- BASTOS, Valeria M. *Sistemas Operacionais I — Unidade III: Escalonamento de Processos.* UFRJ/IM/DCC.
- STALLINGS, William. *Operating Systems: Internals and Design Principles.* Pearson.
- TANENBAUM, A. S.; BOS, H. *Modern Operating Systems.* Pearson.
- SILBERSCHATZ, A.; GALVIN, P. B.; GAGNE, G. *Operating System Concepts.* Wiley.
