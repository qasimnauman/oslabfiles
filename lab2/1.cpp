#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
using namespace std;

int main()
{
    int status = 1;
    pid_t pid = fork();
    cout << pid << endl;
    if (pid > 0)
    {
        cout << "In Parent\n";
        cout << wait(&status) << endl;
        cout << "Status " << status << endl;
        cout << "Parent\n";
    }
    else if (pid==0){
        cout << "In Child\n";
        cout << getpid() << endl;
        cout << getppid() << endl;
        cout << "Child\n";
        exit(0);
    }
    else{
        cout << "\nHello World\n";
    }
}