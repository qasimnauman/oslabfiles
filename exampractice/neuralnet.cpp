#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Define matrices
vector<vector<double>> W = {
    {0.2, -0.5, 10.0, 2.0},
    {1.5, 1.3, 2.1, 0.0},
    {0.0, 0.25, 0.2, -0.3}};

vector<double> x = {56, 231, 24, 2};
vector<double> b = {1.1, 3.2, -1.2};

vector<double> Z(W.size(), 0.0);
vector<double> A(W.size(), 0.0);

// Thread function for matrix multiplication
void* multiply(void* arg) {
    int row = (int&)arg;
    Z[row] = 0.0;
    for (size_t j = 0; j < x.size(); ++j) {
        Z[row] += W[row][j] * x[j];
    }
    pthread_exit(nullptr);
}

// Thread function for adding biases
void* add_bias(void* arg) {
    int row = *(int*)arg;
    Z[row] += b[row];
    pthread_exit(nullptr);
}

// Thread function for applying sigmoid
void* sigmoid(void* arg) {
    int row = *(int*)arg;
    A[row] = 1.0 / (1.0 + exp(-Z[row]));
    pthread_exit(nullptr);
}

int main() {
    pid_t pid1, pid2, pid3;

    // Process P1: Matrix multiplication
    pid1 = fork();
    if (pid1 == 0) {
        pthread_t threads[W.size()];
        int indices[W.size()];

        for (size_t i = 0; i < W.size(); ++i) {
            indices[i] = i;
            pthread_create(&threads[i], nullptr, multiply, &indices[i]);
        }

        for (size_t i = 0; i < W.size(); ++i) {
            pthread_join(threads[i], nullptr);
        }
        exit(0);
    } else {
        waitpid(pid1, nullptr, 0);
    }

    // Process P2: Add biases
    pid2 = fork();
    if (pid2 == 0) {
        pthread_t threads[b.size()];
        int indices[b.size()];

        for (size_t i = 0; i < b.size(); ++i) {
            indices[i] = i;
            pthread_create(&threads[i], nullptr, add_bias, &indices[i]);
        }

        for (size_t i = 0; i < b.size(); ++i) {
            pthread_join(threads[i], nullptr);
        }
        exit(0);
    } else {
        waitpid(pid2, nullptr, 0);
    }

    // Process P3: Apply sigmoid
    pid3 = fork();
    if (pid3 == 0) {
        pthread_t threads[Z.size()];
        int indices[Z.size()];

        for (size_t i = 0; i < Z.size(); ++i) {
            indices[i] = i;
            pthread_create(&threads[i], nullptr, sigmoid, &indices[i]);
        }

        for (size_t i = 0; i < Z.size(); ++i) {
            pthread_join(threads[i], nullptr);
        }
        exit(0);
    } else {
        waitpid(pid3, nullptr, 0);
    }

    // Output the result
    cout << "Resultant A matrix (after sigmoid):" << endl;
    for (double val : A) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
