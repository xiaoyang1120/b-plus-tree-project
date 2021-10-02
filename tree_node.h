#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "address.h"

#include <cstddef>

class TreeNode 
{
private:
    Address *disk;
    int *keys;
    TreeNode **pointers;
    int numOfKeys;
    int maxKeys;
    size_t blockSize;
    bool isLeaf; 

    int getMaxKeys();

public:
    // Constructor
    TreeNode();
    TreeNode(size_t blockSize);

    int getKey(int index);
    TreeNode* getPointer(int index);
    int getNumOfKeys();
    bool getLeaf();
    Address* getDisk(){ return disk; };

    void setKey(int index, int value);
    void setPointer(int index, TreeNode* pointer);
    void setNumOfKeys(int numOfKeys);
    void setLeaf(bool isLeaf);
};
#endif