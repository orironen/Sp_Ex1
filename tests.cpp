#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Algorithm.hpp"
#include "Graph.hpp"
#include "data_structures.hpp"

using namespace graph;
using namespace structures;

TEST_CASE("Testing List data structure")
{
    List<int> list;
    CHECK(list.size() == 0);
    list.push(1);
    list.push(2);
    list.push(3);
    CHECK(list.size() == 3);
    CHECK(list.get(1) == 2);
    list.add(1, 4);
    CHECK(list.get(1) == 4);
    CHECK(list.size() == 4);
    list.remove(1);
    CHECK(list.size() == 3);
    CHECK(list.get(0) == 1);
}

TEST_CASE("Testing Graph")
{
    Graph g(3);
    g.addEdge(0, 1, 8);
    g.addEdge(1, 2, 8);
    auto vertices = g.getVertices();
    CHECK(vertices.size() == 3);
    CHECK(vertices.get(0).neighbors.size() == 1);
    CHECK(vertices.get(1).neighbors.size() == 2);
    CHECK(g.getWeight(0, 1) == 8);
}

TEST_CASE("Testing BFS")
{
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 1);
    Algorithms alg(g);
    Graph bfs = alg.bfs(0);
    auto vertices = bfs.getVertices();
    CHECK(vertices.size() == 4);
}

TEST_CASE("Testing DFS on simple graph")
{
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 1);
    Algorithms alg(g);
    Graph dfsResult = alg.dfs(0);
    auto vertices = dfsResult.getVertices();
    CHECK(vertices.size() == 4);
}

TEST_CASE("Testing Prim")
{
    Graph g(3);
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 7);
    g.addEdge(0, 2, 10);
    Algorithms alg(g);
    Graph prim = alg.prim();
    auto edges = prim.getEdges();
    CHECK(edges.size() == 2); // MST should have 2 edges
}

TEST_CASE("Testing Kruskal")
{
    Graph g(3);
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 7);
    g.addEdge(0, 2, 10);
    Algorithms alg(g);
    Graph kruskal = alg.kruskal();
    auto edges = kruskal.getEdges();
    CHECK(edges.size() == 2); // MST should have 2 edges
}

TEST_CASE("Test total weight prim and kruskal")
{
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    Algorithms alg(g);
    Graph prim = alg.prim();
    Graph kruskal = alg.kruskal();
    CHECK(prim.getTotalWeight() == kruskal.getTotalWeight());
}

TEST_CASE("Testing Dijkstra")
{
    Graph g(4);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 2, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 7);
    Algorithms alg(g);
    Graph dijkstra = alg.dijkstra(0);
    auto vertices = dijkstra.getVertices();
    CHECK(g.getWeight(0, 1) == 2);
    int path = dijkstra.getWeight(0, 1) + dijkstra.getWeight(1, 2) + dijkstra.getWeight(2, 3);
    CHECK(path == 10);
}

TEST_CASE("Dijkstra throws on negative weights")
{
    Graph g(3);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, -5); // קשת שלילית
    g.addEdge(0, 2, 10);
    Algorithms alg(g);
    CHECK_THROWS_AS(alg.dijkstra(0), std::invalid_argument);
}