#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    int pipefd1[2], pipefd2[2];
    pid_t pid;
    char msg1[20] = "Hello from Parent!";
    char msg2[20] = "Hello from Child!";
    char readmsg[20];

    // Create the pipes
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1)
    {
        cout << "Pipes Creation Failed" << endl;
        return -1;
    }

    pid = fork();

    if (pid > 0) // Parent Process
    {
        close(pipefd1[0]); // Close unused read end of pipe 1
        close(pipefd2[1]); // Close unused write end of pipe 2

        cout << "Parent Writing to pipe 1: " << msg1 << endl;
        write(pipefd1[1], msg1, strlen(msg1) + 1); // Write to pipe 1

        read(pipefd2[0], readmsg, sizeof(readmsg)); // Read from pipe 2
        cout << "Parent Reading from pipe 2: " << readmsg << endl;

        close(pipefd1[1]); // Close write end of pipe 1
        close(pipefd2[0]); // Close read end of pipe 2
    }
    else if (pid == 0) // Child Process
    {
        close(pipefd1[1]); // Close unused write end of pipe 1
        close(pipefd2[0]); // Close unused read end of pipe 2

        read(pipefd1[0], readmsg, sizeof(readmsg)); // Read from pipe 1
        cout << "Child Reading from pipe 1: " << readmsg << endl;

        cout << "Child Writing to pipe 2: " << msg2 << endl;
        write(pipefd2[1], msg2, strlen(msg2) + 1); // Write to pipe 2

        close(pipefd1[0]); // Close read end of pipe 1
        close(pipefd2[1]); // Close write end of pipe 2
    }
    else // Fork failed
    {
        cout << "Fork Failed" << endl;
        return -1;
    }

    return 0;
}
