#include "b_plus_tree.h"
#include "tree_node.h"
#include "tree_node.cpp"

#include <iostream>
#include <cstddef>
#include <list>
#include <iterator>

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

void BPlusTree::display(TreeNode *cursor)
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
                display(cursor->getPointer(i));
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
        int leftNumOfKeys = (this->maxKeys + 1) / 2;
        int rightNumOfKeys = this->maxKeys - leftNumOfKeys;
        cursor->setNumOfKeys(leftNumOfKeys);
        newInternalNode->setNumOfKeys(rightNumOfKeys);

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

// find the parent of the node child; finding starts from cursor
TreeNode* BPlusTree::findParent(TreeNode *cursor, TreeNode *child)
{
    TreeNode *parent;
    if (cursor->getLeaf() || (cursor->getPointer(0))->getLeaf())
    {
        return NULL;
    }

    for (int i = 0; i < cursor->getNumOfKeys() + 1; i++)
    {
        if (cursor->getPointer(i) == child) // find the parent in the current level
        {
            parent = cursor;
            return parent;
        }
        else // does not find in this level, need to go down one level
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

// The key value of parent should be the minimum key value of the right subtree
int BPlusTree::findParentValue(TreeNode *cursor) 
{
    while (!cursor->getLeaf()) {
        cursor = cursor->getPointer(0);
    }
    return cursor->getKey(0);
}

void BPlusTree::remove(int x, int &numDel, int &numUpd) {

  if (root == NULL) {
    cout << "Tree empty\n";
  } else {
    TreeNode* cursor = root;
    TreeNode* parent;
    int leftSibling, rightSibling;
    while (cursor->getLeaf() == false) {
      for (int i = 0; i < cursor->getNumOfKeys(); i++) {
        parent = cursor;
        leftSibling = i - 1;
        rightSibling = i + 1;
        if (x < cursor->getKey(i)) {
          cursor = cursor->getPointer(i);
          break;
        }
        if (i == cursor->getNumOfKeys() - 1) {
          leftSibling = i;
          rightSibling = i + 2;
          cursor = cursor->getPointer(i + 1);
          break;
        }
      }
    }
    bool found = false;
    int pos;
    for (pos = 0; pos < cursor->getNumOfKeys(); pos++) {
      if (cursor->getKey(pos) == x) {
        found = true;
        break;
      }
    }
    if (!found) {
      cout << "Not found\n";
      return;
    }
    for (int i = pos; i < cursor->getNumOfKeys(); i++) {
      cursor->setKey(i, cursor->getKey(i + 1));
    }
    cursor->setNumOfKeys(cursor->getNumOfKeys()-1);
    if (cursor == root) {
      for (int i = 0; i < this->maxKeys + 1; i++) {
        cursor->setPointer(i, NULL);
      }
      if (cursor->getNumOfKeys() == 0) {
        numDel++;
        delete cursor;
        root = NULL;
      }
      return;
    }
    cursor->setPointer(cursor->getNumOfKeys(), cursor->getPointer(cursor->getNumOfKeys() + 1));
    //set record as null
    cursor->setPointer(cursor->getNumOfKeys() + 1, NULL);
    if (cursor->getNumOfKeys() >= (this->maxKeys + 1) / 2) {
      numUpd++;
      return;
    }
    if (leftSibling >= 0) {
      TreeNode*leftNode = parent->getPointer(leftSibling);
      if (leftNode->getNumOfKeys() >= (this->maxKeys + 1) / 2 ) {
        for (int i = cursor->getNumOfKeys(); i > 0; i--) {
          cursor->setKey(i, cursor->getKey(i-1));
        }
        cursor->setNumOfKeys(cursor->getNumOfKeys()+1);
        cursor->setPointer(cursor->getNumOfKeys(), cursor->getPointer(cursor->getNumOfKeys() - 1));
        cursor->setPointer(cursor->getNumOfKeys() - 1, NULL);
        cursor->setKey(0, leftNode->getKey(leftNode->getNumOfKeys() - 1));
        leftNode->setNumOfKeys(cursor->getNumOfKeys()-1);
        leftNode->setPointer(leftNode->getNumOfKeys(), cursor);
        leftNode->setPointer(leftNode->getNumOfKeys() + 1, NULL);
        parent->setKey(leftSibling, cursor->getKey(0));
        numUpd++;
        return;
      }
    }
    if (rightSibling <= parent->getNumOfKeys()) {
      TreeNode* rightNode = parent->getPointer(rightSibling);
      if (rightNode->getNumOfKeys() >= (this->maxKeys + 1) / 2 + 1) {
        cursor->setNumOfKeys(cursor->getNumOfKeys()+1);
        cursor->setPointer(cursor->getNumOfKeys(), cursor->getPointer(cursor->getNumOfKeys() - 1));
        cursor->setPointer(cursor->getNumOfKeys() - 1, NULL);
        cursor->setKey(cursor->getNumOfKeys() - 1, rightNode->getKey(0));
        rightNode->setNumOfKeys(cursor->getNumOfKeys()-1);
        rightNode->setPointer(rightNode->getNumOfKeys(), rightNode->getPointer(rightNode->getNumOfKeys() + 1));
        rightNode->setPointer(rightNode->getNumOfKeys() + 1, NULL);
        for (int i = 0; i < rightNode->getNumOfKeys(); i++) {
          rightNode->setKey(i, rightNode->getKey(i+1));
        }
        parent->setKey(rightSibling-1, rightNode->getKey(0));
        numUpd++;
        return;
      }
    }
    if (leftSibling >= 0) {
      TreeNode* leftNode = parent->getPointer(leftSibling);
      for (int i = leftNode->getNumOfKeys(), j = 0; j < cursor->getNumOfKeys(); i++, j++) {
        leftNode->setKey(i, cursor->getKey(j));
      }
      leftNode->setPointer(leftNode->getNumOfKeys(),NULL);
      leftNode->setNumOfKeys(leftNode->getNumOfKeys() + cursor->getNumOfKeys());
      leftNode->setPointer(leftNode->getNumOfKeys(), cursor->getPointer(cursor->getNumOfKeys()));
      numDel++;
      removeInternal(parent->getKey(leftSibling),parent,cursor);

      delete cursor;
    } else if (rightSibling <= parent->getNumOfKeys()) {
      TreeNode* rightNode = parent->getPointer(rightSibling);
      for (int i = cursor->getNumOfKeys(), j = 0; j < rightNode->getNumOfKeys(); i++, j++) {
        cursor->setKey(i, rightNode->getKey(j));
      }
      cursor->setPointer(cursor->getNumOfKeys(),NULL);
      cursor->setNumOfKeys(cursor->getNumOfKeys()+rightNode->getNumOfKeys());
      cursor->setPointer(cursor->getNumOfKeys(),rightNode->getPointer(rightNode->getNumOfKeys()) );
      cout << "Merging two leaf nodes\n";
      numDel++;
      removeInternal(parent->getKey(rightSibling-1),parent,rightNode);
      delete rightNode;

    }
  }
}

void BPlusTree::removeInternal(int x, TreeNode* cursor, TreeNode* child) {
  //numDel++;
  if (cursor == root) {
    if (cursor->getNumOfKeys()== 1) {
      if (cursor->getPointer(1) == child) {
        delete child;
        root = cursor->getPointer(0);
        delete cursor;
        cout << "Changed root node\n";
        return;
      } else if (cursor->getPointer(0) == child) {
        delete child;
        root = cursor->getPointer(1);
        delete cursor;
        cout << "Changed root node\n";
        return;
      }
    }
  }
  int pos;
  for (pos = 0; pos < cursor->getNumOfKeys(); pos++) {
    if (cursor->getKey(pos) == x) {
      break;
    }
  }
  for (int i = pos; i < cursor->getNumOfKeys(); i++) {
    cursor->setKey(i, cursor->getKey(i+1));
  }
  for (pos = 0; pos < cursor->getNumOfKeys() + 1; pos++) {
    if(cursor->getPointer(pos)==child){
    }
  }
  for (int i = pos; i < cursor->getNumOfKeys() + 1; i++) {
    cursor->setPointer(i, cursor->getPointer(i+1));
  }
  cursor->setNumOfKeys(cursor->getNumOfKeys()-1);
  if (cursor->getNumOfKeys() >= (this->maxKeys + 1) / 2 - 1 ) {
    return;
  }
  if (cursor == root)
    return;
  TreeNode* parent = findParent(root, cursor);
  int leftSibling, rightSibling;
  for (pos = 0; pos < parent->getNumOfKeys() + 1; pos++) {
    if (parent->getPointer(pos) == cursor) {
      leftSibling = pos - 1;
      rightSibling = pos + 1;
      break;
    }
  }
  if (leftSibling >= 0) {
    TreeNode* leftNode = parent->getPointer(leftSibling);
    if (leftNode->getNumOfKeys() >= (this->maxKeys + 1) / 2) {
      for (int i = cursor->getNumOfKeys(); i > 0; i--) {
        cursor->setKey(i, cursor->getKey(i-1));
      }
      cursor->setKey(0, parent->getKey(leftSibling));
      parent->setKey(leftSibling, leftNode->getKey(leftNode->getNumOfKeys()-1));
      for (int i = cursor->getNumOfKeys() + 1; i > 0; i--) {
        cursor->setPointer(i, cursor->getPointer(i-1));
      }
      cursor->setPointer(0, leftNode->getPointer(leftNode->getNumOfKeys()));
      cursor->setNumOfKeys(cursor->getNumOfKeys()+1);
      leftNode->setNumOfKeys(leftNode->getNumOfKeys()-1);
      return;
    }
  }
  if (rightSibling <= parent->getNumOfKeys()) {
    TreeNode* rightNode = parent->getPointer(rightSibling);
    if (rightNode->getNumOfKeys() >= (this->maxKeys + 1) / 2) {
      cursor->setKey(cursor->getNumOfKeys(), parent->getKey(pos));
      parent->setKey(pos, rightNode->getKey(0));
      for (int i = 0; i < rightNode->getNumOfKeys() - 1; i++) {
        rightNode->setKey(i, rightNode->getKey(i+1));
      }
      cursor->setPointer(cursor->getNumOfKeys()+1, rightNode->getPointer(0));
      for (int i = 0; i < rightNode->getNumOfKeys(); ++i) {
        rightNode->setPointer(i, rightNode->getPointer(i+1));
      }
      cursor->setNumOfKeys(cursor->getNumOfKeys()+1);
      rightNode->setNumOfKeys(rightNode->getNumOfKeys()-1);
      return;
    }
  }
  if (leftSibling >= 0) {
    TreeNode* leftNode = parent->getPointer(leftSibling);
    leftNode->setKey(leftNode->getNumOfKeys(), parent->getKey(leftSibling));
    for (int i = leftNode->getNumOfKeys() + 1, j = 0; j < cursor->getNumOfKeys(); j++) {
      leftNode->setKey(i, cursor->getKey(j));
    }
    for (int i = leftNode->getNumOfKeys() + 1, j = 0; j < cursor->getNumOfKeys() + 1; j++) {
      leftNode->setPointer(i, cursor->getPointer(j));
      cursor->setPointer(j, NULL);
    }
    leftNode->setNumOfKeys(leftNode->getNumOfKeys() + cursor->getNumOfKeys()+1);
    cursor->setNumOfKeys(0);
    removeInternal(parent->getKey(leftSibling), parent, cursor);
  } else if (rightSibling <= parent->getNumOfKeys()) {
    TreeNode* rightNode = parent->getPointer(rightSibling);
    cursor->setKey(cursor->getNumOfKeys(), parent->getKey(rightSibling-1));
    for (int i = cursor->getNumOfKeys() + 1, j = 0; j < rightNode->getNumOfKeys(); j++) {
      cursor->setKey(i, rightNode->getKey(j));
    }
    for (int i = cursor->getNumOfKeys() + 1, j = 0; j < rightNode->getNumOfKeys() + 1; j++) {
      cursor->setPointer(i, rightNode->getPointer(j));
      rightNode->setPointer(j,NULL);
    }
    cursor->setNumOfKeys(cursor->getNumOfKeys()+rightNode->getNumOfKeys()+1);
    rightNode->setNumOfKeys(0);
    removeInternal(parent->getKey(rightSibling - 1), parent, rightNode);
  }
}

void BPlusTree::getFirstLeaf(){
    TreeNode *node;
    if(root==NULL){
        cout<<"empty tree"<<endl;
    }else{
        node = root;
        while(!node->getLeaf()){
            node=node->getPointer(0);
        }
        cout<<"first leaf, first element: "<<node->getKey(0)<<endl;
    }
}
