#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <unistd.h> // for sleep function
#include <climits>

using namespace std;

// Process structure to store process details
struct Process {
    int pid;        // Process ID
    int priority;   // Process priority
    int burstTime;  // Burst time
    int arrivalTime; // Arrival time
    int remainingTime; // Remaining burst time
};

class Scheduler {
private:
    vector<Process> processes;
    queue<Process> foregroundQueue;
    queue<Process> backgroundQueue;

public:
    // Method to input process details
    void inputProcessDetails() {
        int n;
        cout << "Enter number of processes: ";
        cin >> n;

        for (int i = 0; i < n; i++) {
            Process p;
            p.pid = i + 1;
            
            cout << "Process " << p.pid << " Details:" << endl;
            cout << "Priority (0-10): ";
            cin >> p.priority;
            
            cout << "Burst Time: ";
            cin >> p.burstTime;
            
            cout << "Arrival Time: ";
            cin >> p.arrivalTime;

            p.remainingTime = p.burstTime;
            processes.push_back(p);
        }
    }

    // Priority-based Scheduling
    void priorityScheduling() {
        // Sort processes by priority (higher priority first)
        sort(processes.begin(), processes.end(), 
            [](const Process& a, const Process& b) {
                return a.priority > b.priority;
            });

        int totalWaitingTime = 0;
        int currentTime = 0;

        for (auto& process : processes) {
            // Wait until process arrival time
            while (currentTime < process.arrivalTime) {
                currentTime++;
            }

            cout << "Executing Process " << process.pid 
                 << " (Priority: " << process.priority 
                 << ", Burst Time: " << process.burstTime << ")" << endl;

            // Calculate waiting time
            int waitingTime = currentTime - process.arrivalTime;
            totalWaitingTime += waitingTime;

            // Update current time
            currentTime += process.burstTime;
        }

        float avgWaitingTime = (float)totalWaitingTime / processes.size();
        cout << "Average Waiting Time (Priority Scheduling): " 
             << avgWaitingTime << endl;
    }

    // Shortest Remaining Time First (SRTF)
    void shortestRemainingTimeFirst() {
        int totalWaitingTime = 0;
        int currentTime = 0;
        int completedProcesses = 0;

        while (completedProcesses < processes.size()) {
            int shortestIndex = -1;
            int shortestTime = INT_MAX;

            // Find process with shortest remaining time
            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].arrivalTime <= currentTime && 
                    processes[i].remainingTime > 0 && 
                    processes[i].remainingTime < shortestTime) {
                    shortestIndex = i;
                    shortestTime = processes[i].remainingTime;
                }
            }

            if (shortestIndex == -1) {
                currentTime++;
                continue;
            }

            // Execute the process
            processes[shortestIndex].remainingTime--;

            // Check if process is completed
            if (processes[shortestIndex].remainingTime == 0) {
                completedProcesses++;
                int completionTime = currentTime + 1;
                int turnaroundTime = completionTime - processes[shortestIndex].arrivalTime;
                int waitingTime = turnaroundTime - processes[shortestIndex].burstTime;
                totalWaitingTime += waitingTime;
            }

            currentTime++;
        }

        float avgWaitingTime = (float)totalWaitingTime / processes.size();
        cout << "Average Waiting Time (SRTF): " << avgWaitingTime << endl;
    }

    // Multilevel Feedback Queue
    void multiLevelFeedbackQueue() {
        // Separate processes into foreground and background queues
        for (auto& process : processes) {
            if (process.priority >= 5) {
                foregroundQueue.push(process);
            } else {
                backgroundQueue.push(process);
            }
        }

        int currentTime = 0;
        bool isForegroundTurn = true;
        int foregroundTimeSlice = 10;
        int backgroundTimeSlice = 3;

        while (!foregroundQueue.empty() || !backgroundQueue.empty()) {
            if (isForegroundTurn && !foregroundQueue.empty()) {
                // Execute foreground processes
                Process p = foregroundQueue.front();
                foregroundQueue.pop();

                int executionTime = min(p.remainingTime, foregroundTimeSlice);
                p.remainingTime -= executionTime;
                currentTime += executionTime;

                if (p.remainingTime > 0) {
                    foregroundQueue.push(p);
                }

                isForegroundTurn = false;
            } 
            else if (!backgroundQueue.empty()) {
                // Execute background processes
                Process p = backgroundQueue.front();
                backgroundQueue.pop();

                int executionTime = min(p.remainingTime, backgroundTimeSlice);
                p.remainingTime -= executionTime;
                currentTime += executionTime;

                if (p.remainingTime > 0) {
                    backgroundQueue.push(p);
                }

                isForegroundTurn = true;
            }
        }

        cout << "Multilevel Feedback Queue scheduling completed." << endl;
    }

    void runSchedulers() {
        cout << "\n--- Priority-based Scheduling ---" << endl;
        priorityScheduling();

        cout << "\n--- Shortest Remaining Time First ---" << endl;
        shortestRemainingTimeFirst();

        cout << "\n--- Multilevel Feedback Queue ---" << endl;
        multiLevelFeedbackQueue();
    }
};

int main() {
    Scheduler scheduler;
    scheduler.inputProcessDetails();
    scheduler.runSchedulers();

    return 0;
}