#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
using namespace std;
int value =1;
int main(){
    int status = 1;
    pid_t pid = fork();

    if(pid > 0){
        waitpid(pid,NULL,WNOHANG);
        cout<<"Parent is Executing"<<endl;
        cout<<"STATUS: "<<status<<endl;
        cout<<value<<endl;
    }
    else if(pid == 0){
        cout << "Child is Executing" << endl;
        cout<<getpid()<<endl;
        cout<<getppid()<<endl;
        
        cout<<value<<endl;
        exit(0);
    }
    else {
        cout << "Forking Failed" << endl;
    }
    cout<<"Hello World"<<endl;
}