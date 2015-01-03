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

    const int N = 10000;
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

    const int N = 10000;
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

    int tree_sz = 0;

    for(int i = 0; i < 10000; i++) {
        int v1 = rand() % 1000;
        tree_sz += tree.insert(v1) ? 1 : 0;
        rbtree.insert(v1);
        assert(rbtree.rbt_satisfied());
        int v2 = rand() % 1000;
        tree_sz += tree.insert(v2) ? 1 : 0;
        rbtree.insert(v2);
        assert(rbtree.rbt_satisfied());
        int v3 = rand() % 1000;
        tree_sz -= tree.remove(v3) ? 1 : 0;
        rbtree.remove(v3);
        assert(rbtree.rbt_satisfied());
    }

    assert(tree.size() == tree_sz);
    
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

void test_rb_remove_trivial()
{
    TRedBlackTree<int> rb1;
    assert(rb1.insert(1));
    assert(rb1.remove(1));
    assert(!rb1.find(1));
    assert(rb1.depth() == 0);
    assert(rb1.size() == 0);

    assert(rb1.insert(1)); // << root
    assert(rb1.insert(2)); 
    assert(rb1.remove(1)); // << 2 becomes root 
    assert(rb1.depth() == 1);
    assert(rb1.size() == 1);
    assert(!rb1.find(1));
    assert(rb1.find(2));

    assert(rb1.insert(1)); // << leaf
    assert(rb1.remove(1));
    assert(rb1.depth() == 1);
    assert(rb1.size() == 1);
    assert(!rb1.find(1));
    assert(rb1.find(2));

    assert(rb1.insert(1)); // << left leaf
    assert(rb1.insert(3)); // << right leaf
    assert(rb1.depth() == 2);
    assert(rb1.size() == 3);
    assert(rb1.remove(2));
    assert(rb1.depth() == 2);
    assert(rb1.size() == 2);
    assert(!rb1.find(2));
    assert(rb1.find(1));
    assert(rb1.find(3));
}

int main(int argc, char* argv[])
{
    test_basic();
    test_ascending();
    test_worst_case_ins_depth();
    test_ins_depth();
    test_in_order_traverse();
    test_rb_remove_trivial();
    return 0;
}
