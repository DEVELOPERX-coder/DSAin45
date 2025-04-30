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
 * @brief Comprehensive Shortest Path Algorithms Implementation for #DSAin45 Day 31
 *
 * This file provides implementations of:
 * - Dijkstra's Algorithm
 * - Bellman-Ford Algorithm
 * - Floyd-Warshall Algorithm
 * - Johnson's Algorithm
 * - A* Search Algorithm
 * - Bidirectional Search
 * - Various optimizations and applications
 * - Performance benchmarks
 *
 * @author #DSAin45 Course
 * @date Day 31 of 45-Day DSA Challenge
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
 * UTILITY CLASSES FOR SHORTEST PATH ALGORITHMS
 ****************************************************************************************/

/**
 * @brief Structure to represent a geographic coordinate
 */
struct GeoCoordinate
{
    double latitude;
    double longitude;

    GeoCoordinate(double lat = 0.0, double lon = 0.0)
        : latitude(lat), longitude(lon) {}
};

/**
 * @brief Calculate Haversine distance between two geographic coordinates
 *
 * @param a First coordinate
 * @param b Second coordinate
 * @return Distance in kilometers
 */
double haversineDistance(const GeoCoordinate &a, const GeoCoordinate &b)
{
    const double EARTH_RADIUS_KM = 6371.0;

    // Convert latitude and longitude from degrees to radians
    double lat1 = a.latitude * M_PI / 180.0;
    double lon1 = a.longitude * M_PI / 180.0;
    double lat2 = b.latitude * M_PI / 180.0;
    double lon2 = b.longitude * M_PI / 180.0;

    // Haversine formula
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    double sinDLat = sin(dLat / 2);
    double sinDLon = sin(dLon / 2);

    double a1 = sinDLat * sinDLat + cos(lat1) * cos(lat2) * sinDLon * sinDLon;
    double c = 2 * atan2(sqrt(a1), sqrt(1 - a1));

    return EARTH_RADIUS_KM * c;
}

/**
 * @brief Calculate Manhattan distance between two points
 *
 * @param a First point (x, y)
 * @param b Second point (x, y)
 * @return Manhattan distance
 */
double manhattanDistance(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

/**
 * @brief Calculate Euclidean distance between two points
 *
 * @param a First point (x, y)
 * @param b Second point (x, y)
 * @return Euclidean distance
 */
double euclideanDistance(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    double dx = a.first - b.first;
    double dy = a.second - b.second;
    return std::sqrt(dx * dx + dy * dy);
}

/****************************************************************************************
 * SHORTEST PATH ALGORITHMS
 ****************************************************************************************/

/**
 * @brief Implementation of Dijkstra's algorithm for finding the shortest paths
 *        from a source vertex to all other vertices
 *
 * Time Complexity: O(E log V) with binary heap
 * Space Complexity: O(V)
 *
 * @param graph The graph
 * @param source Source vertex
 * @param distances Output vector of distances from source to each vertex
 * @return Vector of previous vertices in the shortest paths
 */
std::vector<int> dijkstra(const IGraph &graph, int source,
                          std::vector<double> &distances)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
    }

    // Initialize distances and previous vertices
    distances.assign(n, std::numeric_limits<double>::infinity());
    distances[vertexToIdx[source]] = 0;

    std::vector<int> previous(n, -1); // Store previous vertex in shortest path

    // Min priority queue: pair of (distance, vertex)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        pq;

    // Add source to priority queue
    pq.push({0.0, source});

    // Set to keep track of processed vertices
    std::unordered_set<int> processed;

    while (!pq.empty())
    {
        int u = pq.top().second; // Get vertex with minimum distance
        pq.pop();

        // Skip if already processed
        if (processed.find(u) != processed.end())
        {
            continue;
        }

        // Mark as processed
        processed.insert(u);

        // Process all neighbors of u
        for (int v : graph.getNeighbors(u))
        {
            double weight = graph.getEdgeWeight(u, v);
            int vIdx = vertexToIdx[v];
            int uIdx = vertexToIdx[u];

            // Relaxation step: If a shorter path to v is found
            if (distances[uIdx] + weight < distances[vIdx])
            {
                distances[vIdx] = distances[uIdx] + weight;
                previous[vIdx] = u;

                // Add to priority queue
                pq.push({distances[vIdx], v});
            }
        }
    }

    return previous;
}

/**
 * @brief Reconstruct a path from source to destination using the previous vertices array
 *
 * @param previous Vector of previous vertices
 * @param source Source vertex
 * @param destination Destination vertex
 * @param vertexToIdx Mapping from vertices to indices
 * @return Vector representing the path from source to destination
 */
std::vector<int> reconstructPath(const std::vector<int> &previous,
                                 int source, int destination,
                                 const std::unordered_map<int, int> &vertexToIdx)
{
    std::vector<int> path;

    // Check if destination is reachable
    if (previous[vertexToIdx.at(destination)] == -1 && source != destination)
    {
        return path; // Empty path - unreachable
    }

    // Start from destination and work backwards
    for (int at = destination; at != -1; at = previous[vertexToIdx.at(at)])
    {
        path.push_back(at);

        // If we've reached the source, we're done
        if (at == source)
        {
            break;
        }
    }

    // Check if we actually reached the source
    if (path.empty() || path.back() != source)
    {
        return {}; // No path exists
    }

    // Reverse path to get it from source to destination
    std::reverse(path.begin(), path.end());

    return path;
}

/**
 * @brief Single-target Dijkstra's algorithm
 *
 * This version stops once the target is processed, which can be more efficient
 * when only a specific destination is of interest.
 *
 * @param graph The graph
 * @param source Source vertex
 * @param target Target vertex
 * @param shortestDistance Output parameter to store the shortest distance
 * @return Vector representing the shortest path
 */
std::vector<int> dijkstraSingleTarget(const IGraph &graph, int source, int target,
                                      double &shortestDistance)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
    }

    // Initialize distances and previous vertices
    std::vector<double> distances(n, std::numeric_limits<double>::infinity());
    distances[vertexToIdx[source]] = 0;

    std::vector<int> previous(n, -1); // Store previous vertex in shortest path

    // Min priority queue: pair of (distance, vertex)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        pq;

    // Add source to priority queue
    pq.push({0.0, source});

    // Set to keep track of processed vertices
    std::unordered_set<int> processed;

    while (!pq.empty())
    {
        auto [dist, u] = pq.top(); // Get vertex with minimum distance
        pq.pop();

        // If we've reached the target, we're done
        if (u == target)
        {
            shortestDistance = dist;
            return reconstructPath(previous, source, target, vertexToIdx);
        }

        // Skip if already processed
        if (processed.find(u) != processed.end())
        {
            continue;
        }

        // Mark as processed
        processed.insert(u);

        // Process all neighbors of u
        for (int v : graph.getNeighbors(u))
        {
            double weight = graph.getEdgeWeight(u, v);
            int vIdx = vertexToIdx[v];
            int uIdx = vertexToIdx[u];

            // Relaxation step: If a shorter path to v is found
            if (distances[uIdx] + weight < distances[vIdx])
            {
                distances[vIdx] = distances[uIdx] + weight;
                previous[vIdx] = u;

                // Add to priority queue
                pq.push({distances[vIdx], v});
            }
        }
    }

    // If we get here, target is unreachable
    shortestDistance = std::numeric_limits<double>::infinity();
    return {};
}

/**
 * @brief Implementation of the Bellman-Ford algorithm for finding the shortest paths
 *        from a source vertex to all other vertices (works with negative edge weights)
 *
 * Time Complexity: O(V * E)
 * Space Complexity: O(V)
 *
 * @param graph The graph
 * @param source Source vertex
 * @param distances Output vector of distances from source to each vertex
 * @param previous Output vector of previous vertices in the shortest paths
 * @return True if no negative cycles, false otherwise
 */
bool bellmanFord(const IGraph &graph, int source,
                 std::vector<double> &distances,
                 std::vector<int> &previous)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
    }

    // Initialize distances
    distances.assign(n, std::numeric_limits<double>::infinity());
    distances[vertexToIdx[source]] = 0;

    // Initialize previous vertices
    previous.assign(n, -1);

    // Get all edges from the graph
    std::vector<std::tuple<int, int, double>> edges;

    for (int u : vertices)
    {
        for (int v : graph.getNeighbors(u))
        {
            double weight = graph.getEdgeWeight(u, v);
            edges.push_back({u, v, weight});
        }
    }

    // Relax all edges V-1 times
    for (int i = 0; i < n - 1; i++)
    {
        bool anyChange = false;

        for (const auto &[u, v, weight] : edges)
        {
            int uIdx = vertexToIdx[u];
            int vIdx = vertexToIdx[v];

            // If u is not reachable, skip
            if (distances[uIdx] == std::numeric_limits<double>::infinity())
            {
                continue;
            }

            // Relaxation step
            if (distances[uIdx] + weight < distances[vIdx])
            {
                distances[vIdx] = distances[uIdx] + weight;
                previous[vIdx] = u;
                anyChange = true;
            }
        }

        // Early termination if no changes were made in this iteration
        if (!anyChange)
        {
            break;
        }
    }

    // Check for negative cycles
    for (const auto &[u, v, weight] : edges)
    {
        int uIdx = vertexToIdx[u];
        int vIdx = vertexToIdx[v];

        if (distances[uIdx] != std::numeric_limits<double>::infinity() &&
            distances[uIdx] + weight < distances[vIdx])
        {
            // Negative cycle detected
            return false;
        }
    }

    return true; // No negative cycles
}

/**
 * @brief Find a negative cycle in the graph using Bellman-Ford
 *
 * @param graph The graph
 * @param source Source vertex
 * @return Vector representing a negative cycle, or empty if none exists
 */
std::vector<int> findNegativeCycle(const IGraph &graph, int source)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    std::unordered_map<int, int> idxToVertex;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
        idxToVertex[i] = vertices[i];
    }

    // Initialize distances and previous vertices
    std::vector<double> distances(n, std::numeric_limits<double>::infinity());
    distances[vertexToIdx[source]] = 0;

    std::vector<int> previous(n, -1);

    // Get all edges from the graph
    std::vector<std::tuple<int, int, double>> edges;

    for (int u : vertices)
    {
        for (int v : graph.getNeighbors(u))
        {
            double weight = graph.getEdgeWeight(u, v);
            edges.push_back({u, v, weight});
        }
    }

    // Relax all edges V times
    for (int i = 0; i < n; i++)
    {
        for (const auto &[u, v, weight] : edges)
        {
            int uIdx = vertexToIdx[u];
            int vIdx = vertexToIdx[v];

            // If u is not reachable, skip
            if (distances[uIdx] == std::numeric_limits<double>::infinity())
            {
                continue;
            }

            // Relaxation step
            if (distances[uIdx] + weight < distances[vIdx])
            {
                distances[vIdx] = distances[uIdx] + weight;
                previous[vIdx] = u;

                // If we're on the nth iteration, a negative cycle exists
                if (i == n - 1)
                {
                    // Trace back to find a vertex in the cycle
                    std::vector<int> cycle;
                    std::unordered_set<int> visited;

                    int current = v;

                    // Find a vertex that is part of a cycle
                    while (current != -1 && visited.find(current) == visited.end())
                    {
                        visited.insert(current);
                        current = previous[vertexToIdx[current]];
                    }

                    // If we couldn't find a cycle, try with another edge
                    if (current == -1)
                    {
                        continue;
                    }

                    // Reconstruct the cycle
                    int cycleStart = current;
                    cycle.push_back(current);

                    current = previous[vertexToIdx[current]];
                    while (current != cycleStart)
                    {
                        cycle.push_back(current);
                        current = previous[vertexToIdx[current]];
                    }

                    cycle.push_back(cycleStart); // Complete the cycle
                    std::reverse(cycle.begin(), cycle.end());

                    return cycle;
                }
            }
        }
    }

    return {}; // No negative cycle found
}

/**
 * @brief Implementation of the Floyd-Warshall algorithm for finding all-pairs shortest paths
 *
 * Time Complexity: O(V^3)
 * Space Complexity: O(V^2)
 *
 * @param graph The graph
 * @param distances Output matrix of distances between all pairs of vertices
 * @param next Output matrix of the next vertex in the shortest path
 */
