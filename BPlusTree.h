#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <string>
#include "BPTreeNode.h"

class BPlusTree {
public:
    BPlusTree(int n);
    bool insert(int key, std::string value);
    bool remove(int key);
    std::string find(int key);
    void printKeys();
    void printValues();

    int max_order;
    BPTreeNode *root;
};



#endif