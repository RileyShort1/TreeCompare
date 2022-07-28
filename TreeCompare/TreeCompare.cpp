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

    // New function to tests repeat lookups
    void repeatLookups(int randSeed, size_t N, bool normalData, int numTests)
    {
        srand(randSeed);

        // Tree creation
        SplayTree<int> SplayTestTree;
        AVL_Tree<int> AVLTestTree;
        

        // get data

        std::vector<int> rands;

        if (normalData == true)
        {
            for (int i = 0; i < 1000000; i++)
            {
                rands.push_back(rand_gaussian(N));
            }
        }
        else
        {
            for (int i = 0; i < 1000000; i++)
            {
                rands.push_back(rand_uniform(N));
            }
        }

        // fill tree
        for (size_t i = 0; i < rands.size(); i++)
        {
            SplayTestTree.insert(rands[i]);
            AVLTestTree.insert(rands[i]);
        }

        // test lookup
        clock_t AVLFind;
        clock_t SplayFind;

        std::vector<double> AVLTimes;
        std::vector<double> SplayTimes;

        srand(randSeed);
        int randIndex = rand() % 1000000;

        // Splay

        for (int i = 0; i < numTests; i++)
        {
            auto start = high_resolution_clock::now();

            SplayTestTree.contains(rands[randIndex]);

            auto stop = high_resolution_clock::now();
            duration<double, std::milli> ms_double = stop - start;
            SplayTimes.push_back(ms_double.count());
        }

        // AVL
      
        for (int i = 0; i < numTests; i++)
        {
            auto start2 = high_resolution_clock::now();

            AVLTestTree.contains(rands[randIndex]);

            auto stop2 = high_resolution_clock::now();
            duration<double, std::milli> ms_double2 = stop2 - start2;
            AVLTimes.push_back(ms_double2.count());
        }


        // compute averages for Splay
        double SplayTotal = 0;
        for (int i = 0; i < SplayTimes.size(); i++)
        {
            SplayTotal += SplayTimes[i];
        }

        std::cout << "Splay Average find for " << rands[randIndex] << " was: " << SplayTotal / SplayTimes.size() << " milliseconds" << std::endl;

        // compute averages for AVL
        double AVLTotal = 0;
        for (int i = 0; i < AVLTimes.size(); i++)
        {
            AVLTotal += AVLTimes[i];
        }

        std::cout << "AVL Average find for " << rands[randIndex] << " was: " << AVLTotal / AVLTimes.size() << " milliseconds" << std::endl;
       
        if (SplayTotal / SplayTimes.size() < AVLTotal / AVLTimes.size())
        {
            std::cout << "Splay is Faster!" << std::endl;
        }
        else
        {
            std::cout << "AVL is Faster!" << std::endl;
        }

        return;
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

    void testSplay(int randSeed, size_t N, int num_rands, bool normalDist, std::string fileName)
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
   // x.run_tests(5, 5);
    x.repeatLookups(15, 7, true, 1000);
    x.repeatLookups(10, 7, true, 1000);
    x.repeatLookups(100, 7, true, 1000);
    x.repeatLookups(20, 7, true, 1000);
    x.repeatLookups(2, 7, true, 1000);
    x.repeatLookups(155, 7, true, 1000);



    
}


