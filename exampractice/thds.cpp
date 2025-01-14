// #include <iostream>
// #include <pthread.h>

// using namespace std;

// struct thdstr
// {
//     string firstname;
//     string lastname;
//     string classname;
//     string section;
// };

// // Thread function
// void *thredsfunc(void *args)
// {
//     // thdstr *thd = static_cast<thdstr *>(args); // Safely cast to thdstr
//     // cout << "First Name: " << thd->firstname << endl;
//     // cout << "Last Name: " << thd->lastname << endl;
//     // cout << "Class Name: " << thd->classname << endl;
//     // cout << "Section: " << thd->section << endl;
//     // cout << "Air University" << endl;

//     int count = *static_cast<int *>(args);

//     for (int i = 0; i < count; i++)
//     {
//         for (int j = 0; j <= i; j++)
//         {
//             cout << j;
//         }
//         cout << "\n";
//     }

//     pthread_exit(nullptr); // Exit the thread cleanly
// }

// int main()
// {
//     // // Initialize thread structure
//     // thdstr thd;
//     // thd.firstname = "Qasim";
//     // thd.lastname = "Nauman";
//     // thd.classname = "BSCS";
//     // thd.section = "5B";

//     int count = 10;
//     pthread_t thread;

//     // Create the thread
//     if (pthread_create(&thread, nullptr, thredsfunc, &count) != 0)
//     {
//         cerr << "Error creating thread" << endl;
//         return 1;
//     }

//     // Join the thread in the main function
//     if (pthread_join(thread, nullptr) != 0)
//     {
//         cerr << "Error joining thread" << endl;
//         return 1;
//     }

//     return 0;
// }

