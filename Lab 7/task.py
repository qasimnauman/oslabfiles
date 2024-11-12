import time
from queue import Queue

class Process:
    def __init__(self, id, priority, burst_time, arrival_time):
        self.id = id
        self.priority = priority
        self.burst_time = burst_time
        self.arrival_time = arrival_time
        self.remaining_time = burst_time
        self.waiting_time = 0
        self.turnaround_time = 0

    def is_foreground(self):
        return self.priority >= 5

def calculate_waiting_time_priority(processes):
    processes.sort(key=lambda p: (-p.priority, p.arrival_time))
    time = 0
    for process in processes:
        if time < process.arrival_time:
            time = process.arrival_time
        process.waiting_time = time - process.arrival_time
        time += process.burst_time
        process.turnaround_time = process.waiting_time + process.burst_time

    avg_waiting_time = sum(p.waiting_time for p in processes) / len(processes)
    print("Average Waiting Time (Priority Scheduling):", avg_waiting_time)

def calculate_waiting_time_srtf(processes):
    processes.sort(key=lambda p: p.arrival_time)
    time = 0
    completed = 0
    ready_queue = []
    while completed < len(processes):
        # Add processes that have arrived
        for process in processes:
            if process.arrival_time <= time and process.remaining_time > 0 and process not in ready_queue:
                ready_queue.append(process)
        
        if ready_queue:
            ready_queue.sort(key=lambda p: p.remaining_time)
            current = ready_queue[0]
            time += 1
            current.remaining_time -= 1

            if current.remaining_time == 0:
                current.waiting_time = time - current.burst_time - current.arrival_time
                current.turnaround_time = current.waiting_time + current.burst_time
                completed += 1
                ready_queue.remove(current)
        else:
            time += 1

    avg_waiting_time = sum(p.waiting_time for p in processes) / len(processes)
    print("Average Waiting Time (SRTF):", avg_waiting_time)

def multi_level_feedback_queue(processes):
    foreground_queue = Queue()
    background_queue = Queue()

    for process in processes:
        if process.is_foreground():
            foreground_queue.put(process)
        else:
            background_queue.put(process)

    time = 0
    foreground = True
    while not foreground_queue.empty() or not background_queue.empty():
        if foreground and not foreground_queue.empty():
            print("Foreground Queue Execution - 10 seconds")
            for _ in range(10):
                if foreground_queue.empty():
                    break
                process = foreground_queue.get()
                print(f"Executing Process {process.id} with priority {process.priority}")
                time.sleep(1)  # Simulate 1 second of execution
                process.remaining_time -= 1
                if process.remaining_time > 0:
                    foreground_queue.put(process)  # Reinsert process with updated burst time
                else:
                    print(f"Process {process.id} completed")
                time += 1

        elif not foreground and not background_queue.empty():
            print("Background Queue Execution - 3 seconds")
            for _ in range(3):
                if background_queue.empty():
                    break
                process = background_queue.get()
                print(f"Executing Process {process.id} with remaining burst time {process.remaining_time}")
                time.sleep(1)  # Simulate 1 second of execution
                process.remaining_time -= 1
                if process.remaining_time > 0:
                    background_queue.put(process)  # Reinsert process with updated burst time
                else:
                    print(f"Process {process.id} completed")
                time += 1

        foreground = not foreground  # Alternate between foreground and background queues

if __name__ == "__main__":
    n = int(input("Enter the number of processes: "))
    processes = []

    for i in range(n):
        priority = int(input(f"Enter priority for process {i + 1}: "))
        burst_time = int(input(f"Enter burst time for process {i + 1}: "))
        arrival_time = int(input(f"Enter arrival time for process {i + 1}: "))
        processes.append(Process(i + 1, priority, burst_time, arrival_time))

    calculate_waiting_time_priority(processes)
    calculate_waiting_time_srtf(processes)
    multi_level_feedback_queue(processes)
