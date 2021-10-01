#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <cstddef>

template <typename T>
class TreeNode 
{
private:
    T *keys;
    TreeNode<T> **pointers;
    int numOfKeys;
    int maxKeys;
    size_t blockSize;
    bool isLeaf; 

    int getMaxKeys();

public:
    // Constructor
    TreeNode();
    TreeNode(size_t blockSize);

    T getKey(int index);
    TreeNode<T>* getPointer(int index);
    int getNumOfKeys();
    bool getLeaf();

    void setKey(int index, T value);
    void setPointer(int index, TreeNode<T>* pointer);
    void setNumOfKeys(int numOfKeys);
    void setLeaf(bool isLeaf);
};
#endif
