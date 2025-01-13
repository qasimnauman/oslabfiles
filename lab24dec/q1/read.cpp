#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main()
{
    const char* myfifo = "/home/qasim/Desktop/oslabfiles/lab24dec/q1/myfifo";
    mkfifo(myfifo, 0666); // Create the FIFO if it doesn't exist

    char buffer[80];
    int fd;

    cout << "Reader process started. Waiting for messages..." << endl;

    while (true)
    {
        fd = open(myfifo, O_RDONLY); // Open FIFO for reading
        read(fd, buffer, sizeof(buffer)); // Read data from the FIFO
        close(fd);

        // Display the received message
        cout << "Received: " << buffer << endl;

        // Exit if "exit" is received
        if (strcmp(buffer, "exit") == 0)
        {
            cout << "Reader process exiting..." << endl;
            break;
        }

        sleep(1); // Small delay for synchronization
    }

    return 0;
}
