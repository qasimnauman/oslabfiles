#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main()
{
    int i, status;
    for (int i = 0; i < 3; i++)
    {
        if (!fork())
        {
            cout << "Hellow " << i << " PID: " << getpid() << endl;
            exit(0);
        }
        else
        {
            cout << "Goodbye " << i << " PID: " << getpid() << endl;
        }
        wait(&status);
    }
}