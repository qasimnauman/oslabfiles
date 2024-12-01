#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int frequency_nonSub[256] = {0};
int mean_nonSub = 0;
int frequency_sub[16] = {0};
int frequency_sub_changed[16] = {0};
int subMatrixSize = 4;
int matrixSize = 16;
struct Block
{
    int blocknumber;
    int startRow;
    int startCol;
    vector<vector<int>> *matrix;
    int frequency;
    int avg;
    bool meangreaterthan1;
};

/**
 * @brief Processes a submatrix block within a larger matrix.
 *
 * This function calculates the sum and average of elements in a submatrix block,
 * checks if the block's mean is greater than the difference of the global mean,
 * updates the frequency of the block before and after modification, and modifies
 * the submatrix based on certain conditions.
 *
 * @param arg Pointer to a Block structure containing information about the submatrix block.
 * @return void* Always returns NULL.
 *
 * The function performs the following steps:
 * 1. Locks a mutex to ensure thread safety.
 * 2. Calculates the sum and average of elements in the submatrix before modification.
 * 3. Checks if the block's mean is greater than the difference of the global mean.
 * 4. Updates the frequency of the block before modification.
 * 5. Modifies the submatrix based on the block number.
 * 6. Calculates the sum and average of elements in the submatrix after modification.
 * 7. Updates the frequency of the block after modification.
 * 8. Unlocks the mutex to release the lock.
 */
void *matrix_processing(void *arg)
{
    Block *block = (Block *)arg;
    int startRow = block->startRow;
    int startCol = block->startCol;
    vector<vector<int>> *matrix = block->matrix;
    int sum = 0;
    int totalelements = 16;

    pthread_mutex_lock(&mutex);

    // Calculate the sum of elements in the submatrix before modification
    for (int i = startRow; i < startRow + subMatrixSize; i++)
    {
        for (int j = startCol; j < startCol + subMatrixSize; j++)
        {
            sum += (*matrix)[i][j];
        }
    }
    block->avg = sum / totalelements;

    // Checking if block mean is greater than difference of the global mean
    block->meangreaterthan1 = abs(block->avg - mean_nonSub) > 1;
    // Upadate the frequency of the block before modification
    frequency_sub[block->avg]++;
    block->frequency = frequency_sub[block->avg];

    // Modify the submatrix

    // Calculate the relative row and column within the submatrix
    int relrow = block->blocknumber / subMatrixSize;
    int relcol = block->blocknumber % subMatrixSize;

    // Check if the relative row and column are within the submatrix bounds
    if (relrow < subMatrixSize && relcol < subMatrixSize)
    {
        // Modify the element in the submatrix to the global mean value
        (*matrix)[startRow + relrow][startCol + relcol - 1] = mean_nonSub;
    }

    // Calculate the sum of elements in the submatrix after modification

    for (int i = startRow; i < startRow + subMatrixSize; i++)
    {
        for (int j = startCol; j < startCol + subMatrixSize; j++)
        {
            sum += (*matrix)[i][j];
        }
    }
    block->avg = sum / totalelements;

    // Checking if modified block mean is greater than difference of the global mean
    block->meangreaterthan1 = abs(block->avg - mean_nonSub) > 1;

    // Update the frequency of the block after modification
    frequency_sub_changed[block->avg]++;
    block->frequency = frequency_sub_changed[block->avg];

    pthread_mutex_unlock(&mutex); // Release lock

    return NULL;
}

/**
 * @brief This function calculates the mean value for orignal matrix.
 *
 * @param matrix A constant reference to a 2D vector of integers.
 * @return double The mean value of all elements in the matrix.
 */
double calculateMean(const vector<vector<int>> &matrix)
{
    double sum = 0;
    int totalElements = matrix.size() * matrix[0].size();
    for (int i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            sum += matrix[i][j];
        }
    }

    return sum / totalElements;
}

