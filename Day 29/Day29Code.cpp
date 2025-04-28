#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <limits>
#include <iomanip>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <functional>
#include <memory>

/**
 * @brief Comprehensive Graph Implementations for #DSAin45 Day 29
 *
 * This file provides implementations of:
 * - Graph representations (adjacency matrix, adjacency list)
 * - Basic graph operations
 * - Graph utility functions
 * - Performance benchmarks
 * - Real-world applications
 *
 * @author #DSAin45 Course
 * @date Day 29 of 45-Day DSA Challenge
 */

/****************************************************************************************
 * GRAPH REPRESENTATIONS - CLASSES AND IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Interface for graph implementations with common operations
 *
 * This abstract class defines the interface that all specific graph implementations
 * will implement, ensuring consistent behavior across representations.
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
 * @brief Graph implementation using adjacency matrix representation
 *
 * This implementation uses a 2D matrix to store edges between vertices.
 * - Vertex lookup: O(1)
 * - Edge lookup: O(1)
 * - Vertex insertion: O(V²)
 * - Edge insertion: O(1)
 * - Getting all neighbors: O(V)
 * - Memory usage: O(V²)
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
     *
     * @param isDirected Whether the graph is directed
     * @param isWeighted Whether the graph is weighted
     */
    AdjacencyMatrixGraph(bool isDirected = false, bool isWeighted = true)
        : directed(isDirected), weighted(isWeighted), edgeCount(0) {}

    /**
     * @brief Check if the graph is directed
     *
     * @return true if directed, false otherwise
     */
    bool isDirected() const override
    {
        return directed;
    }

    /**
     * @brief Check if the graph is weighted
     *
     * @return true if weighted, false otherwise
     */
    bool isWeighted() const override
    {
        return weighted;
    }

    /**
     * @brief Get the number of vertices in the graph
     *
     * @return Number of vertices
     */
    int getVertexCount() const override
    {
        return static_cast<int>(vertexToIndex.size());
    }

    /**
     * @brief Get the number of edges in the graph
     *
     * @return Number of edges
     */
    int getEdgeCount() const override
    {
        return edgeCount;
    }

    /**
     * @brief Check if a vertex exists in the graph
     *
     * @param v Vertex to check
     * @return true if vertex exists, false otherwise
     */
    bool hasVertex(int v) const override
    {
        return vertexToIndex.find(v) != vertexToIndex.end();
    }

    /**
     * @brief Add a vertex to the graph
     *
     * @param v Vertex to add
     * @return true if added successfully, false if already exists
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
     *
     * @param v Vertex to remove
     * @return true if removed successfully, false if doesn't exist
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
     *
     * @return Vector of vertex IDs
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if edge exists, false otherwise
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @param weight Edge weight (default 1.0)
     * @return true if added successfully, false if vertices don't exist or edge already exists
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if removed successfully, false if vertices or edge don't exist
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @return Edge weight, or infinity if edge doesn't exist
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @param weight New edge weight
     * @return true if weight set successfully, false if vertices or edge don't exist
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
     *
     * @param v Vertex to find neighbors for
     * @return Vector of neighbor vertex IDs
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
     * @brief Get the in-degree of a vertex (number of incoming edges)
     *
     * @param v Vertex to check
     * @return In-degree, or -1 if vertex doesn't exist
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
     * @brief Get the out-degree of a vertex (number of outgoing edges)
     *
     * @param v Vertex to check
     * @return Out-degree, or -1 if vertex doesn't exist
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
     *
     * @return String representation showing adjacency matrix
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

/**
 * @brief Graph implementation using adjacency list representation
 *
 * This implementation uses a map of lists to store vertices and their edges.
 * - Vertex lookup: O(1)
 * - Edge lookup: O(degree(v))
 * - Vertex insertion: O(1)
 * - Edge insertion: O(1)
 * - Getting all neighbors: O(degree(v))
 * - Memory usage: O(V+E)
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
     *
     * @return true if directed, false otherwise
     */
    bool isDirected() const override
    {
        return directed;
    }

    /**
     * @brief Check if the graph is weighted
     *
     * @return true if weighted, false otherwise
     */
    bool isWeighted() const override
    {
        return weighted;
    }

    /**
     * @brief Get the number of vertices in the graph
     *
     * @return Number of vertices
     */
    int getVertexCount() const override
    {
        return static_cast<int>(outEdges.size());
    }

    /**
     * @brief Get the number of edges in the graph
     *
     * @return Number of edges
     */
    int getEdgeCount() const override
    {
        return edgeCount;
    }

    /**
     * @brief Check if a vertex exists in the graph
     *
     * @param v Vertex to check
     * @return true if vertex exists, false otherwise
     */
    bool hasVertex(int v) const override
    {
        return outEdges.find(v) != outEdges.end();
    }

    /**
     * @brief Add a vertex to the graph
     *
     * @param v Vertex to add
     * @return true if added successfully, false if already exists
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
     *
     * @param v Vertex to remove
     * @return true if removed successfully, false if doesn't exist
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
     *
     * @return Vector of vertex IDs
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if edge exists, false otherwise
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @param weight Edge weight (default 1.0)
     * @return true if added successfully, false if vertices don't exist or edge already exists
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @return true if removed successfully, false if vertices or edge don't exist
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @return Edge weight, or infinity if edge doesn't exist
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
     *
     * @param from Source vertex
     * @param to Destination vertex
     * @param weight New edge weight
     * @return true if weight set successfully, false if vertices or edge don't exist
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
     *
     * @param v Vertex to find neighbors for
     * @return Vector of neighbor vertex IDs
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
     * @brief Get the in-degree of a vertex (number of incoming edges)
     *
     * @param v Vertex to check
     * @return In-degree, or -1 if vertex doesn't exist
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
     * @brief Get the out-degree of a vertex (number of outgoing edges)
     *
     * @param v Vertex to check
     * @return Out-degree, or -1 if vertex doesn't exist
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
     *
     * @return String representation showing adjacency lists
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

/****************************************************************************************
 * GRAPH UTILITY FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Creates a random graph with the specified number of vertices and edges
 *
 * @param vertexCount Number of vertices in the graph
 * @param edgeCount Number of edges to generate
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param minWeight Minimum edge weight (for weighted graphs)
 * @param maxWeight Maximum edge weight (for weighted graphs)
 * @param useMatrix Whether to use adjacency matrix representation
 * @return std::unique_ptr<IGraph> Pointer to the created graph
 */
std::unique_ptr<IGraph> createRandomGraph(
    int vertexCount,
    int edgeCount,
    bool directed = false,
    bool weighted = true,
    double minWeight = 1.0,
    double maxWeight = 10.0,
    bool useMatrix = false)
{
    std::unique_ptr<IGraph> graph;
    if (useMatrix)
    {
        graph = std::make_unique<AdjacencyMatrixGraph>(directed, weighted);
    }
    else
    {
        graph = std::make_unique<AdjacencyListGraph>(directed, weighted);
    }

    // Add vertices
    for (int i = 0; i < vertexCount; ++i)
    {
        graph->addVertex(i);
    }

    // Calculate maximum possible edges
    int maxEdges;
    if (directed)
    {
        maxEdges = vertexCount * (vertexCount - 1);
    }
    else
    {
        maxEdges = (vertexCount * (vertexCount - 1)) / 2;
    }

    // Clamp requested edge count to maximum possible
    edgeCount = std::min(edgeCount, maxEdges);

    // Set up random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> vertexDist(0, vertexCount - 1);
    std::uniform_real_distribution<double> weightDist(minWeight, maxWeight);

    // Add random edges
    int addedEdges = 0;
    while (addedEdges < edgeCount)
    {
        int from = vertexDist(gen);
        int to = vertexDist(gen);

        // Skip self-loops and existing edges
        if (from == to || graph->hasEdge(from, to))
        {
            continue;
        }

        double weight = weighted ? weightDist(gen) : 1.0;
        if (graph->addEdge(from, to, weight))
        {
            addedEdges++;
        }
    }

    return graph;
}

/**
 * @brief Creates a complete graph (all vertices connected to all others)
 *
 * @param vertexCount Number of vertices in the graph
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param useMatrix Whether to use adjacency matrix representation
 * @return std::unique_ptr<IGraph> Pointer to the created graph
 */
