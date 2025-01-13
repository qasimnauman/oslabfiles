#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
using namespace std;

int main() {
    char str[256]; // Buffer for reading data
    int fifo_read;

    // Open the named pipe in read-only mode
    fifo_read = open("pipe_one", O_RDONLY);
    if (fifo_read < 0) {
        perror("Error while opening pipe");
        return 1; // Exit if unable to open the pipe
    }

    // Read and display data from the pipe
    cout << "Waiting for data..." << endl;
    while (true) {
        // Initialize the buffer
        memset(str, 0, sizeof(str));

        // Read data from the pipe (blocking call)
        ssize_t bytes_read = read(fifo_read, str, sizeof(str) - 1); // Leave space for null terminator
        if (bytes_read > 0) {
            str[bytes_read] = '\0'; // Null-terminate the string
            cout << "Text: " << str << endl;

            // Exit the loop if "abort" is received
            if (strcmp(str, "abort") == 0) {
                cout << "Abort signal received. Exiting..." << endl;
                break;
            }
        } else if (bytes_read == 0) {
            cout << "Writer process closed the pipe. Exiting..." << endl;
            break;
        } else {
            perror("Error while reading from pipe");
            break;
        }
    }

    // Close the pipe
    close(fifo_read);
    return 0;
}
