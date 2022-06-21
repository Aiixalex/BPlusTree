#include "BPlusTree.h"
#include "BPTreeNode.h"
#include <iostream>

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
    std::cout << bptree.find(2) << bptree.find(6) << bptree.find(11) << bptree.find(23) << bptree.find(65) << std::endl;
}