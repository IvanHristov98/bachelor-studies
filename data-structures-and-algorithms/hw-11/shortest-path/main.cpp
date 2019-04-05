#include <iostream>
#include <vector>
#include <limits.h>
#include <queue>

std::vector<std::vector<std::pair<unsigned int, long>>> adjList;
unsigned int n = 0;

std::vector<long long> dijkstra(unsigned int start)
{
    std::vector<long long> distances(n, LLONG_MAX);
    std::vector<bool> marked(n, false);
    distances[start] = 0;


    using pii = std::pair<unsigned int, long>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> queue;
    queue.push({distances[start], start});

    while (!queue.empty())
    {
        int v = queue.top().second;
        queue.pop();

        if (!marked[v])
        {
            marked[v] = true;

            for (auto edge : adjList[v])
            {
                int to = edge.first;
                int weight = edge.second;

                if (!marked[to])
                {
                    if (distances[to] > distances[v] + weight)
                    {
                        distances[to] = distances[v] + weight;
                        queue.push({ distances[to], to });
                    }
                }
            }
        }
    }

    return distances;
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    short t = 0; std::cin >> t;

    for (short k = 0; k < t; k++)
    {
        unsigned int m = 0;
        std::cin >> n >> m;
        adjList.resize(0);
        adjList.resize(n);

        unsigned int u = 0; unsigned int v = 0; long w = 0;

        for (unsigned int i = 0; i < m; i++)
        {
            std::cin >> u >> v >> w;

            adjList[u - 1].push_back({ v - 1, w });
            adjList[v - 1].push_back({ u - 1, w });
        }

        unsigned int s = 0; std::cin >> s;

        std::vector<long long> distances = dijkstra(s - 1);

        for (unsigned int i = 0; i < n; i++)
        {
            if (i != s - 1)
            {
                if (distances[i] == LLONG_MAX) { std::cout << "-1 "; }
                else
                {
                    std::cout << distances[i] << " ";
                }
            }
        }

        std::cout << std::endl;
    }

    return 0;
}