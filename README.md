# Philosophers

**Philosophers** is a concurrency project written in C that simulates the Dining Philosophers Problem. It is built around thread coordination, mutex protection, timing accuracy, and safe resource management.

<p align="center">
  <img width="700" height="462" alt="philo" src="https://github.com/user-attachments/assets/95c2d26a-66c4-4f08-a0c8-c301ada1065b" />
</p>

## Features

- **Thread-based simulation** of philosophers competing for shared forks
- **One philosopher per thread** in the mandatory part
- **One mutex per fork** to protect shared access
- **Accurate timing control** for eating, sleeping, thinking, and starvation
- **Synchronized logging** messages never mix across threads
- **Death detection** with strict timing requirements
- **Optional meal goal** to stop the simulation when everyone has eaten enough
- **Robust cleanup and error handling** with no crashes, leaks, or data races

## Table of Contents

- [Project Scope](#project-scope)
- [Instructions](#instructions)
  - [Requirements](#requirements)
  - [Usage](#usage)
    - [Running the program](#running-the-program)
    - [Arguments](#arguments)
    - [Output format](#output-format)
- [Build](#build)
  - [Compilation](#compilation)
  - [Build modes](#build-modes)
  - [Debugging](#debugging)
  - [Validation](#validation)
- [Architecture and Implementation](#architecture-and-implementation)
  - [Initialization and setup](#initialization-and-setup)
  - [Simulation flow](#simulation-flow)
  - [Synchronization and monitoring](#synchronization-and-monitoring)
  - [Logging](#logging)
  - [Cleanup](#cleanup)
- [Resources](#resources)
- [License](#license)

## Project Scope

<details>
<summary><strong>Subject rules and scope</strong></summary>

### Core requirements

The project must simulate philosophers sitting around a table, cycling through eating, sleeping, and thinking while competing for shared forks. The simulation stops when a philosopher dies, or when every philosopher has eaten the required number of times if the optional meal count is provided.

For the mandatory part, the program must:

- Be written in **C**
- Create **one thread per philosopher**
- Use **one mutex per fork**
- Prevent **data races** and avoid invalid behavior such as crashes or double frees
- Print philosopher state changes in the required format
- Report a death message within **10 ms** of the actual death
- Free all allocated memory correctly

### Allowed functions and libraries

The mandatory part allows the following functions:

- `memset`, `printf`, `malloc`, `free`, `write`
- `usleep`, `gettimeofday`
- `pthread_create`, `pthread_detach`, `pthread_join`
- `pthread_mutex_init`, `pthread_mutex_destroy`
- `pthread_mutex_lock`, `pthread_mutex_unlock`

Libft is **not** authorized in this project.

### Project rules

The program must follow these rules:

- No **global variables**
- Must follow the **Norm**
- Must not crash unexpectedly
- Must not leak memory
- Must include a `"Makefile"` with `$(NAME)`, `all`, `clean`, `fclean`, and `re`
- Must compile with `cc` and the flags `-Wall -Wextra -Werror`
- Must support up to **200 philosophers**
- Timing values below **60 ms** are not intended for normal evaluation

### Program arguments

The program must accept:

- `number_of_philosophers`: number of philosophers and forks
- `time_to_die`: maximum time without eating before a philosopher dies, in milliseconds
- `time_to_eat`: time spent eating, in milliseconds
- `time_to_sleep`: time spent sleeping, in milliseconds
- `number_of_times_each_philosopher_must_eat`: optional stop condition when all philosophers reach that count

```sh
./philo 5 800 200 200 7
```

### Logging requirements

Each state change must be printed in this form:

- `timestamp_in_ms X has taken a fork`
- `timestamp_in_ms X is eating`
- `timestamp_in_ms X is sleeping`
- `timestamp_in_ms X is thinking`
- `timestamp_in_ms X died`

Additional log rules:

- Messages must not be mixed together
- Death must be announced no more than **10 ms** after it happens

### Bonus scope

**Important:** The official subject also includes a bonus part, but this repository currently implements **mandatory only**.

The subject also defines a bonus part with these rules:

- Program name: `"philo_bonus"`
- Each philosopher is a **process**
- Fork availability is represented by a **semaphore**
- The main process must not be a philosopher

Allowed bonus-specific functions include:

- `fork`, `kill`, `exit`, `waitpid`
- `sem_open`, `sem_close`, `sem_post`, `sem_wait`, `sem_unlink`

**Status in this repository:** only the **mandatory part** is implemented. The bonus is part of the official subject, but it is not included here.

</details>

## Instructions

### Requirements

- C compiler with POSIX threads support
- Unix-like environment for `pthread`, `gettimeofday`, and `usleep`
- Standard build tools such as `make`

### Usage

#### Running the program

Run the program with the required timing arguments:

```sh
./philo 4 410 200 200
```

With the optional meal count:

```sh
./philo 4 410 200 200 5
```

#### Arguments

Example:

- `4`: number of philosophers
- `410`: time to die in ms
- `200`: time to eat in ms
- `200`: time to sleep in ms
- `5`: optional number of meals per philosopher

#### Output format

The program prints timestamped state changes for each philosopher, including fork pickup, eating, sleeping, thinking, and death. Output must remain readable and synchronized even when multiple threads are active.

## Build

### Compilation

```sh
make (all)
make clean
make fclean
make re
make debug
make valgrind
```

### Build modes

The build system supports multiple targets for debugging and runtime behavior:

- `make` builds the default optimized release version.
- `make debug` builds with debug symbols and sanitizers enabled.
- `make valgrind` builds a debug-friendly version for memory checking.
- `make time` enables the timing adjustment used for comparisons.
- `make readable` enables colored output formatting.
- `make verbose` enables additional diagnostic output.
- `make no_rules` disables the extra runtime rule checks.
- `make valgrind_margin` increases timing tolerance for Valgrind runs.

These options can be combined when needed. For example, `make readable verbose no_rules` keeps the output readable and enables extra diagnostics, while `make time` applies the timing adjustment used for certain edge-case tests. Readable output may slightly change and slowdown the runtime behavior, so use it for testing rather than for strict timing checks.

### Debugging

You can check for threading issues with:

```sh
valgrind --tool=helgrind --max-threads=10000 ./philo
```

This is useful for spotting race conditions and lock issues during development, and increases the maximum thread count to test larger simulations.

### Validation
<details>
<summary><strong>Testing notes and edge cases</strong></summary>

### Subject-aware limits

These limits match the usual evaluation scope:

- Do not test with more than **200 philosophers**
- Do not use `time_to_die`, `time_to_eat`, or `time_to_sleep` below **60 ms** for normal subject evaluation

### Evaluation-style checks

These cases stay within the expected project limits and are useful for checking correctness:

```sh
# Should not die
./philo 199 310 100 100
./philo 199 190  60  60
```
```sh
# Margin should be enough to survive.
./philo 200 220 100 100 15
```
```sh
# If only the supervisor checks status, a death can be missed.
# This case should die.
./philo 200 400 200 200 6
```

### Additional edge cases

These are not all mandatory for evaluation, but they are useful stress tests:

```sh
# Borderline timing cases.
./philo 200 130 60 60
./philo 199 190 60 60
```

```sh
# Below normal subject timing limits, but useful for testing behavior and possibly data races. These may survive only if timing is handled too loosely or if microseconds are effectively cut down to milliseconds.
./philo 2 10 1 1
./philo 2 10 4 4
./philo 3 15 1 1
./philo 3 16 5 5
```

`make time` can be used to enable a timing adjustment that may help these cases survive, but is not allowed for normal evaluation. Without this adjustment, these cases for mine implementation should die.

```sh
# These should not survive only if timing is handled too loosely
# or if microseconds are effectively cut down to milliseconds
./philo 2 200 100 100 10
./philo 3 300 100 100 10
```

Extreme stress tests with many philosophers and low timing values can be used to check, if they survive, either you created a very robust implementation or have a fast enough machine to handle the load. These are not mandatory for evaluation, but they are useful stress tests:
```sh
./philo 888 210 100 100
./philo 444 130  60  60
./philo 777 310 100 100
./philo 333 190  60  60

./philo 222  60  20  20
./philo 555 100  20  20

./philo 12000 300 100 100
./philo  9999 320 100 100
```

</details>

## Architecture and Implementation

### Initialization and setup

The program [starts](src/main.c) by [initializing](src/initialize.c) and parsing the input, validating the [rules](src/init_rules.c), and allocating the philosopher structures that drive the simulation. Each philosopher is initialized with its own id, shared data pointer, status, timing values, and fork pairing, where the forks themselves are the mutex-protected shared resources used to keep access synchronized between neighbors.

Once the shared state is ready, the program launches the philosopher [threads](src/init_pthreads_and_mutexes.c) and the [reporter](src/reporter_of_nodes_thread.c) thread, while the main thread becomes the [supervisor](src/supervisor_thread.c) that monitors the simulation and handles shutdown. Each thread enters the philosopher [routine](src/routine.c), which first confirms that startup completed correctly, then handles the special single-philosopher case, and finally moves into the normal simulation cycle.

### Simulation flow

Each philosopher runs in its own thread and repeatedly goes through the core cycle of taking forks, eating, sleeping, and thinking until the simulation ends. That flow is handled in [routine](src/routine.c), while [activity](src/activity.c) contains the actions that make each step happen.

### Synchronization and monitoring

Fork access is protected with one mutex per fork, while each philosopher keeps separate state for timing and status updates to avoid races. Monitoring is handled by the [supervisor](src/supervisor_thread.c) thread, which watches for death and meal completion, the [reporter](src/reporter_of_nodes_thread.c), which prints queued messages in order, and the [time](src/time.c) helpers, which include usleep_interval so sleeps stay accurate without missing a death check.

### Logging


State messages are queued and printed in order so the output stays readable and does not interleave across threads. The [reporter](src/reporter_of_nodes_thread.c) logic keeps the [output](src/print.c) consistent while the simulation is running.

### Cleanup

When the simulation ends, the program joins the threads, destroys the mutexes, frees any remaining resources, and exits [cleanly](src/clean_up.c).

## Resources

### Project references

- [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Threads and Concurrency notes](https://student.cs.uwaterloo.ca/~cs350/S18/notes/threads.pdf)
- [Dining Philosophers Problem and Deadlock](https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/DiningPhil.html)
## License

This project is licensed under the [MIT License](LICENSE).
