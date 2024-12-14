#include <iostream>
#include <pthread.h>
#include <cstring>
#include <unistd.h>

using namespace std;

pthread_t tid[2];      // Array to store thread IDs
int counter = 0;       // Shared counter
pthread_mutex_t lock;  // Mutex for critical section

void* trythis(void* arg) {
    pthread_mutex_lock(&lock); // Lock the mutex

    unsigned long i = 0;
    counter += 2;
    cout << "\n Job " << counter << " has started\n";

    cout << "\n Job " << counter << " has finished\n";

    pthread_mutex_unlock(&lock); // Unlock the mutex

    return nullptr;
}

int main() {
    int i = 0;
    int error;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, nullptr) != 0) {
        cerr << "\n Mutex init has failed\n";
        return 1;
    }

    // Create threads
    while (i < 2) {
        error = pthread_create(&(tid[i]), nullptr, &trythis, nullptr);
        if (error != 0) {
            cerr << "\n Thread can't be created: " << strerror(error) << "\n";
        }
        i++;
    }

    // Wait for threads to finish
    for (int j = 0; j < 2; j++) {
        pthread_join(tid[j], nullptr);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
