#include "b_plus_tree.h"
#include "tree_node.h"
#include "memory_pool.h"

#include <vector>
#include <cstring>
#include <iostream>

using namespace std;

void BPlusTree::search(int left, int right) {
    // maintain a number of nodes visited
    int numberOfAccessedNodes = 0;
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
    } else {
    //        Address rootAddress{diskRootAddress, 0};
    //    root = (TreeNode *) index->load(rootAddress, blockSize);
        //show the content of accessed node
        std::cout << "Root Treenode accessing: ";
        displayNode(root);
        numberOfAccessedNodes++;

        TreeNode* cursor = root;
        bool flag = false;

        while (!cursor->getLeaf()) {
            for (int i = 0; i < cursor->getNumOfKeys(); i++)
            {
                if (left < cursor->getKey(i)) {
                    cursor = cursor->getPointer(i);
//                    cursor = (TreeNode*) index->load(cursor->getDisk()[i], blockSize);
                    // cout << i << endl;
                    std::cout << "Treenode accessing: ";
                    displayNode(cursor);
                    numberOfAccessedNodes++;
                    break;
                }
                // the last traverse of this if block should be accessing a leaf node
                if (i == cursor->getNumOfKeys()-1 ) {
                    cursor = cursor->getPointer(i+1);
//                    cursor = (TreeNode*) index->load(cursor->getDisk()[i+1], blockSize);
                    // cout << i << endl;
                    std::cout << "Treenode accessing: ";
                    displayNode(cursor);
                    numberOfAccessedNodes++;
                    break;
                }
                
            }
        }

       while (!flag) {
           int idx = 0;
           for (; idx < cursor->getNumOfKeys(); idx++) {
               if (cursor->getKey(idx) > right) {
                   flag = true; // find the right boundary
                   break;
               }
               if (cursor->getKey(idx) <= right && cursor->getKey(idx) >= left) {
                   // cout << cursor->getKey(idx) << endl;
                   // here should print the record block
                   //TODO
                   // cout << "--This should be a data block content.--" << endl;
               }
           }
        // if next leaf node exists, go to next leaf node and display, then update cursor
           if (cursor->getPointer(cursor->getNumOfKeys()) != nullptr && cursor->getKey(idx) <= right) {

            //    cursor = (TreeNode*)index->load(cursor->getDisk()[cursor->getNumOfKeys()], blockSize);
            cursor = cursor->getPointer(cursor->getNumOfKeys());
            std::cout << "Treenode accessing: ";
            displayNode(cursor);
            numberOfAccessedNodes++;

           } else {
               flag = true;
           }
       }
        cout << "Number of accessed nodes: " << numberOfAccessedNodes << endl;
        return;
    }
}