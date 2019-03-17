#include <cmath>
#include <iostream>

void sort(unsigned long * numbers, unsigned int arrLength);

bool compareNums(unsigned long i, unsigned long j);

unsigned int getNumDigits(unsigned long i);

int main()
{
    unsigned int numNumbers = 0;
    unsigned long * numbers = 0;

    std::cin >> numNumbers;

    numbers = new unsigned long[numNumbers];

    for (size_t i = 0; i < numNumbers; i++)
    {
        std::cin >> numbers[i];
    }

    sort(numbers, numNumbers);

    if (numbers[0] > 0)
    {
        for (size_t i = 0; i < numNumbers; i++)
        {
            std::cout << numbers[i];
        }
    }
    else { std::cout << numbers[0]; }
    

    if (numbers != nullptr) { delete[] numbers; }

    return 0;
}

void sort(unsigned long * numbers, unsigned int arrLength)
{
    for (size_t i = 0; i < arrLength - 1; ++i)
    {
        unsigned int minIndex = i;

        for (size_t j = i + 1; j < arrLength; ++j)
        {
            if (!compareNums(numbers[minIndex], numbers[j])) { minIndex = j; }
        }

        std::swap(numbers[i], numbers[minIndex]);
    }
}

bool compareNums(unsigned long i, unsigned long j)
{
    unsigned int iDigits = getNumDigits(i);
    unsigned int jDigits = getNumDigits(j);

    unsigned long iVal = i * pow(10, jDigits) + j;
    unsigned long jVal = j * pow(10, iDigits) + i;

    return iVal < jVal ? false : true;
}

unsigned int getNumDigits(unsigned long i)
{
    const unsigned int MIN_AMOUNT_OF_DIGITS = 1;

    return (i > 0) ? (int)(log10(i) + MIN_AMOUNT_OF_DIGITS) : MIN_AMOUNT_OF_DIGITS;
}