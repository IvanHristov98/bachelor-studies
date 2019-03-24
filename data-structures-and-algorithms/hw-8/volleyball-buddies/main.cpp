#include <iostream>

struct Node
{
    int data;
    Node * left;
    Node * right;

    Node(int data) : left(NULL), right(NULL)
    {
        this->data = data;
    }
};

class BST
{
private:
    Node * root;
    int size;

public:
    BST() : root(NULL)
    {
        this->size = 0;
    }

    bool isPresent(int data)
    {
        if (this->root == NULL) { return false; }

        Node * tmp = this->root;

        while (tmp != NULL)
        {
            if (data < tmp->data)
            {
                tmp = tmp->left;
            }
            else if (data > tmp->data)
            {
                tmp = tmp->right;
            }
            else { return true; }
        }

        return false;
    }

    void insert(int data)
    {
        if (this->root == NULL) { this->root = new Node(data); ++size; return; }

        Node * tmp = this->root;

        while (tmp != NULL)
        {
            if (data < tmp->data)
            {
                if (tmp->left == NULL)
                {
                    tmp->left = new Node(data);
                    ++size;
                    return;
                }

                tmp = tmp->left;
            }
            else if (data > tmp->data)
            {
                if (tmp->right == NULL)
                {
                    tmp->right = new Node(data);
                    ++size;
                    return;
                }

                tmp = tmp->right;
            }
            else
            {
                // it's already in
                return;
            }
        }
    }

    void printPreOrder(Node * root)
    {
        if (root == NULL) { return; }

        std::cout << root->data << " ";
        this->printPreOrder(root->left);
        this->printPreOrder(root->right);
    }

    Node * getRoot()
    {
        return this->root;
    }

    int getSize()
    {
        return this->size;
    }
};

int main() 
{
    BST * tree = new BST();

    tree->printPreOrder(tree->getRoot());

    int m = 0, n = 0;
    std::cin >> m >> n;

    tree->insert(m);

    int n1 = 0, n2 = 0;

    for (int i = 0; i < n; i++)
    {
        std::cin >> n1 >> n2;

        if (tree->isPresent(n1))
        {
            tree->insert(n2);
        }
    }

    std::cout << tree->getSize();
    
    return 0;
}