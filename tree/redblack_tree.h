#pragma once
#include <binary_tree.h>

template<class Value>
class TRedBlackTree : private TBinaryTree<Value> {
public:
    typedef TBinaryTree<Value> ParentClass;

    TRedBlackTree() : 
        ParentClass()
    {
    }

    TRedBlackTree(const TRedBlackTree& other) : 
        TBinaryTree<Value>()
    {
        other.in_order_traverse( [this] (Value v) { this->insert(v); } ); 
    }

    const TRedBlackTree& operator=(const TRedBlackTree& other)
    {
        clear();        
        other.in_order_traverse( [this] (Value v) { this->insert(v); } ); 
        return other;
    }

    TRedBlackTree(TRedBlackTree&& other) : 
        TBinaryTree<Value>()
    {
        std::swap(this->root, other.root);
        std::swap(this->sz, other.sz);
    }

    void operator=(TRedBlackTree&& other)
    {
        clear();
        std::swap(this->root, other.root);
        std::swap(this->sz, other.sz);
    }

    virtual ~TRedBlackTree() 
    {
        clear();
    }

    using ParentClass::insert;
    using ParentClass::size;
    using ParentClass::depth;
    using ParentClass::remove;
    using ParentClass::in_order_traverse;
    using ParentClass::pre_order_traverse;
    using ParentClass::post_order_traverse;
    using ParentClass::find;
    using ParentClass::clear;
    using typename ParentClass::PreOrderIterator;
    using ParentClass::begin_preorder;
    using ParentClass::end_preorder;
    using typename ParentClass::InOrderIterator;
    using ParentClass::begin_inorder;
    using ParentClass::end_inorder;
    using typename ParentClass::PostOrderIterator;
    using ParentClass::begin_postorder;
    using ParentClass::end_postorder;

    bool rbt_satisfied()
    {
        RBTNode* r = reinterpret_cast<RBTNode*>(this->root);
        if (r == 0) {
            return true;
        }
        if (r->red) {
            return false;
        }
        int bn_path = 0;
        return rbt_satisfied(r, bn_path);
    }

protected:
    typedef typename TBinaryTree<Value>::Node ParentNodeClass;

    class RBTNode : public ParentNodeClass {
    friend class TRedBlackTree;
    protected:
        RBTNode(const Value& _v) : 
            ParentNodeClass(_v),
            red(true)
        {

        }

        bool red;

        inline RBTNode* left_node() { return reinterpret_cast<RBTNode*>(this->left); }
        inline RBTNode* right_node() { return reinterpret_cast<RBTNode*>(this->right); }
        inline RBTNode* parent_node() { return reinterpret_cast<RBTNode*>(this->parent); }
    };

protected:
    virtual ParentNodeClass* create_node(const Value& v)
    {
        return new RBTNode(v);
    }

    virtual void internal_clear(ParentNodeClass** _r)
    {
        RBTNode** r = reinterpret_cast<RBTNode**>(_r);
        if(*r != 0) {
            internal_clear(&(*r)->left);
            internal_clear(&(*r)->right);
            delete *r;
            *r = 0;
        }
        this->sz = 0;
    }

    inline RBTNode* node_cast(ParentNodeClass* n)
    {
        return reinterpret_cast<RBTNode*>(n);
    }

    virtual void on_insert(ParentNodeClass* _n)
    {
        RBTNode* n = node_cast(_n);
        if (n == node_cast(this->root)) {
            n->red = false;
            return;
        }
        RBTNode* p = n->parent_node();
        if (!p->red) {
            return;
        }
        RBTNode* gp = p->parent_node();
        RBTNode* u = (p == gp->left_node()) ? gp->right_node() : gp->left_node();
        // u might be zero
        if (p->red && u != 0 && u->red) {
            p->red = u->red = false;
            gp->red = true;
            on_insert(gp);
            return;
        }
        bool rot = false;
        if (n == p->right_node() && p == gp->left_node()) {
            rot = true;
            rotate_left(n, p, gp);
        } else if (n == p->left_node() && p == gp->right_node()) {
            rot = true;
            rotate_right(n, p, gp);
        }
        if ( rot ) {
            std::swap(n, p);
        }
        p->red = false;
        gp->red = true;
        RBTNode* ggp = gp->parent_node();
        if (n == p->left_node()) {
            rotate_right(p, gp, ggp);
        } else if (n == p->right_node()) {
            rotate_left(p, gp, ggp);
        }
    }

