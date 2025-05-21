#include "Graph.hpp"
#include "data_structures.hpp"
#include <iostream>

using namespace graph;

Graph::Graph(int v)
{
    for (int i = 0; i < v; ++i)
    {
        Vertex ver;
        vertices.push(ver);
    }
}

Graph::~Graph() {}

// מימוש קונסטרקטור העתקה
Graph::Graph(const Graph &other)
{
    // העתקת כל הקודקודים
    for (int i = 0; i < other.vertices.size(); ++i)
    {
        vertices.push(other.vertices.get(i));
    }
    // העתקת כל הקשתות
    for (int i = 0; i < other.edges.size(); ++i)
    {
        edges.push(other.edges.get(i));
    }
}

// מימוש אופרטור השמה
Graph &Graph::operator=(const Graph &other)
{
    if (this != &other)
    {
        // יצירת רשימות חדשות
        List<Vertex> newVertices;
        List<Edge> newEdges;
        for (int i = 0; i < other.vertices.size(); ++i)
        {
            newVertices.push(other.vertices.get(i));
        }
        for (int i = 0; i < other.edges.size(); ++i)
        {
            newEdges.push(other.edges.get(i));
        }
        vertices = newVertices;
        edges = newEdges;
    }
    return *this;
}
int Graph::getTotalWeight() const
{
    int sum = 0;
    for (int i = 0; i < edges.size(); ++i)
    {
        sum += edges.get(i).weight;
    }
    return sum / 2;
}

List<Vertex> Graph::getVertices() const
{
    return vertices;
}

List<Edge> Graph::getEdges() const
{
    return edges;
}

void Graph::addEdge(int src, int dest, int weight)
{
    // בדיקה שהאינדקסים תקינים
    if (src < 0 || src >= vertices.size() || dest < 0 || dest >= vertices.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }

    Edge e(src, dest, weight);
    edges.push(e);
    vertices.getRef(src).neighbors.push(dest);
    vertices.getRef(dest).neighbors.push(src);
}

void Graph::removeEdge(int src, int dest)
{
    if (src < 0 || src >= vertices.size() || dest < 0 || dest >= vertices.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    for (int i = 0; i < edges.size(); ++i)
    {
        Edge e = edges.get(i);
        if ((e.src == src && e.dest == dest) || (e.src == dest && e.dest == src))
        {
            edges.remove(i);
            break;
        }
    }
    // הסרת השכנים
    Vertex &vsrc = vertices.getRef(src);
    for (int i = 0; i < vsrc.neighbors.size(); ++i)
    {
        if (vsrc.neighbors.get(i) == dest)
        {
            vsrc.neighbors.remove(i);
            break;
        }
    }
    Vertex &vdest = vertices.getRef(dest);
    for (int i = 0; i < vdest.neighbors.size(); ++i)
    {
        if (vdest.neighbors.get(i) == src)
        {
            vdest.neighbors.remove(i);
            break;
        }
    }
}

void Graph::printGraph()
{
    for (int i = 0; i < vertices.size(); ++i)
    {
        Vertex v = vertices.get(i);
        std::cout << "Vertex " << i << " neighbors: ";
        for (int j = 0; j < v.neighbors.size(); ++j)
        {
            std::cout << v.neighbors.get(j) << " ";
        }
        std::cout << std::endl;
    }
    int weight = getTotalWeight();
    std::cout << "the total weight is: " << weight << std::endl;
}

int Graph::getWeight(int src, int dest)
{
    for (int i = 0; i < edges.size(); ++i)
    {
        Edge e = edges.get(i);
        if ((e.src == src && e.dest == dest) || (e.src == dest && e.dest == src))
        {
            return e.weight;
        }
    }
    throw std::invalid_argument("No edge found between given vertices");
}