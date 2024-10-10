#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib> 

using namespace std;

int main() {
    const int childs = 2;
    pid_t pids[childs];

    for (int i = 0; i < childs; ++i) {
        pids[i] = fork();

        if (pids[i] < 0) {
            cout << "Fork failed!" << endl;
            exit(1);
        }

        if (pids[i] == 0) {
            cout << "Child " << i + 1 << " is running!" << endl;

            if (i == 0) {
                cout << "Child 1 exits with success (code 0)." << endl;
                exit(0);
            } else if (i == 1) {
                cout << "Child 2 exits with failure (code 1)." << endl;
                exit(1);
            }
        }
    }

    for (int i = 0; i < childs; ++i) {
        int status;
        pid_t pid = waitpid(pids[i], &status, 0);

        if (WIFEXITED(status)) {
            cout << "Child with PID " << pid << " exited with status: " << WEXITSTATUS(status) << endl;
        }
    }

    return 0;
}
