#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b_plus_tree.h"

//get max num key
int order = getMaxKeys();

node*  BPlusTree::delete (node *root, int key, int *numNodeDeleted, int *numNodeUpdated) {
  node *key_leaf = NULL;
  record *key_record = NULL;
  //int numNodeDeleted = 0;
  //int numNodeUpdated = 0;

  key_record = find(root, key, false, &key_leaf);

  if (key_record != NULL && key_leaf != NULL) {
    root = delete_entry(root, key_leaf, key, key_record, &numNodeDeleted, &numNodeUpdated);
    free(key_record);
  }
  return root;
}

record* BPlusTree::find(node *root, int key, bool verbose, node **leaf_out) {
  if (root == NULL) {
    if (leaf_out != NULL) {
      *leaf_out = NULL;
    }
    return NULL;
  }

  int i = 0;
  node *leaf = NULL;

  leaf = find_leaf(root, key, verbose);

  for (i = 0; i < leaf->numOfKeys; i++)
    if (leaf->keys[i] == key)
      break;
  if (leaf_out != NULL) {
    *leaf_out = leaf;
  }
  if (i == leaf->num_keys)
    return NULL;
  else
    return (record *)leaf->pointers[i];
}

node* BPlusTree::find_leaf(node *const root, int key, bool verbose) {
  if (root == NULL) {
    if (verbose)
      printf("Empty tree.\n");
    return root;
  }
  int i = 0;
  node *c = root;
  while (!c->isLeaf) {
    if (verbose) {
      printf("[");
      for (i = 0; i < c->numOfKeys - 1; i++)
        printf("%d ", c->keys[i]);
      printf("%d] ", c->keys[i]);
    }
    i = 0;
    while (i < c->numOfKeys) {
      if (key >= c->keys[i])
        i++;
      else
        break;
    }
    if (verbose)
      printf("%d ->\n", i);
    c = (node *)c->pointers[i];
  }
  if (verbose) {
    printf("Leaf [");
    for (i = 0; i < c->numOfKeys - 1; i++)
      printf("%d ", c->keys[i]);
    printf("%d] ->\n", c->keys[i]);
  }
  return c;
}


node* BPlusTree::delete_entry(node *root, node *n, int key, void *pointer, int *inNumNodeDeleted, int *inNumNodeUpdated) {
  int min_keys;
  node *neighbor;
  int neighbor_index;
  int k_prime_index, k_prime;
  int capacity;

  n = remove_entry_from_node(n, key, pointer, inNumNodeDeleted);

  if (n == root)
    return adjust_root(root);

  min_keys = n->isLeaf ? cut(order + 1) : cut(order) ;

  if (n->numOfKeys >= min_keys)
    return root;

  neighbor_index = get_neighbor_index(n);
  k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
  k_prime = n->parent->keys[k_prime_index];
  neighbor = neighbor_index == -1 ? n->parent->pointers[1] : n->parent->pointers[neighbor_index];

  capacity = n->isLeaf ? order : order - 1;

  if (neighbor->numOfKeys + n->numOfKeys < capacity)
    return merge_nodes(root, n, neighbor, neighbor_index, k_prime, inNumNodeDeleted);
  else
    return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime, inNumNodeUpdated);
}

node* BPlusTree::remove_entry_from_node(node *n, int key, node *pointer, int *inNumNodeDeleted) {
  int i, num_pointers;
  i = 0;
  *inNumNodeDeleted++;
  while (n->keys[i] != key)
    i++;
  for (++i; i < n->num_keys; i++)
    n->keys[i - 1] = n->keys[i];

  num_pointers = n->isLeaf ? n->numOfKeys : n->numOfKeys + 1;
  i = 0;
  while (n->pointers[i] != pointer)
    i++;
  for (++i; i < num_pointers; i++)
    n->pointers[i - 1] = n->pointers[i];

  n->numOfKeys--;

  if (n->isLeaf)
    for (i = n->numOfKeys; i < order - 1; i++)
      //mark record as deleted
      n->pointers[i]->setDelete();
      n->pointers[i] = NULL;
  else
    for (i = n->numOfKeys + 1; i < order; i++)
      n->pointers[i] = NULL;

  return n;
}



int BPlusTree::cut(int length) {
    return length / 2;
}

node* BPlusTree::adjust_root(node *root) {
  node *new_root;

  if (root->numOfKeys > 0)
    return root;

  if (!root->isLeaf) {
    new_root = root->pointers[0];
    new_root->parent = NULL;
  }

  else
    new_root = NULL;

  free(root->keys);
  free(root->pointers);
  free(root);

  return new_root;
}

