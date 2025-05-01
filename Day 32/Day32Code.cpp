#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>
#include <functional>
#include <limits>
#include <random>
#include <fstream>
#include <memory>
#include <cmath>

/**
 * @brief Comprehensive Minimum Spanning Tree Implementation for #DSAin45 Day 32
 *
 * This file provides implementations of:
 * - Prim's Algorithm
 * - Kruskal's Algorithm
 * - Borůvka's Algorithm
 * - Reverse-Delete Algorithm
 * - Various optimizations and applications
 * - Performance benchmarks
 *
 * @author #DSAin45 Course
 * @date Day 32 of 45-Day DSA Challenge
 */

/****************************************************************************************
 * GRAPH INTERFACE AND IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Interface for graph implementations
 */
class IGraph
{
public:
    // Graph characteristics
    virtual bool isDirected() const = 0;
    virtual bool isWeighted() const = 0;

    // Basic information
    virtual int getVertexCount() const = 0;
    virtual int getEdgeCount() const = 0;

    // Vertex operations
    virtual bool hasVertex(int v) const = 0;
    virtual bool addVertex(int v) = 0;
    virtual bool removeVertex(int v) = 0;
    virtual std::vector<int> getVertices() const = 0;

    // Edge operations
    virtual bool hasEdge(int from, int to) const = 0;
    virtual bool addEdge(int from, int to, double weight = 1.0) = 0;
    virtual bool removeEdge(int from, int to) = 0;
    virtual double getEdgeWeight(int from, int to) const = 0;
    virtual bool setEdgeWeight(int from, int to, double weight) = 0;

    // Neighbor operations
    virtual std::vector<int> getNeighbors(int v) const = 0;
    virtual int getInDegree(int v) const = 0;
    virtual int getOutDegree(int v) const = 0;

    // Utility functions
    virtual void clear() = 0;
    virtual std::string toString() const = 0;

    // Virtual destructor
    virtual ~IGraph() {}
};

/**
 * @brief Graph implementation using adjacency list representation
 */
class AdjacencyListGraph : public IGraph
{
private:
    struct Edge
    {
        int to;        // Destination vertex
        double weight; // Edge weight

        Edge(int t, double w) : to(t), weight(w) {}

        bool operator==(const Edge &other) const
        {
            return to == other.to;
        }
    };

    std::unordered_map<int, std::vector<Edge>> outEdges; // Outgoing edges for each vertex
    std::unordered_map<int, std::vector<Edge>> inEdges;  // Incoming edges for each vertex (for directed graphs)
    bool directed;                                       // Whether the graph is directed
    bool weighted;                                       // Whether the graph is weighted
    int edgeCount;                                       // Number of edges

public:
    /**
     * @brief Constructor
     *
     * @param isDirected Whether the graph is directed
     * @param isWeighted Whether the graph is weighted
     */
    AdjacencyListGraph(bool isDirected = false, bool isWeighted = true)
        : directed(isDirected), weighted(isWeighted), edgeCount(0) {}

    /**
     * @brief Check if the graph is directed
     */
    bool isDirected() const override
    {
        return directed;
    }

    /**
     * @brief Check if the graph is weighted
     */
    bool isWeighted() const override
    {
        return weighted;
    }

    /**
     * @brief Get the number of vertices in the graph
     */
    int getVertexCount() const override
    {
        return static_cast<int>(outEdges.size());
    }

    /**
     * @brief Get the number of edges in the graph
     */
    int getEdgeCount() const override
    {
        return edgeCount;
    }

    /**
     * @brief Check if a vertex exists in the graph
     */
    bool hasVertex(int v) const override
    {
        return outEdges.find(v) != outEdges.end();
    }

    /**
     * @brief Add a vertex to the graph
     */
    bool addVertex(int v) override
    {
        if (hasVertex(v))
        {
            return false; // Vertex already exists
        }

        outEdges[v] = std::vector<Edge>();
        if (directed)
        {
            inEdges[v] = std::vector<Edge>();
        }

        return true;
    }

    /**
     * @brief Remove a vertex from the graph
     */
    bool removeVertex(int v) override
    {
        if (!hasVertex(v))
        {
            return false; // Vertex doesn't exist
        }

        // Remove edges to this vertex from other vertices
        if (directed)
        {
            for (const auto &edge : inEdges[v])
            {
                // For each incoming edge, find and remove the outgoing edge from the source
                auto &sourceOutEdges = outEdges[edge.to];
                for (auto it = sourceOutEdges.begin(); it != sourceOutEdges.end(); ++it)
                {
                    if (it->to == v)
                    {
                        sourceOutEdges.erase(it);
                        break;
                    }
                }
            }

            // Update edge count
            edgeCount -= static_cast<int>(inEdges[v].size() + outEdges[v].size());

            // Remove the vertex
            inEdges.erase(v);
        }
        else
        {
            // For undirected graphs, remove all edges to/from this vertex
            for (const auto &edge : outEdges[v])
            {
                auto &targetOutEdges = outEdges[edge.to];
                for (auto it = targetOutEdges.begin(); it != targetOutEdges.end(); ++it)
                {
                    if (it->to == v)
                    {
                        targetOutEdges.erase(it);
                        break;
                    }
                }
            }

            // Update edge count
            edgeCount -= static_cast<int>(outEdges[v].size());
        }

        outEdges.erase(v);
        return true;
    }

    /**
     * @brief Get all vertices in the graph
     */
    std::vector<int> getVertices() const override
    {
        std::vector<int> vertices;
        vertices.reserve(outEdges.size());

        for (const auto &entry : outEdges)
        {
            vertices.push_back(entry.first);
        }

        return vertices;
    }

