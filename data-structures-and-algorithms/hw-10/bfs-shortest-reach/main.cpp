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

    void insertItem(int v, int w)
    {
        this->adj[v].push_back(w);
    }

    int * BFS(int s)
    {
        int * elements = new int[this->size];
        bool * visited = new bool[this->size];
        
        for (int i = 0; i < this->size; i++) { elements[i] = -1; visited[i] = false; }

        std::list<int> wave;
        std::list<int> levels;

        wave.push_back(s);
        levels.push_back(0);
        visited[s] = true;

        while (!wave.empty())
        {
            int idx = wave.front();
            int lvl = levels.front();
            wave.pop_front();
            levels.pop_front();

            for (std::list<int>::iterator it = this->adj[idx].begin(); it != this->adj[idx].end(); ++it)
            {
                if (!visited[*it])
                {
                    visited[*it] = true;

                    wave.push_back(*it);
                    levels.push_back(lvl+6);

                    // handling resulint array
                    elements[*it] = lvl + 6;
                }
            }
        }

        delete[] visited;
        return elements;
    }
};

int main()
{
    int q = 0; std::cin >> q;

    for (int i = 0; i < q; ++i)
    {
        int n = 0, m = 0; 
        std::cin >> n >> m;
        Graph * g = new Graph(n);
        int u = 0, w = 0, s = 0;

        for (int j = 0; j < m; ++j)
        {
            std::cin >> u >> w;

            g->insertItem(u-1, w-1);
            g->insertItem(w-1, u-1);
        }

        std::cin >> s;

        int * paths = g->BFS(s-1);

        for (int j = 0; j < n; ++j)
        {
            if (j != s-1) { std::cout << (paths[j]) << " "; }
        }
        
        std::cout << std::endl;
    }

    return 0;
}