int BPlusTree::get_neighbor_index(node *n) {
  int i;
  for (i = 0; i <= n->parent->numOfKeys; i++)
    if (n->parent->pointers[i] == n)
      return i - 1;

  printf("Search for nonexistent pointer to node in parent.\n");
  printf("Node:  %#lx\n", (unsigned long)n);
  exit(EXIT_FAILURE);
}

node* BPlusTree::merge_nodes(node *root, node *n, node *neighbor, int neighbor_index, int k_prime, int *inNumNodeDeleted) {
  int i, j, neighbor_insertion_index, n_end;
  node *tmp;

  if (neighbor_index == -1) {
    tmp = n;
    n = neighbor;
    neighbor = tmp;
  }

  neighbor_insertion_index = neighbor->numOfKeys;

  if (!n->isLeaf) {
    neighbor->keys[neighbor_insertion_index] = k_prime;
    neighbor->num_keys++;

    n_end = n->numOfKeys;

    for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
      neighbor->keys[i] = n->keys[j];
      neighbor->pointers[i] = n->pointers[j];
      neighbor->numOfKeys++;
      n->numOfKeys--;
    }

    neighbor->pointers[i] = n->pointers[j];

    for (i = 0; i < neighbor->numOfKeys + 1; i++) {
      tmp = (node *)neighbor->pointers[i];
      tmp->parent = neighbor;
    }
  }

  else {
    for (i = neighbor_insertion_index, j = 0; j < n->numOfKeys; i++, j++) {
      neighbor->keys[i] = n->keys[j];
      neighbor->pointers[i] = n->pointers[j];
      neighbor->numOfKeys++;
    }
    neighbor->pointers[order - 1] = n->pointers[order - 1];
  }

  root = delete_entry(root, n->parent, k_prime, n, inNumNodeDeleted);
  free(n->keys);
  free(n->pointers);
  free(n);
  return root;
}


node* BPlusTree::redistribute_nodes(node *root, node *n, node *neighbor, int neighbor_index,
             int k_prime_index, int k_prime, int *inNumNodeUpdated) {
  int i;
  node *tmp;
  *inNumNodeUpdated++;

  if (neighbor_index != -1) {
    if (!n->isLeaf)
      n->pointers[n->numOfKeys + 1] = n->pointers[n->numOfKeys];
    for (i = n->numOfKeys; i > 0; i--) {
      n->keys[i] = n->keys[i - 1];
      n->pointers[i] = n->pointers[i - 1];
    }
    if (!n->isLeaf) {
      n->pointers[0] = neighbor->pointers[neighbor->numOfKeys];
      tmp = (node *)n->pointers[0];
      tmp->parent = n;
      neighbor->pointers[neighbor->numOfKeys] = NULL;
      n->keys[0] = k_prime;
      n->parent->keys[k_prime_index] = neighbor->keys[neighbor->numOfKeys - 1];
    } else {
      n->pointers[0] = neighbor->pointers[neighbor->numOfKeys - 1];
      neighbor->pointers[neighbor->numOfKeys - 1] = NULL;
      n->keys[0] = neighbor->keys[neighbor->numOfKeys - 1];
      n->parent->keys[k_prime_index] = n->keys[0];
    }
  }

  else {
    if (n->isLeaf) {
      n->keys[n->numOfKeys] = neighbor->keys[0];
      n->pointers[n->numOfKeys] = neighbor->pointers[0];
      n->parent->keys[k_prime_index] = neighbor->keys[1];
    } else {
      n->keys[n->numOfKeys] = k_prime;
      n->pointers[n->numOfKeys + 1] = neighbor->pointers[0];
      tmp = (node *)n->pointers[n->numOfKeys + 1];
      tmp->parent = n;
      n->parent->keys[k_prime_index] = neighbor->keys[0];
    }
    for (i = 0; i < neighbor->numOfKeys - 1; i++) {
      neighbor->keys[i] = neighbor->keys[i + 1];
      neighbor->pointers[i] = neighbor->pointers[i + 1];
    }
    if (!n->isLeaf)
      neighbor->pointers[i] = neighbor->pointers[i + 1];
  }

  n->num_keys++;
  neighbor->numOfKeys--;

  return root;
}

void BPlusTree::getFirstLeaf(node *root){
  if (root->isLeaf){
    //print out contents
    for(int i=0; i < root->numOfKeys; i++){
      node *temp = root->pointers[i]
      //print contents of temp
    }
  }else{
    getFirstLeaf(root->pointers[0]);
  }
}