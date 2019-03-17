#include <cmath>
#include <iostream>

int main() 
{
    unsigned long numTests = 0;
    std::cin >> numTests;

    for (size_t i = 0; i < numTests; ++i)
    {
        unsigned long boxSize = 0;
        std::cin >> boxSize;
        std::cout << (std::ceil(log2(boxSize + 1))) << std::endl;
    }

    return 0;
}