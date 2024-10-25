#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <string.h>
#include <ctime>
#include <fstream>
#include <cmath>

using namespace std;

#define NUM_CHILDREN 3
#define READ_END 0
#define WRITE_END 1

struct ProcessInfo {
    pid_t pid;
    time_t start_time;
    time_t end_time;
    double cpu_usage;
};

static clock_t lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;

void init() {
    FILE* file;
    struct tms timeSample;
    char line[128];

    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    file = fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while(fgets(line, 128, file) != NULL) {
        if (strncmp(line, "processor", 9) == 0) numProcessors++;
    }
    fclose(file);
}

double getCurrentValue() {
    struct tms timeSample;
    clock_t now;
    double percent;

    now = times(&timeSample);
    if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
        timeSample.tms_utime < lastUserCPU) {
        percent = -1.0;
    }
    else {
        percent = (timeSample.tms_stime - lastSysCPU) +
                 (timeSample.tms_utime - lastUserCPU);
        percent /= (now - lastCPU);
        percent /= numProcessors;
        percent *= 100;
    }
    lastCPU = now;
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    return percent;
}

int main() {
    int pipes[NUM_CHILDREN][2];
    pid_t children[NUM_CHILDREN];
    ProcessInfo process_infos[NUM_CHILDREN];
    
    cout << "Starting process monitoring..." << endl;
    
    for(int i = 0; i < NUM_CHILDREN; i++) {
        if(pipe(pipes[i]) == -1) {
            cout << "Pipe creation failed!" << endl;
            return 1;
        }
        
        pid_t pid = fork();
        
        if(pid == -1) {
            cout << "Fork failed!" << endl;
            return 1;
        }
        
        if(pid == 0) {
            for(int j = 0; j < i; j++) {
                close(pipes[j][READ_END]);
                close(pipes[j][WRITE_END]);
            }
            
            close(pipes[i][READ_END]);
            
            ProcessInfo info;
            info.pid = getpid();
            info.start_time = time(NULL);
            
            init();
            
            double result = 0;
            int iterations = 10000000 * (i + 1);
            
            double max_cpu = 0;
            for(int j = 0; j < iterations; j++) {
                result += sin(j) * cos(j);
                if(j % 1000000 == 0) {
                    double current_cpu = getCurrentValue();
                    if(current_cpu > max_cpu) {
                        max_cpu = current_cpu;
                    }
                }
            }
            
            info.end_time = time(NULL);
            info.cpu_usage = max_cpu;
            
            write(pipes[i][WRITE_END], &info, sizeof(ProcessInfo));
            close(pipes[i][WRITE_END]);
            exit(0);
        } 
        else {
            children[i] = pid;
            close(pipes[i][WRITE_END]);
        }
    }
    
    for(int i = 0; i < NUM_CHILDREN; i++) {
        read(pipes[i][READ_END], &process_infos[i], sizeof(ProcessInfo));
        close(pipes[i][READ_END]);
    }
    
    for(int i = 0; i < NUM_CHILDREN; i++) {
        waitpid(children[i], NULL, 0);
    }
    
    ProcessInfo* most_cpu_intensive = &process_infos[0];
    for(int i = 1; i < NUM_CHILDREN; i++) {
        if(process_infos[i].cpu_usage > most_cpu_intensive->cpu_usage) {
            most_cpu_intensive = &process_infos[i];
        }
    }
    
    cout << "\nProcess Execution Summary\n" << endl;
    
    for(int i = 0; i < NUM_CHILDREN; i++) {
        cout << "------------------------" << endl;
        cout << "Process PID: " << process_infos[i].pid << endl;
        cout << "Start Time: " << ctime(&process_infos[i].start_time);
        cout << "End Time: " << ctime(&process_infos[i].end_time);
        cout << "Peak CPU Usage: " << process_infos[i].cpu_usage << "%" << endl;
        cout << "------------------------" << endl;
    }
    
    cout << "\nMost CPU Intensive Process:" << endl;
    cout << "PID: " << most_cpu_intensive->pid << endl;
    cout << "Peak CPU Usage: " << most_cpu_intensive->cpu_usage << "%" << endl;
    
    return 0;
}