#include <iostream>
#include <string>

struct Node
{
    long long data;
    Node * next;
};

Node * top;

void push(long long data)
{
    Node * tmp = new Node();
    tmp->data = data;
    tmp->next = ::top;
    ::top = tmp;
}

int pop()
{
    if (::top == nullptr) { return 0; }

    Node * tmp = ::top;
    ::top = ::top->next;

    long result = tmp->data;
    delete tmp;

    return result;
}

void reverse()
{
    Node *prev, *current, *next;
    current = ::top;
    prev = nullptr;

    while (current != nullptr)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    ::top = prev;
}

long long popSumFewElements(long long numStonesToRemove)
{
    long long result = 0;

    for (size_t i = 0; i < numStonesToRemove; ++i)
    {
        result += pop();
    }

    return result;
}

long long popMaxFewElements(long long numStonesToRemove)
{
    long long max = pop();
    long long popVal = 0;

    for (size_t i = 0; i < numStonesToRemove - 1; ++i)
    {
        popVal = pop();
        max = (popVal > max) ? popVal : max;
    }

    return max;
}


int main() 
{
    long numStones = 0;
    std::cin >> numStones;

    std::string colour;
    long long data = 0;

    for (size_t i = 0; i < numStones; i++)
    {
        std::cin >> colour >> data;

        if (colour == "white") 
        { 
            push(data); 
        }
        else if (colour == "green")
        {
            push(popSumFewElements(data));
        }
        else if (colour == "blue")
        {
            push(popMaxFewElements(data));
        }
    }

    reverse();

    while (::top != nullptr)
    {
        std::cout << pop() << " ";
    }

    return 0;
}