std::unique_ptr<IGraph> createCompleteGraph(
    int vertexCount,
    bool directed = false,
    bool weighted = false,
    bool useMatrix = true)
{
    std::unique_ptr<IGraph> graph;
    if (useMatrix)
    {
        graph = std::make_unique<AdjacencyMatrixGraph>(directed, weighted);
    }
    else
    {
        graph = std::make_unique<AdjacencyListGraph>(directed, weighted);
    }

    // Add vertices
    for (int i = 0; i < vertexCount; ++i)
    {
        graph->addVertex(i);
    }

    // Connect all vertices to all others
    for (int i = 0; i < vertexCount; ++i)
    {
        for (int j = 0; j < vertexCount; ++j)
        {
            if (i != j)
            { // Skip self-loops
                graph->addEdge(i, j);
            }
        }
    }

    return graph;
}

/**
 * @brief Creates a grid graph (vertices arranged in a grid with edges to adjacent cells)
 *
 * @param rows Number of rows in the grid
 * @param cols Number of columns in the grid
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param useMatrix Whether to use adjacency matrix representation
 * @return std::unique_ptr<IGraph> Pointer to the created graph
 */
std::unique_ptr<IGraph> createGridGraph(
    int rows,
    int cols,
    bool directed = false,
    bool weighted = false,
    bool useMatrix = false)
{
    std::unique_ptr<IGraph> graph;
    if (useMatrix)
    {
        graph = std::make_unique<AdjacencyMatrixGraph>(directed, weighted);
    }
    else
    {
        graph = std::make_unique<AdjacencyListGraph>(directed, weighted);
    }

    // Add vertices
    for (int i = 0; i < rows * cols; ++i)
    {
        graph->addVertex(i);
    }

    // Connect vertices to their neighbors
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int current = r * cols + c;

            // Connect to right neighbor
            if (c < cols - 1)
            {
                graph->addEdge(current, current + 1);
            }

            // Connect to bottom neighbor
            if (r < rows - 1)
            {
                graph->addEdge(current, current + cols);
            }
        }
    }

    return graph;
}

/**
 * @brief Convert a graph from one representation to another
 *
 * @param graph Input graph
 * @param useMatrix Whether to convert to adjacency matrix (true) or adjacency list (false)
 * @return std::unique_ptr<IGraph> Converted graph
 */
std::unique_ptr<IGraph> convertGraph(const IGraph &graph, bool useMatrix)
{
    std::unique_ptr<IGraph> newGraph;

    if (useMatrix)
    {
        newGraph = std::make_unique<AdjacencyMatrixGraph>(graph.isDirected(), graph.isWeighted());
    }
    else
    {
        newGraph = std::make_unique<AdjacencyListGraph>(graph.isDirected(), graph.isWeighted());
    }

    // Copy vertices
    for (int v : graph.getVertices())
    {
        newGraph->addVertex(v);
    }

    // Copy edges
    for (int from : graph.getVertices())
    {
        for (int to : graph.getNeighbors(from))
        {
            double weight = graph.getEdgeWeight(from, to);
            newGraph->addEdge(from, to, weight);
        }
    }

    return newGraph;
}

/**
 * @brief Checks if a graph is connected (every vertex can reach every other)
 *
 * @param graph Input graph
 * @return bool True if connected, false otherwise
 */
bool isConnected(const IGraph &graph)
{
    if (graph.getVertexCount() == 0)
    {
        return true; // Empty graph is trivially connected
    }

    // Get all vertices
    std::vector<int> vertices = graph.getVertices();

    // Run BFS from the first vertex
    std::unordered_set<int> visited;
    std::queue<int> queue;

    queue.push(vertices[0]);
    visited.insert(vertices[0]);

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

    // Check if all vertices were visited
    return visited.size() == static_cast<size_t>(graph.getVertexCount());
}

/**
 * @brief Creates a graph from an adjacency matrix
 *
 * @param matrix The adjacency matrix
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param useMatrixRepr Whether to use adjacency matrix representation for the result
 * @return std::unique_ptr<IGraph> Created graph
 */
std::unique_ptr<IGraph> createGraphFromMatrix(
    const std::vector<std::vector<double>> &matrix,
    bool directed = false,
    bool weighted = true,
    bool useMatrixRepr = true)
{
    std::unique_ptr<IGraph> graph;

    if (useMatrixRepr)
    {
        graph = std::make_unique<AdjacencyMatrixGraph>(directed, weighted);
    }
    else
    {
        graph = std::make_unique<AdjacencyListGraph>(directed, weighted);
    }

    // Add vertices
    int n = static_cast<int>(matrix.size());
    for (int i = 0; i < n; ++i)
    {
        graph->addVertex(i);
    }

    // Add edges
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (matrix[i][j] != 0.0)
            {
                graph->addEdge(i, j, matrix[i][j]);
            }
        }
    }

    return graph;
}

/**
 * @brief Creates a graph from an adjacency list representation
 *
 * @param adjList The adjacency list map (vertex -> list of [neighbor, weight] pairs)
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param useMatrix Whether to use adjacency matrix representation for the result
 * @return std::unique_ptr<IGraph> Created graph
 */
std::unique_ptr<IGraph> createGraphFromAdjList(
    const std::unordered_map<int, std::vector<std::pair<int, double>>> &adjList,
    bool directed = false,
    bool weighted = true,
    bool useMatrix = false)
{
    std::unique_ptr<IGraph> graph;

    if (useMatrix)
    {
        graph = std::make_unique<AdjacencyMatrixGraph>(directed, weighted);
    }
    else
    {
        graph = std::make_unique<AdjacencyListGraph>(directed, weighted);
    }

    // Add vertices
    for (const auto &entry : adjList)
    {
        graph->addVertex(entry.first);
    }

    // Add edges
    for (const auto &entry : adjList)
    {
        int from = entry.first;
        for (const auto &edge : entry.second)
        {
            int to = edge.first;
            double weight = edge.second;
            graph->addEdge(from, to, weight);
        }
    }

    return graph;
}

/**
 * @brief Creates a graph from an edge list
 *
 * @param edges List of edges in the format [from, to, weight]
 * @param vertices Vector of vertex IDs (optional)
 * @param directed Whether the graph is directed
 * @param weighted Whether the graph is weighted
 * @param useMatrix Whether to use adjacency matrix representation
 * @return std::unique_ptr<IGraph> Created graph
 */
std::unique_ptr<IGraph> createGraphFromEdgeList(
    const std::vector<std::tuple<int, int, double>> &edges,
    const std::vector<int> &vertices = {},
    bool directed = false,
    bool weighted = true,
    bool useMatrix = false)
{
    std::unique_ptr<IGraph> graph;

    if (useMatrix)
    {
        graph = std::make_unique<AdjacencyMatrixGraph>(directed, weighted);
    }
    else
    {
        graph = std::make_unique<AdjacencyListGraph>(directed, weighted);
    }

    // Add vertices
    std::unordered_set<int> vertexSet;

    // Add explicit vertices
    for (int v : vertices)
    {
        graph->addVertex(v);
        vertexSet.insert(v);
    }

    // Add vertices from edges
    for (const auto &edge : edges)
    {
        int from = std::get<0>(edge);
        int to = std::get<1>(edge);

        if (vertexSet.find(from) == vertexSet.end())
        {
            graph->addVertex(from);
            vertexSet.insert(from);
        }

        if (vertexSet.find(to) == vertexSet.end())
        {
            graph->addVertex(to);
            vertexSet.insert(to);
        }
    }

    // Add edges
    for (const auto &edge : edges)
    {
        int from = std::get<0>(edge);
        int to = std::get<1>(edge);
        double weight = std::get<2>(edge);

        graph->addEdge(from, to, weight);
    }

    return graph;
}

/****************************************************************************************
 * BENCHMARK FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Measures the performance of graph operations for different graph representations
 *
 * @param vertexCounts Vector of vertex counts to test
 * @param density Edge density to use (0.0 to 1.0)
 * @param iterations Number of iterations for each test
 */
