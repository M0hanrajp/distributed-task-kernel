<h1 align="center"> Distributed Task Kernel (DTK)</h1>

## Overview

The Distributed Task Kernel (DTK) is a lightweight, command-line-based system designed to simulate a simple distributed task processing environment. It acts as a rudimentary scheduler that manages a queue of tasks and dispatches them to a pool of available worker nodes. This project demonstrates fundamental concepts of distributed systems, including task queuing, parallel processing, and node management.

```bash
                               +---------------------+
                               |       U S E R       |
                               |       (CLI)         |
                               +---------+-----------+
                                         |
            (Commands: submit, shutdown, exit)
                                         v
                   +---------------------------------------+
                   |           D T K   K E R N E L         |
                   |         (Central Orchestrator)        |
                   +-----------+---------------+-----------+
                               |               |
    (1. Enqueue Task)          |               | (2. Dispatch Task)
                               v               v
            +---------------------+      +----------------------+
            |     T A S K         |      |   N O D E   P O O L  |
            |     Q U E U E       |      |    (Worker Nodes:    | 
            | (FIFO Linked List)  |      |      IDLE/BUSY)      |
            +---------------------+      +----------------------+
                        ^                      |
                        |                      | (3. Simulate Progress & Completion)
                        |                      |
                        +----------------------+
                        (4. Task Status Update)
```

## Features

* **Task Submission:** Users can submit tasks (JOB\_A, JOB\_B, JOB\_C, JOB\_D) with associated input data via a command-line interface.
* **Task Queuing:** Submitted tasks are added to a central FIFO (First-In, First-Out) queue.
* **Multi-Node Dispatching:** The scheduler efficiently dispatches tasks from the queue to an available pool of worker nodes (simulated).
* **Task Execution Simulation:** Nodes simulate task execution over a variable number of "work units," reporting progress and completion.
* **Node Status Management:** Nodes transition between `IDLE` (ready for tasks) and `BUSY` (processing a task) states.
* **Graceful Shutdown:** The system supports a `shutdown` command to clear remaining tasks in the queue and deallocate all system resources, including any tasks still in progress on nodes.
* **Error Handling:** Basic validation for command syntax and task types.

## Architecture

The DTK project consists of three main components:

1.  **Task Queue:** A linked list structure that holds `PENDING` tasks submitted by the user. Tasks are added to the tail and dispatched from the head.
2.  **Node Pool:** An array of `node` structures, simulating worker machines. Each node can be `IDLE`, `BUSY`, or `OFFLINE`.
3.  **Scheduler (`dtkScheduler`):** The core logic that iterates through the `nodePool`. For `IDLE` nodes, it attempts to `dequeue` a task from the `taskQueueHead` and assign it. For `BUSY` nodes, it simulates task progress and handles task completion, marking the node `IDLE` again.

```bash
                               +---------------------+
                               |       U S E R       |
                               |        (CLI)        |
                               +----------+----------+
                                          |
        (Command: submit JOB_A <data>, shutdown, exit)
                                          v
                               +----------+----------+
                               |     K E R N E L     |
                               | (Main Application)  |
                               +----------+----------+
                                          |
                                          | 1. Create Task (PENDING)
                                          | 2. Enqueue Task
                                          v
                               +---------------------+
                               |     T A S K         |
                               |     Q U E U E       |
                               | (FIFO Linked List)  |
                               +----------+----------+
                                          |
                                          | 3. Dequeue & Dispatch
                                          |    (Scheduler Logic)
                                          v
       +------------------------------------------------------------------+
       |                  W O R K E R   N O D E   P O O L                 |
       |  +-------------+  +-------------+   +-------------+  +------+    |
       |  |   NODE 0    |  |   NODE 1    |   |   NODE 2    |  | ...  |    |
       |  | (Simulated) |  | (Simulated) |   | (Simulated) |  |(More)|    |
       |  |   IDLE/BUSY |  |  IDLE/BUSY  |   |  IDLE/BUSY  |  |      |    |
       |  +-------------+  +-------------+   +-------------+  +------+    |
       |        ^                   ^                 ^              ^    |
       |        | 4. Simulate Progress & Completion (e.g., +3 units) |    |
       |        +----------------------------------------------------+    |
       +------------------------------------------------------------------+
                                          |
                                          | 5. Task Result / Completion Notification
                                          v
                               +----------+----------+
                               |     K E R N E L     |
                               |  (Result Logging,   |
                               |   Memory Cleanup,   |
                               |   Shutdown Logic)   |
                               +---------------------+
```

