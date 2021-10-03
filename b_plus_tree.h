#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "memory_pool.h"
#include "tree_node.h"

#include <cstddef>

class BPlusTree
{
private:
    MemoryPool *disk;
    MemoryPool *index;
    void *diskRootAddress;

    size_t blockSize;
    int maxKeys;
    TreeNode *root;

    int height;
    int numOfNodes;

    void insertInternal(int value, TreeNode *cursor, TreeNode *child);
    TreeNode *findParent(TreeNode *cursor, TreeNode *child);
    int findParentValue(TreeNode *cursor);
    void removeInternal(int x, TreeNode *cursor, TreeNode *child);

public:
    // Constructor
    BPlusTree();
    BPlusTree(size_t blockSize, MemoryPool* disk, MemoryPool* index);

    TreeNode *getRoot();
    void displayTree(TreeNode *cursor);
    void displayNode(TreeNode *cursor);
    
    void insert(int value);
    void search(int leftValue, int rightValue);
    void remove(int x, int &numDel, int &numUpd, bool &noMore);
    void getFirstLeaf();

    int getNumOfNodes();
    int getHeight();
    int getMaxKeys();
};

#endif
