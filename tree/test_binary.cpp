#include <assert.h>
#include <cstdlib>
#include <cmath>
#include <binary_tree.h>
#include <redblack_tree.h>
#include <measure.h>
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
    assert(rbtree.size() == tree_sz);
    
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

template <class Tree>
void test_move_copy()
{
    Tree tree1;
    assert(tree1.insert(1));
    assert(tree1.insert(2));

    // copy constructor
    Tree tree2(tree1);
    assert(tree2.find(1));
    assert(tree2.find(2));

    // copy assignment
    Tree tree3;
    assert(tree3.insert(3));
    assert(tree3.find(3));
    assert(tree3.size() == 1);
    tree3 = tree1;
    assert(!tree3.find(3));
    assert(tree3.size() == 2);
    assert(tree3.find(1));
    assert(tree3.find(2));

    // independence checks
    assert(tree1.remove(1));
    assert(!tree1.find(1));
    assert(tree1.find(2));
    assert(tree2.find(1));
    assert(tree2.find(2));
    assert(tree3.find(1));
    assert(tree3.find(2));

    // move constructor
    Tree tree4(std::move(tree2));
    assert(tree2.size() == 0);
    assert(!tree2.find(1));
    assert(!tree2.find(2));
    assert(tree4.size() == 2);
    assert(tree4.find(1));
    assert(tree4.find(2));

    // move assignment
    Tree tree5;
    tree5.insert(3);
    assert(tree5.size() == 1);
    assert(tree5.find(3));
    tree5 = std::move(tree3);
    assert(tree3.size() == 0);
    assert(!tree3.find(1));
    assert(!tree3.find(2));
    assert(tree5.size() == 2);
    assert(tree5.find(1));
    assert(tree5.find(2));
}

template <class Tree>
void test_iterator()
{
    Tree tree;
    assert( tree.begin_preorder() == tree.end_preorder() );
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);
    tree.insert(2);

    TSingleLinkedList<int> preorder_list({ 5, 3, 1, 2, 4, 7, 6, 8 });
    assert(std::equal(tree.begin_preorder(), tree.end_preorder(), preorder_list.begin()));
    TSingleLinkedList<int> inorder_list({ 1, 2, 3, 4, 5, 6, 7, 8 });
    assert(std::equal(tree.begin_inorder(), tree.end_inorder(), inorder_list.begin()));
    TSingleLinkedList<int> postorder_list({ 2, 1, 4, 3, 6, 8, 7, 5 });
    assert(std::equal(tree.begin_postorder(), tree.end_postorder(), postorder_list.begin()));
}

void test_inorder_sorted()
{
    TRedBlackTree<int> rbtree;

    const int N = 1000000;
    for(int i = 0; i < N; i++) {
        int v = rand() % 100000;
        rbtree.insert(v);
    }
    assert(std::is_sorted(rbtree.begin_inorder(), rbtree.end_inorder()));
}

template<class Iterator>
void copy_tree_iterator(Iterator begin, Iterator end, TRedBlackTree<int>& to)
{
    for (; begin != end; ++begin) {
        assert(to.insert(*begin));
    }
}

void test_rbtree_copy_order()
{
    TRedBlackTree<int> rbtree;

    const int N = 2000000;
    for(int i = 0; i < N; i++) {
        int v = rand() % 10000000;
        rbtree.insert(v);
    }
    
    TRedBlackTree<int> tree1;
    std::cout << "Copying " << rbtree.size() << " items with preorder iterator takes " << Nstd::measure<>::execution(
        copy_tree_iterator<TRedBlackTree<int>::PreOrderIterator>,
        rbtree.begin_preorder(),
        rbtree.end_preorder(),
        tree1
    ) << "us" << std::endl;
    assert(tree1.size() == rbtree.size());

    TRedBlackTree<int> tree2;
    std::cout << "Copying " << rbtree.size() << " items with inorder iterator takes " << Nstd::measure<>::execution(
        copy_tree_iterator<TRedBlackTree<int>::InOrderIterator>,
        rbtree.begin_inorder(),
        rbtree.end_inorder(),
        tree2
    ) << "us" << std::endl;
    assert(tree2.size() == rbtree.size());

    TRedBlackTree<int> tree3;
    std::cout << "Copying " << rbtree.size() << " items with postorder iterator takes " << Nstd::measure<>::execution(
        copy_tree_iterator<TRedBlackTree<int>::PostOrderIterator>,
        rbtree.begin_postorder(),
        rbtree.end_postorder(),
        tree3
    ) << "us" << std::endl;
    assert(tree3.size() == rbtree.size());
}

void test_iterator_order()
{
    TBinaryTree<int> tree;
    TRedBlackTree<int> rbtree;

    int tree_sz = 0;

    for(int i = 0; i < 10000; i++) {
        int v1 = rand() % 10000;
        tree_sz += tree.insert(v1) ? 1 : 0;
        rbtree.insert(v1);
    }

    assert(tree.size() == tree_sz);
    assert(rbtree.size() == tree_sz);
    
    TSingleLinkedList<int> in_order_values_tree;
    tree.in_order_traverse( [&in_order_values_tree] (int v) { in_order_values_tree.push_back(v); } );
    assert(std::equal(tree.begin_inorder(), tree.end_inorder(), in_order_values_tree.begin()));

    TSingleLinkedList<int> in_order_values_rbtree;
    rbtree.in_order_traverse( [&in_order_values_rbtree] (int v) { in_order_values_rbtree.push_back(v); } );
    assert(std::equal(rbtree.begin_inorder(), rbtree.end_inorder(), in_order_values_rbtree.begin()));

    TSingleLinkedList<int> pre_order_values_tree;
    tree.pre_order_traverse( [&pre_order_values_tree] (int v) { pre_order_values_tree.push_back(v); } );
    assert(std::equal(tree.begin_preorder(), tree.end_preorder(), pre_order_values_tree.begin()));

    TSingleLinkedList<int> pre_order_values_rbtree;
    rbtree.pre_order_traverse( [&pre_order_values_rbtree] (int v) { pre_order_values_rbtree.push_back(v); } );
    assert(std::equal(rbtree.begin_preorder(), rbtree.end_preorder(), pre_order_values_rbtree.begin()));

    TSingleLinkedList<int> post_order_values_tree;
    tree.post_order_traverse( [&post_order_values_tree] (int v) { post_order_values_tree.push_back(v); } );
    assert(std::equal(tree.begin_postorder(), tree.end_postorder(), post_order_values_tree.begin()));

    TSingleLinkedList<int> post_order_values_rbtree;
    rbtree.post_order_traverse( [&post_order_values_rbtree] (int v) { post_order_values_rbtree.push_back(v); } );
    assert(std::equal(rbtree.begin_postorder(), rbtree.end_postorder(), post_order_values_rbtree.begin()));
}

int main(int argc, char* argv[])
{
    test_basic();
    test_ascending();
    test_worst_case_ins_depth();
    test_ins_depth();
    test_in_order_traverse();
    test_rb_remove_trivial();
    std::cout << "test_move_copy<TBinaryTree<int>>()" << std::endl;
    test_move_copy<TBinaryTree<int>>();
    std::cout << "test_move_copy<TRedBlackTree<int>>()" << std::endl;
    test_move_copy<TRedBlackTree<int>>();
    test_iterator<TBinaryTree<int>>();
    test_iterator<TRedBlackTree<int>>();
    test_inorder_sorted();
    test_iterator_order();
    test_rbtree_copy_order();
    return 0;
}
