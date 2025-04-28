# 🔍 DAY 29: Graph Fundamentals - The Structure Behind Networks #DSAin45

Welcome to Day 29 and Week 5 of our 45-day DSA journey! Yesterday, we tackled tree problems with techniques like finding the Lowest Common Ancestor and examining path sums. Today, we're expanding our horizons into one of the most versatile and powerful data structures in computer science: **Graphs**.

_Imagine a world where everything is connected—social networks linking friends, transportation systems connecting cities, web pages connecting through hyperlinks, atoms connecting in molecules. This interconnected world is perfectly modeled using graphs, which capture relationships between entities in an elegant, mathematically sound structure that enables powerful algorithms to solve complex problems._

## 📌 What is a Graph?

A graph is a non-linear data structure consisting of a finite set of **vertices** (or nodes) connected by **edges**. Unlike trees, which are actually a specific type of graph, general graphs can have any pattern of connections, including cycles, disconnected components, and varying degrees of connectivity.

### Key Graph Terminology:

1. **Vertex (Node)**: An individual point or entity in the graph.
2. **Edge**: A connection between two vertices.
3. **Adjacency**: Two vertices are adjacent if they're connected by an edge.
4. **Path**: A sequence of vertices where each adjacent pair is connected by an edge.
5. **Cycle**: A path that starts and ends at the same vertex.
6. **Connected Graph**: A graph where there's a path between every pair of vertices.
7. **Degree**: The number of edges connecting to a vertex.
8. **Directed Graph (Digraph)**: A graph where edges have a direction (from → to).
9. **Undirected Graph**: A graph where edges have no direction (connections are bidirectional).
10. **Weighted Graph**: A graph where edges have associated weights or costs.
11. **Unweighted Graph**: A graph where all edges have equal importance (or implicit weight of 1).

## 🌐 Graph Representations

There are two primary ways to represent a graph in computer memory, each with its own strengths and weaknesses:

### 1. Adjacency Matrix

An adjacency matrix is a 2D array of size V×V (where V is the number of vertices), where the entry at `matrix[i][j]` indicates whether there's an edge from vertex `i` to vertex `j`.

```cpp
class AdjacencyMatrixGraph {
private:
    std::vector<std::vector<double>> matrix; // The adjacency matrix
    std::unordered_map<int, int> vertexToIndex; // Maps vertex IDs to matrix indices
    std::unordered_map<int, int> indexToVertex; // Maps matrix indices to vertex IDs
    bool directed; // Whether the graph is directed
    bool weighted; // Whether the graph is weighted
    int edgeCount; // Number of edges

public:
    // Constructor, methods, etc.
};
```

**Advantages:**

- O(1) time to check if there's an edge between any two vertices
- Simple to implement and understand
- Efficient for dense graphs
- Easy to implement algorithms that work directly with edge weights

**Disadvantages:**

- O(V²) space complexity, inefficient for sparse graphs
- Adding/removing vertices is O(V²) operation
- Inefficient for iteration over all edges from a vertex when the graph is sparse

### 2. Adjacency List

An adjacency list stores a list of adjacent vertices for each vertex in the graph. This can be implemented using an array of linked lists, a hash map of arrays, or a similar structure.

```cpp
class AdjacencyListGraph {
private:
    struct Edge {
        int to;      // Destination vertex
        double weight; // Edge weight

        Edge(int t, double w) : to(t), weight(w) {}
    };

    std::unordered_map<int, std::vector<Edge>> outEdges;  // Outgoing edges for each vertex
    std::unordered_map<int, std::vector<Edge>> inEdges;   // Incoming edges for each vertex (for directed graphs)
    bool directed; // Whether the graph is directed
    bool weighted; // Whether the graph is weighted
    int edgeCount; // Number of edges

public:
    // Constructor, methods, etc.
};
```

**Advantages:**

- Space-efficient for sparse graphs: O(V+E) space complexity
- Adding vertices is O(1)
- Efficient iteration over all edges from a vertex
- Better for most graph algorithms that explore neighboring vertices

**Disadvantages:**

