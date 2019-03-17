#include <iostream>

void quickSort(unsigned int * arr, int start, int end);

unsigned int partition(unsigned int * arr, int start, int end);

unsigned int countToys(unsigned int * toys, unsigned int numToys, unsigned int budget);

int main()
{
    unsigned int numToys = 0;
    unsigned int budget = 0;

    std::cin >> numToys >> budget;

    unsigned int * toys = new unsigned int[numToys];

    for (size_t i = 0; i < numToys; i++)
    {
        std::cin >> toys[i];
    }

    quickSort(toys, 0, numToys - 1);

    std::cout << countToys(toys, numToys, budget);

    if (toys != nullptr) { delete[] toys; }

    return 0;
}

void quickSort(unsigned int * arr, int start, int end)
{
    if (start < end)
    {
        unsigned int pIndex = partition(arr, start, end);

        quickSort(arr, start, pIndex - 1);
        quickSort(arr, pIndex + 1, end);
    }
}

unsigned int partition(unsigned int * arr, int start, int end)
{
    unsigned int pivot = arr[end];
    unsigned int partitionIndex = start;

    for (size_t i = start; i < end; ++i)
    {
        if (arr[i] <= pivot)
        {
            std::swap(arr[i], arr[partitionIndex]);
            ++partitionIndex;
        }
    }

    std::swap(arr[partitionIndex], arr[end]);
    return partitionIndex;
}

unsigned int countToys(unsigned int * toys, unsigned int numToys, unsigned int budget)
{
    unsigned int currentValue = 0;

    for (size_t i = 0; i < numToys; ++i)
    {
        currentValue += toys[i];

        if (currentValue > budget) { return i; }
    }

    return numToys;
}