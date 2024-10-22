#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(){
    int n;
    int fd[2];
    char buffer[1024];
    char * data = "Hello from the line written using pipp";

    int x =pipe(fd);
    if( x < 0 ) {
        cout << "\nError! Pipe Failed\n";
        exit(EXIT_FAILURE);
    }

    write(fd[1], data, strlen(data));

    if(( n = read(fd[0], buffer, sizeof(buffer))) < 0){
        cout << "\nError! Reading Failed\n";
        return 0;
    }
    else {
        cout << "\nData Read from pipe : " << n << " bytes\n";
        cout << "\nData : " << buffer << endl;
    }
}