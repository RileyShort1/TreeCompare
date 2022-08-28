// 
//
#include "SplayTree.h"
#include "AVL_Tree.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std::chrono;



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

// class for our final benchmarking
class Benchmark {
private:

    // fill tree with numbers 1 - tree size (shuffled order)
    template<typename T> void build_tree(T& theTree, unsigned int seed, int treeSize)
    {
         theTree.clear(); // delete everything in tree

         std::vector<int> rands;

         for (int i = 1; i < treeSize + 1; i++)
         {
             rands.push_back(i);
         }

         auto rng = std::default_random_engine(seed);
         std::shuffle(std::begin(rands), std::end(rands), rng);

         for (size_t j = 0; j < rands.size(); j++)
         {
             theTree.insert(rands[j]);
         }
          
         return;
    }

    //note: & wanted to get rid of the bool parameter and for us to use a conditional in the function instead
    void searchTestsAVL(std::string fileName, double stddev, int treeSize)   
    {
        AVL_Tree<int> avl_tree;
 
        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_avl;
        double time_per_batch = 0;
        size_t avgTreeSize = 0;
        int programTime = 0;
        std::vector<int> finds;
  
        for (int num_seeds = 0; num_seeds < 100; num_seeds++)
        {
            programTime++;
            std::cout << programTime << " / 100\n";

            for (int num_trees = 0; num_trees < 10; num_trees++) // this is essentially how many single tests we want per single seed
            {
                build_tree(avl_tree, num_seeds, treeSize);
                gaussian(finds, num_seeds, stddev, 5000, avl_tree.get_size()); // get nums to find
                avgTreeSize += avl_tree.get_size();
                
                time_per_batch = 0;

                for (int num_find_calls = 0; num_find_calls < 5000; num_find_calls++) 
                {
                    auto start = high_resolution_clock::now();
                    avl_tree.contains(finds[num_find_calls]);     
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> single_time = stop - start;
                    time_per_batch += single_time.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 5000; // avg single find time for tree k (total time / num find operations)
                Total_Over_1K_Trees += time_per_batch; // adds avg find time of items in tree k 
            }
        }

        avg_time_per_find_avl = Total_Over_1K_Trees / 1000.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        avgTreeSize /= 1000; // get avg tree size

        std::fstream AVLFind; // output file
        AVLFind.open(fileName, std::ios::out | std::ios::app);

        AVLFind << std::fixed << std::setprecision(4) << avg_time_per_find_avl << ", " <<
            avgTreeSize <<  "\n";

        AVLFind.close();

        return;
    }

    void searchTestsSplay(std::string fileName, double stddev, int treeSize) 
    {
        SplayTree<int> splay_tree;

        double Total_Over_1K_Trees = 0;
        double avg_time_per_find_splay;
        double time_per_batch = 0;
        size_t avgTreeSize = 0;
        int programTime = 0;
        std::vector<int> finds;

        for (int num_seeds = 0; num_seeds < 100; num_seeds++)
        {
            programTime++;
            std::cout << programTime << " / 100\n";

            for (int num_trees = 0; num_trees < 10; num_trees++)
            {
                build_tree(splay_tree, num_seeds, treeSize);
                gaussian(finds, num_seeds, stddev, 5000, splay_tree.get_size()); // get nums to find
                avgTreeSize += splay_tree.get_size();

                time_per_batch = 0;

                for (int num_find_calls = 0; num_find_calls < 5000; num_find_calls++) 
                {
                    auto start = high_resolution_clock::now();
                    std::cout << splay_tree.contains(finds[num_find_calls]) << std::endl;;
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> single_time = stop - start;
                    time_per_batch += single_time.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 5000; // avg single find time for tree k (total time / num find operations)
                Total_Over_1K_Trees += time_per_batch; // adds avg find time of items in tree k 
            }
        }

        avg_time_per_find_splay = Total_Over_1K_Trees / 1000.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        avgTreeSize /= 1000; // get avg tree size

        std::fstream SplayFind; // output file
        SplayFind.open(fileName, std::ios::out | std::ios::app);

        SplayFind << std::fixed << std::setprecision(4) << avg_time_per_find_splay << ", " <<
            avgTreeSize << "\n";

        SplayFind.close();

        return;
    }

    //
    // https://www.gigacalculator.com/calculators/normality-test-calculator.php
    //

    void uniform(std::vector<int>& randNums, unsigned int randSeed, int numRands) 
    {
        // Mersenne Twister random engine
        std::mt19937 randEngine( randSeed );

        std::uniform_int_distribution<int> generator( 0, 1000000 ); // range 0 to 500k

        randNums.clear();

        for (int i = 0; i < numRands; i++)
        {
            randNums.push_back(generator(randEngine));
        }

        return;
    }

    void gaussian(std::vector<int>& randNums, unsigned int randSeed, double stddev, int numRands, int treeSize) 
    {
        // Mersenne Twister random engine
        std::mt19937 randEngine( randSeed );
        double mean = treeSize / 2.0;

        std::normal_distribution<double> generator( mean, stddev );

        randNums.clear();

        for (int i = 0; i < numRands; i++)
        {
            randNums.push_back(double(generator(randEngine)));
        }

        return;
    }
    /*
    std::vector<double> testSplayInsRmv(unsigned int randSeed, double stddev, bool normalDist, std::string fileName)
    {
        double time_per_batch;
        SplayTree<int> splay_tree;

        std::vector<int> rands;

        // get appropriate data distribution

        if (normalDist == true)
        {
            gaussian(rands, randSeed, stddev, 1000000);
        }
        else
        {
            uniform(rands, randSeed, 1000000);
        }

        // ------------------------------- SPLAY INSERT -----------------------------------------------
        // insert data - noting clock every 1000 elements
        double totalInsertTime = 0;
        double avgInsert;
        int counter = 0;
     
        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                auto startInsert = high_resolution_clock::now();
                splay_tree.insert(rands[counter]);
                auto stopInsert = high_resolution_clock::now();
                duration<double, std::micro> single_insert = stopInsert - startInsert;

                time_per_batch += single_insert.count(); // add time
                counter++;
            }

            totalInsertTime += time_per_batch;
        }

        avgInsert = totalInsertTime / 1000000; // avg time per insert
        size_t sizeAfterInsert = splay_tree.get_size();

        // ------------------------------- SPLAY REMOVE ---------------------------------------------------
        double totalRemoveTime = 0;
        double avgRemove;

        counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                auto startRemove = high_resolution_clock::now();
                splay_tree.remove(rands[counter]); // remove
                auto stopRemove = high_resolution_clock::now();
                duration<double, std::micro> single_remove = stopRemove - startRemove;

                time_per_batch += single_remove.count(); // add time
                counter++;
            }

            totalRemoveTime += time_per_batch;
        }

        avgRemove = totalRemoveTime / 1000000; // avg time per insert
        size_t sizeAfterRemove = splay_tree.get_size();

        // Write results
   
        std::fstream fout; // output file
        fout.open(fileName, std::ios::out | std::ios::app);

        fout << std::fixed << std::setprecision(4) << totalInsertTime << ", "
            << avgInsert << ", " << totalRemoveTime << ", " << avgRemove
            << ", " << sizeAfterInsert << ", " << sizeAfterRemove << "\n";

        fout.close();
    
        std::vector<double> avgReturns = { avgInsert, avgRemove };

        return avgReturns;
    }

    std::vector<double> testAVLInsRmv(unsigned int randSeed, double stddev, bool normalDist, std::string fileName)
    {
        double time_per_batch;
        AVL_Tree<int> avl_tree;

        std::vector<int> rands;

        // get appropriate data distribution

        if (normalDist == true)
        {
            gaussian(rands, randSeed, stddev, 1000000);
        }
        else
        {
            uniform(rands, randSeed, 1000000);
        }

        // ------------------------------- AVL INSERT -----------------------------------------------
        // insert data - noting clock every 1000 elements
        double totalInsertTime = 0;
        double avgInsert;
        int counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                auto startInsert = high_resolution_clock::now();
                avl_tree.insert(rands[counter]);
                auto stopInsert = high_resolution_clock::now();
                duration<double, std::micro> single_insert = stopInsert - startInsert;

                time_per_batch += single_insert.count(); // add time
                counter++;
            }

            totalInsertTime += time_per_batch;
        }

        avgInsert = totalInsertTime / 1000000; // avg time per insert
        size_t sizeAfterInsert = avl_tree.get_size();

        // ------------------------------- AVL REMOVE ---------------------------------------------------
        double totalRemoveTime = 0;
        double avgRemove;

        counter = 0;

        for (int i = 0; i < 1000; i++)
        {
            time_per_batch = 0;

            for (int j = 0; j < 1000; j++)
            {
                auto startRemove = high_resolution_clock::now();
                avl_tree.remove(rands[counter]); // remove
                auto stopRemove = high_resolution_clock::now();
                duration<double, std::micro> single_remove = stopRemove - startRemove;

                time_per_batch += single_remove.count(); // add time
                counter++;
            }

            totalRemoveTime += time_per_batch;
        }

        avgRemove = totalRemoveTime / 1000000; // avg time per insert
        size_t sizeAfterRemove = avl_tree.get_size();

        // Write results

        std::fstream fout; // output file
        fout.open(fileName, std::ios::out | std::ios::app);

        fout << std::fixed << std::setprecision(4) << totalInsertTime << ", " 
            << avgInsert << ", " << totalRemoveTime << ", " << avgRemove 
            << ", " << sizeAfterInsert << ", " << sizeAfterRemove << "\n";

        fout.close();

        std::vector<double> avgReturns = { avgInsert, avgRemove };

        return avgReturns;
    }
    */
public: // ============================================= Public =============================================

    void runSplayFindTest(std::string fileName, double stddev, int treeSize)
    {
        std::fstream foutSplayFind; // output file
        foutSplayFind.open(fileName, std::ios::out | std::ios::app);

        foutSplayFind << "Rand seeds 0-100" << ", " << "Stddev = " << stddev << ", "
            << "Finding 1k normal" << ", " << "Splay Tree with microsecond time containing nums 0 - " << treeSize << "\n"
            << "mean of searched data is half tree size\n" 
            << "Find avg - Tree size" << "\n";
        foutSplayFind.close();

        searchTestsSplay(fileName, stddev, treeSize);

        return;
    }

    void runAVLFindTest(std::string fileName, double stddev, int treeSize)
    {
        std::fstream foutAVLFind; // output file
        foutAVLFind.open(fileName, std::ios::out | std::ios::app);

        foutAVLFind << "Rand seeds 0-100" << ", " << "Stddev = " << stddev << ", "
            << "Finding 1k normal" << ", " << "AVL Tree with microsecond time containing nums 0 - " << treeSize << "\n"
            << "mean of searched data is half tree size\n"
            << "Find avg - Tree size" << "\n";
        foutAVLFind.close();

        searchTestsAVL(fileName, stddev, treeSize);

        return;
    }

    /*
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

        std::vector<double> avgTimes;
        double avgInsert = 0;
        double avgRemove = 0;

        for (int i = 0; i < 10; i++)
        {
           avgTimes = testSplayInsRmv(randSeed, stddev, is_normal, fileName);
           avgInsert += avgTimes[0]; 
           avgRemove += avgTimes[1];

           avgTimes.clear();
        }

        // get avg
        avgInsert /= 10;
        avgRemove /= 10;

        // write avg's
        foutSplay2.open(fileName, std::ios::out | std::ios::app);
        foutSplay2 << "Avg Insert = " << avgInsert << ", " << "Avg Remove = " << avgRemove << "\n";
        foutSplay2.close();

        return;
    }

    void runAVLInsRmvTests(unsigned int randSeed, double stddev, bool is_normal, std::string fileName)
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

        std::fstream foutAVL2; // output file
        foutAVL2.open(fileName, std::ios::out | std::ios::app);
        foutAVL2 << "Rand seed = " << randSeed << ", " << "Stddev = " << stdDev << ", " << "1m random numbers" << ", "
            << dataType << ", " << "AVL Tree with microsecond time" << "\n"
            << "Insert (total, avg)" << ", " << "Delete (total, avg)"
            << ", " << "Max tree size - size after remove" << "\n";
        foutAVL2.close();

        std::vector<double> avgTimes;
        double avgInsert = 0;
        double avgRemove = 0;

        for (int i = 0; i < 10; i++)
        {
            avgTimes = testAVLInsRmv(randSeed, stddev, is_normal, fileName);
            avgInsert += avgTimes[0];
            avgRemove += avgTimes[1];

            avgTimes.clear();
        }

        // get avg
        avgInsert /= 10;
        avgRemove /= 10;

        // write avg's
        foutAVL2.open(fileName, std::ios::out | std::ios::app);
        foutAVL2 << "Avg Insert = " << avgInsert << ", " << "Avg Remove = " << avgRemove << "\n";
        foutAVL2.close();

        return;
    }
    */

};

int main()
{
    //SplayTests rs;
    //rs.generalTests();

    //AVLTests av;
    //av.generalTests();
   
    Benchmark x;

    // Normal Data
    /*
    x.runSplayFindTest("SplayFindNormal.csv", 2.0, 1000000);
    x.runAVLFindTest("AVLFindNormal.csv", 2.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-1.csv", 2.0, 2000000);
    x.runAVLFindTest("AVLFindNormal-1.csv", 2.0, 2000000);

    x.runSplayFindTest("SplayFindNormal-2.csv", 2.0, 3000000);
    x.runAVLFindTest("AVLFindNormal-2.csv", 2.0, 3000000);

    x.runSplayFindTest("SplayFindNormal-3.csv", 2.0, 4000000);
    x.runAVLFindTest("AVLFindNormal-3.csv", 2.0, 4000000);
    */

    return 0;
}


