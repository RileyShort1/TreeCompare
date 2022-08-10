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
#include <chrono>
#include <random>
using namespace std::chrono;


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

    template<typename T> void build_tree(T& theTree, bool normalData, size_t N) // builds tree of size 100k
    {
         theTree.clear(); // delete everything in tree

         // fill tree
         if (normalData == true)
         {
             for (int i = 0; i < 100000; i++)
             {
                 theTree.insert(rand_gaussian(N));
             }
         }
         else
         {
             for (int i = 0; i < 100000; i++)
             {
                 theTree.insert(rand_uniform(N));
             }
         }

         return;
    }

    double searchTestsAVL(size_t N, bool is_normal) // uniform data
        //note: & wanted to get rid of the bool parameter and for us to use a conditional in the function instead
    
    {
        AVL_Tree<int> avl_tree;
 
        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_avl_uniform;
        double time_per_batch = 0;
  
        for (int i = 0; i < 10; i++) // rand seeds
        {
            srand(i);
            
            Total_Over_1K_Trees = 0;

            for (int num_trees = 0; num_trees < 10; num_trees++)
            {
                // function call to build tree k
                build_tree(avl_tree, is_normal, N);
                
                time_per_batch = 0;

                for (int j = 1; j < 11; j++) // 1k rand find targets //change to 1000
                {
                    auto start = high_resolution_clock::now();
                    avl_tree.contains(rand() % 100000);     
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> ms_double = stop - start;
                    time_per_batch += ms_double.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 10; // avg single find time for tree k (total time / num find operations)
                Total_Over_1K_Trees += time_per_batch; // adds avg find time of items in tree k 
                //time_per_batch = 0;
            }
        }

        avg_time_per_find_avl_uniform = Total_Over_1K_Trees / 100.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        return avg_time_per_find_avl_uniform;
    }

    double searchTestsSplay(size_t N, bool normalData) // uniform data
    {
        SplayTree<int> testTree;

        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_avl_uniform;
        double per1k = 0;

        for (int i = 1; i < 11; i++) // rand seeds
        {
            srand(i);

            for (int k = 1; k < 11; k++) // 1 - 1000 diff 100k node AVL Trees
            {
                // function call to build tree
                build_tree(testTree, normalData, N);

                for (int j = 1; j < 11; j++) // 1k rand find targets
                {
                    auto start = high_resolution_clock::now();
                    testTree.contains(rand() % 100000);
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> ms_double = stop - start;
                    per1k += ms_double.count();
                }

                per1k /= 10;
                Total_Over_1K_Trees += per1k;
                per1k = 0;
            }
        }

        avg_time_per_find_avl_uniform = Total_Over_1K_Trees / 100.0; // divide by # of trees

        return avg_time_per_find_avl_uniform;     
    }

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

    //
    // https://www.gigacalculator.com/calculators/normality-test-calculator.php
    //

    void uniform(std::vector<int>& randNums, unsigned int randSeed) // return vector or single rand??
    {
        // Mersenne Twister random engine
        std::mt19937 urbg{randSeed};

        std::uniform_int_distribution<int> rands{1, 100000}; 

        randNums.clear();

        for (int i = 0; i < 100000; i++) 
        {
            randNums.push_back(rands(urbg));
        }

        return;
    }

    void gaussian(std::vector<int>& randNums, unsigned int randSeed) // return vector or single rand??
    {
        // Mersenne Twister random engine
        std::mt19937 urbg{randSeed};

        // https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
        // https://mathworld.wolfram.com/BinomialDistribution.html
        std::binomial_distribution<int> rands{100000, 0.5}; // stl normal dist does not return an int 

        randNums.clear();

        for (int i = 0; i < 100000; i++)
        {
            randNums.push_back(rands(urbg));
        }

        return;
    }

    void testSplay(int randSeed, size_t N, int num_rands, bool normalDist, std::string fileName)
    {
        srand(randSeed); // seed rand
        int seconds_to_micro = 1000000; // convert to microseconds

        clock_t timeForInsert;
        clock_t timeForRemove;
        clock_t timePer1k = 0;

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
        int counter = 0;
     
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                timeForInsert = clock(); // start clock
                testTree.insert(rands[counter]); // insert
                timeForInsert = clock() - timeForInsert; // stop clock
                timePer1k += timeForInsert; // add time
                counter++;
            }

            timesPer1kInsert.push_back(timePer1k); 
            timePer1k = 0;
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

        timePer1k = 0;
        counter = 0;
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                timeForFind = clock(); // start clock
                testTree.contains(rands[counter]); // find
                timeForFind = clock() - timeForFind; // stop clock
                timePer1k += timeForFind; // add time
                counter++;
            }

            timesPer1kFind.push_back(timePer1k);
            timePer1k = 0;
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

        // remove data - noting clock every 1000 elements
        // loop 1 - 100000
       
        timePer1k = 0;
        counter = 0;

        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                timeForRemove = clock(); // start clock
                testTree.contains(rands[counter]); // find
                timeForRemove = clock() - timeForRemove; // stop clock
                timePer1k += timeForRemove; // add time
                counter++;
            }

            timesPer1kRemove.push_back(timePer1k);
            timePer1k = 0;
        }

        clock_t totalTimeRemove = 0;
        double avgRemove;

        for (size_t i = 0; i < timesPer1kRemove.size(); i++)
        {
            totalTimeRemove += timesPer1kRemove[i];
        }

        avgRemove = (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro;
        avgRemove = avgRemove / timesPer1kRemove.size();

   
            std::fstream fout; // output file
            fout.open(fileName, std::ios::out | std::ios::app);

            fout << std::fixed << std::setprecision(1) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avg << ", " <<
                (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgFind << ", " <<
                (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove << ", " << sizeAfterInsert <<
                "\n";
       
        return;

    }

    void testAVL(int randSeed, size_t N, int num_rands, bool normalDist, std::string fileName)
    {
        srand(randSeed); // seed rand
        int seconds_to_micro = 1000000; // convert to microseconds

        AVL_Tree<int> testTree;
      
        clock_t timeForInsert;
        clock_t timeForRemove;
        clock_t timePer1k = 0;

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
        int counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                timeForInsert = clock(); // start clock
                testTree.insert(rands[counter]); // insert
                timeForInsert = clock() - timeForInsert; // stop clock
                timePer1k += timeForInsert; // add time
                counter++;
            }

            timesPer1kInsert.push_back(timePer1k);
            timePer1k = 0;
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

        timePer1k = 0;
        counter = 0;
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                timeForFind = clock(); // start clock
                testTree.contains(rands[counter]); // find
                timeForFind = clock() - timeForFind; // stop clock
                timePer1k += timeForFind; // add time
                counter++;
            }

            timesPer1kFind.push_back(timePer1k);
            timePer1k = 0;
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
     
        // remove data - noting clock every 1000 elements
        // loop 1 - 100000
        timePer1k = 0;
        counter = 0;

        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                timeForRemove = clock(); // start clock
                testTree.contains(rands[counter]); // find
                timeForRemove = clock() - timeForRemove; // stop clock
                timePer1k += timeForRemove; // add time
                counter++;
            }

            timesPer1kRemove.push_back(timePer1k);
            timePer1k = 0;
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
       
            std::fstream fout; // output file
            fout.open(fileName, std::ios::out | std::ios::app);

            fout << std::fixed << std::setprecision(1) << (double)totalTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avg << ", " <<
                (double)totalTimeFind / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgFind << ", " <<
                (double)totalTimeRemove / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove << ", " << sizeAfterInsert <<
                "\n";
      
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
    void runTest_x(int randSeed, size_t N, int numRandCalls, std::string dataType, std::string fileNameSplay, std::string fileNameAVL, int runsPerTest)
    {
        buildFileHeader(randSeed, N, numRandCalls, dataType, fileNameSplay, fileNameAVL); // builds two files

        bool normalDist = false;

        if (dataType == "Normal Data")
        {
            normalDist = true;
        }

        for (int i = 0; i < runsPerTest; i++) // run tests
        {
            testSplay(randSeed, N, numRandCalls, normalDist, fileNameSplay);
            testAVL(randSeed, N, numRandCalls, normalDist, fileNameAVL);
        }

        return;
    }
    void run_tests(int runsPerTest, int randSeed)
    {
        int numRandCalls = 1000000;

        // Tests 1-6 with Uniform Distribution
        runTest_x(randSeed, 1, numRandCalls, "Uniform Data", "SplayTest1.csv", "AVLTest1.csv", runsPerTest);
        runTest_x(randSeed, 2, numRandCalls, "Uniform Data", "SplayTest2.csv", "AVLTest2.csv", runsPerTest);
        runTest_x(randSeed, 3, numRandCalls, "Uniform Data", "SplayTest3.csv", "AVLTest3.csv", runsPerTest);
        runTest_x(randSeed, 4, numRandCalls, "Uniform Data", "SplayTest4.csv", "AVLTest4.csv", runsPerTest);
        runTest_x(randSeed, 5, numRandCalls, "Uniform Data", "SplayTest5.csv", "AVLTest5.csv", runsPerTest);
        runTest_x(randSeed, 10, numRandCalls, "Uniform Data", "SplayTest6.csv", "AVLTest6.csv", runsPerTest);

        // Tests 7 - 12 with Normal Data
        runTest_x(randSeed, 1, numRandCalls, "Normal Data", "SplayTest7.csv", "AVLTest7.csv", runsPerTest);
        runTest_x(randSeed, 2, numRandCalls, "Normal Data", "SplayTest8.csv", "AVLTest8.csv", runsPerTest);
        runTest_x(randSeed, 3, numRandCalls, "Normal Data", "SplayTest9.csv", "AVLTest9.csv", runsPerTest);
        runTest_x(randSeed, 4, numRandCalls, "Normal Data", "SplayTest10.csv", "AVLTest10.csv", runsPerTest);
        runTest_x(randSeed, 5, numRandCalls, "Normal Data", "SplayTest11.csv", "AVLTest11.csv", runsPerTest);
        runTest_x(randSeed, 10, numRandCalls, "Normal Data", "SplayTest12.csv", "AVLTest12.csv", runsPerTest);


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
      // num runs / rand Seed
      //x.run_tests(5, 5);
   
    
   // for (size_t i = 1; i < 20; i++)
    //{
       // x.repeatLookups(15, i, true, 1000);
   // }
   
    x.searchTestsAVL(1, false);
    x.searchTestsSplay(1, false);


    std::fstream fout; // output file
    fout.open("FindTests.csv", std::ios::out | std::ios::app);

    fout << "Splay vs. AVL Find Tests: N = 1 - Uniform Data - Time in microseconds\n" << "AVL" << ", " << "Splay\n";

    double avlTotal = 0;
    double splayTotal = 0;
    double avlData;
    double splayData;

    for (int i = 0; i < 10; i++)
    {
        avlData = x.searchTestsAVL(1, false);
        splayData = x.searchTestsSplay(1, false);
        avlTotal += avlData;
        splayTotal += splayData;

        fout << avlData << ", " << splayData << "\n";
    }

    fout << "AVL avg = " << avlTotal / 10 << ", " << "Splay avg = " << splayTotal / 10 << "\n";

    if (avlTotal / 10 < splayTotal / 10) // AVL wins
    {
        fout << "AVL beats Splay by " << (splayTotal / 10) - (avlTotal / 10) << " microseconds\n";
    }
    else // splay wins
    {
        fout << "Splay beats AVL by " << (avlTotal / 10) - (splayTotal / 10) << " microseconds\n";
    }

    fout.close();
      
   


    return 0;
}