- Checking if there's an edge between two specific vertices takes O(degree) time
- Not as space-efficient for dense graphs
- More complex to implement

### Representation Comparison:

| Operation                | Adjacency Matrix | Adjacency List |
| ------------------------ | ---------------- | -------------- |
| Add Vertex               | O(V²)            | O(1)           |
| Remove Vertex            | O(V²)            | O(V+E)         |
| Add Edge                 | O(1)             | O(1)           |
| Remove Edge              | O(1)             | O(E)           |
| Query: Is there an edge? | O(1)             | O(degree(v))   |
| Find all neighbors       | O(V)             | O(degree(v))   |
| Memory Usage             | O(V²)            | O(V+E)         |

Where:

- V is the number of vertices
- E is the number of edges
- degree(v) is the number of edges connected to vertex v

## 🔄 Basic Graph Operations

Let's implement some fundamental graph operations:

### 1. Adding a Vertex

```cpp
bool addVertex(int v) {
    if (hasVertex(v)) {
        return false;  // Vertex already exists
    }

    outEdges[v] = std::vector<Edge>();
    if (directed) {
        inEdges[v] = std::vector<Edge>();
    }

    return true;
}
```

### 2. Adding an Edge

```cpp
bool addEdge(int from, int to, double weight = 1.0) {
    if (!hasVertex(from) || !hasVertex(to)) {
        return false;
    }

    if (!weighted && weight != 1.0) {
        weight = 1.0;  // Ignore weights for unweighted graphs
    }

    // Check if edge already exists
    for (const auto& edge : outEdges[from]) {
        if (edge.to == to) {
            return false;  // Edge already exists
        }
    }

    // Add edge
    outEdges[from].push_back(Edge(to, weight));

    if (directed) {
        inEdges[to].push_back(Edge(from, weight));
    } else if (from != to) {
        // For undirected graph, add the reverse edge (unless it's a self-loop)
        outEdges[to].push_back(Edge(from, weight));
    }

    edgeCount++;
    return true;
}
```

### 3. Getting Neighbors of a Vertex

```cpp
std::vector<int> getNeighbors(int v) const {
    if (!hasVertex(v)) {
        return {};
    }

    std::vector<int> neighbors;
    neighbors.reserve(outEdges.at(v).size());

    for (const auto& edge : outEdges.at(v)) {
        neighbors.push_back(edge.to);
    }

    return neighbors;
}
```

### 4. Checking if an Edge Exists

```cpp
bool hasEdge(int from, int to) const {
    if (!hasVertex(from) || !hasVertex(to)) {
        return false;
    }

    const auto& edges = outEdges.at(from);
    for (const auto& edge : edges) {
        if (edge.to == to) {
            return true;
        }
    }

    return false;
}
```

## 🚀 Real-World Applications of Graphs

Graphs are everywhere in computing and the real world. Here are some compelling examples:

### 1. Social Networks

Each user is a vertex, and connections (friendships, follows) are edges. Graph algorithms help with:

- Friend suggestions
- Finding shortest paths between users (degrees of separation)
- Community detection
- Influence analysis

```cpp
// Example: Finding a path between two users in a network
std::vector<std::string> findPath(const std::string& fromUser, const std::string& toUser) {
    // BFS implementation to find the shortest path
    // ...

    // Reconstruction of the path
    std::vector<std::string> path;
    int current = toId;

    while (current != fromId) {
        path.push_back(userNameMap[current]);
        current = previous[current];
    }

    path.push_back(fromUser);
    std::reverse(path.begin(), path.end());

    return path;
}
```

### 2. Transportation Networks

Cities/locations are vertices, and routes/roads are edges. Graph algorithms help with:

- Finding shortest/fastest routes
- Traffic optimization
- Network design
- Ridesharing optimization

### 3. Web Page Analysis

Web pages are vertices, and hyperlinks are directed edges. Graph algorithms help with:

