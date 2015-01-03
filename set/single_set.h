#pragma once
#include <redblack_tree.h>
#include <debug_new.h>

template<class Value, class TreeImpl=TRedBlackTree<Value>>
class TSingleSet {
public:
    TSingleSet()
    {
    }

    TSingleSet(std::initializer_list<Value> init)
    {
        for (auto it = init.begin(); it != init.end(); ++it) {
            tree.insert(*it);
        }
    }

    int size() const { return tree.size(); }
    void add(const Value& v) { tree.insert(v); }
    void remove(const Value& v) { tree.remove(v); }

    void add(const TSingleSet& other) 
    {
        other.tree.in_order_traverse( [this] (Value v) { this->add(v); } ); 
    }

    void remove(const TSingleSet& other) 
    {
        other.tree.in_order_traverse( [this] (Value v) { this->remove(v); } ); 
    }

private:
    TreeImpl tree;

};
