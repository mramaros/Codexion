# Codexion

*This project has been created as part of the 42 curriculum by mramaros.*

---

## Description

**Codexion** is a concurrency simulation written in C that models coders competing for shared USB dongles in a co-working hub. Each coder must regularly compile their quantum code using two dongles simultaneously — one in each hand. The challenge is to orchestrate all threads so that no coder ever burns out while avoiding deadlocks and resource starvation.

The simulation is a variant of the classic **Dining Philosophers Problem**, adapted with the following mechanics:

- Coders cycle through three states: **compiling → debugging → refactoring**
- Each coder needs **two dongles** simultaneously to compile
- A coder **burns out** if they go too long without compiling
- Dongles have a **cooldown period** after being released
- Resource arbitration follows a configurable **scheduling policy** (FIFO or EDF)

---

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` binary using `cc` with `-Wall -Wextra -Werror -pthread`.

Available Makefile rules: `all`, `clean`, `fclean`, `re`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are **mandatory**. Times are in milliseconds. The scheduler must be either `fifo` or `edf`.

#### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) in the simulation |
| `time_to_burnout` | Max ms without starting a compile before burnout |
| `time_to_compile` | Duration of the compile phase (holds 2 dongles) |
| `time_to_debug` | Duration of the debug phase |
| `time_to_refactor` | Duration of the refactor phase |
| `number_of_compiles_required` | Target compile count per coder to end the simulation |
| `dongle_cooldown` | Milliseconds a dongle is unavailable after being released |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

#### Example

```bash
./codexion 4 800 200 200 400 5 50 fifo
```

#### Example Output

```
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
405 2 has taken a dongle
406 2 has taken a dongle
406 2 is compiling
606 2 is debugging
806 2 is refactoring
1505 4 burned out
```

---

## Blocking Cases Handled

### Deadlock Prevention

The classic deadlock scenario occurs when every coder holds one dongle and waits for the other. This is prevented by:

- Implementing a **fair scheduling queue** (FIFO or EDF) inside each dongle's state structure, ensuring coders wait in an ordered queue rather than spinning or holding partial resources.
- A coder only acquires dongles **sequentially** (left then right), and the dongle-level queue ensures no circular wait forms under normal conditions.

The four Coffman conditions are addressed as follows:
- **Mutual exclusion**: required by design (dongles are exclusive resources)
- **Hold and wait**: mitigated by the scheduling policy — coders queue atomically
- **No preemption**: maintained; dongles are only released voluntarily after compiling
- **Circular wait**: broken by the consistent left-then-right acquisition order

### Starvation Prevention

Under **EDF scheduling**, each dongle prioritizes the coder whose burnout deadline (`last_compile_start + time_to_burnout`) is earliest. This guarantees that the coder closest to burning out always gets priority, preventing starvation as long as the simulation parameters are feasible.

Under **FIFO scheduling**, requests are served strictly in arrival order, which is fair by construction.

### Cooldown Handling

After a coder releases a dongle, a timestamp is recorded. Any subsequent request for that dongle is blocked until `dongle_cooldown` milliseconds have elapsed, enforced via `pthread_cond_timedwait`.

### Burnout Detection

A dedicated **monitor thread** runs concurrently and polls each coder's last compile timestamp. If the elapsed time exceeds `time_to_burnout`, the monitor logs the burnout event and halts the simulation. The burnout message is guaranteed to appear within **10 ms** of the actual burnout time.

### Log Serialization

All output is protected by a dedicated **print mutex**. No two messages can interleave on the same line or be printed out of order.

---

## Thread Synchronization Mechanisms

### `pthread_mutex_t`

Each dongle has its own mutex protecting its internal state (owner, availability, cooldown timestamp, and wait queue). The print output also has a dedicated mutex.

```c
pthread_mutex_lock(&dongle->mutex);
// safely inspect or modify dongle state
pthread_mutex_unlock(&dongle->mutex);
```

This prevents race conditions where two coders simultaneously see a dongle as available.

### `pthread_cond_t`

Each dongle uses a condition variable so waiting coders sleep instead of busy-waiting. When a dongle becomes available, it signals the appropriate next waiter based on the scheduler policy.

```c
// Coder waits for the dongle to be free and for cooldown to pass
pthread_cond_wait(&dongle->cond, &dongle->mutex);

// After release, the dongle signals the queue
pthread_cond_broadcast(&dongle->cond);
```

### Priority Queue (Heap)

To implement FIFO and EDF ordering, each dongle maintains an internal **min-heap** of waiting coders. For FIFO, the key is the request arrival time. For EDF, the key is the coder's burnout deadline. This queue is allocated and freed cleanly for each dongle.

### Monitor Thread

The monitor is a dedicated thread that sleeps in short intervals and checks each coder's compile timestamp. It uses a shared `simulation_stopped` flag protected by a mutex to signal all coder threads to exit cleanly.

### Thread-Safe Communication

Coders and the monitor communicate exclusively through shared state (coder structs) protected by mutexes. No direct inter-thread calls are made. When the monitor detects burnout, it sets the stop flag, prints the burnout message (under the print mutex), and joins all coder threads gracefully.

---

## Resources

- [POSIX Threads Programming – Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores – Allen B. Downey](https://greenteapress.com/wp/semaphores/)
- [Dining Philosophers Problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling – Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- `man pthread_mutex_init`, `man pthread_cond_wait`, `man gettimeofday`

### AI Usage

AI was used during this project for the following tasks:
- Drafting and formatting this README
- Clarifying the behavior of `pthread_cond_timedwait` and its interaction with absolute vs. relative timeouts
- Suggesting the min-heap structure for the priority queue and reviewing the heap logic

All AI-generated content was reviewed, tested, and fully understood before being included in the project.
