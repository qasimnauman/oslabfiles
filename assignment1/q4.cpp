#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <cmath>

using namespace std;

bool isPrime(int num) {
    if (num < 2) return false;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main() {
    int startRange, endRange, numProcesses;

    cout << "Enter the start of the range: ";
    cin >> startRange;
    cout << "Enter the end of the range: ";
    cin >> endRange;
    cout << "Enter the number of processes to use: ";
    cin >> numProcesses;

    int totalRange = endRange - startRange + 1;
    int chunkSize = totalRange / numProcesses;
    int remaining = totalRange % numProcesses;

    int shmId = shmget(IPC_PRIVATE, numProcesses * sizeof(int), IPC_CREAT | 0666);
    if (shmId < 0) {
        cerr << "Error creating shared memory!" << endl;
        return 1;
    }

    int *primeCounts = (int *)shmat(shmId, nullptr, 0);

    for (int i = 0; i < numProcesses; ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            int chunkStart = startRange + i * chunkSize;
            int chunkEnd = chunkStart + chunkSize - 1;

            if (i == numProcesses - 1) {
                chunkEnd += remaining;
            }

            int count = 0;
            for (int num = chunkStart; num <= chunkEnd; ++num) {
                if (isPrime(num)) {
                    count++;
                }
            }

            primeCounts[i] = count;

            shmdt(primeCounts);
            exit(0);
        }
    }

    for (int i = 0; i < numProcesses; ++i) {
        wait(nullptr);
    }

    int totalPrimeCount = 0;
    for (int i = 0; i < numProcesses; ++i) {
        totalPrimeCount += primeCounts[i];
    }

    cout << "Total prime numbers in the range " << startRange << " to " << endRange << " is: " << totalPrimeCount << endl;

    shmdt(primeCounts);
    shmctl(shmId, IPC_RMID, nullptr);

    return 0;
}
