#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int total_available_resources = 100;
const int total_childs = 5;

int allocate_resources(int resources){
    if (total_available_resources - resources < 0){
        cout << "Too much resources requested \n";
        return -1;
    }
    int allocated_resources = total_available_resources - resources;
    total_available_resources -= resources;
    sleep(2);
    return 0;
}

int deallocate_resources(int resources){
    cout << "Deallocating " << resources << " resources \n";
    sleep(3);
    total_available_resources += resources;
    cout << "Deallocated " << resources << " resources \n";
    return 0;
}



int main(){
    cout << "Parent process with ID " << getpid() << " and " << "resources " << total_available_resources << "\n";

    for (int i = 0; i < total_childs; i++){
        pid_t pid = fork();
        
        if (pid < 0) {
            cout << "Fork Failed\n";
            return -1;
        }
        else if (pid == 0) {
            srand(time(0));
            int resources_request = rand() % total_available_resources;

            cout << "Child Process " << i+1 << " with ID " << getpid() << " requested " << resources_request << " resources \n";
            sleep(3);
            return(resources_request);
        }
        else{
            cout << "\nNow in Parent Process \n";
            int status;
            pid_t waitpid =  wait(&status);
            if (WIFEXITED(status)){
                int resources_req = WEXITSTATUS(status);
                cout << resources_req << " resources are requested by child with ID " << waitpid << "\n";
                if (allocate_resources(resources_req) != -1) {
                    cout << "Allocated " << resources_req << " resources \n";
                    cout << "Total available resources " << total_available_resources << "\n";
                }
                sleep(5);
                deallocate_resources(resources_req);
            }
        }
    }
    return 0;
}