### How it Works

The `main` loop continuously prompts for user commands. Upon a `submit` command, a new task is created and enqueued. After enqueuing, the `dtkScheduler` function is invoked. This function iterates through the simulated nodes:
* If a node is `IDLE` and tasks are available in the queue, it dispatches the next task to that node and removes the task from the queue.
* If a node is `BUSY`, it simulates progress on the `activeTask` assigned to it. Once a task completes, the node becomes `IDLE` again, and the completed task's memory is freed.
The `dtkScheduler` returns the updated head of the task queue to `main`, ensuring the queue state remains synchronized. The `shutdown` command cleans up all remaining tasks and node resources.

## Build Instructions

To build the DTK project, you will need a C++ compiler (like g++) & CMake.

1.  **Navigate to the project directory:**
    ```bash
    cd /path/to/your/dtk-project
    ```
2.  **Compile the source files:**
    ```bash
    mkdir build
    cd build/
    cmake ..
    make   
    ```
3. **Run the project**
    ```bash
    ./dtk_kernel_app
    ```
## Usage

#### **`submit <TaskType> <InputData>`**: Submits a new task to the queue.
    * `<TaskType>`: One of `JOB_A`, `JOB_B`, `JOB_C`, `JOB_D`.
    * `<InputData>`: Any string representing input data for the task (e.g., a number, a message).
    * **Example:** `submit JOB_A 200`
    * **Example:** `submit JOB_C "process_file_xyz"`
    # Note that the data is just for simulation

#### **`shutdown` or `exit` **: 

- Initiates a graceful shutdown of the DTK system. It clears all pending tasks and deallocates node resources.
- Exits the DTK command-line interface. If `shutdown` was not called, it will trigger the shutdown process automatically.

#### **`status:`**

Lists the current nodes that are working together to finish tasks/assigned jobs.

    $ status
    [STAT]: Node ID: 0 Status: BUSY At addr: 192.168.1.10
    [PROG]: Task ID: 1 Status: DISPATCHED @ Node: 192.168.1.10 Progress: (4/5 units).
    [STAT]: Node ID: 1 Status: BUSY At addr: 192.168.1.11
    [PROG]: Task ID: 2 Status: DISPATCHED @ Node: 192.168.1.11 Progress: (2/6 units).
    [PROG]: Task ID: 3 Status: PENDING Progress: (0/7 units).


### Example Interaction

