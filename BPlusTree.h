#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <string>
#include "BPTreeNode.h"

class BPlusTree {
public:
    BPlusTree(int n);
    bool insert(key_t key, value_t value);
    bool remove(key_t key);
    std::string find(key_t key);
    void printKeys();
    void printValues();

private:
    int max_order;
    BPTreeNode *root;

    void __insert_kv_to_leaf_node(BPTreeNode* node, key_t key, value_t value);
    void __insert_key_to_internal_node(BPTreeNode* node, key_t key, BPTreeNode* child);
};



#endif