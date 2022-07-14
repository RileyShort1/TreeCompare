// 
//
#include "SplayTree.h"
#include "AVL_Tree.h"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>


//
// Link to a data structure visualization tool (https://www.cs.usfca.edu/~galles/visualization/Algorithms.html)
//


 
/*

class AVLTests {
public:

    void generalTests()
    {
       
      return;
    }

    void printTree(AVL_Tree<int>::Node* p)
    {
        if (p == nullptr)
        {
            return;
        }

        printTree(p->_left);

        std::cout << p->_data << " -- Height = " << p->_height << " -- Children: ";
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

class SplayTests {
public:
    
    void generalTests() // contains the general tests to make sure the tree works
    {
       
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

*/
/*
In your main you should
srand(XYZ)
create AVL Tree
generate 1M rands and stuff into the tree, noting clock value after every 1000
srand back to xyz
create Splay Tree
same as step 3
Generate 1M rands and look up in AVL, noting clock value after every 1000 - 
- in this case you're only interested in the final number since the tree sizes are fixed, but sample every 1000 anyway.
in a loop from 1 to 100,000 remove each number sequentially from the AVL Tree, noting clock per 1000 as before.
Same for splay trees
Both trees should be empty after all the removals
Pull the data into a google spreadsheetand we can generate any necessary plots.
*/

// class for our final benchmarking
class Benchmark {
public:

    // { 1,2,3,4,5,10 } with N = 1 yeilding uniform data, and N > 1 yeilding rands from aprox gaussian
    int rand_uniform(size_t n) // gets uniform data
    {
        int final_rand = 0;

        for (size_t i = 0; i < n; i++)
        {
            final_rand = rand() % 100000;
        }

        return final_rand;
    }

    int rand_gaussian(size_t n) // gets normal data
    {
        int final_rand = 0;

        for (size_t i = 0; i < n; i++)
        {
            final_rand += rand() % 100000;
        }

        return (int)final_rand / n;
    }