```
$ ./build/dtk_kernel_app
DTK-mpunix $ help
[INFO]: submit <job-type> <input-data>, submit a job with valid input data
[INFO]: continue <no-params>, moves progress of a task by x units
[INFO]: status <no-params>, status of current nodes their tasks
[INFO]: shutdown <no-params>, delete all nodes and tasks assigned
[INFO]: exit <no-params>, exit DTK program
...
...
DTK-mpunix $ submit JOB_A 200
[INFO]: Task enqueue in progress...
[INFO]: Task ID 1 (JOB_A) submitted and queued.
[INFO]: SCHEDULER - Checking Node ID: 0, Status: IDLE, Queue Head Task ID: 1
[INFO]: Dispatched Task ID: 1 to Node ID: 0 @ address: 192.168.1.10
[INFO]: Task ID 1 dequeued from the list!
[INFO]: SCHEDULER - Checking Node ID: 1, Status: IDLE, Queue Head Task ID: NULL
[INFO]: Node ID: 1 is IDLE, there are no new tasks
DTK-mpunix $ submit JOB_B 300
[INFO]: Task enqueue in progress...
[INFO]: Task ID 2 (JOB_B) submitted and queued.
[INFO]: SCHEDULER - Checking Node ID: 0, Status: BUSY, Queue Head Task ID: 2
[INFO]: Currently Node ID: 0 is Busy, checking for task completion
[INFO]: Node ID: 0 is busy with Task ID: 1 (2/5 units).
[INFO]: SCHEDULER - Checking Node ID: 1, Status: IDLE, Queue Head Task ID: 2
[INFO]: Dispatched Task ID: 2 to Node ID: 1 @ address: 192.168.1.11
[INFO]: Task ID 2 dequeued from the list!
DTK-mpunix $ submit JOB_C 400
[INFO]: Task enqueue in progress...
[INFO]: Task ID 3 (JOB_C) submitted and queued.
[INFO]: SCHEDULER - Checking Node ID: 0, Status: BUSY, Queue Head Task ID: 3
[INFO]: Currently Node ID: 0 is Busy, checking for task completion
[INFO]: Node ID: 0 is busy with Task ID: 1 (4/5 units).
[INFO]: SCHEDULER - Checking Node ID: 1, Status: BUSY, Queue Head Task ID: 3
[INFO]: Currently Node ID: 1 is Busy, checking for task completion
[INFO]: Node ID: 1 is busy with Task ID: 2 (2/6 units).
DTK-mpunix $ status
[STAT]: Node ID: 0 Status: BUSY At addr: 192.168.1.10
[PROG]: Task ID: 1 Status: DISPATCHED @ Node: 192.168.1.10 Progress: (4/5 units).
[STAT]: Node ID: 1 Status: BUSY At addr: 192.168.1.11
[PROG]: Task ID: 2 Status: DISPATCHED @ Node: 192.168.1.11 Progress: (2/6 units).
[PROG]: Task ID: 3 Status: PENDING Progress: (0/7 units).
DTK-mpunix $ continue
[INFO]: SCHEDULER - Checking Node ID: 0, Status: BUSY, Queue Head Task ID: 3
[INFO]: Currently Node ID: 0 is Busy, checking for task completion
[INFO]: Task ID: 1 completed over Node ID: 0
[INFO]: SCHEDULER - Checking Node ID: 1, Status: BUSY, Queue Head Task ID: 3
[INFO]: Currently Node ID: 1 is Busy, checking for task completion
[INFO]: Node ID: 1 is busy with Task ID: 2 (4/6 units).
DTK-mpunix $ submit JOB_D 300
[INFO]: Task enqueue in progress...
[INFO]: Task ID 4 (JOB_D) submitted and queued.
[INFO]: SCHEDULER - Checking Node ID: 0, Status: IDLE, Queue Head Task ID: 3
[INFO]: Dispatched Task ID: 3 to Node ID: 0 @ address: 192.168.1.10
[INFO]: Task ID 3 dequeued from the list!
[INFO]: SCHEDULER - Checking Node ID: 1, Status: BUSY, Queue Head Task ID: 4
[INFO]: Currently Node ID: 1 is Busy, checking for task completion
[INFO]: Task ID: 2 completed over Node ID: 1
DTK-mpunix $ status
[STAT]: Node ID: 0 Status: BUSY At addr: 192.168.1.10
[PROG]: Task ID: 3 Status: DISPATCHED @ Node: 192.168.1.10 Progress: (0/7 units).
[STAT]: Node ID: 1 Status: IDLE At addr: 192.168.1.11
[PROG]: Task ID: 4 Status: PENDING Progress: (0/8 units).
DTK-mpunix $ exit
[INFO]: DTK program exit in progress..
[INFO]: Initiating DTK shutdown command ...
[INFO]: Task ID: 4 is deleted..
[INFO]: All Tasks deleted !
[INFO]: Node ID: 0 @ address: 192.168.1.10 deletion in progress...
[INFO]: Task ID: 3 in progress, but deleting...
[INFO]: Node ID: 1 @ address: 192.168.1.11 deletion in progress...
[INFO]: All resources deallocated. Shutting down.
```
---
Thanks :)
