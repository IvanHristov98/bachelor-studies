#include <iostream>
#include <list>

struct Node
{
    int data;
    bool isSeen;
    std::list<Node *> links;

    Node(int data)
    {
        this->data = data;
        this->isSeen = 0;
    }
};

int findPaths(Node * node, int maxStreak, int streak = 0)
{
    // setting status to being seen
    node->isSeen = 1;

    // start from 0
    if (node == nullptr) { return 0; }

    if (node->data == 1) { streak++; }
    else { streak = 0; }

    if (streak > maxStreak) { return 0; }

    // error found doesn't check already seen nodes
    //if (node->links.empty()) { return 1; }

    // else when there are leaves
    int found = 0;
    int numSeen = 0;
    int initialSize = node->links.size();

    while (!node->links.empty())
    {
        Node * tmp = node->links.front();
        node->links.pop_front();

        // recursively increase number of found
        if (!tmp->isSeen)
        {
            found += findPaths(tmp, maxStreak, streak);
        }
        else { numSeen++; }
    }

    if (initialSize - numSeen == 0) { return 1; }

    return found;
}


int main() 
{
    int numNodes = 0; std::cin >> numNodes;
    // holds number of starbucks cafes Dido can withstand
    int maxStreak = 0; std::cin >> maxStreak;

    Node ** nodes = new Node*[numNodes];
    int val = 0;

    // Saving nodes
    for (int i = 0; i < numNodes; ++i)
    {
        std::cin >> val;
        nodes[i] = new Node(val);
    }

    // Adding
    int parent = 0;
    int child = 0;

    for (int i = 0; i < numNodes - 1; ++i)
    {
        std::cin >> parent >> child;
        // root starts from 1
        nodes[parent-1]->links.push_back(nodes[child-1]);
        // it is an unoriented link
        nodes[child - 1]->links.push_back(nodes[parent-1]);
    }

    std::cout << findPaths(nodes[0], maxStreak, 0);

    return 0;
}
