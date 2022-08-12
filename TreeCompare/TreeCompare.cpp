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

    template<typename T> void build_tree(T& theTree, bool is_normal, unsigned int seed, double stddev)
    {
         theTree.clear(); // delete everything in tree

         std::vector<int> rands;

         // fill tree
         if (is_normal == true)
         {
             gaussian(rands, seed, stddev); // grab rands from generator

             for (unsigned int i = 0; i < rands.size(); i++)
             {
                 theTree.insert(rands[i]);
             }
         }
         else
         {
             uniform(rands, seed); // grab rands from generator

             for (unsigned int i = 0; i < rands.size(); i++)
             {
                 theTree.insert(rands[i]);
             }
         }

         return;
    }

    //note: & wanted to get rid of the bool parameter and for us to use a conditional in the function instead
    double searchTestsAVL(size_t N, bool is_normal, double stddev = 2.0)   
    {
        AVL_Tree<int> avl_tree;
 
        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_avl_uniform;
        double time_per_batch = 0;
  
        for (int num_seeds = 0; num_seeds < 10; num_seeds++)
        {
            srand(num_seeds);
            Total_Over_1K_Trees = 0;

            for (int num_trees = 0; num_trees < 10; num_trees++)
            {
                // function call to build tree k
                build_tree(avl_tree, is_normal, num_seeds, stddev);
                
                time_per_batch = 0;

                for (int num_find_calls = 0; num_find_calls < 10; num_find_calls++) //change to 1000
                {
                    auto start = high_resolution_clock::now();
                    avl_tree.contains(rand() % 500000);     
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> single_time = stop - start;
                    time_per_batch += single_time.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 10; // avg single find time for tree k (total time / num find operations)
                Total_Over_1K_Trees += time_per_batch; // adds avg find time of items in tree k 
            }
        }

        avg_time_per_find_avl_uniform = Total_Over_1K_Trees / 100.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        return avg_time_per_find_avl_uniform;
    }

    double searchTestsSplay(size_t N, bool is_normal, double stddev = 2.0) // uniform data
    {
        SplayTree<int> splay_tree;

        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_avl_uniform;
        double time_per_batch = 0;

        for (int num_seeds = 0; num_seeds < 10; num_seeds++)
        {
            srand(num_seeds);
            Total_Over_1K_Trees = 0;

            for (int num_trees = 0; num_trees < 10; num_trees++)
            {
                // function call to build tree k
                build_tree(splay_tree, is_normal, num_seeds, stddev);

                time_per_batch = 0;

                for (int num_find_calls = 0; num_find_calls < 10; num_find_calls++) //change to 1000
                {
                    auto start = high_resolution_clock::now();
                    splay_tree.contains(rand() % 500000);
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> single_time = stop - start;
                    time_per_batch += single_time.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 10; // avg single find time for tree k (total time / num find operations)
                Total_Over_1K_Trees += time_per_batch; // adds avg find time of items in tree k 
            }
        }

        avg_time_per_find_avl_uniform = Total_Over_1K_Trees / 100.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

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

    void uniform(std::vector<int>& randNums, unsigned int randSeed) 
    {
        // Mersenne Twister random engine
        std::mt19937 randEngine{ randSeed };

        std::uniform_int_distribution<int> generator{ 0, 500000 }; // range 0 to 500k

        randNums.clear();

        for (int i = 0; i < 1000000; i++) // 1m rands
        {
            randNums.push_back(generator(randEngine));
        }

        return;
    }

    void gaussian(std::vector<int>& randNums, unsigned int randSeed, double stddev) 
    {
        // Mersenne Twister random engine
        std::mt19937 randEngine{ randSeed };

        std::normal_distribution<double> generator{ 250000.0, stddev }; // (mean, stddev) Note: can't set explicit range, just mean

        randNums.clear();

        for (int i = 0; i < 1000000; i++) // 1m rands
        {
            randNums.push_back(double(generator(randEngine)));
        }

        return;
    }

    void testSplay(unsigned int randSeed, double stddev, int num_rands, bool normalDist, std::string fileName)
    {
        int seconds_to_micro = 1000000; // convert to microseconds
        clock_t timeForRemove;
        clock_t timeForInsert;
        clock_t time_per_batch;

        SplayTree<int> splay_tree;

        std::vector<int> rands;

        // get appropriate data distribution

        if (normalDist == true)
        {
            gaussian(rands, randSeed, stddev);
        }
        else
        {
            uniform(rands, randSeed);
        }

        // ------------------------------- SPLAY INSERT -----------------------------------------------
        // insert data - noting clock every 1000 elements
        clock_t totalInsertTime = 0;
        double avgInsert;
        int counter = 0;
     
        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                timeForInsert = clock(); // start clock
                splay_tree.insert(rands[counter]); // insert
                timeForInsert = clock() - timeForInsert; // stop clock
                time_per_batch += timeForInsert; // add time
                counter++;
            }

            totalInsertTime += time_per_batch;
        }

        avgInsert = (double)totalInsertTime / CLOCKS_PER_SEC * seconds_to_micro; // convert to ms
        avgInsert = avgInsert / 1000000; // get avg time per insert

        size_t sizeAfterInsert = splay_tree.get_size();

        // ------------------------------- SPLAY REMOVE ---------------------------------------------------
        clock_t totalRemoveTime = 0;
        double avgRemove;

        // remove data - noting clock every 1000 elements
        // loop 1 - 500000
       
        counter = 0; // num to remove

        for (int i = 0; i < 500; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                timeForRemove = clock(); // start clock
                splay_tree.remove(counter); // remove
                timeForRemove = clock() - timeForRemove; // stop clock
                time_per_batch += timeForRemove; // add time
                counter++;
            }

            totalRemoveTime += time_per_batch;
        }

        avgRemove = (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro; // convert to ms
        avgRemove = avgRemove / 500; // get avg time per insert


        // Write results
   
        std::fstream fout; // output file
        fout.open(fileName, std::ios::out | std::ios::app);

        fout << std::fixed << std::setprecision(1) << (double)totalInsertTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgInsert << ", " <<
            (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove << ", " << sizeAfterInsert <<
            "\n";
       

        return;
    }

    void testAVL(unsigned int randSeed, double stddev, int num_rands, bool normalDist, std::string fileName)
    {
        int seconds_to_micro = 1000000; // convert to microseconds
        clock_t timeForRemove;
        clock_t timeForInsert;
        clock_t time_per_batch;

        AVL_Tree<int> avl_tree;

        std::vector<int> rands;

        // get appropriate data distribution

        if (normalDist == true)
        {
            gaussian(rands, randSeed, stddev);
        }
        else
        {
            uniform(rands, randSeed);
        }

        // ------------------------------- SPLAY INSERT -----------------------------------------------
        // insert data - noting clock every 1000 elements
        clock_t totalInsertTime = 0;
        double avgInsert;
        int counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                timeForInsert = clock(); // start clock
                avl_tree.insert(rands[counter]); // insert
                timeForInsert = clock() - timeForInsert; // stop clock
                time_per_batch += timeForInsert; // add time
                counter++;
            }

            totalInsertTime += time_per_batch;
        }

        avgInsert = (double)totalInsertTime / CLOCKS_PER_SEC * seconds_to_micro; // convert to ms
        avgInsert = avgInsert / 1000000; // get avg time per insert

        size_t sizeAfterInsert = avl_tree.get_size();

        // ------------------------------- SPLAY REMOVE ---------------------------------------------------
        clock_t totalRemoveTime = 0;
        double avgRemove;

        // remove data - noting clock every 1000 elements
        // loop 1 - 500000

        counter = 0; // num to remove

        for (int i = 0; i < 500; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                timeForRemove = clock(); // start clock
                avl_tree.remove(counter); // remove
                timeForRemove = clock() - timeForRemove; // stop clock
                time_per_batch += timeForRemove; // add time
                counter++;
            }

            totalRemoveTime += time_per_batch;
        }

        avgRemove = (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro; // convert to ms
        avgRemove = avgRemove / 500; // get avg time per insert


        // Write results

        std::fstream fout; // output file
        fout.open(fileName, std::ios::out | std::ios::app);

        fout << std::fixed << std::setprecision(1) << (double)totalInsertTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgInsert << ", " <<
            (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove << ", " << sizeAfterInsert <<
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


