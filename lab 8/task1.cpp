#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void *thd_print(void *)
{
    cout << "Thread created with id " << pthread_self() << endl;
    sleep(1);
    return NULL;
}

int main()
{
    cout << "Enter the no of threads you want to create : ";
    int nthd;
    cin >> nthd;

    for (int i = 0; i < nthd; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, thd_print, NULL);
        pthread_join(thread, NULL);
    }
}