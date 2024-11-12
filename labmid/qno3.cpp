#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

using namespace std;

// These will be globat for the hospital
#define doctors_count 5
const int time_slots[5][2] = {
    {9, 10},
    {10, 11},
    {11, 12},
    {12, 13},
    {13, 14}};

int main()
{
    cout << "Welcome to the hospital.\n";
    cout << "Doctors are available from 9:00 to 14:00.\n";

    int choice;
    cout << "Select which department do you want to manage.\n";
    cout << "1. Pateint\n";
    cout << "2. Doctor\n";
    cout << "3. Appointments\n";
    cout << "4. Billing\n";
    cout << "5. Exit\n";
    cout << "Choice: ";

    pid_t PID = fork();

    if (PID == 0)
    {

        while (choice != 5)
        {
            switch (choice)
            {
            case 1:
                patientsmanagment();
                break;
            case 2:
                doctorsmanagemnt();
                break;
            case 3:
                appointmentsmanagemnt();
                break;
            case 4:
                billingmanagemnt();
                break;
            default:
                break;
            }
        }
    }
    else if (PID > 0)
    {
        wait(NULL);
    }
}

void patientsmanagment()
{
    int pchoice;
    cout << "This block of code will handle the patient data entry and management.\n";
    cout << "Enter 1 for new patient record\n";
    cout << "Enter 2 for updating patient record\n";
    cout << "Enter 3 for deleting patient record\n";
    cout << "Enter 4 to exit\n";
    cin >> pchoice;

    switch (pchoice)
    {
    case 1:
        cout << "The code for patient records entry";
        break;
    case 2:
        cout << "The code for patient records update";
        break;
    case 3:
        cout << "The code for patient records delete";
        break;
    default:
        break;
    }
}

void doctorsmanagemnt()
{
    cout << "This block of code will handle the doctors.\n";
    cout << "A similar code as in the below block will be used to enter the data into the doctors table.\n";
}

void appointmentsmanagemnt()
{
    cout << "This block of code will handle the appointments.\n";

    cout << "This kind of block will be used to get the data from the patients and then show the available slots for the doctors.\n";
    cout << "After a similar block will be used to enter the data into the appointments table.\n";
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[1]);
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        cout << "Date being read from the patients table " << buffer << endl;
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        const char *message; // This will contain the name of the patient and id of the doctor
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        wait(nullptr);
    }
}

void billingmanagemnt()
{
    cout << "This block of code will handle the billing.\n";

    cout << "This kind of block will be used to get the data from the appointments and then display the bill\n";
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0)
    {
        close(fd[1]);
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        cout << "Apppointment info" << buffer << endl;
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        const char *message; // This will contain the total bill
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        wait(nullptr);
    }
}
