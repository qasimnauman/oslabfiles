#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    int n = 221345 % 58;
    int n1 = 221345 % (3 + 4 + 5);
    int n2 = 221345 % (4 + 5);

    cout << "\nSuper Parent Process started with PID: " << getpid() << endl;

    for (int i = 0; i <= n; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            if (i == n1) {
                cout << "Child process " << i << " (PID: " << getpid() << ") will become a zombie." << endl;
                exit(0);
            } else if (i == n2) {
                cout << "Child process " << i << " (PID: " << getpid() << ") will become an orphan." << endl;
                sleep(5);
                cout << "Orphan process adopted by init process (PID 1), PID: " << getpid() << endl;
                exit(0);
            } else {
                cout << "Child process " << i << " (PID: " << getpid() << ") running normally." << endl;
                sleep(2);
                exit(0);
            }
        } else if (pid > 0) {
            if (i == n1) {
                cout << "Super Parent Process: Not waiting for child process " << i << " (PID: " << pid << ") to create zombie." << endl;
            } else if (i == n2) {
                cout << "Super Parent Process exiting to create orphan process.\n";
                exit(0);
            } else {
                wait(NULL);
            }
        } else {
            cerr << "Fork failed!" << endl;
            return 1;
        }
    }

    cout << "Super Parent Process is waiting for the zombie process to terminate..." << endl;
    wait(NULL);
    cout << "Super Parent Process terminated." << endl;

    return 0;
}
