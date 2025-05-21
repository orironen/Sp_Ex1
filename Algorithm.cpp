
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
        int parent = vertices.getRef(i).parent;
        if (parent != -1) // בדיקה שההורה חוקי
        {
            res.addEdge(parent, i, 1);
        }
    }
    return res;
}

// תיקון לפונקציית bfs
Graph Algorithms::bfs(int src)
{
    if (src < 0 || src >= g.getVertices().size())
    {
        throw std::out_of_range("Source vertex index out of range");
    }

    List<Vertex> vertices = g.getVertices();

    // אתחול הקודקודים
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
    int V = g.getVertices().size();          // מספר הקודקודים בגרף
    List<Vertex> vertices = g.getVertices(); // העתקת רשימת הקודקודים
    List<Edge> mstEdges;                     // רשימת קשתות שייכנסו לעץ הפורש המינימלי

    Graph mst(V); // הגרף שיחזיק את ה-MST שנחזיר בסוף

    PriorityQueue<Edge> pq; // תור עדיפויות לשמירת הקשתות לפי משקל (קטן -> גדול)

    // מתחילים מהקודקוד הראשון (צומת 0)
    vertices.getRef(0).visited = true;

    // מוסיפים את כל הקשתות שיוצאות מצומת 0 לתור העדיפויות
    for (int j = 0; j < vertices.get(0).neighbors.size(); ++j)
    {
        int neighbor = vertices.get(0).neighbors.get(j);
        int weight = g.getWeight(0, neighbor);
        pq.insert(Edge(0, neighbor, weight));
    }

    int visitedCount = 1; // ספר כמה קודקודים כבר ב-MST

    // ממשיכים כל עוד יש קשתות בתור ועוד לא ביקרנו בכל הקודקודים
    while (!pq.isEmpty() && visitedCount < V)
    {
        Edge minEdge = pq.extractMin(); // נבחר את הקשת עם המשקל הקטן ביותר

        // אם שני הצמתים של הקשת כבר נמצאים ב-MST, נמשיך הלאה (למנוע מעגלים)
        if (vertices.get(minEdge.dest).visited && vertices.get(minEdge.src).visited)
            continue;

        // נבחר את הקודקוד שעדיין לא היה בתוך העץ
        int newVertex = vertices.get(minEdge.dest).visited ? minEdge.src : minEdge.dest;

        // מוסיפים את הקשת ל-MST, מסמנים את הקודקוד החדש כ"visited"
        mstEdges.push(minEdge);
        vertices.getRef(newVertex).visited = true;
        visitedCount++;

        // מוסיפים את כל הקשתות שיוצאות מהקודקוד החדש, אם הקצה השני עדיין לא ב-MST
        Vertex &v = vertices.getRef(newVertex);
        for (int i = 0; i < v.neighbors.size(); ++i)
        {
            int neighbor = v.neighbors.get(i);
            if (!vertices.get(neighbor).visited)
            {
                int weight = g.getWeight(newVertex, neighbor);
                pq.insert(Edge(newVertex, neighbor, weight));
            }
        }
    }

    // יוצרים את הגרף החדש עם כל הקשתות שנבחרו ל-MST
    for (int i = 0; i < mstEdges.size(); ++i)
    {
        Edge e = mstEdges.get(i);
        mst.addEdge(e.src, e.dest, e.weight);
    }

    return mst; // מחזירים את עץ הפרים שנבנה
}
Graph Algorithms::kruskal()
{
    List<Edge> edges = g.getEdges();
    List<Vertex> vertices = g.getVertices();

    Graph mst(vertices.size());

    // יצירת קבוצה נפרדת לכל צומת
    UnionFind uf(vertices.size());

    // שלב 2 מיון הקשתות לפי משקל מהקטן לגדול (פשוט לקריאה)
    PriorityQueue<Edge> sorted;
    for (int i = 0; i < edges.size(); ++i)
    {
        sorted.insert(edges.get(i));
    }

    // עבור על כל הקשתות מהמיון
    int edgesUsed = 0;
    while (!sorted.isEmpty())
    {
        Edge e = sorted.extractMin();
        ;
        // אם הצומת e.src והצומת e.dest בקבוצות שונות
        if (uf.find(e.src) != uf.find(e.dest))
        {
            uf.unionSets(e.src, e.dest);          // איחוד הקבוצות
            mst.addEdge(e.src, e.dest, e.weight); // הוספת הקשת לעץ הפורש
            edgesUsed++;
        }

        if (edgesUsed == vertices.size() - 1)
        {
            break;
        }
    }

    return mst;
}
Graph Algorithms::dijkstra(int src)
{
    List<Vertex> vertices = g.getVertices(); // כל הקודקודים בגרף
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
    // מחפשים את הקודקוד בעל ה-DIST המינימלי
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
            break; // אין יותר קודקודים נגישים

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
    // אתחול הגרף הסופי
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
