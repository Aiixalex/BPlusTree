#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <string>
#include "BPTreeNode.h"

class BPlusTree {
public:
    BPlusTree(int n);
    ~BPlusTree();
    bool insert(key_t key, value_t value);
    bool remove(key_t key);
    std::string find(key_t key);
    void printKeys();
    void printValues();

private:
    int max_order;
    std::shared_ptr<BPTreeNode> root;

    void __insert_kv_to_leaf_node(std::shared_ptr<BPTreeNode> node, key_t key, value_t value);
    void __insert_key_to_internal_node(std::shared_ptr<BPTreeNode> node, key_t key, std::shared_ptr<BPTreeNode> child);
    void __remove_kv_from_leaf_node(std::shared_ptr<BPTreeNode> node, key_t key);
    void __remove_key_from_internal_node(std::shared_ptr<BPTreeNode> node, key_t key);
    std::shared_ptr<BPTreeNode> __get_left_sibling_node_to_redistribute(std::shared_ptr<BPTreeNode> curr_node);
    std::shared_ptr<BPTreeNode> __get_right_sibling_node_to_redistribute(std::shared_ptr<BPTreeNode> curr_node);
    std::shared_ptr<BPTreeNode> __get_left_sibling_node_to_merge(std::shared_ptr<BPTreeNode> curr_node);
    std::shared_ptr<BPTreeNode> __get_right_sibling_node_to_merge(std::shared_ptr<BPTreeNode> curr_node);
    void __merge(std::shared_ptr<BPTreeNode> left, std::shared_ptr<BPTreeNode> right);
    void __merge_triangle(std::shared_ptr<BPTreeNode> left, std::shared_ptr<BPTreeNode> right, std::shared_ptr<BPTreeNode> parent);
};



#endif