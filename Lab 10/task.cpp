// #include <iostream>
// #define N 9
// using namespace std;
// int grid[N][N] = {
//     {3, 0, 6, 5, 0, 8, 4, 0, 0},
//     {5, 2, 0, 0, 0, 0, 0, 0, 0},
//     {0, 8, 7, 0, 0, 0, 0, 3, 1},
//     {0, 0, 3, 0, 1, 0, 0, 8, 0},
//     {9, 0, 0, 8, 6, 3, 0, 0, 5},
//     {0, 5, 0, 0, 9, 0, 6, 0, 0},
//     {1, 3, 0, 0, 0, 0, 2, 5, 0},
//     {0, 0, 0, 0, 0, 0, 0, 7, 4},
//     {0, 0, 5, 2, 0, 6, 3, 0, 0}};
// bool isPresentInCol(int col, int num)
// { // check whether num is present in col or not
//    for (int row = 0; row < N; row++)
//       if (grid[row][col] == num)
//          return true;
//    return false;
// }
// bool isPresentInRow(int row, int num)
// { // check whether num is present in row or not
//    for (int col = 0; col < N; col++)
//       if (grid[row][col] == num)
//          return true;
//    return false;
// }
// bool isPresentInBox(int boxStartRow, int boxStartCol, int num)
// {
//    // check whether num is present in 3x3 box or not
//    for (int row = 0; row < 3; row++)
//       for (int col = 0; col < 3; col++)
//          if (grid[row + boxStartRow][col + boxStartCol] == num)
//             return true;
//    return false;
// }
// void sudokuGrid()
// { // print the sudoku grid after solve
//    for (int row = 0; row < N; row++)
//    {
//       for (int col = 0; col < N; col++)
//       {
//          if (col == 3 || col == 6)
//             cout << " | ";
//          cout << grid[row][col] << " ";
//       }
//       if (row == 2 || row == 5)
//       {
//          cout << endl;
//          for (int i = 0; i < N; i++)
//             cout << "---";
//       }
//       cout << endl;
//    }
// }
// bool findEmptyPlace(int &row, int &col)
// { // get empty location and update row and column
//    for (row = 0; row < N; row++)
//       for (col = 0; col < N; col++)
//          if (grid[row][col] == 0) // marked with 0 is empty
//             return true;
//    return false;
// }
// bool isValidPlace(int row, int col, int num)
// {
//    // when item not found in col, row and current 3x3 box
//    return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3, col - col % 3, num);
// }
// bool solveSudoku()
// {
//    int row, col;
//    if (!findEmptyPlace(row, col))
//       return true; // when all places are filled
//    for (int num = 1; num <= 9; num++)
//    { // valid numbers are 1 - 9
//       if (isValidPlace(row, col, num))
//       { // check validation, if yes, put the number in the grid
//          grid[row][col] = num;
//          if (solveSudoku()) // recursively go for other rooms in the grid
//             return true;
//          grid[row][col] = 0; // turn to unassigned space when conditions are not satisfied
//       }
//    }
//    return false;
// }
// int main()
// {
//    if (solveSudoku() == true)
//       sudokuGrid();
//    else
//       cout << "No solution exists";
// }

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
