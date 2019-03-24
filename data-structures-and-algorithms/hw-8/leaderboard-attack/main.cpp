#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct Node
{
    double value;
    Node *left;
    Node *right;
    int height;

    Node(double value, Node *left, Node *right)
    {
        this->value = value;
        this->left = left;
        this->right = right;
        this->height = 1;
    }
};

class AVLTree
{
private:
    Node *root;

    bool containsRecursive(Node *current, double value)
    {
        if (current == NULL)
        {
            return false;
        }

        if (current->value == value)
        {
            return true;
        }

        if (value < current->value)
        {
            return containsRecursive(current->left, value);
        }
        else
        {
            return containsRecursive(current->right, value);
        }
    }

    void printRecursive(Node *current)
    {
        if (current == NULL)
        {
            return;
        }

        printRecursive(current->left);
        cout << current->value << " ";
        printRecursive(current->right);
    }

    int getMax(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int getHeight(Node * node)
    {
        if (node == NULL) { return 0; }

        return node->height;
    }

    int getBalanceFactor(Node * node)
    {
        if (node == NULL) { return 0; }

        return this->getHeight(node->left) - this->getHeight(node->right);
    }

    Node * getMinValueNode(Node * node)
    {
        if (node == NULL) { return node; }

        while (node->left != NULL) { node = node->left; }

        return node;
    }

    Node * rightRotate(Node * y)
    {
        Node * x = y->left;
        Node * T = x->right;

        x->right = y;
        y->left = T;

        y->height = this->getMax(this->getHeight(y->left), this->getHeight(y->right)) + 1;
        x->height = this->getMax(this->getHeight(x->left), this->getHeight(x->right)) + 1;

        return x;
    }

    Node * leftRotate(Node * x)
    {
        Node * y = x->right;
        Node * T = y->left;

        y->left = x;
        x->right = T;

        x->height = this->getMax(this->getHeight(x->left), this->getHeight(x->right)) + 1;
        y->height = this->getMax(this->getHeight(y->left), this->getHeight(y->right)) + 1;

        return y;
    }

    Node * insertNode(Node * node, double data)
    {
        if (node == NULL) { return new Node(data, NULL, NULL); }

        if (data < node->value)
        {
            node->left = this->insertNode(node->left, data);
        }
        else if (data > node->value)
        {
            node->right = this->insertNode(node->right, data);
        }
        else { cout << data << " already added" << endl; return node; }

        node->height = this->getMax(this->getHeight(node->left), this->getHeight(node->right)) + 1;

        int balance = this->getBalanceFactor(node);

        // left left
        if (balance > 1 && data < node->left->value)
        {
            return this->rightRotate(node);
        }

        // left right
        if (balance > 1 && data > node->left->value)
        {
            node->left = this->leftRotate(node->left);
            return this->rightRotate(node);
        }
        
        // right right
        if (balance < -1 && data > node->right->value)
        {
            return this->leftRotate(node);
        }

        // right left
        if (balance < -1 && data < node->right->value)
        {
            node->right = this->rightRotate(node->right);    
            return this->leftRotate(node);
        }

        return node;
    }

    Node * deleteNode(Node * node, double data)
    {
        if (node == NULL) { cout << data << " not found to remove" << endl; return NULL; }

        if (data < node->value)
        {
            node->left = this->deleteNode(node->left, data);
        }
        else if (data > node->value)
        {
            node->right = this->deleteNode(node->right, data);
        }
        else
        {
            if (node->left == NULL || node->right == NULL)
            {
                Node * tmp = node->left ? node->left : node->right;

                if (tmp == NULL)
                {
                    tmp = node;
                    node = NULL;
                }
                else { *node = *tmp; }

                delete tmp;
            }
            else
            {
                Node * smallest = this->getMinValueNode(node->right);
                node->value = smallest->value;
                node->right = this->deleteNode(node->right, smallest->value);
            }
        }

        if (node == NULL) { return NULL; }

        node->height = this->getMax(this->getHeight(node->left), this->getHeight(node->right)) + 1;
        
        int balance = this->getBalanceFactor(node);

        // left left
        if (balance > 1 && this->getBalanceFactor(node->left) >= 0)
        {
            return this->rightRotate(node);
        }

        // left right
        if (balance > 1 && this->getBalanceFactor(node->left) < 0)
        {
            node->left = this->leftRotate(node->left);
            return this->rightRotate(node);
        }

        // right right
        if (balance < -1 && this->getBalanceFactor(node->right) <= 0)
        {
            return this->leftRotate(node);
        }

        // right left
        if (balance < -1 && this->getBalanceFactor(node->right) > 0)
        {
            node->right = this->rightRotate(node->right);
            return this->leftRotate(node);
        }

        return node;
    }

public:
    AVLTree() : root(NULL)
    {}

    void add(double value)
    {
        this->root = this->insertNode(this->root, value);
    }

    void remove(double value)
    {
        this->root = this->deleteNode(this->root, value);
    }

    bool contains(double value)
    {
        if (root == NULL)
        {
            return false;
        }

        return containsRecursive(root, value);
    }

    void print()
    {
        if (root == NULL)
        {
            return;
        }

        printRecursive(root);
        cout << endl;
    }
};

int main()
{
    AVLTree tree;
    string operation;
    double number;
    int N;

    cin >> N;
    cout << fixed;

    for (size_t i = 0; i < N; i++)
    {
        cin >> operation;
        if (operation != "print")
        {
            cin >> number;
        }

        if (operation == "add")
        {
            tree.add(number);
        }
        else if (operation == "remove")
        {
            tree.remove(number);
        }
        else if (operation == "contains")
        {
            if (tree.contains(number))
            {
                cout << "yes" << endl;
            }
            else
            {
                cout << "no" << endl;
            }
        }
        else if (operation == "print")
        {
            tree.print();
        }
    }

    return 0;
}