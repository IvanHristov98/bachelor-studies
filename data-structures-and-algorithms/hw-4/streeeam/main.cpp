#include <iostream>

struct Node
{
    short data;
    Node * next;
};

Node * head;
Node * tail;
short max;
short min;
long long sum;

void iterate()
{
    Node * tmp = head;
    
    while (tmp != nullptr)
    {
        max = (tmp->data > max) ? tmp->data : max;
        min = (tmp->data < min) ? tmp->data : min;
        sum += tmp->data;
        
        tmp = tmp->next;
    }
}

void pushBack(int data)
{
    Node * newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;
    Node * tmp = head;

    if (head == nullptr)
    {
        min = data;
        max = data;
        
        head = newNode;
        tail = newNode;
        
        return;
    }
    
    tail->next = newNode;
    tail = tail->next;
}

int main() 
{   
    std::ios::sync_with_stdio(false);

    short input = 0;
    std::cin >> input;
    
    do
    {
        pushBack(input);
    }
    while (std::cin >> input);
    
    iterate();

    std::cout << min << " " << max << " " << sum;

    return 0;
}