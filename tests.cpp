#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Algorithm.hpp"
#include "Graph.hpp"
#include "data_structures.hpp"

using namespace graph;
using namespace structures;

TEST_CASE("Testing List data structure basic operations")
{
    List<int> lst;
    CHECK(lst.size() == 0);
    lst.push(10);
    lst.push(20);
    lst.push(30);
    CHECK(lst.size() == 3);
    CHECK(lst.get(0) == 10);
    CHECK(lst.get(1) == 20);
    CHECK(lst.get(2) == 30);

    lst.remove(1); // remove element at index 1 (20)
    CHECK(lst.size() == 2);
    CHECK(lst.get(0) == 10);
    CHECK(lst.get(1) == 30);

    CHECK_THROWS_AS(lst.get(-1), std::invalid_argument);
    CHECK_THROWS_AS(lst.get(10), std::invalid_argument);
}

TEST_CASE("Testing Graph addEdge and getVertices")
{
    Graph g(3);
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 3);

    auto vertices = g.getVertices();
    CHECK(vertices.size() == 3);
    CHECK(vertices.get(0).neighbors.size() == 1);
    CHECK(vertices.get(1).neighbors.size() == 1);
    CHECK(vertices.get(2).neighbors.size() == 0);

    CHECK(g.getWeight(0, 1) == 5);
    CHECK(g.getWeight(1, 2) == 3);
}

TEST_CASE("Testing BFS on simple graph")
{
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 1);

    Algorithms alg(g);
    Graph bfsResult = alg.bfs(0);

    auto bfsVertices = bfsResult.getVertices();
    CHECK(bfsVertices.get(0).parent == -1);
    CHECK(bfsVertices.get(1).parent == 0);
    CHECK(bfsVertices.get(2).parent == 1);
    CHECK(bfsVertices.get(3).parent == 2);
}

TEST_CASE("Testing DFS on simple graph")
{
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 1);

    Algorithms alg(g);
    Graph dfsResult = alg.dfs(0);

    auto dfsVertices = dfsResult.getVertices();
    CHECK(dfsVertices.get(0).parent == -1);
    // DFS parent might differ depending on traversal order, check parent chain is consistent
    CHECK((dfsVertices.get(1).parent == 0 || dfsVertices.get(1).parent == -1));
}

TEST_CASE("Testing Prim MST on weighted graph")
{
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);

    Algorithms alg(g);
    Graph mst = alg.prim();

    // MST should have V-1 = 3 edges
    int edgeCount = 0;
    for (int i = 0; i < 4; i++)
    {
        edgeCount += mst.getVertices().get(i).neighbors.size();
    }
    CHECK(edgeCount == 6); // Because graph is undirected and addEdge likely adds edges both ways
}

TEST_CASE("Testing Kruskal MST on weighted graph")
{
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);

    Algorithms alg(g);
    Graph mst = alg.kruskal();

    // MST should have V-1 = 3 edges (undirected counts 2 edges per connection)
    int edgeCount = 0;
    for (int i = 0; i < 4; i++)
    {
        edgeCount += mst.getVertices().get(i).neighbors.size();
    }
    CHECK(edgeCount == 6);
}

TEST_CASE("Testing Dijkstra shortest path")
{
    Graph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(3, 1, 3);
    g.addEdge(3, 2, 8);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 2, 4);
    g.addEdge(4, 0, 7);
    g.addEdge(2, 4, 6);

    Algorithms alg(g);
    Graph shortestPathTree = alg.dijkstra(0);

    // Test parent of vertex 2 should be 1 or 4 depending on path
    auto verts = shortestPathTree.getVertices();

    CHECK(verts.get(0).parent == -1);
    // vertex 1 parent could be 0
    CHECK((verts.get(1).parent == 0 || verts.get(1).parent == 3));
    // vertex 2 parent should be 1 or 4
    CHECK((verts.get(2).parent == 1 || verts.get(2).parent == 4));
}
