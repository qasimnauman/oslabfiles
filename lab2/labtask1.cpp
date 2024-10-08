#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

using namespace std;

int main(){
    pid_t pid = fork();

    cout << "PID" << pid;
    if (pid==0)
    {
        cout << "Parent ID " << getppid() <<endl;
        exit(0);
    }
    else if (pid > 0){
        wait(NULL);
        cout << "Parent Process \n";
    }
}