#include <assert.h>
#include <cstdlib>
#include <cmath>
#include <binary_tree.h>
#include <redblack_tree.h>
#include <debug_new.h>

void test_basic()
{
    TBinaryTree<int> tree;
    assert(tree.depth() == 0);
    assert(tree.insert(5));
    assert(tree.depth() == 1);
    assert(tree.insert(10));
    assert(tree.depth() == 2);
    assert(tree.insert(3));
    assert(tree.depth() == 2);
    assert(!tree.insert(5));
    assert(tree.find(5));
    assert(tree.find(10));
    assert(!tree.find(15));
    assert(tree.remove(5));
    assert(!tree.remove(5));
    assert(!tree.find(5));
    assert(tree.remove(10));
    assert(!tree.remove(10));
    assert(!tree.find(10));
    assert(tree.remove(3));
    assert(!tree.remove(3));
    assert(!tree.find(3));
    assert(tree.size() == 0);
}

void test_ascending()
{
    TBinaryTree<int> tree;

    for(int i = 0; i < 1000; i++) {
        tree.insert(i);
        assert(tree.depth() == i + 1);
    }
}

void test_worst_case_ins_depth()
{
    TBinaryTree<int> tree;
    TRedBlackTree<int> rbtree;

    const int N = 100000;
    for(int i = 0; i < N; i++) {
        tree.insert(i);
        rbtree.insert(i);
    }
    std::cout << "Inserted " << N << " ascending items, depth lower bound: " << log2(tree.size() + 1) << std::endl;
    std::cout << "Tree size " << tree.size() << ", depth " << tree.depth() << std::endl;
    std::cout << "Red-black tree size " << rbtree.size() << ", depth " << rbtree.depth() << std::endl; 
}

void test_ins_depth()
{
    TBinaryTree<int> tree;
    TRedBlackTree<int> rbtree;

    const int N = 100000;
    for(int i = 0; i < N; i++) {
        int v = rand() % 100000;
        tree.insert(v);
        rbtree.insert(v);
    }
    std::cout << "Inserted " << N << " items, depth lower bound: " << log2(tree.size() + 1) << std::endl;
    std::cout << "Tree size " << tree.size() << ", depth " << tree.depth() << std::endl;
    std::cout << "Red-black tree size " << rbtree.size() << ", depth " << rbtree.depth() << std::endl; 
}

void test_in_order_traverse()
{
    TBinaryTree<int> tree;
    TRedBlackTree<int> rbtree;

    for(int i = 0; i < 100; i++) {
        int v1 = rand() % 100;
        tree.insert(v1);
        rbtree.insert(v1);
        int v2 = rand() % 100;
        tree.insert(v2);
        rbtree.insert(v2);
        // int v3 = rand() % 100;
        // tree.remove(v3);
    }
    
    TSingleLinkedList<int> in_order_values_tree;
    tree.in_order_traverse( [&in_order_values_tree] (int v) { in_order_values_tree.push_back(v); } );
    assert(std::is_sorted(in_order_values_tree.begin(), in_order_values_tree.end()));

    for (auto v : in_order_values_tree) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    TSingleLinkedList<int> in_order_values_rbtree;
    rbtree.in_order_traverse( [&in_order_values_rbtree] (int v) { in_order_values_rbtree.push_back(v); } );
    assert(std::is_sorted(in_order_values_rbtree.begin(), in_order_values_rbtree.end()));

    for (auto v : in_order_values_rbtree) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    test_basic();
    test_ascending();
    test_worst_case_ins_depth();
    test_ins_depth();
    test_in_order_traverse();
    return 0;
}