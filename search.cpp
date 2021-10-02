#include "b_plus_tree.h"
#include "tree_node.h"
#include "memory_pool.h"

#include <vector>
#include <cstring>
#include <iostream>

using namespace std;

template <typename T>
void BPlusTree<T>::search(T left, T right) {
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
    } else {
        Address rootAddress(diskRootAddress, 0);
        root = (TreeNode<T> *) index->load(rootAddress, blockSize);
        TreeNode<T>* cur = root;
        bool flag = false;

        while (!cur->isLeaf) {
            for (int i = 0; i < cur.getNumOfKeys(); i++)
            {
                if (left < cur->getKey(i)) {
                    cur = (TreeNode<T> *) index->load(cur->getPointer[i], blockSize);
                    cout << i << endl;
                    break;
                }
                if (i == cur->getNumOfKeys()-1 ) {
                    cur = (TreeNode<T> *) index->load(cur->getPointer[i+1], blockSize);
                    cout << i << endl;
                    break;
                }
                
            }
        }

        while (!flag) {
            int idx = 0;
            for (; idx < cur->numOfKeys; idx++) {
                if (cur->getKey(idx) > right) {
                    flag = true; // find the right boundary
                    break;
                }
                if (cur->getKey(idx) <= right && cur->getKeys(idx) >= left) {
                    cout << cur->getKeys(idx) << endl;
                }
            }

            if (cur->diskAddress[cur->getNumOfKeys()].blockAddress != nullptr && cur->getKey(idx) != right) {
                cur = (TreeNode<T> *)index->load(cur->getPointer[cur->getNumOfKeys()], blockSize);
            } else {
                flag = true;
            }
        }
    }
}