#include <queue>
#include <cmath>
#include <iostream>
#include <iomanip>

struct Node
{
    int data;
    unsigned int row;
    Node * left;
    Node * right;

    Node(int data) : left(nullptr), right(nullptr)
    {
        this->data = data;
    }
};

class BinaryTree
{
public:
    Node * root;

    BinaryTree() : root(nullptr)
    {}

    void push(int val, Node * root)
    {
        // If there is no base root
        if (this->root == nullptr)
        {
            this->root = new Node(val);
            return;
        }

        // If equal or greater
        if (root->data <= val)
        {
            if (root->right == nullptr)
            {
                root->right = new Node(val);
            }
            else { this->push(val, root->right); }
        }
        else
        {
            if (root->left == nullptr)
            {
                root->left = new Node(val);
            }
            else { this->push(val, root->left); }
        }
    }

    void printAvgByLevels()
    {
        if (this->root == nullptr)
        {
            return;
        }

        std::queue<Node*> wave;
        this->root->row = 0;
        wave.push(this->root);

        int currentRow = this->root->row;
        double accumulatedSum = 0;
        int numElementsInCurrRow = 0;

        std::cout.precision(2);
        std::cout << std::fixed;

        while (!wave.empty())
        {
            Node * tmp = wave.front();
            wave.pop();

            if (tmp->row > currentRow)
            {
                // updating row
                currentRow = tmp->row;

                std::cout << accumulatedSum / numElementsInCurrRow << std::endl;

                accumulatedSum = 0;
                numElementsInCurrRow = 0;
            }

            accumulatedSum += tmp->data;
            ++numElementsInCurrRow;


            // enqueue next elements
            if (tmp->left != nullptr) { tmp->left->row = tmp->row + 1; wave.push(tmp->left); }
            if (tmp->right != nullptr) { tmp->right->row = tmp->row + 1; wave.push(tmp->right); }
        }

        std::cout << accumulatedSum / numElementsInCurrRow << std::endl;
    }
};

int main() 
{
    BinaryTree * tree = new BinaryTree();

    int numElements = 0;
    std::cin >> numElements;
    int element = 0;

    for (size_t i = 0; i < numElements; ++i)
    {
        std::cin >> element;
        tree->push(element, tree->root);
    }

    tree->printAvgByLevels();

    return 0;
}
