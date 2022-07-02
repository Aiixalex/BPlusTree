#ifndef _BPTREENODE_H_
#define _BPTREENODE_H_

#include <string>
#include <vector>
#include <memory>

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

    // Leaf node only
    std::vector<value_t> values;

    // Internal nodes only
    std::vector<std::shared_ptr<BPTreeNode>> children;
    std::weak_ptr<BPTreeNode> parent;

    std::shared_ptr<BPTreeNode> next;
    std::weak_ptr<BPTreeNode> prev;

    int num_keys;
public:
    BPTreeNode(int max_order, NodeType node_type);
    ~BPTreeNode();
};

#endif