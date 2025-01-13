#include <iostream>
#include <fstream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <cstdlib>

using namespace std;

int main()
{
    int fd1;

    // FIFO file path
    const char* myfifo = "/home/qasim/Desktop/oslabfiles/lab24dec/q1/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char str1[80], str2[80];
    while (true)
    {
        // First open in read-only and read
        fd1 = open(myfifo, O_RDONLY);
        read(fd1, str1, 80);

        // Print the read string and close
        cout << "User1: " << str1 << endl;
        close(fd1);

        // Exit if "bye" is received
        if (strcmp(str1, "bye") == 0) {
            cout << "User1 ended the chat." << endl;
            break;
        }

        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo, O_WRONLY);
        cin.getline(str2, 80);
        write(fd1, str2, strlen(str2) + 1);
        close(fd1);

        // Exit if "bye" is sent
        if (strcmp(str2, "bye") == 0) {
            cout << "You ended the chat." << endl;
            break;
        }

        // Synchronization delay
        sleep(1);
    }
    return 0;
}
