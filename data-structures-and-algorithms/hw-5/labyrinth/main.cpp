#include <iostream>
#include <cmath>

struct Pair
{
    long steps;
    int x;
    int y;
};

struct Node
{
    Pair data;
    Node * next;
};

Node * head = nullptr;
Node * tail = nullptr;

void enqueue(Pair data)
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

Pair dequeue()
{
    Node * tmp = ::head;
    Pair result = ::head->data;
    ::head = ::head->next;

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
    int M = 0;
    int N = 0;
    long numQueries = 0;

    std::cin >> M >> N >> numQueries;

    int ** labyrinth = nullptr;
    labyrinth = new int*[M];

    for (size_t i = 0; i < M; ++i)
    {
        labyrinth[i] = new int[N];
    }

    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            int val = 0;
            std::cin >> val;
            labyrinth[i][j] = (val == 1) ? -1 : 0;
        }
    }

    Pair start = Pair();
    start.x = 0;
    start.y = 0;
    start.steps = 0;
    enqueue(start);

    while (!isEmpty())
    {
        Pair tmp = dequeue();

        for (int i : {-1, 0, 1})
        {
            for (int j : {-1, 0, 1})
            {

                if (abs(i) == abs(j)) { continue; }

                if (tmp.x + i < 0 || tmp.y + j < 0 || tmp.x + i >= M || tmp.y + j >= N) { continue; }

                if (tmp.x + i == 0 && tmp.y + j == 0) { continue; }

                if (labyrinth[tmp.x + i][tmp.y + j] != 0) { continue; }

                Pair position = Pair();
                position.x = tmp.x + i;
                position.y = tmp.y + j;
                position.steps = tmp.steps + 1;

                labyrinth[position.x][position.y] = position.steps;

                enqueue(position);
            }
        }
    }

    int x, y;

    for (size_t i = 0; i < numQueries; ++i)
    {
        std::cin >> x >> y;
        if (!(x == 0 && y == 0) && labyrinth[x][y] == 0) 
        {
            std::cout << -1 << std::endl;
        }
        else
        {
           std::cout << labyrinth[x][y] << std::endl; 
        }
    }

    return 0;
}
