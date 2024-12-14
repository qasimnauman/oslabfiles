#include <iostream>
#include <pthread.h>
#include <unistd.h> // for sleep function

using namespace std;

const int NUM_PHILOSOPHERS = 5;

// Mutexes for forks
pthread_mutex_t forks[NUM_PHILOSOPHERS];

// Philosopher function
void* philosopher(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Thinking
        cout << "Philosopher " << id << " is thinking." << endl;
        sleep(1);

        // Pick up forks
        int left_fork = id;
        int right_fork = (id + 1) % NUM_PHILOSOPHERS;

        // Deadlock prevention: Pick forks in order
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left_fork]);
            pthread_mutex_lock(&forks[right_fork]);
        } else {
            pthread_mutex_lock(&forks[right_fork]);
            pthread_mutex_lock(&forks[left_fork]);
        }

        // Eating
        cout << "Philosopher " << id << " is eating." << endl;
        sleep(1);

        // Put down forks
        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);
    }

    return nullptr;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&forks[i], nullptr);
        ids[i] = i;
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_create(&philosophers[i], nullptr, philosopher, &ids[i]);
    }

    // Join threads (not needed as philosophers run infinitely)
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], nullptr);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
