#include <iostream>

unsigned long getMergeSortInversions(long * arr, unsigned long arrSize);

unsigned long getMergeInversions(long * leftArr, unsigned long leftArrSize, long * rightArr, unsigned long rightArrSize, long * baseArr);

int main() 
{
    unsigned int numQueries = 0;

    std::cin >> numQueries;

    long * queries = new long[numQueries];

    for (size_t i = 0; i < numQueries; ++i)
    {
        unsigned long numSoldiers = 0;
        
        std::cin >> numSoldiers;

        long * soldiers = new long[numSoldiers];

        for (size_t j = 0; j < numSoldiers; ++j)
        {
            std::cin >> soldiers[j];
        }
        
        queries[i] = getMergeSortInversions(soldiers, numSoldiers);

        if (soldiers != nullptr) { delete[] soldiers; }
    }

    for (size_t i = 0; i < numQueries; ++i)
    {
        std::cout << queries[i] << std::endl;
    }

    return 0;
}

unsigned long getMergeSortInversions(long * arr, unsigned long arrSize)
{
    unsigned long numInversions = 0;

    if (arrSize < 2) { return numInversions; }

    unsigned long mid = arrSize / 2;
    long *leftArr = new long[mid];
    long * rightArr = new long[arrSize - mid];

    for (size_t i = 0; i < mid; ++i)
    {
        leftArr[i] = arr[i];
    }

    for (size_t i = mid; i < arrSize; ++i)
    {
        rightArr[i - mid] = arr[i];
    }

    numInversions += getMergeSortInversions(leftArr, mid);
    numInversions += getMergeSortInversions(rightArr, arrSize - mid);
    numInversions += getMergeInversions(leftArr, mid, rightArr, arrSize - mid, arr);

    if (leftArr != nullptr) { delete[] leftArr; }
    if (rightArr != nullptr) { delete[] rightArr; }

    return numInversions;
}

unsigned long getMergeInversions(long * leftArr, unsigned long leftArrSize, long * rightArr, unsigned long rightArrSize, long * baseArr)
{
    long i = 0, j = 0, k = 0;
    long numInversions = 0;

    while (i < leftArrSize && j < rightArrSize)
    {
        if (leftArr[i] <= rightArr[j])
        {
            baseArr[k] = leftArr[i++];
        }
        else 
        { 
            baseArr[k] = rightArr[j++];
            numInversions += leftArrSize - i;
        }

        ++k;
    }

    while (i < leftArrSize)
    {
        baseArr[k++] = leftArr[i++];
    }

    while (j < rightArrSize)
    {
        baseArr[k++] = rightArr[j++];
    }

    return numInversions;
}