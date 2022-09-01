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
    template<typename T> void build_tree(T& theTree, unsigned int seed, double stddev)
    {
        theTree.clear(); // delete everything in tree

        std::vector<int> rands;

        get_array_gaussian(rands, seed, stddev, 1000000); // grab rands from generator

        for (unsigned int i = 0; i < rands.size(); i++)
        {
            theTree.insert(rands[i]);
        }
       
        return;
    }

    //note: & wanted to get rid of the bool parameter and for us to use a conditional in the function instead
    void searchTestsAVL(std::string fileName, double stddev)
    {
        AVL_Tree<int> avl_tree;
 
        double Total_Over_All_Trees = 0;
        double avg_time_per_find_avl;
        double time_per_batch = 0;
        size_t avgTreeSize = 0;
        int programTime = 0;
        std::vector<int> finds;
  
        for (int num_seeds = 0; num_seeds < 25; num_seeds++)
        {
            programTime++;
            std::cout << programTime << " / 25\n";

            for (int num_trees = 0; num_trees < 50; num_trees++) // this is essentially how many single tests we want per single seed
            {
                build_tree(avl_tree, num_seeds, stddev);
                get_array_gaussian(finds, num_seeds, stddev, 250000); // get nums to find
                avgTreeSize += avl_tree.get_size();
                
                time_per_batch = 0;

                for (int num_find_calls = 0; num_find_calls < 250000; num_find_calls++)
                {
                    auto start = high_resolution_clock::now();
                    avl_tree.contains(finds[num_find_calls]);     
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> single_time = stop - start;
                    time_per_batch += single_time.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 250000; // avg single find time for tree k (total time / num find operations)
                Total_Over_All_Trees += time_per_batch; // adds avg find time of items in tree k 
            }
        }

        avg_time_per_find_avl = Total_Over_All_Trees / 1250.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        avgTreeSize /= 1250; // get avg tree size

        std::fstream AVLFind; // output file
        AVLFind.open(fileName, std::ios::out | std::ios::app);

        AVLFind << std::fixed << std::setprecision(4) << avg_time_per_find_avl << ", " <<
            avgTreeSize <<  "\n";

        AVLFind.close();

        return;
    }

    void searchTestsSplay(std::string fileName, double stddev)
    {
        SplayTree<int> splay_tree;

        double Total_Over_All_Trees = 0;
        double avg_time_per_find_splay;
        double time_per_batch = 0;
        size_t avgTreeSize = 0;
        int programTime = 0;
        std::vector<int> finds;

        for (int num_seeds = 0; num_seeds < 25; num_seeds++)
        {
            programTime++;
            std::cout << programTime << " / 25\n";

            for (int num_trees = 0; num_trees < 50; num_trees++)
            {
                build_tree(splay_tree, num_seeds, stddev);
                get_array_gaussian(finds, num_seeds, stddev, 250000); // get nums to find
                avgTreeSize += splay_tree.get_size();

                time_per_batch = 0;

                for (int num_find_calls = 0; num_find_calls < 250000; num_find_calls++)
                {
                    auto start = high_resolution_clock::now();
                    splay_tree.contains(finds[num_find_calls]);
                    auto stop = high_resolution_clock::now();
                    duration<double, std::micro> single_time = stop - start;
                    time_per_batch += single_time.count(); // add single find time to pool of times for tree k
                }

                time_per_batch /= 250000; // avg single find time for tree k (total time / num find operations)
                Total_Over_All_Trees += time_per_batch; // adds avg find time of items in tree k 
            }
        }

        avg_time_per_find_splay = Total_Over_All_Trees / 1250.0; // divide by # of trees tested to get avg time across all trees
        // should be yielding avg time to find a single item across all trees tested 

        avgTreeSize /= 1250; // get avg tree size

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

    /*
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
    */

    void get_array_gaussian(std::vector<int>& randNums, unsigned int randSeed, double stddev, int numRands)
    {
        // Mersenne Twister random engine
        std::mt19937 randEngine(randSeed);

        std::normal_distribution<double> generator(250000.0, stddev); // (mean, stddev) Note: can't set explicit range, just mean

        randNums.clear();

        for (int i = 0; i < numRands; i++)
        {
            randNums.push_back(double(generator(randEngine)));
        }

        return;
    }

public: // ============================================= Public =============================================

    void runSplayFindTest(std::string fileName, double stddev)
    {
        std::fstream foutSplayFind; // output file
        foutSplayFind.open(fileName, std::ios::out | std::ios::app);

        foutSplayFind << "Rand seeds 0-25" << ", " << "Stddev = " << stddev << ", "
            << "Finding 250k normal" << ", " << "Splay Tree with microsecond time"  << "\n"
            << "mean of searched data is 50k\n" 
            << "Find avg - Tree size" << "\n";
        foutSplayFind.close();

        searchTestsSplay(fileName, stddev);

        return;
    }

    void runAVLFindTest(std::string fileName, double stddev)
    {
        std::fstream foutAVLFind; // output file
        foutAVLFind.open(fileName, std::ios::out | std::ios::app);

        foutAVLFind << "Rand seeds 0-25" << ", " << "Stddev = " << stddev << ", "
            << "Finding 250k normal" << ", " << "AVL Tree with microsecond time" << "\n"
            << "mean of searched data is 50k\n"
            << "Find avg - Tree size" << "\n";
        foutAVLFind.close();

        searchTestsAVL(fileName, stddev);

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
    x.runSplayFindTest("SplayFindNormal-1.csv", 1.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-1.csv", 1.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-2.csv", 2.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-2.csv", 2.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-5.csv", 5.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-5.csv", 5.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-10.csv", 10.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-10.csv", 10.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-15.csv", 15.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-15.csv", 15.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-20.csv", 20.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-20.csv", 20.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-25.csv", 25.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-25.csv", 25.0, 1000000);


    x.runSplayFindTest("SplayFindNormal-30.csv", 30.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-30.csv", 30.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-35.csv", 35.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-35.csv", 35.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-40.csv", 40.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-40.csv", 40.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-45.csv", 45.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-45.csv", 45.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-50.csv", 50.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-50.csv", 50.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-55.csv", 55.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-55.csv", 55.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-60.csv", 60.0, 1000000);
    x.runAVLFindTest("AVLFindNormal-60.csv", 60.0, 1000000);
    */

   
   
    //=============
    /*
    x.runSplayFindTest("SplayFindNormalALT.csv", 100.0, 1000000);
    x.runAVLFindTest("AVLFindNormalALT.csv", 100.0, 1000000);

    x.runSplayFindTest("SplayFindNormal-1ALT.csv", 100.0, 2000000);
    x.runAVLFindTest("AVLFindNormal-1ALT.csv", 100.0, 2000000);

    x.runSplayFindTest("SplayFindNormal-2ALT.csv", 100.0, 3000000);
    x.runAVLFindTest("AVLFindNormal-2ALT.csv", 100.0, 3000000);

    x.runSplayFindTest("SplayFindNormal-3ALT.csv", 100.0, 4000000);
    x.runAVLFindTest("AVLFindNormal-3ALT.csv", 100.0, 4000000);
    */


   
    return 0;
}


