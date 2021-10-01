#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "tree_node.h"
#include <cstddef>

template <typename T>
class BPlusTree
{
private:
    size_t blockSize;
    int maxKeys;
    TreeNode<T> *root;

    int getMaxKeys();
    void insertInternal(T value, TreeNode<T> *cursor, TreeNode<T> *child);
    TreeNode<T> *findParent(TreeNode<T> *cursor, TreeNode<T> *child);
public:
    // Constructor
    BPlusTree();
    BPlusTree(size_t blockSize);

    TreeNode<T> *getRoot();
    void levelDisplay(TreeNode<T> *cursor);
    void insert(T value); // 其实应该要带record
};

#endif