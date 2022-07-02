#include "BPlusTree.h"
#include "BPTreeNode.h"
#include <iostream>
#include <cmath>

BPlusTree::BPlusTree(int n) : max_order(n + 1)
{
    root = std::make_shared<BPTreeNode>(max_order, NodeType::kLeafNode);
}

BPlusTree::~BPlusTree()
{
}

std::string BPlusTree::find(key_t key)
{
    if (root == nullptr)
    {
        return "";
    }
    auto curr_node = root;

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

void BPlusTree::__insert_kv_to_leaf_node(std::shared_ptr<BPTreeNode> node, key_t key, value_t value)
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

void BPlusTree::__insert_key_to_internal_node(std::shared_ptr<BPTreeNode> node, key_t key, std::shared_ptr<BPTreeNode> child)
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
        root = std::make_shared<BPTreeNode>(max_order, NodeType::kLeafNode);
        root->keys[0] = key;
        root->values[0] = value;
        root->num_keys = 1;
        return true;
    }

    // if key already exists, return false
    if (find(key) != "")
    {
        std::cout << "Insertion Error: Key " << key << " already exists" << std::endl;
        return false;
    }

    // find the leaf node to insert the key-value pair
    auto curr_node = root;
    while (curr_node->node_type == NodeType::kInternalNode)
    {
        int i = 0;
        while (i < curr_node->num_keys && key > curr_node->keys[i])
        {
            i++;
        }
        curr_node = curr_node->children[i];
    }

    __insert_kv_to_leaf_node(curr_node, key, value);
    
    // if the leaf node is not full, return true
    if (curr_node->num_keys <= max_order - 1)
    {
        return true;
    }

    // split the overflow node with max_order keys
    auto new_node = std::make_shared<BPTreeNode>(max_order, NodeType::kLeafNode);
    new_node->next = curr_node->next;
    curr_node->next = new_node;
    new_node->prev = curr_node;
    if (new_node->next != nullptr)
    {
        new_node->next->prev = new_node;
    }
    int split_right_count = max_order / 2;
    int split_left_count = ceil((double)max_order / 2);
    for (int i = 0; i < split_right_count; i++)
    {
        new_node->keys[i] = curr_node->keys[i + split_left_count];
        new_node->values[i] = curr_node->values[i + split_left_count];
    }
    new_node->num_keys = split_right_count;
    curr_node->num_keys = split_left_count;

    // insert the first key in new_node to the parent node
    std::shared_ptr<BPTreeNode> parent_node = (curr_node->parent).lock();

    if (parent_node == nullptr) // if the parent node is nullptr, create a new root node
    {
        parent_node = std::make_shared<BPTreeNode>(max_order, NodeType::kInternalNode);
        parent_node->keys[0] = new_node->keys[0];
        parent_node->children[0] = curr_node;
        parent_node->children[1] = new_node;
        parent_node->num_keys = 1;
        curr_node->parent = parent_node;
        new_node->parent = parent_node;
        root = parent_node;
        return true;
    }

    __insert_key_to_internal_node(parent_node, new_node->keys[0], new_node);

    // split the parent node until no more overflow
    while (parent_node->num_keys > max_order - 1)
    {
        curr_node = parent_node;
        auto new_node = std::make_shared<BPTreeNode>(max_order, NodeType::kInternalNode);
        new_node->next = curr_node->next;
        curr_node->next = new_node;
        new_node->prev = curr_node;
        if (new_node->next != nullptr)
        {
            new_node->next->prev = new_node;
        }
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

        auto parent_node = (curr_node->parent).lock();
        if (parent_node == nullptr)
        {
            parent_node = std::make_shared<BPTreeNode>(max_order, NodeType::kInternalNode);
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

void BPlusTree::__remove_kv_from_leaf_node(std::shared_ptr<BPTreeNode> node, key_t key)
{
    int i = 0;
    while (i < node->num_keys && key > node->keys[i])
    {
        i++;
    }
    for (int j = i; j < node->num_keys - 1; j++)
    {
        node->keys[j] = node->keys[j + 1];
        node->values[j] = node->values[j + 1];
    }
    node->num_keys--;
}

std::shared_ptr<BPTreeNode> BPlusTree::__get_left_sibling_node_to_redistribute(std::shared_ptr<BPTreeNode> curr_node)
{
    auto left_sibling_node = (curr_node->prev).lock();
    if (left_sibling_node == nullptr || left_sibling_node->parent.lock() != curr_node->parent.lock() || left_sibling_node->num_keys < max_order / 2 + 1)
    {
        return nullptr;
    }
    return left_sibling_node;
}

std::shared_ptr<BPTreeNode> BPlusTree::__get_right_sibling_node_to_redistribute(std::shared_ptr<BPTreeNode> curr_node)
{
    auto right_sibling_node = (curr_node->next);
    if (right_sibling_node == nullptr || right_sibling_node->parent.lock() != curr_node->parent.lock() || right_sibling_node->num_keys < max_order / 2 + 1)
    {
        return nullptr;
    }
    return right_sibling_node;
}

std::shared_ptr<BPTreeNode> BPlusTree::__get_left_sibling_node_to_merge(std::shared_ptr<BPTreeNode> curr_node)
{
    auto left_sibling_node = (curr_node->prev).lock();
    if (left_sibling_node == nullptr || left_sibling_node->parent.lock() != curr_node->parent.lock() || left_sibling_node->num_keys > max_order - 1 - curr_node->num_keys)
    {
        return nullptr;
    }
    return left_sibling_node;
}

std::shared_ptr<BPTreeNode> BPlusTree::__get_right_sibling_node_to_merge(std::shared_ptr<BPTreeNode> curr_node)
{
    auto right_sibling_node = (curr_node->next);
    if (right_sibling_node == nullptr || right_sibling_node->parent.lock() != curr_node->parent.lock() || right_sibling_node->num_keys > max_order - 1 - curr_node->num_keys)
    {
        return nullptr;
    }
    return right_sibling_node;
}

// void BPlusTree::__remove_key_from_internal_node(std::shared_ptr<BPTreeNode> node, key_t key)
// {
//     int i = 0;
//     while (i < node->num_keys && key > node->keys[i])
//     {
//         i++;
//     }
//     for (int j = i; j < node->num_keys - 1; j++)
//     {
//         node->keys[j] = node->keys[j + 1];
//         node->children[j + 1] = node->children[j + 2];
//     }
//     node->num_keys--;
// }

void BPlusTree::__merge_triangle(std::shared_ptr<BPTreeNode> left, std::shared_ptr<BPTreeNode> right, std::shared_ptr<BPTreeNode> parent)
{
    left->keys[left->num_keys] = parent->keys[0];
    for (int i = 0; i <= right->num_keys; i++)
    {
        left->children[left->num_keys + i + 1] = right->children[i];
        left->children[left->num_keys + i + 1]->parent = left;
    }
    for (int i = 0; i < right->num_keys; i++)
    {
        left->keys[left->num_keys + i + 1] = right->keys[i];
    }
    left->num_keys += right->num_keys + 1;
    left->next = right->next;
    if (left->next != nullptr)
    {
        (left->next)->prev = left;
    }
    root = left;
}

void BPlusTree::__merge(std::shared_ptr<BPTreeNode> left, std::shared_ptr<BPTreeNode> right)
{
    for (int i = 0; i < right->num_keys; i++)
    {
        left->keys[left->num_keys + i] = right->keys[i];
        left->values[left->num_keys + i] = right->values[i];
    }
    left->num_keys += right->num_keys;
    left->next = right->next;
    if (left->next != nullptr)
    {
        (left->next)->prev = left;
    }

    auto parent = (left->parent).lock();
    int i = 0;
    for (i = 0; i < parent->num_keys; i++)
    {
        if (parent->children[i] == right) break;
    }
    for (int j = i - 1; j < parent->num_keys - 1; j++)
    {
        parent->keys[j] = parent->keys[j + 1];
        parent->children[j + 1] = parent->children[j + 2];
    }
    parent->num_keys--;

    if (parent == root && parent->num_keys == 0)
    {
        root = left;
        (left->parent).lock() = nullptr;
        return;
    }

    if (parent != root && parent->num_keys < ceil(max_order / 2.0) - 1)
    {
        auto internal = parent;
        auto internal_parent = (internal->parent).lock();
        auto left_sibling = (internal->prev).lock();
        auto right_sibling = (internal->next);

        if (left_sibling != nullptr && (left_sibling->parent.lock() == internal_parent) && left_sibling->num_keys > ceil(max_order / 2.0) - 1)
        {
            for (int i = internal->num_keys - 1; i >= 0; i--)
            {
                internal->keys[i + 1] = internal->keys[i];
            }
            for (int i = internal->num_keys; i >= 0; i--)
            {
                internal->children[i + 1] = internal->children[i];
            }
            int i = 0;
            for (; i < internal_parent->num_keys; i++)
            {
                if (internal_parent->children[i] == internal) break;
            }
            internal->keys[0] = internal_parent->keys[i - 1];
            internal->children[0] = left_sibling->children[left_sibling->num_keys];
            internal->children[0]->parent = internal;
            internal->num_keys++;
            internal_parent->keys[i-1] = left_sibling->keys[left_sibling->num_keys - 1];
            left_sibling->num_keys--;
            return;
        }

        if (right_sibling != nullptr && (right_sibling->parent.lock() == internal_parent) && right_sibling->num_keys > ceil(max_order / 2.0) - 1)
        {
            int i = 0;
            for (; i < internal_parent->num_keys; i++)
            {
                if (internal_parent->children[i] == internal) break;
            }
            internal->keys[internal->num_keys] = internal_parent->keys[i];
            internal->children[internal->num_keys + 1] = right_sibling->children[0];
            internal->children[internal->num_keys + 1]->parent = internal;
            internal->num_keys++;
            internal_parent->keys[i] = right_sibling->keys[0];
            for (int j = 0; j < right_sibling->num_keys - 1; j++)
            {
                right_sibling->keys[j] = right_sibling->keys[j + 1];
            }
            for (int j = 0; j < right_sibling->num_keys; j++)
            {
                right_sibling->children[j] = right_sibling->children[j + 1];
            }
            right_sibling->num_keys--;
            return;
        }

        if (left_sibling != nullptr && (left_sibling->parent.lock() == internal_parent) && left_sibling->num_keys + internal_parent->num_keys < max_order - 1 && internal_parent->num_keys == 1)
        {
            __merge_triangle(left_sibling, internal, internal_parent);
            return;
        }

        if (right_sibling != nullptr && (right_sibling->parent.lock() == internal_parent) && right_sibling->num_keys + internal_parent->num_keys < max_order - 1 && internal_parent->num_keys == 1)
        {
            __merge_triangle(internal, right_sibling, internal_parent);
            return;
        }
    }
}

bool BPlusTree::remove(key_t key)
{
    if (root == nullptr)
    {
        return false;
    }

    if (find(key) == "")
    {
        return false;
    }

    // find the leaf node containing the key
    auto curr_node = root;
    while (curr_node->node_type == NodeType::kInternalNode)
    {
        int i = 0;
        while (i < curr_node->num_keys && key >= curr_node->keys[i])
        {
            i++;
        }
        curr_node = curr_node->children[i];
    }

    // remove the key-value pair from the leaf node
    __remove_kv_from_leaf_node(curr_node, key);

    // if the leaf node is not underflow, return true
    if (curr_node->num_keys >= max_order / 2 || curr_node == root)
    {
        return true;
    }

    // if the leaf node is underflow，redistribute values from the left sibling node
    auto left_redistribute_sibling = __get_left_sibling_node_to_redistribute(curr_node);
    if (left_redistribute_sibling != nullptr)
    {
        left_redistribute_sibling->num_keys--;
        __insert_kv_to_leaf_node(curr_node, left_redistribute_sibling->keys[left_redistribute_sibling->num_keys], left_redistribute_sibling->values[left_redistribute_sibling->num_keys]);

        auto parent_node = (curr_node->parent).lock();
        int i = 0;
        while (i < parent_node->num_keys - 1 && parent_node->keys[i] < curr_node->keys[0])
        {
            i++;
        }
        parent_node->keys[i] = curr_node->keys[0];

        return true;
    }

    // try to redistribute values from the right sibling node
    auto right_redistribute_sibling = __get_right_sibling_node_to_redistribute(curr_node);
    if (right_redistribute_sibling != nullptr)
    {
        __insert_kv_to_leaf_node(curr_node, right_redistribute_sibling->keys[0], right_redistribute_sibling->values[0]);
        right_redistribute_sibling->num_keys--;
        for (int i = 0; i < right_redistribute_sibling->num_keys; i++)
        {
            right_redistribute_sibling->keys[i] = right_redistribute_sibling->keys[i + 1];
            right_redistribute_sibling->values[i] = right_redistribute_sibling->values[i + 1];
        }

        auto parent_node = (curr_node->parent).lock();
        int i = 0;
        while (i < parent_node->num_keys && parent_node->keys[i] < right_redistribute_sibling->keys[0])
        {
            i++;
        }
        i--;
        parent_node->keys[i] = right_redistribute_sibling->keys[0];

        return true;
    }

    // if both left and right sibling nodes are underflow, merge the curr node to its left sibling node
    auto left_merge_sibling = __get_left_sibling_node_to_merge(curr_node);
    if (left_merge_sibling != nullptr)
    {
        __merge(left_merge_sibling, curr_node);

        return true;
    }

    // try merge the right sibling node to the curr node
    auto right_merge_sibling = __get_right_sibling_node_to_merge(curr_node);
    if (right_merge_sibling != nullptr)
    {
        __merge(curr_node, right_merge_sibling);

        return true;
    }

    return true;
}

void BPlusTree::printKeys()
{
    auto first_node_each_level = root;
    while (1)
    {
        auto curr_node = first_node_each_level;
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
    auto curr_node = root;
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
