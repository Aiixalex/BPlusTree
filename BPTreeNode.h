#ifndef _BPTREENODE_H_
#define _BPTREENODE_H_

#include <string>

typedef int key_t;
typedef std::string value_t;

struct node_t
{
    key_t key;
    node_t *prev;
    node_t *next;
};

class BPlusTreeNode {
public:
    node_t *nodes[];
};

#endif