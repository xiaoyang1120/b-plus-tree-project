#include "b_plus_tree.h"

using namespace std;

template <typename T>
TreeNode<T>::TreeNode(size_t blockSize) 
{
    this->blockSize = blockSize;
    this->maxKeys = getMaxKeys();

    this->pointers = new Pointer[maxKeys + 1];
    this->keys = new Keys[maxKeys];

    this->numOfKeys = 0;
}

template <typename T>
int TreeNode<T>::getMaxKeys()
{
    int maxKeys = 0;
    size_t sum = sizeof(Pointer);

    while (sum + sizeof(Pointer) + sizeof(Key) <= this->blockSize)
    {
        sum += (sizeof(Pointer) + sizeof(Key));
        maxKeys++;
    }
    
    return maxKeys;
}

Pointer::Pointer(Block *blockAddress)
{
    this->blockAddress = blockAddress;
}

Block* Pointer::getBlockAddress()
{
    return this->blockAddress;
}

template <typename T>
Key<T>::Key(T value)
{
    this->value = value;
}

template <typename T>
T Key<T>::getValue()
{
    return this->value;
}

template <typename T>
BPlusTree<T>::BPlusTree(size_t blockSize)
{
    this->root = new TreeNode(blockSize);
}

template <typename T>
void BPlusTree<T>::insert(DataBlock block, T value)
{

}

template <typename T>
void BPlusTree<T>::delete(T value)
{

}

template <typename T>
Record BPlusTree<T>::search(T value)
{
    
}

template <typename T>
void BPlusTree<T>::reformatTree()
{

}
