#include "Algorithm.hpp"
#include "Graph.hpp"
#include <iostream>

using namespace std;
using namespace graph;

int main()
{
    // יצירת גרף לדוגמה עם 6 קודקודים
    Graph g(6);

    // הוספת קשתות לגרף
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(2, 4, 5);
    g.addEdge(3, 4, 1);
    g.addEdge(3, 5, 6);
    g.addEdge(4, 5, 8);

    cout << "original:" << endl;
    g.printGraph();

    Algorithms alg(g);

    // BFS מקודקוד 0
    cout << "\nBFS reason " << endl;
    Graph bfsTree = alg.bfs(0);
    bfsTree.printGraph();

    // DFS מקודקוד 0
    cout << "\nDFS reason " << endl;
    Graph dfsTree = alg.dfs(0);
    dfsTree.printGraph();

    // אלגוריתם פרים
    cout << "\nprim reason:" << endl;
    Graph primMST = alg.prim();
    primMST.printGraph();

    // אלגוריתם קרוסקל
    cout << "\nKruskal reason:" << endl;
    Graph kruskalMST = alg.kruskal();
    kruskalMST.printGraph();

    // אלגוריתם דייקסטרה
    cout << "\nDijkstra reason:" << endl;
    Graph shortestPaths = alg.dijkstra(0);
    shortestPaths.printGraph();

    return 0;
}