void benchmarkGraphRepresentations(
    const std::vector<int> &vertexCounts,
    double density = 0.1,
    int iterations = 10)
{
    std::cout << "===== Graph Representation Benchmarks =====" << std::endl;
    std::cout << "Edge density: " << density << ", Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(8) << "Vertices"
              << std::setw(15) << "Edges"
              << std::setw(20) << "AdjMatrix (ms)"
              << std::setw(20) << "AdjList (ms)"
              << std::setw(15) << "Matrix/List"
              << std::endl;

    std::cout << std::string(78, '-') << std::endl;

    // Set up random generator for edge weights
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> weightDist(1.0, 10.0);

    // Test different graph sizes
    for (int vertexCount : vertexCounts)
    {
        // Calculate edge count based on density
        int maxEdges = vertexCount * (vertexCount - 1) / 2; // For undirected graphs
        int edgeCount = static_cast<int>(maxEdges * density);

        // Total time for each representation
        double matrixTime = 0.0;
        double listTime = 0.0;

        for (int iter = 0; iter < iterations; ++iter)
        {
            // Create test graphs
            auto matrixGraph = std::make_unique<AdjacencyMatrixGraph>();
            auto listGraph = std::make_unique<AdjacencyListGraph>();

            // Add vertices to both graphs
            auto startMatrix = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < vertexCount; ++i)
            {
                matrixGraph->addVertex(i);
            }
            auto endMatrix = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> matrixVertexTime = endMatrix - startMatrix;

            auto startList = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < vertexCount; ++i)
            {
                listGraph->addVertex(i);
            }
            auto endList = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> listVertexTime = endList - startList;

            // Add random edges to both graphs
            std::vector<std::pair<int, int>> edgesToAdd;
            for (int e = 0; e < edgeCount; ++e)
            {
                int from = gen() % vertexCount;
                int to = gen() % vertexCount;

                // Avoid self-loops and duplicate edges
                if (from != to && !matrixGraph->hasEdge(from, to))
                {
                    edgesToAdd.emplace_back(from, to);
                }
            }

            // Add edges to adjacency matrix
            startMatrix = std::chrono::high_resolution_clock::now();
            for (const auto &edge : edgesToAdd)
            {
                matrixGraph->addEdge(edge.first, edge.second, weightDist(gen));
            }
            endMatrix = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> matrixEdgeTime = endMatrix - startMatrix;

            // Add edges to adjacency list
            startList = std::chrono::high_resolution_clock::now();
            for (const auto &edge : edgesToAdd)
            {
                listGraph->addEdge(edge.first, edge.second, weightDist(gen));
            }
            endList = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> listEdgeTime = endList - startList;

            // Run operations on adjacency matrix
            startMatrix = std::chrono::high_resolution_clock::now();
            // Vertex existence checks
            for (int i = 0; i < 100; ++i)
            {
                int v = gen() % vertexCount;
                matrixGraph->hasVertex(v);
            }

            // Edge existence checks
            for (int i = 0; i < 100; ++i)
            {
                int from = gen() % vertexCount;
                int to = gen() % vertexCount;
                matrixGraph->hasEdge(from, to);
            }

            // Get neighbors
            for (int i = 0; i < 100; ++i)
            {
                int v = gen() % vertexCount;
                matrixGraph->getNeighbors(v);
            }
            endMatrix = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> matrixOpTime = endMatrix - startMatrix;

            // Run operations on adjacency list
            startList = std::chrono::high_resolution_clock::now();
            // Vertex existence checks
            for (int i = 0; i < 100; ++i)
            {
                int v = gen() % vertexCount;
                listGraph->hasVertex(v);
            }

            // Edge existence checks
            for (int i = 0; i < 100; ++i)
            {
                int from = gen() % vertexCount;
                int to = gen() % vertexCount;
                listGraph->hasEdge(from, to);
            }

            // Get neighbors
            for (int i = 0; i < 100; ++i)
            {
                int v = gen() % vertexCount;
                listGraph->getNeighbors(v);
            }
            endList = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> listOpTime = endList - startList;

            // Sum total times
            matrixTime += matrixVertexTime.count() + matrixEdgeTime.count() + matrixOpTime.count();
            listTime += listVertexTime.count() + listEdgeTime.count() + listOpTime.count();
        }

        // Calculate average times
        matrixTime /= iterations;
        listTime /= iterations;

        // Print results
        std::cout << std::left << std::setw(8) << vertexCount
                  << std::setw(15) << edgeCount
                  << std::setw(20) << std::fixed << std::setprecision(2) << matrixTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << listTime
                  << std::setw(15) << std::fixed << std::setprecision(2) << (matrixTime / listTime)
                  << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmarks memory usage for different graph representations
 *
 * @param vertexCounts Vector of vertex counts to test
 * @param densities Vector of edge densities to test
 */
