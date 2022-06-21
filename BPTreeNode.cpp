#include "BPTreeNode.h"

BPTreeNode::BPTreeNode(int max_order, NodeType node_type) : max_order(max_order), node_type(node_type)
{
    keys = std::vector<key_t>(max_order); // only allows (max_order - 1) keys
    if (node_type == NodeType::kLeafNode)
    {
        values = std::vector<value_t>(max_order); // only allows (max_order - 1) values
    }
    else
    {
        children = std::vector<BPTreeNode *>(max_order);
    }

    parent = nullptr;
    num_keys = 0;
}

