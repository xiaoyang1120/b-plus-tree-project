#include "b_plus_tree.h"
#include "tree_node.h"

#include <iostream>
#include <cstddef>

using namespace std;

BPlusTree::BPlusTree()
{
    this->root = NULL;
    this->maxKeys = this->getMaxKeys();
}

BPlusTree::BPlusTree(size_t blockSize, MemoryPool* disk, MemoryPool* index)
{
    this->blockSize = blockSize;
    this->disk = disk;
    this->index = index;
    BPlusTree();
}

int BPlusTree::getMaxKeys()
{
    // TODO: 回头加上了blocksize一起算
    return 3; // 先hardcode
}

TreeNode *BPlusTree::getRoot()
{
    return this->root;
}

void BPlusTree::levelDisplay(TreeNode *cursor)
{
    if (cursor != NULL)
    {
        for (int i = 0; i < cursor->getNumOfKeys(); i++)
        {
            cout << cursor->getKey(i) << " ";
        }
        cout << "\n";
        if (!cursor->getLeaf())
        {
            for (int i = 0; i < cursor->getNumOfKeys() + 1; i++)
            {
                levelDisplay(cursor->getPointer(i));
            }
        }
    }
}

void BPlusTree::insert(int value)
{
    if (this->root == NULL) // very first node in the whole tree
    {
        this->root = new TreeNode();
        this->root->setKey(0, value);
        this->root->setLeaf(true);
        this->root->setNumOfKeys(1);
        return;
    }
    
    TreeNode *cursor = this->root;
    TreeNode *parent;
    while (!cursor->getLeaf()) // go along the way to locate to the leaf node to insert
    {
        parent = cursor;
        for (int i = 0; i < cursor->getNumOfKeys(); i++) // locate the position within tree node
        {
            if (value < cursor->getKey(i)) // find the first position where value < keys[i]
            {
                cursor = cursor->getPointer(i); // go with its previous pointer
                break;
            }

            if (i == cursor->getNumOfKeys() - 1) // reach the end
            {
                cursor = cursor->getPointer(i + 1); // the position is at last
                break;
            }
        } 
    }

    // already locate to the leaf node
    // see if we need to balance the tree
    if (cursor->getNumOfKeys() < this->maxKeys) // not full, directly insert
    {
        // locate the position (first position that value <= keys[i]) within leaf node
        int position = 0;
        while (value > cursor->getKey(position) && position < cursor->getNumOfKeys())
        {
            position++;
        }

        // move the keys after the position one step to the right
        for (int i = cursor->getNumOfKeys(); i > position; i--)
        {
            cursor->setKey(i, cursor->getKey(i - 1));
            cursor->setPointer(i, cursor->getPointer(i - 1)); // leaf node: key i <-> pointer i
        }

        // insert
        cursor->setKey(position, value);
        cursor->setNumOfKeys(cursor->getNumOfKeys() + 1);
        cursor->setPointer(position, NULL); // TODO: hard code NULL for now, 应该是指向data block
    }
    else // the leaf node is full
    {
        TreeNode *newLeafNode = new TreeNode();

        // helper keys&pointers to insert the new key into the full node
        int MAX = this->maxKeys;
        int *virtualKeys = new int[MAX + 1];
        for (int i = 0; i < this->maxKeys; i++)
        {
            virtualKeys[i] = cursor->getKey(i);
        }
        TreeNode **virtualPointers = new TreeNode*[MAX + 2];
        for (int i = 0; i <= this->maxKeys; i++)
        {
            virtualPointers[i] = cursor->getPointer(i);
        }

        // locate the position (first position that value <= keys[i]) within leaf node
        int position = 0;
        while (value > virtualKeys[position] && position < this->maxKeys)
        {
            position++;
        }

        // move the keys after the position one step to the right
        for (int i = this->maxKeys + 1; i > position; i--)
        {
            virtualKeys[i] = virtualKeys[i - 1];
            virtualPointers[i] = virtualPointers[i - 1]; // leaf node: key i <-> pointer i
        }

        // insert
        virtualKeys[position] = value;
        virtualPointers[position] = NULL; // TODO: hard code NULL for now, 应该是指向data block
        newLeafNode->setLeaf(true);

        int leftNumOfKeys = (this->maxKeys + 1) / 2;
        int rightNumOfKeys = this->maxKeys + 1 - leftNumOfKeys;
        cursor->setNumOfKeys(leftNumOfKeys);
        newLeafNode->setNumOfKeys(rightNumOfKeys);

        // last pointer of left node points to the right node
        cursor->setPointer(cursor->getNumOfKeys(), newLeafNode);
        // last pointer of right node points to the original right node of left node
        newLeafNode->setPointer(newLeafNode->getNumOfKeys(), cursor->getPointer(this->maxKeys));
        cursor->setPointer(this->maxKeys, NULL);

        // copy virtual node value into right node
        for (int i = 0; i < cursor->getNumOfKeys(); i++)
        {
            cursor->setKey(i, virtualKeys[i]);
            cursor->setPointer(i, virtualPointers[i]);
        }
        for (int i = 0, j = cursor->getNumOfKeys(); i < newLeafNode->getNumOfKeys(); i++, j++)
        {
            newLeafNode->setKey(i, virtualKeys[j]);
            newLeafNode->setPointer(i, virtualPointers[j]);
        }

        // remove the useless pointers in left node
        for (int i = cursor->getNumOfKeys() + 1; i < this->maxKeys + 1; i++)
        {
            cursor->setPointer(i, NULL);
        }

        // the splitted leaf node is the only node in leaf level
        // need to produce another non-leaf level
        if (cursor == this->root)
        {
            TreeNode *newRoot = new TreeNode;
            newRoot->setKey(0, newLeafNode->getKey(0)); // parent node value is the right node value
            newRoot->setPointer(0, cursor);
            newRoot->setPointer(1, newLeafNode);
            newRoot->setLeaf(false);
            newRoot->setNumOfKeys(1);
            this->root = newRoot;
        }
        else // need to deal with insertion of non leaf node
        {
            insertInternal(newLeafNode->getKey(0), parent, newLeafNode);
        }
    }
}