void benchmarkMemoryUsage(
    const std::vector<int> &vertexCounts,
    const std::vector<double> &densities)
{
    std::cout << "===== Graph Memory Usage Benchmarks =====" << std::endl;
    std::cout << "Note: Memory usage is theoretical based on the graph structure" << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Vertices"
              << std::setw(10) << "Density"
              << std::setw(15) << "Edges"
              << std::setw(20) << "Matrix (bytes)"
              << std::setw(20) << "List (bytes)"
              << std::setw(15) << "Matrix/List"
              << std::endl;

    std::cout << std::string(90, '-') << std::endl;

    // Assumptions for memory calculations
    const int POINTER_SIZE = 8; // 64-bit pointers (8 bytes)
    const int INT_SIZE = 4;     // 4 bytes for integers
    const int DOUBLE_SIZE = 8;  // 8 bytes for doubles

    // Test different graph sizes
    for (int vertexCount : vertexCounts)
    {
        for (double density : densities)
        {
            // Calculate edge count based on density
            int maxEdges = vertexCount * (vertexCount - 1) / 2; // For undirected graphs
            int edgeCount = static_cast<int>(maxEdges * density);

            // Calculate theoretical memory usage

            // Adjacency matrix
            // - The matrix itself: V^2 doubles
            // - Vertex ID mappings: 2 * V * (int + pointer)
            size_t matrixMemory = vertexCount * vertexCount * DOUBLE_SIZE +
                                  2 * vertexCount * (INT_SIZE + POINTER_SIZE);

            // Adjacency list
            // - Vertex map: V * (pointer + int)
            // - Edge lists: V * pointer + E * (edge struct: int + double)
            // - For directed graphs with in-edges: additional E * (edge struct)
            size_t listMemory = vertexCount * (POINTER_SIZE + INT_SIZE) +
                                vertexCount * POINTER_SIZE +
                                edgeCount * (INT_SIZE + DOUBLE_SIZE);

            // Print results
            std::cout << std::left << std::setw(10) << vertexCount
                      << std::setw(10) << std::fixed << std::setprecision(2) << density
                      << std::setw(15) << edgeCount
                      << std::setw(20) << matrixMemory
                      << std::setw(20) << listMemory
                      << std::setw(15) << std::fixed << std::setprecision(2) << (static_cast<double>(matrixMemory) / listMemory)
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark performance of edge operations for different densities
 *
 * @param vertexCount Number of vertices in the test graphs
 * @param densities Vector of edge densities to test
 * @param iterations Number of iterations for each test
 */
void benchmarkEdgeOperations(
    int vertexCount,
    const std::vector<double> &densities,
    int iterations = 5)
{
    std::cout << "===== Edge Operation Benchmarks =====" << std::endl;
    std::cout << "Vertices: " << vertexCount << ", Iterations: " << iterations << std::endl
              << std::endl;

    // Print header
    std::cout << std::left << std::setw(10) << "Density"
              << std::setw(15) << "Edges"
              << std::setw(20) << "Matrix Add (ms)"
              << std::setw(20) << "List Add (ms)"
              << std::setw(20) << "Matrix Query (ms)"
              << std::setw(20) << "List Query (ms)"
              << std::endl;

    std::cout << std::string(105, '-') << std::endl;

    // Test for each density level
    for (double density : densities)
    {
        // Calculate edge count based on density
        int maxEdges = vertexCount * (vertexCount - 1) / 2; // For undirected graphs
        int edgeCount = static_cast<int>(maxEdges * density);

        // Prepare edge list
        std::vector<std::pair<int, int>> edges;

        // Generate unique random edges
        std::random_device rd;
        std::mt19937 gen(rd());
        std::set<std::pair<int, int>> edgeSet;

        while (edges.size() < edgeCount)
        {
            int from = gen() % vertexCount;
            int to = gen() % vertexCount;

            if (from != to && edgeSet.find(std::make_pair(from, to)) == edgeSet.end())
            {
                edges.emplace_back(from, to);
                edgeSet.insert(std::make_pair(from, to));
            }
        }

        // Generate random query edges (50% existing, 50% non-existing)
        std::vector<std::pair<int, int>> queryEdges;
        for (int i = 0; i < 1000; ++i)
        {
            if (i < 500 && i < edges.size())
            {
                // Use existing edges for half the queries
                queryEdges.push_back(edges[i]);
            }
            else
            {
                // Generate random non-existing edges
                int from, to;
                do
                {
                    from = gen() % vertexCount;
                    to = gen() % vertexCount;
                } while (from == to || edgeSet.find(std::make_pair(from, to)) != edgeSet.end());

                queryEdges.emplace_back(from, to);
            }
        }

        // Time measurements
        double matrixAddTime = 0.0;
        double listAddTime = 0.0;
        double matrixQueryTime = 0.0;
        double listQueryTime = 0.0;

        for (int iter = 0; iter < iterations; ++iter)
        {
            // Create fresh graphs
            auto matrixGraph = std::make_unique<AdjacencyMatrixGraph>();
            auto listGraph = std::make_unique<AdjacencyListGraph>();

            // Add vertices to both graphs
            for (int i = 0; i < vertexCount; ++i)
            {
                matrixGraph->addVertex(i);
                listGraph->addVertex(i);
            }

            // Measure edge addition for adjacency matrix
            auto startMatrixAdd = std::chrono::high_resolution_clock::now();
            for (const auto &edge : edges)
            {
                matrixGraph->addEdge(edge.first, edge.second);
            }
            auto endMatrixAdd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> matrixAddDuration = endMatrixAdd - startMatrixAdd;
            matrixAddTime += matrixAddDuration.count();

            // Measure edge addition for adjacency list
            auto startListAdd = std::chrono::high_resolution_clock::now();
            for (const auto &edge : edges)
            {
                listGraph->addEdge(edge.first, edge.second);
            }
            auto endListAdd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> listAddDuration = endListAdd - startListAdd;
            listAddTime += listAddDuration.count();

            // Measure edge queries for adjacency matrix
            auto startMatrixQuery = std::chrono::high_resolution_clock::now();
            for (const auto &edge : queryEdges)
            {
                matrixGraph->hasEdge(edge.first, edge.second);
            }
            auto endMatrixQuery = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> matrixQueryDuration = endMatrixQuery - startMatrixQuery;
            matrixQueryTime += matrixQueryDuration.count();

            // Measure edge queries for adjacency list
            auto startListQuery = std::chrono::high_resolution_clock::now();
            for (const auto &edge : queryEdges)
            {
                listGraph->hasEdge(edge.first, edge.second);
            }
            auto endListQuery = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> listQueryDuration = endListQuery - startListQuery;
            listQueryTime += listQueryDuration.count();
        }

        // Calculate averages
        matrixAddTime /= iterations;
        listAddTime /= iterations;
        matrixQueryTime /= iterations;
        listQueryTime /= iterations;

        // Print results
        std::cout << std::left << std::setw(10) << std::fixed << std::setprecision(2) << density
                  << std::setw(15) << edgeCount
                  << std::setw(20) << std::fixed << std::setprecision(2) << matrixAddTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << listAddTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << matrixQueryTime
                  << std::setw(20) << std::fixed << std::setprecision(2) << listQueryTime
                  << std::endl;
    }

    std::cout << std::endl;
}

/****************************************************************************************
 * REAL-WORLD APPLICATIONS
 ****************************************************************************************/

/**
 * @brief Simple social network class using graphs
 */
class SocialNetwork
{
private:
    std::unique_ptr<IGraph> graph;
    std::unordered_map<std::string, int> userIdMap;   // Maps usernames to internal IDs
    std::unordered_map<int, std::string> userNameMap; // Maps internal IDs to usernames
    int nextUserId;

public:
    /**
     * @brief Constructor
     *
     * @param directed Whether connections are directed (follows) or undirected (friends)
     * @param useMatrix Whether to use adjacency matrix representation
     */
    SocialNetwork(bool directed = true, bool useMatrix = false)
        : nextUserId(0)
    {
        if (useMatrix)
        {
            graph = std::make_unique<AdjacencyMatrixGraph>(directed, false);
        }
        else
        {
            graph = std::make_unique<AdjacencyListGraph>(directed, false);
        }
    }

    /**
     * @brief Add a user to the network
     *
     * @param username Username of the new user
     * @return bool True if added successfully, false if username already exists
     */
    bool addUser(const std::string &username)
    {
        if (userIdMap.find(username) != userIdMap.end())
        {
            return false; // User already exists
        }

        int userId = nextUserId++;
        userIdMap[username] = userId;
        userNameMap[userId] = username;

        return graph->addVertex(userId);
    }

    /**
     * @brief Remove a user from the network
     *
     * @param username Username to remove
     * @return bool True if removed successfully, false if user doesn't exist
     */
    bool removeUser(const std::string &username)
    {
        auto it = userIdMap.find(username);
        if (it == userIdMap.end())
        {
            return false; // User doesn't exist
        }

        int userId = it->second;
        bool result = graph->removeVertex(userId);

        if (result)
        {
            userIdMap.erase(username);
            userNameMap.erase(userId);
        }

        return result;
    }

    /**
     * @brief Add a connection between users (friend or follow)
     *
     * @param fromUser Username of the user initiating the connection
     * @param toUser Username of the user receiving the connection
     * @return bool True if connection added successfully, false otherwise
     */
    bool addConnection(const std::string &fromUser, const std::string &toUser)
    {
        auto fromIt = userIdMap.find(fromUser);
        auto toIt = userIdMap.find(toUser);

        if (fromIt == userIdMap.end() || toIt == userIdMap.end())
        {
            return false; // One or both users don't exist
        }

        return graph->addEdge(fromIt->second, toIt->second);
    }

    /**
     * @brief Remove a connection between users
     *
     * @param fromUser Username of the connection source
     * @param toUser Username of the connection target
     * @return bool True if connection removed successfully, false otherwise
     */
    bool removeConnection(const std::string &fromUser, const std::string &toUser)
    {
        auto fromIt = userIdMap.find(fromUser);
        auto toIt = userIdMap.find(toUser);

        if (fromIt == userIdMap.end() || toIt == userIdMap.end())
        {
            return false; // One or both users don't exist
        }

        return graph->removeEdge(fromIt->second, toIt->second);
    }

    /**
     * @brief Check if two users are connected
     *
     * @param fromUser Source username
     * @param toUser Target username
     * @return bool True if connected, false otherwise
     */
    bool areConnected(const std::string &fromUser, const std::string &toUser)
    {
        auto fromIt = userIdMap.find(fromUser);
        auto toIt = userIdMap.find(toUser);

        if (fromIt == userIdMap.end() || toIt == userIdMap.end())
        {
            return false; // One or both users don't exist
        }

        return graph->hasEdge(fromIt->second, toIt->second);
    }

    /**
     * @brief Get all connections of a user
     *
     * @param username Username to get connections for
     * @return std::vector<std::string> List of connected usernames
     */
    std::vector<std::string> getConnections(const std::string &username)
    {
        auto it = userIdMap.find(username);
        if (it == userIdMap.end())
        {
            return {}; // User doesn't exist
        }

        int userId = it->second;
        std::vector<int> neighborIds = graph->getNeighbors(userId);

        std::vector<std::string> neighbors;
        neighbors.reserve(neighborIds.size());

        for (int id : neighborIds)
        {
            neighbors.push_back(userNameMap[id]);
        }

        return neighbors;
    }

    /**
     * @brief Get mutual connections between two users
     *
     * @param user1 First username
     * @param user2 Second username
     * @return std::vector<std::string> List of mutual connections
     */
    std::vector<std::string> getMutualConnections(const std::string &user1, const std::string &user2)
    {
        auto it1 = userIdMap.find(user1);
        auto it2 = userIdMap.find(user2);

        if (it1 == userIdMap.end() || it2 == userIdMap.end())
        {
            return {}; // One or both users don't exist
        }

        int id1 = it1->second;
        int id2 = it2->second;

        std::vector<int> neighbors1 = graph->getNeighbors(id1);
        std::vector<int> neighbors2 = graph->getNeighbors(id2);

        std::sort(neighbors1.begin(), neighbors1.end());
        std::sort(neighbors2.begin(), neighbors2.end());

        std::vector<int> mutualIds;
        std::set_intersection(
            neighbors1.begin(), neighbors1.end(),
            neighbors2.begin(), neighbors2.end(),
            std::back_inserter(mutualIds));

        std::vector<std::string> mutualNames;
        mutualNames.reserve(mutualIds.size());

        for (int id : mutualIds)
        {
            mutualNames.push_back(userNameMap[id]);
        }

        return mutualNames;
    }

    /**
     * @brief Suggest friends/connections for a user
     *
     * @param username Username to suggest connections for
     * @param limit Maximum number of suggestions
     * @return std::vector<std::string> List of suggested usernames
     */
    std::vector<std::string> suggestConnections(const std::string &username, int limit = 5)
    {
        auto it = userIdMap.find(username);
        if (it == userIdMap.end())
        {
            return {}; // User doesn't exist
        }

        int userId = it->second;

        // Get direct connections
        std::vector<int> directConnections = graph->getNeighbors(userId);
        std::unordered_set<int> directConnectionSet(directConnections.begin(), directConnections.end());

        // Map of potential connections and their scores (number of mutual connections)
        std::map<int, int> potentialConnections;

        // For each direct connection, look at their connections
        for (int friendId : directConnections)
        {
            std::vector<int> friendsOfFriend = graph->getNeighbors(friendId);

            for (int potentialId : friendsOfFriend)
            {
                // Skip if it's the original user or already a direct connection
                if (potentialId == userId || directConnectionSet.find(potentialId) != directConnectionSet.end())
                {
                    continue;
                }

                // Increment the score for this potential connection
                potentialConnections[potentialId]++;
            }
        }

        // Convert to vector for sorting
        std::vector<std::pair<int, int>> potentialVector;
        for (const auto &entry : potentialConnections)
        {
            potentialVector.emplace_back(entry.first, entry.second);
        }

        // Sort by score (number of mutual connections) in descending order
        std::sort(potentialVector.begin(), potentialVector.end(),
                  [](const auto &a, const auto &b)
                  { return a.second > b.second; });

        // Convert to usernames and limit the results
        std::vector<std::string> suggestions;
        for (size_t i = 0; i < potentialVector.size() && i < static_cast<size_t>(limit); ++i)
        {
            suggestions.push_back(userNameMap[potentialVector[i].first]);
        }

        return suggestions;
    }

    /**
     * @brief Find the shortest path between two users
     *
     * @param fromUser Source username
     * @param toUser Target username
     * @return std::vector<std::string> Path of usernames, empty if no path exists
     */
    std::vector<std::string> findPath(const std::string &fromUser, const std::string &toUser)
    {
        auto fromIt = userIdMap.find(fromUser);
        auto toIt = userIdMap.find(toUser);

        if (fromIt == userIdMap.end() || toIt == userIdMap.end())
        {
            return {}; // One or both users don't exist
        }

        int fromId = fromIt->second;
        int toId = toIt->second;

        // If they're the same user or directly connected, return the trivial path
        if (fromId == toId)
        {
            return {fromUser};
        }

        if (graph->hasEdge(fromId, toId))
        {
            return {fromUser, toUser};
        }

        // BFS to find shortest path
        std::queue<int> queue;
        std::unordered_map<int, int> previous; // Maps each user to the previous user in the path
        std::unordered_set<int> visited;

        queue.push(fromId);
        visited.insert(fromId);

        while (!queue.empty())
        {
            int current = queue.front();
            queue.pop();

            if (current == toId)
            {
                break; // Found the target
            }

            for (int neighbor : graph->getNeighbors(current))
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    previous[neighbor] = current;
                    queue.push(neighbor);
                }
            }
        }

        // Check if we found a path
        if (previous.find(toId) == previous.end())
        {
            return {}; // No path exists
        }

        // Reconstruct the path
        std::vector<int> pathIds;
        int current = toId;

        while (current != fromId)
        {
            pathIds.push_back(current);
            current = previous[current];
        }

        pathIds.push_back(fromId);
        std::reverse(pathIds.begin(), pathIds.end());

        // Convert to usernames
        std::vector<std::string> path;
        path.reserve(pathIds.size());

        for (int id : pathIds)
        {
            path.push_back(userNameMap[id]);
        }

        return path;
    }

    /**
     * @brief Calculate the degrees of separation between all users
     *
     * @return double Average degree of separation
     */
    double calculateAverageDegreesOfSeparation()
    {
        std::vector<int> allUsers = graph->getVertices();
        int n = static_cast<int>(allUsers.size());

        if (n <= 1)
        {
            return 0.0; // No separation for 0 or 1 user
        }

        int totalPaths = 0;
        int totalLength = 0;

        // For each pair of users, find the shortest path
        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                int fromId = allUsers[i];
                int toId = allUsers[j];

                // BFS to find shortest path length
                std::queue<std::pair<int, int>> queue; // Pair of (node, distance)
                std::unordered_set<int> visited;

                queue.emplace(fromId, 0);
                visited.insert(fromId);

                bool pathFound = false;

                while (!queue.empty() && !pathFound)
                {
                    auto [current, distance] = queue.front();
                    queue.pop();

                    if (current == toId)
                    {
                        totalLength += distance;
                        totalPaths++;
                        pathFound = true;
                        break;
                    }

                    for (int neighbor : graph->getNeighbors(current))
                    {
                        if (visited.find(neighbor) == visited.end())
                        {
                            visited.insert(neighbor);
                            queue.emplace(neighbor, distance + 1);
                        }
                    }
                }
            }
        }

        // Calculate the average
        if (totalPaths == 0)
        {
            return std::numeric_limits<double>::infinity(); // Graph is disconnected
        }

        return static_cast<double>(totalLength) / totalPaths;
    }

    /**
     * @brief Get string representation of the social network
     *
     * @return std::string Representation showing users and their connections
     */
    std::string toString() const
    {
        std::ostringstream oss;

        oss << "Social Network (" << (graph->isDirected() ? "follows" : "friends") << "):\n";
        oss << "Users: " << userIdMap.size() << "\n\n";

        // Get sorted list of usernames
        std::vector<std::string> usernames;
        usernames.reserve(userIdMap.size());

        for (const auto &entry : userIdMap)
        {
            usernames.push_back(entry.first);
        }

        std::sort(usernames.begin(), usernames.end());

        // Print connections for each user
        for (const auto &username : usernames)
        {
            int userId = userIdMap.at(username);
            std::vector<int> neighborIds = graph->getNeighbors(userId);

            oss << username << " -> [ ";

            if (neighborIds.empty())
            {
                oss << "No connections";
            }
            else
            {
                std::vector<std::string> neighborNames;
                neighborNames.reserve(neighborIds.size());

                for (int id : neighborIds)
                {
                    neighborNames.push_back(userNameMap.at(id));
                }

                std::sort(neighborNames.begin(), neighborNames.end());

                for (size_t i = 0; i < neighborNames.size(); ++i)
                {
                    oss << neighborNames[i];
                    if (i < neighborNames.size() - 1)
                    {
                        oss << ", ";
                    }
                }
            }

            oss << " ]\n";
        }

        return oss.str();
    }
};

