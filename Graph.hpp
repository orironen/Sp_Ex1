#ifndef GRAPH_H
#define GRAPH_H
#include "data_structures.hpp"
using namespace structures;

namespace graph
{
    struct Edge
    {
        Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
        int src;
        int dest;
        int weight;
        bool operator<=(const Edge &other) const
        {
            return this->weight <= other.weight;
        }
        bool operator==(const Edge &other) const
        {
            return src == other.src && dest == other.dest && weight == other.weight;
        }
    };

    struct Vertex
    {
        bool visited;
        int parent;
        List<int> neighbors;
        Vertex() : visited(false), parent(-1) {}
    };

    class Graph
    {
    private:
        List<Edge> edges;
        List<Vertex> vertices;

    public:
        Graph(int verticesCount);
        ~Graph();
        // copy constructor
        Graph(const Graph &other);
        // assignment operator
        Graph &operator=(const Graph &other);
        // returns the total weight of the graph
        int getTotalWeight() const;
        List<Vertex> getVertices() const;
        List<Edge> getEdges() const;
        void addEdge(int src, int dest, int weight = 1);
        void removeEdge(int src, int dest);
        void printGraph();
        // returns the weight of the edge between two vertices
        int getWeight(int src, int dest);
    };
}

#endif
