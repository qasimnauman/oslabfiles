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
    char input[1025];

    cout << "Enter the string to write to pipe: ";
    cin.getline(input, 1025);

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
            while (true)
            {
                read(fd1[0], buf, 1024);
                if (buf == "bye!")
                {
                    break;
                }
                write(fd[1], buf, strlen(buf));
            }
            close(fd[1]);
            close(fd1[0]);
        }
        else
        {
            close(fd[1]);
            close(fd1[0]);
            while (input != "bye")
            {
                write(fd1[1], input, strlen(input));
                if ((n = read(fd[0], buf, 1024)) >= 0)
                {
                    buf[n] = 0;
                    cout << "\nRead " << n << " bytes from pipe\n"
                         << buf << endl;
                }
                cout << "Enter the string to write to pipe: ";
                cin.getline(input, 1025);
            }
        }
    }
    return 0;
}
