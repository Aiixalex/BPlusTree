#ifndef _BPTREENODE_H_
#define _BPTREENODE_H_

#include <string>
#include <vector>

typedef int key_t;
typedef std::string value_t;

enum class NodeType {
    kLeafNode = 0,
    kInternalNode
};

class BPTreeNode {
    friend class BPlusTree;
private:
    int max_order;
    NodeType node_type;
    std::vector<key_t> keys;
    std::vector<value_t> values;
    std::vector<BPTreeNode*> children;
    BPTreeNode* parent;
    int num_keys;
public:
    BPTreeNode(int max_order, NodeType node_type);
};

#endif