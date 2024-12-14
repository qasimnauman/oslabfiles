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

using namespace std;

#define MAX_ARGS 10

// Function to parse commands and their arguments
vector<char *> parse_arguments(string &command)
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
        size_t end = expanded.find(' ', pos);
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
            cerr << "Invalid pipeline: Empty command detected." << endl;
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
bool handle_builtin_commands(const vector<char *> &args)
{
    string command = args[0];

    // Handle `cd` command
    if (command == "cd")
    {
        if (args.size() < 2 || args[1] == nullptr)
        {
            cerr << "cd: missing operand" << endl;
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

    // Handle `mv` command
    // Handle `mv` command
    if (command == "mv")
    {
        if (args.size() < 3 || args[1] == nullptr || args[2] == nullptr)
        {
            cerr << "mv: missing operands" << endl;
        }
        else
        {
            struct stat dest_stat;
            string dest_path = args[2];

            // Check if the destination is a directory
            if (stat(args[2], &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode))
            {
                string src_file = args[1];
                size_t last_slash = src_file.find_last_of("/");
                string file_name = (last_slash == string::npos) ? src_file : src_file.substr(last_slash + 1);
                dest_path += "/" + file_name;
            }

            // Rename the file or directory
            if (rename(args[1], dest_path.c_str()) != 0)
            {
                perror("mv");
            }
        }
        return true;
    }

    // Handle `cp` command
    if (command == "cp")
    {
        if (args.size() < 3 || args[1] == nullptr || args[2] == nullptr)
        {
            cerr << "cp: missing operands" << endl;
        }
        else
        {
            struct stat dest_stat;
            string dest_path = args[2];

            // Check if the destination is a directory
            if (stat(args[2], &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode))
            {
                string src_file = args[1];
                size_t last_slash = src_file.find_last_of("/");
                string file_name = (last_slash == string::npos) ? src_file : src_file.substr(last_slash + 1);
                dest_path += "/" + file_name;
            }

            ifstream src(args[1], ios::binary);
            ofstream dest(dest_path, ios::binary);

            if (!src)
            {
                cerr << "cp: source file does not exist or cannot be opened." << endl;
            }
            else if (!dest)
            {
                cerr << "cp: destination file cannot be created or opened." << endl;
            }
            else
            {
                dest << src.rdbuf();
                if (dest.fail())
                {
                    cerr << "cp: error while copying the file." << endl;
                }
            }
        }
        return true;
    }

    return false; // Not a built-in command
}

int main()
{
    string input;
    vector<string> history;

    cout << "Custom Shell. Type 'quit' to exit.\n";

    while (true)
    {
        cout << "shell> ";
        getline(cin, input);
        input = expand_environment_variables(input);

        if (input == "quit")
        {
            break;
        }

        if (input == "history")
        {
            for (size_t i = 0; i < history.size(); ++i)
            {
                cout << i + 1 << ": " << history[i] << endl;
            }
            continue;
        }

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
            if (handle_builtin_commands(args))
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
