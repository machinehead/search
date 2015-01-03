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

    TSingleSet(const TSingleSet& other) :
        tree(other.tree)
    {
    }

    const TSingleSet& operator=(const TSingleSet& other)
    {
        tree = other.tree;
    }

    TSingleSet(TSingleSet&& other) :
        tree(std::move(other.tree))
    {
    }

    void operator=(TSingleSet&& other)
    {
        tree = std::move(other.tree);
    }

    int size() const { return tree.size(); }
    bool add(const Value& v) { return tree.insert(v); }
    bool remove(const Value& v) { return tree.remove(v); }
    bool find(const Value& v) { return tree.find(v); }

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
