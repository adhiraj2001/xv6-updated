# Assignment - 4 : xv6

## Spec1

### strace() syscall:

- Made required changes to the following files to add the syscall:

1. `syscall.h`
2. `defs.h`
3. `user.h`
4. `sysproc.c`
5. `usys.S`
6. `syscall.c`

- Added `rtime`, `stime`, `etime` in `struct proc`.

- There values update acccording to `clockitr()`.

- Added `strace.c` user program in `user` and `Makefile`.

## Schedulers:

Added `SCHEDULER` object in `Makefile`.

### FCFS

- Iterate over all processes and select the one with lowest creation time which has status `RUNNABLE`.

### PBS

- Added `priority` and `niceness` in `struct proc`.

- Process is selected according to the defined Dynamic Priority.

- After process is scheduled, niceness is updated according to runtime and waiting time for the process.

- Added `set_priority` syscall to change the static priority of the process.

- Added `setpriority` user program in the `user` and `Makefile`.

### MLFQ

- Added `queue_priority` to store current queue priority.

- `mlfq_init()`, initializes the required values of queues.

- `push(proc, pr)`, pushes the given proc into the queue whose number is pr.

- `pop(proc, pr)`, removes the given proc from the queue whose number is pr and shifts the PBCs coming after it one step to the left.

- We iterate over all process and select the first process with hight priority. And then implement different time slices for different queues.

- Aging and starvation is also implemented for this.

### How could this be exploited by a process (MLFQ)?

- If a process gives up CPU before MAX_AGE, it will not demote to lower priority.

- Thus proccesses can just preempt themselves just before max_age and add themselves to queue again.

- This explotation keeps the process in priority 0, forever.

### Comparison:

- DEFAULT (RR):

Total rtime: 189, Total wtime: 1171

Average rtime: 18, Average wtime: 117

- FCFS

Total rtime: 449, Total wtime: 547

Average rtime: 44, Average wtime: 54

- PBS

Total rtime: 244, Total wtime: 1084

Average rtime: 24, Average wtime: 108