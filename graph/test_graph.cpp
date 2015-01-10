#include <measure.h>
#include "adjacency_list.h"

void test_adj_list_basic()
{
    TAdjacencyListGraph<> gr1(5);
    assert(gr1.vertices() == 5);
    assert(gr1.add_edge(0, 1));
    assert(!gr1.add_edge(0, 1));
    assert(!gr1.add_edge(1, 0));

    TAdjacencyListGraph<graph_traits::oriented> gr2(5);
    assert(gr2.vertices() == 5);
    assert(gr2.add_edge(0, 1));
    assert(!gr2.add_edge(0, 1));
    assert(gr2.add_edge(1, 0));
    assert(!gr2.add_edge(1, 0));
}

void create_clique_graph_unoriented(int n_vertices, int& edges)
{
    TAdjacencyListGraph<> graph(n_vertices);
    for(int i = 0; i < n_vertices; ++i) {
        for(int j = n_vertices - 1; j >= i; --j) {
            assert(graph.add_edge(i, j));
        }
    }
    edges = graph.edges();
}

void create_clique_graph_oriented(int n_vertices, int& edges)
{
    TAdjacencyListGraph<graph_traits::oriented> graph(n_vertices);
    for(int i = 0; i < n_vertices; ++i) {
        for(int j = n_vertices - 1; j >= 0; --j) {
            assert(graph.add_edge(i, j));
        }
    }
    edges = graph.edges();
}

void measure_clique_graph(int n_vertices)
{
    int edges;
    std::cout << "Creating unoriented clique graph with " 
              << n_vertices 
              << " vertices takes " 
              << Nstd::measure<>::execution(create_clique_graph_unoriented, n_vertices, edges) 
              << "us" 
              << std::endl;
    std::cout << "Resulting graph has "
              << edges
              << " edges"
              << std::endl;
    std::cout << "Creating oriented clique graph with " 
              << n_vertices 
              << " vertices takes " 
              << Nstd::measure<>::execution(create_clique_graph_oriented, n_vertices, edges) 
              << "us" 
              << std::endl;
    std::cout << "Resulting graph has "
              << edges
              << " edges"
              << std::endl;
}

int main(int argc, char* argv[])
{
    test_adj_list_basic();
    measure_clique_graph(100);
    measure_clique_graph(200);
    measure_clique_graph(400);
    measure_clique_graph(800);
    return 0;
}