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

int main(int argc, char* argv[])
{
    test_basic();
    test_init();
    test_set_ops();
    measure_ins_time();
    return 0;
}