#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

unsigned int getLetterQuantityInString(std::string haystack, char letter);

void initBuckets(std::vector<std::string>& buckets);

int main() 
{
    std::string needle;
    std::string haystack;

    std::vector<std::string> buckets;
    initBuckets(buckets);

    std::cin >> needle >> haystack;

    for (size_t i = 0; i < needle.length(); ++i)
    {
        unsigned short index = (unsigned short)(needle.at(i) - 'a');

        if (buckets.at(index).length() < getLetterQuantityInString(haystack, needle.at(i)))
        {
            buckets.at(index) += needle.at(i);
        }
    }

    for (size_t i = 0; i < buckets.size(); ++i)
    {
        std::cout << buckets.at(i);
    }

    return 0;
}

unsigned int getLetterQuantityInString(std::string haystack, char letter)
{
    int numTimes = 0;

    for (size_t i = 0; i < haystack.length(); ++i)
    {
        if (haystack.at(i) == letter) { ++numTimes; }
    }

    return numTimes;
}

void initBuckets(std::vector<std::string>& buckets)
{
    unsigned short numLetters = (unsigned short)('z' - 'a') + 1;

    for (size_t i = 0; i < numLetters; ++i)
    {
        buckets.push_back("");
    }
}