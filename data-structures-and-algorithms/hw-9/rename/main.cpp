#include <iostream>
#include <string>

struct Node
{
    bool isBreakPoint;
    char letter;
    bool isEndOfName;
    int found;

    Node ** next;

    Node (char letter) : next(nullptr)
    {
        this->found = 0;
        this->isEndOfName = false;
        this->letter = letter;

        // initializing collection of child nodes addressed to each letter
        this->next = new Node*['z'-'a'+1];
        for (size_t i = 0; i < 'z' - 'a' + 1; i++) { this->next[i] = NULL; }
    }
};

class Trie
{
private:
    Node * root;

public:
    Trie():root(NULL)
    {
        this->root = new Node('a');
    }

    void addString(std::string word)
    {
        Node * tmp = this->root;
        bool foundEndPoint = false;

        for (std::string::iterator it = word.begin(); it != word.end(); ++it)
        {
            // if no such node exists
            if (tmp->next[*it - 'a'] == NULL) 
            { 
                // then create it
                tmp->next[*it - 'a'] = new Node(*it);
                // and assign it as an end point
                if (!foundEndPoint)
                {
                    tmp->next[*it - 'a']->isBreakPoint = true;//
                    foundEndPoint = true;
                    std::cout << *it;
                }
            }

            // moving on to the next one
            tmp = tmp->next[*it - 'a'];

            if (!foundEndPoint)
            {
                std::cout << tmp->letter;
            }

            // checking at the end if the word has been previously found
            if (!foundEndPoint && tmp->found > 0 && it+1 == word.end())
            {
                tmp->found++;

                if (tmp->found > 1) { std::cout << " " << tmp->found; }
            }
            else if (it + 1 == word.end())
            {
                tmp->found = 1;
            }
        }
    }
};

int main() 
{
    Trie * trie = new Trie();

    int n = 0; std::cin >> n;
    std::string word;

    for (int i = 0; i < n; i++)
    {
        std::cin >> word;
        trie->addString(word);
        std::cout << std::endl;
    }


    return 0;
}
