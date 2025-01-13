#include <iostream>
#include <fstream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
    const char* logpipe = "/home/qasim/Desktop/oslabfiles/lab24dec/q4/myfifo";
    mkfifo(logpipe, 0666); // Create the named pipe

    char buffer[256];
    int fd;
    ofstream logFile("logs.txt", ios::app); // Open log file for appending

    if (!logFile.is_open())
    {
        cerr << "Error: Unable to open log file!" << endl;
        return 1;
    }

    cout << "Log Server started. Waiting for log messages..." << endl;

    while (true)
    {
        fd = open(logpipe, O_RDONLY); // Open the pipe for reading
        if (fd < 0)
        {
            cerr << "Error: Unable to open the log pipe for reading!" << endl;
            sleep(1);
            continue;
        }

        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        read(fd, buffer, sizeof(buffer)); // Read log message from the pipe
        close(fd);

        if (strcmp(buffer, "exit") == 0)
        {
            cout << "Log Server exiting..." << endl;
            break;
        }

        // Write the log entry to the log file
        logFile << buffer << endl;
        cout << "Logged: " << buffer << endl;
    }

    logFile.close();
    return 0;
}
