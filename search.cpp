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

        Address rootAddress{diskRootAddress, 0};
        root = (TreeNode *) index->load(rootAddress, blockSize);

        //show the content of accessed node
        std::cout << "Treenode accessing: ";
        display(root);
        numberOfAccessedNodes++;

        TreeNode* cur = root;
        bool flag = false;

        while (!cur->getLeaf()) {
            for (int i = 0; i < cur->getNumOfKeys(); i++)
            {
                if (left < cur->getKey(i)) {
                    cur = (TreeNode*) index->load(cur->getDisk()[i], blockSize);
                    // cout << i << endl;
                    std::cout << "Treenode accessing: ";
                    display(cur);
                    break;
                }
                if (i == cur->getNumOfKeys()-1 ) {
                    cur = (TreeNode*) index->load(cur->getDisk()[i+1], blockSize);
                    // cout << i << endl;
                    std::cout << "Treenode accessing: ";
                    display(cur);
                    break;
                }
                
            }
        }

        while (!flag) {
            int idx = 0;
            for (; idx < cur->getNumOfKeys(); idx++) {
                if (cur->getKey(idx) > right) {
                    flag = true; // find the right boundary
                    break;
                }
                if (cur->getKey(idx) <= right && cur->getKey(idx) >= left) {
                    // cout << cur->getKey(idx) << endl;
                    std::cout << "Treenode (leaf node) accessing: ";
                    display(cur);
                    // here should print the record block
                    //TODO
                }
            }

            if (cur->getDisk()[cur->getNumOfKeys()].blockAddress != nullptr && cur->getKey(idx) != right) {
                cur = (TreeNode*)index->load(cur->getDisk()[cur->getNumOfKeys()], blockSize);
                std::cout << "Treenode accessing: ";
                display(cur);
            } else {
                flag = true;
            }
        }
    }
}