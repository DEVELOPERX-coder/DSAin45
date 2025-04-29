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

/**
 * @brief Comprehensive Graph Traversals Implementation for #DSAin45 Day 30
 *
 * This file provides implementations of:
 * - BFS (Breadth-First Search)
 * - DFS (Depth-First Search) - recursive and iterative
 * - Various applications of graph traversals
 * - Performance benchmarks
 *
 * @author #DSAin45 Course
 * @date Day 30 of 45-Day DSA Challenge
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
};

/**
 * @brief Graph implementation using adjacency matrix representation
 */
class AdjacencyMatrixGraph : public IGraph
{
private:
    std::vector<std::vector<double>> matrix;    // The adjacency matrix
    std::unordered_map<int, int> vertexToIndex; // Maps vertex IDs to matrix indices
    std::unordered_map<int, int> indexToVertex; // Maps matrix indices to vertex IDs
    bool directed;                              // Whether the graph is directed
    bool weighted;                              // Whether the graph is weighted
    int edgeCount;                              // Number of edges

public:
    /**
     * @brief Constructor
     */
    AdjacencyMatrixGraph(bool isDirected = false, bool isWeighted = true)
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
        return static_cast<int>(vertexToIndex.size());
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
        return vertexToIndex.find(v) != vertexToIndex.end();
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

        int index = static_cast<int>(matrix.size());
        vertexToIndex[v] = index;
        indexToVertex[index] = v;

        // Resize the matrix to accommodate the new vertex
        matrix.push_back(std::vector<double>(index, 0.0));
        for (int i = 0; i < index; i++)
        {
            matrix[i].push_back(0.0);
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

        int index = vertexToIndex[v];

        // Count and remove edges involving this vertex
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (i != index)
            {
                if (matrix[index][i] != 0.0)
                {
                    edgeCount--;
                }
                if (!directed && matrix[i][index] != 0.0)
                {
                    // For undirected graphs, don't double-count the edges
                    // we've already counted above
                    if (i > index)
                    {
                        edgeCount--;
                    }
                }
                else if (directed && matrix[i][index] != 0.0)
                {
                    edgeCount--;
                }
            }
        }

        // Remove the row corresponding to the vertex
        matrix.erase(matrix.begin() + index);

        // Remove the column corresponding to the vertex
        for (auto &row : matrix)
        {
            row.erase(row.begin() + index);
        }

        // Update the vertex-to-index and index-to-vertex maps
        indexToVertex.erase(index);
        vertexToIndex.erase(v);

        // Update the indices of vertices that come after the removed vertex
        for (auto &entry : vertexToIndex)
        {
            if (entry.second > index)
            {
                entry.second--;
            }
        }
        for (auto &entry : indexToVertex)
        {
            if (entry.first > index)
            {
                indexToVertex[entry.first - 1] = entry.second;
                indexToVertex.erase(entry.first);
            }
        }

        return true;
    }

    /**
     * @brief Get all vertices in the graph
     */
    std::vector<int> getVertices() const override
    {
        std::vector<int> vertices;
        vertices.reserve(vertexToIndex.size());

        for (const auto &entry : vertexToIndex)
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

        int fromIndex = vertexToIndex.at(from);
        int toIndex = vertexToIndex.at(to);

        return matrix[fromIndex][toIndex] != 0.0;
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

        int fromIndex = vertexToIndex[from];
        int toIndex = vertexToIndex[to];

        if (matrix[fromIndex][toIndex] != 0.0)
        {
            return false; // Edge already exists
        }

        matrix[fromIndex][toIndex] = weight;
        edgeCount++;

        if (!directed && from != to)
        {
            matrix[toIndex][fromIndex] = weight;
            // Don't increment edge count for undirected graph, as we count each edge once
        }

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

        int fromIndex = vertexToIndex[from];
        int toIndex = vertexToIndex[to];

        if (matrix[fromIndex][toIndex] == 0.0)
        {
            return false; // Edge doesn't exist
        }

        matrix[fromIndex][toIndex] = 0.0;
        edgeCount--;

        if (!directed && from != to)
        {
            matrix[toIndex][fromIndex] = 0.0;
            // Don't decrement edge count for undirected graph, as we count each edge once
        }

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

        int fromIndex = vertexToIndex.at(from);
        int toIndex = vertexToIndex.at(to);

        double weight = matrix[fromIndex][toIndex];
        return (weight == 0.0) ? std::numeric_limits<double>::infinity() : weight;
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

        int fromIndex = vertexToIndex.at(from);
        int toIndex = vertexToIndex.at(to);

        if (matrix[fromIndex][toIndex] == 0.0)
        {
            return false; // Edge doesn't exist
        }

        matrix[fromIndex][toIndex] = weight;

        if (!directed && from != to)
        {
            matrix[toIndex][fromIndex] = weight;
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

        int vIndex = vertexToIndex.at(v);
        std::vector<int> neighbors;

        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[vIndex][i] != 0.0)
            {
                neighbors.push_back(indexToVertex.at(static_cast<int>(i)));
            }
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

        int vIndex = vertexToIndex.at(v);
        int inDegree = 0;

        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][vIndex] != 0.0)
            {
                inDegree++;
            }
        }

        return inDegree;
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

        int vIndex = vertexToIndex.at(v);
        int outDegree = 0;

        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (matrix[vIndex][i] != 0.0)
            {
                outDegree++;
            }
        }

        return outDegree;
    }

    /**
     * @brief Clear all vertices and edges from the graph
     */
    void clear() override
    {
        matrix.clear();
        vertexToIndex.clear();
        indexToVertex.clear();
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

        oss << "Adjacency Matrix Graph (" << (directed ? "directed" : "undirected")
            << ", " << (weighted ? "weighted" : "unweighted") << "):\n";
        oss << "Vertices: " << vertices.size() << ", Edges: " << edgeCount << "\n\n";

        // Header row
        oss << std::setw(5) << "";
        for (int v : vertices)
        {
            oss << std::setw(5) << v;
        }
        oss << "\n";

        // Matrix rows
        for (int from : vertices)
        {
            oss << std::setw(5) << from;
            int fromIndex = vertexToIndex.at(from);

            for (int to : vertices)
            {
                int toIndex = vertexToIndex.at(to);
                double weight = matrix[fromIndex][toIndex];

                if (weight == 0.0)
                {
                    oss << std::setw(5) << "0";
                }
                else
                {
                    oss << std::setw(5) << weight;
                }
            }
            oss << "\n";
        }

        return oss.str();
    }
};

/****************************************************************************************
 * GRAPH TRAVERSAL ALGORITHMS
 ****************************************************************************************/

/**
 * @brief Breadth-First Search (BFS) traversal of a graph
 *
 * @param graph The graph to traverse
 * @param startVertex The vertex to start the traversal from
 * @param printOutput Whether to print the traversal order
 * @return std::vector<int> The vertices in BFS traversal order
 *
 * Time Complexity: O(V + E) where V is the number of vertices and E is the number of edges
 * Space Complexity: O(V) for the queue and visited set
 */
std::vector<int> BFS(const IGraph &graph, int startVertex, bool printOutput = true)
{
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex))
    {
        if (printOutput)
        {
            std::cout << "Start vertex " << startVertex << " does not exist in the graph." << std::endl;
        }
        return {};
    }

    // Create a queue for BFS
    std::queue<int> queue;

    // Create a set to keep track of visited vertices
    std::unordered_set<int> visited;

    // Result vector to store traversal order
    std::vector<int> result;

    // Mark the start vertex as visited and enqueue it
    visited.insert(startVertex);
    queue.push(startVertex);

    if (printOutput)
    {
        std::cout << "BFS traversal starting from vertex " << startVertex << ": ";
    }

    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        int currentVertex = queue.front();
        queue.pop();

        result.push_back(currentVertex);

        if (printOutput)
        {
            std::cout << currentVertex << " ";
        }

        // Get all adjacent vertices of the dequeued vertex
        // If an adjacent vertex has not been visited, mark it
        // visited and enqueue it
        for (int neighbor : graph.getNeighbors(currentVertex))
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }

    if (printOutput)
    {
        std::cout << std::endl;
    }

    return result;
}

