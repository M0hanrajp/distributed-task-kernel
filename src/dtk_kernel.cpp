#include "dtk_kernel.hpp"
#include <ostream>
#include <thread>
#include <chrono>
#include <iostream>

// Helper function to convert nodeStatus enum to string for debugging
std::string getNodeStatusString(nodeStatus status) {
    switch (status) {
        case IDLE:    return "IDLE";
        case BUSY:    return "BUSY";
        case OFFLINE: return "OFFLINE";
        default:      return "UNKNOWN";
    }
}

// Helper function to convert taskStatus enum to string for debugging
std::string getTaskStatusString(taskStatus status) {
    switch (status) {
        case PENDING:    return "PENDING";
        case DISPATCHED: return "DISPATCHED";
        case COMPLETED:  return "COMPLETED";
        case FAILED:     return "FAILED";
        default:         return "UNKNOWN";
    }
}

// scheduler function
task *dtkScheduler(task *currentTaskQueueHead, node **nodePool) {
    for(int i = 0; i < MAX_NODES; i++) {
        std::cout << "[INFO]: SCHEDULER - Checking Node ID: " << nodePool[i]->nodeID
                  << ", Status: " << getNodeStatusString(nodePool[i]->status)
                  << ", Queue Head Task ID: " << (currentTaskQueueHead ? std::to_string(currentTaskQueueHead->taskID) : "NULL")
                  << std::endl;

        // Dispatch a new task if node is IDLE
        if(nodePool[i]->status == IDLE && (!isTaskQueueEmpty(currentTaskQueueHead))) {
            task *taskToDispatch = currentTaskQueueHead;
            nodePool[i]->activeTask = taskToDispatch;
            nodePool[i]->status = BUSY;
            taskToDispatch->status = DISPATCHED;
            std::cout << "[INFO]: Dispatched Task ID: " << taskToDispatch->taskID
                      << " to Node ID: " << nodePool[i]->nodeID
                      << " @ address: " << nodePool[i]->nodeAddress << std::endl;

            // DEBUG LOG: About to dequeue
            //std::cout << "[DEBUG]: SCHEDULER - About to dequeue Task ID: " << taskToDispatch->taskID
            //          << " for Node ID: " << nodePool[i]->nodeID << std::endl;
            // FIX #1: Correctly update the currentTaskQueueHead with the new head returned by dequeueTask
            // FIX #2: Pass the actual current head of the queue to dequeueTask
            currentTaskQueueHead = dequeueTask(currentTaskQueueHead);
            std::this_thread::sleep_for(std::chrono::milliseconds(75));

            /* the task that is dispatched to node is removed
             * from the linked list using dequeue, a new head 
             * is returned to taskQueueHead.
             *
             * By using continue below we are checking for the
             * next empty node & if taskQueueHead is valid then
             * it is dispatched to the next node that is IDLE.
             */
            continue;
        } else if(nodePool[i]->status == BUSY) {
            std::cout << "[INFO]: Currently Node ID: " << nodePool[i]->nodeID
                      << " is Busy, checking for task completion\n";
            // ----- Task simulation to make node BUSY -> IDLE -----

            /* if node is busy,
             * checkin on that node if task is completed (a threshold)
             * if the task meets the threshold call a execute function
             * to complete the task and make the node IDLE to dispatch
             * the next task
             *
             * if task is not yet complete, simulate the task by using
             * simulatedWorkUnits & increase it everytime to move the
             * counter ahead to simulte the task's completion
             */
            nodePool[i]->activeTask->simulatedProgress += 2;

            if(nodePool[i]->activeTask->simulatedProgress >=
                    nodePool[i]->activeTask->simulatedWorkUnits) {
                // --- Task is now "done" ---
                // assign the result data to the task
                nodePool[i]->activeTask->resultData = "[TASK COMPLETE]";
                nodePool[i]->activeTask->status = COMPLETED;
                std::cout << "[INFO]: Task ID: " << nodePool[i]->activeTask->taskID
                          << " completed over Node ID: "<< nodePool[i]->nodeID << std::endl;

                // --- Delete task's contents ---
                task *completedTask = nodePool[i]->activeTask;
                nodePool[i]->activeTask = nullptr;
                nodePool[i]->status = IDLE;
                delete completedTask;
                completedTask = nullptr;

                // keep checking for next node
                continue;
            } else {
                // Task is not yet complete hence keep checking for next nodes
                std::cout << "[INFO]: Node ID: " << nodePool[i]->nodeID
                          << " is busy with Task ID: " << nodePool[i]->activeTask->taskID
                          << " (" << nodePool[i]->activeTask->simulatedProgress
                          << "/" << nodePool[i]->activeTask->simulatedWorkUnits << " units).\n";
                continue;
            }

        } else if(nodePool[i]->status == OFFLINE) {
            std::cout << "[WARNING]: Node ID: " << nodePool[i]->nodeID
                      << " is offline, checking next node\n";
            continue;
        } else {
            // node is IDLE, head is nullptr
            std::cout << "[INFO]: Node ID: " << nodePool[i]->nodeID
                      << " is IDLE, there are no new tasks\n";
            continue;
        }
    }
    return currentTaskQueueHead;
}

