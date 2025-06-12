#ifndef KERNEL_H
#define KERNEL_H

#include <string>
#define MAX_NODES 2

typedef enum taskType {
    JOB_A,
    JOB_B,
    JOB_C,
    JOB_D
} taskType;

typedef enum taskStatus {
    PENDING,
    DISPATCHED,
    COMPLETED,
    FAILED
} taskStatus;

typedef enum nodeStatus {
    IDLE,
    BUSY,
    OFFLINE
} nodeStatus;

typedef enum packetType {
    TASK_DISPATCH,
    TASK_RESULT,
    HEARTBEAT_REQUEST,
    HEARTBEAT_RESPONSE
} packetType;

typedef struct task {
    int taskID;
    taskType task;
    taskStatus status;
    std::string inputData;
    std::string resultData;
    int simulatedProgress;    // How much work has been done (0 to simulatedWorkUnits)
    int simulatedWorkUnits;   // Total work required for this task
    struct task *next;
} task;

typedef struct node {
    int nodeID;
    nodeStatus status;
    bool isResponsive;
    std::string nodeAddress;
    task *activeTask;
} node;

typedef struct packet {
    int sourceID;
    int destinationID;
    std::string payload;
    char flags;
    packetType pktType;
} packet;

extern task *taskQueueHead;

/* Task handlers */

/**
 * @brief Adds a new task to the end of the linked list queue.
 * @param head The current head of the task queue.
 * @param newTask The dynamically allocated task object to be added.
 * @return task* The (potentially new) head of the task queue. This is important
 * because if the queue was empty, 'head' will change to 'newTask'.
 */
task* enqueueTask(task* head, task* newTask);

/**
 * @brief Removes the front task from the queue and returns the new head.
 * @param currentHead The current head of the task queue.
 * @return task* A pointer to the new head of the queue after the operation.
 * Returns nullptr if the queue becomes empty.
 */
task* dequeueTask(task* currentHead);

/**
 * @brief Returns the task from the front of the linked list without removing it.
 * @param head The current head of the task queue.
 * @return task* A pointer to the first task, or nullptr if the queue is empty.
 */
task* peekTask(task* head);

/**
 * @brief Checks if the task queue is empty.
 * @param head The current head of the task queue.
 * @return bool True if the queue is empty (head is nullptr), false otherwise.
 */
bool isTaskQueueEmpty(task* head);

/**
 * @brief Frees all dynamically allocated task objects remaining in the queue.
 * This function is typically used during system shutdown.
 * @param head The current head of the task queue.
 */
void cleanUpTaskQueue(task* head);

/* system handlers */

/**
 * @brief The main scheduler function responsible for dispatching tasks to nodes
 * and monitoring their progress. It iterates through the node pool,
 * assigning pending tasks to idle nodes and checking busy nodes for completion.
 * @param currentTaskQueueHead The current head of the global task queue. This
 * parameter is passed by value and updated internally, with the new head
 * being returned.
 * @param nodePool A pointer to an array of pointers to 'node' structures,
 * representing the pool of worker nodes.
 * @return task* The updated head of the task queue after potential dispatches.
 */
task *dtkScheduler(task *currentTaskQueueHead, node **nodePool);

/**
 * @brief Provides a status overview of the DTK system, including the current
 * state of the task queue and each worker node.
 * @param currentHead The current head of the global task queue.
 * @param nodePool A pointer to an array of pointers to 'node' structures,
 * representing the pool of worker nodes.
 */
void dtkStatus(task *currentHead, node **nodePool);

/**
 * @brief Initiates a graceful shutdown of the DTK system. It clears all
 * remaining tasks in the queue and deallocates all worker node resources.
 * @param taskQueueHead The current head of the global task queue.
 * @param nodePool A pointer to an array of pointers to 'node' structures,
 * representing the pool of worker nodes.
 * @return bool True if shutdown was successful, false otherwise.
 */
bool dtkShutdown(task *taskQueueHead, node **nodePool);

#endif
