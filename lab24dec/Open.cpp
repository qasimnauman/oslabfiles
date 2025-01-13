#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
using namespace std;

int main() {
    string str;
    int fifo_write;

    // Create the named pipe
    int f1 = mkfifo("pipe_one", 0666);
    if (f1 < 0 && errno != EEXIST) {
        cout << "Error while creating pipe" << endl;
        return 1;
    }

    // Open the named pipe in write-only mode
    fifo_write = open("pipe_one", O_WRONLY);
    if (fifo_write < 0) {
        cout << "Error opening file" << endl;
        return 1;
    }

    // Loop to write user input to the pipe
    while (true) {
        cout << "Enter text: " << endl;
        getline(cin, str); // Read the entire line, including spaces

        // Write the string to the pipe
        ssize_t bytes_written = write(fifo_write, str.c_str(), str.size() + 1); // Include null terminator
        if (bytes_written < 0) {
            cout << "Error writing to pipe" << endl;
            break;
        }

        cout << "∗" << str << "∗" << endl;

        // Break if "abort" is entered
        if (str == "abort") {
            break;
        }
    }

    // Close the named pipe
    close(fifo_write);
    return 0;
}