    void testSplay(int randSeed, size_t N, int num_rands, bool createFile)
    {
        srand(randSeed); // seed rand
        int seconds_to_micro = 1000000; // convert to microseconds

        if (createFile == true) // create file branch
        {
            std::fstream fout; // output file
            fout.open("Splay_Data.csv", std::ios::out | std::ios::app);
        }

        clock_t timeForInsert;
        clock_t timeForRemove;

        SplayTree<int> testTree;

        std::vector<int> rands;

        // get data to insert (put it in a vector??)
        // get normal data

        for (int i = 0; i < num_rands; i++)
        {
            rands.push_back(rand_gaussian(N));
        }

        // ------------------------------- SPLAY INSERT -----------------------------------------------
        // insert data - noting clock every 1000 elements
        std::vector<clock_t> timesPer1kInsert;

        timeForInsert = clock();

        for (int i = 1; i < num_rands + 1; i++)
        {
            testTree.insert(rands[i - 1]);
           
            if (i % 1000 == 0)
            {
                timeForInsert = clock() - timeForInsert; // get clock time
                timesPer1kInsert.push_back(timeForInsert);
                timeForInsert = clock(); // reset clock
            }
        }

       clock_t totalTime = 0;
       double avg;

        for (size_t i = 0; i < timesPer1kInsert.size(); i++)
        {
            totalTime += timesPer1kInsert[i];
        }

        avg = (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro;
        avg = avg / timesPer1kInsert.size();

        std::cout << "Processor time taken for inserting 1M elements into splay tree: "
            << std::fixed << std::setprecision(5) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

        std::cout << "Average time per 1k inserts in microseconds: " << std::fixed << std::setprecision(5) << avg << std::endl;
        std::cout << "Splay tree size: " << testTree.get_size() << "\n\n";

        // ------------------------------------ SPLAY FIND -----------------------------------------------
        clock_t timeForFind;
        std::vector<clock_t> timesPer1kFind;

        timeForFind = clock();

        for (size_t i = 1; i < rands.size() + 1; i++)
        {
            testTree.contains(rands[i - 1]);

            if (i % 1000 == 0)
            {
                timeForFind = clock() - timeForFind; // get clock time
                timesPer1kFind.push_back(timeForFind);
                timeForFind = clock(); // reset clock
            }
        }

        clock_t totalTimeFind = 0;
        double avgFind;

        for (size_t i = 0; i < timesPer1kFind.size(); i++)
        {
            totalTimeFind += timesPer1kFind[i];
        }

        avgFind = (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro;
        avgFind = avgFind / timesPer1kFind.size();

        std::cout << "Processor time taken to find 1m elements in Splay tree: "
            << std::fixed << std::setprecision(5) << (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

        std::cout << "Average time per 1k find operations in microseconds: " << std::fixed << std::setprecision(5) << avgFind << std::endl;
        std::cout << "Splay tree size: " << testTree.get_size() << "\n\n";


        // ------------------------------- SPLAY REMOVE ---------------------------------------------------
        std::vector<clock_t> timesPer1kRemove;
        timeForRemove = clock();

        // remove data - noting clock every 1000 elements
        // loop 1 - 100000
        for (int i = 1; i < 100001; i++)
        {
            testTree.remove(i);

            if (i % 1000 == 0)
            {
                timeForRemove = clock() - timeForRemove; // get clock time
                timesPer1kRemove.push_back(timeForRemove);
                timeForRemove = clock(); // reset clock
            }
        }

        clock_t totalTimeRemove = 0;
        double avgRemove;

        for (size_t i = 0; i < timesPer1kRemove.size(); i++)
        {
            totalTimeRemove += timesPer1kRemove[i];
        }

        avgRemove = (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro;
        avgRemove = avgRemove / timesPer1kRemove.size();

        std::cout << "Processor time taken for removing all elements (1 - 100,000) from Splay tree: "
            << std::fixed << std::setprecision(5) << (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

        std::cout << "Average time per 1k remove operations in microseconds: " << std::fixed << std::setprecision(5) << avgRemove << std::endl;
        std::cout << "Splay tree size: " << testTree.get_size() << "\n";

        if (createFile == true)
        {


        }
        else
        {
            // ------------------------------------- INSERT ---------------------------------
            std::cout << "Processor time taken for inserting 1M elements into splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k inserts in microseconds: " << std::fixed << std::setprecision(5) << avg << std::endl;
            std::cout << "Splay tree size: " << testTree.get_size() << "\n\n";

            // ----------------------------------- FIND ----------------------------------------
            std::cout << "Processor time taken to find 1m elements in Splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k find operations in microseconds: " << std::fixed << std::setprecision(5) << avgFind << std::endl;
            std::cout << "Splay tree size: " << testTree.get_size() << "\n\n";

            // ------------------------------------ DELETE -------------------------------------
            std::cout << "Processor time taken for removing all elements (1 - 100,000) from Splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k remove operations in microseconds: " << std::fixed << std::setprecision(5) << avgRemove << std::endl;
            std::cout << "Splay tree size: " << testTree.get_size() << "\n";
        }


        
        return;

    }

    void testAVL(int randSeed, size_t N, int num_rands, bool createFile)
    {
        srand(randSeed); // seed rand
        int seconds_to_micro = 1000000; // convert to microseconds

        AVL_Tree<int> testTree;

        if (createFile == true) // create file branch
        {
            std::fstream fout; // output file
            fout.open("AVL_Data.csv", std::ios::out | std::ios::app);
        }
        
        clock_t timeForInsert;
        clock_t timeForRemove;

        std::vector<int> rands;

        // get data to insert (put it in a vector??)
        // get normal data

        for (int i = 0; i < num_rands; i++)
        {
            rands.push_back(rand_uniform(N));
        }

        // ------------------------------ AVL INSERT --------------------------------------
        // insert data - noting clock every 1000 elements
        std::vector<clock_t> timesPer1kInsert;

        timeForInsert = clock();

        for (int i = 1; i < num_rands + 1; i++)
        {         
            testTree.insert(rands[i-1]);

            if (i % 1000 == 0)
            {
                timeForInsert = clock() - timeForInsert; // get clock time
                timesPer1kInsert.push_back(timeForInsert);
                timeForInsert = clock(); // reset clock
            }
        }

        clock_t totalTime = 0;
        double avg;

        for (size_t i = 0; i < timesPer1kInsert.size(); i++)
        {
            totalTime += timesPer1kInsert[i];
        }

        avg = (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro;
        avg = avg / timesPer1kInsert.size();

        // ----------------------------- AVL FIND ----------------------------------------------

        clock_t timeForFind;
        std::vector<clock_t> timesPer1kFind;

        timeForFind = clock();

        for (size_t i = 1; i < rands.size() + 1; i++)
        {
            testTree.contains(rands[i - 1]);

            if (i % 1000 == 0)
            {
                timeForFind = clock() - timeForFind; // get clock time
                timesPer1kFind.push_back(timeForFind);
                timeForFind = clock(); // reset clock
            }
        }

        clock_t totalTimeFind = 0;
        double avgFind;

        for (size_t i = 0; i < timesPer1kFind.size(); i++)
        {
            totalTimeFind += timesPer1kFind[i];
        }

        avgFind = (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro;
        avgFind = avgFind / timesPer1kFind.size();

     
        // ----------------------------- AVL REMOVE ---------------------------------------------

        std::vector<clock_t> timesPer1kRemove;
        timeForRemove = clock();
     
        // remove data - noting clock every 1000 elements
        // loop 1 - 100000
        for (int i = 1; i < 100001; i++)
        {
            testTree.remove(i);

            if (i % 1000 == 0)
            {
                timeForRemove = clock() - timeForRemove; // get clock time
                timesPer1kRemove.push_back(timeForRemove);
                timeForRemove = clock(); // reset clock
            }
        }

        clock_t totalTimeRemove = 0;
        double avgRemove;

        for (size_t i = 0; i < timesPer1kRemove.size(); i++)
        {
            totalTimeRemove += timesPer1kRemove[i];
        }

        avgRemove = (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro;
        avgRemove = avgRemove / timesPer1kRemove.size();

       
        // compile data
        if (createFile == true) // insert data in file
        {

        }
        else // just print for testing
        {
            // ------------------------ INSERT -------------------------------------------

            std::cout << "Processor time taken for inserting 1M elements into AVL tree: "
                << std::fixed << std::setprecision(5) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k inserts in microseconds: " << std::fixed << std::setprecision(5) << avg << std::endl;
            std::cout << "AVL tree size: " << testTree.get_size() << "\n\n";

            // ----------------------------- FIND ---------------------------------------------

            std::cout << "Processor time taken to find 1m elements in AVL tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k find operations in microseconds: " << std::fixed << std::setprecision(5) << avgFind << std::endl;
            std::cout << "AVL tree size: " << testTree.get_size() << "\n\n";

            // ------------------------------- REMOVE ---------------------------------------

            std::cout << "Processor time taken for removing all elements (1 - 100,000) from AVL tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k remove operations in microseconds: " << std::fixed << std::setprecision(5) << avgRemove << std::endl;
            std::cout << "AVL tree size: " << testTree.get_size() << "\n";
        }

        return;
    }
};

int main()
{
    //SplayTests rs;
    //rs.generalTests();

    //AVLTests av;
    //av.generalTests();
   
    Benchmark x;
    std::cout << "SPLAY: \n";
    //       seed, N, number of rand calls, output to file?
    x.testSplay(5, 1, 1000000, false);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "AVL: \n";
    //     seed, N, number of rand calls, output to file?
    x.testAVL(5, 1, 1000000, false);
    // call functions

    
}


