# xv6 Operating System

xv6 is a teaching operating system that reimplements Unix Version 6 for the RISC-V architecture. This project extends xv6 with custom system calls and multiple CPU scheduling algorithms to demonstrate core OS concepts including process management, scheduling, and kernel programming.

## Building and Running

### Prerequisites
- RISC-V toolchain (riscv64-unknown-elf or riscv64-linux-gnu)
- QEMU for RISC-V (qemu-system-riscv64)

### Build Commands

```bash
# Build and run with default Round Robin scheduler
make qemu

# Build with specific scheduler
make qemu SCHEDULER=FCFS      # First Come First Served
make qemu SCHEDULER=PBS       # Priority Based Scheduler
make qemu SCHEDULER=MLFQ      # Multi-Level Feedback Queue

# Exit QEMU: Press Ctrl-A then X
```

## Implementation Details

### System Calls

#### strace - System Call Tracer
- Intercepts and logs system calls made by processes
- Uses bitmask to selectively trace specific syscalls
- Trace mask inherited by child processes through fork

#### waitx - Extended Wait
- Returns detailed timing statistics: runtime, sleep time, exit time
- Used for benchmarking scheduler performance

#### set_priority - Priority Modification
- Dynamically modifies process static priority for PBS scheduler
- Triggers rescheduling when priority is increased

---

### Schedulers

#### 1. FCFS (First Come First Served)
- Non-preemptive scheduler selecting process with lowest creation time
- Avg runtime: 44 ticks | Avg wait: 54 ticks

#### 2. PBS (Priority-Based Scheduler)
- Dynamic priority: `max(0, min(Static Priority - niceness + 5, 100))`
- Niceness: `(sleep_time / (runtime + sleep_time)) × 10`
- I/O-bound processes get priority boosts
- Avg runtime: 24 ticks | Avg wait: 108 ticks

#### 3. MLFQ (Multi-Level Feedback Queue)
- 5 priority queues (0 = highest) with time slices: 1, 2, 4, 8, 16 ticks
- Aging: Processes waiting >50 ticks promoted to prevent starvation
- **Exploitation:** Process can yield before time slice expires to avoid demotion

---

## Performance Comparison

Benchmarked using `schedulertest`:

| Scheduler | Avg Runtime | Avg Wait Time | Total Runtime | Total Wait Time | Analysis |
|-----------|-------------|---------------|---------------|-----------------|----------|
| **DEFAULT (RR)** | 18 | 117 | 189 | 1171 | Best runtime due to fair time-slicing; high wait time from frequent context switches |
| **FCFS** | 44 | 54 | 449 | 547 | Highest runtime, lowest wait time; processes run to completion; convoy effect |
| **PBS** | 24 | 108 | 244 | 1084 | Best balance; dynamic priority adapts to I/O vs CPU-bound processes |
| **MLFQ** | - | - | - | - | Adaptive multi-queue; favors interactive processes with aging |

**Key Insight:** No universally "best" scheduler. Round Robin excels for interactive systems, FCFS for batch processing, PBS/MLFQ for mixed workloads. Choice depends on workload characteristics and priorities.

---

## User Programs

Once xv6 boots, available commands include:

```bash
$ ls                    # List files
$ cat README            # View file
$ strace ls             # Trace system calls made by ls
$ setpriority 50 3      # Set priority of process 3 to 50
$ schedulertest         # Run scheduler benchmark
```
