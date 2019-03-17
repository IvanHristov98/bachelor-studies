#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

struct Pair
{
    int sock1;
    int sock2;

    unsigned int getDifferenceBetweenSocks()
    {
        return abs(this->sock1 - this->sock2);
    }

    int getShorterSock()
    {
        if (this->sock1 < this->sock2) { return this->sock1; }

        return this->sock2;
    }
};

std::vector<Pair> groupPairs(std::vector<unsigned int> socks, unsigned int numSocks);

bool pairSort(Pair pair1, Pair pair2);

int main() 
{
    unsigned int numSocks = 0;
    unsigned int numDay = 0;
    
    std::vector<unsigned int> socks;

    std::cin >> numSocks >> numDay;

    for (size_t i = 0; i < numSocks; i++)
    {
        int sock = 0;
        std::cin >> sock;

        socks.push_back(sock);
    }

    std::vector<Pair> pairs = groupPairs(socks, numSocks);
    std::sort(pairs.begin(), pairs.end(), pairSort);

    std::cout << pairs.at(numDay-1).sock1 << " " << pairs.at(numDay-1).sock2;

    return 0;
}

std::vector<Pair> groupPairs(std::vector<unsigned int> socks, unsigned int numSocks)
{
    std::vector<Pair> pairs;

    for (size_t i = 0; i < numSocks; i++)
    {
        for (size_t j = i+1; j < numSocks; j++)
        {
            Pair pair = Pair();

            pair.sock1 = socks.at(i);
            pair.sock2 = socks.at(j);

            pairs.push_back(pair);
        }
    }

    return pairs;
}

bool pairSort(Pair pair1, Pair pair2)
{
    if (pair1.getDifferenceBetweenSocks() == pair2.getDifferenceBetweenSocks())
    {
        return pair1.getShorterSock() < pair2.getShorterSock();
    }

    return pair1.getDifferenceBetweenSocks() < pair2.getDifferenceBetweenSocks();
}