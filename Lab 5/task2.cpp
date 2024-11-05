#include <iostream>
#include <queue>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 5

int main() {
    queue<int> buffer;
    int iterations = 10;

    for (int i = 0; i < iterations; i++) {
        cout << "Cycle " << i + 1 << ":" << endl;

        if (buffer.size() == BUFFER_SIZE) {
            cout << "Buffer is full, producer waiting..." << endl;
        } else {
            int item = rand() % 100;
            buffer.push(item);
            cout << "Produced: " << item << endl;
        }

        if (buffer.empty()) {
            cout << "Buffer is empty, consumer waiting..." << endl;
        } else {
            int item = buffer.front();
            buffer.pop();
            cout << "Consumed: " << item << endl;
        }

        sleep(5);
    }

    return 0;
}
