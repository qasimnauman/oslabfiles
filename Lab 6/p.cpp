#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main()
{
    if (fork() & fork())
    {
        if (fork() || fork())
        {
            fork();
            cout << "\n M1\n";
        }
    }
    cout << "\n M2\n";
}