#include <iostream>

const unsigned char FIRST_LETTER_FROM_ALPHABET = 'a';
const unsigned char LAST_LETTER_FROM_ALPHABET = 'z';

unsigned int getIndexOfGreatestNumOfDifferentLetters(unsigned int * buckets, unsigned int numBuckets, const char symbol);

bool areNeihgbourLettersDifferent(unsigned int * buckets, unsigned int numBuckets, unsigned int index);

int main() 
{
    unsigned int numBuckets = (unsigned int)(LAST_LETTER_FROM_ALPHABET - FIRST_LETTER_FROM_ALPHABET + 1);
    unsigned int * buckets = new unsigned int[numBuckets];

    for (size_t i = 0; i < numBuckets; ++i) { buckets[i] = 0; }

    char letter = 0;

    while (true)
    {
        letter = getchar();

        if ((unsigned int)letter >= FIRST_LETTER_FROM_ALPHABET && (unsigned int)letter <= LAST_LETTER_FROM_ALPHABET)
        {
            ++buckets[(unsigned int)(letter - FIRST_LETTER_FROM_ALPHABET)];

            continue;
        }

        break;
    }

    if (areNeihgbourLettersDifferent(buckets, numBuckets, getIndexOfGreatestNumOfDifferentLetters(buckets, numBuckets, -1)))
    {
        std::cout << "Yes";
    }
    else { std::cout << "No"; }

    if (buckets != nullptr) { delete [] buckets; }

    return 0;
}

unsigned int getIndexOfGreatestNumOfDifferentLetters(unsigned int * buckets, unsigned int numBuckets, const char symbol)
{
    unsigned int maximum = 0;

    if (maximum == symbol - FIRST_LETTER_FROM_ALPHABET) { ++maximum; }

    for (size_t i = 0; i < numBuckets; ++i)
    {
        if (i != (symbol - FIRST_LETTER_FROM_ALPHABET))
        {
            maximum = (buckets[i] > buckets[maximum]) ? i : maximum;
        }
    }

    return maximum;
}

bool areNeihgbourLettersDifferent(unsigned int * buckets, unsigned int numBuckets, unsigned int index)
{
    if (buckets[index] > 0) 
    {
        --buckets[index];

        unsigned int newIndex = getIndexOfGreatestNumOfDifferentLetters(buckets, numBuckets, index + FIRST_LETTER_FROM_ALPHABET);

        if (buckets[newIndex] == 0)
        {
            return (buckets[index] == 0) ? true : false;
        }

        return areNeihgbourLettersDifferent(buckets, numBuckets, newIndex);
    }

    return true;
}