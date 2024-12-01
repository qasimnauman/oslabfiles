#include <iostream>
#include <pthread.h>
#include <vector>

#define MATRIX_SIZE 16
#define SUBMATRIX_SIZE 4
#define NUM_THREADS 16

// Matrix to process
std::vector<std::vector<int>> matrix(MATRIX_SIZE, std::vector<int>(MATRIX_SIZE, 1));

// Structure to pass data to threads
struct ThreadData {
    int startRow;
    int startCol;
    int threadId;
};

// Function to process a submatrix
void* processSubMatrix(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    int startRow = data->startRow;
    int startCol = data->startCol;
    int threadId = data->threadId;

    std::cout << "Thread " << threadId << " processing submatrix starting at (" 
              << startRow << ", " << startCol << ")" << std::endl;

    // Process the 4x4 submatrix
    for (int i = startRow; i < startRow + SUBMATRIX_SIZE; ++i) {
        for (int j = startCol; j < startCol + SUBMATRIX_SIZE; ++j) {
            // Perform some processing (e.g., increment each element)
            matrix[i][j] += threadId;
        }
    }

    pthread_exit(nullptr);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    int threadIndex = 0;

    // Divide the matrix into 4x4 submatrices
    for (int i = 0; i < MATRIX_SIZE; i += SUBMATRIX_SIZE) {
        for (int j = 0; j < MATRIX_SIZE; j += SUBMATRIX_SIZE) {
            threadData[threadIndex].startRow = i;
            threadData[threadIndex].startCol = j;
            threadData[threadIndex].threadId = threadIndex;

            // Create a thread for each submatrix
            if (pthread_create(&threads[threadIndex], nullptr, processSubMatrix, 
                               (void*)&threadData[threadIndex]) != 0) {
                std::cerr << "Error creating thread " << threadIndex << std::endl;
                return 1;
            }

            threadIndex++;
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Output the processed matrix
    std::cout << "\nProcessed Matrix:" << std::endl;
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
