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
private:

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
    void searchTestsAVL(bool is_normal, std::string fileName, double stddev = 2.0)   
    {
        AVL_Tree<int> avl_tree;
 
        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_avl;
        double time_per_batch = 0;
        size_t avgTreeSize = 0;
  
        for (int num_seeds = 0; num_seeds < 10; num_seeds++)
        {
            srand(num_seeds);
            Total_Over_1K_Trees = 0;

            for (int num_trees = 0; num_trees < 10; num_trees++)
            {
                // function call to build tree k
                build_tree(avl_tree, is_normal, num_seeds, stddev);
                avgTreeSize += avl_tree.get_size();
                
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

        avg_time_per_find_avl = Total_Over_1K_Trees / 100.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        avgTreeSize /= 100; // get avg tree size

        std::fstream AVLFind; // output file
        AVLFind.open(fileName, std::ios::out | std::ios::app);

        AVLFind << avg_time_per_find_avl << ", " <<
            avgTreeSize << "\n";

        AVLFind.close();

        return;
    }

    void searchTestsSplay(bool is_normal, std::string fileName, double stddev = 2.0) // uniform data
    {
        SplayTree<int> splay_tree;

        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_splay;
        double time_per_batch = 0;
        size_t avgTreeSize = 0;

        for (int num_seeds = 0; num_seeds < 10; num_seeds++)
        {
            srand(num_seeds);
            Total_Over_1K_Trees = 0;

            for (int num_trees = 0; num_trees < 10; num_trees++)
            {
                // function call to build tree k
                build_tree(splay_tree, is_normal, num_seeds, stddev);
                avgTreeSize += splay_tree.get_size();

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

        avg_time_per_find_splay = Total_Over_1K_Trees / 100.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        avgTreeSize /= 100; // get avg tree size

        std::fstream SplayFind; // output file
        SplayFind.open(fileName, std::ios::out | std::ios::app);

        SplayFind << avg_time_per_find_splay << ", " <<
            avgTreeSize << "\n";

        SplayFind.close();

        return;
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

    void testSplayInsRmv(unsigned int randSeed, double stddev, bool normalDist, std::string fileName)
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
       
        counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                timeForRemove = clock(); // start clock
                splay_tree.remove(rands[counter]); // remove
                timeForRemove = clock() - timeForRemove; // stop clock
                time_per_batch += timeForRemove; // add time
                counter++;
            }

            totalRemoveTime += time_per_batch;
        }

        avgRemove = (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro; // convert to ms
        avgRemove = avgRemove / 1000000; // get avg time per 1 

        size_t sizeAfterRemove = splay_tree.get_size();

        // Write results
   
        std::fstream fout; // output file
        fout.open(fileName, std::ios::out | std::ios::app);

        fout << std::fixed << std::setprecision(1) << (double)totalInsertTime / CLOCKS_PER_SEC * seconds_to_micro << ", "
            << avgInsert << ", " << (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove
            << ", " << sizeAfterInsert << ", " << sizeAfterRemove << "\n";

        fout.close();
    
        return;
    }

    void testAVLInsRmv(unsigned int randSeed, double stddev, bool normalDist, std::string fileName)
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

        counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                timeForRemove = clock(); // start clock
                avl_tree.remove(rands[counter]); // remove
                timeForRemove = clock() - timeForRemove; // stop clock
                time_per_batch += timeForRemove; // add time
                counter++;
            }

            totalRemoveTime += time_per_batch;
        }

        avgRemove = (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro; // convert to ms
        avgRemove = avgRemove / 1000000; // get avg time per remove

        size_t sizeAfterRemove = avl_tree.get_size();

        // Write results

        std::fstream fout; // output file
        fout.open(fileName, std::ios::out | std::ios::app);

        fout << std::fixed << std::setprecision(1) << (double)totalInsertTime / CLOCKS_PER_SEC * seconds_to_micro << ", " 
            << avgInsert << ", " << (double)totalRemoveTime / CLOCKS_PER_SEC * seconds_to_micro << ", " << avgRemove 
            << ", " << sizeAfterInsert << ", " << sizeAfterRemove << "\n";

        fout.close();

        return;
    }

public: // ============================================= Public =============================================

    void runSplayFindTest(bool is_normal, std::string fileName, double stddev = 2.0)
    {
        std::string dataType;
        std::string stdDev;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << stddev;
        stdDev = stream.str();

        if (is_normal == true)
        {
            dataType = "Normal/Gaussian Data";
        }
        else
        {
            dataType = "Uniform Data";
            stdDev = "inapplicable";
        }

        std::fstream foutSplayFind; // output file
        foutSplayFind.open(fileName, std::ios::out | std::ios::app);

        foutSplayFind << "Rand seeds 0-1k " << ", " << "Stddev = " << stdDev << ", " << "1m nums per tree" << ", "
            << dataType << ", " << "Splay Tree with microsecond time" << "\n"
            << "Find avg - Avg Tree size" << "\n";
        foutSplayFind.close();

        searchTestsSplay(is_normal, fileName, stddev);

        return;
    }

    void runAVLFindTest(bool is_normal, std::string fileName, double stddev = 2.0)
    {
        std::string dataType;
        std::string stdDev;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << stddev;
        stdDev = stream.str();

        if (is_normal == true)
        {
            dataType = "Normal/Gaussian Data";
        }
        else
        {
            dataType = "Uniform Data";
            stdDev = "inapplicable";
        }

        std::fstream foutAVLFind; // output file
        foutAVLFind.open(fileName, std::ios::out | std::ios::app);

        foutAVLFind << "Rand seeds 0-1k" << ", " << "Stddev = " << stdDev << ", " << "1m nums per tree" << ", "
            << dataType << ", " << "AVL Tree with microsecond time" << "\n"
            << "Find avg - Avg Tree size" << "\n";
        foutAVLFind.close();

        searchTestsAVL(is_normal, fileName, stddev);

        return;
    }

    void runSplayInsRmvTests(unsigned int randSeed, double stddev, bool is_normal, std::string fileName)
    {
        std::string dataType;
        std::string stdDev;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << stddev;
        stdDev = stream.str();


        if (is_normal == true)
        {
            dataType = "Normal/Gaussian Data";
        }
        else
        {
            dataType = "Uniform Data";
            stdDev = "inapplicable";
        }

        std::fstream foutSplay2; // output file
        foutSplay2.open(fileName, std::ios::out | std::ios::app);

        foutSplay2 << "Rand seed = " << randSeed << ", " << "Stddev = " << stdDev << ", " << "1m random numbers" << ", "
            << dataType << ", " << "Splay Tree with microsecond time" << "\n"
            << "Insert (total, avg)" << ", " << "Delete (total, avg)"
            << ", " << "Max tree size - size after remove" << "\n";
        foutSplay2.close();

        for (int i = 0; i < 10; i++)
        {
            testSplayInsRmv(randSeed, stddev, is_normal, fileName);
        }

        return;
    }

    void runAVLInsRmvTests(unsigned int randSeed, double stddev, bool is_normal, std::string fileName)
    {
        std::string dataType;

        if (is_normal == true)
        {
            dataType = "Normal/Gaussian Data";
        }
        else
        {
            dataType = "Uniform Data";
        }

        std::fstream foutAVL2; // output file
        foutAVL2.open(fileName, std::ios::out | std::ios::app);
        foutAVL2 << "Rand seed = " << randSeed << ", " << "Stddev = " << stddev << ", " << "1m random numbers" << ", "
            << dataType << ", " << "AVL Tree with microsecond time" << "\n"
            << "Insert (total, avg)" << ", " << "Delete (total, avg)"
            << ", " << "Max tree size - size after remove" << "\n";
        foutAVL2.close();

        for (int i = 0; i < 10; i++)
        {
            testAVLInsRmv(randSeed, stddev, is_normal, fileName);
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
 
    /*
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
      
   */

    //x.runSplayInsRmvTests(250, 2.0, false, "SplayTree.csv");

   // x.runAVLInsRmvTests(250, 2.0, false, "AVLTree.csv");

    x.runAVLFindTest(false, "SplayFind.csv", 15.0);

    return 0;
}