void floydWarshall(const IGraph &graph,
                   std::vector<std::vector<double>> &distances,
                   std::vector<std::vector<int>> &next)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    std::unordered_map<int, int> idxToVertex;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
        idxToVertex[i] = vertices[i];
    }

    // Initialize distance matrix
    distances = std::vector<std::vector<double>>(
        n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    // Initialize next vertex matrix
    next = std::vector<std::vector<int>>(
        n, std::vector<int>(n, -1));

    // Initialize with direct edge weights
    for (int i = 0; i < n; i++)
    {
        distances[i][i] = 0; // Distance to self is 0

        for (int neighbor : graph.getNeighbors(vertices[i]))
        {
            int j = vertexToIdx[neighbor];
            double weight = graph.getEdgeWeight(vertices[i], neighbor);
            distances[i][j] = weight;
            next[i][j] = j;
        }
    }

    // Main Floyd-Warshall algorithm
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (distances[i][k] != std::numeric_limits<double>::infinity() &&
                    distances[k][j] != std::numeric_limits<double>::infinity() &&
                    distances[i][k] + distances[k][j] < distances[i][j])
                {

                    distances[i][j] = distances[i][k] + distances[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

/**
 * @brief Reconstruct a path from the next vertex matrix
 *
 * @param next Matrix of next vertices
 * @param source Source vertex
 * @param destination Destination vertex
 * @param vertexToIdx Mapping from vertices to indices
 * @param idxToVertex Mapping from indices to vertices
 * @return Vector representing the path from source to destination
 */
std::vector<int> reconstructPathFloydWarshall(
    const std::vector<std::vector<int>> &next,
    int source, int destination,
    const std::unordered_map<int, int> &vertexToIdx,
    const std::unordered_map<int, int> &idxToVertex)
{

    int sourceIdx = vertexToIdx.at(source);
    int destIdx = vertexToIdx.at(destination);

    if (next[sourceIdx][destIdx] == -1)
    {
        return {}; // No path exists
    }

    std::vector<int> path = {source};
    int at = sourceIdx;

    while (at != destIdx)
    {
        at = next[at][destIdx];
        path.push_back(idxToVertex.at(at));
    }

    return path;
}

/**
 * @brief Implementation of Johnson's algorithm for all-pairs shortest paths
 *
 * Johnson's algorithm is more efficient than Floyd-Warshall for sparse graphs.
 * It uses a combination of Bellman-Ford and Dijkstra's algorithms.
 *
 * Time Complexity: O(V^2 log V + VE)
 * Space Complexity: O(V^2)
 *
 * @param graph The graph
 * @param distances Output matrix of distances between all pairs of vertices
 * @return True if successful, false if a negative cycle was detected
 */
bool johnsonsAlgorithm(const IGraph &graph,
                       std::vector<std::vector<double>> &distances)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    std::unordered_map<int, int> idxToVertex;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
        idxToVertex[i] = vertices[i];
    }

    // Create a copy of the graph with an additional vertex
    // connected to all other vertices with zero-weight edges
    auto augmentedGraph = std::make_unique<AdjacencyListGraph>(true, true);

    // Add the original vertices
    for (int v : vertices)
    {
        augmentedGraph->addVertex(v);
    }

    // Add a new source vertex
    int q = -1; // Use -1 as the new vertex ID (assuming original IDs are non-negative)
    augmentedGraph->addVertex(q);

    // Add edges from the original graph
    for (int u : vertices)
    {
        for (int v : graph.getNeighbors(u))
        {
            double weight = graph.getEdgeWeight(u, v);
            augmentedGraph->addEdge(u, v, weight);
        }
    }

    // Add zero-weight edges from the new vertex to all others
    for (int v : vertices)
    {
        augmentedGraph->addEdge(q, v, 0);
    }

    // Run Bellman-Ford from the new source vertex to compute h values
    std::vector<double> h;
    std::vector<int> previous;
    if (!bellmanFord(*augmentedGraph, q, h, previous))
    {
        return false; // Negative cycle detected
    }

    // Create a vertex-to-h mapping
    std::unordered_map<int, double> vertexToH;
    for (int i = 0; i < augmentedGraph->getVertexCount(); i++)
    {
        int v = (i < n) ? vertices[i] : q;
        vertexToH[v] = h[i];
    }

    // Reweight the original graph
    auto reweightedGraph = std::make_unique<AdjacencyListGraph>(graph.isDirected(), true);

    // Add vertices
    for (int v : vertices)
    {
        reweightedGraph->addVertex(v);
    }

    // Add reweighted edges
    for (int u : vertices)
    {
        for (int v : graph.getNeighbors(u))
        {
            double originalWeight = graph.getEdgeWeight(u, v);
            double reweightedEdge = originalWeight + vertexToH[u] - vertexToH[v];
            reweightedGraph->addEdge(u, v, reweightedEdge);
        }
    }

    // Initialize the distance matrix
    distances = std::vector<std::vector<double>>(
        n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    // Run Dijkstra from each vertex
    for (int i = 0; i < n; i++)
    {
        int u = vertices[i];
        std::vector<double> vertexDistances;
        std::vector<int> vertexPrevious = dijkstra(*reweightedGraph, u, vertexDistances);

        // Convert the reweighted distances back to original weights
        for (int j = 0; j < n; j++)
        {
            int v = vertices[j];
            if (vertexDistances[j] != std::numeric_limits<double>::infinity())
            {
                distances[i][j] = vertexDistances[j] - vertexToH[u] + vertexToH[v];
            }
        }
    }

    return true; // Success
}

/**
 * @brief Implementation of the A* search algorithm for finding the shortest path
 *
 * A* is an informed search algorithm that uses a heuristic to guide its search.
 *
 * Time Complexity: O(E log V) with a consistent heuristic
 * Space Complexity: O(V)
 *
 * @param graph The graph
 * @param start Start vertex
 * @param goal Goal vertex
 * @param heuristic Function that estimates the distance from a vertex to the goal
 * @return Vector representing the path from start to goal
 */
std::vector<int> aStarSearch(const IGraph &graph, int start, int goal,
                             std::function<double(int, int)> heuristic)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    std::unordered_map<int, int> idxToVertex;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
        idxToVertex[i] = vertices[i];
    }

    // Priority queue for A* - pair of (f-value, vertex)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        openSet;

    std::unordered_set<int> closedSet;
    std::vector<double> gScore(n, std::numeric_limits<double>::infinity()); // Cost from start to vertex
    std::vector<double> fScore(n, std::numeric_limits<double>::infinity()); // Estimated total cost
    std::vector<int> previous(n, -1);

    // Initialize start node
    gScore[vertexToIdx[start]] = 0;
    fScore[vertexToIdx[start]] = heuristic(start, goal);
    openSet.push({fScore[vertexToIdx[start]], start});

    while (!openSet.empty())
    {
        int current = openSet.top().second;
        openSet.pop();

        // If we've reached the goal
        if (current == goal)
        {
            std::vector<int> path;
            for (int at = goal; at != -1; at = previous[vertexToIdx[at]])
            {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // If already processed, skip
        if (closedSet.find(current) != closedSet.end())
        {
            continue;
        }

        closedSet.insert(current);

        // Process all neighbors
        for (int neighbor : graph.getNeighbors(current))
        {
            // If already processed, skip
            if (closedSet.find(neighbor) != closedSet.end())
            {
                continue;
            }

            // Calculate tentative g score
            double tentativeGScore = gScore[vertexToIdx[current]] + graph.getEdgeWeight(current, neighbor);

            // If this path is better than the previous one
            if (tentativeGScore < gScore[vertexToIdx[neighbor]])
            {
                previous[vertexToIdx[neighbor]] = current;
                gScore[vertexToIdx[neighbor]] = tentativeGScore;
                fScore[vertexToIdx[neighbor]] = gScore[vertexToIdx[neighbor]] + heuristic(neighbor, goal);

                // Add to open set if not already present
                openSet.push({fScore[vertexToIdx[neighbor]], neighbor});
            }
        }
    }

    return {}; // No path found
}

/**
 * @brief Implementation of bidirectional search for finding the shortest path
 *
 * This technique runs two simultaneous searches: one forward from the source
 * and one backward from the destination.
 *
 * Time Complexity: O(b^(d/2)) where b is the branching factor and d is the distance
 * Space Complexity: O(b^(d/2))
 *
 * @param graph The graph
 * @param source Source vertex
 * @param destination Destination vertex
 * @return Vector representing the path from source to destination
 */
std::vector<int> bidirectionalSearch(const IGraph &graph, int source, int destination)
{
    // Forward search data structures
    std::queue<int> forwardQueue;
    std::unordered_map<int, int> forwardParent;
    std::unordered_set<int> forwardVisited;

    // Backward search data structures
    std::queue<int> backwardQueue;
    std::unordered_map<int, int> backwardParent;
    std::unordered_set<int> backwardVisited;

    // Initialize
    forwardQueue.push(source);
    forwardVisited.insert(source);
    forwardParent[source] = -1;

    backwardQueue.push(destination);
    backwardVisited.insert(destination);
    backwardParent[destination] = -1;

    int meetingPoint = -1;

    while (!forwardQueue.empty() && !backwardQueue.empty())
    {
        // Expand forward search
        int currentForward = forwardQueue.front();
        forwardQueue.pop();

        for (int neighbor : graph.getNeighbors(currentForward))
        {
            if (forwardVisited.find(neighbor) == forwardVisited.end())
            {
                forwardQueue.push(neighbor);
                forwardVisited.insert(neighbor);
                forwardParent[neighbor] = currentForward;

                if (backwardVisited.find(neighbor) != backwardVisited.end())
                {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1)
        {
            break;
        }

        // Expand backward search
        int currentBackward = backwardQueue.front();
        backwardQueue.pop();

        for (int neighbor : graph.getNeighbors(currentBackward))
        {
            if (backwardVisited.find(neighbor) == backwardVisited.end())
            {
                backwardQueue.push(neighbor);
                backwardVisited.insert(neighbor);
                backwardParent[neighbor] = currentBackward;

                if (forwardVisited.find(neighbor) != forwardVisited.end())
                {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1)
        {
            break;
        }
    }

    if (meetingPoint == -1)
    {
        return {}; // No path found
    }

    // Reconstruct path
    std::vector<int> path;

    // Build path from source to meeting point
    for (int at = meetingPoint; at != -1; at = forwardParent[at])
    {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    // Build path from meeting point to destination
    for (int at = backwardParent[meetingPoint]; at != -1; at = backwardParent[at])
    {
        path.push_back(at);
    }

    return path;
}

/**
 * @brief Implementation of bidirectional Dijkstra's algorithm for finding the shortest path
 *
 * This version of bidirectional search uses Dijkstra's algorithm in both directions.
 *
 * Time Complexity: O(E log V)
 * Space Complexity: O(V)
 *
 * @param graph The graph
 * @param source Source vertex
 * @param destination Destination vertex
 * @param shortestDistance Output parameter for the shortest distance
 * @return Vector representing the path from source to destination
 */
std::vector<int> bidirectionalDijkstra(const IGraph &graph, int source, int destination,
                                       double &shortestDistance)
{
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a vertex-to-index mapping
    std::unordered_map<int, int> vertexToIdx;
    for (int i = 0; i < n; i++)
    {
        vertexToIdx[vertices[i]] = i;
    }

    // Forward search data structures
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        forwardPQ;
    std::vector<double> forwardDist(n, std::numeric_limits<double>::infinity());
    std::vector<int> forwardPrev(n, -1);
    std::unordered_set<int> forwardProcessed;

    // Backward search data structures
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        backwardPQ;
    std::vector<double> backwardDist(n, std::numeric_limits<double>::infinity());
    std::vector<int> backwardPrev(n, -1);
    std::unordered_set<int> backwardProcessed;

    // Initialize forward search
    forwardDist[vertexToIdx[source]] = 0;
    forwardPQ.push({0, source});

    // Initialize backward search
    backwardDist[vertexToIdx[destination]] = 0;
    backwardPQ.push({0, destination});

    // Keep track of the shortest path found so far
    double mu = std::numeric_limits<double>::infinity();
    int meetingPoint = -1;

    // Run bidirectional Dijkstra
    while (!forwardPQ.empty() && !backwardPQ.empty())
    {
        // Check if we can terminate early
        if (forwardPQ.top().first + backwardPQ.top().first >= mu)
        {
            break;
        }

        // Forward search step
        auto [forwardDist, u] = forwardPQ.top();
        forwardPQ.pop();

        if (forwardProcessed.find(u) != forwardProcessed.end())
        {
            continue;
        }

        forwardProcessed.insert(u);
        int uIdx = vertexToIdx[u];

        // Check if u has been processed by backward search
        if (backwardProcessed.find(u) != backwardProcessed.end())
        {
            double pathLength = forwardDist + backwardDist[uIdx];
            if (pathLength < mu)
            {
                mu = pathLength;
                meetingPoint = u;
            }
        }

        // Process neighbors
        for (int v : graph.getNeighbors(u))
        {
            int vIdx = vertexToIdx[v];
            double weight = graph.getEdgeWeight(u, v);

            if (forwardDist[uIdx] + weight < forwardDist[vIdx])
            {
                forwardDist[vIdx] = forwardDist[uIdx] + weight;
                forwardPrev[vIdx] = u;
                forwardPQ.push({forwardDist[vIdx], v});

                // Check if this gives a shorter path
                if (backwardProcessed.find(v) != backwardProcessed.end() &&
                    forwardDist[vIdx] + backwardDist[vIdx] < mu)
                {
                    mu = forwardDist[vIdx] + backwardDist[vIdx];
                    meetingPoint = v;
                }
            }
        }

        // Backward search step
        auto [backwardDist, v] = backwardPQ.top();
        backwardPQ.pop();

        if (backwardProcessed.find(v) != backwardProcessed.end())
        {
            continue;
        }

        backwardProcessed.insert(v);
        int vIdx = vertexToIdx[v];

        // Check if v has been processed by forward search
        if (forwardProcessed.find(v) != forwardProcessed.end())
        {
            double pathLength = forwardDist[vIdx] + backwardDist;
            if (pathLength < mu)
            {
                mu = pathLength;
                meetingPoint = v;
            }
        }

        // Process neighbors (in reverse)
        for (int u : graph.getNeighbors(v))
        {
            int uIdx = vertexToIdx[u];
            double weight = graph.getEdgeWeight(u, v); // Note: gets edge weight in reverse

            if (backwardDist[vIdx] + weight < backwardDist[uIdx])
            {
                backwardDist[uIdx] = backwardDist[vIdx] + weight;
                backwardPrev[uIdx] = v;
                backwardPQ.push({backwardDist[uIdx], u});

                // Check if this gives a shorter path
                if (forwardProcessed.find(u) != forwardProcessed.end() &&
                    forwardDist[uIdx] + backwardDist[uIdx] < mu)
                {
                    mu = forwardDist[uIdx] + backwardDist[uIdx];
                    meetingPoint = u;
                }
            }
        }
    }

    if (meetingPoint == -1)
    {
        shortestDistance = std::numeric_limits<double>::infinity();
        return {}; // No path found
    }

    // Reconstruct the path
    std::vector<int> path;

    // Forward path
    for (int at = meetingPoint; at != -1; at = forwardPrev[vertexToIdx[at]])
    {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    // Backward path (excluding meeting point)
    for (int at = backwardPrev[vertexToIdx[meetingPoint]]; at != -1; at = backwardPrev[vertexToIdx[at]])
    {
        path.push_back(at);
    }

    shortestDistance = mu;
    return path;
}

/****************************************************************************************
 * ADVANCED APPLICATIONS
 ****************************************************************************************/

/**
 * @brief A simplified navigation system implementation
 */
class NavigationSystem
{
private:
    std::unique_ptr<IGraph> roadNetwork;
    std::unordered_map<int, GeoCoordinate> nodeLocations;

public:
    NavigationSystem()
    {
        roadNetwork = std::make_unique<AdjacencyListGraph>(true, true);
    }

    void addNode(int nodeId, double latitude, double longitude)
    {
        roadNetwork->addVertex(nodeId);
        nodeLocations[nodeId] = GeoCoordinate(latitude, longitude);
    }

    void addRoad(int fromNode, int toNode, double distance, bool bidirectional = true)
    {
        roadNetwork->addEdge(fromNode, toNode, distance);
        if (bidirectional)
        {
            roadNetwork->addEdge(toNode, fromNode, distance);
        }
    }

    int findClosestNode(const GeoCoordinate &location) const
    {
        int closestNode = -1;
        double minDistance = std::numeric_limits<double>::infinity();

        for (const auto &[nodeId, nodeLocation] : nodeLocations)
        {
            double distance = haversineDistance(location, nodeLocation);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestNode = nodeId;
            }
        }

        return closestNode;
    }

    std::vector<int> findRoute(const GeoCoordinate &start, const GeoCoordinate &end)
    {
        // Find closest nodes to coordinates
        int startNode = findClosestNode(start);
        int endNode = findClosestNode(end);

        if (startNode == -1 || endNode == -1)
        {
            return {};
        }

        // Calculate route using A* with haversine distance as heuristic
        return aStarSearch(*roadNetwork, startNode, endNode,
                           [this](int node, int goal)
                           {
                               return haversineDistance(nodeLocations[node],
                                                        nodeLocations[goal]);
                           });
    }

    double estimateTravelTime(const std::vector<int> &route, double speedKmh = 50.0)
    {
        double distance = 0.0;

        for (size_t i = 0; i < route.size() - 1; i++)
        {
            distance += roadNetwork->getEdgeWeight(route[i], route[i + 1]);
        }

        // Convert distance to time (in hours)
        return distance / speedKmh;
    }

    void printRoute(const std::vector<int> &route)
    {
        if (route.empty())
        {
            std::cout << "No route found." << std::endl;
            return;
        }

        std::cout << "Route from " << route.front() << " to " << route.back() << ":" << std::endl;

        double totalDistance = 0.0;

        for (size_t i = 0; i < route.size() - 1; i++)
        {
            int current = route[i];
            int next = route[i + 1];
            double segmentDistance = roadNetwork->getEdgeWeight(current, next);

            std::cout << "  " << current << " -> " << next
                      << " (" << std::fixed << std::setprecision(2) << segmentDistance << " km)" << std::endl;

            totalDistance += segmentDistance;
        }

        double estimatedTime = estimateTravelTime(route);

        std::cout << "Total distance: " << std::fixed << std::setprecision(2)
                  << totalDistance << " km" << std::endl;
        std::cout << "Estimated travel time: " << std::fixed << std::setprecision(2)
                  << estimatedTime * 60 << " minutes" << std::endl;
    }
};

/**
 * @brief A network router implementation
 */
class NetworkRouter
{
private:
    std::unique_ptr<IGraph> network;
    std::vector<std::vector<double>> distanceTable;
    std::vector<std::vector<int>> nextHopTable;
    std::unordered_map<int, int> vertexToIdx;
    std::unordered_map<int, int> idxToVertex;

public:
    NetworkRouter()
    {
        network = std::make_unique<AdjacencyListGraph>(true, true);
    }

    void addRouter(int routerId)
    {
        network->addVertex(routerId);
    }

    void addLink(int router1, int router2, double bandwidth, double delay)
    {
        // Use delay as the weight for shortest path calculations
        network->addEdge(router1, router2, delay);
        network->addEdge(router2, router1, delay); // Bidirectional link
    }

    void updateRoutingTables()
    {
        // Create vertex-to-index mappings
        std::vector<int> vertices = network->getVertices();
        vertexToIdx.clear();
        idxToVertex.clear();

        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertexToIdx[vertices[i]] = i;
            idxToVertex[i] = vertices[i];
        }

        // Use Floyd-Warshall to compute all-pairs shortest paths
        floydWarshall(*network, distanceTable, nextHopTable);
    }

    int getNextHop(int source, int destination)
    {
        if (vertexToIdx.find(source) == vertexToIdx.end() ||
            vertexToIdx.find(destination) == vertexToIdx.end())
        {
            return -1; // Invalid router
        }

        int sourceIdx = vertexToIdx[source];
        int destIdx = vertexToIdx[destination];

        if (nextHopTable[sourceIdx][destIdx] == -1)
        {
            return -1; // No path exists
        }

        return idxToVertex[nextHopTable[sourceIdx][destIdx]];
    }

    std::vector<int> getPath(int source, int destination)
    {
        if (vertexToIdx.find(source) == vertexToIdx.end() ||
            vertexToIdx.find(destination) == vertexToIdx.end())
        {
            return {}; // Invalid router
        }

        std::vector<int> path;
        int sourceIdx = vertexToIdx[source];
        int destIdx = vertexToIdx[destination];

        if (nextHopTable[sourceIdx][destIdx] == -1)
        {
            return path; // No path exists
        }

        path.push_back(source);
        int at = sourceIdx;

        while (at != destIdx)
        {
            at = nextHopTable[at][destIdx];
            path.push_back(idxToVertex[at]);
        }

        return path;
    }

    void handleLinkFailure(int router1, int router2)
    {
        // Remove the link
        network->removeEdge(router1, router2);
        network->removeEdge(router2, router1);

        // Update routing tables
        updateRoutingTables();
    }

    void printRoutingTable(int router)
    {
        if (vertexToIdx.find(router) == vertexToIdx.end())
        {
            std::cout << "Invalid router ID." << std::endl;
            return;
        }

        int routerIdx = vertexToIdx[router];

        std::cout << "Routing table for Router " << router << ":" << std::endl;
        std::cout << "Destination\tNext Hop\tDelay" << std::endl;
        std::cout << "---------------------------------" << std::endl;

        for (size_t i = 0; i < distanceTable[routerIdx].size(); i++)
        {
            int dest = idxToVertex[i];

            if (dest == router)
            {
                continue; // Skip self
            }

            int nextHop = (nextHopTable[routerIdx][i] != -1) ? idxToVertex[nextHopTable[routerIdx][i]] : -1;

            double delay = distanceTable[routerIdx][i];

            std::cout << dest << "\t\t";

            if (nextHop == -1)
            {
                std::cout << "N/A\t\tINF" << std::endl;
            }
            else
            {
                std::cout << nextHop << "\t\t" << std::fixed << std::setprecision(2)
                          << delay << "ms" << std::endl;
            }
        }
    }
};

/**
 * @brief A circuit router implementation
 */
class CircuitRouter
{
private:
    std::unique_ptr<IGraph> gridGraph;
    int rows;
    int cols;

    // Convert 2D coordinates to node ID
    int coordToNode(const std::pair<int, int> &coord) const
    {
        return coord.first * cols + coord.second;
    }

    // Convert node ID to 2D coordinates
    std::pair<int, int> nodeToCoord(int node) const
    {
        return {node / cols, node % cols};
    }

public:
    CircuitRouter(int numRows, int numCols) : rows(numRows), cols(numCols)
    {
        gridGraph = std::make_unique<AdjacencyListGraph>(false, true);

        // Create grid graph
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                int node = coordToNode({r, c});
                gridGraph->addVertex(node);
            }
        }

        // Add edges for adjacent cells
        const int dr[] = {-1, 0, 1, 0};
        const int dc[] = {0, 1, 0, -1};

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                int node = coordToNode({r, c});

                for (int i = 0; i < 4; i++)
                {
                    int nr = r + dr[i];
                    int nc = c + dc[i];

                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols)
                    {
                        int neighbor = coordToNode({nr, nc});
                        gridGraph->addEdge(node, neighbor, 1.0);
                    }
                }
            }
        }
    }

    void blockCell(const std::pair<int, int> &coord)
    {
        int node = coordToNode(coord);

        // Remove all edges to/from this node
        std::vector<int> neighbors = gridGraph->getNeighbors(node);
        for (int neighbor : neighbors)
        {
            gridGraph->removeEdge(node, neighbor);
        }
    }

    std::vector<std::pair<int, int>> routeConnection(
        const std::pair<int, int> &start, const std::pair<int, int> &end)
    {

        // Convert grid coordinates to node IDs
        int startNode = coordToNode(start);
        int endNode = coordToNode(end);

        // Use A* search for routing
        auto path = aStarSearch(*gridGraph, startNode, endNode,
                                [this, &end](int node, int goal)
                                {
                                    auto nodeCoord = nodeToCoord(node);
                                    auto goalCoord = nodeToCoord(goal);
                                    return manhattanDistance(nodeCoord, goalCoord);
                                });

        // Convert node IDs back to coordinates
        std::vector<std::pair<int, int>> route;
        for (int node : path)
        {
            route.push_back(nodeToCoord(node));
        }

        return route;
    }

    void printGrid(const std::vector<std::pair<int, int>> &route = {})
    {
        // Create a set of route cells for O(1) lookup
        std::unordered_set<int> routeCells;
        for (const auto &coord : route)
        {
            routeCells.insert(coordToNode(coord));
        }

        // Print the grid
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                int node = coordToNode({r, c});
                char cell = '.'; // Empty cell

                // Check if this is a blocked cell
                if (gridGraph->getNeighbors(node).empty())
                {
                    cell = '#'; // Blocked cell
                }

                // Check if this is part of the route
                if (routeCells.find(node) != routeCells.end())
                {
                    cell = '*'; // Route cell
                }

                // Check if this is the start or end of the route
                if (!route.empty())
                {
                    if (node == coordToNode(route.front()))
                    {
                        cell = 'S'; // Start
                    }
                    else if (node == coordToNode(route.back()))
                    {
                        cell = 'E'; // End
                    }
                }

                std::cout << cell << ' ';
            }
            std::cout << std::endl;
        }
    }
};

/**
 * @brief A flight planner implementation
 */
class FlightPlanner
{
private:
    enum class CostMetric
    {
        DISTANCE,
        TIME,
        FUEL,
        PRICE
    };

    struct Airport
    {
        std::string code;
        std::string name;
        GeoCoordinate location;

        Airport(const std::string &c = "", const std::string &n = "",
                double lat = 0.0, double lon = 0.0)
            : code(c), name(n), location(lat, lon) {}
    };

