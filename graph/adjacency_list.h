#pragma once
#include <single_list.h>
#include <debug_new.h>

struct graph_traits {
    struct oriented {
    };

    struct unoriented {
    };
};

template<class Orientation=graph_traits::unoriented>
class TAdjacencyListGraph {
private:
    typedef TSingleLinkedList<int> EdgeEndList;
    typedef TSingleLinkedList<EdgeEndList> EdgeStartList;
    typedef EdgeStartList::ForwardIterator EdgeStartIterator;
    typedef EdgeEndList::ForwardIterator EdgeEndIterator;

public:
    TAdjacencyListGraph(int _n_vertices) :
        n_vertices( _n_vertices )
    {
        edge_list.reserve(n_vertices);
    }

    int vertices() const { return n_vertices; }
    int edges() const { return n_edges; }

    bool add_edge(int i, int j) 
    {
        assert(i < n_vertices);
        assert(j < n_vertices);
        return add_edge(i, j, Orientation());
    }

private:
    int n_vertices;
    int n_edges;
    EdgeStartList edge_list;

    bool add_edge(int i, int j, const graph_traits::oriented&) 
    {
        if (add_edge_internal(i, j)) {
            ++n_edges;
            return true;
        }
        return false;
    }

    bool add_edge(int i, int j, const graph_traits::unoriented&) 
    {
        if (add_edge_internal(std::min(i, j), std::max(i, j))) {
            ++n_edges;
            return true;
        }
        return false;
    }

    bool add_edge_internal(int i, int j) 
    {
        int ni = 0;
        for (EdgeStartIterator it = edge_list.begin(); it != edge_list.end(); ++it, ++ni) {
            if (ni < i) {
                continue;
            }
            assert (ni == i);
            bool inserted = false;
            for (EdgeEndIterator it2 = it->begin(); it2 != it->end(); ++it2) {
                if (*it2 == j) {
                    return false;
                } else if (*it2 > j) {
                    it->insert(it2, j);
                    inserted = true;
                    assert(*it2 == j);
                    break;
                }
            }
            if (!inserted) {
                it->push_back(j);
            }
            return true;
        }
        assert(false);
        return true;
    }
};