- Search ranking (e.g., Google's PageRank)
- Web crawling
- Identifying important pages
- Finding connected components of the web

### 4. Computer Networking

Network devices are vertices, and connections are edges. Graph algorithms help with:

- Routing protocols
- Network topology analysis
- Fault tolerance assessment
- Bandwidth optimization

### 5. Biological Networks

Proteins/genes/organisms are vertices, and interactions are edges. Graph algorithms help with:

- Protein interaction networks
- Metabolic pathways
- Evolutionary relationships
- Disease propagation models

## 📊 Time and Space Complexity Analysis

The choice between adjacency matrix and adjacency list representation significantly impacts performance:

### Sparse vs. Dense Graphs

- **Sparse Graph**: Few edges compared to the maximum possible (E << V²)
- **Dense Graph**: Many edges, approaching the maximum possible (E ≈ V²)

For a graph with V vertices, the maximum possible number of edges is:

- V(V-1)/2 for an undirected graph (without self-loops)
- V² for a directed graph (allowing self-loops)

### Memory Usage

The memory footprint of our graph representations:

1. **Adjacency Matrix**: O(V²) regardless of the number of edges

   - For a graph with 10,000 vertices: ~800 MB (assuming 8-byte doubles)

2. **Adjacency List**: O(V + E)
   - For a sparse graph with 10,000 vertices and 50,000 edges: ~1.2 MB
   - For a dense graph with 10,000 vertices and 50 million edges: ~600 MB

### Performance Characteristics

Performance benchmarks show:

1. **Edge Operations**:

   - For dense graphs, adjacency matrices outperform adjacency lists for edge queries
   - For sparse graphs, adjacency lists are faster for most operations

2. **Vertex Operations**:

   - Adjacency lists always perform better for vertex addition/removal

3. **Graph Traversal**:
   - Adjacency lists are much faster for graph traversal algorithms like BFS and DFS
   - Adjacency matrices require O(V²) time to traverse, while adjacency lists require O(V+E)

## 😄 Graph Jokes and Analogies

1. **The Social Network Analogy**:
   "A graph is like a social network where vertices are people and edges are relationships. Some relationships are directed (like Instagram follows), some are undirected (like Facebook friendships), and some are weighted (like how much you actually like each 'friend')."

2. **The Adjacency Matrix vs. List Joke**:
   "An adjacency matrix walked into a bar with 1,000,000 vertices. The bartender asked, 'Why do you need all that space?' The adjacency matrix replied, 'I have memory issues.' An adjacency list watching from the corner smirked and said, 'I only bring the connections that matter.'"

3. **The Edge Case Pun**:
   "What do you call a test for graph algorithms? An edge case! And if it's particularly tricky, it might leave you in a complex cycle of debugging."

4. **The Transportation Network Analogy**:
   "A weighted graph is like a GPS navigation system. Each road (edge) has a distance or time (weight), and Dijkstra's algorithm helps find the shortest path home—unless there's a traffic jam, in which case the weights dynamically change!"

5. **The Directed Graph Joke**:
   "In a directed graph, relationships can be one-sided, just like when I text 'Hey, let's hang out!' and get left on read for three days."

## 🧩 LeetCode Practice Problems

### Easy:

#### 1. Find the Town Judge (LeetCode #997)

**Problem:** In a town, there are n people labeled from 1 to n. There is a rumor that one of these people is secretly the town judge. The town judge trusts nobody and everybody (except for the town judge) trusts the town judge. You're given trust, an array of pairs `trust[i] = [a, b]` representing that person `a` trusts person `b`. If the town judge exists and can be identified, return their label, otherwise return -1.

**Solution:**

```cpp
int findJudge(int n, std::vector<std::vector<int>>& trust) {
    // Create arrays to track in-degree and out-degree for each person
    std::vector<int> trustCount(n + 1, 0);  // People who trust this person
    std::vector<int> trusting(n + 1, 0);    // People this person trusts

    for (const auto& relation : trust) {
        int a = relation[0];  // a trusts b
        int b = relation[1];  // b is trusted by a

        trusting[a]++;        // a trusts someone
        trustCount[b]++;      // b is trusted by someone
    }

    // The judge should be trusted by everyone except themselves (n-1 people)
    // and should trust nobody (0 people)
    for (int i = 1; i <= n; i++) {
        if (trustCount[i] == n - 1 && trusting[i] == 0) {
            return i;
        }
    }

    return -1;  // No judge found
}
```

#### 2. Find Center of Star Graph (LeetCode #1791)

**Problem:** There is an undirected star graph consisting of n nodes labeled from 1 to n. A star graph is a graph where there is one center node and exactly n - 1 edges that connect the center node with every other node. You are given a 2D array edges where each `edges[i] = [ui, vi]` indicates that there is an edge between the nodes ui and vi. Return the center of the given star graph.

**Solution:**

```cpp
int findCenter(std::vector<std::vector<int>>& edges) {
    // In a star graph, the center node appears in every edge
    // We just need to check the first two edges to find the common node

    int node1 = edges[0][0];
    int node2 = edges[0][1];
    int node3 = edges[1][0];
    int node4 = edges[1][1];

    if (node1 == node3 || node1 == node4) {
        return node1;
    } else {
        return node2;
    }
}
```

### Medium:

#### 1. Course Schedule (LeetCode #207)

**Problem:** There are a total of numCourses courses you have to take, labeled from 0 to numCourses-1. Some courses have prerequisites, for example, to take course 0 you have to first take course 1, which is expressed as a pair: [0,1]. Given the total number of courses and a list of prerequisite pairs, is it possible for you to finish all courses?

**Solution:**

```cpp
bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites) {
    // Create adjacency list
    std::vector<std::vector<int>> graph(numCourses);

    for (const auto& prereq : prerequisites) {
        int course = prereq[0];
        int prerequisite = prereq[1];
        graph[prerequisite].push_back(course);  // prerequisite -> course
    }

    // 0 = not visited, 1 = visiting, 2 = visited
    std::vector<int> visited(numCourses, 0);

    // DFS to detect cycles
    for (int i = 0; i < numCourses; i++) {
        if (visited[i] == 0) {
            if (hasCycle(graph, visited, i)) {
                return false;  // Cycle detected, cannot finish all courses
            }
        }
    }

    return true;  // No cycles, can finish all courses
}

bool hasCycle(const std::vector<std::vector<int>>& graph, std::vector<int>& visited, int node) {
    visited[node] = 1;  // Mark as visiting

    for (int neighbor : graph[node]) {
        if (visited[neighbor] == 1) {
            return true;  // Found a cycle
        }
        if (visited[neighbor] == 0 && hasCycle(graph, visited, neighbor)) {
            return true;  // Cycle detected in recursive call
        }
    }

    visited[node] = 2;  // Mark as visited
    return false;
}
```

#### 2. Clone Graph (LeetCode #133)

**Problem:** Given a reference of a node in a connected undirected graph, return a deep copy (clone) of the graph.

**Solution:**

```cpp
class Node {
public:
    int val;
    std::vector<Node*> neighbors;

    Node(int _val = 0) {
        val = _val;
        neighbors = std::vector<Node*>();
    }
};

Node* cloneGraph(Node* node) {
    if (!node) {
        return nullptr;
    }

    // Map original nodes to their clones
    std::unordered_map<Node*, Node*> visited;

    // Use DFS to clone the graph
    return cloneDFS(node, visited);
}

Node* cloneDFS(Node* node, std::unordered_map<Node*, Node*>& visited) {
    // If we've already processed this node, return its clone
    if (visited.find(node) != visited.end()) {
        return visited[node];
    }

    // Create a clone of the current node
    Node* clone = new Node(node->val);
    visited[node] = clone;

    // Clone all neighbors recursively
    for (Node* neighbor : node->neighbors) {
        clone->neighbors.push_back(cloneDFS(neighbor, visited));
    }

    return clone;
}
```

### Hard:

#### 1. Network Delay Time (LeetCode #743)

**Problem:** You are given a network of n nodes, labeled from 1 to n. You are also given times, a list of travel times as directed edges `times[i] = (ui, vi, wi)`, where ui is the source node, vi is the target node, and wi is the time it takes for a signal to travel from source to target. We will send a signal from a given node k. Return the minimum time it takes for all the n nodes to receive the signal. If it is impossible for all the n nodes to receive the signal, return -1.

**Solution:**

```cpp
int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k) {
    // Create adjacency list
    std::vector<std::vector<std::pair<int, int>>> graph(n + 1);

    for (const auto& time : times) {
        int source = time[0];
        int target = time[1];
        int weight = time[2];
        graph[source].push_back({target, weight});
    }

    // Distance array
    std::vector<int> dist(n + 1, INT_MAX);
    dist[k] = 0;  // Starting node

    // Priority queue for Dijkstra's algorithm
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;
    pq.push({0, k});  // {distance, node}

    while (!pq.empty()) {
        auto [d, node] = pq.top();
        pq.pop();

        if (d > dist[node]) {
            continue;  // Skip if we've found a better path
        }

        for (auto [neighbor, weight] : graph[node]) {
            if (dist[node] + weight < dist[neighbor]) {
                dist[neighbor] = dist[node] + weight;
                pq.push({dist[neighbor], neighbor});
            }
        }
    }

    // Find the maximum distance (time for the signal to reach the furthest node)
    int maxDist = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) {
            return -1;  // Not all nodes can be reached
        }
        maxDist = std::max(maxDist, dist[i]);
    }

    return maxDist;
}
```

#### 2. Critical Connections in a Network (LeetCode #1192)

**Problem:** There are n servers numbered from 0 to n-1 connected by undirected server-to-server connections forming a network where connections[i] = [a, b] represents a connection between servers a and b. Any server can reach any other server directly or indirectly through the network. A critical connection is a connection that, if removed, will make some server unable to reach some other server. Return all critical connections in the network in any order.

**Solution:**

```cpp
std::vector<std::vector<int>> criticalConnections(int n, std::vector<std::vector<int>>& connections) {
    // Build adjacency list
    std::vector<std::vector<int>> graph(n);

    for (const auto& connection : connections) {
        int a = connection[0];
        int b = connection[1];
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // Arrays for Tarjan's algorithm
    std::vector<int> disc(n, -1);  // Discovery time
    std::vector<int> low(n, -1);   // Earliest visited vertex
    std::vector<std::vector<int>> bridges;
    int time = 0;

    // DFS to find bridges
    for (int i = 0; i < n; i++) {
        if (disc[i] == -1) {
            dfs(graph, i, -1, disc, low, bridges, time);
        }
    }

    return bridges;
}

void dfs(const std::vector<std::vector<int>>& graph, int node, int parent,
         std::vector<int>& disc, std::vector<int>& low,
         std::vector<std::vector<int>>& bridges, int& time) {

    disc[node] = low[node] = ++time;

    for (int neighbor : graph[node]) {
        if (neighbor == parent) {
            continue;  // Skip parent
        }

        if (disc[neighbor] == -1) {
            // If neighbor is not visited
            dfs(graph, neighbor, node, disc, low, bridges, time);

            // Update low value
            low[node] = std::min(low[node], low[neighbor]);

            // Check if bridge
            if (low[neighbor] > disc[node]) {
                bridges.push_back({node, neighbor});
            }
        } else {
            // If neighbor is already visited, update low value
            low[node] = std::min(low[node], disc[neighbor]);
        }
    }
}
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll explore Graph Traversal algorithms, which are fundamental techniques for visiting all vertices in a graph. We'll cover:

- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Applications of these traversal techniques
- Implementation strategies
- Time and space complexity analysis
- Real-world use cases

_If graph fundamentals are the blueprint, traversal algorithms are the navigational tools that allow us to explore the structure efficiently. Whether you're finding the shortest path between two points, detecting cycles, or identifying connected components, mastering graph traversal is essential for solving a wide range of problems across domains. Join us tomorrow as we navigate the intricate pathways of graph structures!_

#DSAin45 #Day29 #GraphTheory #DataStructures #Algorithms #CPlusPlus #NetworkAnalysis #GraphRepresentation #CodingInterview #ComputerScience
