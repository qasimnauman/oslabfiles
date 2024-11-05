#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
using namespace std;

int main()
{
    if ((!fork() && fork()) || !fork())
    {
        cout << "hello" << endl;
        fork();
        if (!fork() && fork())
        {
            cout << "bye" << endl;
        }
        if ((fork() || !fork()) && fork())
        {
            fork();
            cout << "bye" << endl;
        }
    }
}