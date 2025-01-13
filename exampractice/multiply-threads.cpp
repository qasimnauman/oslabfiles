#include <iostream>
#include <pthread.h>
#include <vector>

#define NUM_THREADS 2

using namespace std;

// Define matrix dimensions
const int ROWS_A = 4;
const int COLS_A = 4;
const int ROWS_B = 4;
const int COLS_B = 4;

// Matrices
int A[ROWS_A][COLS_A] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}};

int B[ROWS_B][COLS_B] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}};

int C[ROWS_A][COLS_B] = {0}; // Result matrix

// Structure to hold thread data
struct ThreadData {
    int thread_id;
    int start_row;
    int end_row;
};

// Thread function
void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < COLS_B; ++j) {
            for (int k = 0; k < COLS_A; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(nullptr);
}

int main() {
    if (COLS_A != ROWS_B) {
        cout << "Matrix multiplication not possible with given dimensions." << endl;
        return -1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int rows_per_thread = ROWS_A / NUM_THREADS;

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == NUM_THREADS - 1) ? ROWS_A : (i + 1) * rows_per_thread;

        if (pthread_create(&threads[i], nullptr, multiply, &thread_data[i]) != 0) {
            cout << "Error creating thread " << i << endl;
            return -1;
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Print the result matrix
    cout << "Resulting matrix C:" << endl;
    for (int i = 0; i < ROWS_A; ++i) {
        for (int j = 0; j < COLS_B; ++j) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
