#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    const int childs = 2;
    pid_t pids[childs];
    bool isChildActive[childs] = {true, true};

    for (int i = 0; i < childs; ++i) {
        pids[i] = fork();

        if (pids[i] < 0) {
            cerr << "Fork failed!" << endl;
            exit(1);
        }

        if (pids[i] == 0) {
            cout << "Child " << i + 1 << " started with PID " << getpid() << "." << endl;
            sleep(2 + i);
            exit(i);
        }
    }

    int childrenRemaining = childs;
    while (childrenRemaining > 0) {
        for (int i = 0; i < childs; ++i) {
            if (isChildActive[i]) {
                int status;
                pid_t result = waitpid(pids[i], &status, WNOHANG);

                if (result == 0) {
                    cout << "Child " << i + 1 << " is still running." << endl;
                } else if (result > 0) {
                    if (WIFEXITED(status)) {
                        cout << "Child " << i + 1 << " with PID " << pids[i] 
                             << " exited with status: " << WEXITSTATUS(status) << endl;
                        isChildActive[i] = false;
                        --childrenRemaining;
                    }
                }
            }
        }
        cout << "Parent is doing other work..." << endl;
        sleep(1);
    }

    cout << "All children have terminated. Parent process exiting." << endl;
    return 0;
}
