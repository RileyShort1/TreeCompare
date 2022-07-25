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

    void testSplay(int randSeed, size_t N, int num_rands, bool createFile, bool normalDist, std::string fileName)
    {
        srand(randSeed); // seed rand
        int seconds_to_micro = 1000000; // convert to microseconds

        clock_t timeForInsert;
        clock_t timeForRemove;

        SplayTree<int> testTree;

        std::vector<int> rands;

        // get appropriate data distribution

        if (normalDist == true)
        {
            for (int i = 0; i < num_rands; i++)
            {
                rands.push_back(rand_gaussian(N));
            }
        }
        else
        {
            for (int i = 0; i < num_rands; i++)
            {
                rands.push_back(rand_uniform(N));
            }
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

        size_t sizeAfterInsert = testTree.get_size();

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

        size_t sizeAfterRemove = testTree.get_size();

      
        if (createFile == true)
        {
            std::fstream fout; // output file
            fout.open(fileName, std::ios::out | std::ios::app);

            fout << std::fixed << std::setprecision(1) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avg << ", " <<
                (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgFind << ", " <<
                (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove << ", " << sizeAfterInsert <<
                "\n";
        }
        else
        {
            std::cout << "SPLAY TREE: " << std::endl;
            // ------------------------------------- INSERT ---------------------------------
            std::cout << "Processor time taken for attempting insertion of " << num_rands << " elements into splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k insert attempts in microseconds: " << std::fixed << std::setprecision(5) << avg << std::endl;
            std::cout << "Splay tree size: " << sizeAfterInsert << "\n\n";

            // ----------------------------------- FIND ----------------------------------------
            std::cout << "Processor time taken for attempting to find " << num_rands << " in Splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k find operations in microseconds: " << std::fixed << std::setprecision(5) << avgFind << std::endl;
            std::cout << "Splay tree size: " << sizeAfterInsert << "\n\n";

            // ------------------------------------ DELETE -------------------------------------
            std::cout << "Processor time taken for removing all elements (1 - 100,000) from Splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k remove operations in microseconds: " << std::fixed << std::setprecision(5) << avgRemove << std::endl;
            std::cout << "Splay tree size: " << sizeAfterRemove << "\n";
        }


        
        return;

    }

    void testAVL(int randSeed, size_t N, int num_rands, bool createFile, bool normalDist, std::string fileName)
    {
        srand(randSeed); // seed rand
        int seconds_to_micro = 1000000; // convert to microseconds

        AVL_Tree<int> testTree;
      
        clock_t timeForInsert;
        clock_t timeForRemove;

        std::vector<int> rands;

        // get data to insert (put it in a vector??)
        // get normal data

        if (normalDist == true)
        {
            for (int i = 0; i < num_rands; i++)
            {
                rands.push_back(rand_gaussian(N));
            }
        }
        else
        {
            for (int i = 0; i < num_rands; i++)
            {
                rands.push_back(rand_uniform(N));
            }
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

        size_t sizeAfterInsert = testTree.get_size();

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

        size_t sizeAfterRemove = testTree.get_size();

        // compile data
        if (createFile == true) // insert data in file
        {

            std::fstream fout; // output file
            fout.open(fileName, std::ios::out | std::ios::app);

            fout << std::fixed << std::setprecision(1) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avg << ", " <<
                (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgFind << ", " <<
                (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove << ", " << sizeAfterInsert <<
                "\n";
        }
        else // just print for testing
        {
            // ------------------------------------- INSERT ---------------------------------
            std::cout << "AVL TREE: " << std::endl;
            std::cout << "Processor time taken for attempting insertion of " << num_rands << " elements into splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k insert attempts in microseconds: " << std::fixed << std::setprecision(5) << avg << std::endl;
            std::cout << "Splay tree size: " << sizeAfterInsert << "\n\n";

            // ----------------------------------- FIND ----------------------------------------
            std::cout << "Processor time taken for attempting to find " << num_rands << " in Splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k find operations in microseconds: " << std::fixed << std::setprecision(5) << avgFind << std::endl;
            std::cout << "Splay tree size: " << sizeAfterInsert << "\n\n";

            // ------------------------------------ DELETE -------------------------------------
            std::cout << "Processor time taken for removing all elements (1 - 100,000) from Splay tree: "
                << std::fixed << std::setprecision(5) << (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << " microseconds" << std::endl;

            std::cout << "Average time per 1k remove operations in microseconds: " << std::fixed << std::setprecision(5) << avgRemove << std::endl;
            std::cout << "Splay tree size: " << sizeAfterRemove << "\n";
        }

        return;
    }
    void buildFileHeader(int randSeed, size_t N, int numRandCalls, std::string dataType, std::string fileNameSplay, std::string fileNameAVL) // writes header for two files
    {
        std::fstream foutSplay2; // output file
        foutSplay2.open(fileNameSplay, std::ios::out | std::ios::app);

        foutSplay2 << "Rand seed = " << randSeed << ", " << "N = " << N << ", " << "num of rands = " << numRandCalls << ", "
            << dataType << ", " << "Splay Tree with microsecond time" << "\n"
            << "Insert (total, avg)" << ", " << "Find (total, avg)" << ", " << "Delete (total, avg)"
            << ", " << "Max tree size" << "\n";
        foutSplay2.close();

        std::fstream foutAVL2; // output file
        foutAVL2.open(fileNameAVL, std::ios::out | std::ios::app);
        foutAVL2 << "Rand seed = " << randSeed << ", " << "N = " << N << ", " << "num of rands = " << numRandCalls << ", "
            << dataType << ", " << "AVL Tree with microsecond time" << "\n"
            << "Insert (total, avg)" << ", " << "Find (total, avg)" << ", " << "Delete (total, avg)"
            << ", " << "Max tree size" << "\n";
        foutAVL2.close();

        return;
    }
    void run_tests(int runsPerTest)
    {
        // input vars -------------
        int randSeed = 5;
        size_t N = 1;
        int numRandCalls = 1000000;
        //-------------------------


        // TEST 1 - Uniform Dist, rand seed = 5, N = 1 ---------------------------- 
        buildFileHeader(randSeed, N, numRandCalls, "Uniform Data", "Splay1.csv", "AVL1.csv"); // builds two files

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, true, false, "Splay1.csv");
            testAVL(randSeed, N, numRandCalls, true, false, "AVL1.csv");
        }
        // ------------------------------------------------------------------


        // TEST 2 - Uniform Dist, rand seed = 5, N = 2 ------------------
       
        N = 2;
    
        buildFileHeader(randSeed, N, numRandCalls, "Uniform Data", "Splay2.csv", "AVL2.csv"); // builds two files

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, true, false, "Splay2.csv");
            testAVL(randSeed, N, numRandCalls, true, false, "AVL2.csv");
        }
        // -------------------------------------------------------------

        // TEST 3 - Uniform Dist, rand seed = 5, N = 3 ------------------

        N = 3;

        buildFileHeader(randSeed, N, numRandCalls, "Uniform Data", "Splay3.csv", "AVL3.csv"); // builds two files

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, true, false, "Splay3.csv");
            testAVL(randSeed, N, numRandCalls, true, false, "AVL3.csv");
        }
        // -------------------------------------------------------------

        // TEST 4 - Uniform Dist, rand seed = 5, N = 4 ------------------

        N = 4;

        buildFileHeader(randSeed, N, numRandCalls, "Uniform Data", "Splay4.csv", "AVL4.csv"); // builds two files

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, true, false, "Splay4.csv");
            testAVL(randSeed, N, numRandCalls, true, false, "AVL4.csv");
        }
        // -------------------------------------------------------------

        // TEST 5 - Uniform Dist, rand seed = 5, N = 5 ------------------

        N = 5;

        buildFileHeader(randSeed, N, numRandCalls, "Uniform Data", "Splay5.csv", "AVL5.csv"); // builds two files

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, true, false, "Splay5.csv");
            testAVL(randSeed, N, numRandCalls, true, false, "AVL5.csv");
        }
        // -------------------------------------------------------------


        // TEST 2 - Uniform Dist, rand seed = 5, N = 10 ------------------

        N = 10;

        buildFileHeader(randSeed, N, numRandCalls, "Uniform Data", "Splay6.csv", "AVL6.csv"); // builds two files

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, true, false, "Splay6.csv");
            testAVL(randSeed, N, numRandCalls, true, false, "AVL6.csv");
        }
        // -------------------------------------------------------------










       

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

   // Results were generated for values of N{ 1, 2, 3, 4, 5, 10 }.

    /*
    std::cout << "SPLAY: \n";
    //   rand seed, N, number of rand calls, output to file?, normal Distribution?, filename
    x.testSplay(5, 1, 1000000, true, false, "Splay.csv");
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "AVL: \n";
    // rand seed, N, number of rand calls, output to file?, normal Distribution?, filename
    x.testAVL(5, 1, 1000000, true, false, "AVL.csv");
    */

    x.run_tests(5);



    
}


