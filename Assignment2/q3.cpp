#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int frequency[256] = {0};

void* calculatefrequency(void* arg)
{
    vector<int> *vec = (vector<int> *)arg;

    for (int num : *vec)
    {
        frequency[num]++;
    }

    return NULL;
}

int main()
{
    ifstream file("Matrix.txt");
    if (!file.is_open())
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;
    vector<vector<int>> vectors;

    while (getline(file, line))
    {
        stringstream ss(line);
        string numStr;
        vector<int> vec;

        while (getline(ss, numStr, ','))
        {
            vec.push_back(stoi(numStr));
        }

        vectors.push_back(vec);
    }

    file.close();

    for (const vector<int> &vec : vectors)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, calculatefrequency, (void *)&vec);
        pthread_join(tid, NULL);
    }

    for (int i = 0; i < 256; i++)
    {
        cout << i << " : " << frequency[i] << "\t";

        if (i % 16 == 15)
        {
            cout << endl;
        }
    }

    cout << "\n";
    return 0;
}