/**
 * @brief A simple transportation network class using graphs
 */
class TransportationNetwork
{
private:
    std::unique_ptr<IGraph> graph;
    std::unordered_map<std::string, int> locationIdMap;   // Maps location names to internal IDs
    std::unordered_map<int, std::string> locationNameMap; // Maps internal IDs to location names
    int nextLocationId;

public:
    /**
     * @brief Constructor
     *
     * @param directed Whether routes are one-way (true) or two-way (false)
     * @param useMatrix Whether to use adjacency matrix representation
     */
    TransportationNetwork(bool directed = false, bool useMatrix = false)
        : nextLocationId(0)
    {
        if (useMatrix)
        {
            graph = std::make_unique<AdjacencyMatrixGraph>(directed, true);
        }
        else
        {
            graph = std::make_unique<AdjacencyListGraph>(directed, true);
        }
    }

    /**
     * @brief Add a location to the network
     *
     * @param locationName Name of the location
     * @return bool True if added successfully, false if location already exists
     */
    bool addLocation(const std::string &locationName)
    {
        if (locationIdMap.find(locationName) != locationIdMap.end())
        {
            return false; // Location already exists
        }

        int locationId = nextLocationId++;
        locationIdMap[locationName] = locationId;
        locationNameMap[locationId] = locationName;

        return graph->addVertex(locationId);
    }