void BPlusTree::insertInternal(int value, TreeNode *cursor, TreeNode *child)
{
    if (cursor->getNumOfKeys() < this->maxKeys) // not full
    {
        // locate the position (first position that value <= keys[i]) to insert value
        int position = 0;
        while (value > cursor->getKey(position) && position < cursor->getNumOfKeys())
        {
            position++;
        }

        // move the keys after the position one step to the right
        for (int i = cursor->getNumOfKeys(); i > position; i--)
        {
            cursor->setKey(i, cursor->getKey(i - 1));
        }
        // non leaf node: key i <-> pointer i + 1
        for (int i = cursor->getNumOfKeys() + 1; i > position + 1; i--)
        {
            cursor->setPointer(i, cursor->getPointer(i - 1));
        }

        // insert
        cursor->setKey(position, value);
        cursor->setNumOfKeys(cursor->getNumOfKeys() + 1);
        cursor->setPointer(position + 1, child);
    }
    else // full, need to balance the tree
    {
        TreeNode *newInternalNode = new TreeNode;

        // helper keys&pointers to insert the new key into the full node
        int *virtualKeys = new int[this->maxKeys + 1];
        TreeNode **virtualPointers = new TreeNode*[this->maxKeys + 2];
        for (int i = 0; i < this->maxKeys; i++)
        {
            virtualKeys[i] = cursor->getKey(i);
        }
        for (int i = 0; i <= this->maxKeys; i++)
        {
            virtualPointers[i] = cursor->getPointer(i);
        }

        // locate the position (first position that value <= keys[i]) within leaf node
        int position = 0;
        while (value > virtualKeys[position] && position < this->maxKeys)
        {
            position++;
        }

        // move the keys after the position one step to the right
        for (int i = this->maxKeys + 1; i > position; i--)
        {
            virtualKeys[i] = virtualKeys[i - 1];
        }
        // insert
        virtualKeys[position] = value;
        
        // for non leaf node, key i <-> pointer i + 1
        for (int i = this->maxKeys + 2; i > position + 1; i--)
        {
            virtualPointers[i] = virtualPointers[i - 1];
        }
        // insert
        virtualPointers[position + 1] = child;

        newInternalNode->setLeaf(false);
        int leftNumOfKeys = (this->maxKeys + 1) / 2; // 还是maxKey / 2?
        int rightNumOfKeys = this->maxKeys - leftNumOfKeys; // 还是maxKey + 1 - left?
        cursor->setNumOfKeys(leftNumOfKeys);
        newInternalNode->setNumOfKeys(rightNumOfKeys);

        // 这里是否要cursor->numOfKeys "+ 1"？
        for (int i = 0, j = cursor->getNumOfKeys() + 1; i < newInternalNode->getNumOfKeys(); i++, j++)
        {
            newInternalNode->setKey(i, virtualKeys[j]);
        }
        for (int i = 0, j = cursor->getNumOfKeys() + 1; i < newInternalNode->getNumOfKeys() + 1; i++, j++)
        {
            newInternalNode->setPointer(i, virtualPointers[j]);
        }

        if (cursor == this->root) // splitted node is the only node for the current level
        {
            TreeNode *newRoot = new TreeNode;
            // newRoot->setKey(0, cursor->getKey(cursor->getNumOfKeys()));
            newRoot->setKey(0, findParentValue(newInternalNode));
            newRoot->setPointer(0, cursor);
            newRoot->setPointer(1, newInternalNode);
            newRoot->setLeaf(false);
            newRoot->setNumOfKeys(1);
            this->root = newRoot;
        }
        else
        {
            insertInternal(cursor->getKey(cursor->getNumOfKeys()), findParent(this->root, cursor), newInternalNode);
        }
    }
}

// find the parent of the child node
TreeNode* BPlusTree::findParent(TreeNode *cursor, TreeNode *child)
{
    TreeNode *parent;
    if (cursor->getLeaf() || (cursor->getPointer(0))->getLeaf())
    {
        return NULL;
    }

    for (int i = 0; i < cursor->getNumOfKeys() + 1; i++)
    {
        if (cursor->getPointer(i) == child) // 当前层就找到了
        {
            parent = cursor;
            return parent;
        }
        else // 要往下层找
        {
            parent = findParent(cursor->getPointer(i), child);
            if (parent != NULL)
            {
                return parent;
            }
        }
    }

    return parent;
}

int BPlusTree::findParentValue(TreeNode *cursor) 
{
    while (!cursor->getLeaf()) {
        cursor = cursor->getPointer(0);
    }
    return cursor->getKey(0);
}

// int main()
// {
//     BPlusTree<int> bptree;
//     // int numbers[12] = {1, 4, 7, 10, 17, 21, 31, 25, 19, 20, 28, 42};
//     bptree.insert(1);
//     bptree.insert(4);
//     bptree.insert(7);
//     bptree.insert(10);
//     bptree.insert(17);
//     bptree.insert(21);
//     bptree.insert(31);
//     bptree.insert(25);
//     bptree.insert(19);
//     // bptree.insert(20);
//     // bptree.insert(28);
//     // bptree.insert(42);
//     cout << "root: " << bptree.getRoot()->getKey(0) << endl;
//     bptree.levelDisplay(bptree.getRoot());
// }
