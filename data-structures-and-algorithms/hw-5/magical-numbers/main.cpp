#include <iostream>

struct Node
{
    long data;
    Node * next;
};

Node * head = nullptr;
Node * tail = nullptr;

void enqueue(long data)
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

long dequeue()
{
    if (::head == nullptr)
    {
        return 0;
    }

    Node * tmp = ::head;
    ::head = ::head->next;
    long result = tmp->data;

    delete tmp;
    return result;
}

bool isEmpty()
{
    if (::head == nullptr) { return true; }

    return false;
}

int main() 
{
    long n = 0;
    std::cin >> n;

    for (size_t i = 1; i <= n && i <= 9; i++)
    {
        enqueue(i);
    }

    while (!isEmpty())
    {
        long num = dequeue();
        short lastDigit = num % 10;

        long leftNum = num * 10 + lastDigit - 2;
        long rightNum = num * 10 + lastDigit + 2;

        if (leftNum > 0 && leftNum <= n && lastDigit - 2 >= 0) { enqueue(leftNum); }

        if (rightNum > 0 && rightNum <= n && lastDigit + 2 <= 9) { enqueue(rightNum); }

        std::cout << num << " ";
    }

    return 0;
}
