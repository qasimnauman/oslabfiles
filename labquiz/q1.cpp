#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

#define MAX_INPUT 1024

using namespace std;

// Function to read user input
string read_input() {
    cout << "myshell$ ";
    string input;
    getline(cin, input);
    return input;
}

// Function to parse input into tokens
vector<string> tokenize(const string& input, const char delimiter = ' ') {
    stringstream ss(input);
    string token;
    vector<string> tokens;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to execute a single command
void execute_command(vector<string>& args) {
    vector<char*> c_args;
    for (auto& arg : args) {
        c_args.push_back(&arg[0]);
    }
    c_args.push_back(nullptr); // Null-terminate the arguments list

    if (execvp(c_args[0], c_args.data()) < 0) {
        perror("Command execution failed");
    }
    exit(1); // Exit the child process if execvp fails
}

// Function to handle piped commands
void execute_piped_commands(vector<string>& commands) {
    int num_pipes = commands.size() - 1;
    int pipefds[2 * num_pipes];

    // Create pipes
    for (int i = 0; i < num_pipes; ++i) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("Pipe failed");
            exit(1);
        }
    }

    // Execute each command in the pipeline
    for (size_t i = 0; i < commands.size(); ++i) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            if (i > 0) { // Redirect input from the previous pipe
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            if (i < commands.size() - 1) { // Redirect output to the next pipe
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors in the child
            for (int j = 0; j < 2 * num_pipes; ++j) {
                close(pipefds[j]);
            }

            // Parse and execute the current command
            vector<string> args = tokenize(commands[i]);
            execute_command(args);
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
    }

    // Close all pipe file descriptors in the parent
    for (int i = 0; i < 2 * num_pipes; ++i) {
        close(pipefds[i]);
    }

    // Wait for all child processes to complete
    for (size_t i = 0; i < commands.size(); ++i) {
        wait(nullptr);
    }
}

// Main function
int main() {
    while (true) {
        string input = read_input(); // Read user input

        if (input == "quit") { // Termination command
            cout << "Exiting myshell...\n";
            break;
        }

        if (input.find('|') != string::npos) { // Handle piped commands
            vector<string> commands = tokenize(input, '|');
            execute_piped_commands(commands);
        } else { // Handle simple commands
            vector<string> args = tokenize(input);
            pid_t pid = fork();

            if (pid == 0) { // Child process
                execute_command(args);
            } else if (pid > 0) { // Parent process
                wait(nullptr); // Wait for the child to finish
            } else {
                perror("Fork failed");
            }
        }
    }
    return 0;
}