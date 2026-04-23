*This project has been created as part of the 42 curriculum by flauweri*

## Description

**Codexion** is a concurrency simulation project written in C using POSIX threads.  
It models multiple coders working in a shared environment where they must compete for limited resources (USB dongles) in order to compile their code.

Each coder alternates between compiling, debugging, and refactoring. To compile, a coder must acquire two dongles (left and right). The challenge is to coordinate access to these shared resources efficiently while avoiding concurrency issues such as deadlocks, starvation, and race conditions.

The simulation ends when either:
- A coder burns out (fails to compile within the allowed time), or
- All coders have completed the required number of compiles.

This project focuses on synchronization, scheduling strategies, and safe multi-threaded programming.

---

## Instructions

### Compilation

```bash
make
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Arguments

- `number_of_coders`: number of coders (and dongles)
- `time_to_burnout`: max time without compiling before burnout (ms)
- `time_to_compile`: duration of compiling (ms)
- `time_to_debug`: duration of debugging (ms)
- `time_to_refactor`: duration of refactoring (ms)
- `number_of_compiles_required`: simulation stops when all coders reach this count
- `dongle_cooldown`: cooldown time before a dongle can be reused (ms)
- `scheduler`: arbitration policy (`fifo` or `edf`)

### Example

```bash
./codexion 5 800 200 200 200 3 50 fifo
```

---

## Blocking cases handled

### Deadlock prevention
- Avoidance of circular wait conditions
- Controlled acquisition of resources

### Starvation prevention
- EDF scheduling prioritizes coders closest to burnout

### Resource contention
- Mutex protection for each dongle
- Fair scheduling (FIFO / EDF)

### Cooldown handling
- Dongles unavailable after release for a fixed time

### Precise burnout detection
- Dedicated monitor thread with strict timing guarantees

### Log serialization
- Mutex-protected output to prevent interleaving

---

## Thread synchronization mechanisms

### Mutexes (`pthread_mutex_t`)
- Protect shared resources and logs

### Condition variables (`pthread_cond_t`)
- Manage waiting queues and synchronization

### Monitor thread
- Detects burnout and stops simulation

### Priority queue (heap)
- Implements FIFO and EDF scheduling

### Race condition prevention
- All shared data is protected and synchronized

---

## Resources

### Documentation
- POSIX Threads (pthreads)
- `man pthread_create`
- `man pthread_mutex_*`
- `man pthread_cond_*`
- `gettimeofday`
- Dining Philosophers Problem
- Deadlocks and Coffman conditions
- Thread synchronization
- Scheduling algorithms (FIFO, EDF)
- https://codexion-visualizer.sacha-dev.me/

### AI usage

Research Notes

- Parsing issues (atoi, edge cases like "0000", condition errors)
- Pointer and struct handling (. vs ->, memset)
- Thread creation issues (pthread_create, segfaults, data passing)
- Shared data access and synchronization rules
- Mutex usage (pthread_mutex_init, protecting reads/writes, printf)
- Condition variables (pthread_cond_wait, thread coordination)
- Race condition debugging (Valgrind / Helgrind)
- Simulation logic (init → threads → monitor → cleanup)
- Features: burnout detection, dongle cooldown, EDF scheduling

- probleme de decalage de de quelque milliseconde au lancement
- single coder infinite loop