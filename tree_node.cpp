#include "tree_node.h"
#include <cstddef>

using namespace std;

template <typename T>
TreeNode<T>::TreeNode()
{
    this->maxKeys = this->getMaxKeys();
    this->keys = new T[maxKeys];
    this->pointers = new TreeNode<T> *[maxKeys + 1];
    for (int i = 0; i < maxKeys + 1; i++)
    {
        pointers[i] = NULL;
    }
}

template <typename T>
TreeNode<T>::TreeNode(size_t blockSize)
{
    this->blockSize = blockSize;
    TreeNode();
}

template <typename T>
T TreeNode<T>::getKey(int index)
{
    return this->keys[index];
}

template <typename T>
TreeNode<T>* TreeNode<T>::getPointer(int index)
{
    return this->pointers[index];
}

template <typename T>
int TreeNode<T>::getNumOfKeys()
{
    return this->numOfKeys;
}

template <typename T>
bool TreeNode<T>::getLeaf()
{
    return this->isLeaf;
}

template <typename T>
void TreeNode<T>::setKey(int index, T value)
{
    this->keys[index] = value;
}

template <typename T>
void TreeNode<T>::setPointer(int index, TreeNode<T>* pointer)
{
    this->pointers[index] = pointer;
}

template <typename T>
void TreeNode<T>::setNumOfKeys(int numOfKeys)
{
    this->numOfKeys = numOfKeys;
}

template <typename T>
void TreeNode<T>::setLeaf(bool isLeaf)
{
    this->isLeaf = isLeaf;
}

template <typename T>
int TreeNode<T>::getMaxKeys()
{
    // TODO: 回头加上了blocksize一起算
    return 3; // 先hardcode
}
