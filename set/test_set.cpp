#include <single_set.h>
#include <measure.h>
#include <debug_new.h>

void test_basic()
{
    TSingleSet<int> set;
    assert(set.size() == 0);
    set.add(1);
    assert(set.size() == 1);
    set.remove(1);
    assert(set.size() == 0);
}

void test_init()
{
    TSingleSet<int> set1({ 1, 2, 3, 4, 5 });
    assert(set1.size() == 5);
    TSingleSet<int> set2({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
    assert(set2.size() == 5);
}

void test_set_ops()
{
    TSingleSet<int> set1({ 1, 2, 3, 4, 5 });
    assert(set1.size() == 5);
    TSingleSet<int> set2({ 4, 5, 6, 7, 8 });
    assert(set2.size() == 5);
    set1.add(set2);
    assert(set1.size() == 8);
    TSingleSet<int> set3({ 1, 3, 5, 7, 9, 11, 13, 15 });
    set3.remove(set1);
    assert(set3.size() == 4);
}

void set_insert_rb(TSingleSet<int>& set, int N)
{
    for (int i = 0; i < N; i++) {
        set.add(i);
    }
}

void set_insert_bt(TSingleSet<int, TBinaryTree<int>>& set, int N)
{
    for (int i = 0; i < N; i++) {
        set.add(i);
    }
}

void measure_ins_time()
{
    int N = 5;
    while(N < 10000000) {
        TSingleSet<int> set;
        std::cout << "Inserting " << N << " items to RB-tree-based set takes " << Nstd::measure<>::execution(set_insert_rb, set, N) << " us" << std::endl;
        N *= 2;
    }
    N = 5;
    while(N < 50000) {
        TSingleSet<int, TBinaryTree<int>> set;
        std::cout << "Inserting " << N << " items to binary tree-based set takes " << Nstd::measure<>::execution(set_insert_bt, set, N) << " us" << std::endl;
        N *= 2;
    }
}

template <class Tree>
void test_move_copy()
{
    TSingleSet<int, Tree> set1;
    assert(set1.add(1));
    assert(set1.add(2));

    // copy constructor
    TSingleSet<int, Tree> set2(set1);
    assert(set2.find(1));
    assert(set2.find(2));

    // copy assignment
    TSingleSet<int, Tree> set3;
    assert(set3.add(3));
    assert(set3.find(3));
    assert(set3.size() == 1);
    set3 = set1;
    assert(!set3.find(3));
    assert(set3.size() == 2);
    assert(set3.find(1));
    assert(set3.find(2));

    // independence checks
    assert(set1.remove(1));
    assert(!set1.find(1));
    assert(set1.find(2));
    assert(set2.find(1));
    assert(set2.find(2));
    assert(set3.find(1));
    assert(set3.find(2));

    // move constructor
    TSingleSet<int, Tree> set4(std::move(set2));
    assert(set2.size() == 0);
    assert(!set2.find(1));
    assert(!set2.find(2));
    assert(set4.size() == 2);
    assert(set4.find(1));
    assert(set4.find(2));

    // move assignment
    TSingleSet<int, Tree> set5;
    set5.add(3);
    assert(set5.size() == 1);
    assert(set5.find(3));
    set5 = std::move(set3);
    assert(set3.size() == 0);
    assert(!set3.find(1));
    assert(!set3.find(2));
    assert(set5.size() == 2);
    assert(set5.find(1));
    assert(set5.find(2));
}

void test_arith_ops()
{
    std::cout << "test_arith_ops()" << std::endl;
    TSingleSet<int> set1({10, 20, 30, 40, 50, 60});
    TSingleSet<int> set2;
    for(int i = 0; i < 100; i++) {
        set2.add(i);
    }
    TSingleSet<int> set3(set2 - set1);
    for(int i = 0; i < 100; i++) {
        assert( (set1.find(i) && !set3.find(i)) || (set2.find(i) && set3.find(i)) ); 
    }
    TSingleSet<int> set4 = set1 & TSingleSet<int>({10, 20, 30});
    assert(set4.size() == 3);
    assert((set1 - set4).size() == 3);
}

int main(int argc, char* argv[])
{
    test_basic();
    test_init();
    test_set_ops();
    measure_ins_time();
    test_move_copy<TBinaryTree<int>>();
    test_move_copy<TRedBlackTree<int>>();
    test_arith_ops();
    return 0;
}