// This funciton displays the block numebres with mean difference greater than one.
void *displayblockswithmean(void *arg)
{
    Block *block = (Block *)arg;
    cout << "Block No " << block->blocknumber << "\n";
    return NULL;
}

// This function is responsible for calculting the frequency for the orignal matrix
// It accepts the vector of integers as an argument and calculates the frequency of each element in the vector.

void *calculatefrequency_nonSub(void *arg)
{
    vector<int> *vec = (vector<int> *)arg;

    for (int num : *vec)
    {
        pthread_mutex_lock(&mutex);
        frequency_nonSub[num]++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main()
{
    // Reading the contents from the Matrix.txt file
    ifstream file("Matrix.txt");
    if (!file.is_open())
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;
    vector<vector<int>> orignal_matirx;

    while (getline(file, line))
    {
        stringstream ss(line);
        string numStr;
        vector<int> vec;

        while (getline(ss, numStr, ','))
        {
            vec.push_back(stoi(numStr));
        }

        orignal_matirx.push_back(vec);
    }

    file.close();

    // Calculating the size for the orignal matrix for no of threads
    int count = orignal_matirx.size();

    // Calculating the number of submatrices in the original matrix
    int numSubmatrices = (matrixSize / subMatrixSize) * (matrixSize / subMatrixSize);

    // Creating an array of Block structures to hold information for each submatrix
    Block blocks[numSubmatrices];

    // Creating an array of pthread_t to hold thread identifiers
    pthread_t threads[numSubmatrices];

    int threadIndex = 0;
    for (int i = 0; i < matrixSize; i += subMatrixSize)
    {
        for (int j = 0; j < matrixSize; j += subMatrixSize)
        {
            blocks[threadIndex] = {threadIndex + 1, i, j, &orignal_matirx}; // Initialize block
            // blocks[threadIndex] = {i, j, &orignal_matirx}; // Initialize block
            pthread_create(&threads[threadIndex], NULL, matrix_processing, (void *)&blocks[threadIndex]);
            threadIndex++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < threadIndex; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Calculate the mean value for the orignal matrix

    // Create an array of pthread_t to hold thread identifiers for frequency calculation
    pthread_t tid[count];

    // Create threads to calculate the frequency of each row in the original matrix
    for (size_t i = 0; i < count; i++)
    {
        pthread_create(&tid[i], NULL, calculatefrequency_nonSub, (void *)&orignal_matirx[i]);
    }

    // Wait for all frequency calculation threads to finish
    for (size_t i = 0; i < count; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Displaying the Block numbers with mean difference greated than one.
    cout << "Block numbers with mean difference greater than 1 are\n";
    for (int i = 0; i < numSubmatrices; i++)
    {
        if (blocks[i].meangreaterthan1)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, displayblockswithmean, (void *)&blocks[i]);
            pthread_join(tid, NULL);
        }
    }

    cout << "\nFrequency is displayed in the following format \n";
    cout << "Element(Index No) : Frequency\n";

    // Displaying the frequency for the orignal matrix
    cout << "\nFrequency for the orignal matrix\n";
    for (int i = 0; i < 256; i++)
    {
        cout << i << " : " << frequency_nonSub[i] << "\t";

        if (i % 16 == 15)
        {
            cout << endl;
        }
    }

    // Displaying the frequency for the submatrix
    cout << "\nFrequency for the submatrix\n";
    for (int i = 0; i < 256; i++)
    {
        cout << i << " : " << frequency_sub[i] << "\t";

        if (i % 16 == 15)
        {
            cout << endl;
        }
    }

    // Displaying the frequency for the submatrix after modification
    cout << "\nFrequency for the submatrix after modification\n";
    for (int i = 0; i < 256; i++)
    {
        cout << i << " : " << frequency_sub_changed[i] << "\t";

        if (i % 16 == 15)
        {
            cout << endl;
        }
    }

    pthread_mutex_destroy(&mutex);
    cout << "\n";
    return 0;
}
