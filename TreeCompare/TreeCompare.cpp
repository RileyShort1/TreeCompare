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
    // right now it seems my splay tree does not match the refrence site that I linked to above,
    // but I think that it just splays it in a slightly different way - I will double check it

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

        printTree(myTree._root);

        //std::cout << myTree._root->_data << std::endl;
        //std::cout << myTree._root->_left->_data << std::endl;
        //std::cout << myTree._root->_right->_data << std::endl;
        //std::cout << myTree._root->_left->_left->_data << std::endl;


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