/**
 * @brief BFS traversal with path tracking
 *
 * @param graph The graph to traverse
 * @param startVertex The vertex to start the traversal from
 * @return std::unordered_map<int, int> Map of vertices to their parent in the BFS tree
 *
 * This version of BFS keeps track of the parent of each vertex,
 * which can be used to reconstruct paths from the start vertex to any other vertex.
 */
std::unordered_map<int, int> BFSWithPathTracking(const IGraph &graph, int startVertex)
{
    std::unordered_map<int, int> parent; // Maps each vertex to its parent in the BFS tree
    std::queue<int> queue;
    std::unordered_set<int> visited;

    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex))
    {
        return parent;
    }

    // Mark the start vertex as visited and enqueue it
    visited.insert(startVertex);
    queue.push(startVertex);
    parent[startVertex] = -1; // Start vertex has no parent

    while (!queue.empty())
    {
        int currentVertex = queue.front();
        queue.pop();

        for (int neighbor : graph.getNeighbors(currentVertex))
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                queue.push(neighbor);
                parent[neighbor] = currentVertex; // Set parent
            }
        }
    }

    return parent;
}

/**
 * @brief Reconstruct a path from start to end using a parent map
 *
 * @param parent Map of vertices to their parent
 * @param start Start vertex
 * @param end End vertex
 * @return std::vector<int> Path from start to end, or empty if no path exists
 */
std::vector<int> reconstructPath(const std::unordered_map<int, int> &parent, int start, int end)
{
    std::vector<int> path;

    // Check if end was reached from start
    if (parent.find(end) == parent.end() && end != start)
    {
        return path; // No path exists
    }

    // Start from the end and work backwards
    for (int at = end; at != -1; at = parent.count(at) ? parent.at(at) : -1)
    {
        path.push_back(at);

        if (at == start)
        {
            break; // Reached the start
        }
    }

    // Check if we actually reached the start
    if (path.empty() || path.back() != start)
    {
        return {}; // No path exists
    }

    // Reverse path to get it from start to end
    std::reverse(path.begin(), path.end());

    return path;
}

/**
 * @brief Recursive helper function for DFS
 *
 * @param graph The graph to traverse
 * @param vertex Current vertex
 * @param visited Set of visited vertices
 * @param result Vector to store traversal order
 * @param printOutput Whether to print the traversal order
 */
void DFSRecursiveHelper(
    const IGraph &graph,
    int vertex,
    std::unordered_set<int> &visited,
    std::vector<int> &result,
    bool printOutput)
{
    // Mark the current vertex as visited and add to result
    visited.insert(vertex);
    result.push_back(vertex);

    if (printOutput)
    {
        std::cout << vertex << " ";
    }

    // Recur for all adjacent vertices
    for (int neighbor : graph.getNeighbors(vertex))
    {
        if (visited.find(neighbor) == visited.end())
        {
            DFSRecursiveHelper(graph, neighbor, visited, result, printOutput);
        }
    }
}

/**
 * @brief Depth-First Search (DFS) traversal of a graph (recursive implementation)
 *
 * @param graph The graph to traverse
 * @param startVertex The vertex to start the traversal from
 * @param printOutput Whether to print the traversal order
 * @return std::vector<int> The vertices in DFS traversal order
 *
 * Time Complexity: O(V + E) where V is the number of vertices and E is the number of edges
 * Space Complexity: O(V) for the recursion stack and visited set
 */
std::vector<int> DFSRecursive(const IGraph &graph, int startVertex, bool printOutput = true)
{
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex))
    {
        if (printOutput)
        {
            std::cout << "Start vertex " << startVertex << " does not exist in the graph." << std::endl;
        }
        return {};
    }

    std::unordered_set<int> visited;
    std::vector<int> result;

    if (printOutput)
    {
        std::cout << "DFS traversal (recursive) starting from vertex " << startVertex << ": ";
    }

    DFSRecursiveHelper(graph, startVertex, visited, result, printOutput);

    if (printOutput)
    {
        std::cout << std::endl;
    }

    return result;
}

/**
 * @brief Depth-First Search (DFS) traversal of a graph (iterative implementation)
 *
 * @param graph The graph to traverse
 * @param startVertex The vertex to start the traversal from
 * @param printOutput Whether to print the traversal order
 * @return std::vector<int> The vertices in DFS traversal order
 *
 * Time Complexity: O(V + E) where V is the number of vertices and E is the number of edges
 * Space Complexity: O(V) for the stack and visited set
 */
std::vector<int> DFSIterative(const IGraph &graph, int startVertex, bool printOutput = true)
{
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex))
    {
        if (printOutput)
        {
            std::cout << "Start vertex " << startVertex << " does not exist in the graph." << std::endl;
        }
        return {};
    }

    // Create a stack for DFS
    std::stack<int> stack;

    // Create a set to keep track of visited vertices
    std::unordered_set<int> visited;

    // Result vector to store traversal order
    std::vector<int> result;

    // Push the start vertex to the stack
    stack.push(startVertex);

    if (printOutput)
    {
        std::cout << "DFS traversal (iterative) starting from vertex " << startVertex << ": ";
    }

    while (!stack.empty())
    {
        // Pop a vertex from stack
        int currentVertex = stack.top();
        stack.pop();

        // If the vertex is not visited, mark it as visited and print it
        if (visited.find(currentVertex) == visited.end())
        {
            visited.insert(currentVertex);
            result.push_back(currentVertex);

            if (printOutput)
            {
                std::cout << currentVertex << " ";
            }

            // Push all adjacent vertices to the stack (in reverse order to match recursive DFS)
            std::vector<int> neighbors = graph.getNeighbors(currentVertex);
            for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it)
            {
                if (visited.find(*it) == visited.end())
                {
                    stack.push(*it);
                }
            }
        }
    }

    if (printOutput)
    {
        std::cout << std::endl;
    }

    return result;
}

/**
 * @brief Iterative Deepening Depth-First Search (IDDFS)
 *
 * @param graph The graph to traverse
 * @param startVertex The vertex to start the traversal from
 * @param maxDepth Maximum depth to search
 * @param printOutput Whether to print the traversal order
 * @return std::vector<int> The vertices in IDDFS traversal order
 *
 * Time Complexity: O(b^d) where b is the branching factor and d is the depth
 * Space Complexity: O(d) where d is the depth
 */
std::vector<int> IDDFS(const IGraph &graph, int startVertex, int maxDepth, bool printOutput = true)
{
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex))
    {
        if (printOutput)
        {
            std::cout << "Start vertex " << startVertex << " does not exist in the graph." << std::endl;
        }
        return {};
    }

    std::vector<int> result;
    bool targetFound = false;

    if (printOutput)
    {
        std::cout << "IDDFS traversal starting from vertex " << startVertex << ":" << std::endl;
    }

    for (int depth = 0; depth <= maxDepth; depth++)
    {
        if (printOutput)
        {
            std::cout << "  Depth " << depth << ": ";
        }

        std::unordered_set<int> visited;
        std::vector<int> depthResult;

        // Perform DFS with depth limit
        std::function<bool(int, int)> depthLimitedDFS = [&](int vertex, int depthLimit) -> bool
        {
            visited.insert(vertex);
            depthResult.push_back(vertex);

            if (printOutput)
            {
                std::cout << vertex << " ";
            }

            if (depthLimit == 0)
            {
                return false;
            }

            for (int neighbor : graph.getNeighbors(vertex))
            {
                if (visited.find(neighbor) == visited.end())
                {
                    if (depthLimitedDFS(neighbor, depthLimit - 1))
                    {
                        return true;
                    }
                }
            }

            return false;
        };

        depthLimitedDFS(startVertex, depth);

        if (printOutput)
        {
            std::cout << std::endl;
        }

        // Add unique vertices to result
        for (int vertex : depthResult)
        {
            if (std::find(result.begin(), result.end(), vertex) == result.end())
            {
                result.push_back(vertex);
            }
        }
    }

    return result;
}

