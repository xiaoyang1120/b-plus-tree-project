#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "memory_pool.h"
#include "tree_node.h"

#include <cstddef>

template <typename T>
class BPlusTree
{
private:
    MemoryPool *disk;
    MemoryPool *index;
    void *diskRootAddress;

    size_t blockSize;
    int maxKeys;
    TreeNode<T> *root;

    int getMaxKeys();
    void insertInternal(T value, TreeNode<T> *cursor, TreeNode<T> *child);
    TreeNode<T> *findParent(TreeNode<T> *cursor, TreeNode<T> *child);
public:
    // Constructor
    BPlusTree();
    BPlusTree(size_t blockSize, MemoryPool* disk, MemoryPool* index);

    TreeNode<T> *getRoot();
    void levelDisplay(TreeNode<T> *cursor);
    void insert(T value); // 其实应该要带record
    void search(T leftValue, T rightValue);
};

#endif