    /**
     * @brief Check if an edge exists between two vertices
     */
    bool hasEdge(int from, int to) const override
    {
        if (!hasVertex(from) || !hasVertex(to))
        {
            return false;
        }

        const auto &edges = outEdges.at(from);
        for (const auto &edge : edges)
        {
            if (edge.to == to)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Add an edge between two vertices
     */
    bool addEdge(int from, int to, double weight = 1.0) override
    {
        if (!hasVertex(from) || !hasVertex(to))
        {
            return false;
        }

        if (!weighted && weight != 1.0)
        {
            weight = 1.0; // Ignore weights for unweighted graphs
        }

        // Check if edge already exists
        for (const auto &edge : outEdges[from])
        {
            if (edge.to == to)
            {
                return false; // Edge already exists
            }
        }

        // Add edge
        outEdges[from].push_back(Edge(to, weight));

        if (directed)
        {
            inEdges[to].push_back(Edge(from, weight));
        }
        else if (from != to)
        {
            // For undirected graph, add the reverse edge (unless it's a self-loop)
            outEdges[to].push_back(Edge(from, weight));
        }

        edgeCount++;
        return true;
    }

    /**
     * @brief Remove an edge between two vertices
     */
    bool removeEdge(int from, int to) override
    {
        if (!hasVertex(from) || !hasVertex(to))
        {
            return false;
        }

        // Find and remove the edge in the outgoing edges list
        auto &fromEdges = outEdges[from];
        auto it = std::find_if(fromEdges.begin(), fromEdges.end(),
                               [to](const Edge &e)
                               { return e.to == to; });

        if (it == fromEdges.end())
        {
            return false; // Edge doesn't exist
        }

        fromEdges.erase(it);

        if (directed)
        {
            // Find and remove the edge in the incoming edges list
            auto &toInEdges = inEdges[to];
            auto inIt = std::find_if(toInEdges.begin(), toInEdges.end(),
                                     [from](const Edge &e)
                                     { return e.to == from; });

            if (inIt != toInEdges.end())
            {
                toInEdges.erase(inIt);
            }
        }
        else if (from != to)
        {
            // For undirected graph, remove the reverse edge (unless it's a self-loop)
            auto &toEdges = outEdges[to];
            auto revIt = std::find_if(toEdges.begin(), toEdges.end(),
                                      [from](const Edge &e)
                                      { return e.to == from; });

            if (revIt != toEdges.end())
            {
                toEdges.erase(revIt);
            }
        }

        edgeCount--;
        return true;
    }

    /**
     * @brief Get the weight of an edge
     */
    double getEdgeWeight(int from, int to) const override
    {
        if (!hasVertex(from) || !hasVertex(to))
        {
            return std::numeric_limits<double>::infinity();
        }

        const auto &edges = outEdges.at(from);
        for (const auto &edge : edges)
        {
            if (edge.to == to)
            {
                return edge.weight;
            }
        }

        return std::numeric_limits<double>::infinity(); // Edge doesn't exist
    }

    /**
     * @brief Set the weight of an existing edge
     */
    bool setEdgeWeight(int from, int to, double weight) override
    {
        if (!weighted)
        {
            return false; // Can't set weights for unweighted graphs
        }

        if (!hasVertex(from) || !hasVertex(to))
        {
            return false;
        }

        // Find and update the outgoing edge
        auto &fromEdges = outEdges[from];
        auto it = std::find_if(fromEdges.begin(), fromEdges.end(),
                               [to](const Edge &e)
                               { return e.to == to; });

        if (it == fromEdges.end())
        {
            return false; // Edge doesn't exist
        }

        it->weight = weight;

        if (directed)
        {
            // Update the incoming edge as well
            auto &toInEdges = inEdges[to];
            auto inIt = std::find_if(toInEdges.begin(), toInEdges.end(),
                                     [from](const Edge &e)
                                     { return e.to == from; });

            if (inIt != toInEdges.end())
            {
                inIt->weight = weight;
            }
        }
        else if (from != to)
        {
            // For undirected graph, update the reverse edge (unless it's a self-loop)
            auto &toEdges = outEdges[to];
            auto revIt = std::find_if(toEdges.begin(), toEdges.end(),
                                      [from](const Edge &e)
                                      { return e.to == from; });

            if (revIt != toEdges.end())
            {
                revIt->weight = weight;
            }
        }

        return true;
    }

    /**
     * @brief Get all neighbors of a vertex
     */
    std::vector<int> getNeighbors(int v) const override
    {
        if (!hasVertex(v))
        {
            return {};
        }

        std::vector<int> neighbors;
        neighbors.reserve(outEdges.at(v).size());

        for (const auto &edge : outEdges.at(v))
        {
            neighbors.push_back(edge.to);
        }

        return neighbors;
    }

    /**
     * @brief Get the in-degree of a vertex
     */
    int getInDegree(int v) const override
    {
        if (!hasVertex(v))
        {
            return -1;
        }

        if (directed)
        {
            return static_cast<int>(inEdges.at(v).size());
        }
        else
        {
            // For undirected graphs, in-degree equals out-degree
            return static_cast<int>(outEdges.at(v).size());
        }
    }

    /**
     * @brief Get the out-degree of a vertex
     */
    int getOutDegree(int v) const override
    {
        if (!hasVertex(v))
        {
            return -1;
        }

        return static_cast<int>(outEdges.at(v).size());
    }

    /**
     * @brief Clear all vertices and edges from the graph
     */
    void clear() override
    {
        outEdges.clear();
        inEdges.clear();
        edgeCount = 0;
    }

    /**
     * @brief Get string representation of the graph
     */
    std::string toString() const override
    {
        std::ostringstream oss;
        std::vector<int> vertices = getVertices();
        std::sort(vertices.begin(), vertices.end());

        oss << "Adjacency List Graph (" << (directed ? "directed" : "undirected")
            << ", " << (weighted ? "weighted" : "unweighted") << "):\n";
        oss << "Vertices: " << vertices.size() << ", Edges: " << edgeCount << "\n\n";

        for (int v : vertices)
        {
            oss << v << " -> ";
            const auto &edges = outEdges.at(v);

            if (edges.empty())
            {
                oss << "[]\n";
                continue;
            }

            oss << "[ ";
            for (size_t i = 0; i < edges.size(); i++)
            {
                if (weighted)
                {
                    oss << edges[i].to << "(" << edges[i].weight << ")";
                }
                else
                {
                    oss << edges[i].to;
                }

                if (i < edges.size() - 1)
                {
                    oss << ", ";
                }
            }
            oss << " ]\n";
        }

        return oss.str();
    }

    /**
     * @brief Get all edges in the graph
     *
     * @return Vector of (source, destination, weight) tuples
     */
    std::vector<std::tuple<int, int, double>> getAllEdges() const
    {
        std::vector<std::tuple<int, int, double>> edges;

        for (const auto &[from, edgeList] : outEdges)
        {
            for (const auto &edge : edgeList)
            {
                // For undirected graphs, only add each edge once
                if (!directed || from <= edge.to)
                {
                    edges.push_back({from, edge.to, edge.weight});
                }
            }
        }

        return edges;
    }
};

/****************************************************************************************
 * DISJOINT-SET (UNION-FIND) DATA STRUCTURE
 ****************************************************************************************/

/**
 * @brief Disjoint-Set (Union-Find) data structure
 *
 * Used to efficiently determine if adding an edge would create a cycle.
 * Implemented with path compression and union by rank optimizations.
 */
class DisjointSet
{
private:
    std::unordered_map<int, int> parent; // Maps vertex to its parent
    std::unordered_map<int, int> rank;   // Maps vertex to its rank (approx. height)

public:
    /**
     * @brief Constructor
     *
     * @param vertices List of vertices to initialize
     */
    DisjointSet(const std::vector<int> &vertices)
    {
        // Initialize each vertex as a separate set
        for (int v : vertices)
        {
            makeSet(v);
        }
    }

    /**
     * @brief Create a new set with a single element
     *
     * @param v Vertex to create a set for
     */
    void makeSet(int v)
    {
        parent[v] = v; // Each vertex is its own parent initially
        rank[v] = 0;   // Initial rank is 0
    }

    /**
     * @brief Find the representative (root) of a set
     *
     * @param v Vertex to find the representative for
     * @return Representative of the set containing v
     */
    int findSet(int v)
    {
        if (parent[v] != v)
        {
            // Path compression: make all nodes on the path point directly to the root
            parent[v] = findSet(parent[v]);
        }
        return parent[v];
    }

    /**
     * @brief Merge two sets
     *
     * @param u First vertex
     * @param v Second vertex
     */
    void unionSets(int u, int v)
    {
        int rootU = findSet(u);
        int rootV = findSet(v);

        if (rootU != rootV)
        {
            // Union by rank: attach the shorter tree to the root of the taller tree
            if (rank[rootU] < rank[rootV])
            {
                parent[rootU] = rootV;
            }
            else if (rank[rootU] > rank[rootV])
            {
                parent[rootV] = rootU;
            }
            else
            {
                parent[rootV] = rootU;
                rank[rootU]++; // Increase the rank of rootU if both trees have the same rank
            }
        }
    }

    /**
     * @brief Check if two vertices are in the same set
     *
     * @param u First vertex
     * @param v Second vertex
     * @return True if u and v are in the same set, false otherwise
     */
    bool sameSet(int u, int v)
    {
        return findSet(u) == findSet(v);
    }
};

/****************************************************************************************
 * MINIMUM SPANNING TREE ALGORITHMS
 ****************************************************************************************/

/**
 * @brief Implementation of Prim's algorithm for finding the MST
 *
 * Prim's algorithm starts with a single vertex and grows the MST one vertex at a time.
 * It always adds the lowest-weight edge that connects a vertex in the MST to a vertex outside the MST.
 *
 * Time Complexity: O(E log V) with a binary heap
 * Space Complexity: O(V + E)
 *
 * @param graph The graph to find the MST for
 * @param totalWeight Output parameter for the total weight of the MST
 * @return Vector of (source, destination) pairs representing MST edges
 */
std::vector<std::pair<int, int>> primMST(const IGraph &graph, double &totalWeight)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Initialize result vector to store MST edges
    std::vector<std::pair<int, int>> mstEdges;

    // Initialize set to keep track of vertices in MST
    std::unordered_set<int> inMST;

    // Start with the first vertex
    int startVertex = vertices[0];
    inMST.insert(startVertex);

    // Initialize totalWeight
    totalWeight = 0.0;

    // Priority queue to store edges: (weight, (from, to))
    std::priority_queue<std::tuple<double, int, int>,
                        std::vector<std::tuple<double, int, int>>,
                        std::greater<std::tuple<double, int, int>>>
        pq;

    // Add all edges from startVertex to the priority queue
    for (int neighbor : graph.getNeighbors(startVertex))
    {
        double weight = graph.getEdgeWeight(startVertex, neighbor);
        pq.push({weight, startVertex, neighbor});
    }

    // Process until all vertices are included in MST
    while (!pq.empty() && inMST.size() < n)
    {
        // Get the minimum weight edge
        auto [weight, from, to] = pq.top();
        pq.pop();

        // If the destination vertex is already in MST, skip
        if (inMST.find(to) != inMST.end())
        {
            continue;
        }

        // Add the vertex to MST
        inMST.insert(to);

        // Add the edge to MST
        mstEdges.push_back({from, to});

        // Update total weight
        totalWeight += weight;

        // Add all edges from the newly added vertex
        for (int neighbor : graph.getNeighbors(to))
        {
            // Only consider vertices not yet in MST
            if (inMST.find(neighbor) == inMST.end())
            {
                double weight = graph.getEdgeWeight(to, neighbor);
                pq.push({weight, to, neighbor});
            }
        }
    }

    // Check if we have a spanning tree
    if (inMST.size() != n)
    {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}

/**
 * @brief Eager implementation of Prim's algorithm
 *
 * This implementation maintains a priority queue of vertices instead of edges,
 * which can be more efficient for dense graphs.
 *
 * Time Complexity: O(E log V) with a binary heap
 * Space Complexity: O(V)
 *
 * @param graph The graph to find the MST for
 * @param totalWeight Output parameter for the total weight of the MST
 * @return Vector of (source, destination) pairs representing MST edges
 */
std::vector<std::pair<int, int>> primMSTEager(const IGraph &graph, double &totalWeight)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Initialize result vector to store MST edges
    std::vector<std::pair<int, int>> mstEdges;

    // Initialize array to track vertices in MST
    std::unordered_set<int> inMST;

    // Map to store key (minimum weight edge) for each vertex
    std::unordered_map<int, double> key;

    // Map to store parent of each vertex in MST
    std::unordered_map<int, int> parent;

    // Initialize totalWeight
    totalWeight = 0.0;

    // Initialize key values and parent
    for (int v : vertices)
    {
        key[v] = std::numeric_limits<double>::infinity();
        parent[v] = -1;
    }

    // Start with the first vertex
    int startVertex = vertices[0];
    key[startVertex] = 0.0;

    // Priority queue to store vertices: (key, vertex)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        pq;

    // Add all vertices to priority queue
    for (int v : vertices)
    {
        pq.push({key[v], v});
    }

    // Process until priority queue is empty
    while (!pq.empty())
    {
        // Get vertex with minimum key
        auto [keyValue, u] = pq.top();
        pq.pop();

        // Skip if already processed
        if (inMST.find(u) != inMST.end())
        {
            continue;
        }

        // Add to MST
        inMST.insert(u);

        // Add edge to MST if not the start vertex
        if (parent[u] != -1)
        {
            mstEdges.push_back({parent[u], u});
            totalWeight += keyValue;
        }

        // Update key values of adjacent vertices
        for (int v : graph.getNeighbors(u))
        {
            double weight = graph.getEdgeWeight(u, v);

            // Only update key if vertex is not in MST and weight is smaller
            if (inMST.find(v) == inMST.end() && weight < key[v])
            {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    // Check if we have a spanning tree
    if (inMST.size() != n)
    {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}

/**
 * @brief Implementation of Kruskal's algorithm for finding the MST
 *
 * Kruskal's algorithm sorts all edges by weight and then iteratively adds edges
 * to the MST as long as they don't create a cycle.
 *
 * Time Complexity: O(E log E) or O(E log V) since E is at most V²
 * Space Complexity: O(V + E)
 *
 * @param graph The graph to find the MST for
 * @param totalWeight Output parameter for the total weight of the MST
 * @return Vector of (source, destination) pairs representing MST edges
 */
std::vector<std::pair<int, int>> kruskalMST(const IGraph &graph, double &totalWeight)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Initialize result vector to store MST edges
    std::vector<std::pair<int, int>> mstEdges;

    // Initialize totalWeight
    totalWeight = 0.0;

    // Get all edges from the graph
    std::vector<std::tuple<double, int, int>> edges;

    for (int u : vertices)
    {
        for (int v : graph.getNeighbors(u))
        {
            // For undirected graphs, add each edge only once
            if (!graph.isDirected() && u < v)
            {
                double weight = graph.getEdgeWeight(u, v);
                edges.push_back({weight, u, v});
            }
            else if (graph.isDirected())
            {
                double weight = graph.getEdgeWeight(u, v);
                edges.push_back({weight, u, v});
            }
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    // Initialize disjoint-set data structure
    DisjointSet ds(vertices);

    // Process edges in order of increasing weight
    for (const auto &[weight, u, v] : edges)
    {
        // If adding this edge doesn't create a cycle, add it to the MST
        if (!ds.sameSet(u, v))
        {
            ds.unionSets(u, v);
            mstEdges.push_back({u, v});
            totalWeight += weight;

            // Early termination: MST has n-1 edges
            if (mstEdges.size() == n - 1)
            {
                break;
            }
        }
    }

    // Check if we have a spanning tree
    if (mstEdges.size() != n - 1)
    {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}

/**
 * @brief Implementation of Borůvka's algorithm for finding the MST
 *
 * Borůvka's algorithm works by repeatedly finding the minimum-weight edge
 * out of each connected component.
 *
 * Time Complexity: O(E log V)
 * Space Complexity: O(V + E)
 *
 * @param graph The graph to find the MST for
 * @param totalWeight Output parameter for the total weight of the MST
 * @return Vector of (source, destination) pairs representing MST edges
 */
std::vector<std::pair<int, int>> boruvkaMST(const IGraph &graph, double &totalWeight)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Initialize result vector to store MST edges
    std::vector<std::pair<int, int>> mstEdges;

    // Initialize totalWeight
    totalWeight = 0.0;

    // Initialize disjoint-set data structure
    DisjointSet ds(vertices);

    // Continue until we have a single connected component
    bool somethingAdded = true;
    while (somethingAdded && mstEdges.size() < n - 1)
    {
        somethingAdded = false;

        // For each component, find the lightest edge connecting it to another component
        std::unordered_map<int, std::tuple<double, int, int>> minEdges;

        // For each vertex
        for (int u : vertices)
        {
            int setU = ds.findSet(u);

            // For each of its neighbors
            for (int v : graph.getNeighbors(u))
            {
                int setV = ds.findSet(v);

                // Skip if both vertices are in the same component
                if (setU == setV)
                {
                    continue;
                }

                double weight = graph.getEdgeWeight(u, v);

                // Update minimum edge for component setU if needed
                if (minEdges.find(setU) == minEdges.end() ||
                    std::get<0>(minEdges[setU]) > weight)
                {
                    minEdges[setU] = {weight, u, v};
                }
            }
        }

        // Add all minimum edges to the MST
        for (const auto &[set, edge] : minEdges)
        {
            auto [weight, u, v] = edge;

            // If adding this edge doesn't create a cycle, add it to the MST
            if (!ds.sameSet(u, v))
            {
                ds.unionSets(u, v);
                mstEdges.push_back({u, v});
                totalWeight += weight;
                somethingAdded = true;
            }
        }
    }

    // Check if we have a spanning tree
    if (mstEdges.size() != n - 1)
    {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}

/**
 * @brief Helper function to check if a graph is connected
 *
 * @param graph The graph to check
 * @return True if the graph is connected, false otherwise
 */
bool isConnected(const IGraph &graph)
{
    std::vector<int> vertices = graph.getVertices();
    if (vertices.empty())
    {
        return true;
    }

    std::unordered_set<int> visited;
    std::queue<int> queue;

    // Start BFS from the first vertex
    int start = vertices[0];
    visited.insert(start);
    queue.push(start);

    while (!queue.empty())
    {
        int current = queue.front();
        queue.pop();

        for (int neighbor : graph.getNeighbors(current))
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }

    // Check if all vertices are visited
    return visited.size() == vertices.size();
}

/**
 * @brief Implementation of the Reverse-Delete algorithm for finding the MST
 *
 * The Reverse-Delete algorithm starts with all edges and removes them in order of
 * decreasing weight, as long as removing the edge doesn't disconnect the graph.
 *
 * Time Complexity: O(E log E) for sorting, O(E(V+E)) for connectivity checks
 * Space Complexity: O(V + E)
 *
 * @param graph The graph to find the MST for
 * @param totalWeight Output parameter for the total weight of the MST
 * @return Vector of (source, destination) pairs representing MST edges
 */
std::vector<std::pair<int, int>> reverseDeleteMST(const IGraph &graph, double &totalWeight)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Get all edges from the graph
    std::vector<std::tuple<double, int, int>> edges;

    for (int u : vertices)
    {
        for (int v : graph.getNeighbors(u))
        {
            // For undirected graphs, add each edge only once
            if (!graph.isDirected() && u < v)
            {
                double weight = graph.getEdgeWeight(u, v);
                edges.push_back({weight, u, v});
            }
            else if (graph.isDirected())
            {
                double weight = graph.getEdgeWeight(u, v);
                edges.push_back({weight, u, v});
            }
        }
    }

    // Sort edges by weight in descending order
    std::sort(edges.begin(), edges.end(), std::greater<std::tuple<double, int, int>>());

    // Create a copy of the graph
    auto mstGraph = std::make_unique<AdjacencyListGraph>(false, true);

    // Add all vertices
    for (int v : vertices)
    {
        mstGraph->addVertex(v);
    }

    // Add all edges
    for (const auto &[weight, u, v] : edges)
    {
        mstGraph->addEdge(u, v, weight);
    }

    // Process edges in order of decreasing weight
    for (const auto &[weight, u, v] : edges)
    {
        // Temporarily remove the edge
        mstGraph->removeEdge(u, v);

        // Check if the graph is still connected
        bool connected = isConnected(*mstGraph);

        // If removing the edge disconnects the graph, add it back
        if (!connected)
        {
            mstGraph->addEdge(u, v, weight);
        }
    }

    // Extract the MST edges
    std::vector<std::pair<int, int>> mstEdges;
    totalWeight = 0.0;

    for (int u : vertices)
    {
        for (int v : mstGraph->getNeighbors(u))
        {
            // For undirected graphs, add each edge only once
            if (u < v)
            {
                double weight = mstGraph->getEdgeWeight(u, v);
                mstEdges.push_back({u, v});
                totalWeight += weight;
            }
        }
    }

    return mstEdges;
}

/****************************************************************************************
 * APPLICATIONS OF MINIMUM SPANNING TREES
 ****************************************************************************************/

/**
 * @brief Class for designing optimal network layouts
 *
 * This class demonstrates how MSTs can be used to design networks (e.g., communications,
 * power, transportation) with minimal total cost.
 */
class NetworkDesigner
{
private:
    std::unique_ptr<IGraph> networkGraph;
    std::unordered_map<int, std::pair<double, double>> coordinates;

public:
    /**
     * @brief Constructor
     */
    NetworkDesigner()
    {
        networkGraph = std::make_unique<AdjacencyListGraph>(false, true);
    }

    /**
     * @brief Add a location to the network
     *
     * @param locationId Unique ID for the location
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    void addLocation(int locationId, double x, double y)
    {
        networkGraph->addVertex(locationId);
        coordinates[locationId] = {x, y};
    }

    /**
     * @brief Add a potential connection between two locations
     *
     * @param loc1 First location ID
     * @param loc2 Second location ID
     * @param cost Cost of the connection
     */
    void addPotentialConnection(int loc1, int loc2, double cost)
    {
        networkGraph->addEdge(loc1, loc2, cost);
    }

    /**
     * @brief Automatically generate connections based on distances
     *
     * This method creates a complete graph with edges weighted by Euclidean distance.
     */
    void generateConnections()
    {
        std::vector<int> locations = networkGraph->getVertices();

        for (size_t i = 0; i < locations.size(); i++)
        {
            for (size_t j = i + 1; j < locations.size(); j++)
            {
                int loc1 = locations[i];
                int loc2 = locations[j];

                const auto &[x1, y1] = coordinates[loc1];
                const auto &[x2, y2] = coordinates[loc2];

                double distance = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
                networkGraph->addEdge(loc1, loc2, distance);
            }
        }
    }

    /**
     * @brief Compute the optimal network design using Kruskal's algorithm
     *
     * @return Pair of (connections, total cost)
     */
    std::pair<std::vector<std::pair<int, int>>, double> designOptimalNetwork()
    {
        double totalCost;
        auto connections = kruskalMST(*networkGraph, totalCost);
        return {connections, totalCost};
    }

    /**
     * @brief Get the coordinates of a location
     *
     * @param locationId Location ID
     * @return Pair of (x, y) coordinates
     */
    std::pair<double, double> getLocationCoordinates(int locationId) const
    {
        if (coordinates.find(locationId) != coordinates.end())
        {
            return coordinates.at(locationId);
        }
        return {0.0, 0.0};
    }

    /**
     * @brief Get the cost of a connection
     *
     * @param loc1 First location ID
     * @param loc2 Second location ID
     * @return Cost of the connection
     */
    double getConnectionCost(int loc1, int loc2) const
    {
        return networkGraph->getEdgeWeight(loc1, loc2);
    }

    /**
     * @brief Print the network design
     *
     * @param connections List of connections in the design
     */
    void printNetworkDesign(const std::vector<std::pair<int, int>> &connections) const
    {
        double totalCost = 0.0;

        std::cout << "Optimal Network Design:" << std::endl;
        std::cout << "------------------------" << std::endl;

        for (const auto &[from, to] : connections)
        {
            double cost = networkGraph->getEdgeWeight(from, to);
            totalCost += cost;

            std::cout << "Connect location " << from << " to location " << to
                      << " (cost: " << cost << ")" << std::endl;
        }

        std::cout << "------------------------" << std::endl;
        std::cout << "Total Cost: " << totalCost << std::endl;
    }
};

/**
 * @brief Class for clustering data points using MST
 *
 * This class demonstrates how MSTs can be used for clustering in data mining.
 */
class ClusterAnalyzer
{
private:
    std::vector<std::vector<double>> dataPoints;
    std::unique_ptr<IGraph> similarityGraph;

public:
    /**
     * @brief Constructor
     *
     * @param data Vector of data points, where each data point is a vector of features
     */
    ClusterAnalyzer(const std::vector<std::vector<double>> &data)
    {
        dataPoints = data;
        similarityGraph = std::make_unique<AdjacencyListGraph>(false, true);

        // Add vertices for each data point
        for (int i = 0; i < dataPoints.size(); i++)
        {
            similarityGraph->addVertex(i);
        }

        // Add edges with distances between data points
        for (int i = 0; i < dataPoints.size(); i++)
        {
            for (int j = i + 1; j < dataPoints.size(); j++)
            {
                double distance = euclideanDistance(dataPoints[i], dataPoints[j]);
                similarityGraph->addEdge(i, j, distance);
            }
        }
    }

    /**
     * @brief Calculate Euclidean distance between two data points
     *
     * @param p1 First data point
     * @param p2 Second data point
     * @return Euclidean distance
     */
    double euclideanDistance(const std::vector<double> &p1, const std::vector<double> &p2)
    {
        double sum = 0.0;
        for (size_t i = 0; i < p1.size(); i++)
        {
            double diff = p1[i] - p2[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }

    /**
     * @brief Create clusters by removing the k-1 most expensive edges from the MST
     *
     * @param k Number of clusters to create
     * @return Vector of clusters, where each cluster is a vector of data point indices
     */
    std::vector<std::vector<int>> createClusters(int k)
    {
        // Get the MST
        double totalWeight;
        auto mstEdges = kruskalMST(*similarityGraph, totalWeight);

        // Sort MST edges by weight (distance)
        std::vector<std::tuple<double, int, int>> sortedEdges;
        for (const auto &[u, v] : mstEdges)
        {
            double weight = similarityGraph->getEdgeWeight(u, v);
            sortedEdges.push_back({weight, u, v});
        }
        std::sort(sortedEdges.begin(), sortedEdges.end(), std::greater<std::tuple<double, int, int>>());

        // Remove the k-1 most expensive edges to create k clusters
        int edgesToRemove = std::min(k - 1, static_cast<int>(sortedEdges.size()));
        std::vector<std::pair<int, int>> edgesToKeep;

        for (int i = edgesToRemove; i < sortedEdges.size(); i++)
        {
            auto [weight, u, v] = sortedEdges[i];
            edgesToKeep.push_back({u, v});
        }

        // Create clusters using the remaining MST edges
        DisjointSet ds(similarityGraph->getVertices());
        for (const auto &[u, v] : edgesToKeep)
        {
            ds.unionSets(u, v);
        }

        // Collect data points in each cluster
        std::unordered_map<int, std::vector<int>> clusterMap;
        for (int i = 0; i < dataPoints.size(); i++)
        {
            int clusterId = ds.findSet(i);
            clusterMap[clusterId].push_back(i);
        }

        // Convert to vector of clusters
        std::vector<std::vector<int>> clusters;
        for (const auto &[clusterId, points] : clusterMap)
        {
            clusters.push_back(points);
        }

        return clusters;
    }

    /**
     * @brief Print the clusters
     *
     * @param clusters Vector of clusters
     */
    void printClusters(const std::vector<std::vector<int>> &clusters) const
    {
        std::cout << "Data Clusters:" << std::endl;
        std::cout << "--------------" << std::endl;

        for (size_t i = 0; i < clusters.size(); i++)
        {
            std::cout << "Cluster " << i + 1 << " (" << clusters[i].size() << " points):" << std::endl;

            for (int pointIdx : clusters[i])
            {
                std::cout << "  Point " << pointIdx << ": [";

                for (size_t j = 0; j < dataPoints[pointIdx].size(); j++)
                {
                    std::cout << dataPoints[pointIdx][j];
                    if (j < dataPoints[pointIdx].size() - 1)
                    {
                        std::cout << ", ";
                    }
                }

                std::cout << "]" << std::endl;
            }

            std::cout << std::endl;
        }
    }
};

/**
 * @brief Class for image segmentation using MST
 *
 * This class demonstrates how MSTs can be used for image segmentation.
 */
class ImageSegmenter
{
private:
    std::vector<std::vector<int>> image; // Pixel values
    std::unique_ptr<IGraph> pixelGraph;
    int width, height;

public:
    /**
     * @brief Constructor
     *
     * @param img Image represented as a 2D vector of pixel values
     */
    ImageSegmenter(const std::vector<std::vector<int>> &img)
    {
        image = img;
        height = image.size();
        width = height > 0 ? image[0].size() : 0;

        pixelGraph = std::make_unique<AdjacencyListGraph>(false, true);

        // Add vertices for each pixel
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int pixelId = y * width + x;
                pixelGraph->addVertex(pixelId);
            }
        }

        // Add edges between adjacent pixels
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int pixelId = y * width + x;

                // Connect to right neighbor
                if (x + 1 < width)
                {
                    int neighborId = y * width + (x + 1);
                    double diff = std::abs(image[y][x] - image[y][x + 1]);
                    pixelGraph->addEdge(pixelId, neighborId, diff);
                }

                // Connect to bottom neighbor
                if (y + 1 < height)
                {
                    int neighborId = (y + 1) * width + x;
                    double diff = std::abs(image[y][x] - image[y + 1][x]);
                    pixelGraph->addEdge(pixelId, neighborId, diff);
                }
            }
        }
    }

    /**
     * @brief Segment the image by creating the specified number of segments
     *
     * @param numSegments Number of segments to create
     * @return 2D vector where each pixel is labeled with its segment ID
     */
    std::vector<std::vector<int>> segmentImage(int numSegments)
    {
        // Get the MST
        double totalWeight;
        auto mstEdges = kruskalMST(*pixelGraph, totalWeight);

        // Sort MST edges by weight (pixel difference)
        std::vector<std::tuple<double, int, int>> sortedEdges;
        for (const auto &[u, v] : mstEdges)
        {
            double weight = pixelGraph->getEdgeWeight(u, v);
            sortedEdges.push_back({weight, u, v});
        }
        std::sort(sortedEdges.begin(), sortedEdges.end(), std::greater<std::tuple<double, int, int>>());

        // Remove the numSegments-1 most expensive edges
        int edgesToRemove = std::min(numSegments - 1, static_cast<int>(sortedEdges.size()));
        std::vector<std::pair<int, int>> edgesToKeep;

        for (int i = edgesToRemove; i < sortedEdges.size(); i++)
        {
            auto [weight, u, v] = sortedEdges[i];
            edgesToKeep.push_back({u, v});
        }

        // Create segments using the remaining MST edges
        DisjointSet ds(pixelGraph->getVertices());
        for (const auto &[u, v] : edgesToKeep)
        {
            ds.unionSets(u, v);
        }

        // Create segmentation result
        std::vector<std::vector<int>> segmentation(height, std::vector<int>(width));
        std::unordered_map<int, int> segmentIds;
        int nextSegmentId = 0;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int pixelId = y * width + x;
                int setId = ds.findSet(pixelId);

                // Assign segment ID
                if (segmentIds.find(setId) == segmentIds.end())
                {
                    segmentIds[setId] = nextSegmentId++;
                }

                segmentation[y][x] = segmentIds[setId];
            }
        }

        return segmentation;
    }

    /**
     * @brief Print the segmented image
     *
     * @param segmentation Segmented image as a 2D vector of segment IDs
     */
    void printSegmentation(const std::vector<std::vector<int>> &segmentation) const
    {
        std::cout << "Image Segmentation:" << std::endl;
        std::cout << "------------------" << std::endl;

        const char *symbols = " .:-=+*#%@"; // Different symbols for different segments
        int maxSymbols = 10;                // Length of the symbols array

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int segmentId = segmentation[y][x] % maxSymbols;
                std::cout << symbols[segmentId] << symbols[segmentId];
            }
            std::cout << std::endl;
        }
    }
};

