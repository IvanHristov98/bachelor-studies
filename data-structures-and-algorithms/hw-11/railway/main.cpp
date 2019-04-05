#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>

struct Edge
{
    unsigned int to;
    long price;

    Edge(unsigned int to, long price)
    {
        this->to = to;
        this->price = price;
    }
};

std::vector<std::vector<Edge>> roads;
std::vector<Edge> rails;

unsigned int numCities = 0;
unsigned int numRoads = 0;
unsigned int numRailRoads = 0;

struct PQTemplate
{
    unsigned int to;
    long price;
    bool isRail;
    bool operator < (const PQTemplate & other) const
    {
        if (this->price != other.price) { return this->price > other.price; }
        else
        {
            return this->isRail;
        }
    }
};

unsigned int dijkstra(unsigned int start)
{
    std::vector<bool> marked(numCities, false);
    std::vector<long long> distances(numCities, LLONG_MAX);
    std::priority_queue<PQTemplate> queue;
    queue.push({start, 0, false});

    for (auto edge : rails) { queue.push({edge.to, edge.price, true}); }

    unsigned int numToRemove = 0;

    while (!queue.empty())
    {
        PQTemplate curr = queue.top();
        queue.pop();

        if (marked[curr.to]) { continue; }
        marked[curr.to] = true;
        distances[curr.to] = curr.price;

        if (curr.isRail) { numToRemove++; }

        for (auto edge : roads[curr.to])
        {
            if (marked[edge.to]) { continue; }

            queue.push({edge.to, curr.price + edge.price, false});
        }
    }

    return numToRemove;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin >> numCities >> numRoads >> numRailRoads;
    roads.resize(numCities);

    unsigned int u = 0; unsigned int v = 0; long w = 0;

    for (unsigned int i = 0; i < numRoads; ++i)
    {
        std::cin >> u >> v >> w;

        roads[u - 1].push_back(Edge(v - 1, w));
        roads[v - 1].push_back(Edge(u - 1, w));
    }

    for (unsigned int i = 0; i < numRailRoads; ++i)
    {
        std::cin >> v >> w;

        rails.push_back(Edge(v - 1, w));
    }

    std::cout << (numRailRoads-dijkstra(0));

    return 0;
}