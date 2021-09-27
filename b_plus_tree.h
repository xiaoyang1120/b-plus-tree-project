#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "record.h"
#include "block.h"

/**
 * A tree node in B+ tree
 **/
template <typename T>
class TreeNode:Block 
{
private:
    // Variables
    Pointer *pointers; // pointer to Block // [maxKeys]
    Key<T> *keys; // [maxKeys + 1]
    int numOfKeys; // current valid keys number
    int maxKeys;
    int blockSize;

    // Helper functions
    int getMaxKeys();

public:
    // Constructor
    TreeNode<T>(size_t blockSize);

    // Methods
};

class Pointer
{
private:
    Block *blockAddress;

public:
    Pointer(Block *blockAddress);
    Block *getBlockAddress();
};

template <typename T>
class Key
{
private:
    T value; // generic

public:
    // Constructor
    Key<T>(T value);

    // Method
    T getValue();
};

template <typename T>
class LeafNode: TreeNode
{
private:

public:
    // Constructor
};

template <typename T>
class NonLeafNone: TreeNode
{
private:
    /* data */
public:
    
};


/**
 * B+ Tree
 **/
template <typename T>
class BPlusTree
{
private:
    TreeNode<T> root;

    void reformatTree();

public:
    // Constructor
    BPlusTree(size_t blockSize);

    // Method
    void insert(DataBlock block, T value);
    Record search(T value);
    void delete(T value);
};

#endif
