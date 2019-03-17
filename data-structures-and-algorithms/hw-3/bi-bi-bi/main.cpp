#include <cmath>
#include <iostream>

void mergeSort(long * arr, long ARR_SIZE);

void merge(long * arr, long * leftArr, long LEFT_ARR_SIZE, long * rightArr, long RIGHT_ARR_SIZE);

void getBinarySearchIndex(long * arr, unsigned long arrSize, long element);

long jumpSearchRight(long * arr, long element, long left, long right);

long jumpSearchLeft(long * arr, long element, long left, long right);

int main() 
{
    unsigned long arrSize = 0;
    std::cin >> arrSize;
    long * arr = new long[arrSize];
    
    for (size_t i = 0; i < arrSize; ++i)
    {
        std::cin >> arr[i];
    }

    unsigned long numQueries = 0;
    std::cin >> numQueries;
    long * queryNumbers = new long[numQueries];
    
    for (size_t i = 0; i < numQueries; ++i)
    {
        std::cin >> queryNumbers[i];
    }

    mergeSort(arr, arrSize);

    for (size_t i = 0; i < numQueries; ++i)
    {
        getBinarySearchIndex(arr, arrSize, queryNumbers[i]);
    }

    if (queryNumbers != nullptr) { delete[] queryNumbers; }
    if (arr != nullptr) { delete[] arr; }

    return 0;
}

void mergeSort(long * arr, long ARR_SIZE)
{
    if (ARR_SIZE < 2) { return; }

    long mid = ARR_SIZE / 2;
    long * leftArr = new long[mid];
    long * rightArr = new long[ARR_SIZE - mid];

    for (size_t i = 0; i < mid; ++i)
    {
        leftArr[i] = arr[i];
    }

    for (size_t i = mid; i < ARR_SIZE; ++i)
    {
        rightArr[i - mid] = arr[i];
    }

    mergeSort(leftArr, mid);
    mergeSort(rightArr, ARR_SIZE - mid);
    merge(arr, leftArr, mid, rightArr, ARR_SIZE - mid);

    if (leftArr != nullptr) { delete[] leftArr; }
    if (rightArr != nullptr) { delete[] rightArr; }
}

void merge(long * arr, long * leftArr, long LEFT_ARR_SIZE, long * rightArr, long RIGHT_ARR_SIZE)
{
    unsigned long i = 0, j = 0, k = 0;

    while (i < LEFT_ARR_SIZE && j < RIGHT_ARR_SIZE)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k++] = leftArr[i++];
        }
        else
        {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < LEFT_ARR_SIZE)
    {
        arr[k++] = leftArr[i++];
    }

    while (j < RIGHT_ARR_SIZE)
    {
        arr[k++] = rightArr[j++];
    }
}

void getBinarySearchIndex(long * arr, unsigned long arrSize, long element)
{
    unsigned long left = 0;
    unsigned long right = arrSize - 1;

    while (left <= right)
    {
        unsigned long mid = left + (right - left) / 2;

        if (arr[mid] == element)
        {
            std::cout 
                << (jumpSearchLeft(arr, element, left, mid) + 1)
                << " " 
                << (jumpSearchRight(arr, element, mid, right) + 1) << std::endl;

            return;
        }
        else if (element < arr[mid])
        {
            right = mid - 1;
        }
        else { left = mid + 1; }
    }

    std::cout << -1 << " " << -1 << std::endl;
}

long jumpSearchRight(long * arr, long element, long left, long right)
{
    if (left == right)
    {
        return left;
    }
    
    long length = right - left + 1;
    long step = left + sqrt(length);
    long prev = left;
    
    while (arr[std::min(step, right)] == element)
    {
        prev = step;
        step += sqrt(length);

        if (prev >= right)
        {
            return right;
        }
    }

    while (arr[prev] == element)
    {
        ++prev;

        if (prev == std::min(step, length)) { return prev - 1; }
    }

    if (element < arr[prev])
    {
        return prev - 1;
    }

    return left;
}

long jumpSearchLeft(long * arr, long element, long left, long right)
{
    if (left == right)
    {
        return right;
    }
    
    long length = right - left + 1;
    long step = left + sqrt(length);
    long prev = left;

    while (arr[std::min(step, right)] < element)
    {
        prev = step;
        step += sqrt(length);

        if (prev >= right)
        {
            return right;
        }
    }

    while (arr[prev] < element)
    {
        ++prev;

        if (prev == std::min(step, length)) { return prev; }
    }

    if (element == arr[prev])
    {
        return prev;
    }

    return right;
}