    virtual void internal_remove(ParentNodeClass** _n)
    {
        RBTNode** n = reinterpret_cast<RBTNode**>(_n);
        if ((*n)->left == 0 || (*n)->right == 0) {
            delete_one_child(n);
        } else {
            RBTNode** m = reinterpret_cast<RBTNode**>(this->leftmost_descendant(&(*n)->right));
            (*n)->v = (*m)->v;
            delete_one_child(m);
        }
    }

private:
    bool rbt_satisfied(RBTNode* n, int& bn_path)
    {
        if (n == 0) {
            return true;
        }
        if (n->red) {
            if (n->left != 0 && n->left_node()->red) {
                return false;
            } else if (n->right != 0 && n->right_node()->red) {
                return false;
            }
        }
        int bn_left = 0, bn_right = 0;
        bool sat_left = rbt_satisfied(n->left_node(), bn_left);
        bool sat_right = rbt_satisfied(n->right_node(), bn_right);
        if(bn_left != bn_right) {
            return false;
        }
        bn_path = bn_left + ((!n->red) ? 1 : 0);
        return sat_left && sat_right;
    }

    inline void delete_one_child(RBTNode** n)
    {
        RBTNode* child = ((*n)->left == 0) ? (*n)->right_node() : (*n)->left_node();

        if (!(*n)->red) {
            if (child != 0 && child->red) {
                child->red = false;
            } else {
                delete_cases(*n);
            }
        }
        if(child != 0) {
            child->parent = (*n)->parent_node();
        }
        RBTNode* tmp = *n;
        *n = child;
        delete tmp;
    }

    inline void delete_cases(RBTNode* n) 
    {
        if(n->parent == 0) {
            return;
        }
        RBTNode* p = n->parent_node();
        RBTNode* s = (n == p->left_node()) ? p->right_node() : p->left_node();
        if(s != 0 && s->red) {
            p->red = true;
            s->red = false;
            if (s == p->left_node()) {
                rotate_right(s, p, p->parent_node());
                s = p->left_node();
            } else {
                rotate_left(s, p, p->parent_node());
                s = p->right_node();
            }
        }
        if (!p->red && !s->red && (s->left == 0 || !s->left_node()->red) && (s->right == 0 || !s->right_node()->red)) {
            s->red = true;
            delete_cases(p);
            return;
        }
        if (p->red && !s->red && (s->left == 0 || !s->left_node()->red) && (s->right == 0 || !s->right_node()->red)) {
            s->red = true;
            p->red = false;
            return;
        }
        if (!s->red) {
            if (n == p->left_node() && (s->right == 0 || !s->right_node()->red) && s->left != 0 && s->left_node()->red) {
                s->red = true;
                RBTNode* sl = s->left_node();
                sl->red = false;
                rotate_right(sl, s, s->parent_node());
                std::swap(sl, s);
            } else if (n == p->right_node() && (s->left == 0 || !s->left_node()->red) && s->right != 0 && s->right_node()->red) {
                s->red = true;
                RBTNode* sr = s->right_node();
                sr->red = false;
                rotate_left(sr, s, s->parent_node());
                std::swap(sr, s);
            }
        }
        s->red = p->red;
        p->red = false;
        if (n == p->left_node()) {
            s->right_node()->red = false;
            rotate_left(s, p, p->parent_node());
        } else {
            s->left_node()->red = false;
            rotate_right(s, p, p->parent_node());
        }
    }

    inline void rotate_left(RBTNode* n, RBTNode* p, RBTNode* gp) {
        RBTNode* tmp_left_n = n->left_node();
        if (gp != 0) {
            if (p == gp->left_node()) {
                gp->left = n;
            } else {
                gp->right = n;
            }
            n->parent = gp;
        } else {
            this->root = n;
            n->parent = 0;            
        }
        n->left = p; p->parent = n;
        p->right = tmp_left_n; 
        if (tmp_left_n != 0) tmp_left_n->parent = p;
    }

    inline void rotate_right(RBTNode* n, RBTNode* p, RBTNode* gp) {
        RBTNode* tmp_right_n = n->right_node();
        if(gp != 0) {
            if (p == gp->left_node()) {
                gp->left = n;
            } else {
                gp->right = n;
            }
            n->parent = gp;
        } else {
            this->root = n;
            n->parent = 0;            
        }
        n->right = p; p->parent = n;
        p->left = tmp_right_n; 
        if (tmp_right_n != 0) tmp_right_n->parent = p;
    }

};