    /**
     * @brief Remove a location from the network
     *
     * @param locationName Name of the location to remove
     * @return bool True if removed successfully, false if location doesn't exist
     */
    bool removeLocation(const std::string &locationName)
    {
        auto it = locationIdMap.find(locationName);
        if (it == locationIdMap.end())
        {
            return false; // Location doesn't exist
        }

        int locationId = it->second;
        bool result = graph->removeVertex(locationId);

        if (result)
        {
            locationIdMap.erase(locationName);
            locationNameMap.erase(locationId);
        }

        return result;
    }

    /**
     * @brief Add a route between locations with a given distance
     *
     * @param fromLocation Starting location name
     * @param toLocation Ending location name
     * @param distance Distance between locations
     * @return bool True if route added successfully, false otherwise
     */
    bool addRoute(const std::string &fromLocation, const std::string &toLocation, double distance)
    {
        auto fromIt = locationIdMap.find(fromLocation);
        auto toIt = locationIdMap.find(toLocation);

        if (fromIt == locationIdMap.end() || toIt == locationIdMap.end())
        {
            return false; // One or both locations don't exist
        }

        return graph->addEdge(fromIt->second, toIt->second, distance);
    }

    /**
     * @brief Remove a route between locations
     *
     * @param fromLocation Starting location name
     * @param toLocation Ending location name
     * @return bool True if route removed successfully, false otherwise
     */
    bool removeRoute(const std::string &fromLocation, const std::string &toLocation)
    {
        auto fromIt = locationIdMap.find(fromLocation);
        auto toIt = locationIdMap.find(toLocation);

        if (fromIt == locationIdMap.end() || toIt == locationIdMap.end())
        {
            return false; // One or both locations don't exist
        }

        return graph->removeEdge(fromIt->second, toIt->second);
    }

    /**
     * @brief Find the shortest path between two locations
     *
     * @param fromLocation Starting location name
     * @param toLocation Ending location name
     * @return std::pair<std::vector<std::string>, double> Path and total distance
     */
    std::pair<std::vector<std::string>, double> findShortestPath(
        const std::string &fromLocation,
        const std::string &toLocation)
    {
        auto fromIt = locationIdMap.find(fromLocation);
        auto toIt = locationIdMap.find(toLocation);

        if (fromIt == locationIdMap.end() || toIt == locationIdMap.end())
        {
            return {{}, std::numeric_limits<double>::infinity()}; // One or both locations don't exist
        }

        int fromId = fromIt->second;
        int toId = toIt->second;

        // If they're the same location, return the trivial path
        if (fromId == toId)
        {
            return {{fromLocation}, 0.0};
        }

        // Dijkstra's algorithm for shortest path
        std::unordered_map<int, double> distance; // Maps location IDs to distances
        std::unordered_map<int, int> previous;    // Maps each location to the previous in the path
        std::unordered_set<int> unvisited;        // Set of unvisited locations

        // Initialize all distances to infinity
        for (int id : graph->getVertices())
        {
            distance[id] = std::numeric_limits<double>::infinity();
            unvisited.insert(id);
        }

        // Distance to starting location is 0
        distance[fromId] = 0.0;

        // Process each location
        while (!unvisited.empty())
        {
            // Find the unvisited location with the smallest distance
            int current = -1;
            double smallestDistance = std::numeric_limits<double>::infinity();

            for (int id : unvisited)
            {
                if (distance[id] < smallestDistance)
                {
                    smallestDistance = distance[id];
                    current = id;
                }
            }

            // If we can't find a location or the smallest distance is infinity,
            // there's no path to the remaining locations
            if (current == -1 || smallestDistance == std::numeric_limits<double>::infinity())
            {
                break;
            }

            // If we've reached the destination, we're done
            if (current == toId)
            {
                break;
            }

            // Remove the current location from unvisited
            unvisited.erase(current);

            // Update distances to neighbors
            for (int neighbor : graph->getNeighbors(current))
            {
                if (unvisited.find(neighbor) != unvisited.end())
                {
                    double edgeWeight = graph->getEdgeWeight(current, neighbor);
                    double alternativeDistance = distance[current] + edgeWeight;

                    if (alternativeDistance < distance[neighbor])
                    {
                        distance[neighbor] = alternativeDistance;
                        previous[neighbor] = current;
                    }
                }
            }
        }

        // Check if we found a path
        if (distance[toId] == std::numeric_limits<double>::infinity())
        {
            return {{}, std::numeric_limits<double>::infinity()}; // No path exists
        }

        // Reconstruct the path
        std::vector<int> pathIds;
        int current = toId;

        while (current != fromId)
        {
            pathIds.push_back(current);
            current = previous[current];
        }

        pathIds.push_back(fromId);
        std::reverse(pathIds.begin(), pathIds.end());

        // Convert to location names
        std::vector<std::string> path;
        path.reserve(pathIds.size());

        for (int id : pathIds)
        {
            path.push_back(locationNameMap[id]);
        }

        return {path, distance[toId]};
    }

    /**
     * @brief Find all accessible locations from a starting point
     *
     * @param fromLocation Starting location name
     * @return std::vector<std::string> List of accessible location names
     */
    std::vector<std::string> findAccessibleLocations(const std::string &fromLocation)
    {
        auto it = locationIdMap.find(fromLocation);
        if (it == locationIdMap.end())
        {
            return {}; // Location doesn't exist
        }

        int fromId = it->second;

        // BFS to find all accessible locations
        std::queue<int> queue;
        std::unordered_set<int> visited;

        queue.push(fromId);
        visited.insert(fromId);

        while (!queue.empty())
        {
            int current = queue.front();
            queue.pop();

            for (int neighbor : graph->getNeighbors(current))
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    queue.push(neighbor);
                }
            }
        }

        // Convert location IDs to names (excluding the starting location)
        std::vector<std::string> accessibleLocations;
        for (int id : visited)
        {
            if (id != fromId)
            {
                accessibleLocations.push_back(locationNameMap[id]);
            }
        }

        std::sort(accessibleLocations.begin(), accessibleLocations.end());
        return accessibleLocations;
    }

    /**
     * @brief Get a string representation of the transportation network
     *
     * @return std::string Representation showing locations and routes
     */
    std::string toString() const
    {
        std::ostringstream oss;

        oss << "Transportation Network (" << (graph->isDirected() ? "one-way" : "two-way") << " routes):\n";
        oss << "Locations: " << locationIdMap.size() << "\n\n";

        // Get sorted list of location names
        std::vector<std::string> locationNames;
        locationNames.reserve(locationIdMap.size());

        for (const auto &entry : locationIdMap)
        {
            locationNames.push_back(entry.first);
        }

        std::sort(locationNames.begin(), locationNames.end());

        // Print routes for each location
        for (const auto &locationName : locationNames)
        {
            int locationId = locationIdMap.at(locationName);
            std::vector<int> neighborIds = graph->getNeighbors(locationId);

            oss << locationName << " -> ";

            if (neighborIds.empty())
            {
                oss << "No routes";
            }
            else
            {
                oss << "[ ";

                std::vector<std::pair<std::string, double>> routes;
                for (int id : neighborIds)
                {
                    double distance = graph->getEdgeWeight(locationId, id);
                    routes.emplace_back(locationNameMap.at(id), distance);
                }

                std::sort(routes.begin(), routes.end());

                for (size_t i = 0; i < routes.size(); ++i)
                {
                    oss << routes[i].first << " (" << routes[i].second << " km)";
                    if (i < routes.size() - 1)
                    {
                        oss << ", ";
                    }
                }

                oss << " ]";
            }

            oss << "\n";
        }

        return oss.str();
    }
};

/****************************************************************************************
 * DEMONSTRATION FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Demonstrate basic graph operations with both representations
 */
