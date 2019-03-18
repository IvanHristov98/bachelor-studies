#include <iostream>

struct Node
{
    int data;
    Node * next;
};

Node * head = nullptr;
Node * tail = nullptr;

void pushBack(long data)
{
    Node * tmp = new Node();
    tmp->data = data;
    tmp->next = nullptr;

    if (::head == nullptr)
    {
        ::head = tmp;
        ::tail = tmp;
        return;
    }

    ::tail->next = tmp;
    ::tail = ::tail->next;
}


int main()
{
    unsigned int numKnights = 0;
    std::cin >> numKnights;

    if (numKnights <= 2)
    {
        std::cout << 1;
    }
    else
    {
        for (size_t i = 1; i <= numKnights; i += 2)
        {
            pushBack(i);
        }

        ::tail->next = ::head;

        Node * current = (numKnights % 2 == 0) ? ::head : ::tail;

        Node * prev = nullptr;
        Node * toDelete = nullptr;

        while (current->next != current && current->next != nullptr)
        {
            prev = current;
            toDelete = current->next;
            current = current->next->next;
            prev->next = current;

            delete toDelete;
        }

        std::cout << prev->data;
    }

    return 0;
}