#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main() {
    char *args[] = {"/bin/echo", "Hello World", NULL};
    execv("/bin/echo",args);

    cout << "If exec fails, this will be printed" << endl;
    return 0;
}