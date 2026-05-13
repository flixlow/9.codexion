*This project has been created as part of the 42 curriculum by flauweri.*

## Description

**Codexion** is a concurrency simulation project written in C using POSIX threads.  
It models multiple coders working in a shared environment where they must compete for limited resources (USB dongles) in order to compile their code.

Each coder alternates between compiling, debugging, and refactoring. To compile, a coder must acquire two dongles (left and right). The challenge is to coordinate access to these shared resources efficiently while avoiding concurrency issues such as deadlocks, starvation, and race conditions.

The simulation ends when either:
- A coder burns out (fails to compile within the allowed time), or
- All coders have completed the required number of compiles.

This project focuses on synchronization, scheduling strategies, and safe multi-threaded programming. It's an advanced study of the **Dining Philosophers Problem** applied to resource-constrained computing environments.

---

## Features

- **Multi-threaded simulation** with configurable number of coders
- **Resource arbitration** using FIFO or EDF (Earliest Deadline First) scheduling
- **Burnout detection** via dedicated monitor thread with precise millisecond timing
- **Dongle cooldown mechanism** to simulate resource recovery time
- **Thread-safe logging** preventing output interleaving
- **Deadlock prevention** through controlled resource acquisition in fixed order
- **Starvation mitigation** via EDF priority scheduling with dynamic deadline updates
- **State-based resource allocation** (unavailable flag + cooldown timestamp)

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
- **Avoidance of circular wait (Coffman's condition #4)**: Resources always acquired in consistent order by comparing dongle names (`if (dongle_one->name < dongle_two->name)`)
- **Controlled resource acquisition**: `try_to_take()` checks both `is_unavailable` flag and cooldown timestamp before acquisition
- **Mutual exclusion**: Each dongle protected by dedicated `pthread_mutex_t`

### Starvation prevention
- **EDF scheduling prioritizes coders closest to burnout**: `edf_sort_queue()` performs bubble sort on queue, ordering coders by deadline (computed as `get_time_ms() + burnout_threshold`)
- **FIFO fallback**: When scheduler is FIFO, `pop_n_push()` moves completed coder to end of queue, ensuring rotation
- **Dynamic deadline updates**: Each compile resets coder's deadline to current time + burnout threshold, preventing stale priorities
- **Monitor enforcement**: Dedicated thread polls all coders every 2ms, detecting burnout immediately and terminating simulation

### Resource contention management
- **Mutex protection per dongle**: Each `t_dongle` has `pthread_mutex_t dongle_mutex`
- **Flag updates**: `is_unavailable` and `cooldown` fields only modified within locked critical sections
- **State-based arbitration**: `try_to_take()` checks both conditions: `!(dongle->is_unavailable || dongle->cooldown > get_time_ms())`
- **Queue-based scheduling**: Fixed-size array `int *queue` holds coders in scheduling order, protected by `scheduler_mutex`

### Cooldown handling
- **Timestamp-based tracking**: `dongle->cooldown` stores absolute time when dongle becomes available again
- **Pre-acquisition validation**: In `try_to_take()`, cooldown checked before attempting to lock: `if (dongle->cooldown > get_time_ms()) return 0`
- **Release with cooldown**: `release_dongle()` sets `dongle->cooldown = get_time_ms() + dongle->config_cooldown`
- **Prevents rapid reuse**: Ensures hardware-like behavior where resources need recovery time

### Precise burnout detection
- **Dedicated monitor thread**: `monitor()` function polls all coders independently
- **Strict timing guarantees**: Compares `get_coder_deadline(&coders[i]) <= get_time_ms()` every 2ms
- **Immediate termination**: Sets `global->burnout = 1` within `stop_mutex` and prints burned-out coder
- **Clean shutdown**: Allows final "burned out" message even after `simulation_is_running()` returns false

### Log serialization
- **Global mutex for printf**: All `print()` calls protected by `print_mutex`
- **Prevents output corruption**: Ensures writes combining timestamp, coder ID, and message
- **Maintains readability**: No interleaving of concurrent thread outputs
- **Burnout always logged**: Special case allows "burned out" message even when simulation stopped

---

## Thread synchronization mechanisms

### Core Primitives

**Mutexes (`pthread_mutex_t`)**

*Global mutexes:*
- `start_mutex`: Protects `start` timestamp and initial deadline setup via `start()` function
- `print_mutex`: Serializes all `printf()` calls with `print()` function
- `stop_mutex`: Protects `burnout` flag to signal simulation termination
- `scheduler_mutex`: Protects `queue[]` array during sorting and reordering

*Per-resource mutexes:*
- `dongle->dongle_mutex`: Protects `is_unavailable` and `cooldown` fields in `try_to_take()` and `release_dongle()`
- `coder->coder_mutex`: Protects `compil_counter`, `deadline` fields; ensures counter updates and deadline reads

**Condition Variables (`pthread_cond_t`)**
- `global->cond`: Used for synchronization at simulation start; main thread broadcasts via `pthread_cond_broadcast()`, worker threads wait in `waiting_to_start()`
- No condition variables on dongles: Uses busy-waiting with `usleep(500)` in `has_taken_a_dongle()` to retry acquisition

**Monitor Thread**
- Created implicitly via `monitor()` function (runs in main thread, not pthread)
- Polls `get_coder_deadline()` for all coders every 2ms via `usleep(2000)`
- Independently checks `is_compile_done()` to detect when all coders finish
- Sets `global->burnout` flag to halt all worker threads

**Scheduling Queue (Array-based)**
- `int *queue` of size `config.n_coders`
- Initialized in `init_queue()`: even-numbered coders first (0, 2, 4...), then odd (1, 3, 5...)
- FIFO mode: `pop_n_push()` rotates coder to end of queue
- EDF mode: `edf_sort_queue()` performs bubble sort based on `get_coder_deadline()` values

### Race Condition Prevention

**Example 1: Dongle acquisition**
```c
pthread_mutex_lock(&dongle->dongle_mutex);
if (dongle->is_unavailable || (dongle->cooldown > get_time_ms()))
{
    pthread_mutex_unlock(&dongle->dongle_mutex);
    return (0);
}
dongle->is_unavailable = 1;
pthread_mutex_unlock(&dongle->dongle_mutex);
```
Without mutex: Two coders could both see `is_unavailable == 0` and both acquire same dongle.  
With mutex: Only one thread can check and modify state.

**Example 2: Compile counter update**
```c
pthread_mutex_lock(&coder->coder_mutex);
coder->compil_counter++;
pthread_mutex_unlock(&coder->coder_mutex);
```
Without mutex: Lost updates when multiple threads access counter.  
With mutex: All increments serialized.

**Example 3: Burnout detection**
```c
if (get_coder_deadline(&global->coders[i]) <= get_time_ms())
{
    pthread_mutex_lock(&global->stop_mutex);
    global->burnout = 1;
    pthread_mutex_unlock(&global->stop_mutex);
    print(global, ..., "burned out");
    return (0);
}
```
Without mutex: Race where multiple coders detect burnout simultaneously or read stale burnout flag.  
With mutex: Exactly one burnout detection, clean termination signal.

**Example 4: Deadlock avoidance**
```c
if (coder->dongle_one->name < coder->dongle_two->name)
    has_taken_a_dongle(coder, coder->dongle_one, coder->dongle_two);
else
    has_taken_a_dongle(coder, coder->dongle_two, coder->dongle_one);
```
Always acquires in consistent order (lower ID first). Prevents circular wait: if coder A holds dongle 0 and waits for 1, coder B cannot hold dongle 1 and wait for 0.

### Thread-safe Communication

- **Shared state updates**: All changes to `compil_counter`, `deadline`, `burnout`, dongle state occur within critical sections
- **Condition variable signaling**: Main thread broadcasts `cond` once to unblock all workers at simulation start
- **Monitor thread acts as arbiter**: Only monitor writes to `burnout` flag; workers only read it via `simulation_is_running()`
- **Busy-waiting for resources**: Workers spin on `usleep(500)` in `has_taken_a_dongle()` rather than condition variables for more responsive resource acquisition
- **Queue synchronization**: All queue modifications (`pop_n_push`, `edf_sort_queue`) protected by `scheduler_mutex`, preventing concurrent access during sorting

### Synchronization Flow

1. **Initialization**: Main thread allocates structures, initializes mutexes
2. **Thread creation**: Main thread spawns `config.n_coders` worker threads
3. **Start barrier**: All workers blocked in `waiting_to_start()` waiting on condition variable
4. **Simulation start**: Main thread calls `start()`, broadcasts condition, records timestamp
5. **Resource acquisition**: Workers busy-wait for dongles with 500µs sleep intervals
6. **Scheduling**: After release, queue reordered (FIFO rotate or EDF sort) within scheduler_mutex
7. **Burnout monitoring**: Main thread in `monitor()` polls deadlines every 2ms
8. **Termination**: Either all compiles done or burnout detected; `burnout` flag halts all workers
9. **Cleanup**: Main thread joins all workers, destroys all mutexes/conditions

---

## Resources

### Documentation
- POSIX Threads (pthreads): `man pthread_create`, `man pthread_mutex_*`, `man pthread_cond_*`
- Timing: `gettimeofday` for millisecond-precision timestamps, `usleep` for sub-millisecond delays
- Classic problems: Dining Philosophers Problem, Resource allocation graphs
- Coffman's conditions for deadlock (mutual exclusion, hold and wait, no preemption, circular wait)
- Scheduling algorithms: FIFO (First-In-First-Out), EDF (Earliest Deadline First with dynamic recalculation)
- Thread synchronization: https://codexion-visualizer.sacha-dev.me/
- Race condition debugging: Valgrind with Helgrind tool (`valgrind --tool=helgrind ./codexion ...`)

### AI Usage

**Research & Problem-solving**
- Parsing and input validation (handling atoi edge cases, boundary checks for 999999999 limit)
- Pointer arithmetic and struct access patterns (coder and dongle references, array indexing)
- Thread lifecycle management (pthread_create error handling, clean thread joining)

**Implementation guidance**
- Mutex initialization and proper locking patterns (pthread_mutex_init, protecting all reads/writes)
- Condition variable usage (pthread_cond_wait setup, broadcast timing for startup)
- Shared data safety (identifying which fields require protection via mutex)

**Debugging & Optimization**
- Race condition detection and fixing (Valgrind/Helgrind output analysis)
- Simulation correctness (initialization flow, monitor loop timing, cleanup order)
- Performance tuning: sleep interval selection (500µs for responsiveness, 2ms for monitor polling)
- Feature implementation: EDF bubble sort, queue rotation, cooldown timestamps

**Known challenges addressed**
- Millisecond-precision timing at startup with proper `gettimeofday` usage
- Single-coder edge case handling (circular dongle reference `next = (i + 1) % n_coders` prevents infinite loops)
- Scheduler switching without crashing (both FIFO and EDF use same queue structure)
- Deadlock-free ordering via deterministic dongle acquisition (comparing dongle names)
- Output synchronization without missed burnout messages (special case in print function)

### Project Structure

```
codexion/
├── coders/
│   ├── main.c           # Entry point, initialization orchestration
│   ├── init.c           # Thread, dongle, coder, queue initialization
│   ├── routine.c        # Worker thread main loop, resource acquisition
│   ├── action.c         # Compile, debug, refactor actions with timing
│   ├── scheduler.c      # FIFO/EDF scheduling, queue management
│   ├── monitor.c        # Burnout detection, simulation monitoring
│   ├── parsing.c        # Argument validation and config parsing
│   ├── utils.c          # Timing, synchronization checks, output
│   ├── ending.c         # Thread cleanup, mutex destruction, memory freeing
│   └── codexion.h       # Header with all type definitions
├── Makefile
└── README.md
```