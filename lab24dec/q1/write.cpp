#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
    const char* myfifo = "/home/qasim/Desktop/oslabfiles/lab24dec/q1/myfifo";
    mkfifo(myfifo, 0666);

    char message[80];

    while (true)
    {
        int fd = open(myfifo, O_WRONLY);
        if (fd < 0)
        {
            cerr << "Error: Unable to open FIFO for writing!" << endl;
            return 1;
        }

        memset(message, 0, sizeof(message)); // Clear buffer
        cout << "Enter message (type \"exit\" to quit): ";
        cin.getline(message, sizeof(message));

        write(fd, message, strlen(message) + 1);
        close(fd);

        if (strcmp(message, "exit") == 0)
        {
            cout << "Writer process exiting..." << endl;
            break;
        }

        sleep(1);
    }

    return 0;
}
