#pragma once
#include <assert.h>
#include <iostream>
#include <debug_new.h>
#include <merge.h>

template<class Value>
class TSingleLinkedList {
public:
    class ForwardIterator;

private:
    class Node;

public:
    TSingleLinkedList()
    {
        initialize();
    }

    ~TSingleLinkedList()
    {
        clear();
    }

    TSingleLinkedList(std::initializer_list<Value> init)
    {
        initialize();
        reserve(init.size());
        Nstd::copy(init.begin(), init.end(), begin());
    }

    TSingleLinkedList(const TSingleLinkedList& other)
    {
        initialize();
        *this = other;
    }

    TSingleLinkedList(TSingleLinkedList&& other)
    {
        initialize();
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(sz, other.sz);
    }

    void operator=(const TSingleLinkedList& other)
    {
        clear();
        reserve(other.size());
        Nstd::copy(other.begin(), other.end(), begin());
    }

    void operator=(TSingleLinkedList&& other)
    {
        clear();
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(sz, other.sz);
    }

    ForwardIterator begin() const
    {
        return ForwardIterator(head);
    }

    ForwardIterator end() const
    {
        static const ForwardIterator result(0);
        return result;
    }

    void clear() 
    {
        while (sz > 0) {
            pop_front();
        }
    }

    void push_front(const Value& v) 
    {
        if(sz == 0) {
            assert(head == 0);
            assert(tail == 0);
        }
        if(head == 0) {
            assert(tail == 0);
            head = tail = new Node(v);
        } else {
            head = new Node(v, head);
        }
        ++sz;
    }

    void push_back(const Value& v) 
    {
        if(sz == 0) {
            assert(head == 0);
            assert(tail == 0);
        }
        if(tail == 0) {
            assert(head == 0);
            head = tail = new Node(v);
        } else {
            tail = tail->next = new Node(v);
        }
        ++sz;
    }

    void insert(const ForwardIterator& it, const Value& v)
    {
        assert(it.node != 0);
        Node* newnode = new Node(std::move(it.node->v));
        it.node->v = v;
        newnode->next = it.node->next;
        it.node->next = newnode;
        if (tail == it.node) {
            tail = newnode;
        }
        sz++;
    }

    Value pop_front()
    {
        assert(size());
        assert(head != 0);
        Node* tmp = head;
        head = head->next;
        Value ret = tmp->v;
        delete tmp;
        if(--sz == 0) {
            tail = head;
        } 
        return ret;
    }

    void reserve(unsigned int n)
    {
        for(unsigned int i = 0; i < n; ++i) {
            push_back(Value());
        }
    }

    int size() const 
    {
        return sz;
    }

    TSingleLinkedList<Value> sorted()
    {
        return sorted(begin(), end());
    }

public:
    class ForwardIterator : public std::iterator<std::forward_iterator_tag, Value> {
    friend class TSingleLinkedList;
    public:
        Value& operator*() const {
            assert(node != 0);
            return node->v;
        }

        Value* operator->() const {
            assert(node != 0);
            return &node->v;
        }

        ForwardIterator& operator++() // prefix
        {
            node = node->next;
            return *this;
        }

        ForwardIterator operator++(int) // postfix
        {
            ForwardIterator ret = ForwardIterator(*this);
            node = node->next;
            return ret;
        }

        bool operator==(const ForwardIterator& other) const 
        {
            return node == other.node;
        }

        bool operator!=(const ForwardIterator& other) const 
        {
            return node != other.node;
        }

    private:
        explicit ForwardIterator(Node* _node) : 
            node( _node )
        {
        }

    private:
        Node* node;

    };

private:
    Node* head;
    Node* tail;
    int sz;

    void initialize() 
    {
        head = 0;
        tail = 0;
        sz = 0;
    }

    TSingleLinkedList<Value> sorted(ForwardIterator first, ForwardIterator last)
    {
        TSingleLinkedList<Value> res;
        int steps = 0;
        if (first != last) {
            ForwardIterator it1 = first, it2 = first;
            while (++it1 != last && ++it2 != last && ++it2 != last) { ++steps; }
            if (it1 == last) {
                // single-element list
                res.reserve(1);
                Nstd::copy(first, last, res.begin());
            } else {
                if(steps == 0) {
                    // two-element list, no need to recurse
                    res.push_back(std::min(*first, *it1));
                    res.push_back(std::max(*first, *it1));
                } else {
                    TSingleLinkedList<Value> tmp1 = sorted(first, it1);
                    TSingleLinkedList<Value> tmp2 = sorted(it1, last);
                    res.reserve(tmp1.size() + tmp2.size());
                    Nstd::merge(tmp1.begin(), tmp1.end(), tmp2.begin(), tmp2.end(), res.begin());
                }
            }
        }
        return res;
    }

private:
    class Node {
    friend class TSingleLinkedList;
    friend class TSingleLinkedList::ForwardIterator;

    private:
        Value v;
        Node* next;

        Node(const Value& _v, Node* _next = 0) : 
            v(_v),
            next(_next)
        {
        }
    };
};
