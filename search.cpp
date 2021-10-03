#include "b_plus_tree.h"
#include "tree_node.h"
#include "memory_pool.h"

#include <vector>
#include <cstring>
#include <iostream>

using namespace std;

void BPlusTree::search(int left, int right) {
    int numberOfAccessedNodes = 0;
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
    } else {
        //maintain a number of nodes visited

//        Address rootAddress{diskRootAddress, 0};
//        root = (TreeNode *) index->load(rootAddress, blockSize);

        //show the content of accessed node
        std::cout << "Root Treenode accessing: ";
        for (int i = 0; i < root->getNumOfKeys(); i++)
        {
            cout << root->getKey(i) << " ";
        }
        cout << "\n";
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
                    for (int i = 0; i < cursor->getNumOfKeys(); i++)
                    {
                        cout << cursor->getKey(i) << " ";
                    }
                    cout << "\n";
                    numberOfAccessedNodes++;
                    break;
                }
                if (i == cursor->getNumOfKeys()-1 ) {
                    cursor = cursor->getPointer(i+1);
//                    cursor = (TreeNode*) index->load(cursor->getDisk()[i+1], blockSize);
                    // cout << i << endl;
                    std::cout << "Treenode accessing: ";
                    for (int i = 0; i < cursor->getNumOfKeys(); i++)
                    {
                        cout << cursor->getKey(i) << " ";
                    }
                    cout << "\n";
                    numberOfAccessedNodes++;
                    break;
                }
                
            }
        }

//        while (!flag) {
//            int idx = 0;
//            for (; idx < cursor->getNumOfKeys(); idx++) {
//                if (cursor->getKey(idx) > right) {
//                    flag = true; // find the right boundary
//                    cout << "Number of accessed nodes: " << numberOfAccessedNodes << endl;
//                    return;
//                }
//                if (cursor->getKey(idx) <= right && cursor->getKey(idx) >= left) {
//                    // cout << cursor->getKey(idx) << endl;
////                    std::cout << "Treenode (leaf node) accessing: ";
////                    cout << cursor->getKey(idx) << " ";
////                    cout << "\n";
//                    // here should print the record block
//                    //TODO
//                }
//            }
//            cursor = cursor->getPointer(cursor->getNumOfKeys());
////            if (cursor != nullptr && cursor->getKey(idx) <= right) {
//
////                cursor = (TreeNode*)index->load(cursor->getDisk()[cursor->getNumOfKeys()], blockSize);
//                std::cout << "Treenode accessing: ";
//                for (int i = 0; i < cursor->getNumOfKeys(); i++)
//                {
//                    cout << cursor->getKey(i) << " ";
//                }
//                cout << endl;
//                numberOfAccessedNodes++;

//            } else {
//                flag = true;
//            }
//        }

    }
}