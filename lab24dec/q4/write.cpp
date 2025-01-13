#include <iostream>
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

    char logMessage[256];
    int fd;

    cout << "Log Writer started. Write log messages below:\n";

    while (true)
    {
        fd = open(logpipe, O_WRONLY); // Open the pipe for writing
        if (fd < 0)
        {
            cerr << "Error: Unable to open the log pipe for writing!" << endl;
            return 1;
        }

        cout << "Enter log message (type \"exit\" to quit): ";
        cin.getline(logMessage, sizeof(logMessage));

        // Write log message to the pipe
        write(fd, logMessage, strlen(logMessage) + 1);
        close(fd);

        if (strcmp(logMessage, "exit") == 0)
        {
            cout << "Log Writer exiting..." << endl;
            break;
        }
    }

    return 0;
}
