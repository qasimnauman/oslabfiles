#include <iostream>
using namespace std;

int sum(int arr[]){
    return arr[0]+arr[1];
}

int subtract(int arr[]){
    return arr[0]-arr[1];
}

int main(int argc, char* argv[])
{
    int numbers[2];
    int i =0;
    for(int i=0; i<2; i++){
        numbers[i] = atoi(argv[i+1]);
    }

    cout << "\nCalculating SUM\n";
    cout << "Sum is " << sum(numbers) << "\n";

    cout << "\nCalculating Difference\n";
    cout << "Difference is " << subtract(numbers) << "\n";
    return 0;
}