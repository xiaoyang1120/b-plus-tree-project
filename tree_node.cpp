#include "tree_node.h"
#include <cstddef>

using namespace std;

TreeNode::TreeNode()
{
    this->maxKeys = 3;
    this->keys = new int[maxKeys];
    this->pointers = new TreeNode *[maxKeys + 1];
}

TreeNode::TreeNode(size_t blockSize)
{
    this->blockSize = blockSize;
    this->maxKeys = TreeNode::calculateMaxKeys(blockSize);
    this->keys = new int[maxKeys];
    this->pointers = new TreeNode *[maxKeys + 1];
    for (int i = 0; i <= maxKeys; i++)
    {
        this->pointers[i] = NULL;
    }
}

int TreeNode::getKey(int index)
{
    return this->keys[index];
}

TreeNode* TreeNode::getPointer(int index)
{
    return this->pointers[index];
}

int TreeNode::getNumOfKeys()
{
    return this->numOfKeys;
}

bool TreeNode::getLeaf()
{
    return this->isLeaf;
}

void TreeNode::setKey(int index, int value)
{
    this->keys[index] = value;
}

void TreeNode::setPointer(int index, TreeNode* pointer)
{
    this->pointers[index] = pointer;
}

void TreeNode::setNumOfKeys(int numOfKeys)
{
    this->numOfKeys = numOfKeys;
}

void TreeNode::setLeaf(bool isLeaf)
{
    this->isLeaf = isLeaf;
}

int TreeNode::calculateMaxKeys(size_t blockSize)
{
    int sum = sizeof(TreeNode*);
    int maxKeys = 0;
    while (sum + sizeof(int) + sizeof(TreeNode*) <= blockSize) {
        maxKeys++;
        sum += sizeof(int) + sizeof(TreeNode*);
    }
    
    // return maxKeys;
    return 3;
}
