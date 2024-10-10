#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        exit(1);
    }

    if (pid == 0) {
        cout << "Child process chal raha hai, uski ID: " << getpid() << endl;
        sleep(2);
        cout << "Child process band ho raha jiski ID: " << getpid() << endl;
        exit(0);
    } else {
        cout << "Parent process bhi chal raha hai, uski ID: " << getpid() << endl;
        cout << "Child process zombie hogya hai 5 seconds ke lye" << endl;
        sleep(5);

        cout << "Zombie process is being cleaned up." << endl;
        wait(NULL);

        cout << "Child process cleaned up. No more zombies." << endl;
    }

    return 0;
}
