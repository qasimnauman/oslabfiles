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
    char const *data = "Hello this is written to pipe";
    int x = pipe(fd);
    if (x == -1)
    {
        cout << "pipe failed " << endl;
        exit(1);
    }
    else
    {
        int y = pipe(fd1);
        pid_t pid = fork();
        if (pid == 0)
        {
            close(fd[0]);
            close(fd1[1]);
            read(fd1[0], buf, strlen(buf));
            write(fd[1], data, strlen(data));
        }
        else
        {
            close(fd[1]);
            close(fd1[0]);
            write(fd1[1], data, strlen(data));
            if ((n = read(fd[0], buf, 1024)) >= 0)
            {
                buf[n] = 0;
                cout << "\nRead " << n << " bytes from pipe\n" << buf << endl;
            }
            return 0;
        }
    }
}