// provides status for nodes and tasks
void dtkStatus(task *currentHead, node **nodePool) {

    for(int i = 0; i < MAX_NODES; i++) {
        std::cout << "[STAT]: Node ID: " << nodePool[i]->nodeID
                  << " Status: " << getNodeStatusString(nodePool[i]->status)
                  << " At addr: " << nodePool[i]->nodeAddress << std::endl;
        if(nodePool[i]->status == BUSY) {
            std::cout << "[PROG]: Task ID: " << nodePool[i]->activeTask->taskID
                      << " Status: " << getTaskStatusString(nodePool[i]->activeTask->status)
                      << " @ Node: " << nodePool[i]->nodeAddress
                      << " Progress: (" << nodePool[i]->activeTask->simulatedProgress
                      << "/" << nodePool[i]->activeTask->simulatedWorkUnits << " units).\n";
        }
    }

    if(currentHead == nullptr) {
        std::cout << "[STAT]: No Tasks are found in queue!\n";
    } else {
        task *head = currentHead;
        do {
            std::cout << "[PROG]: Task ID: " << head->taskID
                      << " Status: " << getTaskStatusString(head->status)
                      << " Progress: (" << head->simulatedProgress
                      << "/" << head->simulatedWorkUnits << " units).\n";
            if(head->next == nullptr)
                break;
            head = head->next;
        } while (head->next != nullptr);
    }
}

// shutdown function
bool dtkShutdown(task *taskQueueHead, node **nodePool) {
    std::cout << "[INFO]: Initiating DTK shutdown command ... \n";
    // clear all tasks
    cleanUpTaskQueue(taskQueueHead);
    if(isTaskQueueEmpty(taskQueueHead) == true)
        std::cout << "[INFO]: All tasks have been cleared from the system.\n";

    // empty all the nodes.
    for(int i = 0; i < MAX_NODES; i++) {
        std::cout << "[INFO]: Node ID: " << nodePool[i]->nodeID 
                  << " @ address: " << nodePool[i]->nodeAddress 
                  << " deletion in progress...\n";
        if(nodePool[i] != nullptr) {
            // clear the task that might have been left during execution
            if(nodePool[i]->activeTask != nullptr) {
                std::cout << "[INFO]: Task ID: " << nodePool[i]->activeTask->taskID
                          << " in progress, but deleting...\n";
                delete nodePool[i]->activeTask;
                nodePool[i]->activeTask = nullptr;
            }
            delete nodePool[i];
            nodePool[i] = nullptr;
        }
    }
    std::cout << "[INFO]: All resources deallocated. Shutting down.\n";
    return true;
}
