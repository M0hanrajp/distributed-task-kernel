#include "dtk_kernel.hpp"
#include <iostream>

/* @breif: Adds a newTask to the end of the linked list 
 * if head is not empty 
 * @head: The current head of the task queue.
 * @newTask: The dynamically allocated task object to be added.
 * @Return: task* - The (potentially new) head of the task queue. 
 * This is important because if the queue was empty, head will 
 * change to newTask. original head is returned, so the head always
 * points to the first task in main().
 * */
task* enqueueTask(task* head, task* newTask) {
    std::cout << "[INFO]: Task enqueue in progress...\n";

    // make current task's next link as NULL
    newTask->next = nullptr;

    // then link the current task to last task or return it 
    if(head == nullptr)
        return newTask;
    else {
        task *current = head;
        while(current->next != nullptr)
            current = current->next;
        current->next = newTask;
    }
    return head;
}

/* @breif Removes the front task and returns the new head of the queue.
 * @currentHead: The current head of the task queue.
 * @Return: task* - A pointer to the new head of the queue after the 
 * operation. Returns nullptr if the queue becomes empty.
 */
task* dequeueTask(task* currentHead) {
    // Corrected log to show which task is being dequeued
    if (currentHead != nullptr) {
        std::cout << "[INFO]: Task ID " << currentHead->taskID << " dequeued from the list!\n";
    } else {
        std::cout << "[INFO]: Attempted to dequeue from empty list.\n";
    }

    if(currentHead == nullptr)
        return nullptr;
    task *taskToReturn = currentHead;
    task *newHead = currentHead->next;

    taskToReturn->next = nullptr; // Detach the dequeued task
    return newHead;
}

/* @breif Returns the task from the front of the linked list without removing it.
 * @head: The current head of the task queue.
 * @Return: task* - A pointer to the first task, or nullptr if the queue is empty.
 */
task* peekTask(task* head) {
    return head;
}

/* @breif Checks if the task queue is empty.
 * @head: The current head of the task queue.
 * @return: Return: bool - true if head is nullptr, false otherwise.
 */
bool isTaskQueueEmpty(task* head) {
    return (head == nullptr);
}

/* @breif Frees all dynamically allocated task objects remaining in the queue. 
 * This will be used during shutdown.
 * @head: The current head of the task queue.
 */
void cleanUpTaskQueue(task* head) {
    task *current = head;
    task *nextTask = nullptr;

    while (current != nullptr) {
        std::cout << "[INFO]: Task ID: " << current->taskID << " is deleted..\n";
        nextTask = current->next;
        delete current;
        current = nextTask;
    }

    std::cout << "[INFO]: All Tasks deleted !\n";
}
