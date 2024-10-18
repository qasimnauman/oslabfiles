#include <iostream>
#include <unistd.h>     // For fork(), sleep()
#include <sys/wait.h>   // For wait()
#include <cstdlib>      // For rand() and srand()
#include <ctime>        // For time()

#define MAX_RESOURCES 5  // Maximum number of resources
#define NUM_CHILDREN 3   // Number of child processes

int available_resources = MAX_RESOURCES;

// Function for resource allocation
int allocate_resources(int count) {
    if (available_resources < count) {
        std::cout << "Not enough resources available for allocation.\n";
        return -1;
    }
    available_resources -= count;
    std::cout << "Allocated " << count << " resources, " << available_resources << " remaining.\n";
    return 0;
}

// Function for resource deallocation
void release_resources(int count) {
    available_resources += count;
    std::cout << "Released " << count << " resources, " << available_resources << " remaining.\n";
}

// Function for child processes to simulate resource allocation
void child_process(int id) {
    srand(time(0) + id);  // Seed the random number generator with a different seed for each child
    int request = rand() % MAX_RESOURCES + 1;  // Randomly request 1 to MAX_RESOURCES resources

    std::cout << "Child " << id << " requesting " << request << " resources.\n";
    exit(request);  // Return the number of resources requested as exit status
}

int main() {
    std::cout << "Parent starting with " << MAX_RESOURCES << " available resources.\n";

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "Fork failed!\n";
            return 1;
        } else if (pid == 0) {
            // This is the child process
            child_process(i + 1);
        }
    }

    // Parent process: manage allocation and deallocation
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        int status;
        pid_t child_pid = wait(&status);  // Wait for any child process to finish

        if (WIFEXITED(status)) {
            int requested_resources = WEXITSTATUS(status);  // Get the resources requested by the child
            std::cout << "Child process " << child_pid << " finished, requested " << requested_resources << " resources.\n";

            // Try to allocate resources for the next process
            if (allocate_resources(requested_resources) == -1) {
                std::cout << "Resource allocation failed for child " << child_pid << ".\n";
            }

            // Simulate some work by the child and release the resources
            sleep(1);  // Simulate work done by the child process
            release_resources(requested_resources);
        }
    }

    std::cout << "All children finished, final available resources: " << available_resources << ".\n";
    return 0;
}
