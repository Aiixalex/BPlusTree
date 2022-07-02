// #include "BPlusTree.h"
// #include "BPTreeNode.h"
// #include <iostream>

// int main()
// {
//     BPlusTree bptree(3);
//     bptree.insert(2, "a");
//     bptree.printKeys();
//     bptree.insert(11, "e");
//     bptree.printKeys();
//     bptree.insert(21, "f");
//     bptree.printKeys();
//     bptree.insert(8, "d");
//     bptree.printKeys();
//     bptree.insert(64, "h");
//     bptree.printKeys();
//     bptree.insert(5, "b");
//     bptree.printKeys();
//     bptree.insert(23, "g");
//     bptree.printKeys();
//     bptree.insert(6, "c");
//     bptree.printKeys();
//     bptree.insert(65, "i");
//     bptree.printKeys();
//     bptree.insert(66, "j");
//     bptree.insert(66, "j");
//     bptree.printKeys();
//     bptree.insert(67, "k");
//     bptree.printKeys();
//     bptree.insert(1, "1");
//     bptree.printKeys();
//     bptree.insert(68, "l");
//     bptree.printKeys();
//     bptree.printValues();
//     std::cout << bptree.find(2) << bptree.find(6) << bptree.find(11) << bptree.find(23) << bptree.find(65) << std::endl;

//     bptree.remove(6);
//     bptree.printKeys();
//     bptree.remove(5);
//     bptree.printKeys();
//     bptree.remove(8);
//     bptree.printKeys();
//     bptree.remove(2);
//     bptree.printKeys();
//     bptree.remove(23);
//     bptree.printKeys();
//     // bptree.remove(11);
//     // bptree.printKeys();
//     bptree.remove(65);
//     bptree.printKeys();
//     bptree.remove(11);
//     bptree.printKeys();
//     bptree.remove(1);
//     bptree.printKeys();
//     bptree.remove(21);
//     bptree.printKeys();
//     bptree.remove(68);
//     bptree.printKeys();
// }

// Basic test for BP Tree
// John Edgar
// June 2022

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>
#include "BPlusTree.h"
using namespace std;

void simpleTest();

int main() {

	simpleTest();

	cout << endl;
}

void simpleTest()
{
	BPlusTree bp1(4);

	bp1.insert(7, "seven");
	bp1.insert(1, "one");
	bp1.insert(3, "three");
	bp1.insert(9, "nine");
	bp1.insert(5, "five");

	cout << "find 3: " << bp1.find(3) << " (three)" << endl;
	bp1.remove(7);
	cout << "find 7: " << bp1.find(7) << " (<empty>)" << endl << endl;

	bp1.printKeys();
	cout << endl << "CHECK" << endl;
	cout << "[5]" << endl;
	cout << "[1 3] [5 9]" << endl << endl;
	bp1.printValues();
	cout << endl << "CHECK" << endl;
	cout << "one" << endl << "three" << endl << "five" << endl << "nine" << endl;

	BPlusTree bp2(bp1);

	BPlusTree bp3(7);
	bp3.insert(13, "thirteen");
	bp3 = bp1;

	cout << endl << "simple test complete" << endl;
}