/**
 * @brief Class for approximating the Traveling Salesman Problem (TSP) using MST
 *
 * This class demonstrates how MSTs can be used to approximate the TSP.
 */
class TSPApproximator
{
private:
    std::unique_ptr<IGraph> graph;
    std::unordered_map<int, std::pair<double, double>> coordinates;

public:
    /**
     * @brief Constructor
     */
    TSPApproximator()
    {
        graph = std::make_unique<AdjacencyListGraph>(false, true);
    }

    /**
     * @brief Add a city to the problem
     *
     * @param cityId City ID
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    void addCity(int cityId, double x, double y)
    {
        graph->addVertex(cityId);
        coordinates[cityId] = {x, y};

        // Add edges to all existing cities
        for (const auto &[existingCity, _] : coordinates)
        {
            if (existingCity != cityId)
            {
                double distance = euclideanDistance(x, y, coordinates[existingCity].first, coordinates[existingCity].second);
                graph->addEdge(cityId, existingCity, distance);
            }
        }
    }

    /**
     * @brief Calculate Euclidean distance between two points
     *
     * @param x1 X-coordinate of first point
     * @param y1 Y-coordinate of first point
     * @param x2 X-coordinate of second point
     * @param y2 Y-coordinate of second point
     * @return Euclidean distance
     */
    double euclideanDistance(double x1, double y1, double x2, double y2)
    {
        double dx = x1 - x2;
        double dy = y1 - y2;
        return std::sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief Approximate the TSP solution using MST
     *
     * This method uses a 2-approximation algorithm for TSP based on MST.
     *
     * @return Vector of city IDs representing the TSP tour
     */
    std::vector<int> approximateTSPSolution()
    {
        // Step 1: Find MST
        double mstWeight;
        auto mstEdges = primMST(*graph, mstWeight);

        // Step 2: Convert MST to a multigraph by doubling each edge
        auto multiGraph = std::make_unique<AdjacencyListGraph>(true, true);

        for (int cityId : graph->getVertices())
        {
            multiGraph->addVertex(cityId);
        }

        for (const auto &[u, v] : mstEdges)
        {
            double weight = graph->getEdgeWeight(u, v);
            multiGraph->addEdge(u, v, weight);
            multiGraph->addEdge(v, u, weight);
        }

        // Step 3: Find an Eulerian circuit
        std::vector<int> eulerianCircuit = findEulerianCircuit(*multiGraph);

        // Step 4: Create Hamiltonian circuit by skipping repeated vertices
        std::vector<int> tspRoute;
        std::unordered_set<int> visited;

        for (int city : eulerianCircuit)
        {
            if (visited.find(city) == visited.end())
            {
                tspRoute.push_back(city);
                visited.insert(city);
            }
        }

        // Complete the circuit
        if (!tspRoute.empty())
        {
            tspRoute.push_back(tspRoute[0]);
        }

        return tspRoute;
    }

    /**
     * @brief Find an Eulerian circuit in the graph
     *
     * This method implements Hierholzer's algorithm for finding an Eulerian circuit.
     *
     * @param g The graph to find the Eulerian circuit in
     * @return Vector of vertex IDs representing the Eulerian circuit
     */
    std::vector<int> findEulerianCircuit(const IGraph &g)
    {
        // Implementation of Hierholzer's algorithm for Eulerian circuit
        // This is a simplified version and may not handle all edge cases

        std::vector<int> circuit;
        std::vector<int> vertices = g.getVertices();

        if (vertices.empty())
        {
            return circuit;
        }

        // Start at any vertex
        int start = vertices[0];

        // Create a working copy of the graph
        auto workGraph = std::make_unique<AdjacencyListGraph>(true, true);

        for (int v : vertices)
        {
            workGraph->addVertex(v);
        }

        for (int u : vertices)
        {
            for (int v : g.getNeighbors(u))
            {
                double weight = g.getEdgeWeight(u, v);
                workGraph->addEdge(u, v, weight);
            }
        }

        // Start with a single vertex in the circuit
        circuit.push_back(start);
        int currentVertex = start;

        while (true)
        {
            std::vector<int> neighbors = workGraph->getNeighbors(currentVertex);

            if (neighbors.empty())
            {
                break;
            }

            // Take any edge
            int nextVertex = neighbors[0];

            // Remove the edge
            workGraph->removeEdge(currentVertex, nextVertex);

            // Move to the next vertex
            currentVertex = nextVertex;
            circuit.push_back(currentVertex);
        }

        return circuit;
    }

    /**
     * @brief Calculate the total cost of a route
     *
     * @param route Vector of city IDs representing the route
     * @return Total cost of the route
     */
    double getRouteCost(const std::vector<int> &route)
    {
        double totalCost = 0.0;

        for (size_t i = 0; i < route.size() - 1; i++)
        {
            int from = route[i];
            int to = route[i + 1];
            totalCost += graph->getEdgeWeight(from, to);
        }

        return totalCost;
    }

    /**
     * @brief Print the TSP route
     *
     * @param route Vector of city IDs representing the route
     */
    void printTSPRoute(const std::vector<int> &route) const
    {
        std::cout << "TSP Route:" << std::endl;
        std::cout << "----------" << std::endl;

        for (size_t i = 0; i < route.size(); i++)
        {
            int city = route[i];

            std::cout << "City " << city;

            if (i < route.size() - 1)
            {
                int nextCity = route[i + 1];
                double distance = graph->getEdgeWeight(city, nextCity);
                std::cout << " -> City " << nextCity << " (distance: " << distance << ")";
            }

            std::cout << std::endl;
        }

        double totalDistance = 0.0;
        for (size_t i = 0; i < route.size() - 1; i++)
        {
            totalDistance += graph->getEdgeWeight(route[i], route[i + 1]);
        }

        std::cout << "Total distance: " << totalDistance << std::endl;
    }
};

/****************************************************************************************
 * PERFORMANCE BENCHMARKS
 ****************************************************************************************/

/**
 * @brief Benchmark different MST algorithms
 *
 * @param graphSizes Vector of graph sizes to test
 * @param edgeDensities Vector of edge densities to test
 * @param iterations Number of iterations for each test
 */
void benchmarkMSTAlgorithms(
    const std::vector<int> &graphSizes,
    const std::vector<double> &edgeDensities,
    int iterations = 5)
{

    std::cout << "===== Minimum Spanning Tree Algorithm Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(10) << "Edges"
              << std::setw(15) << "Density"
              << std::setw(15) << "Prim (ms)"
              << std::setw(15) << "Kruskal (ms)"
              << std::setw(15) << "Borůvka (ms)"
              << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // Test different graph sizes and densities
    for (int size : graphSizes)
    {
        for (double density : edgeDensities)
        {
            double primTime = 0.0;
            double kruskalTime = 0.0;
            double boruvkaTime = 0.0;
            int totalEdges = 0;

            for (int iter = 0; iter < iterations; ++iter)
            {
                // Generate a random weighted graph
                auto graph = std::make_unique<AdjacencyListGraph>(false, true);

                // Add vertices
                for (int i = 0; i < size; i++)
                {
                    graph->addVertex(i);
                }

                // Add random edges
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> distr(0.0, 1.0);
                std::uniform_real_distribution<> weightDistr(1.0, 100.0);

                for (int i = 0; i < size; i++)
                {
                    for (int j = i + 1; j < size; j++)
                    {
                        if (distr(gen) < density)
                        {
                            graph->addEdge(i, j, weightDistr(gen));
                        }
                    }
                }

                totalEdges = graph->getEdgeCount();

                // Ensure the graph is connected
                for (int i = 1; i < size; i++)
                {
                    if (!graph->hasEdge(i - 1, i) && !graph->hasEdge(i, i - 1))
                    {
                        graph->addEdge(i - 1, i, weightDistr(gen));
                        totalEdges++;
                    }
                }

                // Benchmark Prim's algorithm
                double totalWeight;
                auto startPrim = std::chrono::high_resolution_clock::now();
                auto primMST_result = primMST(*graph, totalWeight);
                auto endPrim = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> primDuration = endPrim - startPrim;
                primTime += primDuration.count();

                // Benchmark Kruskal's algorithm
                auto startKruskal = std::chrono::high_resolution_clock::now();
                auto kruskalMST_result = kruskalMST(*graph, totalWeight);
                auto endKruskal = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> kruskalDuration = endKruskal - startKruskal;
                kruskalTime += kruskalDuration.count();

                // Benchmark Borůvka's algorithm
                auto startBoruvka = std::chrono::high_resolution_clock::now();
                auto boruvkaMST_result = boruvkaMST(*graph, totalWeight);
                auto endBoruvka = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> boruvkaDuration = endBoruvka - startBoruvka;
                boruvkaTime += boruvkaDuration.count();
            }

            // Calculate averages
            primTime /= iterations;
            kruskalTime /= iterations;
            boruvkaTime /= iterations;

            // Print results
            std::cout << std::left << std::setw(10) << size
                      << std::setw(10) << totalEdges
                      << std::setw(15) << std::fixed << std::setprecision(2) << density
                      << std::setw(15) << std::fixed << std::setprecision(2) << primTime
                      << std::setw(15) << std::fixed << std::setprecision(2) << kruskalTime
                      << std::setw(15) << std::fixed << std::setprecision(2) << boruvkaTime
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark MST algorithms on dense vs. sparse graphs
 *
 * @param sizes Vector of graph sizes to test
 * @param iterations Number of iterations for each test
 */
void benchmarkMSTDenseVsSparse(
    const std::vector<int> &sizes,
    int iterations = 5)
{

    std::cout << "===== MST Algorithm Performance: Dense vs. Sparse Graphs =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(20) << "Prim (Sparse ms)"
              << std::setw(20) << "Kruskal (Sparse ms)"
              << std::setw(20) << "Prim (Dense ms)"
              << std::setw(20) << "Kruskal (Dense ms)"
              << std::endl;

    std::cout << std::string(90, '-') << std::endl;

    // Densities for sparse and dense graphs
    double sparseDensity = 0.05; // 5% of possible edges
    double denseDensity = 0.75;  // 75% of possible edges

    // Test different graph sizes
    for (int size : sizes)
    {
        double primSparseTime = 0.0;
        double kruskalSparseTime = 0.0;
        double primDenseTime = 0.0;
        double kruskalDenseTime = 0.0;

        for (int iter = 0; iter < iterations; ++iter)
        {
            // Generate a sparse random weighted graph
            auto sparseGraph = std::make_unique<AdjacencyListGraph>(false, true);

            // Add vertices
            for (int i = 0; i < size; i++)
            {
                sparseGraph->addVertex(i);
            }

            // Add random edges
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distr(0.0, 1.0);
            std::uniform_real_distribution<> weightDistr(1.0, 100.0);

            for (int i = 0; i < size; i++)
            {
                for (int j = i + 1; j < size; j++)
                {
                    if (distr(gen) < sparseDensity)
                    {
                        sparseGraph->addEdge(i, j, weightDistr(gen));
                    }
                }
            }

            // Ensure the graph is connected
            for (int i = 1; i < size; i++)
            {
                if (!sparseGraph->hasEdge(i - 1, i) && !sparseGraph->hasEdge(i, i - 1))
                {
                    sparseGraph->addEdge(i - 1, i, weightDistr(gen));
                }
            }

            // Generate a dense random weighted graph
            auto denseGraph = std::make_unique<AdjacencyListGraph>(false, true);

            // Add vertices
            for (int i = 0; i < size; i++)
            {
                denseGraph->addVertex(i);
            }

            // Add random edges
            for (int i = 0; i < size; i++)
            {
                for (int j = i + 1; j < size; j++)
                {
                    if (distr(gen) < denseDensity)
                    {
                        denseGraph->addEdge(i, j, weightDistr(gen));
                    }
                }
            }

            // Ensure the graph is connected
            for (int i = 1; i < size; i++)
            {
                if (!denseGraph->hasEdge(i - 1, i) && !denseGraph->hasEdge(i, i - 1))
                {
                    denseGraph->addEdge(i - 1, i, weightDistr(gen));
                }
            }

            // Benchmark Prim's algorithm on sparse graph
            double totalWeight;
            auto startPrimSparse = std::chrono::high_resolution_clock::now();
            auto primSparse = primMST(*sparseGraph, totalWeight);
            auto endPrimSparse = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> primSparseDuration = endPrimSparse - startPrimSparse;
            primSparseTime += primSparseDuration.count();

            // Benchmark Kruskal's algorithm on sparse graph
            auto startKruskalSparse = std::chrono::high_resolution_clock::now();
            auto kruskalSparse = kruskalMST(*sparseGraph, totalWeight);
            auto endKruskalSparse = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> kruskalSparseDuration = endKruskalSparse - startKruskalSparse;
            kruskalSparseTime += kruskalSparseDuration.count();

            // Benchmark Prim's algorithm on dense graph
            auto startPrimDense = std::chrono::high_resolution_clock::now();
            auto primDense = primMST(*denseGraph, totalWeight);
            auto endPrimDense = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> primDenseDuration = endPrimDense - startPrimDense;
            primDenseTime += primDenseDuration.count();

            // Benchmark Kruskal's algorithm on dense graph
            auto startKruskalDense = std::chrono::high_resolution_clock::now();
            auto kruskalDense = kruskalMST(*denseGraph, totalWeight);
            auto endKruskalDense = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> kruskalDenseDuration = endKruskalDense - startKruskalDense;
            kruskalDenseTime += kruskalDenseDuration.count();
        }

        // Calculate averages
        primSparseTime /= iterations;
        kruskalSparseTime /= iterations;
        primDenseTime /= iterations;
        kruskalDenseTime /= iterations;

        // Print results
        std::cout << std::left << std::setw(10) << size
                  << std::setw(20) << std::fixed << std::setprecision(2) << primSparseTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << kruskalSparseTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << primDenseTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << kruskalDenseTime
                  << std::endl;
    }

    std::cout << std::endl;
}

/****************************************************************************************
 * DEMONSTRATION FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Demonstrate basic MST algorithms on a sample graph
 */
void demoBasicMST()
{
    std::cout << "===== Basic MST Algorithm Demo =====" << std::endl;

    // Create a sample graph
    auto graph = std::make_unique<AdjacencyListGraph>(false, true);

    // Add vertices (A through F)
    for (int i = 0; i < 6; i++)
    {
        graph->addVertex(i);
    }

    // Add edges to form the graph from the article
    //      6       5
    //  A ----- B ----- C
    //  |       |       |
    // 4|       |1      |2
    //  |       |       |
    //  D ----- E ----- F
    //      3       8

    graph->addEdge(0, 1, 6); // A-B
    graph->addEdge(0, 3, 4); // A-D
    graph->addEdge(1, 2, 5); // B-C
    graph->addEdge(1, 4, 1); // B-E
    graph->addEdge(2, 5, 2); // C-F
    graph->addEdge(3, 4, 3); // D-E
    graph->addEdge(4, 5, 8); // E-F

    // Print the graph
    std::cout << "Sample graph:" << std::endl;
    std::cout << graph->toString() << std::endl;

    // Run Prim's algorithm
    std::cout << "Running Prim's algorithm..." << std::endl;
    double primWeight;
    auto primEdges = primMST(*graph, primWeight);

    std::cout << "MST edges (Prim's algorithm):" << std::endl;
    for (const auto &[from, to] : primEdges)
    {
        char fromChar = 'A' + from;
        char toChar = 'A' + to;
        double weight = graph->getEdgeWeight(from, to);

        std::cout << "  " << fromChar << " -- " << toChar << " (weight: " << weight << ")" << std::endl;
    }
    std::cout << "Total MST weight: " << primWeight << std::endl;

    // Run Kruskal's algorithm
    std::cout << "\nRunning Kruskal's algorithm..." << std::endl;
    double kruskalWeight;
    auto kruskalEdges = kruskalMST(*graph, kruskalWeight);

    std::cout << "MST edges (Kruskal's algorithm):" << std::endl;
    for (const auto &[from, to] : kruskalEdges)
    {
        char fromChar = 'A' + from;
        char toChar = 'A' + to;
        double weight = graph->getEdgeWeight(from, to);

        std::cout << "  " << fromChar << " -- " << toChar << " (weight: " << weight << ")" << std::endl;
    }
    std::cout << "Total MST weight: " << kruskalWeight << std::endl;

    // Run Borůvka's algorithm
    std::cout << "\nRunning Borůvka's algorithm..." << std::endl;
    double boruvkaWeight;
    auto boruvkaEdges = boruvkaMST(*graph, boruvkaWeight);

    std::cout << "MST edges (Borůvka's algorithm):" << std::endl;
    for (const auto &[from, to] : boruvkaEdges)
    {
        char fromChar = 'A' + from;
        char toChar = 'A' + to;
        double weight = graph->getEdgeWeight(from, to);

        std::cout << "  " << fromChar << " -- " << toChar << " (weight: " << weight << ")" << std::endl;
    }
    std::cout << "Total MST weight: " << boruvkaWeight << std::endl;

    // Check if all algorithms produce the same MST weight
    std::cout << "\nAll algorithms produce" << (std::abs(primWeight - kruskalWeight) < 1e-6 && std::abs(kruskalWeight - boruvkaWeight) < 1e-6 ? " the same" : " different") << " MST weights." << std::endl;
}

/**
 * @brief Demonstrate the network design application
 */
void demoNetworkDesign()
{
    std::cout << "===== Network Design Demo =====" << std::endl;

    // Create a network designer
    NetworkDesigner designer;

    // Add locations
    designer.addLocation(1, 0.0, 0.0);   // City 1
    designer.addLocation(2, 10.0, 0.0);  // City 2
    designer.addLocation(3, 5.0, 8.66);  // City 3
    designer.addLocation(4, 15.0, 8.66); // City 4
    designer.addLocation(5, 7.5, 13.0);  // City 5

    // Generate connections based on distances
    designer.generateConnections();

    // Design the optimal network
    auto [connections, totalCost] = designer.designOptimalNetwork();

    // Print the network design
    designer.printNetworkDesign(connections);
}

/**
 * @brief Demonstrate the clustering application
 */
void demoClusterAnalysis()
{
    std::cout << "===== Cluster Analysis Demo =====" << std::endl;

    // Create some sample data points
    std::vector<std::vector<double>> dataPoints = {
        {1.0, 1.0},   // Point 0
        {1.5, 1.5},   // Point 1
        {2.0, 1.0},   // Point 2
        {5.0, 5.0},   // Point 3
        {5.5, 4.5},   // Point 4
        {5.0, 4.0},   // Point 5
        {10.0, 10.0}, // Point 6
        {10.5, 9.5},  // Point 7
        {9.5, 9.0}    // Point 8
    };

    // Create a cluster analyzer
    ClusterAnalyzer analyzer(dataPoints);

    // Create clusters
    int k = 3; // Number of clusters
    std::vector<std::vector<int>> clusters = analyzer.createClusters(k);

    // Print the clusters
    analyzer.printClusters(clusters);
}

/**
 * @brief Demonstrate the image segmentation application
 */
void demoImageSegmentation()
{
    std::cout << "===== Image Segmentation Demo =====" << std::endl;

    // Create a simple grayscale image (intensity values from 0 to 255)
    std::vector<std::vector<int>> image = {
        {50, 50, 50, 50, 150, 150, 150, 150},
        {50, 50, 50, 50, 150, 150, 150, 150},
        {50, 50, 50, 50, 150, 150, 150, 150},
        {50, 50, 50, 50, 150, 150, 150, 150},
        {100, 100, 100, 100, 200, 200, 200, 200},
        {100, 100, 100, 100, 200, 200, 200, 200},
        {100, 100, 100, 100, 200, 200, 200, 200},
        {100, 100, 100, 100, 200, 200, 200, 200}};

    // Print the original image
    std::cout << "Original Image:" << std::endl;
    for (const auto &row : image)
    {
        for (int pixel : row)
        {
            // Print a character based on intensity
            char symbol = ' ';
            if (pixel > 180)
                symbol = '@';
            else if (pixel > 130)
                symbol = '#';
            else if (pixel > 80)
                symbol = '+';
            else if (pixel > 30)
                symbol = '.';

            std::cout << symbol << symbol;
        }
        std::cout << std::endl;
    }

    // Create an image segmenter
    ImageSegmenter segmenter(image);

    // Segment the image into 3 segments
    std::vector<std::vector<int>> segmentation = segmenter.segmentImage(3);

    // Print the segmentation
    std::cout << std::endl;
    segmenter.printSegmentation(segmentation);
}

/**
 * @brief Demonstrate the TSP approximation application
 */
void demoTSPApproximation()
{
    std::cout << "===== TSP Approximation Demo =====" << std::endl;

    // Create a TSP approximator
    TSPApproximator tsp;

    // Add cities
    tsp.addCity(1, 0.0, 0.0);   // City 1
    tsp.addCity(2, 10.0, 0.0);  // City 2
    tsp.addCity(3, 5.0, 8.66);  // City 3
    tsp.addCity(4, 15.0, 8.66); // City 4
    tsp.addCity(5, 7.5, 13.0);  // City 5

    // Approximate the TSP solution
    std::vector<int> route = tsp.approximateTSPSolution();

    // Print the TSP route
    tsp.printTSPRoute(route);

    // Calculate the total cost of the route
    double totalCost = tsp.getRouteCost(route);

    std::cout << "The MST-based TSP approximation has a total cost of " << totalCost << std::endl;
}

/**
 * @brief Main function
 */
int main()
{
    std::cout << "======================================================" << std::endl;
    std::cout << "=== DAY 32: MINIMUM SPANNING TREES                 ===" << std::endl;
    std::cout << "=== Part of #DSAin45 Course                        ===" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;

    // Demonstrate basic MST algorithms
    demoBasicMST();
    std::cout << std::endl;

    // Demonstrate network design application
    demoNetworkDesign();
    std::cout << std::endl;

    // Demonstrate cluster analysis application
    demoClusterAnalysis();
    std::cout << std::endl;

    // Demonstrate image segmentation application
    demoImageSegmentation();
    std::cout << std::endl;

    // Demonstrate TSP approximation application
    demoTSPApproximation();
    std::cout << std::endl;

    // Run benchmarks (optional - can be commented out for faster execution)
    std::cout << "Running benchmarks (this may take a moment)..." << std::endl;

    benchmarkMSTAlgorithms({50, 100, 200}, {0.05, 0.2}, 2);
    benchmarkMSTDenseVsSparse({100, 200, 300}, 2);

    std::cout << "======================================================" << std::endl;
    std::cout << "=== End of DAY 32 Demonstrations                   ===" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}