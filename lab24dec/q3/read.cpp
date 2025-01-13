#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h> // For error handling

using namespace std;

int main()
{
    const char* myfifo = "/home/qasim/Desktop/oslabfiles/lab24dec/q3/myfifo";
    mkfifo(myfifo, 0666); // Create FIFO if it doesn't exist

    char buffer[80];
    int fd;

    cout << "Non-blocking reader started. Waiting for messages..." << endl;

    while (true)
    {
        fd = open(myfifo, O_RDONLY | O_NONBLOCK); // Open FIFO in non-blocking mode
        if (fd < 0)
        {
            cerr << "Error: Unable to open FIFO for reading! Retrying..." << endl;
            sleep(1);
            continue; // Retry opening
        }

        memset(buffer, 0, sizeof(buffer)); // Clear buffer

        int bytesRead = read(fd, buffer, sizeof(buffer)); // Attempt to read
        if (bytesRead > 0)
        {
            cout << "Received: " << buffer << endl;

            // Check if "exit" command was received
            if (strcmp(buffer, "exit") == 0)
            {
                cout << "Reader exiting..." << endl;
                close(fd);
                break;
            }
        }
        else if (bytesRead == -1 && errno == EAGAIN)
        {
            // No data available; simulate work
            cout << "No data available. Working on other tasks..." << endl;
        }
        else if (bytesRead == -1)
        {
            cerr << "Error: Failed to read from FIFO!" << endl;
        }

        close(fd); // Close FIFO after reading
        sleep(1); // Delay to retry reading
    }

    return 0;
}
