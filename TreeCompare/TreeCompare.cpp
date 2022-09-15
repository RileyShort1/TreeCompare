#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

//#include "BST.h"
//#include "Tree_Algorithms.h"
#include "AVL_Tree.h"
#include "SplayTree.h"

using namespace std;


int main() {
    const size_t max_std_dev_tested = 100 * 1000;
    const size_t expt_mean = 50 * 1000; //expect nums to be centered around here, sigma determines spread
    const size_t std_dev_step = 50;
    const size_t num_random_runs = 25;
    const size_t num_trees_per_run = 100;
    const size_t num_inserts_per_tree = 100 * 1000;
    const size_t num_finds_per_tree = 500 * 1000;
    
    for (int n = 0; n < num_random_runs; n++) {
        cout << endl;
        cout << "#SEED = " << n << ":\n";
        cout << "###############################################################";
        
        //sigma can't be 0 because the generator does not accept it
        for (int sigma = 1; sigma <= max_std_dev_tested; sigma += std_dev_step) {
            std::mt19937 randEngine(n); //seed
            std::normal_distribution<double> generator(expt_mean, (double) sigma);
            
            cout << "\n#SIGMA, SPLAY TIME, AVL TIME, TREE SIZE\n";  //FIXED
            vector<double> avg_over_all_trees{0,0,0,0};
            for (int t = 0; t < num_trees_per_run; t++) {
                
                AVL_Tree<int> avl_tree;
                //SplayTree<int> splay_tree;
                BST<int> splay_tree;
                
                
                            
                //Make our full trees
                for (size_t i = 0; i < num_inserts_per_tree; i++) {
                    int my_random = (int)generator(randEngine);
                    avl_tree.insert(my_random);
                    //splay_tree.insert(my_random);
                    Tx::splay_insert(splay_tree, my_random);
                }
                
                //At this point, both our trees are ready to test and filled with identical randoms
                
                //time our find methods
                double total_avl_find_time = 0, total_splay_find_time = 0;
                for (size_t f = 0; f < num_finds_per_tree; f++) {
                    int num_to_find = (int) generator(randEngine);
                    
                    auto then = std::chrono::high_resolution_clock::now();
                    (void) avl_tree.contains(num_to_find);
                    auto now = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::micro> diff = now - then;
                    total_avl_find_time += diff.count();
                    
                    //same thing for splay
                    then = std::chrono::high_resolution_clock::now();
                    //(void) splay_tree.contains(num_to_find);
                    Tx::splay_contains(splay_tree, num_to_find);
                    now = std::chrono::high_resolution_clock::now();
                    diff = now - then;
                    total_splay_find_time += diff.count();
                    
                }
                cout << sigma << ", " << total_splay_find_time << ", " << total_avl_find_time << ", " << splay_tree.get_size() << endl;             //changed to splay
                avg_over_all_trees[0] += sigma;
                avg_over_all_trees[1] += total_splay_find_time;
                avg_over_all_trees[2] += total_avl_find_time;
                avg_over_all_trees[3] += splay_tree.get_size();   //changed to splay
                
                avl_tree.clear();     //Fix AVL deconstructor and remove this line afterwards
                
            } //num trees
            cout << "#Avg: " << avg_over_all_trees[0] / num_trees_per_run
                 << ", "   << avg_over_all_trees[1] / num_trees_per_run
                 << ", "   << avg_over_all_trees[2] / num_trees_per_run
                 << ", "   << avg_over_all_trees[3] / num_trees_per_run;
            //return 0;   //temporary
        } //sigma
        //return 0;      //temporary
    } //seeds
    
    return 0;
}
