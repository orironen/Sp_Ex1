#include "Algorithm.hpp"
#include "Graph.hpp"
#include <iostream>

using namespace std;
using namespace graph;

int main()
{
    Graph g(6);
    g.addEdge(0, 1, 3);
    g.addEdge(1, 2, 5);
    g.addEdge(2, 3, 2);
    g.addEdge(3, 4, 4);
    g.addEdge(4, 5, 1);

    cout << "origin graph:" << endl;
    g.printGraph();

    Algorithms alg(g);

    // BFS
    cout << "\nBFS traversal:" << endl;
    Graph bfsTree = alg.bfs(0);
    bfsTree.printGraph();

    // DFS
    cout << "\nDFS traversal:" << endl;
    Graph dfsTree = alg.dfs(0);
    dfsTree.printGraph();

    // Prim algorithm
    cout << "\nPrim's algorithm result:" << endl;
    Graph primMST = alg.prim();
    primMST.printGraph();

    // Kruskal algorithm
    cout << "\nKruskal's algorithm result:" << endl;
    Graph kruskalMST = alg.kruskal();
    kruskalMST.printGraph();

    // Dijkstra algorithm
    cout << "\nDijkstra's algorithm result:" << endl;
    Graph shortestPaths = alg.dijkstra(0);
    shortestPaths.printGraph();

    return 0;
}
