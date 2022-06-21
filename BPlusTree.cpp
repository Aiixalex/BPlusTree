#include "BPlusTree.h"
#include "BPTreeNode.h"
#include <iostream>
#include <cmath>

BPlusTree::BPlusTree(int n) : max_order(n + 1)
{
    root = new BPTreeNode(max_order, NodeType::kLeafNode);
}

std::string BPlusTree::find(key_t key)
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

void BPlusTree::__insert_kv_to_leaf_node(BPTreeNode* node, key_t key, value_t value)
{
    int i = 0;
    while (i < node->num_keys && key > node->keys[i])
    {
        i++;
    }
    for (int j = node->num_keys; j > i; j--)
    {
        node->keys[j] = node->keys[j - 1];
        node->values[j] = node->values[j - 1];
    }
    node->keys[i] = key;
    node->values[i] = value;
    node->num_keys++;
}

void BPlusTree::__insert_key_to_internal_node(BPTreeNode* node, key_t key, BPTreeNode* child)
{
    int i = 0;
    while (i < node->num_keys && key > node->keys[i])
    {
        i++;
    }
    for (int j = node->num_keys; j > i; j--)
    {
        node->keys[j] = node->keys[j - 1];
        node->children[j+1] = node->children[j];
    }
    node->keys[i] = key;
    node->children[i+1] = child;
    child->parent = node;
    node->num_keys++;
}

bool BPlusTree::insert(key_t key, value_t value)
{
    if (root == nullptr)
    {
        root = new BPTreeNode(max_order, NodeType::kLeafNode);
        root->keys[0] = key;
        root->values[0] = value;
        root->num_keys = 1;
        return true;
    }

    if (find(key) != "")
    {
        return false;
    }

    BPTreeNode* curr_node = root;
    while (curr_node->node_type == NodeType::kInternalNode)
    {
        int i = 0;
        while (i < curr_node->num_keys && key > curr_node->keys[i])
        {
            i++;
        }
        curr_node = curr_node->children[i];
    }

    if (curr_node->num_keys < max_order - 1)
    {
        __insert_kv_to_leaf_node(curr_node, key, value);
        return true;
    }
    else
    {
        __insert_kv_to_leaf_node(curr_node, key, value);

        // split the overflow node with max_order keys
        BPTreeNode* new_node = new BPTreeNode(max_order, NodeType::kLeafNode);
        new_node->next = curr_node->next;
        curr_node->next = new_node;
        int split_right_count = ceil((double)max_order / 2);
        int split_left_count = max_order / 2;
        for (int i = 0; i < split_right_count; i++)
        {
            new_node->keys[i] = curr_node->keys[i + split_left_count];
            new_node->values[i] = curr_node->values[i + split_left_count];
        }
        new_node->num_keys = split_right_count;
        curr_node->num_keys = split_left_count;

        // insert the first key in new_node to the parent node
        BPTreeNode* parent_node = curr_node->parent;
        if (parent_node == nullptr)
        {
            parent_node = new BPTreeNode(max_order, NodeType::kInternalNode);
            parent_node->keys[0] = new_node->keys[0];
            parent_node->children[0] = curr_node;
            parent_node->children[1] = new_node;
            parent_node->num_keys = 1;
            curr_node->parent = parent_node;
            new_node->parent = parent_node;
            root = parent_node;
            return true;
        }
        else
        {
            __insert_key_to_internal_node(parent_node, new_node->keys[0], new_node);
            while (parent_node->num_keys > max_order - 1)
            {
                curr_node = parent_node;
                new_node = new BPTreeNode(max_order, NodeType::kInternalNode);
                new_node->next = curr_node->next;
                curr_node->next = new_node;
                int split_right_count = (max_order - 1) / 2;
                int split_left_count = ceil(((double)max_order - 1) / 2);
                for (int i = 0; i < split_right_count; i++)
                {
                    new_node->keys[i] = curr_node->keys[i + split_left_count + 1];
                }
                for (int i = 0; i < split_right_count + 1; i++)
                {
                    new_node->children[i] = curr_node->children[i + split_left_count + 1];
                    new_node->children[i]->parent = new_node;
                }
                new_node->num_keys = split_right_count;
                curr_node->num_keys = split_left_count;

                parent_node = curr_node->parent;
                if (parent_node == nullptr)
                {
                    parent_node = new BPTreeNode(max_order, NodeType::kInternalNode);
                    parent_node->keys[0] = curr_node->keys[split_left_count];
                    parent_node->children[0] = curr_node;
                    parent_node->children[1] = new_node;
                    parent_node->num_keys = 1;
                    curr_node->parent = parent_node;
                    new_node->parent = parent_node;
                    root = parent_node;
                }
                else
                {
                    __insert_key_to_internal_node(parent_node, curr_node->keys[split_left_count], new_node);
                }
            }
            return true;
        }
    }
}

void BPlusTree::printKeys()
{
    BPTreeNode* first_node_each_level = root;
    while (1)
    {
        BPTreeNode* curr_node = first_node_each_level;
        while (curr_node != nullptr)
        {
            if (curr_node != first_node_each_level)
            {
                std::cout << " ";
            }
            std::cout << "[";
            for (int i = 0; i < curr_node->num_keys; i++)
            {
                if (i != 0)
                {
                    std::cout << ",";
                }
                std::cout << curr_node->keys[i];
            }
            std::cout << "]";
            curr_node = curr_node->next;
        }
        std::cout << std::endl;
        if (first_node_each_level->node_type == NodeType::kInternalNode)
        {
            first_node_each_level = first_node_each_level->children[0];
        }
        else
        {
            break;
        }
    }
}

void BPlusTree::printValues()
{
    BPTreeNode* curr_node = root;
    while (curr_node->node_type == NodeType::kInternalNode)
    {
        curr_node = curr_node->children[0];
    }

    while (curr_node != nullptr)
    {
        for (int i = 0; i < curr_node->num_keys; i++)
        {
            std::cout << curr_node->values[i] << std::endl;
        }
        curr_node = curr_node->next;
    }
}

int main()
{
    BPlusTree bptree(3);
    bptree.insert(2, "a");
    bptree.printKeys();
    bptree.insert(11, "e");
    bptree.printKeys();
    bptree.insert(21, "f");
    bptree.printKeys();
    bptree.insert(8, "d");
    bptree.printKeys();
    bptree.insert(64, "h");
    bptree.printKeys();
    bptree.insert(5, "b");
    bptree.printKeys();
    bptree.insert(23, "g");
    bptree.printKeys();
    bptree.insert(6, "c");
    bptree.printKeys();
    bptree.insert(65, "i");
    bptree.printKeys();
    bptree.insert(66, "j");
    bptree.printKeys();
    bptree.printValues();
}