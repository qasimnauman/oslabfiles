#include <iostream>
#include <pthread.h>
#include <vector>
#include <memory>
#include <cstdint> // For intptr_t

using namespace std;

// Structure for thread parameters
struct Parameters {
    int row;            // Starting row
    int col;            // Starting column
    const int (*board)[9]; // Pointer to the Sudoku board
};

// Function prototypes
void* validateRows(void* arg);
void* validateCols(void* arg);
void* validateSquare(void* arg);

int main() {
    // ====== Create the Sudoku board ======
    const int board[9][9] = {  
        {6, 2, 4, 5, 3, 9, 1, 8, 7},
        {5, 1, 9, 7, 2, 8, 6, 3, 4},
        {8, 3, 7, 6, 1, 4, 2, 9, 5},
        {1, 4, 3, 8, 6, 5, 7, 2, 9},
        {9, 5, 8, 2, 4, 7, 3, 6, 1},
        {7, 6, 2, 3, 9, 1, 4, 5, 8},
        {3, 7, 1, 9, 5, 6, 8, 4, 2},
        {4, 9, 6, 1, 8, 2, 5, 7, 3},
        {2, 8, 5, 4, 7, 3, 9, 1, 6}
    };

    // Parameters for rows, columns, and squares
    Parameters rowParams = {0, 0, board};
    Parameters colParams = {0, 0, board};
    vector<Parameters> squareParams;

    // Initialize parameters for 3x3 squares
    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            squareParams.push_back({row, col, board});
        }
    }

    // ====== Create pthreads ======
    pthread_t threadRows, threadCols;
    vector<pthread_t> squareThreads(9);

    // Results from threads
    void* rowResult;
    void* colResult;
    vector<void*> squareResults(9);

    // Create threads for rows and columns
    pthread_create(&threadRows, nullptr, validateRows, &rowParams);
    pthread_create(&threadCols, nullptr, validateCols, &colParams);

    // Create threads for 3x3 squares
    for (int i = 0; i < 9; ++i) {
        pthread_create(&squareThreads[i], nullptr, validateSquare, &squareParams[i]);
    }

    // ====== Wait for threads to complete ======
    pthread_join(threadRows, &rowResult);
    pthread_join(threadCols, &colResult);
    for (int i = 0; i < 9; ++i) {
        pthread_join(squareThreads[i], &squareResults[i]);
    }

    // ====== Check results ======
    bool isValid = (reinterpret_cast<intptr_t>(rowResult) == 1) &&
                   (reinterpret_cast<intptr_t>(colResult) == 1);

    for (int i = 0; i < 9 && isValid; ++i) {
        isValid &= (reinterpret_cast<intptr_t>(squareResults[i]) == 1);
    }

    // Output the result
    if (isValid) {
        cout << "The Sudoku Puzzle is valid and correctly solved!\n";
    } else {
        cout << "The Sudoku Puzzle is not valid and incorrect.\n";
    }

    return 0;
}

/**
 * Validates that all rows contain unique numbers 1-9.
 */
void* validateRows(void* arg) {
    Parameters* params = static_cast<Parameters*>(arg);
    const int (*board)[9] = params->board;

    for (int i = 0; i < 9; ++i) {
        int seen[10] = {0};
        for (int j = 0; j < 9; ++j) {
            int num = board[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                return reinterpret_cast<void*>(0); // Invalid row
            }
            seen[num] = 1;
        }
    }
    return reinterpret_cast<void*>(1); // Valid rows
}

/**
 * Validates that all columns contain unique numbers 1-9.
 */
void* validateCols(void* arg) {
    Parameters* params = static_cast<Parameters*>(arg);
    const int (*board)[9] = params->board;

    for (int i = 0; i < 9; ++i) {
        int seen[10] = {0};
        for (int j = 0; j < 9; ++j) {
            int num = board[j][i];
            if (num < 1 || num > 9 || seen[num]) {
                return reinterpret_cast<void*>(0); // Invalid column
            }
            seen[num] = 1;
        }
    }
    return reinterpret_cast<void*>(1); // Valid columns
}

/**
 * Validates that a 3x3 square contains unique numbers 1-9.
 */
void* validateSquare(void* arg) {
    Parameters* params = static_cast<Parameters*>(arg);
    const int (*board)[9] = params->board;

    int startRow = params->row;
    int startCol = params->col;
    int seen[10] = {0};

    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            int num = board[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                return reinterpret_cast<void*>(0); // Invalid square
            }
            seen[num] = 1;
        }
    }
    return reinterpret_cast<void*>(1); // Valid square
}