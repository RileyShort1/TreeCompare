// 
//
#include "SplayTree.h"
#include "AVL_Tree.h"
#include <iostream>

//
// Link to a data structure visualization tool (https://www.cs.usfca.edu/~galles/visualization/Algorithms.html)
//


class AVLTests {

};

class SplayTests {

    void generalTests() // contains the general tests to make sure the tree works
    {
        //bool insert(const T & elem) { return splay_insert(elem); }
       // bool remove(const T & elem) { return splay_remove(elem); }

        SplayTree<int> myTree;

        myTree.insert(5);
        myTree.insert(7);
        myTree.insert(3);
        myTree.insert(8);
        myTree.insert(16);
        myTree.insert(1);
        myTree.insert(4);
        myTree.insert(10);
        myTree.insert(13);

    }

    void printTree(SplayTree<int>::Node* root)
    {
        return;
    }

};

int main()
{
    std::cout << "Hello World!\n";
}


