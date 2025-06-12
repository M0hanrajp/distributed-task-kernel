#include "dtk_kernel.hpp"
#include <iostream>
#include <ostream>

#define CLEANUPTASKQUEUE 0

int main(void) {

    task *taskQueueHead = nullptr;

        // --- Start of Task Queue Testing Block ---
    std::cout << "\n--- Starting Task Queue Functional Test ---\n";

    // 3.1. Verify initial empty state
    std::cout << "Is queue empty initially? " << (isTaskQueueEmpty(taskQueueHead) ? "Yes" : "No") << std::endl;
    std::cout << "Peek at empty queue: " << peekTask(taskQueueHead) << std::endl; // Should print 0 (nullptr)

    // 3.2. Enqueue a few tasks
    std::cout << "\nEnqueuing 3 tasks...\n";
    task* task1 = new task;
    task1->taskID = 101;
    task1->task = JOB_A; // Assuming JOB_A is defined in your enum
    task1->inputData = "data_A";
    task1->status = PENDING;
    task1->resultData = "";
    taskQueueHead = enqueueTask(taskQueueHead, task1);
    std::cout << "Enqueued Task ID: " << task1->taskID << std::endl;

    task* task2 = new task;
    task2->taskID = 102;
    task2->task = JOB_B; // Assuming JOB_B
    task2->inputData = "data_B";
    task2->status = PENDING;
    task2->resultData = "";
    taskQueueHead = enqueueTask(taskQueueHead, task2);
    std::cout << "Enqueued Task ID: " << task2->taskID << std::endl;

    task* task3 = new task;
    task3->taskID = 103;
    task3->task = JOB_C; // Assuming JOB_C
    task3->inputData = "data_C";
    task3->status = PENDING;
    task3->resultData = "";
    taskQueueHead = enqueueTask(taskQueueHead, task3);
    std::cout << "Enqueued Task ID: " << task3->taskID << std::endl;

    std::cout << "\nIs queue empty after enqueuing? " << (isTaskQueueEmpty(taskQueueHead) ? "Yes" : "No") << std::endl;
    std::cout << "Peek at head after enqueuing: Task ID " << peekTask(taskQueueHead)->taskID << std::endl; // Should be 101

    // 3.2.5 Delete all the tasks using cleanUpTaskQueue
    // using ! operator in the if statement to execute code inside if block
    // by default else is made to execute in program
    if(!CLEANUPTASKQUEUE) {
        std::cout << "Deleting all tasks using cleanUpTaskQueue ..." << std::endl;
        cleanUpTaskQueue(taskQueueHead);
    } else {

        // 3.3. Dequeue tasks and verify order
        std::cout << "\nDequeuing tasks...\n";

        task* taskToProcess = taskQueueHead; // Save the old head
        while (taskQueueHead != nullptr) {
            if(taskQueueHead != NULL)
                std::cout << "Peek at head after first dequeue: Task ID " 
                          << peekTask(taskQueueHead)->taskID << std::endl; // Should be 102
            taskToProcess = taskQueueHead; // Save the old head
            taskQueueHead = dequeueTask(taskQueueHead); // update global head
            std::cout << "Dequeued Task ID: " << taskToProcess->taskID << std::endl;
            delete taskToProcess; // Free memory for task 
        }

        std::cout << "\nIs queue empty after all dequeues? " << (isTaskQueueEmpty(taskQueueHead) ? "Yes" : "No") << std::endl;
        std::cout << "Peek at head after all dequeues: " << peekTask(taskQueueHead) << std::endl; // Should print 0 (nullptr)

        // 3.4. Test dequeuing from an empty queue
        std::cout << "\nAttempting to dequeue from an empty queue...\n";
        // can be done safely instead of using taskQueueHead
        taskQueueHead = dequeueTask(taskQueueHead);
        if (taskQueueHead == nullptr) { // Check if the function returned nullptr
            std::cout << "Successfully handled dequeue from empty queue.\n";
        }
        std::cout << "--- End of Task Queue Functional Test ---\n\n";
        // --- End of Task Queue Testing Block ---
    }
    return 0;
}
