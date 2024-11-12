#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sstream>

void parseCommand(const std::string &input, std::vector<char*> &args) {
    std::istringstream iss(input);
    std::string token;
    
    // Clear any previous arguments
    args.clear();
    
    // Split input into tokens
    while (iss >> token) {
        // Store a mutable copy of the argument (execvp requires char* array)
        char *arg = new char[token.size() + 1];
        std::strcpy(arg, token.c_str());
        args.push_back(arg);
    }
    
    // Null-terminate the array (required by execvp)
    args.push_back(nullptr);
}

int main() {
    std::string command;

    std::cout << "Welcome to myshell! Type 'exit' to quit.\n";

    while (true) {
        // Display prompt
        std::cout << "myshell> ";
        std::getline(std::cin, command);

        // Exit condition
        if (command == "exit") {
            break;
        }

        // Parse the command and arguments
        std::vector<char*> args;
        parseCommand(command, args);

        // Fork a new process
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            std::cerr << "Failed to create process." << std::endl;
            continue;
        } else if (pid == 0) {
            // Child process: execute the command using execvp()
            if (execvp(args[0], args.data()) == -1) {
                std::cerr << "Command execution failed.\n";
            }
            // Free memory for args in the child process
            for (char* arg : args) delete[] arg;
            exit(1); // Exit with error code if execvp fails
        } else {
            // Parent process: wait for the child to complete
            int status;
            pid_t child_pid = wait(&status);

            if (child_pid == -1) {
                std::cerr << "Error waiting for child process." << std::endl;
            } else {
                std::cout << "Process " << child_pid << " completed." << std::endl;
            }

            // Free memory for args in the parent process
            for (char* arg : args) delete[] arg;
        }
    }

    std::cout << "Exiting myshell." << std::endl;
    return 0;
}
