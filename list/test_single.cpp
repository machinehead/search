#include <iostream>
#include <single_list.h>
#include <merge.h>
#include <debug_new.h>

template<class Value>
class TCopyCounter {
public:
    TCopyCounter(Value _v) :
        v( _v )
    {

    }

    TCopyCounter(const TCopyCounter& other)
    {
        v = other.v;
        copy_counts += 1;
    }

    Value value() const
    {
        return v;
    }

    const TCopyCounter& operator=(const TCopyCounter& other) 
    {
        v = other.v;
        copy_counts += 1;
        return other;
    }

    static int get_copy_counts() 
    { 
        return copy_counts;
    }

private:
    Value v;

    static int copy_counts;
};

template<class CharT, class Value>
std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const TCopyCounter<Value>& c) 
{
    return os << c.value();
}

template<>
int TCopyCounter<int>::copy_counts = 0;

typedef TSingleLinkedList<TCopyCounter<int>> CountingListType;

void test_counting()
{
    CountingListType lst;
    lst.push_front(TCopyCounter<int>(1));
    lst.push_front(TCopyCounter<int>(2));
    lst.push_front(TCopyCounter<int>(3));

    std::cout << "Values copied " << TCopyCounter<int>::get_copy_counts() << " times after initializing list of size " << lst.size() << "." << std::endl;

    for(CountingListType::ForwardIterator it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Values copied " << TCopyCounter<int>::get_copy_counts() << " times." << std::endl;

    for(auto v : lst) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    std::cout << "Values copied " << TCopyCounter<int>::get_copy_counts() << " times." << std::endl;
}

void test_size()
{
    typedef TSingleLinkedList<int> IntListType;

    IntListType lst;
    lst.push_front(1);
    lst.push_front(2);
    assert(lst.size() == 2);
    
    int count = 0;
    for(IntListType::ForwardIterator it = lst.begin(); it != lst.end(); ++it) {
        ++count;
    }
    assert(count == 2);
    
    lst.push_back(3);
    lst.push_back(4);
    assert(lst.size() == 4);

    IntListType checkList({2, 1, 3, 4});
    assert(std::equal(lst.begin(), lst.end(), checkList.begin()));
    
    count = 0;
    for(auto v : lst) {
        ++count;
    }
    assert(count == 4);

    assert(lst.pop_front() == 2);
    assert(lst.size() == 3);
    assert(lst.pop_front() == 1);
    assert(lst.size() == 2);
    assert(lst.pop_front() == 3);
    assert(lst.size() == 1);
    assert(lst.pop_front() == 4);
    assert(lst.size() == 0);

    lst.push_back(100);
    assert(lst.pop_front() == 100);
    assert(lst.size() == 0);
}

void test_copy()
{
    typedef TSingleLinkedList<int> IntListType;

    IntListType lst1, lst2;
    lst1.push_front(1);
    lst1.push_front(2);
    assert(lst1.size() == 2);

    lst2.reserve(lst1.size());
    assert(Nstd::copy(lst1.begin(), lst1.end(), lst2.begin()) == lst2.end());
    assert(std::equal(lst1.begin(), lst1.end(), lst2.begin()));
    assert(lst2.pop_front() == 2);
    assert(lst2.pop_front() == 1);
    assert(lst2.size() == 0);
    assert(lst1.size() == 2);
}

void test_init_list()
{
    typedef TSingleLinkedList<int> IntListType;
    
    IntListType lst({1,2,3,4,5});
    assert(lst.size() == 5);
    assert(lst.pop_front() == 1);   
    assert(lst.pop_front() == 2);   
}

void test_move()
{
    typedef TSingleLinkedList<int> IntListType;
    
    IntListType lst2;
    {
        lst2 = IntListType({1,2,3,4,5});
    }
    assert(lst2.size() == 5);
    assert(lst2.pop_front() == 1);   
    assert(lst2.pop_front() == 2);   
}

void test_sorted()
{
    typedef TSingleLinkedList<int> IntListType;
    
    IntListType lst({1,10,2,9,3,8,4,7,5,6,30,11,29,12,28,13,27,14,26,15,25,16,24,17,23,18,22,19,21,20});
    IntListType lst2 = lst.sorted();
    for(auto v : lst) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    for(auto v : lst2) {
        std::cout << v << " ";
    }
    assert(std::is_sorted(lst2.begin(), lst2.end()));
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    test_size();
    test_counting();
    test_copy();
    test_init_list();
    test_move();
    test_sorted();
    return 0;
}