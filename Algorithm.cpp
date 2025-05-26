#include "Algorithm.hpp"
using namespace graph;
#include <iostream>
#include <climits>
#include <stdexcept>

Algorithms::~Algorithms() {}
Graph Algorithms::buildGraph(List<Vertex> vertices)
{
    Graph res(vertices.size());
    for (int i = 0; i < vertices.size(); ++i)
    {
        int parent = vertices.get(i).parent;
        if (parent != -1) // check that the parent is valid
        {
            res.addEdge(parent, i, 1);
        }
    }
    return res;
}

Graph Algorithms::bfs(int src)
{
    if (src < 0 || src >= g.getVertices().size())
    {
        throw std::out_of_range("Source vertex index out of range");
    }
    List<Vertex> vertices = g.getVertices();
    for (int i = 0; i < vertices.size(); ++i)
    {
        vertices.getRef(i).visited = false;
        vertices.getRef(i).parent = -1;
    }
    Queue<int> q;
    vertices.getRef(src).visited = true;
    q.enqueue(src);
    while (!q.isEmpty())
    {
        int curr = q.dequeue();
        if (curr < 0 || curr >= vertices.size())
        {
            throw std::out_of_range("Invalid vertex index: " + std::to_string(curr));
        }
        Vertex &v = vertices.getRef(curr);
        for (int i = 0; i < v.neighbors.size(); ++i)
        {
            int neighbor = v.neighbors.get(i);
            if (neighbor < 0 || neighbor >= vertices.size())
            {
                throw std::out_of_range("Invalid neighbor index: " + std::to_string(neighbor));
            }
            Vertex &n = vertices.getRef(neighbor);
            if (!n.visited)
            {
                n.visited = true;
                n.parent = curr;
                q.enqueue(neighbor);
            }
        }
    }
    return buildGraph(vertices);
}

Graph Algorithms::dfs(int src)
{
    if (src < 0 || src >= g.getVertices().size())
    {
        throw std::out_of_range("Source vertex index out of range");
    }

    List<Vertex> vertices = g.getVertices();
    for (int i = 0; i < vertices.size(); ++i)
    {
        vertices.getRef(i).visited = false;
        vertices.getRef(i).parent = -1;
    }
    List<int> stack;
    stack.push(src);
    vertices.getRef(src).visited = true;
    while (!stack.isEmpty())
    {
        int ind = stack.size() - 1;
        int curr = stack.get(ind);
        stack.remove(ind); // pop

        if (curr < 0 || curr >= vertices.size())
        {
            throw std::out_of_range("Invalid vertex index: " + std::to_string(curr));
        }
        Vertex &v = vertices.getRef(curr);
        for (int i = 0; i < v.neighbors.size(); ++i)
        {
            int neighbor = v.neighbors.get(i);
            if (neighbor < 0 || neighbor >= vertices.size())
            {
                throw std::out_of_range("Invalid neighbor index: " + std::to_string(neighbor));
            }

            Vertex &n = vertices.getRef(neighbor);
            if (!n.visited)
            {
                n.visited = true;
                n.parent = curr;
                stack.push(neighbor);
            }
        }
    }
    return buildGraph(vertices);
}

