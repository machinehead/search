#pragma once
#include <binary_tree.h>

template<class Value>
class TRedBlackTree : public TBinaryTree<Value> {
public:
    virtual ~TRedBlackTree() 
    {
        clear(&this->root);
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

    virtual void clear(ParentNodeClass** _r)
    {
        RBTNode** r = reinterpret_cast<RBTNode**>(_r);
        if(*r != 0) {
            clear(&(*r)->left);
            clear(&(*r)->right);
            delete *r;
            *r = 0;
        }
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

private:
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