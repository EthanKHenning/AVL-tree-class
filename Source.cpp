#include <iostream>
#include <fstream> 
#include <queue>   //level-order traversal
#include <string>
#include <algorithm> //max func
using namespace std;

class Node
{
private:
    int key;   
    Node* left;
    Node* right;
    int height;

public:
    Node()
    {
        right = nullptr;
        left = nullptr;
        height = 1;
        key = 0;     
    }

    int getKey() 
    { 
        return key; 
    }

    void setKey(int inputKey) 
    { 
        key = inputKey; 
    }

    Node* getRight() 
    { 
        return right; 
    }

    void setRight(Node* inputRight) 
    { 
        right = inputRight; 
    }

    int getHeight() 
    { 
        return height; 
    }

    void setHeight(int h) 
    { 
        height = h;
    }

    Node* getLeft() 
    { 
        return left; 
    }

    void setLeft(Node* inputLeft) 
    { 
        left = inputLeft; 
    }
};

class AVLBST
{
private:
    Node* root;

public:

    AVLBST()
    {
        root = nullptr;
    }

    int height(Node* node) 
    {
        if (node == nullptr) 
        {
            return 0;
        }
        return node->getHeight();
    }

    //calculate balance factor of a node
    int getBalance(Node* node) 
    {
        if (node == nullptr) 
        {
            return 0;
        }
        return height(node->getLeft()) - height(node->getRight());
    }

    // left rotation
    Node* leftRotate(Node* x) 
    {
        Node* y = x->getRight();
        Node* T2 = y->getLeft();

        y->setLeft(x);
        x->setRight(T2);

        x->setHeight(max(height(x->getLeft()), height(x->getRight())) + 1);
        y->setHeight(max(height(y->getLeft()), height(y->getRight())) + 1);

        return y;
    }

    // right rotation
    Node* rightRotate(Node* y) 
    {
        Node* x = y->getLeft();
        Node* T2 = x->getRight();

        x->setRight(y);
        y->setLeft(T2);

        y->setHeight(max(height(y->getLeft()), height(y->getRight())) + 1);
        x->setHeight(max(height(x->getLeft()), height(x->getRight())) + 1);

        return x;
    }

    // Rebalance
    Node* rebalance(Node* node) 
    {
        node->setHeight(max(height(node->getLeft()), height(node->getRight())) + 1);
        int balance = getBalance(node);

        // left Heavy
        if (balance > 1) 
        {
            if (getBalance(node->getLeft()) < 0) 
            {
                node->setLeft(leftRotate(node->getLeft())); // LR case
            }
            return rightRotate(node); // LL case
        }

        // Right Heavy
        if (balance < -1) 
        {
            if (getBalance(node->getRight()) > 0) 
            {
                node->setRight(rightRotate(node->getRight())); // RL case
            }
            return leftRotate(node); // RR case
        }

        return node; // balanced
    }

    Node* insert(Node* node, int input) 
    {
        if (node == nullptr) 
        {
            Node* newNode = new Node();
            newNode->setKey(input);
            return newNode;
        }

        if (input < node->getKey()) 
        {
            node->setLeft(insert(node->getLeft(), input));
        }
        else if (input > node->getKey()) 
        {
            node->setRight(insert(node->getRight(), input));
        }
        else 
        {
            return node; // no duplicates
        }

        return rebalance(node);
    }

    void insert(int key) 
    {
        root = insert(root, key);
    }

    // level traversal / print tree
    void levelOrder(Node* root, ofstream& outFile)
    {
        if (!root) return;

        queue<pair<Node*, int>> q; // pair to store the node and its level
        q.push({ root, 0 });         // Start with root at level 0

        while (!q.empty())
        {
            Node* node = q.front().first; // extract the node
            int level = q.front().second; // extract the level
            q.pop();

            
            outFile << "Key: " << node->getKey()
                << " Level: " << level
                << " Height: " << node->getHeight()
                << " Balance Factor: " << getBalance(node) << endl;

            // enqueue left and right children with their level
            if (node->getLeft()) q.push({ node->getLeft(), level + 1 });
            if (node->getRight()) q.push({ node->getRight(), level + 1 });
        }
    }

    void displayTree(ofstream& outFile) 
    {
        levelOrder(root, outFile);
    }
};

int main() 
{
    AVLBST tree;

    ifstream inputFile("input.txt");
    if (!inputFile) 
    {
        cout << "Failed to open input file." << endl;
        return -1;
    }

    int key;
    while (inputFile >> key) 
    {
        tree.insert(key);
    }

    inputFile.close();

    ofstream outputFile("output.txt");
    if (!outputFile) 
    {
        cout << "Failed to open output file." << endl;
        return -1;
    }

    tree.displayTree(outputFile);

    outputFile.close();

    return 0;
}
