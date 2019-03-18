#include <iostream>

const int MAX_SIZE = 1235;
bool buckets[MAX_SIZE];

bool mesh[MAX_SIZE][MAX_SIZE];

struct Node 
{
    short data;
    Node * next;
};

Node * head = nullptr;
Node * tail = nullptr;

void pushBack(short data)
{
    Node * tmp = new Node();
    tmp->data = data;
    tmp->next = nullptr;

    if (head == nullptr)
    {
        head = tmp;
        tail = tmp;
        return;
    }

    tail->next = tmp;
    tail = tail->next;
}

int main()
{
    long numElements = 0;
    std::cin >> numElements;

    for (size_t i = 0; i < numElements; ++i)
    {
        short number = 0;
        std::cin >> number;
        pushBack(number);
    }

    long numNumbers = 0;
    std::cin >> numNumbers;

    for (size_t i = 0; i < numNumbers; ++i)
    {
        long long number = 0;
        std::cin >> number;

        if (number < MAX_SIZE)
        {
            buckets[number] = 1;
        }
    }

    Node * tmp = head;
    long found = 0;

    while (tmp->next != nullptr)
    {
        if (buckets[tmp->data] && buckets[tmp->next->data] && !mesh[tmp->data][tmp->next->data])
        {
            mesh[tmp->data][tmp->next->data] = 1;
            mesh[tmp->next->data][tmp->data] = 1;

            ++found;
        }

        tmp = tmp->next;
    }

    std::cout << found;

    return 0;
}