Graph graph::Algorithms::prim()
{
    int V = g.getVertices().size();
    List<Vertex> vertices = g.getVertices();
    // Edges of the minimum spanning tree
    List<Edge> mstEdges;
    Graph mst(V);
    PriorityQueue<Edge> pq; // Priority queue
    // start from the first vertex (node 0)
    vertices.getRef(0).visited = true;
    // add all edges from source vertex's neighbors to priority queue
    for (int j = 0; j < vertices.get(0).neighbors.size(); ++j)
    {
        int neighbor = vertices.get(0).neighbors.get(j);
        int weight = g.getWeight(0, neighbor);
        pq.insert(Edge(0, neighbor, weight));
    }
    int visitedCount = 1; // count how many vertices we have visited
    // continue as long as there are edges in the queue and not all vertices are visited
    while (!pq.isEmpty() && visitedCount < V)
    {
        Edge minEdge = pq.extractMin();
        // if both vertices of the edge are already visited – skip
        if (vertices.get(minEdge.dest).visited && vertices.get(minEdge.src).visited)
            continue;
        // choose the vertex that was not visited yet
        int newV;
        if (vertices.get(minEdge.dest).visited)
            newV = minEdge.src;
        else
            newV = minEdge.dest;
        mstEdges.push(minEdge);
        vertices.getRef(newV).visited = true;
        visitedCount++;
        // add to the queue all edges from the new vertex
        Vertex &v = vertices.getRef(newV);
        for (int i = 0; i < v.neighbors.size(); ++i)
        {
            int neighbor = v.neighbors.get(i);
            if (!vertices.get(neighbor).visited)
            {
                int weight = g.getWeight(newV, neighbor);
                pq.insert(Edge(newV, neighbor, weight));
            }
        }
    }
    // buildGraph
    for (int i = 0; i < mstEdges.size(); ++i)
    {
        Edge e = mstEdges.get(i);
        mst.addEdge(e.src, e.dest, e.weight);
    }

    return mst;
}

Graph Algorithms::kruskal()
{
    List<Edge> edges = g.getEdges();
    List<Vertex> vertices = g.getVertices();

    Graph mst(vertices.size());
    // create a separate set for each vertex
    UnionFind uf(vertices.size());
    // sort the edges by weight
    PriorityQueue<Edge> sorted;
    for (int i = 0; i < edges.size(); ++i)
    {
        sorted.insert(edges.get(i));
    }

    // go over all the sorted edges
    int used = 0;
    while (!sorted.isEmpty())
    {
        // take the minimum edge
        Edge e = sorted.extractMin();
        // check if the vertices are in different sets
        if (uf.find(e.src) != uf.find(e.dest))
        {
            uf.unionSets(e.src, e.dest);          // Merge the sets
            mst.addEdge(e.src, e.dest, e.weight); // Add the edge to the MST
            used++;
        }
        // a tree with n vertices has exactly n-1 edges
        if (used == vertices.size() - 1)
        {
            break;
        }
    }
    return mst;
}

Graph Algorithms::dijkstra(int src)
{
    if (src < 0 || src >= g.getVertices().size())
    {
        throw std::out_of_range("Source vertex index out of range");
    }
    // check if the graph contains any negative edge weights
    for (int i = 0; i < g.getEdges().size(); ++i)
    {
        if (g.getEdges().get(i).weight < 0)
        {
            throw std::invalid_argument("Dijkstra's algorithm does not support graphs with negative edge weights. Consider using Bellman-Ford for such graphs.");
        }
    }
    List<Vertex> vertices = g.getVertices();
    List<Edge> edges = g.getEdges();
    int size = vertices.size();
    List<int> dist;
    for (int i = 0; i < size; ++i)
    {
        vertices.getRef(i).visited = false;
        vertices.getRef(i).parent = -1;
        dist.push(INT_MAX);
    }
    dist.set(src, 0);
    // choose the vertex with the smallest distance that hasn’t been visited yet
    for (int i = 0; i < size; ++i)
    {
        int v = -1;
        int minDist = INT_MAX;
        for (int j = 0; j < size; ++j)
        {
            if (!vertices.get(j).visited && dist.get(j) < minDist)
            {
                minDist = dist.get(j);
                v = j;
            }
        }
        if (v == -1)
            break; // no more reachable vertices
        vertices.getRef(v).visited = true;
        // relax
        List<int> neighbors = vertices.getRef(v).neighbors;
        for (int i = 0; i < neighbors.size(); ++i)
        {
            int n = neighbors.get(i);
            int weight = g.getWeight(v, n);
            if (dist.get(v) + weight < dist.get(n))
            {
                dist.set(n, dist.get(v) + weight);
                vertices.getRef(n).parent = v;
            }
        }
    }
    // buildGraph
    Graph res(vertices.size());
    for (int i = 0; i < size; ++i)
    {
        int p = vertices.get(i).parent;
        if (p != -1)
        {
            int weight = g.getWeight(p, i);
            res.addEdge(p, i, weight);
        }
    }
    return res;
}