/**
 * @brief Find a path between two vertices using Bidirectional BFS
 *
 * @param graph The graph to traverse
 * @param start Start vertex
 * @param end End vertex
 * @return std::vector<int> Path from start to end, or empty if no path exists
 *
 * Time Complexity: O(b^(d/2)) where b is the branching factor and d is the distance between vertices
 * Space Complexity: O(b^(d/2))
 */
std::vector<int> bidirectionalBFS(const IGraph &graph, int start, int end)
{
    // Check if both vertices exist
    if (!graph.hasVertex(start) || !graph.hasVertex(end))
    {
        return {};
    }

    // Check if start and end are the same
    if (start == end)
    {
        return {start};
    }

    // Forward search from start
    std::queue<int> queueStart;
    std::unordered_set<int> visitedStart;
    std::unordered_map<int, int> parentStart;

    // Backward search from end
    std::queue<int> queueEnd;
    std::unordered_set<int> visitedEnd;
    std::unordered_map<int, int> parentEnd;

    // Initialize both searches
    queueStart.push(start);
    visitedStart.insert(start);
    parentStart[start] = -1;

    queueEnd.push(end);
    visitedEnd.insert(end);
    parentEnd[end] = -1;

    int meetingPoint = -1; // Where the two searches meet

    while (!queueStart.empty() && !queueEnd.empty())
    {
        // Expand forward search
        int currentStart = queueStart.front();
        queueStart.pop();

        for (int neighbor : graph.getNeighbors(currentStart))
        {
            if (visitedStart.find(neighbor) == visitedStart.end())
            {
                visitedStart.insert(neighbor);
                queueStart.push(neighbor);
                parentStart[neighbor] = currentStart;

                // Check if we've reached a vertex visited by the backward search
                if (visitedEnd.find(neighbor) != visitedEnd.end())
                {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1)
            break; // Searches have met

        // Expand backward search
        int currentEnd = queueEnd.front();
        queueEnd.pop();

        for (int neighbor : graph.getNeighbors(currentEnd))
        {
            if (visitedEnd.find(neighbor) == visitedEnd.end())
            {
                visitedEnd.insert(neighbor);
                queueEnd.push(neighbor);
                parentEnd[neighbor] = currentEnd;

                // Check if we've reached a vertex visited by the forward search
                if (visitedStart.find(neighbor) != visitedStart.end())
                {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1)
            break; // Searches have met
    }

    // If no meeting point was found, there's no path
    if (meetingPoint == -1)
    {
        return {};
    }

    // Reconstruct the path
    std::vector<int> path;

    // Add the path from start to meeting point
    for (int at = meetingPoint; at != -1; at = parentStart[at])
    {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    // Add the path from meeting point to end (excluding meeting point)
    for (int at = parentEnd[meetingPoint]; at != -1; at = parentEnd[at])
    {
        path.push_back(at);
    }

    return path;
}

/****************************************************************************************
 * GRAPH UTILITY FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Check if a graph is connected (all vertices are reachable from any starting point)
 *
 * @param graph The graph to check
 * @return bool True if the graph is connected, false otherwise
 */
bool isConnected(const IGraph &graph)
{
    // Get all vertices
    std::vector<int> vertices = graph.getVertices();

    // Empty graph is trivially connected
    if (vertices.empty())
    {
        return true;
    }

    // Start BFS from the first vertex
    int startVertex = vertices[0];

    // Get all reachable vertices
    std::vector<int> reachable = BFS(graph, startVertex, false);

    // Check if all vertices are reachable
    return reachable.size() == vertices.size();
}

/**
 * @brief Check if a graph is cyclic (contains a cycle)
 *
 * @param graph The graph to check
 * @return bool True if the graph contains a cycle, false otherwise
 */
bool isCyclic(const IGraph &graph)
{
    // Get all vertices
    std::vector<int> vertices = graph.getVertices();

    // Use different algorithms based on whether the graph is directed
    if (graph.isDirected())
    {
        // For directed graphs, use DFS with three states:
        // 0 = not visited, 1 = visiting, 2 = visited
        std::unordered_map<int, int> state;

        // Initialize all vertices as not visited
        for (int v : vertices)
        {
            state[v] = 0; // not visited
        }

        // DFS function to detect cycles
        std::function<bool(int)> hasCycle = [&](int vertex) -> bool
        {
            state[vertex] = 1; // visiting

            for (int neighbor : graph.getNeighbors(vertex))
            {
                if (state[neighbor] == 1)
                {
                    return true; // Back edge found -> cycle
                }

                if (state[neighbor] == 0 && hasCycle(neighbor))
                {
                    return true;
                }
            }

            state[vertex] = 2; // visited
            return false;
        };

        // Try starting DFS from each vertex
        for (int v : vertices)
        {
            if (state[v] == 0 && hasCycle(v))
            {
                return true;
            }
        }

        return false; // No cycle found
    }
    else
    {
        // For undirected graphs, use DFS with parent pointers
        std::unordered_set<int> visited;

        // DFS function to detect cycles
        std::function<bool(int, int)> hasCycle = [&](int vertex, int parent) -> bool
        {
            visited.insert(vertex);

            for (int neighbor : graph.getNeighbors(vertex))
            {
                // If neighbor is already visited and not the parent, cycle found
                if (visited.find(neighbor) != visited.end())
                {
                    if (neighbor != parent)
                    {
                        return true;
                    }
                }
                else
                {
                    // Recursive call with vertex as the parent
                    if (hasCycle(neighbor, vertex))
                    {
                        return true;
                    }
                }
            }

            return false;
        };

        // Try starting DFS from each vertex
        for (int v : vertices)
        {
            if (visited.find(v) == visited.end())
            {
                if (hasCycle(v, -1))
                {
                    return true;
                }
            }
        }

        return false; // No cycle found
    }
}

/**
 * @brief Find all connected components in an undirected graph
 *
 * @param graph The graph to analyze
 * @return std::vector<std::vector<int>> List of connected components
 */
std::vector<std::vector<int>> findConnectedComponents(const IGraph &graph)
{
    std::vector<std::vector<int>> components;
    std::unordered_set<int> visited;

    // Get all vertices
    std::vector<int> vertices = graph.getVertices();

    // Function to perform DFS and collect all vertices in a component
    std::function<void(int, std::vector<int> &)> dfs = [&](int vertex, std::vector<int> &component)
    {
        visited.insert(vertex);
        component.push_back(vertex);

        for (int neighbor : graph.getNeighbors(vertex))
        {
            if (visited.find(neighbor) == visited.end())
            {
                dfs(neighbor, component);
            }
        }
    };

    // Find all components
    for (int v : vertices)
    {
        if (visited.find(v) == visited.end())
        {
            std::vector<int> component;
            dfs(v, component);
            components.push_back(component);
        }
    }

    return components;
}

/**
 * @brief Perform topological sort on a directed acyclic graph (DAG)
 *
 * @param graph The directed graph to sort
 * @return std::vector<int> Topologically sorted vertices, or empty if graph has a cycle
 */
std::vector<int> topologicalSort(const IGraph &graph)
{
    // Check if the graph is directed
    if (!graph.isDirected())
    {
        return {}; // Topological sort is only defined for directed graphs
    }

    // Check if the graph contains a cycle
    if (isCyclic(graph))
    {
        return {}; // Topological sort is only defined for acyclic graphs
    }

    // Get all vertices
    std::vector<int> vertices = graph.getVertices();

    // Create a vector to store the sorted vertices
    std::vector<int> sorted;

    // Create a set to keep track of visited vertices
    std::unordered_set<int> visited;

    // DFS function to perform topological sort
    std::function<void(int)> dfs = [&](int vertex)
    {
        visited.insert(vertex);

        for (int neighbor : graph.getNeighbors(vertex))
        {
            if (visited.find(neighbor) == visited.end())
            {
                dfs(neighbor);
            }
        }

        // Add current vertex to sorted list (in reverse order)
        sorted.push_back(vertex);
    };

    // Perform DFS on all vertices
    for (int v : vertices)
    {
        if (visited.find(v) == visited.end())
        {
            dfs(v);
        }
    }

    // Reverse the sorted list to get the topological sort
    std::reverse(sorted.begin(), sorted.end());

    return sorted;
}

/**
 * @brief Find articulation points (cut vertices) in an undirected graph
 *
 * @param graph The graph to analyze
 * @return std::vector<int> List of articulation points
 */
std::vector<int> findArticulationPoints(const IGraph &graph)
{
    // Get all vertices
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Create a map from vertex IDs to indices
    std::unordered_map<int, int> vertexToIndex;
    for (int i = 0; i < n; i++)
    {
        vertexToIndex[vertices[i]] = i;
    }

    // Discovery time and lowest time for each vertex
    std::vector<int> disc(n, -1);
    std::vector<int> low(n, -1);

    // Parent of each vertex in the DFS tree
    std::vector<int> parent(n, -1);

    // Set to store articulation points
    std::set<int> articulationPoints;

    // Time counter for DFS
    int time = 0;

    // DFS function to find articulation points
    std::function<void(int)> dfs = [&](int u)
    {
        // Count of children in DFS tree
        int children = 0;

        // Mark current vertex as visited
        disc[u] = low[u] = ++time;

        // Go through all neighbors
        int uVertex = vertices[u];
        for (int vVertex : graph.getNeighbors(uVertex))
        {
            int v = vertexToIndex[vVertex];

            // If v is not visited
            if (disc[v] == -1)
            {
                children++;
                parent[v] = u;

                dfs(v);

                // Check if subtree rooted with v has a connection to one of the ancestors of u
                low[u] = std::min(low[u], low[v]);

                // u is an articulation point in the following cases:
                // 1) u is root of DFS tree and has two or more children
                if (parent[u] == -1 && children > 1)
                {
                    articulationPoints.insert(uVertex);
                }

                // 2) If u is not root and low value of one of its children is more than or equal to discovery value of u
                if (parent[u] != -1 && low[v] >= disc[u])
                {
                    articulationPoints.insert(uVertex);
                }
            }
            // Update low value of u for parent function calls
            else if (v != parent[u])
            {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    // Call DFS for all vertices
    for (int i = 0; i < n; i++)
    {
        if (disc[i] == -1)
        {
            dfs(i);
        }
    }

    // Convert set to vector and return
    return std::vector<int>(articulationPoints.begin(), articulationPoints.end());
}

/**
 * @brief Generate a random graph
 *
 * @param numVertices Number of vertices
 * @param edgeProbability Probability of an edge between any two vertices [0,1]
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param minWeight Minimum edge weight (for weighted graphs)
 * @param maxWeight Maximum edge weight (for weighted graphs)
 * @return std::unique_ptr<IGraph> Pointer to the generated graph
 */
std::unique_ptr<IGraph> generateRandomGraph(
    int numVertices,
    double edgeProbability,
    bool directed = false,
    bool weighted = true,
    double minWeight = 1.0,
    double maxWeight = 10.0)
{
    // Create a new graph
    std::unique_ptr<IGraph> graph = std::make_unique<AdjacencyListGraph>(directed, weighted);

    // Add vertices
    for (int i = 0; i < numVertices; i++)
    {
        graph->addVertex(i);
    }

    // Set up random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> edgeDist(0.0, 1.0);
    std::uniform_real_distribution<double> weightDist(minWeight, maxWeight);

    // Add edges
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            if (i == j)
                continue; // No self-loops

            // For undirected graphs, only add edges where i < j
            if (!directed && i > j)
                continue;

            // Add edge with probability edgeProbability
            if (edgeDist(gen) < edgeProbability)
            {
                double weight = weighted ? weightDist(gen) : 1.0;
                graph->addEdge(i, j, weight);
            }
        }
    }

    return graph;
}

/****************************************************************************************
 * APPLICATIONS OF GRAPH TRAVERSAL
 ****************************************************************************************/

/**
 * @brief Find the shortest path between two vertices in an unweighted graph using BFS
 *
 * @param graph The graph to traverse
 * @param start Start vertex
 * @param end End vertex
 * @return std::pair<std::vector<int>, int> Path and its length, or empty if no path exists
 */
std::pair<std::vector<int>, int> shortestPathBFS(const IGraph &graph, int start, int end)
{
    // Check if both vertices exist
    if (!graph.hasVertex(start) || !graph.hasVertex(end))
    {
        return {{}, -1};
    }

    // Same vertex, path is just the vertex itself
    if (start == end)
    {
        return {{start}, 0};
    }

    // Get parent map from BFS
    std::unordered_map<int, int> parent = BFSWithPathTracking(graph, start);

    // Reconstruct the path
    std::vector<int> path = reconstructPath(parent, start, end);

    // Return path and its length
    return {path, path.empty() ? -1 : static_cast<int>(path.size() - 1)};
}

/**
 * @brief Implement a simple web crawler using BFS
 *
 * @param startUrl The URL to start crawling from
 * @param maxPages Maximum number of pages to crawl
 * @return std::vector<std::string> List of URLs crawled
 *
 * This is a simplified simulation of a web crawler.
 */
std::vector<std::string> webCrawlerBFS(const std::string &startUrl, int maxPages)
{
    // In a real implementation, we would use HTTP requests to get page content
    // For this example, we'll simulate it with a predefined link structure

    // Simulated web structure (URL -> list of links on that page)
    std::unordered_map<std::string, std::vector<std::string>> webGraph = {
        {"https://example.com", {"https://example.com/about", "https://example.com/products", "https://blog.example.com"}},
        {"https://example.com/about", {"https://example.com", "https://example.com/contact"}},
        {"https://example.com/products", {"https://example.com", "https://example.com/product/1", "https://example.com/product/2"}},
        {"https://example.com/product/1", {"https://example.com/products", "https://example.com/checkout"}},
        {"https://example.com/product/2", {"https://example.com/products", "https://example.com/checkout"}},
        {"https://example.com/contact", {"https://example.com"}},
        {"https://example.com/checkout", {"https://example.com", "https://example.com/products"}},
        {"https://blog.example.com", {"https://example.com", "https://blog.example.com/post/1", "https://blog.example.com/post/2"}},
        {"https://blog.example.com/post/1", {"https://blog.example.com"}},
        {"https://blog.example.com/post/2", {"https://blog.example.com"}},
    };

    // Queue for BFS
    std::queue<std::string> queue;

    // Set of visited URLs
    std::unordered_set<std::string> visited;

    // Result vector
    std::vector<std::string> result;

    // Start crawling
    queue.push(startUrl);
    visited.insert(startUrl);

    int pageCount = 0;

    std::cout << "Web Crawler starting from " << startUrl << std::endl;

    while (!queue.empty() && pageCount < maxPages)
    {
        std::string current = queue.front();
        queue.pop();

        // Process the page
        std::cout << "Crawling: " << current << std::endl;
        result.push_back(current);
        pageCount++;

        // Get links from the page
        if (webGraph.find(current) != webGraph.end())
        {
            for (const std::string &link : webGraph[current])
            {
                if (visited.find(link) == visited.end())
                {
                    visited.insert(link);
                    queue.push(link);
                }
            }
        }
    }

    std::cout << "Crawling completed. Visited " << pageCount << " pages." << std::endl;

    return result;
}

/**
 * @brief Solve a maze using DFS
 *
 * @param maze The maze represented as a 2D grid (0 = path, 1 = wall)
 * @param start Start position (row, col)
 * @param end End position (row, col)
 * @return std::vector<std::pair<int, int>> Path through the maze, or empty if no path exists
 */
std::vector<std::pair<int, int>> solveMazeDFS(
    const std::vector<std::vector<int>> &maze,
    const std::pair<int, int> &start,
    const std::pair<int, int> &end)
{
    int rows = maze.size();
    if (rows == 0)
        return {};

    int cols = maze[0].size();
    if (cols == 0)
        return {};

    // Check if start and end positions are valid
    if (start.first < 0 || start.first >= rows || start.second < 0 || start.second >= cols ||
        end.first < 0 || end.first >= rows || end.second < 0 || end.second >= cols ||
        maze[start.first][start.second] == 1 || maze[end.first][end.second] == 1)
    {
        return {};
    }

    // Create a visited matrix
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Create a parent matrix to reconstruct the path
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    // DFS function to solve the maze
    std::function<bool(int, int)> dfs = [&](int row, int col) -> bool
    {
        // Mark current cell as visited
        visited[row][col] = true;

        // Check if we've reached the end
        if (row == end.first && col == end.second)
        {
            return true;
        }

        // Define possible moves (up, right, down, left)
        const int dr[] = {-1, 0, 1, 0};
        const int dc[] = {0, 1, 0, -1};

        // Try all four directions
        for (int i = 0; i < 4; i++)
        {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            // Check if the new position is valid
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
                maze[newRow][newCol] == 0 && !visited[newRow][newCol])
            {

                // Set parent for backtracking
                parent[newRow][newCol] = {row, col};

                // Recursive DFS call
                if (dfs(newRow, newCol))
                {
                    return true;
                }
            }
        }

        return false;
    };

    // Start DFS from the start position
    bool foundPath = dfs(start.first, start.second);

    if (!foundPath)
    {
        return {}; // No path found
    }

    // Reconstruct the path
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> current = end;

    while (current.first != -1 && current.second != -1)
    {
        path.push_back(current);

        if (current == start)
        {
            break;
        }

        current = parent[current.first][current.second];
    }

    // Reverse the path to get it from start to end
    std::reverse(path.begin(), path.end());

    return path;
}

/**
 * @brief Find all friends within k degrees of separation in a social network
 *
 * @param socialNetwork The social network graph
 * @param user The user to start from
 * @param k Maximum degrees of separation
 * @return std::vector<int> List of users within k degrees of separation
 */
std::vector<int> friendsWithinKDegrees(const IGraph &socialNetwork, int user, int k)
{
    // Check if the user exists
    if (!socialNetwork.hasVertex(user))
    {
        return {};
    }

    // Queue for BFS with (user, distance) pairs
    std::queue<std::pair<int, int>> queue;

    // Set of visited users
    std::unordered_set<int> visited;

    // Result vector
    std::vector<int> result;

    // Start BFS
    queue.push({user, 0});
    visited.insert(user);

    while (!queue.empty())
    {
        auto [currentUser, distance] = queue.front();
        queue.pop();

        // If within k degrees and not the starting user, add to result
        if (distance > 0 && distance <= k)
        {
            result.push_back(currentUser);
        }

        // If we've reached k degrees, don't explore further
        if (distance >= k)
        {
            continue;
        }

        // Process friends
        for (int friend_ : socialNetwork.getNeighbors(currentUser))
        {
            if (visited.find(friend_) == visited.end())
            {
                visited.insert(friend_);
                queue.push({friend_, distance + 1});
            }
        }
    }

    return result;
}

/**
 * @brief Generate a maze using DFS
 *
 * @param rows Number of rows in the maze
 * @param cols Number of columns in the maze
 * @return std::vector<std::vector<int>> The generated maze (0 = path, 1 = wall)
 */
std::vector<std::vector<int>> generateMazeDFS(int rows, int cols)
{
    // Initialize maze with all walls
    std::vector<std::vector<int>> maze(rows, std::vector<int>(cols, 1));

    // Define possible moves (up, right, down, left)
    const int dr[] = {-2, 0, 2, 0};
    const int dc[] = {0, 2, 0, -2};

    // Set up random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // DFS function to carve out paths
    std::function<void(int, int)> dfs = [&](int row, int col)
    {
        // Mark current cell as path
        maze[row][col] = 0;

        // Create a random ordering of directions
        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), gen);

        // Try all four directions
        for (int dir : directions)
        {
            int newRow = row + dr[dir];
            int newCol = col + dc[dir];

            // Check if the new position is valid and still a wall
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && maze[newRow][newCol] == 1)
            {
                // Carve a path between current and new cell
                maze[row + dr[dir] / 2][col + dc[dir] / 2] = 0;

                // Continue DFS from the new cell
                dfs(newRow, newCol);
            }
        }
    };

    // Start DFS from a random position
    int startRow = gen() % rows;
    if (startRow % 2 == 0)
        startRow = startRow > 0 ? startRow - 1 : startRow + 1;

    int startCol = gen() % cols;
    if (startCol % 2 == 0)
        startCol = startCol > 0 ? startCol - 1 : startCol + 1;

    dfs(startRow, startCol);

    return maze;
}

/****************************************************************************************
 * PERFORMANCE BENCHMARKS
 ****************************************************************************************/

/**
 * @brief Benchmark different graph traversal algorithms
 *
 * @param graphSizes Vector of graph sizes to test
 * @param edgeDensities Vector of edge densities to test
 * @param iterations Number of iterations for each test
 */
void benchmarkTraversalAlgorithms(
    const std::vector<int> &graphSizes,
    const std::vector<double> &edgeDensities,
    int iterations = 5)
{
    std::cout << "===== Graph Traversal Algorithm Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(10) << "Edges"
              << std::setw(15) << "Density"
              << std::setw(15) << "BFS (ms)"
              << std::setw(20) << "DFS Recursive (ms)"
              << std::setw(20) << "DFS Iterative (ms)"
              << std::endl;

    std::cout << std::string(90, '-') << std::endl;

    // Test different graph sizes and densities
    for (int size : graphSizes)
    {
        for (double density : edgeDensities)
        {
            double bfsTime = 0.0;
            double dfsRecursiveTime = 0.0;
            double dfsIterativeTime = 0.0;
            int totalEdges = 0;

            for (int iter = 0; iter < iterations; ++iter)
            {
                // Generate a random graph
                auto graph = generateRandomGraph(size, density);
                totalEdges = graph->getEdgeCount();

                // Pick a random start vertex
                std::vector<int> vertices = graph->getVertices();
                int startVertex = vertices[0];

                // Benchmark BFS
                auto startBFS = std::chrono::high_resolution_clock::now();
                BFS(*graph, startVertex, false);
                auto endBFS = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> bfsDuration = endBFS - startBFS;
                bfsTime += bfsDuration.count();

                // Benchmark DFS (recursive)
                auto startDFSRecursive = std::chrono::high_resolution_clock::now();
                DFSRecursive(*graph, startVertex, false);
                auto endDFSRecursive = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> dfsRecursiveDuration = endDFSRecursive - startDFSRecursive;
                dfsRecursiveTime += dfsRecursiveDuration.count();

                // Benchmark DFS (iterative)
                auto startDFSIterative = std::chrono::high_resolution_clock::now();
                DFSIterative(*graph, startVertex, false);
                auto endDFSIterative = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> dfsIterativeDuration = endDFSIterative - startDFSIterative;
                dfsIterativeTime += dfsIterativeDuration.count();
            }

            // Calculate averages
            bfsTime /= iterations;
            dfsRecursiveTime /= iterations;
            dfsIterativeTime /= iterations;

            // Print results
            std::cout << std::left << std::setw(10) << size
                      << std::setw(10) << totalEdges
                      << std::setw(15) << std::fixed << std::setprecision(2) << density
                      << std::setw(15) << std::fixed << std::setprecision(2) << bfsTime
                      << std::setw(20) << std::fixed << std::setprecision(2) << dfsRecursiveTime
                      << std::setw(20) << std::fixed << std::setprecision(2) << dfsIterativeTime
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark path finding algorithms
 *
 * @param graphSizes Vector of graph sizes to test
 * @param iterations Number of iterations for each test
 */
void benchmarkPathFinding(
    const std::vector<int> &graphSizes,
    int iterations = 5)
{
    std::cout << "===== Path Finding Algorithm Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(10) << "Edges"
              << std::setw(20) << "BFS Path (ms)"
              << std::setw(20) << "Bidirectional BFS (ms)"
              << std::setw(20) << "Ratio (BFS/BiDir)"
              << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // Edge density for tests
    const double density = 0.1;

    // Test different graph sizes
    for (int size : graphSizes)
    {
        double bfsTime = 0.0;
        double bidirTime = 0.0;
        int totalEdges = 0;

        for (int iter = 0; iter < iterations; ++iter)
        {
            // Generate a random graph
            auto graph = generateRandomGraph(size, density);
            totalEdges = graph->getEdgeCount();

            // Pick random start and end vertices
            std::vector<int> vertices = graph->getVertices();
            int startVertex = vertices[0];
            int endVertex = vertices[vertices.size() - 1];

            // Benchmark BFS Path Finding
            auto startBFS = std::chrono::high_resolution_clock::now();
            auto bfsPath = shortestPathBFS(*graph, startVertex, endVertex);
            auto endBFS = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> bfsDuration = endBFS - startBFS;
            bfsTime += bfsDuration.count();

            // Benchmark Bidirectional BFS
            auto startBiDir = std::chrono::high_resolution_clock::now();
            auto bidirPath = bidirectionalBFS(*graph, startVertex, endVertex);
            auto endBiDir = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> bidirDuration = endBiDir - startBiDir;
            bidirTime += bidirDuration.count();
        }

        // Calculate averages
        bfsTime /= iterations;
        bidirTime /= iterations;

        // Calculate ratio
        double ratio = bidirTime > 0 ? bfsTime / bidirTime : 0.0;

        // Print results
        std::cout << std::left << std::setw(10) << size
                  << std::setw(10) << totalEdges
                  << std::setw(20) << std::fixed << std::setprecision(2) << bfsTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << bidirTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << ratio
                  << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark connected component finding
 *
 * @param graphSizes Vector of graph sizes to test
 * @param componentCounts Vector of target component counts
 * @param iterations Number of iterations for each test
 */
void benchmarkConnectedComponents(
    const std::vector<int> &graphSizes,
    const std::vector<int> &componentCounts,
    int iterations = 5)
{
    std::cout << "===== Connected Component Finding Benchmarks =====" << std::endl;
    std::cout << "Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(15) << "Components"
              << std::setw(10) << "Edges"
              << std::setw(20) << "Time (ms)"
              << std::endl;

    std::cout << std::string(55, '-') << std::endl;

    // Test different graph sizes and component counts
    for (int size : graphSizes)
    {
        for (int targetComponents : componentCounts)
        {
            double time = 0.0;
            int totalEdges = 0;
            int actualComponents = 0;

            for (int iter = 0; iter < iterations; ++iter)
            {
                // Generate a random graph with separate components
                // The strategy is to create targetComponents separate subgraphs,
                // each with size/targetComponents vertices
                auto graph = std::make_unique<AdjacencyListGraph>();

                int verticesPerComponent = size / targetComponents;

                // Add all vertices
                for (int i = 0; i < size; i++)
                {
                    graph->addVertex(i);
                }

                // Add edges within each component
                for (int c = 0; c < targetComponents; c++)
                {
                    int start = c * verticesPerComponent;
                    int end = (c + 1) * verticesPerComponent;
                    if (c == targetComponents - 1)
                    {
                        end = size; // Last component gets any remaining vertices
                    }

                    // Connect vertices within this component
                    double componentDensity = 0.3; // Density within a component
                    for (int i = start; i < end; i++)
                    {
                        for (int j = i + 1; j < end; j++)
                        {
                            if (std::rand() % 100 < componentDensity * 100)
                            {
                                graph->addEdge(i, j);
                            }
                        }
                    }
                }

                totalEdges = graph->getEdgeCount();

                // Benchmark connected component finding
                auto startTime = std::chrono::high_resolution_clock::now();
                auto components = findConnectedComponents(*graph);
                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = endTime - startTime;
                time += duration.count();

                actualComponents = components.size();
            }

            // Calculate average
            time /= iterations;

            // Print results
            std::cout << std::left << std::setw(10) << size
                      << std::setw(15) << actualComponents
                      << std::setw(10) << totalEdges
                      << std::setw(20) << std::fixed << std::setprecision(2) << time
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

/****************************************************************************************
 * DEMONSTRATION FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Demonstrate basic graph traversal algorithms
 */
void demoBasicTraversal()
{
    std::cout << "===== Basic Graph Traversal Demo =====" << std::endl;

    // Create a sample graph
    auto graph = std::make_unique<AdjacencyListGraph>();

    // Add vertices (labeled 0 to 8)
    for (int i = 0; i < 9; i++)
    {
        graph->addVertex(i);
    }

    // Add edges to form a specific structure
    // 0 -- 1 -- 2
    // |    |    |
    // 3 -- 4 -- 5
    // |    |    |
    // 6 -- 7 -- 8

    graph->addEdge(0, 1);
    graph->addEdge(0, 3);
    graph->addEdge(1, 2);
    graph->addEdge(1, 4);
    graph->addEdge(2, 5);
    graph->addEdge(3, 4);
    graph->addEdge(3, 6);
    graph->addEdge(4, 5);
    graph->addEdge(4, 7);
    graph->addEdge(5, 8);
    graph->addEdge(6, 7);
    graph->addEdge(7, 8);

    // Print the graph
    std::cout << "Sample graph:" << std::endl;
    std::cout << graph->toString() << std::endl;

    // Test BFS
    std::cout << "Testing BFS:" << std::endl;
    BFS(*graph, 0);

    // Test DFS (recursive)
    std::cout << "\nTesting DFS (recursive):" << std::endl;
    DFSRecursive(*graph, 0);

    // Test DFS (iterative)
    std::cout << "\nTesting DFS (iterative):" << std::endl;
    DFSIterative(*graph, 0);

    // Test IDDFS
    std::cout << "\nTesting Iterative Deepening DFS:" << std::endl;
    IDDFS(*graph, 0, 3);

    // Compare traversal orders from different start vertices
    std::cout << "\nComparing traversal orders from different start vertices:" << std::endl;

    for (int startVertex : {0, 4, 8})
    {
        std::cout << "\nStarting from vertex " << startVertex << ":" << std::endl;

        std::cout << "BFS: ";
        auto bfsResult = BFS(*graph, startVertex, false);
        for (int v : bfsResult)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;

        std::cout << "DFS (recursive): ";
        auto dfsRecursiveResult = DFSRecursive(*graph, startVertex, false);
        for (int v : dfsRecursiveResult)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;

        std::cout << "DFS (iterative): ";
        auto dfsIterativeResult = DFSIterative(*graph, startVertex, false);
        for (int v : dfsIterativeResult)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Demonstrate graph analysis algorithms
 */
void demoGraphAnalysis()
{
    std::cout << "===== Graph Analysis Demo =====" << std::endl;

    // Create a connected graph
    auto connectedGraph = std::make_unique<AdjacencyListGraph>();

    // Add vertices
    for (int i = 0; i < 5; i++)
    {
        connectedGraph->addVertex(i);
    }

    // Add edges to form a connected graph
    connectedGraph->addEdge(0, 1);
    connectedGraph->addEdge(0, 2);
    connectedGraph->addEdge(1, 2);
    connectedGraph->addEdge(2, 3);
    connectedGraph->addEdge(3, 4);

    std::cout << "Connected Graph:" << std::endl;
    std::cout << connectedGraph->toString() << std::endl;

    // Check if graph is connected
    std::cout << "Is connected: " << (isConnected(*connectedGraph) ? "Yes" : "No") << std::endl;

    // Check if graph is cyclic
    std::cout << "Has cycle: " << (isCyclic(*connectedGraph) ? "Yes" : "No") << std::endl;

    // Find connected components
    std::cout << "Connected components: " << findConnectedComponents(*connectedGraph).size() << std::endl;

    // Find articulation points
    std::cout << "Articulation points: ";
    auto articulationPoints = findArticulationPoints(*connectedGraph);
    if (articulationPoints.empty())
    {
        std::cout << "None";
    }
    else
    {
        for (int v : articulationPoints)
        {
            std::cout << v << " ";
        }
    }
    std::cout << std::endl
              << std::endl;

    // Create a disconnected graph
    auto disconnectedGraph = std::make_unique<AdjacencyListGraph>();

    // Add vertices
    for (int i = 0; i < 8; i++)
    {
        disconnectedGraph->addVertex(i);
    }

    // Add edges to form a disconnected graph with 3 components
    disconnectedGraph->addEdge(0, 1);
    disconnectedGraph->addEdge(1, 2);
    disconnectedGraph->addEdge(3, 4);
    disconnectedGraph->addEdge(5, 6);
    disconnectedGraph->addEdge(6, 7);

    std::cout << "Disconnected Graph:" << std::endl;
    std::cout << disconnectedGraph->toString() << std::endl;

    // Check if graph is connected
    std::cout << "Is connected: " << (isConnected(*disconnectedGraph) ? "Yes" : "No") << std::endl;

    // Check if graph is cyclic
    std::cout << "Has cycle: " << (isCyclic(*disconnectedGraph) ? "Yes" : "No") << std::endl;

    // Find connected components
    auto components = findConnectedComponents(*disconnectedGraph);
    std::cout << "Connected components: " << components.size() << std::endl;

    // Print each component
    std::cout << "Components:" << std::endl;
    for (size_t i = 0; i < components.size(); i++)
    {
        std::cout << "Component " << i + 1 << ": ";
        for (int v : components[i])
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    // Find articulation points
    std::cout << "Articulation points: ";
    articulationPoints = findArticulationPoints(*disconnectedGraph);
    if (articulationPoints.empty())
    {
        std::cout << "None";
    }
    else
    {
        for (int v : articulationPoints)
        {
            std::cout << v << " ";
        }
    }
    std::cout << std::endl
              << std::endl;

    // Create a directed acyclic graph (DAG)
    auto dag = std::make_unique<AdjacencyListGraph>(true);

    // Add vertices
    for (int i = 0; i < 6; i++)
    {
        dag->addVertex(i);
    }

    // Add edges to form a DAG
    dag->addEdge(0, 1);
    dag->addEdge(0, 2);
    dag->addEdge(1, 3);
    dag->addEdge(1, 4);
    dag->addEdge(2, 4);
    dag->addEdge(3, 5);
    dag->addEdge(4, 5);

    std::cout << "Directed Acyclic Graph (DAG):" << std::endl;
    std::cout << dag->toString() << std::endl;

    // Check if graph is cyclic
    std::cout << "Has cycle: " << (isCyclic(*dag) ? "Yes" : "No") << std::endl;

    // Perform topological sort
    std::cout << "Topological sort: ";
    auto sorted = topologicalSort(*dag);
    for (int v : sorted)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl
              << std::endl;

    // Create a directed graph with a cycle
    auto cyclicGraph = std::make_unique<AdjacencyListGraph>(true);

    // Add vertices
    for (int i = 0; i < 4; i++)
    {
        cyclicGraph->addVertex(i);
    }

    // Add edges to form a cycle
    cyclicGraph->addEdge(0, 1);
    cyclicGraph->addEdge(1, 2);
    cyclicGraph->addEdge(2, 3);
    cyclicGraph->addEdge(3, 0); // This creates a cycle

    std::cout << "Directed Cyclic Graph:" << std::endl;
    std::cout << cyclicGraph->toString() << std::endl;

    // Check if graph is cyclic
    std::cout << "Has cycle: " << (isCyclic(*cyclicGraph) ? "Yes" : "No") << std::endl;

    // Try to perform topological sort (should fail for cyclic graph)
    std::cout << "Topological sort: ";
    sorted = topologicalSort(*cyclicGraph);
    if (sorted.empty())
    {
        std::cout << "Not possible (graph has a cycle)";
    }
    else
    {
        for (int v : sorted)
        {
            std::cout << v << " ";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Demonstrate path finding algorithms
 */
void demoPathFinding()
{
    std::cout << "===== Path Finding Demo =====" << std::endl;

    // Create a sample graph for path finding
    auto graph = std::make_unique<AdjacencyListGraph>();

    // Add vertices
    for (int i = 0; i < 8; i++)
    {
        graph->addVertex(i);
    }

    // Add edges to form a specific structure
    // 0 -- 1 -- 2
    // |         |
    // 3         4
    // |         |
    // 5 -- 6 -- 7

    graph->addEdge(0, 1);
    graph->addEdge(0, 3);
    graph->addEdge(1, 2);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);
    graph->addEdge(4, 7);
    graph->addEdge(5, 6);
    graph->addEdge(6, 7);

    std::cout << "Sample graph for path finding:" << std::endl;
    std::cout << graph->toString() << std::endl;

    // Test path finding algorithms
    std::pair<int, int> testPairs[] = {
        {0, 7}, // Path should exist
        {0, 4}, // Path should exist
        {2, 5}, // Path should exist
        {1, 6}  // Path should exist
    };

    for (const auto &[start, end] : testPairs)
    {
        std::cout << "\nFinding path from " << start << " to " << end << ":" << std::endl;

        // BFS Path Finding
        std::cout << "BFS Path Finding:" << std::endl;
        auto [bfsPath, bfsLength] = shortestPathBFS(*graph, start, end);

        if (bfsPath.empty())
        {
            std::cout << "No path found." << std::endl;
        }
        else
        {
            std::cout << "Path (length " << bfsLength << "): ";
            for (size_t i = 0; i < bfsPath.size(); i++)
            {
                std::cout << bfsPath[i];
                if (i < bfsPath.size() - 1)
                {
                    std::cout << " -> ";
                }
            }
            std::cout << std::endl;
        }

        // Bidirectional BFS
        std::cout << "Bidirectional BFS:" << std::endl;
        auto bidirPath = bidirectionalBFS(*graph, start, end);

        if (bidirPath.empty())
        {
            std::cout << "No path found." << std::endl;
        }
        else
        {
            std::cout << "Path (length " << bidirPath.size() - 1 << "): ";
            for (size_t i = 0; i < bidirPath.size(); i++)
            {
                std::cout << bidirPath[i];
                if (i < bidirPath.size() - 1)
                {
                    std::cout << " -> ";
                }
            }
            std::cout << std::endl;
        }
    }
}

/**
 * @brief Demonstrate maze solving using DFS
 */
void demoMazeSolving()
{
    std::cout << "===== Maze Solving Demo =====" << std::endl;

    // Create a sample maze (0 = path, 1 = wall)
    std::vector<std::vector<int>> maze = {
        {0, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}};

    // Print the maze
    std::cout << "Maze (0 = path, 1 = wall):" << std::endl;
    for (const auto &row : maze)
    {
        for (int cell : row)
        {
            std::cout << (cell == 0 ? "  " : "██");
        }
        std::cout << std::endl;
    }

    // Define start and end positions
    std::pair<int, int> start = {0, 0};
    std::pair<int, int> end = {6, 6};

    std::cout << "\nStart: (" << start.first << ", " << start.second << ")" << std::endl;
    std::cout << "End: (" << end.first << ", " << end.second << ")" << std::endl;

    // Solve the maze using DFS
    std::cout << "\nSolving maze using DFS..." << std::endl;
    auto path = solveMazeDFS(maze, start, end);

    if (path.empty())
    {
        std::cout << "No solution found!" << std::endl;
    }
    else
    {
        std::cout << "Solution found! Path length: " << path.size() << std::endl;

        // Create a copy of the maze for visualization
        std::vector<std::vector<char>> solvedMaze(maze.size(), std::vector<char>(maze[0].size(), ' '));

        // Fill in walls
        for (size_t i = 0; i < maze.size(); i++)
        {
            for (size_t j = 0; j < maze[i].size(); j++)
            {
                if (maze[i][j] == 1)
                {
                    solvedMaze[i][j] = '█';
                }
                else
                {
                    solvedMaze[i][j] = ' ';
                }
            }
        }

        // Mark the path
        for (const auto &[row, col] : path)
        {
            solvedMaze[row][col] = '●';
        }

        // Mark start and end
        solvedMaze[start.first][start.second] = 'S';
        solvedMaze[end.first][end.second] = 'E';

        // Print the solved maze
        std::cout << "\nSolved Maze:" << std::endl;
        for (const auto &row : solvedMaze)
        {
            for (char cell : row)
            {
                std::cout << cell << ' ';
            }
            std::cout << std::endl;
        }

        // Print the path coordinates
        std::cout << "\nPath coordinates:" << std::endl;
        for (size_t i = 0; i < path.size(); i++)
        {
            std::cout << "(" << path[i].first << ", " << path[i].second << ")";
            if (i < path.size() - 1)
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    // Generate a random maze using DFS
    std::cout << "\nGenerating a random maze using DFS..." << std::endl;
    int rows = 15, cols = 15;
    auto randomMaze = generateMazeDFS(rows, cols);

    // Print the generated maze
    std::cout << "\nGenerated Maze:" << std::endl;
    for (const auto &row : randomMaze)
    {
        for (int cell : row)
        {
            std::cout << (cell == 0 ? "  " : "██");
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Demonstrate social network analysis
 */
void demoSocialNetwork()
{
    std::cout << "===== Social Network Analysis Demo =====" << std::endl;

    // Create a sample social network
    auto socialNetwork = std::make_unique<AdjacencyListGraph>();

    // Add users
    for (int i = 1; i <= 10; i++)
    {
        socialNetwork->addVertex(i);
    }

    // Add connections (friendships)
    socialNetwork->addEdge(1, 2);
    socialNetwork->addEdge(1, 3);
    socialNetwork->addEdge(1, 4);
    socialNetwork->addEdge(2, 3);
    socialNetwork->addEdge(2, 5);
    socialNetwork->addEdge(3, 4);
    socialNetwork->addEdge(3, 6);
    socialNetwork->addEdge(4, 6);
    socialNetwork->addEdge(5, 7);
    socialNetwork->addEdge(5, 8);
    socialNetwork->addEdge(6, 7);
    socialNetwork->addEdge(7, 8);
    socialNetwork->addEdge(7, 9);
    socialNetwork->addEdge(8, 9);
    socialNetwork->addEdge(8, 10);
    socialNetwork->addEdge(9, 10);

    std::cout << "Social Network Graph:" << std::endl;
    std::cout << socialNetwork->toString() << std::endl;

    // Find friends within different degrees of separation
    int user = 1;
    std::cout << "User " << user << "'s connections:" << std::endl;

    for (int k = 1; k <= 3; k++)
    {
        std::cout << "  Friends within " << k << " degree" << (k == 1 ? "" : "s") << " of separation: ";

        auto friends = friendsWithinKDegrees(*socialNetwork, user, k);

        if (friends.empty())
        {
            std::cout << "None";
        }
        else
        {
            for (size_t i = 0; i < friends.size(); i++)
            {
                std::cout << friends[i];
                if (i < friends.size() - 1)
                {
                    std::cout << ", ";
                }
            }
        }
        std::cout << std::endl;
    }

    // Find shortest paths between users
    std::cout << "\nShortest paths between users:" << std::endl;

    std::pair<int, int> userPairs[] = {
        {1, 10},
        {2, 9},
        {3, 8}};

    for (const auto &[user1, user2] : userPairs)
    {
        std::cout << "  Path from User " << user1 << " to User " << user2 << ": ";

        auto [path, length] = shortestPathBFS(*socialNetwork, user1, user2);

        if (path.empty())
        {
            std::cout << "No connection";
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
            std::cout << " (Degrees of separation: " << length << ")";
        }
        std::cout << std::endl;
    }

    // Find mutual friends
    std::cout << "\nMutual friends:" << std::endl;

    std::pair<int, int> friendPairs[] = {
        {1, 5},
        {2, 6},
        {3, 7}};

    for (const auto &[user1, user2] : friendPairs)
    {
        std::cout << "  Mutual friends of User " << user1 << " and User " << user2 << ": ";

        std::vector<int> mutualFriends;

        // Find mutual friends
        for (int friend1 : socialNetwork->getNeighbors(user1))
        {
            for (int friend2 : socialNetwork->getNeighbors(user2))
            {
                if (friend1 == friend2)
                {
                    mutualFriends.push_back(friend1);
                }
            }
        }

        if (mutualFriends.empty())
        {
            std::cout << "None";
        }
        else
        {
            for (size_t i = 0; i < mutualFriends.size(); i++)
            {
                std::cout << mutualFriends[i];
                if (i < mutualFriends.size() - 1)
                {
                    std::cout << ", ";
                }
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Demonstrate web crawler using BFS
 */
void demoWebCrawler()
{
    std::cout << "===== Web Crawler Demo =====" << std::endl;

    // Run a simulated web crawler
    std::string startUrl = "https://example.com";
    int maxPages = 8;

    std::cout << "Crawling from " << startUrl << " with max pages: " << maxPages << std::endl
              << std::endl;

    auto crawledPages = webCrawlerBFS(startUrl, maxPages);

    std::cout << "\nCrawled " << crawledPages.size() << " pages:" << std::endl;
    for (size_t i = 0; i < crawledPages.size(); i++)
    {
        std::cout << i + 1 << ". " << crawledPages[i] << std::endl;
    }
}

/**
 * @brief Main function
 */
int main()
{
    std::cout << "======================================================" << std::endl;
    std::cout << "=== DAY 30: GRAPH TRAVERSAL ALGORITHMS             ===" << std::endl;
    std::cout << "=== Part of #DSAin45 Course                        ===" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;

    // Demonstrate basic graph traversal
    demoBasicTraversal();
    std::cout << std::endl;

    // Demonstrate graph analysis
    demoGraphAnalysis();
    std::cout << std::endl;

    // Demonstrate path finding
    demoPathFinding();
    std::cout << std::endl;

    // Demonstrate maze solving
    demoMazeSolving();
    std::cout << std::endl;

    // Demonstrate social network analysis
    demoSocialNetwork();
    std::cout << std::endl;

    // Demonstrate web crawler
    demoWebCrawler();
    std::cout << std::endl;

    // Run benchmarks (optional - can be commented out for faster execution)
    std::cout << "Running benchmarks (this may take a moment)..." << std::endl;

    benchmarkTraversalAlgorithms({100, 500, 1000}, {0.01, 0.1}, 2);
    benchmarkPathFinding({100, 500, 1000}, 2);
    benchmarkConnectedComponents({100, 500}, {2, 5}, 2);

    std::cout << "======================================================" << std::endl;
    std::cout << "=== End of DAY 30 Demonstrations                   ===" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}