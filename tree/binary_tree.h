#pragma once
#include <single_list.h>
#include <debug_new.h>

template<class Value>
class TBinaryTree {
protected:
    class Node {
    friend class TBinaryTree;
    protected:
        Node(const Value& _v) : 
            left( 0 ),
            right( 0 ),
            parent( 0 ),
            v( _v )
        {

        }

        Node* left;
        Node* right;
        Node* parent;
        Value v;
    };

public:
    TBinaryTree() : 
        root( 0 ),
        sz( 0 )
    {
    }

    virtual ~TBinaryTree() 
    {
        clear(&root);
    }

    bool insert(const Value& v)
    {
        Node* inserted = 0;
        if (root != 0) {
            Node* parent = internal_find_ins_parent(v);
            assert(parent != 0);
            if (parent->v == v) {
                return false;
            }
            inserted = create_node(v);
            if (parent->v > v) {
                parent->left = inserted;
            } else {
                parent->right = inserted;
            }
            inserted->parent = parent;
        } else {
            root = inserted = create_node(v);
        }
        on_insert(inserted);
        ++sz;
        return true;
    }

    bool find(const Value& v)
    {
        Node** n = internal_find(v);
        if (*n != 0) {
            assert((*n)->v == v);
            return true;
        }
        return false;
    }

    bool remove(const Value& v)
    {
        Node** n = internal_find(v);
        if (*n == 0) {
            return false;
        }
        internal_remove(n);
        --sz;
        return true;
    }

    int size() const
    {
        return sz;
    }

    int depth() const
    {
        TSingleLinkedList<std::tuple<Node*, int>> stack;
        int result = 0, d = 1;
        Node* n = root;
        while (n != 0) {
            result = std::max(result, d);
            if (n->left != 0) {
                stack.push_front(std::make_tuple(n, d));
                n = n->left;
                ++d;
            } else if (n->right != 0) {
                n = n->right;
                ++d;
            } else {
                n = 0;
                while (stack.size() > 0) {
                    std::tie(n, d) = stack.pop_front();
                    if ( n->right != 0 ) {
                        n = n->right;
                        ++d;
                        break;
                    } else {
                        n = 0;
                    }
                }
            }
        }
        return result;
    }

    template<class Func>
    void in_order_traverse(Func f)
    {
        in_order_traverse(root, f);
    }

protected:
    virtual Node* create_node(const Value& v)
    {
        return new Node(v);
    }

    virtual void clear(Node** r)
    {
        if(*r != 0) {
            clear(&(*r)->left);
            clear(&(*r)->right);
            delete *r;
            *r = 0;
        }
    }

    virtual void on_insert(Node* n)
    {

    }

protected:
    Node* root;
    int sz;

private:
    Node** internal_find(const Value& v)
    {
        Node** n = &root;
        while (*n != 0) {
            if ((*n)->v == v) {
                return n;
            }
            if ((*n)->v > v) {
                n = &(*n)->left;
            } else {
                n = &(*n)->right;
            }
        }
        return n;
    }

    Node* internal_find_ins_parent(const Value& v)
    {
        assert(root != 0);
        Node* n = root;
        while (n->v != v) {
            Node* newnode = 0;
            if (n->v > v) {
                newnode = n->left;
            } else {
                newnode = n->right;
            }
            if(newnode == 0) {
                break;
            } else {
                n = newnode;
            }
        }
        return n;
    }

    Node** leftmost_descendant(Node** n)
    {
        assert(*n != 0);
        while ((*n)->left != 0) {
            n = &(*n)->left;
        }
        return n;
    }

    void internal_remove(Node** n) 
    {
        if ((*n)->left == 0) {
            Node* tmp = *n;
            *n = (*n)->right;
            delete tmp;
        } else if ((*n)->right == 0) {
            Node* tmp = *n;
            *n = (*n)->left;
            delete tmp;
        } else {
            Node** d = leftmost_descendant(&(*n)->right);
            if(*d == (*n)->right) {
                (*d)->left = (*n)->left;
                delete *n;
                *n = *d;
            } else {
                (*n)->v = (*d)->v;
                internal_remove(d);
            }
        }
    }

    template<class Func>
    void in_order_traverse(Node* n, Func f)
    {
        if (n != 0) {
            in_order_traverse(n->left, f);
            f(n->v);
            in_order_traverse(n->right, f);
        }
    }

};
