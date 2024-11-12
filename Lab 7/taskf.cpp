#include <iostream>

using namespace std;

struct Process
{
    int pno;
    int at;
    int et;
    int wt;
    int tat;
    int priority;
};

void insertionSort(Process array[], int size) {
    for (int step = 1; step < size; step++) {
        Process key = array[step];
        int j = step - 1;

        // Shift elements that are greater than key's arrival time to one position ahead
        while (j >= 0 && array[j].et > key.et) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = key;
    }
}

int waittimestf(int n, Process processes[]){
    insertionSort(processes, n);

    for (int i = 0; i < n; i++)
    {
        if (processes[i].at > processes[i + 1].at)
        {
            processes[i + 1].wt = processes[i].et + processes[i].at;
        }
        else
        {
            processes[i + 1].wt = processes[i].et + processes[i + 1].at;
        }
        
    }
    
}

int main(){
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    Process processes[n];
    for (int i = 0; i < n; ++i)
    {
        cout << "Enter arrival time, execution time, and priority for process " << i + 1 << ": ";
        cin >> processes[i].at >> processes[i].et >> processes[i].priority;
        processes[i].pno = i + 1;
    }
}