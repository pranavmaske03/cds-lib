#include <iostream>
#include "include/stack.hpp"
#include "include/queue.hpp"
#include "include/bst.hpp"
#include "include/singly_list.hpp"
#include "include/doubly_list.hpp"
#include "include/singly_circular.hpp"
#include "include/doubly_circular.hpp"
#include "include/array.hpp"
#include "include/hash_map.hpp"
#include "include/lru_cache.hpp"

// ============================================================
//  main.cpp — cds (Custom Data Structures) userdefined namespace
//
//  USAGE:
//  1. include only the header you need
//  2. create object using cds:: namespace and use its methods
// ============================================================

int main() {

    cds::singly_list<int> my_singly;            // singly linear linked list
    cds::doubly_list<int> my_doubly;            // doubly linear linked list
    cds::singly_circular<int> my_s_circular;    // singly circular linked list
    cds::doubly_circular<int> my_d_circular;    // doubly circular linked list

    cds::stack<int>  my_stack;   // LIFO — push, pop, top
    cds::queue<int>  my_queue;   // FIFO — enqueue, dequeue, front, back

    cds::bst<int>    my_tree;    // binary search tree — insert, erase, contains, traversals
    cds::array<int>  my_array(10);  // fixed size array — search, sort

    cds::hash_map<std::string, int> my_map; // hash map — insert, get, erase, contains
    cds::lru_cache<int, std::string> my_cache(5); // LRU cache — get, put, remove, contains

    return 0;
}