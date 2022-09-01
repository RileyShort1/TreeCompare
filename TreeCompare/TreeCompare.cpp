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
    template<typename T> void build_tree(T& theTree, unsigned int seed, double stddev, int num_rand_calls)
    {
        theTree.clear(); // delete everything in tree

        std::vector<int> rands;

        get_array_gaussian(rands, seed, stddev, num_rand_calls); // grab rands from generator

        for (unsigned int i = 0; i < rands.size(); i++)
        {
            theTree.insert(rands[i]);
        }
       
        return;
    }

    //note: & wanted to get rid of the bool parameter and for us to use a conditional in the function instead
    void searchTests(std::string fileName, double stddev)
    {
        AVL_Tree<int> avl_tree;
        SplayTree<int> splay_tree;
 
        double total_over_all_avl_trees = 0;
        double total_over_all_splay_trees = 0;

        double avg_time_per_find_avl;
        double avg_time_per_find_splay;

        double time_per_batch_avl = 0;
        double time_per_batch_splay = 0;

        size_t avgTreeSize = 0;
        int programTime = 0;
        std::vector<int> finds;
  
        for (int num_seeds = 0; num_seeds < 25; num_seeds++)
        {
            programTime++;
            std::cout << programTime << " / 25\n";

            for (int num_trees = 0; num_trees < 50; num_trees++) // this is essentially how many single tests we want per single seed
            {
                // build both trees
                build_tree(avl_tree, num_seeds, stddev, 250000);
                build_tree(splay_tree, num_seeds, stddev, 250000);

                get_array_gaussian(finds, num_seeds, stddev, 250000); // get nums to find
                avgTreeSize += avl_tree.get_size();
                
                time_per_batch_avl = 0;
                time_per_batch_splay = 0;

                for (int num_find_calls = 0; num_find_calls < 250000; num_find_calls++)
                {
                    // AVL Time
                    auto start_avl = high_resolution_clock::now();
                    avl_tree.contains(finds[num_find_calls]);     
                    auto stop_avl = high_resolution_clock::now();
                    duration<double, std::micro> single_time_avl = stop_avl - start_avl;

                    // Splay Time
                    auto start_splay = high_resolution_clock::now();
                    avl_tree.contains(finds[num_find_calls]);
                    auto stop_splay = high_resolution_clock::now();
                    duration<double, std::micro> single_time_splay = stop_splay - start_splay;

                    // add times
                    time_per_batch_avl += single_time_avl.count(); // add single find time to pool of times for avl tree k
                    time_per_batch_splay += single_time_splay.count(); // add single find time to pool of times for splay tree k
                }

                time_per_batch_avl /= 250000; // avg single find time for tree k (total time / num find operations)
                time_per_batch_splay /= 250000;

                total_over_all_avl_trees += time_per_batch_avl; // adds avg find time of items in tree k 
                total_over_all_splay_trees += time_per_batch_splay;
            }
        }

        avg_time_per_find_avl = total_over_all_avl_trees / 1250.0; // divide by # of trees tested to get avg time across all trees
        avg_time_per_find_splay = total_over_all_splay_trees / 1250.0; 

   
        avgTreeSize /= 1250; // get avg tree size

        std::fstream find; // output file
        find.open(fileName, std::ios::out | std::ios::app);

        find << std::fixed << std::setprecision(4) << "AVL avg time: " << avg_time_per_find_avl << ", " << avgTreeSize << "\n" <<
            "Splay avg time: " << avg_time_per_find_splay << ", " << avgTreeSize << "\n";

        find.close();

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

        std::normal_distribution<double> generator(50000.0, stddev); // (mean, stddev) Note: can't set explicit range, just mean

        randNums.clear();

        for (int i = 0; i < numRands; i++)
        {
            randNums.push_back(double(generator(randEngine)));
        }

        return;
    }

public: // ============================================= Public =============================================

    void runFindTest(std::string fileName, double stddev)
    {
        std::fstream foutSplayFind; // output file
        foutSplayFind.open(fileName, std::ios::out | std::ios::app);

        foutSplayFind << "Rand seeds 0-25" << ", " << "Stddev = " << stddev << ", "
            << "Finding 250k normal" << ", " << "Splay and AVL Tree with microsecond time"  << "\n"
            << "mean of data = 50k\n" 
            << "Find avg - Tree size" << "\n";
        foutSplayFind.close();

        searchTests(fileName, stddev);

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
    std::string fileName = "findTests-";

    for (int i = 50; i < 250001; i += 50)
    {
        fileName += std::to_string(i);
        fileName += ".csv";
        x.runFindTest(fileName, i);

        fileName = "findTests-"; // reset filename
    }
    */
   
    return 0;
}


