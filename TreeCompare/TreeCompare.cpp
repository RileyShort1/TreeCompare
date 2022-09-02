#include <iostream>
#include <random>
#include <chrono>

#include "SplayTree.h"
#include "AVL_Tree.h"

using namespace std;


int main(){
    const size_t max_std_dev_tested = 100 * 1000;
    const size_t expt_mean = 50 * 1000; //expect nums to be centered around here, sigma determines spread
    const size_t std_dev_step = 50;
    const size_t num_random_runs = 25;
    const size_t num_trees_per_run = 100;
    const size_t num_inserts_per_tree = 100 * 1000;
    const size_t num_finds_per_tree = 500 * 1000;

    for (int n = 0; n < num_random_runs; n++) {
        for (int sigma = 0; sigma < max_std_dev_tested; sigma += std_dev_step) {
            std::mt19937 randEngine(n); //seed
            std::normal_distribution<double> generator(expt_mean, sigma);
            
            for (int t = 0; t < num_trees_per_run; t++) {
                
                AVL_Tree<int> avl_tree;
                SplayTree<int> splay_tree;
                
                int my_random = (int) generator(randEngine);
                
                //Make our full trees
                for (size_t i = 0; i < num_inserts_per_tree; i++) {
                    avl_tree.insert(my_random);
                    splay_tree.insert(my_random);
                }
                
                //At this point, both our trees are ready to test and filled with identical randoms
                cout << avl_tree.get_size() << "\t";
                
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
                    (void) splay_tree.contains(num_to_find);
                    now = std::chrono::high_resolution_clock::now();
                    diff = now - then;
                    total_splay_find_time += diff.count();
                    
                }
                cout << sigma << "\t" << total_splay_find_time << "\t" << total_avl_find_time << endl;
            } //num trees
        } //sigma
    } //seeds
    
    return 0;
}
