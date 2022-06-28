// 
//
#include "SplayTree.h"
#include "AVL_Tree.h"
#include <iostream>
#include <string>

//
// Link to a data structure visualization tool (https://www.cs.usfca.edu/~galles/visualization/Algorithms.html)
//


class AVLTests {

};

class SplayTests {
public:
    // NOTES: 
    // splay tree matches refrence site for most insertions, but is different for some
    // our splay tree still maintains the correct properties, so I think the our tree just 
    // splays in a different way compared to the test site.

    void generalTests() // contains the general tests to make sure the tree works
    {
        SplayTree<int> myTree;
 
        myTree.insert(5); // matches example site
        myTree.insert(21); // matches example site
        myTree.insert(35); // matches example site
        myTree.insert(100); // matches example site
        myTree.insert(6); // matches example site
        myTree.insert(1); // matches example site
        myTree.insert(4); // matches example site
        myTree.insert(10); // Does NOT match refrence site (right sub-tree matches, but left sub-tree does not) 
        myTree.insert(13); // Does NOT match refrence site (right sub-tree matches, but left sub-tree does not) 

        //printTree(myTree._root);
        //std::cout << "Size of tree: " << myTree._size << std::endl;
        //std::cout << "Root is: " << myTree._root->_data << std::endl;

        myTree.remove(5); // matches refrence site 
        myTree.remove(10); // matches refrence site 
        myTree.remove(1); // matches refrence site
        myTree.remove(21); // Does NOT match refrence site (right sub-tree matches, but left sub-tree does not) 
        myTree.remove(35); // Does NOT match refrence site (right sub-tree matches, but left sub-tree does not) 
        myTree.remove(6); // matches refrence site
        myTree.remove(100); // matches refrence site

        printTree(myTree._root);
        std::cout << "Size of tree: " << myTree._size << std::endl;
        std::cout << "Root is: " << myTree._root->_data << std::endl;

        return;
    }

    void printTree(SplayTree<int>::Node* p)
    {
        if (p == nullptr)
        {
            return;
        }

        printTree(p->_left);

        std::cout << p->_data << " -- Children: ";
        if (p->_left == nullptr && p->_right != nullptr)
        {
            std::cout << "left is null -";
            std::cout << " right is " << p->_right->_data << std::endl;
        }
        else if (p->_right == nullptr && p->_left != nullptr)
        {
            std::cout << " right is null -";
            std::cout << " left is " << p->_left->_data << std::endl;
        }
        else if (p->_right != nullptr && p->_left != nullptr)
        {
            std::cout << " right is " << p->_right->_data;
            std::cout << " - left is " << p->_left->_data << std::endl;
        }
        else
        {
            std::cout << "Left and Right are null" << std::endl;
        }

        printTree(p->_right); 
    }
};

int main()
{
    SplayTests rs;
    rs.generalTests();
}