void demoBasicOperations()
{
    std::cout << "===== Basic Graph Operations Demo =====" << std::endl;

    // Create an adjacency matrix graph
    std::cout << "Creating an adjacency matrix graph..." << std::endl;
    AdjacencyMatrixGraph matrixGraph(false, true);

    // Add vertices
    matrixGraph.addVertex(1);
    matrixGraph.addVertex(2);
    matrixGraph.addVertex(3);
    matrixGraph.addVertex(4);
    matrixGraph.addVertex(5);

    // Add edges
    matrixGraph.addEdge(1, 2, 7.5);
    matrixGraph.addEdge(1, 3, 9.2);
    matrixGraph.addEdge(2, 4, 15.0);
    matrixGraph.addEdge(3, 4, 11.3);
    matrixGraph.addEdge(4, 5, 6.0);

    // Display the graph
    std::cout << matrixGraph.toString() << std::endl;

    // Create an adjacency list graph with the same structure
    std::cout << "Creating an adjacency list graph..." << std::endl;
    AdjacencyListGraph listGraph(false, true);

    // Add vertices
    listGraph.addVertex(1);
    listGraph.addVertex(2);
    listGraph.addVertex(3);
    listGraph.addVertex(4);
    listGraph.addVertex(5);

    // Add edges
    listGraph.addEdge(1, 2, 7.5);
    listGraph.addEdge(1, 3, 9.2);
    listGraph.addEdge(2, 4, 15.0);
    listGraph.addEdge(3, 4, 11.3);
    listGraph.addEdge(4, 5, 6.0);

    // Display the graph
    std::cout << listGraph.toString() << std::endl;

    // Demonstrate operations on the adjacency list graph
    std::cout << "Demonstrating operations on the adjacency list graph:" << std::endl;

    // Get neighbors
    std::cout << "Neighbors of vertex 1: ";
    std::vector<int> neighbors = listGraph.getNeighbors(1);
    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        std::cout << neighbors[i];
        if (i < neighbors.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Get edge weight
    std::cout << "Weight of edge (1, 3): " << listGraph.getEdgeWeight(1, 3) << std::endl;

    // Modify the graph
    std::cout << "Removing vertex 3..." << std::endl;
    listGraph.removeVertex(3);

    // Display the modified graph
    std::cout << listGraph.toString() << std::endl;

    // Add a new vertex and connect it
    std::cout << "Adding vertex 6 and connecting it to vertices 1 and 5..." << std::endl;
    listGraph.addVertex(6);
    listGraph.addEdge(1, 6, 8.7);
    listGraph.addEdge(5, 6, 12.4);

    // Display the final graph
    std::cout << listGraph.toString() << std::endl;
}

/**
 * @brief Demonstrate graph representation conversions
 */
void demoConversion()
{
    std::cout << "===== Graph Representation Conversion Demo =====" << std::endl;

    // Create an adjacency list graph
    std::cout << "Creating an adjacency list graph..." << std::endl;
    AdjacencyListGraph listGraph(true, true); // Directed, weighted

    // Add vertices and edges
    for (int i = 1; i <= 5; ++i)
    {
        listGraph.addVertex(i);
    }

    listGraph.addEdge(1, 2, 3.5);
    listGraph.addEdge(1, 3, 7.2);
    listGraph.addEdge(2, 4, 1.8);
    listGraph.addEdge(3, 1, 2.0); // Cycle back to 1
    listGraph.addEdge(3, 5, 9.1);
    listGraph.addEdge(4, 3, 2.7);
    listGraph.addEdge(5, 4, 6.3);

    // Display the original graph
    std::cout << "Original Adjacency List Graph:" << std::endl;
    std::cout << listGraph.toString() << std::endl;

    // Convert to adjacency matrix
    std::cout << "Converting to Adjacency Matrix representation..." << std::endl;
    std::unique_ptr<IGraph> matrixGraph = convertGraph(listGraph, true);

    // Display the converted graph
    std::cout << "Converted Adjacency Matrix Graph:" << std::endl;
    std::cout << matrixGraph->toString() << std::endl;

    // Convert back to adjacency list
    std::cout << "Converting back to Adjacency List representation..." << std::endl;
    std::unique_ptr<IGraph> convertedListGraph = convertGraph(*matrixGraph, false);

    // Display the final graph
    std::cout << "Final Adjacency List Graph:" << std::endl;
    std::cout << convertedListGraph->toString() << std::endl;

    // Verify the graphs are equivalent
    std::cout << "\nVerifying graph equivalence..." << std::endl;
    bool equivalent = true;

    // Check vertices
    std::vector<int> originalVertices = listGraph.getVertices();
    std::vector<int> finalVertices = convertedListGraph->getVertices();

    std::sort(originalVertices.begin(), originalVertices.end());
    std::sort(finalVertices.begin(), finalVertices.end());

    if (originalVertices != finalVertices)
    {
        equivalent = false;
        std::cout << "Vertex sets differ!" << std::endl;
    }

    // Check edges
    if (equivalent)
    {
        for (int from : originalVertices)
        {
            for (int to : originalVertices)
            {
                double originalWeight = listGraph.getEdgeWeight(from, to);
                double finalWeight = convertedListGraph->getEdgeWeight(from, to);

                bool originalHasEdge = originalWeight != std::numeric_limits<double>::infinity();
                bool finalHasEdge = finalWeight != std::numeric_limits<double>::infinity();

                if (originalHasEdge != finalHasEdge ||
                    (originalHasEdge && finalHasEdge && std::abs(originalWeight - finalWeight) > 1e-10))
                {
                    equivalent = false;
                    std::cout << "Edge (" << from << ", " << to << ") differs!" << std::endl;
                    break;
                }
            }
            if (!equivalent)
                break;
        }
    }

    std::cout << "Graphs are " << (equivalent ? "equivalent" : "different") << "!" << std::endl;
}

/**
 * @brief Demonstrate social network application
 */
void demoSocialNetwork()
{
    std::cout << "===== Social Network Demo =====" << std::endl;

    // Create an undirected social network (friendship model)
    std::cout << "Creating a friendship network (undirected)..." << std::endl;
    SocialNetwork friendNetwork(false, false); // Undirected, using adjacency list

    // Add users
    std::cout << "Adding users Alice, Bob, Charlie, Dave, and Eva..." << std::endl;
    friendNetwork.addUser("Alice");
    friendNetwork.addUser("Bob");
    friendNetwork.addUser("Charlie");
    friendNetwork.addUser("Dave");
    friendNetwork.addUser("Eva");

    // Add friendships
    std::cout << "Creating friendship connections..." << std::endl;
    friendNetwork.addConnection("Alice", "Bob");
    friendNetwork.addConnection("Alice", "Charlie");
    friendNetwork.addConnection("Bob", "Charlie");
    friendNetwork.addConnection("Bob", "Dave");
    friendNetwork.addConnection("Charlie", "Eva");
    friendNetwork.addConnection("Dave", "Eva");

    // Display the network
    std::cout << friendNetwork.toString() << std::endl;

    // Find mutual friends
    std::cout << "Mutual friends between Bob and Eva: ";
    std::vector<std::string> mutualFriends = friendNetwork.getMutualConnections("Bob", "Dave");
    if (mutualFriends.empty())
    {
        std::cout << "None" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < mutualFriends.size(); ++i)
        {
            std::cout << mutualFriends[i];
            if (i < mutualFriends.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    // Friend suggestions
    std::cout << "Friend suggestions for Alice: ";
    std::vector<std::string> suggestions = friendNetwork.suggestConnections("Alice");
    if (suggestions.empty())
    {
        std::cout << "None" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < suggestions.size(); ++i)
        {
            std::cout << suggestions[i];
            if (i < suggestions.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    // Find a path between users
    std::cout << "Path from Alice to Eva: ";
    std::vector<std::string> path = friendNetwork.findPath("Alice", "Eva");
    if (path.empty())
    {
        std::cout << "No path found" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < path.size(); ++i)
        {
            std::cout << path[i];
            if (i < path.size() - 1)
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    // Create a directed social network (follow model)
    std::cout << "\nCreating a follow network (directed)..." << std::endl;
    SocialNetwork followNetwork(true, false); // Directed, using adjacency list

    // Add users
    followNetwork.addUser("User1");
    followNetwork.addUser("User2");
    followNetwork.addUser("User3");
    followNetwork.addUser("User4");
    followNetwork.addUser("User5");

    // Add follows
    followNetwork.addConnection("User1", "User2");
    followNetwork.addConnection("User1", "User3");
    followNetwork.addConnection("User2", "User1"); // User2 follows User1 back
    followNetwork.addConnection("User3", "User4");
    followNetwork.addConnection("User4", "User5");
    followNetwork.addConnection("User5", "User2");

    // Display the network
    std::cout << followNetwork.toString() << std::endl;

    // Calculate average degrees of separation
    std::cout << "Average degrees of separation in the friendship network: "
              << friendNetwork.calculateAverageDegreesOfSeparation() << std::endl;
}

/**
 * @brief Demonstrate transportation network application
 */
void demoTransportationNetwork()
{
    std::cout << "===== Transportation Network Demo =====" << std::endl;

    // Create a transportation network
    std::cout << "Creating a city transportation network..." << std::endl;
    TransportationNetwork cityNetwork(false, false); // Undirected, using adjacency list

    // Add locations
    std::cout << "Adding locations..." << std::endl;
    cityNetwork.addLocation("Downtown");
    cityNetwork.addLocation("Airport");
    cityNetwork.addLocation("University");
    cityNetwork.addLocation("Mall");
    cityNetwork.addLocation("Beach");
    cityNetwork.addLocation("Suburb");

    // Add routes with distances in km
    std::cout << "Adding routes with distances..." << std::endl;
    cityNetwork.addRoute("Downtown", "Airport", 15.3);
    cityNetwork.addRoute("Downtown", "University", 5.7);
    cityNetwork.addRoute("Downtown", "Mall", 8.1);
    cityNetwork.addRoute("Airport", "Mall", 20.4);
    cityNetwork.addRoute("University", "Mall", 10.2);
    cityNetwork.addRoute("University", "Beach", 12.8);
    cityNetwork.addRoute("Mall", "Beach", 18.6);
    cityNetwork.addRoute("Mall", "Suburb", 7.3);
    cityNetwork.addRoute("Beach", "Suburb", 14.5);

    // Display the network
    std::cout << cityNetwork.toString() << std::endl;

    // Find shortest path
    std::cout << "Finding shortest path from Airport to Beach..." << std::endl;
    auto [path, distance] = cityNetwork.findShortestPath("Airport", "Beach");

    std::cout << "Shortest path: ";
    if (path.empty())
    {
        std::cout << "No path found" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < path.size(); ++i)
        {
            std::cout << path[i];
            if (i < path.size() - 1)
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
        std::cout << "Total distance: " << distance << " km" << std::endl;
    }

    // Find accessible locations
    std::cout << "\nLocations accessible from Downtown: ";
    std::vector<std::string> accessibleLocations = cityNetwork.findAccessibleLocations("Downtown");

    if (accessibleLocations.empty())
    {
        std::cout << "None" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < accessibleLocations.size(); ++i)
        {
            std::cout << accessibleLocations[i];
            if (i < accessibleLocations.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    // Create a one-way transportation network
    std::cout << "\nCreating a one-way transportation network..." << std::endl;
    TransportationNetwork oneWayNetwork(true, false); // Directed, using adjacency list

    // Add locations
    oneWayNetwork.addLocation("A");
    oneWayNetwork.addLocation("B");
    oneWayNetwork.addLocation("C");
    oneWayNetwork.addLocation("D");
    oneWayNetwork.addLocation("E");

    // Add one-way routes
    oneWayNetwork.addRoute("A", "B", 5.0);
    oneWayNetwork.addRoute("B", "C", 3.0);
    oneWayNetwork.addRoute("C", "E", 8.0);
    oneWayNetwork.addRoute("A", "D", 7.0);
    oneWayNetwork.addRoute("D", "E", 6.0);
    oneWayNetwork.addRoute("E", "A", 10.0); // Creates a cycle

    // Display the network
    std::cout << oneWayNetwork.toString() << std::endl;

    // Find shortest path with one-way restrictions
    std::cout << "Finding shortest path from C to A in the one-way network..." << std::endl;
    auto [oneWayPath, oneWayDistance] = oneWayNetwork.findShortestPath("C", "A");

    std::cout << "Shortest path: ";
    if (oneWayPath.empty())
    {
        std::cout << "No path found" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < oneWayPath.size(); ++i)
        {
            std::cout << oneWayPath[i];
            if (i < oneWayPath.size() - 1)
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
        std::cout << "Total distance: " << oneWayDistance << " km" << std::endl;
    }
}

/**
 * @brief Demonstrate graph generation functions
 */
void demoGraphGeneration()
{
    std::cout << "===== Graph Generation Demo =====" << std::endl;

    // Generate a random graph
    std::cout << "Generating a random graph with 6 vertices and 10 edges..." << std::endl;
    auto randomGraph = createRandomGraph(6, 10, false, true, 1.0, 10.0, false);
    std::cout << randomGraph->toString() << std::endl;

    // Generate a complete graph
    std::cout << "Generating a complete graph with 5 vertices..." << std::endl;
    auto completeGraph = createCompleteGraph(5, false, true, true);
    std::cout << completeGraph->toString() << std::endl;

    // Generate a grid graph
    std::cout << "Generating a 3x3 grid graph..." << std::endl;
    auto gridGraph = createGridGraph(3, 3, false, false, false);
    std::cout << gridGraph->toString() << std::endl;

    // Create a graph from an adjacency matrix
    std::cout << "Creating a graph from an adjacency matrix..." << std::endl;
    std::vector<std::vector<double>> matrix = {
        {0.0, 7.0, 9.0, 0.0, 0.0},
        {7.0, 0.0, 0.0, 5.0, 0.0},
        {9.0, 0.0, 0.0, 2.0, 0.0},
        {0.0, 5.0, 2.0, 0.0, 6.0},
        {0.0, 0.0, 0.0, 6.0, 0.0}};

    auto matrixGraph = createGraphFromMatrix(matrix, false, true, false);
    std::cout << matrixGraph->toString() << std::endl;

    // Create a graph from an adjacency list
    std::cout << "Creating a graph from an adjacency list..." << std::endl;
    std::unordered_map<int, std::vector<std::pair<int, double>>> adjList = {
        {1, {{2, 3.5}, {3, 7.2}}},
        {2, {{1, 3.5}, {4, 1.8}}},
        {3, {{1, 7.2}, {5, 9.1}}},
        {4, {{2, 1.8}, {3, 2.7}}},
        {5, {{3, 9.1}, {4, 6.3}}}};

    auto adjListGraph = createGraphFromAdjList(adjList, false, true, false);
    std::cout << adjListGraph->toString() << std::endl;

    // Create a graph from an edge list
    std::cout << "Creating a graph from an edge list..." << std::endl;
    std::vector<std::tuple<int, int, double>> edges = {
        {1, 2, 5.0},
        {1, 3, 2.0},
        {2, 3, 1.0},
        {2, 4, 3.0},
        {3, 4, 7.0}};

    auto edgeListGraph = createGraphFromEdgeList(edges, {1, 2, 3, 4}, false, true, false);
    std::cout << edgeListGraph->toString() << std::endl;
}

/**
 * @brief Main function to demonstrate graph fundamentals
 */
int main()
{
    std::cout << "======================================================" << std::endl;
    std::cout << "=== DAY 29: GRAPH FUNDAMENTALS                     ===" << std::endl;
    std::cout << "=== Part of #DSAin45 Course                        ===" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;

    // Demo basic graph operations
    demoBasicOperations();
    std::cout << std::endl;

    // Demo graph representation conversion
    demoConversion();
    std::cout << std::endl;

    // Demo graph generation
    demoGraphGeneration();
    std::cout << std::endl;

    // Demo real-world applications
    demoSocialNetwork();
    std::cout << std::endl;

    demoTransportationNetwork();
    std::cout << std::endl;

    // Run benchmarks
    std::cout << "Running benchmarks (this may take a moment)..." << std::endl;

    // Benchmark graph representations
    benchmarkGraphRepresentations({100, 500, 1000}, 0.05, 3);

    // Benchmark memory usage
    benchmarkMemoryUsage({100, 1000, 10000}, {0.01, 0.1, 0.5});

    // Benchmark edge operations
    benchmarkEdgeOperations(1000, {0.01, 0.1, 0.5}, 3);

    std::cout << "======================================================" << std::endl;
    std::cout << "=== End of DAY 29 Demonstrations                   ===" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}