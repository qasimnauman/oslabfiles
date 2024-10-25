#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

void merge(int *left, int left_size, int *right, int right_size, int *result)
{
    int i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size)
        result[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
    while (i < left_size)
        result[k++] = left[i++];
    while (j < right_size)
        result[k++] = right[j++];
}

void childProcess(int arr[], int start, int end, int fd[])
{
    int size = end - start;
    int *subarray = new int[size];
    for (int i = 0; i < size; ++i)
        subarray[i] = arr[start + i];
    std::sort(subarray, subarray + size);
    close(fd[0]);
    write(fd[1], subarray, size * sizeof(int));
    close(fd[1]);
    delete[] subarray;
    _exit(0);
}

int main()
{
    int arr[] = {12, 4, 4, 13, 5, 23, 64, 23, 755, 1332, 24};
    int n = sizeof(arr) / sizeof(arr[0]), third = n / 3;
    int fds[3][2];
    for (int i = 0; i < 3; ++i)
        pipe(fds[i]);

    if (fork() == 0)
        childProcess(arr, 0, third, fds[0]);
    if (fork() == 0)
        childProcess(arr, third, 2 * third, fds[1]);
    if (fork() == 0)
        childProcess(arr, 2 * third, n, fds[2]);

    for (int i = 0; i < 3; ++i)
        wait(nullptr);

    int sorted1[third], sorted2[third], sorted3[n - 2 * third];
    close(fds[0][1]);
    read(fds[0][0], sorted1, sizeof(sorted1));
    close(fds[0][0]);
    close(fds[1][1]);
    read(fds[1][0], sorted2, sizeof(sorted2));
    close(fds[1][0]);
    close(fds[2][1]);
    read(fds[2][0], sorted3, sizeof(sorted3));
    close(fds[2][0]);

    int merged1[2 * third];
    merge(sorted1, third, sorted2, third, merged1);

    int final_sorted[n];
    merge(merged1, 2 * third, sorted3, n - 2 * third, final_sorted);

    for (int i = 0; i < n; ++i)
        std::cout << final_sorted[i] << " ";
    std::cout << std::endl;
    return 0;
}
