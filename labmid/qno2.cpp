#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    string command;

    cout << "Enter the commadnds you want to execute below type 'exit' to quit.\n";

    while (true)
    {
        cout << "Command: ";
        getline(cin, command);

        if (command == "exit")
        {
            break;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            int status = system(command.c_str());
            if (status == 0)
            {
                cout << "Command executed successfully.\n";
                exit(0);
            }
            else
            {
                cout << "Command execution failed.\n";
                exit(1);
            }
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
    }

    return 0;
}
