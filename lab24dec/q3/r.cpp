#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    const char* pipeName = "/tmp/nonblocking_pipe";

    // Create the named pipe using a shell command
    system(("mkfifo " + string(pipeName)).c_str());

    // Open the pipe for reading and writing
    int pipeFd = open(pipeName, O_RDWR); // Reading and writing mode
    if (pipeFd == -1) {
        cerr << "Error opening pipe" << endl;
        return 1;
    }

    char buffer[128];
    string input;

    cout << "Type messages to write to the pipe. Type 'exit' to quit." << endl;

    while (true) {
        // Writing to the pipe
        cout << "Enter: ";
        getline(cin, input);

        if (input == "exit") break;

        write(pipeFd, input.c_str(), input.size());

        // Simulate non-blocking read using usleep
        usleep(500000); // Sleep for 500ms

        ssize_t bytesRead = read(pipeFd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            cout << "Read from pipe: " << buffer << endl;
        } else {
            cout << "No data available to read." << endl;
        }
    }

    close(pipeFd);
    unlink(pipeName); // Remove the named pipe
    return 0;
}