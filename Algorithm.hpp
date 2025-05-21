
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "data_structures.hpp"
#include "Graph.hpp"
using namespace structures;

namespace graph
{
    class Algorithms
    {
    private:
        Graph &g;

    public:
        Algorithms(Graph &g) : g(g) {}
        ~Algorithms();
        Graph bfs(int src);
        Graph dfs(int src);
        Graph dijkstra(int src);
        Graph prim();
        Graph kruskal();
        Graph buildGraph(List<Vertex> vertices);
    };
}
#endif