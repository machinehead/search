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

    TBinaryTree(const TBinaryTree& other) : 
        root( 0 ),
        sz( 0 )
    {
        other.in_order_traverse( [this] (Value v) { this->insert(v); } ); 
    }

    const TBinaryTree& operator=(const TBinaryTree& other)
    {
        clear();        
        other.in_order_traverse( [this] (Value v) { this->insert(v); } ); 
        return other;
    }

    TBinaryTree(TBinaryTree&& other) : 
        root( 0 ),
        sz( 0 )
    {
        std::swap(root, other.root);
        std::swap(sz, other.sz);
    }

    void operator=(TBinaryTree&& other)
    {
        clear();
        std::swap(root, other.root);
        std::swap(sz, other.sz);
    }

    virtual ~TBinaryTree() 
    {
        clear();
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

    bool find(const Value& v) const
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
    void in_order_traverse(Func f) const
    {
        internal_in_order_traverse(root, f);
    }

    template<class Func>
    void pre_order_traverse(Func f) const
    {
        internal_pre_order_traverse(root, f);
    }

    template<class Func>
    void post_order_traverse(Func f) const
    {
        internal_post_order_traverse(root, f);
    }

    void clear()
    {
        internal_clear(&root);
    }

public:
    class IteratorBase : public std::iterator<std::forward_iterator_tag, Value> {
    public:
        virtual ~IteratorBase()
        {
        }

        bool operator==(const IteratorBase& other) 
        {
            return node == other.node;
        }

        bool operator!=(const IteratorBase& other) 
        {
            return !(node == other.node);
        }

        const Value& operator*() const 
        {
            return node->v;
        }

        IteratorBase& operator++() // prefix
        {
            advance();
            return *this;
        }

    protected:
        Node* node;

        IteratorBase(Node* _node) : 
            node( _node )
        {
        }

        virtual void advance() = 0;
    };

    class PreOrderIterator : public IteratorBase {
    public:
        PreOrderIterator(Node* _node) : 
            IteratorBase( _node )
        {
        }

        virtual void advance()
        {
            assert(this->node != 0);
            if (this->node->left != 0) {
                this->node = this->node->left;
            } else if (this->node->right != 0) {
                this->node = this->node->right;
            } else {
                Node* prev = 0;
                do {
                    prev = this->node;
                    this->node = this->node->parent;
                } while (this->node != 0 && (prev == this->node->right || this->node->right == 0));
                if (this->node != 0 && this->node->right != 0) {
                    this->node = this->node->right;
                }
            }
        }
    };

    class InOrderIterator : public IteratorBase {
    public:
        InOrderIterator(Node* _node) : 
            IteratorBase( _node )
        {
            if (this->node != 0) {
                set_leftmost_child(this->node);
            }
        }

        virtual void advance() 
        {
            assert(this->node != 0);
            if (this->node->right != 0) {
                set_leftmost_child(this->node->right);
            } else if (this->node->parent == 0) {
                this->node = 0;
            } else {
                Node* prev = this->node;
                do {
                    prev = this->node;
                    this->node = this->node->parent;
                } while (this->node != 0 && prev == this->node->right);
            }
        }

    private:
        void set_leftmost_child(Node* n)
        {
            assert( n != 0 );
            while (n->left != 0) {
                n = n->left;
            }
            this->node = n;
        }
    };

    class PostOrderIterator : public IteratorBase {
    public:
        PostOrderIterator(Node* _node) : 
            IteratorBase( _node )
        {
            if (this->node != 0) {
               set_left_deepest_child(this->node);
            }
        }

        virtual void advance() 
        {
            assert(this->node != 0);
            Node* prev = this->node;
            this->node = this->node->parent;
            if (this->node != 0 && prev == this->node->left && this->node->right != 0) {
                set_left_deepest_child(this->node->right);
            }
        }

    private:
        void set_left_deepest_child(Node* n)
        {
            assert( n != 0 );
            while (true) {
                if (n->left != 0) {
                    n = n->left;
                } else if (n->right != 0) {
                    n = n->right;
                } else {
                    break;
                }
            }
            this->node = n;
        }
    };

    PreOrderIterator begin_preorder()
    {
        return PreOrderIterator(root);
    }

    PreOrderIterator end_preorder()
    {
        return PreOrderIterator(0);
    }

    InOrderIterator begin_inorder()
    {
        return InOrderIterator(root);
    }

    InOrderIterator end_inorder()
    {
        return InOrderIterator(0);
    }

    PostOrderIterator begin_postorder()
    {
        return PostOrderIterator(root);
    }

    PostOrderIterator end_postorder()
    {
        return PostOrderIterator(0);
    }

protected:
    virtual Node* create_node(const Value& v)
    {
        return new Node(v);
    }

    virtual void internal_clear(Node** r)
    {
        if(*r != 0) {
            internal_clear(&(*r)->left);
            internal_clear(&(*r)->right);
            delete *r;
            *r = 0;
        }
        sz = 0;
    }

    virtual void on_insert(Node* n)
    {

    }

    virtual void internal_remove(Node** n) 
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
            (*n)->v = (*d)->v;
            internal_remove(d);
        }
    }

    Node** leftmost_descendant(Node** n)
    {
        assert(*n != 0);
        while ((*n)->left != 0) {
            n = &(*n)->left;
        }
        return n;
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

    Node** internal_find(const Value& v) const 
    {
        return const_cast<TBinaryTree*>(this)->internal_find(v);
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

    template<class Func>
    void internal_in_order_traverse(Node* n, Func f) const
    {
        if (n != 0) {
            internal_in_order_traverse(n->left, f);
            f(n->v);
            internal_in_order_traverse(n->right, f);
        }
    }

    template<class Func>
    void internal_pre_order_traverse(Node* n, Func f) const
    {
        if (n != 0) {
            f(n->v);
            internal_pre_order_traverse(n->left, f);
            internal_pre_order_traverse(n->right, f);
        }
    }

    template<class Func>
    void internal_post_order_traverse(Node* n, Func f) const
    {
        if (n != 0) {
            internal_post_order_traverse(n->left, f);
            internal_post_order_traverse(n->right, f);
            f(n->v);
        }
    }

};
