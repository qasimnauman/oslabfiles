#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <regex>

using namespace std;

#define MAX_ARGS 10

// Function to parse commands and their arguments
vector<char *> parse_arguments(const string &command)
{
    vector<char *> args;
    stringstream ss(command);
    string arg;
    while (ss >> arg)
    {
        char *arg_cstr = new char[arg.length() + 1];
        strcpy(arg_cstr, arg.c_str());
        args.push_back(arg_cstr);
    }
    args.push_back(nullptr);
    return args;
}

// Function to expand environment variables
string expand_environment_variables(const string &input)
{
    string expanded = input;
    size_t pos = expanded.find('$');
    while (pos != string::npos)
    {
        size_t end = expanded.find_first_of(" /\t", pos + 1);
        string var = expanded.substr(pos + 1, end - pos - 1);
        const char *value = getenv(var.c_str());
        if (value)
        {
            expanded.replace(pos, var.length() + 1, value);
        }
        pos = expanded.find('$', pos + 1);
    }
    return expanded;
}

// Function to parse input into pipeline commands
void parse_input(const string &input, vector<string> &commands)
{
    size_t pos = 0, start = 0;
    while ((pos = input.find('|', start)) != string::npos)
    {
        commands.push_back(input.substr(start, pos - start));
        start = pos + 1;
    }
    commands.push_back(input.substr(start));
}

// Function to validate pipeline commands
bool validate_pipeline(const vector<string> &commands)
{
    for (const string &cmd : commands)
    {
        if (cmd.empty())
        {
            cout << "Invalid pipeline: Empty command detected." << endl;
            return false;
        }
    }
    return true;
}

// Function to execute a single command
void execute_command(char *args[])
{
    if (execvp(args[0], args) == -1)
    {
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }
}

// Function to handle built-in commands
bool handle_builtin_commands(const vector<char *> &args, vector<string> &history)
{
    if (args.empty() || args[0] == nullptr)
        return false;

    string command = args[0];

    // Handle `cd` command
    if (command == "cd")
    {
        if (args.size() < 2 || args[1] == nullptr)
        {
            cout << "cd: missing operand" << endl;
        }
        else
        {
            if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
        }
        return true;
    }

    // Handle `pwd` command
    if (command == "pwd")
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
        {
            cout << cwd << endl;
        }
        else
        {
            perror("pwd");
        }
        return true;
    }

    // Handle `history` command
    if (command == "history")
    {
        for (size_t i = 0; i < history.size(); ++i)
        {
            cout << i + 1 << ": " << history[i] << endl;
        }
        return true;
    }

    // Handle `exit` command
    if (command == "exit")
    {
        cout << "Exiting the shell." << endl;
        exit(0);
    }

    return false; // Not a built-in command
}

int main()
{
    string input;
    vector<string> history;

    cout << "Custom Shell. Type 'exit' to quit.\n";

    while (true)
    {
        // Display prompt
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
        {
            cout << "shell [" << cwd << "]> ";
        }
        else
        {
            perror("getcwd");
        }

        getline(cin, input);
        input = expand_environment_variables(input);

        if (input.empty())
            continue;

        history.push_back(input);

        vector<string> commands;
        parse_input(input, commands);

        if (!validate_pipeline(commands))
        {
            continue;
        }

        int num_commands = commands.size();
        int pipe_fds[2 * (num_commands - 1)];

        // Create pipes
        for (int i = 0; i < num_commands - 1; ++i)
        {
            if (pipe(pipe_fds + i * 2) == -1)
            {
                perror("Pipe failed");
                return 1;
            }
        }

        for (int i = 0; i < num_commands; ++i)
        {
            vector<char *> args = parse_arguments(commands[i]);

            // Check for built-in commands
            if (handle_builtin_commands(args, history))
            {
                for (char *arg : args)
                    delete[] arg;
                continue;
            }

            pid_t pid = fork();

            if (pid < 0)
            {
                perror("Fork failed");
                return 1;
            }
            else if (pid == 0)
            {
                // Child process
                if (i > 0)
                { // If not the first command, get input from previous pipe
                    dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
                }
                if (i < num_commands - 1)
                { // If not the last command, output to next pipe
                    dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
                }

                for (int j = 0; j < 2 * (num_commands - 1); ++j)
                {
                    close(pipe_fds[j]);
                }

                execute_command(args.data());
            }

            // Clean up argument memory
            for (char *arg : args)
            {
                delete[] arg;
            }
        }

        for (int i = 0; i < 2 * (num_commands - 1); ++i)
        {
            close(pipe_fds[i]);
        }

        for (int i = 0; i < num_commands; ++i)
        {
            wait(nullptr);
        }
    }

    return 0;
}
