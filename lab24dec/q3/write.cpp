#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
    const char* myfifo = "/home/qasim/Desktop/oslabfiles/lab24dec/q3/myfifo";
    mkfifo(myfifo, 0666); // Create FIFO if it doesn't exist

    char message[80];
    int fd;

    cout << "Non-blocking writer started. Enter messages..." << endl;

    while (true)
    {
        fd = open(myfifo, O_WRONLY | O_NONBLOCK); // Open FIFO in non-blocking mode
        if (fd < 0)
        {
            cerr << "Error: Unable to open FIFO! Retrying..." << endl;
            sleep(1);
            continue; // Retry opening
        }

        memset(message, 0, sizeof(message)); // Clear buffer

        cout << "Enter message (type \"exit\" to quit): ";
        cin.getline(message, sizeof(message));

        int bytesWritten = write(fd, message, strlen(message) + 1); // Attempt to write
        if (bytesWritten > 0)
        {
            cout << "Message sent: " << message << endl;

            // Check if "exit" command was sent
            if (strcmp(message, "exit") == 0)
            {
                cout << "Writer exiting..." << endl;
                close(fd);
                break;
            }
        }
        else
        {
            cerr << "Error: Failed to write to FIFO! Retrying..." << endl;
        }

        close(fd); // Close FIFO after writing
        sleep(1); // Delay to retry writing
    }

    return 0;
}
