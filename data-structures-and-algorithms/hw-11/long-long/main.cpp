#include <iostream>
#include <vector>

struct Edge
{
    int from;
    int to;
    long long weight;

    Edge()
    {}
};

int numVertices = 0, numEdges = 0;
Edge * edges;

long long getMaxDist(int start, int finish)
{
    std::vector<bool> marked(numVertices, false);
    std::vector<long long> distances(numVertices, 0);

    marked[start] = true;

    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numEdges; j++)
        {
            if (!marked[edges[j].from]) { continue; }

            long long cand = distances[edges[j].from] - edges[j].weight;

            if (cand < distances[edges[j].to] || !marked[edges[j].to])
            {
                distances[edges[j].to] = cand;
                marked[edges[j].to] = true;
            }
        }
    }

    if (marked[finish]) { return -distances[finish]; }
    else
    {
        return -1;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    int start = 0, finish = 0;
    std::cin >> numVertices >> numEdges >> start >> finish;

    edges = new Edge[numEdges];

    int v1 = 0, v2 = 0;
    long long weight = 0;

    for (int i = 0; i < numEdges; ++i)
    {
        std::cin >> edges[i].from >> edges[i].to >> edges[i].weight;
    }

    std::cout << getMaxDist(start, finish);
    
    return 0;
}