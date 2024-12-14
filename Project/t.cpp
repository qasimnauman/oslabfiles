#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <cstring>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() (std::cout << "\033[H\033[J")

// Greeting shell during startup
void init_shell() {
    clear();
    std::cout << "\n\n\n\n******************"
              << "************************";
    std::cout << "\n\n\n\t****MY SHELL****";
    std::cout << "\n\n\t-USE AT YOUR OWN RISK-";
    std::cout << "\n\n\n\n*******************"
              << "***********************";
    const char* username = getenv("USER");
    std::cout << "\n\n\nUSER is: @" << username;
    std::cout << "\n";
    sleep(1);
    clear();
}

// Function to take input
int takeInput(std::string& input) {
    char* buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        input = std::string(buf);
        free(buf);
        return 0;
    } else {
        free(buf);
        return 1;
    }
}

// Function to print Current Directory.
void printDir() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        std::cout << "\nDir: " << cwd;
    }
}

// Function where the system command is executed
void execArgs(char** parsed) {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "\nFailed forking child..";
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            std::cerr << "\nCould not execute command..";
        }
        exit(0);
    } else {
        wait(NULL); // waiting for child to terminate
        return;
    }
}

// Function where the piped system commands are executed
void execArgsPiped(char** parsed, char** parsedpipe) {
    int pipefd[2]; // 0 is read end, 1 is write end
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        std::cerr << "\nPipe could not be initialized";
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        std::cerr << "\nCould not fork";
        return;
    }

    if (p1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            std::cerr << "\nCould not execute command 1..";
            exit(0);
        }
    } else {
        p2 = fork();
        if (p2 < 0) {
            std::cerr << "\nCould not fork";
            return;
        }

        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                std::cerr << "\nCould not execute command 2..";
                exit(0);
            }
        } else {
            wait(NULL);
            wait(NULL);
        }
    }
}

// Help command builtin
void openHelp() {
    std::cout << "\n***WELCOME TO MY SHELL HELP***"
              << "\nCopyright @ Suprotik Dey"
              << "\n-Use the shell at your own risk..."
              << "\nList of Commands supported:"
              << "\n>cd"
              << "\n>ls"
              << "\n>exit"
              << "\n>all other general commands available in UNIX shell"
              << "\n>pipe handling"
              << "\n>improper space handling\n";
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed) {
    const int NoOfOwnCmds = 4;
    std::string ListOfOwnCmds[NoOfOwnCmds] = {"exit", "cd", "help", "hello"};
    const char* username = getenv("USER");

    for (int i = 0; i < NoOfOwnCmds; i++) {
        if (parsed[0] == ListOfOwnCmds[i]) {
            switch (i) {
                case 0:
                    std::cout << "\nGoodbye\n";
                    exit(0);
                case 1:
                    chdir(parsed[1]);
                    return 1;
                case 2:
                    openHelp();
                    return 1;
                case 3:
                    std::cout << "\nHello " << username << ".\nMind that this is "
                              << "not a place to play around.\nUse help to know more..\n";
                    return 1;
                default:
                    break;
            }
        }
    }
    return 0;
}

// Function for finding pipe
int parsePipe(std::string& str, std::vector<std::string>& strpiped) {
    size_t pos = str.find('|');
    if (pos != std::string::npos) {
        strpiped.push_back(str.substr(0, pos));
        strpiped.push_back(str.substr(pos + 1));
        return 1;
    }
    return 0; // returns zero if no pipe is found.
}

// Function for parsing command words
void parseSpace(const std::string& str, std::vector<char*>& parsed) {
    char* token = strtok(const_cast<char*>(str.c_str()), " ");
    while (token != nullptr) {
        parsed.push_back(token);
        token = strtok(nullptr, " ");
    }
    parsed.push_back(nullptr);
}

int processString(const std::string& str, std::vector<char*>& parsed, std::vector<char*>& parsedpipe) {
    std::vector<std::string> strpiped;
    int piped = parsePipe(const_cast<std::string&>(str), strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    } else {
        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed.data()))
        return 0;
    else
        return 1 + piped;
}

int main() {
    std::string inputString;
    std::vector<char*> parsedArgs;
    std::vector<char*> parsedArgsPiped;
    int execFlag = 0;

    init_shell();

    while (true) {
        printDir();
        if (takeInput(inputString))
            continue;

        parsedArgs.clear();
        parsedArgsPiped.clear();

        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        if (execFlag == 1)
            execArgs(parsedArgs.data());

        if (execFlag == 2)
            execArgsPiped(parsedArgs.data(), parsedArgsPiped.data());
    }

    return 0;
}
