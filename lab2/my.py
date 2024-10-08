import os
import time

def child_process():
    print(f"\nChild Process:")
    print(f"PID: {os.getpid()}")
    print(f"Parent PID: {os.getppid()}")
    print("Child is working...")
    time.sleep(2)  # Simulate some work
    print("Child finished.")

def parent_process(child_pid):
    print(f"\nParent Process:")
    print(f"PID: {os.getpid()}")
    print(f"Child's PID: {child_pid}")
    print("Parent is working...")
    time.sleep(1)  # Simulate some work
    print("Parent finished its work and is now waiting for the child...")

def main():
    print("Starting main process")
    
    pid = os.fork()

    if pid == 0:
        # This is the child process
        child_process()
    else:
        # This is the parent process
        parent_process(pid)
        
        # Wait for the child process to complete
        _, status = os.waitpid(pid, 0)
        print(f"Child process finished with status {status}")

    print(f"Process {os.getpid()} is exiting.")

if __name__ == "__main__":
    main()