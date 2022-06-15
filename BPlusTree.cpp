#include "BPlusTree.h"
#include "BPTreeNode.h"

BPlusTree::BPlusTree(int n) : max_order(n)
{
    root = new BPTreeNode(max_order, NodeType::kLeafNode);
}

bool BPlusTree::insert(int key, std::string value)
{
    if (root == nullptr)
    {
        root = new BPTreeNode(max_order, NodeType::kLeafNode);
    }

}

std::string BPlusTree::find(int key)
{
    if (root == nullptr)
    {
        return "";
    }
    BPTreeNode* curr_node = root;

    while (1)
    {
        if (curr_node->node_type == NodeType::kLeafNode)
        {
            for (int i = 0; i < curr_node->num_keys; i++)
            {
                if (curr_node->keys[i] == key)
                {
                    return curr_node->values[i];
                }
            }
            return "";
        }
        else if (curr_node->node_type == NodeType::kInternalNode)
        {
            int i = 0;
            while (i < curr_node->num_keys && key >= curr_node->keys[i])
            {
                i++;
            }
            curr_node = curr_node->children[i];
        }
    }
}

void BPlusTree::printKeys()
{
    
}