    struct FlightInfo
    {
        int airline;      // Airline ID
        double distance;  // Distance in km
        double duration;  // Duration in hours
        double fuelUsage; // Fuel usage in gallons
        double price;     // Price in USD

        FlightInfo(int a = 0, double d = 0.0, double t = 0.0, double f = 0.0, double p = 0.0)
            : airline(a), distance(d), duration(t), fuelUsage(f), price(p) {}
    };

    std::unique_ptr<IGraph> airportNetwork;
    std::unordered_map<int, Airport> airports;
    std::unordered_map<std::pair<int, int>, FlightInfo, PairHash> flightInfo;

    // Hash function for std::pair
    struct PairHash
    {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &pair) const
        {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    void adjustEdgeWeights(CostMetric metric)
    {
        // Adjust edge weights based on the selected metric
        for (int u : airportNetwork->getVertices())
        {
            for (int v : airportNetwork->getNeighbors(u))
            {
                auto flight = flightInfo[{u, v}];
                double weight = 0.0;

                switch (metric)
                {
                case CostMetric::DISTANCE:
                    weight = flight.distance;
                    break;
                case CostMetric::TIME:
                    weight = flight.duration;
                    break;
                case CostMetric::FUEL:
                    weight = flight.fuelUsage;
                    break;
                case CostMetric::PRICE:
                    weight = flight.price;
                    break;
                }

                airportNetwork->setEdgeWeight(u, v, weight);
            }
        }
    }

public:
    FlightPlanner()
    {
        airportNetwork = std::make_unique<AdjacencyListGraph>(true, true);
    }

    void addAirport(int airportId, const std::string &code, const std::string &name,
                    double latitude, double longitude)
    {
        airportNetwork->addVertex(airportId);
        airports[airportId] = Airport(code, name, latitude, longitude);
    }

    void addFlight(int departureAirport, int arrivalAirport, int airline,
                   double distance, double duration, double fuelUsage, double price)
    {
        airportNetwork->addEdge(departureAirport, arrivalAirport, distance);
        flightInfo[{departureAirport, arrivalAirport}] =
            FlightInfo(airline, distance, duration, fuelUsage, price);
    }

    std::vector<int> planRoute(int departureAirport, int arrivalAirport,
                               const std::string &metricStr = "distance")
    {
        // Convert string to enum
        CostMetric metric;
        if (metricStr == "time")
        {
            metric = CostMetric::TIME;
        }
        else if (metricStr == "fuel")
        {
            metric = CostMetric::FUEL;
        }
        else if (metricStr == "price")
        {
            metric = CostMetric::PRICE;
        }
        else
        {
            metric = CostMetric::DISTANCE;
        }

        // Adjust edge weights based on the selected metric
        adjustEdgeWeights(metric);

        // Run Dijkstra's algorithm
        std::vector<double> distances;
        std::vector<int> previous = dijkstra(*airportNetwork, departureAirport, distances);

        // Create a vertex-to-index mapping
        std::vector<int> vertices = airportNetwork->getVertices();
        std::unordered_map<int, int> vertexToIdx;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertexToIdx[vertices[i]] = i;
        }

        // Reconstruct the path
        return reconstructPath(previous, departureAirport, arrivalAirport, vertexToIdx);
    }

    double getRouteCost(const std::vector<int> &route, const std::string &metricStr = "distance")
    {
        double cost = 0.0;

        for (size_t i = 0; i < route.size() - 1; i++)
        {
            int u = route[i];
            int v = route[i + 1];
            auto flight = flightInfo[{u, v}];

            if (metricStr == "time")
            {
                cost += flight.duration;
            }
            else if (metricStr == "fuel")
            {
                cost += flight.fuelUsage;
            }
            else if (metricStr == "price")
            {
                cost += flight.price;
            }
            else
            {
                cost += flight.distance;
            }
        }

        return cost;
    }

    void printRoute(const std::vector<int> &route, const std::string &metricStr = "distance")
    {
        if (route.empty())
        {
            std::cout << "No route found." << std::endl;
            return;
        }

        std::cout << "Flight route from " << airports[route.front()].code
                  << " to " << airports[route.back()].code << ":" << std::endl;

        for (size_t i = 0; i < route.size() - 1; i++)
        {
            int from = route[i];
            int to = route[i + 1];
            auto flight = flightInfo[{from, to}];

            std::cout << "  " << airports[from].code << " (" << airports[from].name << ")"
                      << " -> " << airports[to].code << " (" << airports[to].name << ")" << std::endl;

            std::cout << "    Distance: " << std::fixed << std::setprecision(0)
                      << flight.distance << " km, Duration: " << std::fixed << std::setprecision(2)
                      << flight.duration << " hours, Price: $" << std::fixed << std::setprecision(2)
                      << flight.price << std::endl;
        }

        double totalCost = getRouteCost(route, metricStr);
        std::string unit;

        if (metricStr == "time")
        {
            unit = " hours";
        }
        else if (metricStr == "fuel")
        {
            unit = " gallons";
        }
        else if (metricStr == "price")
        {
            unit = " USD";
        }
        else
        {
            unit = " km";
        }

        std::cout << "Total " << metricStr << ": " << std::fixed << std::setprecision(2)
                  << totalCost << unit << std::endl;
    }

    std::vector<int> planMultiCityRoute(const std::vector<int> &cities)
    {
        if (cities.size() <= 1)
        {
            return cities;
        }

        // For each pair of cities, compute the shortest path
        std::vector<std::vector<int>> pathSegments;

        for (size_t i = 0; i < cities.size() - 1; i++)
        {
            int from = cities[i];
            int to = cities[i + 1];

            std::vector<double> distances;
            std::vector<int> previous = dijkstra(*airportNetwork, from, distances);

            // Create a vertex-to-index mapping
            std::vector<int> vertices = airportNetwork->getVertices();
            std::unordered_map<int, int> vertexToIdx;
            for (size_t j = 0; j < vertices.size(); j++)
            {
                vertexToIdx[vertices[j]] = j;
            }

            // Reconstruct the path
            auto path = reconstructPath(previous, from, to, vertexToIdx);

            if (path.empty())
            {
                std::cout << "No path found between " << airports[from].code
                          << " and " << airports[to].code << "." << std::endl;
                return {};
            }

            pathSegments.push_back(path);
        }

        // Merge the path segments
        std::vector<int> fullRoute;

        for (size_t i = 0; i < pathSegments.size(); i++)
        {
            // Add all vertices except the last one (it will be added as the first of the next segment)
            for (size_t j = 0; j < pathSegments[i].size() - (i < pathSegments.size() - 1 ? 1 : 0); j++)
            {
                fullRoute.push_back(pathSegments[i][j]);
            }
        }

        return fullRoute;
    }
};

/****************************************************************************************
 * PERFORMANCE BENCHMARKS
 ****************************************************************************************/

/**
 * @brief Benchmark different shortest path algorithms
 *
 * @param graphSizes Vector of graph sizes to test
 * @param edgeDensities Vector of edge densities to test
 * @param iterations Number of iterations for each test
 */
