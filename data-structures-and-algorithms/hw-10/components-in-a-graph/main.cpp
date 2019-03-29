#include <iostream>
#include <list>

class Graph
{
private:
    int size;
    std::list<int> * adj;

public:
    Graph(int size) : adj(NULL)
    {
        this->size = size;
        this->adj = new std::list<int>[this->size];
    }

    void insertEdge(int v, int w)
    {
        this->adj[v].push_back(w);
    }

    void BFS()
    {
        bool * visited = new bool[this->size];
        for (int i = 0; i < this->size; ++i) { visited[i] = false; }

        int minConnectedSubGraphSize = -1;
        int maxConnectedSubGraphSize = -1;

        for (int i = 0; i < this->size; ++i)
        {
            if (!visited[i])
            {
                int SubGraphLength = 0;

                visited[i] = true;

                std::list<int> queue;
                queue.push_back(i);

                while (!queue.empty())
                {
                    int node = queue.front();
                    queue.pop_front();

                    for (std::list<int>::iterator iter = this->adj[node].begin(); iter != this->adj[node].end(); ++iter)
                    {
                        if (!visited[*iter])
                        {
                            visited[*iter] = true;
                            queue.push_back(*iter);
                        }
                    }

                    ++SubGraphLength;
                }

                if (SubGraphLength > maxConnectedSubGraphSize) { maxConnectedSubGraphSize = SubGraphLength; }

                if ((SubGraphLength < minConnectedSubGraphSize || minConnectedSubGraphSize == -1) && SubGraphLength != 1)
                {
                    minConnectedSubGraphSize = SubGraphLength;
                }
            }
        }

        std::cout << minConnectedSubGraphSize << " " << maxConnectedSubGraphSize;
    }
};


int main()
{
    int n = 0; std::cin >> n;
    Graph * g = new Graph(2*n);
    int gi = 0, bi = 0;

    for (int i = 0; i < n; ++i)
    {
        std::cin >> gi >> bi;

        g->insertEdge(gi-1, bi-1);
        g->insertEdge(bi-1, gi-1);
    }

    g->BFS();
    
    return 0;
}