#include <iostream>
#include <unistd.h>
#include <string.h>
using namespace std;

int main()
{
    int n;
    int fd[2];
    int fd1[2];
    char buf[1025];
    char data[1025] = "Hello this is written to pipe";

    if (pipe(fd) == -1 || pipe(fd1) == -1)
    {
        cout << "Pipe creation failed" << endl;
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        close(fd[0]);  // Close read end of fd
        close(fd1[1]); // Close write end of fd1

        read(fd1[0], buf, 1024); // Read data sent by
        cout << "Enter data to be written back to parent : ";
        cin.getline(data, 1024);
        write(fd[1], data, strlen(data)); // Write data back to parent
    }
    else // Parent process
    {
        close(fd[1]);  // Close write end of fd
        close(fd1[0]); // Close read end of fd1

        write(fd1[1], data, strlen(data));     // Write data to child
        if ((n = read(fd[0], buf, 1024)) >= 0) // Read data from child
        {
            buf[n] = 0; // Null-terminate buffer
            cout << "\nRead " << n << " bytes from pipe\n"
                 << buf << endl;
        }
    }
    return 0;
}