void benchmarkShortestPathAlgorithms(
    const std::vector<int> &graphSizes,
    const std::vector<double> &edgeDensities,
    int iterations = 5)
{

    std::cout << "===== Shortest Path Algorithm Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(10) << "Edges"
              << std::setw(15) << "Density"
              << std::setw(15) << "Dijkstra (ms)"
              << std::setw(20) << "Bellman-Ford (ms)"
              << std::setw(20) << "Floyd-Warshall (ms)"
              << std::endl;

    std::cout << std::string(90, '-') << std::endl;

    // Test different graph sizes and densities
    for (int size : graphSizes)
    {
        for (double density : edgeDensities)
        {
            double dijkstraTime = 0.0;
            double bellmanFordTime = 0.0;
            double floydWarshallTime = 0.0;
            int totalEdges = 0;

            for (int iter = 0; iter < iterations; ++iter)
            {
                // Generate a random weighted graph
                auto graph = std::make_unique<AdjacencyListGraph>(true, true);

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
                    for (int j = 0; j < size; j++)
                    {
                        if (i != j && distr(gen) < density)
                        {
                            graph->addEdge(i, j, weightDistr(gen));
                        }
                    }
                }

                totalEdges = graph->getEdgeCount();

                // Pick a random source vertex
                int source = 0;

                // Benchmark Dijkstra's algorithm
                auto startDijkstra = std::chrono::high_resolution_clock::now();
                std::vector<double> distances;
                auto previous = dijkstra(*graph, source, distances);
                auto endDijkstra = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> dijkstraDuration = endDijkstra - startDijkstra;
                dijkstraTime += dijkstraDuration.count();

                // Benchmark Bellman-Ford algorithm
                auto startBellmanFord = std::chrono::high_resolution_clock::now();
                std::vector<double> bfDistances;
                std::vector<int> bfPrevious;
                bellmanFord(*graph, source, bfDistances, bfPrevious);
                auto endBellmanFord = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> bellmanFordDuration = endBellmanFord - startBellmanFord;
                bellmanFordTime += bellmanFordDuration.count();

                // Benchmark Floyd-Warshall algorithm
                auto startFloydWarshall = std::chrono::high_resolution_clock::now();
                std::vector<std::vector<double>> fwDistances;
                std::vector<std::vector<int>> fwNext;
                floydWarshall(*graph, fwDistances, fwNext);
                auto endFloydWarshall = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> floydWarshallDuration = endFloydWarshall - startFloydWarshall;
                floydWarshallTime += floydWarshallDuration.count();
            }

            // Calculate averages
            dijkstraTime /= iterations;
            bellmanFordTime /= iterations;
            floydWarshallTime /= iterations;

            // Print results
            std::cout << std::left << std::setw(10) << size
                      << std::setw(10) << totalEdges
                      << std::setw(15) << std::fixed << std::setprecision(2) << density
                      << std::setw(15) << std::fixed << std::setprecision(2) << dijkstraTime
                      << std::setw(20) << std::fixed << std::setprecision(2) << bellmanFordTime
                      << std::setw(20) << std::fixed << std::setprecision(2) << floydWarshallTime
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark path finding with different single-source algorithms
 *
 * @param graphSizes Vector of graph sizes to test
 * @param iterations Number of iterations for each test
 */
void benchmarkPathFindingAlgorithms(
    const std::vector<int> &graphSizes,
    int iterations = 5)
{

    std::cout << "===== Path Finding Algorithm Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(10) << "Edges"
              << std::setw(20) << "Dijkstra (ms)"
              << std::setw(20) << "A* Search (ms)"
              << std::setw(20) << "Bidirectional (ms)"
              << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // Edge density for tests
    const double density = 0.1;

    // Test different graph sizes
    for (int size : graphSizes)
    {
        double dijkstraTime = 0.0;
        double aStarTime = 0.0;
        double bidirTime = 0.0;
        int totalEdges = 0;

        for (int iter = 0; iter < iterations; ++iter)
        {
            // Generate a random graph for a 2D grid
            auto graph = std::make_unique<AdjacencyListGraph>(false, true);

            // Assign (x,y) coordinates to each vertex
            std::unordered_map<int, std::pair<int, int>> coordinates;

            int gridSize = static_cast<int>(std::sqrt(size));
            int vertexCount = 0;

            // Add vertices
            for (int x = 0; x < gridSize && vertexCount < size; x++)
            {
                for (int y = 0; y < gridSize && vertexCount < size; y++)
                {
                    int vertex = vertexCount++;
                    graph->addVertex(vertex);
                    coordinates[vertex] = {x, y};
                }
            }

            // Add edges (use a grid structure with some random edges)
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distr(0.0, 1.0);

            for (int i = 0; i < vertexCount; i++)
            {
                auto [x1, y1] = coordinates[i];

                for (int j = i + 1; j < vertexCount; j++)
                {
                    auto [x2, y2] = coordinates[j];

                    // Add edge if vertices are adjacent in the grid
                    if ((std::abs(x1 - x2) == 1 && y1 == y2) ||
                        (std::abs(y1 - y2) == 1 && x1 == x2))
                    {
                        double weight = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
                        graph->addEdge(i, j, weight);
                    }
                    // Add some random edges
                    else if (distr(gen) < 0.01)
                    {
                        double weight = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
                        graph->addEdge(i, j, weight);
                    }
                }
            }

            totalEdges = graph->getEdgeCount();

            // Pick source and destination vertices (opposite corners)
            int source = 0;
            int destination = vertexCount - 1;

            // Benchmark Dijkstra's algorithm
            auto startDijkstra = std::chrono::high_resolution_clock::now();
            double dijkstraDist;
            auto dijkstraPath = dijkstraSingleTarget(*graph, source, destination, dijkstraDist);
            auto endDijkstra = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dijkstraDuration = endDijkstra - startDijkstra;
            dijkstraTime += dijkstraDuration.count();

            // Benchmark A* search
            auto startAStar = std::chrono::high_resolution_clock::now();
            auto aStarPath = aStarSearch(*graph, source, destination,
                                         [&coordinates](int a, int b)
                                         {
                                             auto [x1, y1] = coordinates[a];
                                             auto [x2, y2] = coordinates[b];
                                             return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
                                         });
            auto endAStar = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> aStarDuration = endAStar - startAStar;
            aStarTime += aStarDuration.count();

            // Benchmark Bidirectional search
            auto startBidir = std::chrono::high_resolution_clock::now();
            double bidirDist;
            auto bidirPath = bidirectionalDijkstra(*graph, source, destination, bidirDist);
            auto endBidir = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> bidirDuration = endBidir - startBidir;
            bidirTime += bidirDuration.count();
        }

        // Calculate averages
        dijkstraTime /= iterations;
        aStarTime /= iterations;
        bidirTime /= iterations;

        // Print results
        std::cout << std::left << std::setw(10) << size
                  << std::setw(10) << totalEdges
                  << std::setw(20) << std::fixed << std::setprecision(2) << dijkstraTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << aStarTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << bidirTime
                  << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark graph with negative edges
 *
 * @param graphSizes Vector of graph sizes to test
 * @param iterations Number of iterations for each test
 */
void benchmarkNegativeEdgeGraphs(
    const std::vector<int> &graphSizes,
    int iterations = 5)
{

    std::cout << "===== Negative Edge Graph Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(15) << "Edges"
              << std::setw(15) << "Neg. Edges"
              << std::setw(20) << "Bellman-Ford (ms)"
              << std::setw(20) << "Floyd-Warshall (ms)"
              << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // Edge density for tests
    const double density = 0.1;

    // Test different graph sizes
    for (int size : graphSizes)
    {
        double bellmanFordTime = 0.0;
        double floydWarshallTime = 0.0;
        int totalEdges = 0;
        int negativeEdges = 0;

        for (int iter = 0; iter < iterations; ++iter)
        {
            // Generate a random weighted graph with some negative edges
            auto graph = std::make_unique<AdjacencyListGraph>(true, true);

            // Add vertices
            for (int i = 0; i < size; i++)
            {
                graph->addVertex(i);
            }

            // Add random edges with some negative weights
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distr(0.0, 1.0);
            std::uniform_real_distribution<> weightDistr(-50.0, 100.0);

            negativeEdges = 0;

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (i != j && distr(gen) < density)
                    {
                        double weight = weightDistr(gen);
                        graph->addEdge(i, j, weight);

                        if (weight < 0)
                        {
                            negativeEdges++;
                        }
                    }
                }
            }

            totalEdges = graph->getEdgeCount();

            // Ensure no negative cycles
            std::vector<double> testDist;
            std::vector<int> testPrev;
            if (!bellmanFord(*graph, 0, testDist, testPrev))
            {
                // Graph has a negative cycle, try again
                iter--;
                continue;
            }

            // Pick a random source vertex
            int source = 0;

            // Benchmark Bellman-Ford algorithm
            auto startBellmanFord = std::chrono::high_resolution_clock::now();
            std::vector<double> bfDistances;
            std::vector<int> bfPrevious;
            bellmanFord(*graph, source, bfDistances, bfPrevious);
            auto endBellmanFord = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> bellmanFordDuration = endBellmanFord - startBellmanFord;
            bellmanFordTime += bellmanFordDuration.count();

            // Benchmark Floyd-Warshall algorithm
            auto startFloydWarshall = std::chrono::high_resolution_clock::now();
            std::vector < st
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
                 * @brief Comprehensive Shortest Path Algorithms Implementation for #DSAin45 Day 31
                 *
                 * This file provides implementations of:
                 * - Dijkstra's Algorithm
                 * - Bellman-Ford Algorithm
                 * - Floyd-Warshall Algorithm
                 * - Johnson's Algorithm
                 * - A* Search Algorithm
                 * - Bidirectional Search
                 * - Various optimizations and applications
                 * - Performance benchmarks
                 *
                 * @author #DSAin45 Course
                 * @date Day 31 of 45-Day DSA Challenge
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
             * UTILITY CLASSES FOR SHORTEST PATH ALGORITHMS
             ****************************************************************************************/

            /**
             * @brief Structure to represent a geographic coordinate
             */
            struct GeoCoordinate
            {
                double latitude;
                double longitude;

                GeoCoordinate(double lat = 0.0, double lon = 0.0)
                    : latitude(lat), longitude(lon) {}
            };

            /**
             * @brief Calculate Haversine distance between two geographic coordinates
             *
             * @param a First coordinate
             * @param b Second coordinate
             * @return Distance in kilometers
             */
            double haversineDistance(const GeoCoordinate &a, const GeoCoordinate &b)
            {
                const double EARTH_RADIUS_KM = 6371.0;

                // Convert latitude and longitude from degrees to radians
                double lat1 = a.latitude * M_PI / 180.0;
                double lon1 = a.longitude * M_PI / 180.0;
                double lat2 = b.latitude * M_PI / 180.0;
                double lon2 = b.longitude * M_PI / 180.0;

                // Haversine formula
                double dLat = lat2 - lat1;
                double dLon = lon2 - lon1;
                double sinDLat = sin(dLat / 2);
                double sinDLon = sin(dLon / 2);

                double a1 = sinDLat * sinDLat + cos(lat1) * cos(lat2) * sinDLon * sinDLon;
                double c = 2 * atan2(sqrt(a1), sqrt(1 - a1));

                return EARTH_RADIUS_KM * c;
            }

            /**
             * @brief Calculate Manhattan distance between two points
             *
             * @param a First point (x, y)
             * @param b Second point (x, y)
             * @return Manhattan distance
             */
            double manhattanDistance(const std::pair<int, int> &a, const std::pair<int, int> &b)
            {
                return std::abs(a.first - b.first) + std::abs(a.second - b.second);
            }

            /**
             * @brief Calculate Euclidean distance between two points
             *
             * @param a First point (x, y)
             * @param b Second point (x, y)
             * @return Euclidean distance
             */
            double euclideanDistance(const std::pair<int, int> &a, const std::pair<int, int> &b)
            {
                double dx = a.first - b.first;
                double dy = a.second - b.second;
                return std::sqrt(dx * dx + dy * dy);
            }

            /****************************************************************************************
             * SHORTEST PATH ALGORITHMS
             ****************************************************************************************/

            /**
             * @brief Implementation of Dijkstra's algorithm for finding the shortest paths
             *        from a source vertex to all other vertices
             *
             * Time Complexity: O(E log V) with binary heap
             * Space Complexity: O(V)
             *
             * @param graph The graph
             * @param source Source vertex
             * @param distances Output vector of distances from source to each vertex
             * @return Vector of previous vertices in the shortest paths
             */
            std::vector<int> dijkstra(const IGraph &graph, int source,
                                      std::vector<double> &distances)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                }

                // Initialize distances and previous vertices
                distances.assign(n, std::numeric_limits<double>::infinity());
                distances[vertexToIdx[source]] = 0;

                std::vector<int> previous(n, -1); // Store previous vertex in shortest path

                // Min priority queue: pair of (distance, vertex)
                std::priority_queue<std::pair<double, int>,
                                    std::vector<std::pair<double, int>>,
                                    std::greater<std::pair<double, int>>>
                    pq;

                // Add source to priority queue
                pq.push({0.0, source});

                // Set to keep track of processed vertices
                std::unordered_set<int> processed;

                while (!pq.empty())
                {
                    int u = pq.top().second; // Get vertex with minimum distance
                    pq.pop();

                    // Skip if already processed
                    if (processed.find(u) != processed.end())
                    {
                        continue;
                    }

                    // Mark as processed
                    processed.insert(u);

                    // Process all neighbors of u
                    for (int v : graph.getNeighbors(u))
                    {
                        double weight = graph.getEdgeWeight(u, v);
                        int vIdx = vertexToIdx[v];
                        int uIdx = vertexToIdx[u];

                        // Relaxation step: If a shorter path to v is found
                        if (distances[uIdx] + weight < distances[vIdx])
                        {
                            distances[vIdx] = distances[uIdx] + weight;
                            previous[vIdx] = u;

                            // Add to priority queue
                            pq.push({distances[vIdx], v});
                        }
                    }
                }

                return previous;
            }

            /**
             * @brief Reconstruct a path from source to destination using the previous vertices array
             *
             * @param previous Vector of previous vertices
             * @param source Source vertex
             * @param destination Destination vertex
             * @param vertexToIdx Mapping from vertices to indices
             * @return Vector representing the path from source to destination
             */
            std::vector<int> reconstructPath(const std::vector<int> &previous,
                                             int source, int destination,
                                             const std::unordered_map<int, int> &vertexToIdx)
            {
                std::vector<int> path;

                // Check if destination is reachable
                if (previous[vertexToIdx.at(destination)] == -1 && source != destination)
                {
                    return path; // Empty path - unreachable
                }

                // Start from destination and work backwards
                for (int at = destination; at != -1; at = previous[vertexToIdx.at(at)])
                {
                    path.push_back(at);

                    // If we've reached the source, we're done
                    if (at == source)
                    {
                        break;
                    }
                }

                // Check if we actually reached the source
                if (path.empty() || path.back() != source)
                {
                    return {}; // No path exists
                }

                // Reverse path to get it from source to destination
                std::reverse(path.begin(), path.end());

                return path;
            }

            /**
             * @brief Single-target Dijkstra's algorithm
             *
             * This version stops once the target is processed, which can be more efficient
             * when only a specific destination is of interest.
             *
             * @param graph The graph
             * @param source Source vertex
             * @param target Target vertex
             * @param shortestDistance Output parameter to store the shortest distance
             * @return Vector representing the shortest path
             */
            std::vector<int> dijkstraSingleTarget(const IGraph &graph, int source, int target,
                                                  double &shortestDistance)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                }

                // Initialize distances and previous vertices
                std::vector<double> distances(n, std::numeric_limits<double>::infinity());
                distances[vertexToIdx[source]] = 0;

                std::vector<int> previous(n, -1); // Store previous vertex in shortest path

                // Min priority queue: pair of (distance, vertex)
                std::priority_queue<std::pair<double, int>,
                                    std::vector<std::pair<double, int>>,
                                    std::greater<std::pair<double, int>>>
                    pq;

                // Add source to priority queue
                pq.push({0.0, source});

                // Set to keep track of processed vertices
                std::unordered_set<int> processed;

                while (!pq.empty())
                {
                    auto [dist, u] = pq.top(); // Get vertex with minimum distance
                    pq.pop();

                    // If we've reached the target, we're done
                    if (u == target)
                    {
                        shortestDistance = dist;
                        return reconstructPath(previous, source, target, vertexToIdx);
                    }

                    // Skip if already processed
                    if (processed.find(u) != processed.end())
                    {
                        continue;
                    }

                    // Mark as processed
                    processed.insert(u);

                    // Process all neighbors of u
                    for (int v : graph.getNeighbors(u))
                    {
                        double weight = graph.getEdgeWeight(u, v);
                        int vIdx = vertexToIdx[v];
                        int uIdx = vertexToIdx[u];

                        // Relaxation step: If a shorter path to v is found
                        if (distances[uIdx] + weight < distances[vIdx])
                        {
                            distances[vIdx] = distances[uIdx] + weight;
                            previous[vIdx] = u;

                            // Add to priority queue
                            pq.push({distances[vIdx], v});
                        }
                    }
                }

                // If we get here, target is unreachable
                shortestDistance = std::numeric_limits<double>::infinity();
                return {};
            }

            /**
             * @brief Implementation of the Bellman-Ford algorithm for finding the shortest paths
             *        from a source vertex to all other vertices (works with negative edge weights)
             *
             * Time Complexity: O(V * E)
             * Space Complexity: O(V)
             *
             * @param graph The graph
             * @param source Source vertex
             * @param distances Output vector of distances from source to each vertex
             * @param previous Output vector of previous vertices in the shortest paths
             * @return True if no negative cycles, false otherwise
             */
            bool bellmanFord(const IGraph &graph, int source,
                             std::vector<double> &distances,
                             std::vector<int> &previous)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                }

                // Initialize distances
                distances.assign(n, std::numeric_limits<double>::infinity());
                distances[vertexToIdx[source]] = 0;

                // Initialize previous vertices
                previous.assign(n, -1);

                // Get all edges from the graph
                std::vector<std::tuple<int, int, double>> edges;

                for (int u : vertices)
                {
                    for (int v : graph.getNeighbors(u))
                    {
                        double weight = graph.getEdgeWeight(u, v);
                        edges.push_back({u, v, weight});
                    }
                }

                // Relax all edges V-1 times
                for (int i = 0; i < n - 1; i++)
                {
                    bool anyChange = false;

                    for (const auto &[u, v, weight] : edges)
                    {
                        int uIdx = vertexToIdx[u];
                        int vIdx = vertexToIdx[v];

                        // If u is not reachable, skip
                        if (distances[uIdx] == std::numeric_limits<double>::infinity())
                        {
                            continue;
                        }

                        // Relaxation step
                        if (distances[uIdx] + weight < distances[vIdx])
                        {
                            distances[vIdx] = distances[uIdx] + weight;
                            previous[vIdx] = u;
                            anyChange = true;
                        }
                    }

                    // Early termination if no changes were made in this iteration
                    if (!anyChange)
                    {
                        break;
                    }
                }

                // Check for negative cycles
                for (const auto &[u, v, weight] : edges)
                {
                    int uIdx = vertexToIdx[u];
                    int vIdx = vertexToIdx[v];

                    if (distances[uIdx] != std::numeric_limits<double>::infinity() &&
                        distances[uIdx] + weight < distances[vIdx])
                    {
                        // Negative cycle detected
                        return false;
                    }
                }

                return true; // No negative cycles
            }

            /**
             * @brief Find a negative cycle in the graph using Bellman-Ford
             *
             * @param graph The graph
             * @param source Source vertex
             * @return Vector representing a negative cycle, or empty if none exists
             */
            std::vector<int> findNegativeCycle(const IGraph &graph, int source)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                std::unordered_map<int, int> idxToVertex;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                    idxToVertex[i] = vertices[i];
                }

                // Initialize distances and previous vertices
                std::vector<double> distances(n, std::numeric_limits<double>::infinity());
                distances[vertexToIdx[source]] = 0;

                std::vector<int> previous(n, -1);

                // Get all edges from the graph
                std::vector<std::tuple<int, int, double>> edges;

                for (int u : vertices)
                {
                    for (int v : graph.getNeighbors(u))
                    {
                        double weight = graph.getEdgeWeight(u, v);
                        edges.push_back({u, v, weight});
                    }
                }

                // Relax all edges V times
                for (int i = 0; i < n; i++)
                {
                    for (const auto &[u, v, weight] : edges)
                    {
                        int uIdx = vertexToIdx[u];
                        int vIdx = vertexToIdx[v];

                        // If u is not reachable, skip
                        if (distances[uIdx] == std::numeric_limits<double>::infinity())
                        {
                            continue;
                        }

                        // Relaxation step
                        if (distances[uIdx] + weight < distances[vIdx])
                        {
                            distances[vIdx] = distances[uIdx] + weight;
                            previous[vIdx] = u;

                            // If we're on the nth iteration, a negative cycle exists
                            if (i == n - 1)
                            {
                                // Trace back to find a vertex in the cycle
                                std::vector<int> cycle;
                                std::unordered_set<int> visited;

                                int current = v;

                                // Find a vertex that is part of a cycle
                                while (current != -1 && visited.find(current) == visited.end())
                                {
                                    visited.insert(current);
                                    current = previous[vertexToIdx[current]];
                                }

                                // If we couldn't find a cycle, try with another edge
                                if (current == -1)
                                {
                                    continue;
                                }

                                // Reconstruct the cycle
                                int cycleStart = current;
                                cycle.push_back(current);

                                current = previous[vertexToIdx[current]];
                                while (current != cycleStart)
                                {
                                    cycle.push_back(current);
                                    current = previous[vertexToIdx[current]];
                                }

                                cycle.push_back(cycleStart); // Complete the cycle
                                std::reverse(cycle.begin(), cycle.end());

                                return cycle;
                            }
                        }
                    }
                }

                return {}; // No negative cycle found
            }

            /**
             * @brief Implementation of the Floyd-Warshall algorithm for finding all-pairs shortest paths
             *
             * Time Complexity: O(V^3)
             * Space Complexity: O(V^2)
             *
             * @param graph The graph
             * @param distances Output matrix of distances between all pairs of vertices
             * @param next Output matrix of the next vertex in the shortest path
             */
            void floydWarshall(const IGraph &graph,
                               std::vector<std::vector<double>> &distances,
                               std::vector<std::vector<int>> &next)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                std::unordered_map<int, int> idxToVertex;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                    idxToVertex[i] = vertices[i];
                }

                // Initialize distance matrix
                distances = std::vector<std::vector<double>>(
                    n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

                // Initialize next vertex matrix
                next = std::vector<std::vector<int>>(
                    n, std::vector<int>(n, -1));

                // Initialize with direct edge weights
                for (int i = 0; i < n; i++)
                {
                    distances[i][i] = 0; // Distance to self is 0

                    for (int neighbor : graph.getNeighbors(vertices[i]))
                    {
                        int j = vertexToIdx[neighbor];
                        double weight = graph.getEdgeWeight(vertices[i], neighbor);
                        distances[i][j] = weight;
                        next[i][j] = j;
                    }
                }

                // Main Floyd-Warshall algorithm
                for (int k = 0; k < n; k++)
                {
                    for (int i = 0; i < n; i++)
                    {
                        for (int j = 0; j < n; j++)
                        {
                            if (distances[i][k] != std::numeric_limits<double>::infinity() &&
                                distances[k][j] != std::numeric_limits<double>::infinity() &&
                                distances[i][k] + distances[k][j] < distances[i][j])
                            {

                                distances[i][j] = distances[i][k] + distances[k][j];
                                next[i][j] = next[i][k];
                            }
                        }
                    }
                }
            }

            /**
             * @brief Reconstruct a path from the next vertex matrix
             *
             * @param next Matrix of next vertices
             * @param source Source vertex
             * @param destination Destination vertex
             * @param vertexToIdx Mapping from vertices to indices
             * @param idxToVertex Mapping from indices to vertices
             * @return Vector representing the path from source to destination
             */
            std::vector<int> reconstructPathFloydWarshall(
                const std::vector<std::vector<int>> &next,
                int source, int destination,
                const std::unordered_map<int, int> &vertexToIdx,
                const std::unordered_map<int, int> &idxToVertex)
            {

                int sourceIdx = vertexToIdx.at(source);
                int destIdx = vertexToIdx.at(destination);

                if (next[sourceIdx][destIdx] == -1)
                {
                    return {}; // No path exists
                }

                std::vector<int> path = {source};
                int at = sourceIdx;

                while (at != destIdx)
                {
                    at = next[at][destIdx];
                    path.push_back(idxToVertex.at(at));
                }

                return path;
            }

            /**
             * @brief Implementation of Johnson's algorithm for all-pairs shortest paths
             *
             * Johnson's algorithm is more efficient than Floyd-Warshall for sparse graphs.
             * It uses a combination of Bellman-Ford and Dijkstra's algorithms.
             *
             * Time Complexity: O(V^2 log V + VE)
             * Space Complexity: O(V^2)
             *
             * @param graph The graph
             * @param distances Output matrix of distances between all pairs of vertices
             * @return True if successful, false if a negative cycle was detected
             */
            bool johnsonsAlgorithm(const IGraph &graph,
                                   std::vector<std::vector<double>> &distances)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                std::unordered_map<int, int> idxToVertex;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                    idxToVertex[i] = vertices[i];
                }

                // Create a copy of the graph with an additional vertex
                // connected to all other vertices with zero-weight edges
                auto augmentedGraph = std::make_unique<AdjacencyListGraph>(true, true);

                // Add the original vertices
                for (int v : vertices)
                {
                    augmentedGraph->addVertex(v);
                }

                // Add a new source vertex
                int q = -1; // Use -1 as the new vertex ID (assuming original IDs are non-negative)
                augmentedGraph->addVertex(q);

                // Add edges from the original graph
                for (int u : vertices)
                {
                    for (int v : graph.getNeighbors(u))
                    {
                        double weight = graph.getEdgeWeight(u, v);
                        augmentedGraph->addEdge(u, v, weight);
                    }
                }

                // Add zero-weight edges from the new vertex to all others
                for (int v : vertices)
                {
                    augmentedGraph->addEdge(q, v, 0);
                }

                // Run Bellman-Ford from the new source vertex to compute h values
                std::vector<double> h;
                std::vector<int> previous;
                if (!bellmanFord(*augmentedGraph, q, h, previous))
                {
                    return false; // Negative cycle detected
                }

                // Create a vertex-to-h mapping
                std::unordered_map<int, double> vertexToH;
                for (int i = 0; i < augmentedGraph->getVertexCount(); i++)
                {
                    int v = (i < n) ? vertices[i] : q;
                    vertexToH[v] = h[i];
                }

                // Reweight the original graph
                auto reweightedGraph = std::make_unique<AdjacencyListGraph>(graph.isDirected(), true);

                // Add vertices
                for (int v : vertices)
                {
                    reweightedGraph->addVertex(v);
                }

                // Add reweighted edges
                for (int u : vertices)
                {
                    for (int v : graph.getNeighbors(u))
                    {
                        double originalWeight = graph.getEdgeWeight(u, v);
                        double reweightedEdge = originalWeight + vertexToH[u] - vertexToH[v];
                        reweightedGraph->addEdge(u, v, reweightedEdge);
                    }
                }

                // Initialize the distance matrix
                distances = std::vector<std::vector<double>>(
                    n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

                // Run Dijkstra from each vertex
                for (int i = 0; i < n; i++)
                {
                    int u = vertices[i];
                    std::vector<double> vertexDistances;
                    std::vector<int> vertexPrevious = dijkstra(*reweightedGraph, u, vertexDistances);

                    // Convert the reweighted distances back to original weights
                    for (int j = 0; j < n; j++)
                    {
                        int v = vertices[j];
                        if (vertexDistances[j] != std::numeric_limits<double>::infinity())
                        {
                            distances[i][j] = vertexDistances[j] - vertexToH[u] + vertexToH[v];
                        }
                    }
                }

                return true; // Success
            }

            /**
             * @brief Implementation of the A* search algorithm for finding the shortest path
             *
             * A* is an informed search algorithm that uses a heuristic to guide its search.
             *
             * Time Complexity: O(E log V) with a consistent heuristic
             * Space Complexity: O(V)
             *
             * @param graph The graph
             * @param start Start vertex
             * @param goal Goal vertex
             * @param heuristic Function that estimates the distance from a vertex to the goal
             * @return Vector representing the path from start to goal
             */
            std::vector<int> aStarSearch(const IGraph &graph, int start, int goal,
                                         std::function<double(int, int)> heuristic)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                std::unordered_map<int, int> idxToVertex;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                    idxToVertex[i] = vertices[i];
                }

                // Priority queue for A* - pair of (f-value, vertex)
                std::priority_queue<std::pair<double, int>,
                                    std::vector<std::pair<double, int>>,
                                    std::greater<std::pair<double, int>>>
                    openSet;

                std::unordered_set<int> closedSet;
                std::vector<double> gScore(n, std::numeric_limits<double>::infinity()); // Cost from start to vertex
                std::vector<double> fScore(n, std::numeric_limits<double>::infinity()); // Estimated total cost
                std::vector<int> previous(n, -1);

                // Initialize start node
                gScore[vertexToIdx[start]] = 0;
                fScore[vertexToIdx[start]] = heuristic(start, goal);
                openSet.push({fScore[vertexToIdx[start]], start});

                while (!openSet.empty())
                {
                    int current = openSet.top().second;
                    openSet.pop();

                    // If we've reached the goal
                    if (current == goal)
                    {
                        std::vector<int> path;
                        for (int at = goal; at != -1; at = previous[vertexToIdx[at]])
                        {
                            path.push_back(at);
                        }
                        std::reverse(path.begin(), path.end());
                        return path;
                    }

                    // If already processed, skip
                    if (closedSet.find(current) != closedSet.end())
                    {
                        continue;
                    }

                    closedSet.insert(current);

                    // Process all neighbors
                    for (int neighbor : graph.getNeighbors(current))
                    {
                        // If already processed, skip
                        if (closedSet.find(neighbor) != closedSet.end())
                        {
                            continue;
                        }

                        // Calculate tentative g score
                        double tentativeGScore = gScore[vertexToIdx[current]] + graph.getEdgeWeight(current, neighbor);

                        // If this path is better than the previous one
                        if (tentativeGScore < gScore[vertexToIdx[neighbor]])
                        {
                            previous[vertexToIdx[neighbor]] = current;
                            gScore[vertexToIdx[neighbor]] = tentativeGScore;
                            fScore[vertexToIdx[neighbor]] = gScore[vertexToIdx[neighbor]] + heuristic(neighbor, goal);

                            // Add to open set if not already present
                            openSet.push({fScore[vertexToIdx[neighbor]], neighbor});
                        }
                    }
                }

                return {}; // No path found
            }

            /**
             * @brief Implementation of bidirectional search for finding the shortest path
             *
             * This technique runs two simultaneous searches: one forward from the source
             * and one backward from the destination.
             *
             * Time Complexity: O(b^(d/2)) where b is the branching factor and d is the distance
             * Space Complexity: O(b^(d/2))
             *
             * @param graph The graph
             * @param source Source vertex
             * @param destination Destination vertex
             * @return Vector representing the path from source to destination
             */
            std::vector<int> bidirectionalSearch(const IGraph &graph, int source, int destination)
            {
                // Forward search data structures
                std::queue<int> forwardQueue;
                std::unordered_map<int, int> forwardParent;
                std::unordered_set<int> forwardVisited;

                // Backward search data structures
                std::queue<int> backwardQueue;
                std::unordered_map<int, int> backwardParent;
                std::unordered_set<int> backwardVisited;

                // Initialize
                forwardQueue.push(source);
                forwardVisited.insert(source);
                forwardParent[source] = -1;

                backwardQueue.push(destination);
                backwardVisited.insert(destination);
                backwardParent[destination] = -1;

                int meetingPoint = -1;

                while (!forwardQueue.empty() && !backwardQueue.empty())
                {
                    // Expand forward search
                    int currentForward = forwardQueue.front();
                    forwardQueue.pop();

                    for (int neighbor : graph.getNeighbors(currentForward))
                    {
                        if (forwardVisited.find(neighbor) == forwardVisited.end())
                        {
                            forwardQueue.push(neighbor);
                            forwardVisited.insert(neighbor);
                            forwardParent[neighbor] = currentForward;

                            if (backwardVisited.find(neighbor) != backwardVisited.end())
                            {
                                meetingPoint = neighbor;
                                break;
                            }
                        }
                    }

                    if (meetingPoint != -1)
                    {
                        break;
                    }

                    // Expand backward search
                    int currentBackward = backwardQueue.front();
                    backwardQueue.pop();

                    for (int neighbor : graph.getNeighbors(currentBackward))
                    {
                        if (backwardVisited.find(neighbor) == backwardVisited.end())
                        {
                            backwardQueue.push(neighbor);
                            backwardVisited.insert(neighbor);
                            backwardParent[neighbor] = currentBackward;

                            if (forwardVisited.find(neighbor) != forwardVisited.end())
                            {
                                meetingPoint = neighbor;
                                break;
                            }
                        }
                    }

                    if (meetingPoint != -1)
                    {
                        break;
                    }
                }

                if (meetingPoint == -1)
                {
                    return {}; // No path found
                }

                // Reconstruct path
                std::vector<int> path;

                // Build path from source to meeting point
                for (int at = meetingPoint; at != -1; at = forwardParent[at])
                {
                    path.push_back(at);
                }
                std::reverse(path.begin(), path.end());

                // Build path from meeting point to destination
                for (int at = backwardParent[meetingPoint]; at != -1; at = backwardParent[at])
                {
                    path.push_back(at);
                }

                return path;
            }

            /**
             * @brief Implementation of bidirectional Dijkstra's algorithm for finding the shortest path
             *
             * This version of bidirectional search uses Dijkstra's algorithm in both directions.
             *
             * Time Complexity: O(E log V)
             * Space Complexity: O(V)
             *
             * @param graph The graph
             * @param source Source vertex
             * @param destination Destination vertex
             * @param shortestDistance Output parameter for the shortest distance
             * @return Vector representing the path from source to destination
             */
            std::vector<int> bidirectionalDijkstra(const IGraph &graph, int source, int destination,
                                                   double &shortestDistance)
            {
                std::vector<int> vertices = graph.getVertices();
                int n = vertices.size();

                // Create a vertex-to-index mapping
                std::unordered_map<int, int> vertexToIdx;
                for (int i = 0; i < n; i++)
                {
                    vertexToIdx[vertices[i]] = i;
                }

                // Forward search data structures
                std::priority_queue<std::pair<double, int>,
                                    std::vector<std::pair<double, int>>,
                                    std::greater<std::pair<double, int>>>
                    forwardPQ;
                std::vector<double> forwardDist(n, std::numeric_limits<double>::infinity());
                std::vector<int> forwardPrev(n, -1);
                std::unordered_set<int> forwardProcessed;

                // Backward search data structures
                std::priority_queue<std::pair<double, int>,
                                    std::vector<std::pair<double, int>>,
                                    std::greater<std::pair<double, int>>>
                    backwardPQ;
                std::vector<double> backwardDist(n, std::numeric_limits<double>::infinity());
                std::vector<int> backwardPrev(n, -1);
                std::unordered_set<int> backwardProcessed;

                // Initialize forward search
                forwardDist[vertexToIdx[source]] = 0;
                forwardPQ.push({0, source});

                // Initialize backward search
                backwardDist[vertexToIdx[destination]] = 0;
                backwardPQ.push({0, destination});

                // Keep track of the shortest path found so far
                double mu = std::numeric_limits<double>::infinity();
                int meetingPoint = -1;

                // Run bidirectional Dijkstra
                while (!forwardPQ.empty() && !backwardPQ.empty())
                {
                    // Check if we can terminate early
                    if (forwardPQ.top().first + backwardPQ.top().first >= mu)
                    {
                        break;
                    }

                    // Forward search step
                    auto [forwardDist, u] = forwardPQ.top();
                    forwardPQ.pop();

                    if (forwardProcessed.find(u) != forwardProcessed.end())
                    {
                        continue;
                    }

                    forwardProcessed.insert(u);
                    int uIdx = vertexToIdx[u];

                    // Check if u has been processed by backward search
                    if (backwardProcessed.find(u) != backwardProcessed.end())
                    {
                        double pathLength = forwardDist + backwardDist[uIdx];
                        if (pathLength < mu)
                        {
                            mu = pathLength;
                            meetingPoint = u;
                        }
                    }

                    // Process neighbors
                    for (int v : graph.getNeighbors(u))
                    {
                        int vIdx = vertexToIdx[v];
                        double weight = graph.getEdgeWeight(u, v);

                        if (forwardDist[uIdx] + weight < forwardDist[vIdx])
                        {
                            forwardDist[vIdx] = forwardDist[uIdx] + weight;
                            forwardPrev[vIdx] = u;
                            forwardPQ.push({forwardDist[vIdx], v});

                            // Check if this gives a shorter path
                            if (backwardProcessed.find(v) != backwardProcessed.end() &&
                                forwardDist[vIdx] + backwardDist[vIdx] < mu)
                            {
                                mu = forwardDist[vIdx] + backwardDist[vIdx];
                                meetingPoint = v;
                            }
                        }
                    }

                    // Backward search step
                    auto [backwardDist, v] = backwardPQ.top();
                    backwardPQ.pop();

                    if (backwardProcessed.find(v) != backwardProcessed.end())
                    {
                        continue;
                    }

                    backwardProcessed.insert(v);
                    int vIdx = vertexToIdx[v];

                    // Check if v has been processed by forward search
                    if (forwardProcessed.find(v) != forwardProcessed.end())
                    {
                        double pathLength = forwardDist[vIdx] + backwardDist;
                        if (pathLength < mu)
                        {
                            mu = pathLength;
                            meetingPoint = v;
                        }
                    }

                    // Process neighbors (in reverse)
                    for (int u : graph.getNeighbors(v))
                    {
                        int uIdx = vertexToIdx[u];
                        double weight = graph.getEdgeWeight(u, v); // Note: gets edge weight in reverse

                        if (backwardDist[vIdx] + weight < backwardDist[uIdx])
                        {
                            backwardDist[uIdx] = backwardDist[vIdx] + weight;
                            backwardPrev[uIdx] = v;
                            backwardPQ.push({backwardDist[uIdx], u});

                            // Check if this gives a shorter path
                            if (forwardProcessed.find(u) != forwardProcessed.end() &&
                                forwardDist[uIdx] + backwardDist[uIdx] < mu)
                            {
                                mu = forwardDist[uIdx] + backwardDist[uIdx];
                                meetingPoint = u;
                            }
                        }
                    }
                }

                if (meetingPoint == -1)
                {
                    shortestDistance = std::numeric_limits<double>::infinity();
                    return {}; // No path found
                }

                // Reconstruct the path
                std::vector<int> path;

                // Forward path
                for (int at = meetingPoint; at != -1; at = forwardPrev[vertexToIdx[at]])
                {
                    path.push_back(at);
                }
                std::reverse(path.begin(), path.end());

                // Backward path (excluding meeting point)
                for (int at = backwardPrev[vertexToIdx[meetingPoint]]; at != -1; at = backwardPrev[vertexToIdx[at]])
                {
                    path.push_back(at);
                }

                shortestDistance = mu;
                return path;
            }

            /****************************************************************************************
             * ADVANCED APPLICATIONS
             ****************************************************************************************/

            /**
             * @brief A simplified navigation system implementation
             */
            class NavigationSystem
            {
            private:
                std::unique_ptr<IGraph> roadNetwork;
                std::unordered_map<int, GeoCoordinate> nodeLocations;

            public:
                NavigationSystem()
                {
                    roadNetwork = std::make_unique<AdjacencyListGraph>(true, true);
                }

                void addNode(int nodeId, double latitude, double longitude)
                {
                    roadNetwork->addVertex(nodeId);
                    nodeLocations[nodeId] = GeoCoordinate(latitude, longitude);
                }

                void addRoad(int fromNode, int toNode, double distance, bool bidirectional = true)
                {
                    roadNetwork->addEdge(fromNode, toNode, distance);
                    if (bidirectional)
                    {
                        roadNetwork->addEdge(toNode, fromNode, distance);
                    }
                }

                int findClosestNode(const GeoCoordinate &location) const
                {
                    int closestNode = -1;
                    double minDistance = std::numeric_limits<double>::infinity();

                    for (const auto &[nodeId, nodeLocation] : nodeLocations)
                    {
                        double distance = haversineDistance(location, nodeLocation);
                        if (distance < minDistance)
                        {
                            minDistance = distance;
                            closestNode = nodeId;
                        }
                    }

                    return closestNode;
                }

                std::vector<int> findRoute(const GeoCoordinate &start, const GeoCoordinate &end)
                {
                    // Find closest nodes to coordinates
                    int startNode = findClosestNode(start);
                    int endNode = findClosestNode(end);

                    if (startNode == -1 || endNode == -1)
                    {
                        return {};
                    }

                    // Calculate route using A* with haversine distance as heuristic
                    return aStarSearch(*roadNetwork, startNode, endNode,
                                       [this](int node, int goal)
                                       {
                                           return haversineDistance(nodeLocations[node],
                                                                    nodeLocations[goal]);
                                       });
                }

                double estimateTravelTime(const std::vector<int> &route, double speedKmh = 50.0)
                {
                    double distance = 0.0;

                    for (size_t i = 0; i < route.size() - 1; i++)
                    {
                        distance += roadNetwork->getEdgeWeight(route[i], route[i + 1]);
                    }

                    // Convert distance to time (in hours)
                    return distance / speedKmh;
                }

                void printRoute(const std::vector<int> &route)
                {
                    if (route.empty())
                    {
                        std::cout << "No route found." << std::endl;
                        return;
                    }

                    std::cout << "Route from " << route.front() << " to " << route.back() << ":" << std::endl;

                    double totalDistance = 0.0;

                    for (size_t i = 0; i < route.size() - 1; i++)
                    {
                        int current = route[i];
                        int next = route[i + 1];
                        double segmentDistance = roadNetwork->getEdgeWeight(current, next);

                        std::cout << "  " << current << " -> " << next
                                  << " (" << std::fixed << std::setprecision(2) << segmentDistance << " km)" << std::endl;

                        totalDistance += segmentDistance;
                    }

                    double estimatedTime = estimateTravelTime(route);

                    std::cout << "Total distance: " << std::fixed << std::setprecision(2)
                              << totalDistance << " km" << std::endl;
                    std::cout << "Estimated travel time: " << std::fixed << std::setprecision(2)
                              << estimatedTime * 60 << " minutes" << std::endl;
                }
            };

            /**
             * @brief A network router implementation
             */
            class NetworkRouter
            {
            private:
                std::unique_ptr<IGraph> network;
                std::vector<std::vector<double>> distanceTable;
                std::vector<std::vector<int>> nextHopTable;
                std::unordered_map<int, int> vertexToIdx;
                std::unordered_map<int, int> idxToVertex;

            public:
                NetworkRouter()
                {
                    network = std::make_unique<AdjacencyListGraph>(true, true);
                }

                void addRouter(int routerId)
                {
                    network->addVertex(routerId);
                }

                void addLink(int router1, int router2, double bandwidth, double delay)
                {
                    // Use delay as the weight for shortest path calculations
                    network->addEdge(router1, router2, delay);
                    network->addEdge(router2, router1, delay); // Bidirectional link
                }

                void updateRoutingTables()
                {
                    // Create vertex-to-index mappings
                    std::vector<int> vertices = network->getVertices();
                    vertexToIdx.clear();
                    idxToVertex.clear();

                    for (size_t i = 0; i < vertices.size(); i++)
                    {
                        vertexToIdx[vertices[i]] = i;
                        idxToVertex[i] = vertices[i];
                    }

                    // Use Floyd-Warshall to compute all-pairs shortest paths
                    floydWarshall(*network, distanceTable, nextHopTable);
                }

                int getNextHop(int source, int destination)
                {
                    if (vertexToIdx.find(source) == vertexToIdx.end() ||
                        vertexToIdx.find(destination) == vertexToIdx.end())
                    {
                        return -1; // Invalid router
                    }

                    int sourceIdx = vertexToIdx[source];
                    int destIdx = vertexToIdx[destination];

                    if (nextHopTable[sourceIdx][destIdx] == -1)
                    {
                        return -1; // No path exists
                    }

                    return idxToVertex[nextHopTable[sourceIdx][destIdx]];
                }

                std::vector<int> getPath(int source, int destination)
                {
                    if (vertexToIdx.find(source) == vertexToIdx.end() ||
                        vertexToIdx.find(destination) == vertexToIdx.end())
                    {
                        return {}; // Invalid router
                    }

                    std::vector<int> path;
                    int sourceIdx = vertexToIdx[source];
                    int destIdx = vertexToIdx[destination];

                    if (nextHopTable[sourceIdx][destIdx] == -1)
                    {
                        return path; // No path exists
                    }

                    path.push_back(source);
                    int at = sourceIdx;

                    while (at != destIdx)
                    {
                        at = nextHopTable[at][destIdx];
                        path.push_back(idxToVertex[at]);
                    }

                    return path;
                }

                void handleLinkFailure(int router1, int router2)
                {
                    // Remove the link
                    network->removeEdge(router1, router2);
                    network->removeEdge(router2, router1);

                    // Update routing tables
                    updateRoutingTables();
                }

                void printRoutingTable(int router)
                {
                    if (vertexToIdx.find(router) == vertexToIdx.end())
                    {
                        std::cout << "Invalid router ID." << std::endl;
                        return;
                    }

                    int routerIdx = vertexToIdx[router];

                    std::cout << "Routing table for Router " << router << ":" << std::endl;
                    std::cout << "Destination\tNext Hop\tDelay" << std::endl;
                    std::cout << "---------------------------------" << std::endl;

                    for (size_t i = 0; i < distanceTable[routerIdx].size(); i++)
                    {
                        int dest = idxToVertex[i];

                        if (dest == router)
                        {
                            continue; // Skip self
                        }

                        int nextHop = (nextHopTable[routerIdx][i] != -1) ? idxToVertex[nextHopTable[routerIdx][i]] : -1;

                        double delay = distanceTable[routerIdx][i];

                        std::cout << dest << "\t\t";

                        if (nextHop == -1)
                        {
                            std::cout << "N/A\t\tINF" << std::endl;
                        }
                        else
                        {
                            std::cout << nextHop << "\t\t" << std::fixed << std::setprecision(2)
                                      << delay << "ms" << std::endl;
                        }
                    }
                }
            };

            /**
             * @brief A circuit router implementation
             */
            class CircuitRouter
            {
            private:
                std::unique_ptr<IGraph> gridGraph;
                int rows;
                int cols;

                // Convert 2D coordinates to node ID
                int coordToNode(const std::pair<int, int> &coord) const
                {
                    return coord.first * cols + coord.second;
                }

                // Convert node ID to 2D coordinates
                std::pair<int, int> nodeToCoord(int node) const
                {
                    return {node / cols, node % cols};
                }

            public:
                CircuitRouter(int numRows, int numCols) : rows(numRows), cols(numCols)
                {
                    gridGraph = std::make_unique<AdjacencyListGraph>(false, true);

                    // Create grid graph
                    for (int r = 0; r < rows; r++)
                    {
                        for (int c = 0; c < cols; c++)
                        {
                            int node = coordToNode({r, c});
                            gridGraph->addVertex(node);
                        }
                    }

                    // Add edges for adjacent cells
                    const int dr[] = {-1, 0, 1, 0};
                    const int dc[] = {0, 1, 0, -1};

                    for (int r = 0; r < rows; r++)
                    {
                        for (int c = 0; c < cols; c++)
                        {
                            int node = coordToNode({r, c});

                            for (int i = 0; i < 4; i++)
                            {
                                int nr = r + dr[i];
                                int nc = c + dc[i];

                                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols)
                                {
                                    int neighbor = coordToNode({nr, nc});
                                    gridGraph->addEdge(node, neighbor, 1.0);
                                }
                            }
                        }
                    }
                }

                void blockCell(const std::pair<int, int> &coord)
                {
                    int node = coordToNode(coord);

                    // Remove all edges to/from this node
                    std::vector<int> neighbors = gridGraph->getNeighbors(node);
                    for (int neighbor : neighbors)
                    {
                        gridGraph->removeEdge(node, neighbor);
                    }
                }

                std::vector<std::pair<int, int>> routeConnection(
                    const std::pair<int, int> &start, const std::pair<int, int> &end)
                {

                    // Convert grid coordinates to node IDs
                    int startNode = coordToNode(start);
                    int endNode = coordToNode(end);

                    // Use A* search for routing
                    auto path = aStarSearch(*gridGraph, startNode, endNode,
                                            [this, &end](int node, int goal)
                                            {
                                                auto nodeCoord = nodeToCoord(node);
                                                auto goalCoord = nodeToCoord(goal);
                                                return manhattanDistance(nodeCoord, goalCoord);
                                            });

                    // Convert node IDs back to coordinates
                    std::vector<std::pair<int, int>> route;
                    for (int node : path)
                    {
                        route.push_back(nodeToCoord(node));
                    }

                    return route;
                }

                void printGrid(const std::vector<std::pair<int, int>> &route = {})
                {
                    // Create a set of route cells for O(1) lookup
                    std::unordered_set<int> routeCells;
                    for (const auto &coord : route)
                    {
                        routeCells.insert(coordToNode(coord));
                    }

                    // Print the grid
                    for (int r = 0; r < rows; r++)
                    {
                        for (int c = 0; c < cols; c++)
                        {
                            int node = coordToNode({r, c});
                            char cell = '.'; // Empty cell

                            // Check if this is a blocked cell
                            if (gridGraph->getNeighbors(node).empty())
                            {
                                cell = '#'; // Blocked cell
                            }

                            // Check if this is part of the route
                            if (routeCells.find(node) != routeCells.end())
                            {
                                cell = '*'; // Route cell
                            }

                            // Check if this is the start or end of the route
                            if (!route.empty())
                            {
                                if (node == coordToNode(route.front()))
                                {
                                    cell = 'S'; // Start
                                }
                                else if (node == coordToNode(route.back()))
                                {
                                    cell = 'E'; // End
                                }
                            }

                            std::cout << cell << ' ';
                        }
                        std::cout << std::endl;
                    }
                }
            };

            /**
             * @brief A flight planner implementation
             */
            class FlightPlanner
            {
            private:
                enum class CostMetric
                {
                    DISTANCE,
                    TIME,
                    FUEL,
                    PRICE
                };

                struct Airport
                {
                    std::string code;
                    std::string name;
                    GeoCoordinate location;

                    Airport(const std::string &c = "", const std::string &n = "",
                            double lat = 0.0, double lon = 0.0)
                        : code(c), name(n), location(lat, lon) {}
                };

                struct FlightInfo
                {
                    int airline;      // Airline ID
                    double distance;  // Distance in km
                    double duration;  // Duration in hours
                    double fuelUsage; // Fuel usage in gallons
                    double price;     // Price in USD

                    FlightInfo(int a = 0, double d = 0.0, double t = 0.0, double f = 0.0, double p = 0.0)
                        : airline(a), distance(d), duration(t), fuelUsage(f), price(p) {}
                };

                std::unique_ptr<IGraph> airportNetwork;
                std::unordered_map<int, Airport> airports;
                std::unordered_map<std::pair<int, int>, FlightInfo, PairHash> flightInfo;

                // Hash function for std::pair
                struct PairHash
                {
                    template <class T1, class T2>
                    std::size_t operator()(const std::pair<T1, T2> &pair) const
                    {
                        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
                    }
                };

                void adjustEdgeWeights(CostMetric metric)
                {
                    // Adjust edge weights based on the selected metric
                    for (int u : airportNetwork->getVertices())
                    {
                        for (int v : airportNetwork->getNeighbors(u))
                        {
                            auto flight = flightInfo[{u, v}];
                            double weight = 0.0;

                            switch (metric)
                            {
                            case CostMetric::DISTANCE:
                                weight = flight.distance;
                                break;
                            case CostMetric::TIME:
                                weight = flight.duration;
                                break;
                            case CostMetric::FUEL:
                                weight = flight.fuelUsage;
                                break;
                            case CostMetric::PRICE:
                                weight = flight.price;
                                break;
                            }

                            airportNetwork->setEdgeWeight(u, v, weight);
                        }
                    }
                }

            public:
                FlightPlanner()
                {
                    airportNetwork = std::make_unique<AdjacencyListGraph>(true, true);
                }

                void addAirport(int airportId, const std::string &code, const std::string &name,
                                double latitude, double longitude)
                {
                    airportNetwork->addVertex(airportId);
                    airports[airportId] = Airport(code, name, latitude, longitude);
                }

                void addFlight(int departureAirport, int arrivalAirport, int airline,
                               double distance, double duration, double fuelUsage, double price)
                {
                    airportNetwork->addEdge(departureAirport, arrivalAirport, distance);
                    flightInfo[{departureAirport, arrivalAirport}] =
                        FlightInfo(airline, distance, duration, fuelUsage, price);
                }

                std::vector<int> planRoute(int departureAirport, int arrivalAirport,
                                           const std::string &metricStr = "distance")
                {
                    // Convert string to enum
                    CostMetric metric;
                    if (metricStr == "time")
                    {
                        metric = CostMetric::TIME;
                    }
                    else if (metricStr == "fuel")
                    {
                        metric = CostMetric::FUEL;
                    }
                    else if (metricStr == "price")
                    {
                        metric = CostMetric::PRICE;
                    }
                    else
                    {
                        metric = CostMetric::DISTANCE;
                    }

                    // Adjust edge weights based on the selected metric
                    adjustEdgeWeights(metric);

                    // Run Dijkstra's algorithm
                    std::vector<double> distances;
                    std::vector<int> previous = dijkstra(*airportNetwork, departureAirport, distances);

                    // Create a vertex-to-index mapping
                    std::vector<int> vertices = airportNetwork->getVertices();
                    std::unordered_map<int, int> vertexToIdx;
                    for (size_t i = 0; i < vertices.size(); i++)
                    {
                        vertexToIdx[vertices[i]] = i;
                    }

                    // Reconstruct the path
                    return reconstructPath(previous, departureAirport, arrivalAirport, vertexToIdx);
                }

                double getRouteCost(const std::vector<int> &route, const std::string &metricStr = "distance")
                {
                    double cost = 0.0;

                    for (size_t i = 0; i < route.size() - 1; i++)
                    {
                        int u = route[i];
                        int v = route[i + 1];
                        auto flight = flightInfo[{u, v}];

                        if (metricStr == "time")
                        {
                            cost += flight.duration;
                        }
                        else if (metricStr == "fuel")
                        {
                            cost += flight.fuelUsage;
                        }
                        else if (metricStr == "price")
                        {
                            cost += flight.price;
                        }
                        else
                        {
                            cost += flight.distance;
                        }
                    }

                    return cost;
                }

                void printRoute(const std::vector<int> &route, const std::string &metricStr = "distance")
                {
                    if (route.empty())
                    {
                        std::cout << "No route found." << std::endl;
                        return;
                    }

                    std::cout << "Flight route from " << airports[route.front()].code
                              << " to " << airports[route.back()].code << ":" << std::endl;

                    for (size_t i = 0; i < route.size() - 1; i++)
                    {
                        int from = route[i];
                        int to = route[i + 1];
                        auto flight = flightInfo[{from, to}];

                        std::cout << "  " << airports[from].code << " (" << airports[from].name << ")"
                                  << " -> " << airports[to].code << " (" << airports[to].name << ")" << std::endl;

                        std::cout << "    Distance: " << std::fixed << std::setprecision(0)
                                  << flight.distance << " km, Duration: " << std::fixed << std::setprecision(2)
                                  << flight.duration << " hours, Price: $" << std::fixed << std::setprecision(2)
                                  << flight.price << std::endl;
                    }

                    double totalCost = getRouteCost(route, metricStr);
                    std::string unit;

                    if (metricStr == "time")
                    {
                        unit = " hours";
                    }
                    else if (metricStr == "fuel")
                    {
                        unit = " gallons";
                    }
                    else if (metricStr == "price")
                    {
                        unit = " USD";
                    }
                    else
                    {
                        unit = " km";
                    }

                    std::cout << "Total " << metricStr << ": " << std::fixed << std::setprecision(2)
                              << totalCost << unit << std::endl;
                }

                std::vector<int> planMultiCityRoute(const std::vector<int> &cities)
                {
                    if (cities.size() <= 1)
                    {
                        return cities;
                    }

                    // For each pair of cities, compute the shortest path
                    std::vector<std::vector<int>> pathSegments;

                    for (size_t i = 0; i < cities.size() - 1; i++)
                    {
                        int from = cities[i];
                        int to = cities[i + 1];

                        std::vector<double> distances;
                        std::vector<int> previous = dijkstra(*airportNetwork, from, distances);

                        // Create a vertex-to-index mapping
                        std::vector<int> vertices = airportNetwork->getVertices();
                        std::unordered_map<int, int> vertexToIdx;
                        for (size_t j = 0; j < vertices.size(); j++)
                        {
                            vertexToIdx[vertices[j]] = j;
                        }

                        // Reconstruct the path
                        auto path = reconstructPath(previous, from, to, vertexToIdx);

                        if (path.empty())
                        {
                            std::cout << "No path found between " << airports[from].code
                                      << " and " << airports[to].code << "." << std::endl;
                            return {};
                        }

                        pathSegments.push_back(path);
                    }

                    // Merge the path segments
                    std::vector<int> fullRoute;

                    for (size_t i = 0; i < pathSegments.size(); i++)
                    {
                        // Add all vertices except the last one (it will be added as the first of the next segment)
                        for (size_t j = 0; j < pathSegments[i].size() - (i < pathSegments.size() - 1 ? 1 : 0); j++)
                        {
                            fullRoute.push_back(pathSegments[i][j]);
                        }
                    }

                    return fullRoute;
                }
            };

            /****************************************************************************************
             * PERFORMANCE BENCHMARKS
             ****************************************************************************************/

            /**
             * @brief Benchmark different shortest path algorithms
             *
             * @param graphSizes Vector of graph sizes to test
             * @param edgeDensities Vector of edge densities to test
             * @param iterations Number of iterations for each test
             */
            void benchmarkShortestPathAlgorithms(
                const std::vector<int> &graphSizes,
                const std::vector<double> &edgeDensities,
                int iterations = 5)
            {

                std::cout << "===== Shortest Path Algorithm Benchmarks =====" << std::endl;
                std::cout << "Iterations: " << iterations << std::endl
                          << std::endl;

                // Print header
                std::cout << std::left << std::setw(10) << "Vertices"
                          << std::setw(10) << "Edges"
                          << std::setw(15) << "Density"
                          << std::setw(15) << "Dijkstra (ms)"
                          << std::setw(20) << "Bellman-Ford (ms)"
                          << std::setw(20) << "Floyd-Warshall (ms)"
                          << std::endl;

                std::cout << std::string(90, '-') << std::endl;

                // Test different graph sizes and densities
                for (int size : graphSizes)
                {
                    for (double density : edgeDensities)
                    {
                        double dijkstraTime = 0.0;
                        double bellmanFordTime = 0.0;
                        double floydWarshallTime = 0.0;
                        int totalEdges = 0;

                        for (int iter = 0; iter < iterations; ++iter)
                        {
                            // Generate a random weighted graph
                            auto graph = std::make_unique<AdjacencyListGraph>(true, true);

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
                                for (int j = 0; j < size; j++)
                                {
                                    if (i != j && distr(gen) < density)
                                    {
                                        graph->addEdge(i, j, weightDistr(gen));
                                    }
                                }
                            }

                            totalEdges = graph->getEdgeCount();

                            // Pick a random source vertex
                            int source = 0;

                            // Benchmark Dijkstra's algorithm
                            auto startDijkstra = std::chrono::high_resolution_clock::now();
                            std::vector<double> distances;
                            auto previous = dijkstra(*graph, source, distances);
                            auto endDijkstra = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> dijkstraDuration = endDijkstra - startDijkstra;
                            dijkstraTime += dijkstraDuration.count();

                            // Benchmark Bellman-Ford algorithm
                            auto startBellmanFord = std::chrono::high_resolution_clock::now();
                            std::vector<double> bfDistances;
                            std::vector<int> bfPrevious;
                            bellmanFord(*graph, source, bfDistances, bfPrevious);
                            auto endBellmanFord = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> bellmanFordDuration = endBellmanFord - startBellmanFord;
                            bellmanFordTime += bellmanFordDuration.count();

                            // Benchmark Floyd-Warshall algorithm
                            auto startFloydWarshall = std::chrono::high_resolution_clock::now();
                            std::vector<std::vector<double>> fwDistances;
                            std::vector<std::vector<int>> fwNext;
                            floydWarshall(*graph, fwDistances, fwNext);
                            auto endFloydWarshall = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> floydWarshallDuration = endFloydWarshall - startFloydWarshall;
                            floydWarshallTime += floydWarshallDuration.count();
                        }

                        // Calculate averages
                        dijkstraTime /= iterations;
                        bellmanFordTime /= iterations;
                        floydWarshallTime /= iterations;

                        // Print results
                        std::cout << std::left << std::setw(10) << size
                                  << std::setw(10) << totalEdges
                                  << std::setw(15) << std::fixed << std::setprecision(2) << density
                                  << std::setw(15) << std::fixed << std::setprecision(2) << dijkstraTime
                                  << std::setw(20) << std::fixed << std::setprecision(2) << bellmanFordTime
                                  << std::setw(20) << std::fixed << std::setprecision(2) << floydWarshallTime
                                  << std::endl;
                    }
                }

                std::cout << std::endl;
            }

            /**
             * @brief Benchmark path finding with different single-source algorithms
             *
             * @param graphSizes Vector of graph sizes to test
             * @param iterations Number of iterations for each test
             */
            void benchmarkPathFindingAlgorithms(
                const std::vector<int> &graphSizes,
                int iterations = 5)
            {

                std::cout << "===== Path Finding Algorithm Benchmarks =====" << std::endl;
                std::cout << "Iterations: " << iterations << std::endl
                          << std::endl;

                // Print header
                std::cout << std::left << std::setw(10) << "Vertices"
                          << std::setw(10) << "Edges"
                          << std::setw(20) << "Dijkstra (ms)"
                          << std::setw(20) << "A* Search (ms)"
                          << std::setw(20) << "Bidirectional (ms)"
                          << std::endl;

                std::cout << std::string(80, '-') << std::endl;

                // Edge density for tests
                const double density = 0.1;

                // Test different graph sizes
                for (int size : graphSizes)
                {
                    double dijkstraTime = 0.0;
                    double aStarTime = 0.0;
                    double bidirTime = 0.0;
                    int totalEdges = 0;

                    for (int iter = 0; iter < iterations; ++iter)
                    {
                        // Generate a random graph for a 2D grid
                        auto graph = std::make_unique<AdjacencyListGraph>(false, true);

                        // Assign (x,y) coordinates to each vertex
                        std::unordered_map<int, std::pair<int, int>> coordinates;

                        int gridSize = static_cast<int>(std::sqrt(size));
                        int vertexCount = 0;

                        // Add vertices
                        for (int x = 0; x < gridSize && vertexCount < size; x++)
                        {
                            for (int y = 0; y < gridSize && vertexCount < size; y++)
                            {
                                int vertex = vertexCount++;
                                graph->addVertex(vertex);
                                coordinates[vertex] = {x, y};
                            }
                        }

                        // Add edges (use a grid structure with some random edges)
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_real_distribution<> distr(0.0, 1.0);

                        for (int i = 0; i < vertexCount; i++)
                        {
                            auto [x1, y1] = coordinates[i];

                            for (int j = i + 1; j < vertexCount; j++)
                            {
                                auto [x2, y2] = coordinates[j];

                                // Add edge if vertices are adjacent in the grid
                                if ((std::abs(x1 - x2) == 1 && y1 == y2) ||
                                    (std::abs(y1 - y2) == 1 && x1 == x2))
                                {
                                    double weight = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
                                    graph->addEdge(i, j, weight);
                                }
                                // Add some random edges
                                else if (distr(gen) < 0.01)
                                {
                                    double weight = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
                                    graph->addEdge(i, j, weight);
                                }
                            }
                        }

                        totalEdges = graph->getEdgeCount();

                        // Pick source and destination vertices (opposite corners)
                        int source = 0;
                        int destination = vertexCount - 1;

                        // Benchmark Dijkstra's algorithm
                        auto startDijkstra = std::chrono::high_resolution_clock::now();
                        double dijkstraDist;
                        auto dijkstraPath = dijkstraSingleTarget(*graph, source, destination, dijkstraDist);
                        auto endDijkstra = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> dijkstraDuration = endDijkstra - startDijkstra;
                        dijkstraTime += dijkstraDuration.count();

                        // Benchmark A* search
                        auto startAStar = std::chrono::high_resolution_clock::now();
                        auto aStarPath = aStarSearch(*graph, source, destination,
                                                     [&coordinates](int a, int b)
                                                     {
                                                         auto [x1, y1] = coordinates[a];
                                                         auto [x2, y2] = coordinates[b];
                                                         return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
                                                     });
                        auto endAStar = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> aStarDuration = endAStar - startAStar;
                        aStarTime += aStarDuration.count();

                        // Benchmark Bidirectional search
                        auto startBidir = std::chrono::high_resolution_clock::now();
                        double bidirDist;
                        auto bidirPath = bidirectionalDijkstra(*graph, source, destination, bidirDist);
                        auto endBidir = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> bidirDuration = endBidir - startBidir;
                        bidirTime += bidirDuration.count();
                    }

                    // Calculate averages
                    dijkstraTime /= iterations;
                    aStarTime /= iterations;
                    bidirTime /= iterations;

                    // Print results
                    std::cout << std::left << std::setw(10) << size
                              << std::setw(10) << totalEdges
                              << std::setw(20) << std::fixed << std::setprecision(2) << dijkstraTime
                              << std::setw(20) << std::fixed << std::setprecision(2) << aStarTime
                              << std::setw(20) << std::fixed << std::setprecision(2) << bidirTime
                              << std::endl;
                }

                std::cout << std::endl;
            }

            /**
             * @brief Benchmark graph with negative edges
             *
             * @param graphSizes Vector of graph sizes to test
             * @param iterations Number of iterations for each test
             */
            void benchmarkNegativeEdgeGraphs(
                const std::vector<int> &graphSizes,
                int iterations = 5)
            {

                std::cout << "===== Negative Edge Graph Benchmarks =====" << std::endl;
                std::cout << "Iterations: " << iterations << std::endl
                          << std::endl;

                // Print header
                std::cout << std::left << std::setw(10) << "Vertices"
                          << std::setw(15) << "Edges"
                          << std::setw(15) << "Neg. Edges"
                          << std::setw(20) << "Bellman-Ford (ms)"
                          << std::setw(20) << "Floyd-Warshall (ms)"
                          << std::endl;

                std::cout << std::string(80, '-') << std::endl;

                // Edge density for tests
                const double density = 0.1;

                // Test different graph sizes
                for (int size : graphSizes)
                {
                    double bellmanFordTime = 0.0;
                    double floydWarshallTime = 0.0;
                    int totalEdges = 0;
                    int negativeEdges = 0;

                    for (int iter = 0; iter < iterations; ++iter)
                    {
                        // Generate a random weighted graph with some negative edges
                        auto graph = std::make_unique<AdjacencyListGraph>(true, true);

                        // Add vertices
                        for (int i = 0; i < size; i++)
                        {
                            graph->addVertex(i);
                        }

                        // Add random edges with some negative weights
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_real_distribution<> distr(0.0, 1.0);
                        std::uniform_real_distribution<> weightDistr(-50.0, 100.0);

                        negativeEdges = 0;

                        for (int i = 0; i < size; i++)
                        {
                            for (int j = 0; j < size; j++)
                            {
                                if (i != j && distr(gen) < density)
                                {
                                    double weight = weightDistr(gen);
                                    graph->addEdge(i, j, weight);

                                    if (weight < 0)
                                    {
                                        negativeEdges++;
                                    }
                                }
                            }
                        }

                        totalEdges = graph->getEdgeCount();

                        // Ensure no negative cycles
                        std::vector<double> testDist;
                        std::vector<int> testPrev;
                        if (!bellmanFord(*graph, 0, testDist, testPrev))
                        {
                            // Graph has a negative cycle, try again
                            iter--;
                            continue;
                        }

                        // Pick a random source vertex
                        int source = 0;

                        // Benchmark Bellman-Ford algorithm
                        auto startBellmanFord = std::chrono::high_resolution_clock::now();
                        std::vector<double> bfDistances;
                        std::vector<int> bfPrevious;
                        bellmanFord(*graph, source, bfDistances, bfPrevious);
                        auto endBellmanFord = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> bellmanFordDuration = endBellmanFord - startBellmanFord;
                        bellmanFordTime += bellmanFordDuration.count();

                        // Benchmark Floyd-Warshall algorithm
                        auto startFloydWarshall = std::chrono::high_resolution_clock::now();
                        std::vector<std::vector<double>> fwDistances;
                        std::vector<std::vector<int>> fwNext;
                        floydWarshall(*graph, fwDistances, fwNext);
                        auto endFloydWarshall = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> floydWarshallDuration = endFloydWarshall - startFloydWarshall;
                        floydWarshallTime += floydWarshallDuration.count();
                    }

                    // Calculate averages
                    bellmanFordTime /= iterations;
                    floydWarshallTime /= iterations;

                    // Print results
                    std::cout << std::left << std::setw(10) << size
                              << std::setw(15) << totalEdges
                              << std::setw(15) << negativeEdges
                              << std::setw(20) << std::fixed << std::setprecision(2) << bellmanFordTime
                              << std::setw(20) << std::fixed << std::setprecision(2) << floydWarshallTime
                              << std::endl;
                }

                std::cout << std::endl;
            }

            /****************************************************************************************
             * DEMONSTRATION FUNCTIONS
             ****************************************************************************************/

            /**
             * @brief Demonstrate Dijkstra's algorithm on a sample graph
             */
            void demoDijkstra()
            {
                std::cout << "===== Dijkstra's Algorithm Demo =====" << std::endl;

                // Create a sample graph
                auto graph = std::make_unique<AdjacencyListGraph>(false, true);

                // Add vertices (A through F)
                for (int i = 0; i < 6; i++)
                {
                    graph->addVertex(i);
                }

                // Add edges to form the graph from the article
                //      2       3
                //  A ----- B ----- C
                //  |       |       |
                // 4|       |1      |5
                //  |       |       |
                //  D ----- E ----- F
                //      7       1

                graph->addEdge(0, 1, 2); // A-B
                graph->addEdge(0, 3, 4); // A-D
                graph->addEdge(1, 2, 3); // B-C
                graph->addEdge(1, 4, 1); // B-E
                graph->addEdge(2, 5, 5); // C-F
                graph->addEdge(3, 4, 7); // D-E
                graph->addEdge(4, 5, 1); // E-F

                // Print the graph
                std::cout << "Sample graph:" << std::endl;
                std::cout << graph->toString() << std::endl;

                // Run Dijkstra's algorithm from vertex A (0)
                int source = 0;
                std::vector<double> distances;
                std::vector<int> previous = dijkstra(*graph, source, distances);

                // Create a vertex-to-index mapping
                std::vector<int> vertices = graph->getVertices();
                std::unordered_map<int, int> vertexToIdx;
                for (size_t i = 0; i < vertices.size(); i++)
                {
                    vertexToIdx[vertices[i]] = i;
                }

                // Print shortest distances
                std::cout << "Shortest distances from vertex " << source << " (A):" << std::endl;
                for (int i = 0; i < distances.size(); i++)
                {
                    char vertex = 'A' + i;
                    std::cout << "  " << vertex << ": ";

                    if (distances[i] == std::numeric_limits<double>::infinity())
                    {
                        std::cout << "INF";
                    }
                    else
                    {
                        std::cout << distances[i];
                    }

                    // Print the previous vertex in the shortest path
                    if (previous[i] != -1)
                    {
                        char prevVertex = 'A' + previous[i];
                        std::cout << " (via " << prevVertex << ")";
                    }

                    std::cout << std::endl;
                }

                // Print shortest paths to all vertices
                std::cout << "\nShortest paths from vertex " << source << " (A):" << std::endl;
                for (int i = 0; i < vertices.size(); i++)
                {
                    if (i == source)
                        continue;

                    int destination = i;
                    char destVertex = 'A' + i;

                    std::vector<int> path = reconstructPath(previous, source, destination, vertexToIdx);

                    std::cout << "  Path to " << destVertex << ": ";

                    if (path.empty())
                    {
                        std::cout << "No path";
                    }
                    else
                    {
                        for (size_t j = 0; j < path.size(); j++)
                        {
                            char pathVertex = 'A' + path[j];
                            std::cout << pathVertex;

                            if (j < path.size() - 1)
                            {
                                std::cout << " -> ";
                            }
                        }

                        std::cout << " (distance: " << distances[vertexToIdx[destination]] << ")";
                    }

                    std::cout << std::endl;
                }
            }

            /**
             * @brief Demonstrate Bellman-Ford algorithm on a graph with negative edges
             */
            void demoBellmanFord()
            {
                std::cout << "===== Bellman-Ford Algorithm Demo =====" << std::endl;

                // Create a sample graph with negative edges
                auto graph = std::make_unique<AdjacencyListGraph>(true, true);

                // Add vertices (A through F)
                for (int i = 0; i < 6; i++)
                {
                    graph->addVertex(i);
                }

                // Add edges to form the graph with negative edges
                //      2       -5
                //  A ----- B ----- C
                //  |       |       |
                // 4|       |1      |2
                //  |       |       |
                //  D ----- E ----- F
                //      -3      1

                graph->addEdge(0, 1, 2);  // A-B
                graph->addEdge(0, 3, 4);  // A-D
                graph->addEdge(1, 2, -5); // B-C (negative)
                graph->addEdge(1, 4, 1);  // B-E
                graph->addEdge(2, 5, 2);  // C-F
                graph->addEdge(3, 4, -3); // D-E (negative)
                graph->addEdge(4, 5, 1);  // E-F

                // Print the graph
                std::cout << "Sample graph with negative edges:" << std::endl;
                std::cout << graph->toString() << std::endl;

                // Run Bellman-Ford algorithm from vertex A (0)
                int source = 0;
                std::vector<double> distances;
                std::vector<int> previous;

                bool noNegativeCycle = bellmanFord(*graph, source, distances, previous);

                // Create a vertex-to-index mapping
                std::vector<int> vertices = graph->getVertices();
                std::unordered_map<int, int> vertexToIdx;
                for (size_t i = 0; i < vertices.size(); i++)
                {
                    vertexToIdx[vertices[i]] = i;
                }

                if (!noNegativeCycle)
                {
                    std::cout << "Negative cycle detected!" << std::endl;

                    // Try to find the negative cycle
                    std::vector<int> cycle = findNegativeCycle(*graph, source);

                    if (!cycle.empty())
                    {
                        std::cout << "Negative cycle: ";
                        for (size_t i = 0; i < cycle.size(); i++)
                        {
                            char vertex = 'A' + cycle[i];
                            std::cout << vertex;

                            if (i < cycle.size() - 1)
                            {
                                std::cout << " -> ";
                            }
                        }
                        std::cout << std::endl;

                        // Calculate the cycle weight
                        double cycleWeight = 0.0;
                        for (size_t i = 0; i < cycle.size() - 1; i++)
                        {
                            cycleWeight += graph->getEdgeWeight(cycle[i], cycle[i + 1]);
                        }
                        std::cout << "Cycle weight: " << cycleWeight << std::endl;
                    }
                }
                else
                {
                    // Print shortest distances
                    std::cout << "Shortest distances from vertex " << source << " (A):" << std::endl;
                    for (int i = 0; i < distances.size(); i++)
                    {
                        char vertex = 'A' + i;
                        std::cout << "  " << vertex << ": ";

                        if (distances[i] == std::numeric_limits<double>::infinity())
                        {
                            std::cout << "INF";
                        }
                        else
                        {
                            std::cout << distances[i];
                        }

                        // Print the previous vertex in the shortest path
                        if (previous[i] != -1)
                        {
                            char prevVertex = 'A' + previous[i];
                            std::cout << " (via " << prevVertex << ")";
                        }

                        std::cout << std::endl;
                    }

                    // Print shortest paths to all vertices
                    std::cout << "\nShortest paths from vertex " << source << " (A):" << std::endl;
                    for (int i = 0; i < vertices.size(); i++)
                    {
                        if (i == source)
                            continue;

                        int destination = i;
                        char destVertex = 'A' + i;

                        std::vector<int> path = reconstructPath(previous, source, destination, vertexToIdx);

                        std::cout << "  Path to " << destVertex << ": ";

                        if (path.empty())
                        {
                            std::cout << "No path";
                        }
                        else
                        {
                            for (size_t j = 0; j < path.size(); j++)
                            {
                                char pathVertex = 'A' + path[j];
                                std::cout << pathVertex;

                                if (j < path.size() - 1)
                                {
                                    std::cout << " -> ";
                                }
                            }

                            std::cout << " (distance: " << distances[vertexToIdx[destination]] << ")";
                        }

                        std::cout << std::endl;
                    }
                }

                // Now try a graph with a negative cycle
                std::cout << "\nCreating a graph with a negative cycle..." << std::endl;

                auto cycleGraph = std::make_unique<AdjacencyListGraph>(true, true);

                // Add vertices
                for (int i = 0; i < 4; i++)
                {
                    cycleGraph->addVertex(i);
                }

                // Add edges to form a graph with a negative cycle
                cycleGraph->addEdge(0, 1, 1);  // A-B
                cycleGraph->addEdge(1, 2, 2);  // B-C
                cycleGraph->addEdge(2, 3, 3);  // C-D
                cycleGraph->addEdge(3, 1, -7); // D-B (negative cycle)

                // Print the graph
                std::cout << "Graph with a negative cycle:" << std::endl;
                std::cout << cycleGraph->toString() << std::endl;

                // Run Bellman-Ford algorithm
                source = 0;
                std::vector<double> cycleDistances;
                std::vector<int> cyclePrevious;

                noNegativeCycle = bellmanFord(*cycleGraph, source, cycleDistances, cyclePrevious);

                if (!noNegativeCycle)
                {
                    std::cout << "Negative cycle detected, as expected!" << std::endl;

                    // Find the negative cycle
                    std::vector<int> cycle = findNegativeCycle(*cycleGraph, source);

                    if (!cycle.empty())
                    {
                        std::cout << "Negative cycle: ";
                        for (size_t i = 0; i < cycle.size(); i++)
                        {
                            char vertex = 'A' + cycle[i];
                            std::cout << vertex;

                            if (i < cycle.size() - 1)
                            {
                                std::cout << " -> ";
                            }
                        }
                        std::cout << std::endl;

                        // Calculate the cycle weight
                        double cycleWeight = 0.0;
                        for (size_t i = 0; i < cycle.size() - 1; i++)
                        {
                            cycleWeight += cycleGraph->getEdgeWeight(cycle[i], cycle[i + 1]);
                        }
                        std::cout << "Cycle weight: " << cycleWeight << std::endl;
                    }
                }
            }

            /**
             * @brief Demonstrate Floyd-Warshall algorithm on a sample graph
             */
            void demoFloydWarshall()
            {
                std::cout << "===== Floyd-Warshall Algorithm Demo =====" << std::endl;

                // Create a sample graph
                auto graph = std::make_unique<AdjacencyListGraph>(true, true);

                // Add vertices (A through E)
                for (int i = 0; i < 5; i++)
                {
                    graph->addVertex(i);
                }

                // Add edges to form the graph from the article
                //      2       3
                //  A ----- B ----- C
                //  |               |
                // 6|               |1
                //  |               |
                //  D ------------- E
                //          4

                graph->addEdge(0, 1, 2); // A-B
                graph->addEdge(0, 3, 6); // A-D
                graph->addEdge(1, 2, 3); // B-C
                graph->addEdge(2, 4, 1); // C-E
                graph->addEdge(3, 4, 4); // D-E

                // Print the graph
                std::cout << "Sample graph:" << std::endl;
                std::cout << graph->toString() << std::endl;

                // Run Floyd-Warshall algorithm
                std::vector<std::vector<double>> distances;
                std::vector<std::vector<int>> next;
                floydWarshall(*graph, distances, next);

                // Create mappings
                std::vector<int> vertices = graph->getVertices();
                std::unordered_map<int, int> vertexToIdx;
                std::unordered_map<int, int> idxToVertex;
                for (size_t i = 0; i < vertices.size(); i++)
                {
                    vertexToIdx[vertices[i]] = i;
                    idxToVertex[i] = vertices[i];
                }

                // Print all-pairs shortest distances
                std::cout << "All-pairs shortest distances:" << std::endl;

                // Print header
                std::cout << "    ";
                for (int v : vertices)
                {
                    char vertex = 'A' + v;
                    std::cout << std::setw(5) << vertex;
                }
                std::cout << std::endl;

                // Print distances
                for (int u : vertices)
                {
                    char vertex = 'A' + u;
                    std::cout << std::setw(4) << vertex;

                    for (int v : vertices)
                    {
                        int uIdx = vertexToIdx[u];
                        int vIdx = vertexToIdx[v];

                        if (distances[uIdx][vIdx] == std::numeric_limits<double>::infinity())
                        {
                            std::cout << std::setw(5) << "INF";
                        }
                        else
                        {
                            std::cout << std::setw(5) << distances[uIdx][vIdx];
                        }
                    }

                    std::cout << std::endl;
                }

                // Print some specific paths
                std::cout << "\nSome specific paths:" << std::endl;

                std::pair<int, int> pathPairs[] = {
                    {0, 4}, // A to E
                    {1, 3}, // B to D
                    {2, 0}, // C to A
                    {3, 2}, // D to C
                };

                for (const auto &[source, destination] : pathPairs)
                {
                    char sourceVertex = 'A' + source;
                    char destVertex = 'A' + destination;

                    std::cout << "  Path from " << sourceVertex << " to " << destVertex << ": ";

                    std::vector<int> path = reconstructPathFloydWarshall(next, source, destination, vertexToIdx, idxToVertex);

                    if (path.empty())
                    {
                        std::cout << "No path";
                    }
                    else
                    {
                        for (size_t i = 0; i < path.size(); i++)
                        {
                            char pathVertex = 'A' + path[i];
                            std::cout << pathVertex;

                            if (i < path.size() - 1)
                            {
                                std::cout << " -> ";
                            }
                        }

                        std::cout << " (distance: " << distances[vertexToIdx[source]][vertexToIdx[destination]] << ")";
                    }

                    std::cout << std::endl;
                }
            }

            /**
             * @brief Demonstrate Johnson's algorithm on a graph with negative edges
             */
            void demoJohnsonAlgorithm()
            {
                std::cout << "===== Johnson's Algorithm Demo =====" << std::endl;

                // Create a sample graph with negative edges
                auto graph = std::make_unique<AdjacencyListGraph>(true, true);

                // Add vertices (A through F)
                for (int i = 0; i < 6; i++)
                {
                    graph->addVertex(i);
                }

                // Add edges to form the graph with negative edges
                //      2       -5
                //  A ----- B ----- C
                //  |       |       |
                // 4|       |1      |2
                //  |       |       |
                //  D ----- E ----- F
                //      -3      1

                graph->addEdge(0, 1, 2);  // A-B
                graph->addEdge(0, 3, 4);  // A-D
                graph->addEdge(1, 2, -5); // B-C (negative)
                graph->addEdge(1, 4, 1);  // B-E
                graph->addEdge(2, 5, 2);  // C-F
                graph->addEdge(3, 4, -3); // D-E (negative)
                graph->addEdge(4, 5, 1);  // E-F

                // Print the graph
                std::cout << "Sample graph with negative edges:" << std::endl;
                std::cout << graph->toString() << std::endl;

                // Run Johnson's algorithm
                std::vector<std::vector<double>> distances;
                bool success = johnsonsAlgorithm(*graph, distances);

                if (!success)
                {
                    std::cout << "Negative cycle detected!" << std::endl;
                }
                else
                {
                    // Print all-pairs shortest distances
                    std::cout << "All-pairs shortest distances:" << std::endl;

                    // Print header
                    std::cout << "    ";
                    for (int v = 0; v < 6; v++)
                    {
                        char vertex = 'A' + v;
                        std::cout << std::setw(5) << vertex;
                    }
                    std::cout << std::endl;

                    // Print distances
                    for (int u = 0; u < 6; u++)
                    {
                        char vertex = 'A' + u;
                        std::cout << std::setw(4) << vertex;

                        for (int v = 0; v < 6; v++)
                        {
                            if (distances[u][v] == std::numeric_limits<double>::infinity())
                            {
                                std::cout << std::setw(5) << "INF";
                            }
                            else
                            {
                                std::cout << std::setw(5) << distances[u][v];
                            }
                        }

                        std::cout << std::endl;
                    }
                }
            }

            /**
             * @brief Demonstrate A* search algorithm on a 2D grid
             */
            void demoAStarSearch()
            {
                std::cout << "===== A* Search Algorithm Demo =====" << std::endl;

                // Create a 2D grid graph
                int gridSize = 10;
                auto graph = std::make_unique<AdjacencyListGraph>(false, true);

                // Assign (x,y) coordinates to each vertex
                std::unordered_map<int, std::pair<int, int>> coordinates;

                // Add vertices
                for (int x = 0; x < gridSize; x++)
                {
                    for (int y = 0; y < gridSize; y++)
                    {
                        int vertex = x * gridSize + y;
                        graph->addVertex(vertex);
                        coordinates[vertex] = {x, y};
                    }
                }

                // Add edges (grid structure)
                for (int x = 0; x < gridSize; x++)
                {
                    for (int y = 0; y < gridSize; y++)
                    {
                        int vertex = x * gridSize + y;

                        // Add edges to adjacent cells
                        const int dx[] = {-1, 0, 1, 0};
                        const int dy[] = {0, 1, 0, -1};

                        for (int i = 0; i < 4; i++)
                        {
                            int nx = x + dx[i];
                            int ny = y + dy[i];

                            if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize)
                            {
                                int neighbor = nx * gridSize + ny;
                                double weight = 1.0; // All edges have the same weight
                                graph->addEdge(vertex, neighbor, weight);
                            }
                        }
                    }
                }

                // Add some obstacles (remove edges)
                std::vector<std::pair<int, int>> obstacles = {
                    {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}};

                for (const auto &[x, y] : obstacles)
                {
                    int vertex = x * gridSize + y;

                    // Remove all edges to/from this vertex
                    std::vector<int> neighbors = graph->getNeighbors(vertex);
                    for (int neighbor : neighbors)
                    {
                        graph->removeEdge(vertex, neighbor);
                    }
                }

                // Define start and goal vertices
                int start = 0;                      // Top-left corner
                int goal = gridSize * gridSize - 1; // Bottom-right corner

                // Define heuristic function (Manhattan distance)
                auto heuristic = [&coordinates](int a, int b)
                {
                    const auto &[x1, y1] = coordinates[a];
                    const auto &[x2, y2] = coordinates[b];
                    return std::abs(x1 - x2) + std::abs(y1 - y2);
                };

                // Run A* search
                std::cout << "Running A* search from (" << coordinates[start].first << ","
                          << coordinates[start].second << ") to ("
                          << coordinates[goal].first << "," << coordinates[goal].second << ")" << std::endl;

                auto path = aStarSearch(*graph, start, goal, heuristic);

                // Print the grid with the path
                std::cout << "Grid with obstacles and path:" << std::endl;

                // Create a set of path cells for O(1) lookup
                std::unordered_set<int> pathCells;
                for (int vertex : path)
                {
                    pathCells.insert(vertex);
                }

                for (int x = 0; x < gridSize; x++)
                {
                    for (int y = 0; y < gridSize; y++)
                    {
                        int vertex = x * gridSize + y;

                        if (vertex == start)
                        {
                            std::cout << "S ";
                        }
                        else if (vertex == goal)
                        {
                            std::cout << "G ";
                        }
                        else if (pathCells.find(vertex) != pathCells.end())
                        {
                            std::cout << "* ";
                        }
                        else if (graph->getNeighbors(vertex).empty())
                        {
                            std::cout << "# ";
                        }
                        else
                        {
                            std::cout << ". ";
                        }
                    }
                    std::cout << std::endl;
                }

                // Print the path
                if (path.empty())
                {
                    std::cout << "No path found!" << std::endl;
                }
                else
                {
                    std::cout << "Path found with length " << path.size() << ":" << std::endl;

                    for (size_t i = 0; i < path.size(); i++)
                    {
                        const auto &[x, y] = coordinates[path[i]];
                        std::cout << "(" << x << "," << y << ")";

                        if (i < path.size() - 1)
                        {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << std::endl;
                }
            }

            /**
             * @brief Demonstrate bidirectional search on a sample graph
             */
            void demoBidirectionalSearch()
            {
                std::cout << "===== Bidirectional Search Demo =====" << std::endl;

                // Create a sample graph
                auto graph = std::make_unique<AdjacencyListGraph>(false, true);

                // Add vertices (labeled 0 to 14)
                for (int i = 0; i < 15; i++)
                {
                    graph->addVertex(i);
                }

                // Add edges to form a 5x3 grid
                // 0 -- 1 -- 2 -- 3 -- 4
                // |    |    |    |    |
                // 5 -- 6 -- 7 -- 8 -- 9
                // |    |    |    |    |
                // 10-- 11-- 12-- 13-- 14

                // Horizontal edges
                for (int row = 0; row < 3; row++)
                {
                    for (int col = 0; col < 4; col++)
                    {
                        int from = row * 5 + col;
                        int to = from + 1;
                        graph->addEdge(from, to, 1.0);
                    }
                }

                // Vertical edges
                for (int row = 0; row < 2; row++)
                {
                    for (int col = 0; col < 5; col++)
                    {
                        int from = row * 5 + col;
                        int to = from + 5;
                        graph->addEdge(from, to, 1.0);
                    }
                }

                // Add some diagonal edges for variety
                graph->addEdge(0, 6, std::sqrt(2.0));
                graph->addEdge(1, 7, std::sqrt(2.0));
                graph->addEdge(2, 8, std::sqrt(2.0));
                graph->addEdge(5, 11, std::sqrt(2.0));
                graph->addEdge(6, 12, std::sqrt(2.0));
                graph->addEdge(9, 13, std::sqrt(2.0));

                // Remove some edges to create a more interesting graph
                graph->removeEdge(1, 6);
                graph->removeEdge(3, 8);
                graph->removeEdge(7, 12);
                graph->removeEdge(11, 12);

                // Print the graph
                std::cout << "Sample grid graph (showing only a few edges for clarity):" << std::endl;
                std::cout << "0 -- 1 -- 2 -- 3 -- 4" << std::endl;
                std::cout << "| \\  |    |    |    |" << std::endl;
                std::cout << "5    6 -- 7    8 -- 9" << std::endl;
                std::cout << "| \\  | \\     \\ |    |" << std::endl;
                std::cout << "10-- 11   12-- 13-- 14" << std::endl;
                std::cout << std::endl;

                // Define start and end vertices
                int start = 0; // Top-left
                int end = 14;  // Bottom-right

                // Run different search algorithms
                std::cout << "Finding path from " << start << " to " << end << "..." << std::endl;

                // Run Dijkstra's algorithm
                std::cout << "Running Dijkstra's algorithm..." << std::endl;
                double dijkstraDistance;
                auto dijkstraStart = std::chrono::high_resolution_clock::now();
                auto dijkstraPath = dijkstraSingleTarget(*graph, start, end, dijkstraDistance);
                auto dijkstraEnd = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> dijkstraDuration = dijkstraEnd - dijkstraStart;

                // Run bidirectional search
                std::cout << "Running bidirectional search..." << std::endl;
                auto bidirStart = std::chrono::high_resolution_clock::now();
                auto bidirPath = bidirectionalSearch(*graph, start, end);
                auto bidirEnd = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> bidirDuration = bidirEnd - bidirStart;

                // Run bidirectional Dijkstra
                std::cout << "Running bidirectional Dijkstra..." << std::endl;
                double bidirDijkstraDistance;
                auto bidirDijkstraStart = std::chrono::high_resolution_clock::now();
                auto bidirDijkstraPath = bidirectionalDijkstra(*graph, start, end, bidirDijkstraDistance);
                auto bidirDijkstraEnd = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> bidirDijkstraDuration = bidirDijkstraEnd - bidirDijkstraStart;

                // Print results
                std::cout << "\nDijkstra's algorithm:" << std::endl;
                std::cout << "  Path length: " << dijkstraPath.size() << std::endl;
                std::cout << "  Path: ";
                for (size_t i = 0; i < dijkstraPath.size(); i++)
                {
                    std::cout << dijkstraPath[i];
                    if (i < dijkstraPath.size() - 1)
                    {
                        std::cout << " -> ";
                    }
                }
                std::cout << std::endl;
                std::cout << "  Distance: " << dijkstraDistance << std::endl;
                std::cout << "  Time: " << dijkstraDuration.count() << " ms" << std::endl;

                std::cout << "\nBidirectional search:" << std::endl;
                std::cout << "  Path length: " << bidirPath.size() << std::endl;
                std::cout << "  Path: ";
                for (size_t i = 0; i < bidirPath.size(); i++)
                {
                    std::cout << bidirPath[i];
                    if (i < bidirPath.size() - 1)
                    {
                        std::cout << " -> ";
                    }
                }
                std::cout << std::endl;
                std::cout << "  Time: " << bidirDuration.count() << " ms" << std::endl;

                std::cout << "\nBidirectional Dijkstra:" << std::endl;
                std::cout << "  Path length: " << bidirDijkstraPath.size() << std::endl;
                std::cout << "  Path: ";
                for (size_t i = 0; i < bidirDijkstraPath.size(); i++)
                {
                    std::cout << bidirDijkstraPath[i];
                    if (i < bidirDijkstraPath.size() - 1)
                    {
                        std::cout << " -> ";
                    }
                }
                std::cout << std::endl;
                std::cout << "  Distance: " << bidirDijkstraDistance << std::endl;
                std::cout << "  Time: " << bidirDijkstraDuration.count() << " ms" << std::endl;
            }

            /**
             * @brief Demonstrate the navigation system
             */
            void demoNavigationSystem()
            {
                std::cout << "===== Navigation System Demo =====" << std::endl;

                // Create a navigation system
                NavigationSystem nav;

                // Add nodes (representing cities or intersections)
                nav.addNode(1, 40.7128, -74.0060);  // New York
                nav.addNode(2, 34.0522, -118.2437); // Los Angeles
                nav.addNode(3, 41.8781, -87.6298);  // Chicago
                nav.addNode(4, 29.7604, -95.3698);  // Houston
                nav.addNode(5, 39.9526, -75.1652);  // Philadelphia
                nav.addNode(6, 33.4484, -112.0740); // Phoenix
                nav.addNode(7, 29.4241, -98.4936);  // San Antonio
                nav.addNode(8, 32.7157, -117.1611); // San Diego

                // Add roads between cities (with distances in km)
                nav.addRoad(1, 3, 1270.0); // New York - Chicago
                nav.addRoad(1, 5, 151.0);  // New York - Philadelphia
                nav.addRoad(2, 6, 598.0);  // Los Angeles - Phoenix
                nav.addRoad(2, 8, 180.0);  // Los Angeles - San Diego
                nav.addRoad(3, 4, 1514.0); // Chicago - Houston
                nav.addRoad(3, 5, 1094.0); // Chicago - Philadelphia
                nav.addRoad(4, 7, 313.0);  // Houston - San Antonio
                nav.addRoad(6, 7, 1419.0); // Phoenix - San Antonio
                nav.addRoad(6, 8, 490.0);  // Phoenix - San Diego

                // Find a route from New York to San Diego
                std::cout << "Finding route from New York to San Diego..." << std::endl;

                GeoCoordinate newYork(40.7128, -74.0060);
                GeoCoordinate sanDiego(32.7157, -117.1611);

                auto route = nav.findRoute(newYork, sanDiego);
                nav.printRoute(route);
            }

            /**
             * @brief Demonstrate the network router
             */
            void demoNetworkRouter()
            {
                std::cout << "===== Network Router Demo =====" << std::endl;

                // Create a network router
                NetworkRouter router;

                // Add routers
                for (int i = 1; i <= 6; i++)
                {
                    router.addRouter(i);
                }

                // Add links between routers (with bandwidth in Mbps and delay in ms)
                router.addLink(1, 2, 100.0, 10.0); // Router 1 - Router 2
                router.addLink(1, 3, 50.0, 15.0);  // Router 1 - Router 3
                router.addLink(2, 3, 80.0, 12.0);  // Router 2 - Router 3
                router.addLink(2, 4, 60.0, 20.0);  // Router 2 - Router 4
                router.addLink(3, 5, 70.0, 8.0);   // Router 3 - Router 5
                router.addLink(4, 5, 40.0, 25.0);  // Router 4 - Router 5
                router.addLink(4, 6, 90.0, 5.0);   // Router 4 - Router 6
                router.addLink(5, 6, 120.0, 6.0);  // Router 5 - Router 6

                // Update routing tables
                router.updateRoutingTables();

                // Print routing table for Router 1
                router.printRoutingTable(1);

                // Test routing packets
                std::cout << "\nRouting packets:" << std::endl;

                std::pair<int, int> packets[] = {
                    {1, 6}, // Router 1 to Router 6
                    {2, 5}, // Router 2 to Router 5
                    {3, 4}, // Router 3 to Router 4
                    {6, 1}  // Router 6 to Router 1
                };

                for (const auto &[source, dest] : packets)
                {
                    std::cout << "  Packet from Router " << source << " to Router " << dest << ":" << std::endl;

                    // Get the path
                    auto path = router.getPath(source, dest);

                    std::cout << "    Path: ";
                    for (size_t i = 0; i < path.size(); i++)
                    {
                        std::cout << path[i];

                        if (i < path.size() - 1)
                        {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << std::endl;

                    // Get the next hop
                    int nextHop = router.getNextHop(source, dest);
                    std::cout << "    Next hop: " << nextHop << std::endl;
                }

                // Simulate a link failure
                std::cout << "\nSimulating failure of link between Router 3 and Router 5..." << std::endl;
                router.handleLinkFailure(3, 5);

                // Print updated routing table for Router 3
                router.printRoutingTable(3);

                // Test routing packets after failure
                std::cout << "\nRouting packets after link failure:" << std::endl;

                for (const auto &[source, dest] : packets)
                {
                    std::cout << "  Packet from Router " << source << " to Router " << dest << ":" << std::endl;

                    // Get the path
                    auto path = router.getPath(source, dest);

                    std::cout << "    Path: ";
                    if (path.empty())
                    {
                        std::cout << "No path available";
                    }
                    else
                    {
                        for (size_t i = 0; i < path.size(); i++)
                        {
                            std::cout << path[i];

                            if (i < path.size() - 1)
                            {
                                std::cout << " -> ";
                            }
                        }
                    }
                    std::cout << std::endl;

                    // Get the next hop
                    int nextHop = router.getNextHop(source, dest);
                    if (nextHop == -1)
                    {
                        std::cout << "    Next hop: Not available" << std::endl;
                    }
                    else
                    {
                        std::cout << "    Next hop: " << nextHop << std::endl;
                    }
                }
            }

            /**
             * @brief Demonstrate the circuit router
             */
            void demoCircuitRouter()
            {
                std::cout << "===== Circuit Router Demo =====" << std::endl;

                // Create a circuit router with a 15x15 grid
                CircuitRouter router(15, 15);

                // Add some obstacles (representing existing components or wires)
                std::vector<std::pair<int, int>> obstacles = {
                    {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {5, 10}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9}, {11, 3}, {11, 4}, {11, 5}, {11, 6}, {11, 7}, {11, 8}, {11, 9}, {11, 10}};

                for (const auto &coord : obstacles)
                {
                    router.blockCell(coord);
                }

                // Print the initial grid
                std::cout << "Initial grid (. = empty, # = obstacle):" << std::endl;
                router.printGrid();

                // Route a connection
                std::pair<int, int> start = {0, 0};
                std::pair<int, int> end = {14, 14};

                std::cout << "\nRouting connection from (" << start.first << "," << start.second
                          << ") to (" << end.first << "," << end.second << ")..." << std::endl;

                auto route = router.routeConnection(start, end);

                // Print the grid with the route
                std::cout << "Grid with route (S = start, E = end, * = route, # = obstacle):" << std::endl;
                router.printGrid(route);

                // Route another connection
                std::pair<int, int> start2 = {0, 14};
                std::pair<int, int> end2 = {14, 0};

                std::cout << "\nRouting connection from (" << start2.first << "," << start2.second
                          << ") to (" << end2.first << "," << end2.second << ")..." << std::endl;

                auto route2 = router.routeConnection(start2, end2);

                // Print the grid with the route
                std::cout << "Grid with route (S = start, E = end, * = route, # = obstacle):" << std::endl;
                router.printGrid(route2);
            }

            /**
             * @brief Demonstrate the flight planner
             */
            void demoFlightPlanner()
            {
                std::cout << "===== Flight Planner Demo =====" << std::endl;

                // Create a flight planner
                FlightPlanner planner;

                // Add airports
                planner.addAirport(1, "JFK", "New York JFK", 40.6413, -73.7781);
                planner.addAirport(2, "LAX", "Los Angeles", 33.9416, -118.4085);
                planner.addAirport(3, "ORD", "Chicago O'Hare", 41.9742, -87.9073);
                planner.addAirport(4, "ATL", "Atlanta", 33.6407, -84.4277);
                planner.addAirport(5, "LHR", "London Heathrow", 51.4700, -0.4543);
                planner.addAirport(6, "CDG", "Paris Charles de Gaulle", 49.0097, 2.5479);
                planner.addAirport(7, "FRA", "Frankfurt", 50.0379, 8.5622);
                planner.addAirport(8, "DXB", "Dubai", 25.2532, 55.3657);

                // Add flights
                // Parameters: departure, arrival, airline, distance (km), duration (hours), fuel (gallons), price ($)
                planner.addFlight(1, 2, 1, 3983.0, 5.5, 5000.0, 350.0);    // JFK-LAX
                planner.addFlight(1, 3, 1, 1188.0, 2.0, 1500.0, 220.0);    // JFK-ORD
                planner.addFlight(1, 4, 1, 1223.0, 2.2, 1600.0, 240.0);    // JFK-ATL
                planner.addFlight(1, 5, 2, 5541.0, 7.0, 7000.0, 600.0);    // JFK-LHR
                planner.addFlight(2, 3, 1, 2795.0, 4.0, 3500.0, 280.0);    // LAX-ORD
                planner.addFlight(2, 4, 1, 3108.0, 4.5, 4000.0, 300.0);    // LAX-ATL
                planner.addFlight(2, 8, 3, 13420.0, 16.0, 20000.0, 900.0); // LAX-DXB
                planner.addFlight(3, 4, 1, 958.0, 1.5, 1200.0, 180.0);     // ORD-ATL
                planner.addFlight(3, 5, 2, 6347.0, 8.0, 8000.0, 650.0);    // ORD-LHR
                planner.addFlight(4, 5, 2, 6765.0, 8.5, 8500.0, 680.0);    // ATL-LHR
                planner.addFlight(5, 6, 2, 344.0, 1.0, 500.0, 120.0);      // LHR-CDG
                planner.addFlight(5, 7, 2, 654.0, 1.5, 800.0, 150.0);      // LHR-FRA
                planner.addFlight(5, 8, 3, 5502.0, 7.0, 7000.0, 580.0);    // LHR-DXB
                planner.addFlight(6, 7, 2, 479.0, 1.2, 600.0, 130.0);      // CDG-FRA
                planner.addFlight(6, 8, 3, 5246.0, 6.5, 6500.0, 560.0);    // CDG-DXB
                planner.addFlight(7, 8, 3, 4861.0, 6.0, 6000.0, 540.0);    // FRA-DXB

                // Plan a route from JFK to Dubai optimizing for different metrics
                std::cout << "Planning route from JFK to Dubai..." << std::endl;

                std::vector<std::string> metrics = {"distance", "time", "fuel", "price"};

                for (const auto &metric : metrics)
                {
                    std::cout << "\nOptimizing for " << metric << ":" << std::endl;
                    auto route = planner.planRoute(1, 8, metric);
                    planner.printRoute(route, metric);
                }

                // Plan a multi-city route
                std::cout << "\nPlanning a multi-city route: JFK -> LHR -> FRA -> DXB" << std::endl;
                auto multiCityRoute = planner.planMultiCityRoute({1, 5, 7, 8});
                planner.printRoute(multiCityRoute, "distance");
            }

            /**
             * @brief Main function
             */
            int main()
            {
                std::cout << "======================================================" << std::endl;
                std::cout << "=== DAY 31: SHORTEST PATH ALGORITHMS               ===" << std::endl;
                std::cout << "=== Part of #DSAin45 Course                        ===" << std::endl;
                std::cout << "======================================================" << std::endl;
                std::cout << std::endl;

                // Demonstrate Dijkstra's algorithm
                demoDijkstra();
                std::cout << std::endl;

                // Demonstrate Bellman-Ford algorithm
                demoBellmanFord();
                std::cout << std::endl;

                // Demonstrate Floyd-Warshall algorithm
                demoFloydWarshall();
                std::cout << std::endl;

                // Demonstrate Johnson's algorithm
                demoJohnsonAlgorithm();
                std::cout << std::endl;

                // Demonstrate A* search algorithm
                demoAStarSearch();
                std::cout << std::endl;

                // Demonstrate bidirectional search
                demoBidirectionalSearch();
                std::cout << std::endl;

                // Demonstrate real-world applications
                demoNavigationSystem();
                std::cout << std::endl;

                demoNetworkRouter();
                std::cout << std::endl;

                demoCircuitRouter();
                std::cout << std::endl;

                demoFlightPlanner();
                std::cout << std::endl;

                // Run benchmarks (optional - can be commented out for faster execution)
                std::cout << "Running benchmarks (this may take a moment)..." << std::endl;

                benchmarkShortestPathAlgorithms({50, 100, 200}, {0.05, 0.1}, 2);
                benchmarkPathFindingAlgorithms({100, 400, 900}, 2);
                benchmarkNegativeEdgeGraphs({50, 100, 200}, 2);

                std::cout << "======================================================" << std::endl;
                std::cout << "=== End of DAY 31 Demonstrations                   ===" << std::endl;
                std::cout << "======================================================" << std::endl;

                return 0;
            }