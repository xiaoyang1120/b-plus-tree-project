#include "b_plus_tree.h"
#include "tree_node.h"
#include "memory_pool.h"

#include <vector>
#include <cstring>
#include <iostream>

using namespace std;

void BPlusTree::search(int left, int right) {
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
    } else {
        Address rootAddress{diskRootAddress, 0};
        root = (TreeNode *) index->load(rootAddress, blockSize);
        TreeNode* cur = root;
        bool flag = false;

        while (!cur->getLeaf()) {
            for (int i = 0; i < cur->getNumOfKeys(); i++)
            {
                if (left < cur->getKey(i)) {
                    cur = (TreeNode*) index->load(cur->getDisk()[i], blockSize);
                    cout << i << endl;
                    break;
                }
                if (i == cur->getNumOfKeys()-1 ) {
                    cur = (TreeNode*) index->load(cur->getDisk()[i+1], blockSize);
                    cout << i << endl;
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
                    cout << cur->getKey(idx) << endl;
                }
            }

            if (cur->getDisk()[cur->getNumOfKeys()].blockAddress != nullptr && cur->getKey(idx) != right) {
                cur = (TreeNode*)index->load(cur->getDisk()[cur->getNumOfKeys()], blockSize);
            } else {
                flag = true;
            }
        }
    }
}