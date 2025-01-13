#include <iostream>
#include <pthread.h>

using namespace std;

int global = 0;
pthread_mutex_t lock;

void *function(void *arg)
{
    int num = *static_cast<int *>(arg); // Safely dereference the integer pointer
    pthread_mutex_lock(&lock);          // Lock the mutex

    global += num; // Critical section

    pthread_mutex_unlock(&lock); // Unlock the mutex
    pthread_exit(nullptr);
}

void *deletefunc(void *arg)
{
    int num = *static_cast<int *>(arg);
    pthread_mutex_lock(&lock);

    global -= num;

    pthread_mutex_unlock(&lock);
    pthread_exit(nullptr);
}

int main()
{
    pthread_t threads[2];
    int value = 20; // Value to pass to threads

    // Initialize the mutex and check for success
    if (pthread_mutex_init(&lock, nullptr) != 0)
    {
        cout << "Mutex not initialized" << endl;
        return 1;
    }

    // Create threads
    for (int i = 0; i < 2; i++)
    {
        if (rand() % 2 == 0)
        {
            pthread_create(&threads[i], nullptr, function, &value);
            cout<<"Global is "<<global<<endl;
        }
        else
        {
            pthread_create(&threads[i], nullptr, deletefunc, &value);
            cout<<"Global is "<<global<<endl;
        }
    }

    // Wait for threads to complete
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    // Print the result
    cout << "Global is " << global << endl;

    return 0;
}
