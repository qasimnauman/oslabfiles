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
    int fd;

    // FIFO file path
    const char* myfifo = "/home/qasim/Desktop/oslabfiles/lab24dec/q1/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[80], arr2[80];
    while (true)
    {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        // Take an input string from user.
        // 80 is maximum length
        cin.getline(arr2, 80);

        // Write the input string on FIFO
        // and close it
        write(fd, arr2, strlen(arr2) + 1);
        close(fd);

        // Exit if "bye" is sent
        if (strcmp(arr2, "bye") == 0) {
            cout << "You ended the chat." << endl;
            break;
        }

        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, arr1, sizeof(arr1));

        // Print the read message
        cout << "User2: " << arr1 << endl;
        close(fd);

        // Exit if "bye" is received
        if (strcmp(arr1, "bye") == 0) {
            cout << "User2 ended the chat." << endl;
            break;
        }

        // Synchronization delay
        sleep(1);
    }
    return 0;
}
