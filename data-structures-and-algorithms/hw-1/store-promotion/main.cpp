#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

unsigned long calcDiscount(std::vector<unsigned long> products, unsigned long k)
{
    unsigned long result = 0;

    for (size_t i = k-1; i < products.size(); i += k)
    {
        result += products[i];
    }

    return result;
}

unsigned long calcTotal(std::vector<unsigned long> products)
{
    unsigned long result = 0;

    for (size_t i = 0; i < products.size(); i++)
    {
        result += products[i];
    }

    return result;
}

bool compare(unsigned long i, unsigned long j)
{
    return i > j;
}

int main()
{
    unsigned long numProducts = 0;
    int k = 0;
    std::vector<unsigned long> products;

    std::cin >> numProducts >> k;

    for (size_t i = 0; i < numProducts; i++)
    {
        int item = 0;
        std::cin >> item;

        products.push_back(item);
    }

    std::sort(products.begin(), products.end(), compare);

    std::cout << (calcTotal(products) - calcDiscount(products, k));

    return 0;
}
