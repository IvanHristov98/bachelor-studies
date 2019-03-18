#include <iostream>

int main()
{
    long numClonings = 0;
    std::cin >> numClonings;

    long * clones = new long[numClonings + 1];
    long * ancestors = new long[numClonings + 1];
    long long totalMass = 0;

    clones[0] = 0;
    ancestors[0] = 0;

    for (size_t i = 0; i < numClonings; ++i)
    {
        int index = 0;
        int mass = 0;
        std::cin >> index;
        std::cin >> mass;

        if (mass == 0)
        {
            mass += clones[ancestors[index]];
            ancestors[i + 1] = ancestors[ancestors[index]];
        }
        else
        {
            mass += clones[index];
            ancestors[i + 1] = index;
        }

        totalMass += mass;

        clones[i + 1] = mass;
    }

    std::cout << totalMass;

    return 0;
}