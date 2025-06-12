#include "dtk_kernel.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>

task *taskQueueHead = nullptr;

int main(void) {

    /* a server rack
     * complete set of worker nodes 
     * kernel is aware of this and 
     * is able to dispatch tasks*/
    node *nodePool[MAX_NODES];
    for(int i = 0; i < MAX_NODES; i++) {
         nodePool[i] = new node;
         if(nodePool[i] == nullptr)
             std::cout << "[ERROR]: Memory allocation failed\n";

         nodePool[i]->nodeID = i;
         nodePool[i]->status = IDLE;
         nodePool[i]->isResponsive = true;
         nodePool[i]->nodeAddress = "192.168.1.1" + std::to_string(i);
         nodePool[i]->activeTask = nullptr;
    }

    bool unsupportedJobType = false;
    bool isShutdownNeeded = true;
    static int newTaskID = 1;
    static int simulatedWorkUnits = 5;

    const char *userName = getenv("USER");
    const std::string GREEN = "\033[1;32m";
    const std::string RESET = "\033[0m";

    // program main loop
    while (true) {
        std::cout << GREEN << "DTK-" << userName << " $ " << RESET;
        /* DTK commands:
         * 1. submit <TaskType> <InputData> [ submit a job ]
         * 2. shutdown [shutdown DTK]
         */
        std::string userInput;
        std::getline(std::cin, userInput);

        std::stringstream ss(userInput);
        std::string command;
        // extract the first word (the command)
        ss >> command;

        // shutdown command
        if(command == "shutdown") {
            if(dtkShutdown(taskQueueHead, nodePool)) {
                // if user does not close the program gracefully, main() will handle the shutdown
                isShutdownNeeded = false;
            } else {
                std::cout << "[ERROR]: Shutdown not successful\n";
            }
        // submitting a job to the task handler
        } else if(command == "submit") {
            std::string taskTypeStr;
            std::string inputDataStr;
            ss >> taskTypeStr >> inputDataStr;

            if (taskTypeStr.empty() || inputDataStr.empty()) {
                std::cout << "[ERROR]: Invalid submit command format."
                             "Usage: submit <TaskType> <InputData>\n";
                continue; // Skip to the next loop iteration
            }

            task *createNewTask = new (std::nothrow) task;
            if (createNewTask == nullptr) {
                std::cout << "[ERROR]: Failed to allocate memory for new task."
                             "System might be out of resources.\n";
                continue; // Skip to the next loop iteration
            }

            createNewTask->taskID = newTaskID;
            createNewTask->status = PENDING;
            createNewTask->inputData = inputDataStr;
            createNewTask->resultData = "";
            /* amount of units required to complete the task
             * this variable is purely to simulate completion
             * of a task
             * check dtk_kernel.cpp on how task is finished
             */
            createNewTask->simulatedWorkUnits = simulatedWorkUnits++;
            // the progress for the task is made 0
            createNewTask->simulatedProgress  = 0;

            // set task type
            switch (taskTypeStr[4]) {
                case 'A': createNewTask->task = JOB_A; break;
                case 'B': createNewTask->task = JOB_B; break;
                case 'C': createNewTask->task = JOB_C; break;
                case 'D': createNewTask->task = JOB_D; break;
                default : std::cout << "[ERROR]: Unknown TaskType: " << taskTypeStr
                                    << ". Supported: JOB_A, JOB_B, JOB_C, JOB_D.\n";
                          unsupportedJobType  = true;  break;
            }

            if(unsupportedJobType) {
                delete createNewTask;
                createNewTask = nullptr;
                unsupportedJobType = false;
                continue;
            }

            // Add the new task to the queue 
            taskQueueHead = enqueueTask(taskQueueHead, createNewTask);
            std::cout << "[INFO]: Task ID " << createNewTask->taskID
                      << " (" << taskTypeStr << ") submitted and queued.\n";
            newTaskID++; // Increment ID only after successful enqueue

            // when task is created (success) call scheduler
            /* the dtkScheduler acts as task simulator simulatenously
             * the values simulatedProgress & simulatedWorkUnits are 
             * controlled when NODE is busy.
             *
             * simulatedProgress is incremented based on availablility
             * of a node so that tasks is completed and NODE is made
             * free to take up the next tasks
             */
            taskQueueHead = dtkScheduler(taskQueueHead, nodePool);

        } else if(command == "status") {
            /* status will take the taskQueueHead and nodePool
             * to get the data of nodes and tasks that are currently
             * running, offline, busy
             */
            dtkStatus(taskQueueHead, nodePool);

        } else if(command == "help") {
            // get information on all commands
            std::cout << "[INFO]: submit <job-type> <input-data>, submit a job with valid input data\n"; 
            std::cout << "[INFO]: continue <no-params>, moves progress of a task by x units\n";
            std::cout << "[INFO]: status <no-params>, status of current nodes their tasks\n";
            std::cout << "[INFO]: shutdown <no-params>, delete all nodes and tasks assigned\n";
            std::cout << "[INFO]: exit <no-params>, exit DTK program\n";

        } else if(command == "continue") {
            /* Since the scheduler acts as task simulator, calling the
             * function below simulates the task by some %, there is no
             * change in task, it only moves the task progress ahead by 
             * % progress units, continue command can be used to simulate 
             * the progress of a task till node becomes free to take up next
             * task that is present in the queue.
             */
            taskQueueHead = dtkScheduler(taskQueueHead, nodePool);
        // exit the DTK CLI
        } else if(command == "exit") {
            std::cout << "[INFO]: DTK program exit in progress..\n";
            break;
        // for invalid command entered
        } else {
            std::cout << "[ERROR]: Unknown command: " << command
                      << ". enter $ help for more info!\n";
        }
    }

    if(isShutdownNeeded) {
        dtkShutdown(taskQueueHead, nodePool);
    } else {
        std::cout << "[INFO]: All tasks have been deleted and nodes are IDLE, shutting down...\n";
    }

    return 0;
}
