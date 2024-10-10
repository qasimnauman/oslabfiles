#include <iostream>
#include <unistd.h>

using namespace std;

int main(){
    int superparentprocessid = getpid();

    pid_t fork1 = fork();
    if(fork1 > 0){
        if( fork() == 0){
            fork();
        }
    }
    else if( fork1 == 0){
        if ( fork() == 0){
            if ( fork() ==0){
                if( fork() ==0){
                    if ( fork()==0){
                        cout << "ID of Process 'P' is " << superparentprocessid << endl;
                    }
                }
            }
        }
    }
    else{
        cout << "fork failed\n";
    }
    return 0;
}