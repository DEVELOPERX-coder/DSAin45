# 🛣️ DAY 31: Shortest Path Algorithms - Finding the Most Efficient Routes #DSAin45

Welcome to Day 31 of our 45-day DSA journey! Yesterday, we explored graph traversal algorithms that help us visit every vertex in a graph. Today, we'll focus on **Shortest Path Algorithms** — specialized techniques for finding the most efficient routes between vertices in a graph.

_Imagine you're planning a road trip across the country. You could take countless different routes to reach your destination, but you probably want the one that minimizes either distance, time, or cost. This is precisely the problem that shortest path algorithms solve, not just for road networks, but for any system that can be modeled as a weighted graph._

## 📌 Shortest Path Fundamentals

A shortest path between two vertices in a weighted graph is a path that minimizes the sum of the weights of its constituent edges. Depending on what the weights represent (distance, time, cost, etc.), the "shortest" path gives us the most efficient route according to that metric.

Key concepts in shortest path algorithms:

1. **Source vertex**: The starting point of the path
2. **Destination vertex**: The endpoint of the path
3. **Edge weights**: Values assigned to edges representing distance, time, cost, etc.
4. **Path weight**: The sum of the weights of all edges in a path
5. **Optimal substructure**: The property that a shortest path between two vertices contains other shortest paths within it

Different shortest path algorithms solve different variations of the problem:

- **Single-source shortest paths**: Find shortest paths from a single source vertex to all other vertices
- **Single-destination shortest paths**: Find shortest paths from all vertices to a single destination
- **Single-pair shortest path**: Find the shortest path between a specific pair of vertices
- **All-pairs shortest paths**: Find shortest paths between every pair of vertices

## 🧭 Dijkstra's Algorithm

Dijkstra's algorithm solves the single-source shortest path problem for a graph with non-negative edge weights. It's a greedy algorithm that builds up the solution incrementally, always choosing the vertex with the smallest distance from the source.

### Key Characteristics of Dijkstra's Algorithm:

- Guarantees the shortest path in graphs with non-negative edge weights
- Does not work correctly with negative edge weights
- Uses a priority queue to efficiently select the next vertex
- Time complexity: O(V log V + E log V) with a binary heap priority queue
- Space complexity: O(V + E)

### Dijkstra's Algorithm Implementation:

```cpp
std::vector<int> dijkstra(const IGraph& graph, int source,
                          std::vector<double>& distances) {
    int V = graph.getVertexCount();

    // Initialize distances and previous vertices
    distances.assign(V, std::numeric_limits<double>::infinity());
    distances[source] = 0;

    std::vector<int> previous(V, -1); // Store previous vertex in shortest path

    // Min priority queue: pair of (distance, vertex)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>> pq;

    // Add source to priority queue
    pq.push({0.0, source});

    // Set to keep track of processed vertices
    std::unordered_set<int> processed;

    while (!pq.empty()) {
        int u = pq.top().second; // Get vertex with minimum distance
        pq.pop();

        // Skip if already processed
        if (processed.find(u) != processed.end()) {
            continue;
        }

        // Mark as processed
        processed.insert(u);

        // Process all neighbors of u
        for (int v : graph.getNeighbors(u)) {
            double weight = graph.getEdgeWeight(u, v);

            // Relaxation step: If a shorter path to v is found
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;

                // Add to priority queue
                pq.push({distances[v], v});
            }
        }
    }

    return previous;
}

// Function to reconstruct a path from source to destination
std::vector<int> reconstructPath(const std::vector<int>& previous, int source, int destination) {
    std::vector<int> path;

    // Check if destination is unreachable
    if (previous[destination] == -1 && source != destination) {
        return path; // Return empty path
    }

    // Reconstruct path by following previous pointers
    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }

    // Reverse path to get it from source to destination
    std::reverse(path.begin(), path.end());

    return path;
}
```

### Dijkstra's Algorithm With An Example:

Let's trace through the algorithm on this weighted graph:

```
      2       3
  A ----- B ----- C
  |       |       |
 4|       |1      |5
  |       |       |
  D ----- E ----- F
      7       1
```

Starting from vertex A:

1. Initialize distances: A=0, B=∞, C=∞, D=∞, E=∞, F=∞
2. Add A to priority queue
3. Process A:
   - Update distances: B=2, D=4
   - Previous pointers: B←A, D←A
   - Priority queue: {(2,B), (4,D)}
4. Process B (smallest distance in queue):
   - Update distances: C=5, E=3
   - Previous pointers: C←B, E←B
   - Priority queue: {(3,E), (4,D), (5,C)}
5. Process E:
   - Update distances: F=4
   - Previous pointers: F←E
   - Priority queue: {(4,D), (4,F), (5,C)}
6. Process D (or F, same distance):
   - Check if E can be reached cheaper through D: 4+7=11 > 3, so no update
   - Priority queue: {(4,F), (5,C)}
7. Process F:
   - No updates
   - Priority queue: {(5,C)}
8. Process C:
   - No updates
   - Priority queue is empty, algorithm terminates

Final distances from A: {A=0, B=2, C=5, D=4, E=3, F=4}  
Shortest path from A to F: A → B → E → F with weight 4

### Optimizations for Dijkstra's Algorithm:

1. **Early Termination**: If we're only interested in the shortest path to a specific destination, we can stop once we've processed that vertex.

```cpp
std::vector<int> dijkstraSingleTarget(const IGraph& graph, int source, int target,
                                     double& shortestDistance) {
    std::vector<double> distances;
    std::vector<int> previous = dijkstra(graph, source, distances);

    // Store the shortest distance
    shortestDistance = distances[target];

    // Reconstruct and return the path
    return reconstructPath(previous, source, target);
}
```

2. **Bidirectional Dijkstra**: Run two simultaneous searches, one from the source and one from the destination, and stop when they meet in the middle.

3. **A\* Search**: An extension of Dijkstra's algorithm that uses a heuristic to guide the search toward the destination, often resulting in much faster path finding.

## 🚧 Bellman-Ford Algorithm

Unlike Dijkstra's algorithm, the Bellman-Ford algorithm can handle graphs with negative edge weights. It is also capable of detecting negative cycles (cycles whose total weight is negative).

### Key Characteristics of Bellman-Ford:

- Works with graphs containing negative edge weights
- Can detect negative cycles
- Slower than Dijkstra's algorithm (but more versatile)
- Time complexity: O(V·E)
- Space complexity: O(V)

### Bellman-Ford Algorithm Implementation:

```cpp
bool bellmanFord(const IGraph& graph, int source,
                std::vector<double>& distances,
                std::vector<int>& previous) {
    int V = graph.getVertexCount();

    // Initialize distances
    distances.assign(V, std::numeric_limits<double>::infinity());
    distances[source] = 0;

    // Initialize previous vertices
    previous.assign(V, -1);

    // Get all edges from the graph
    std::vector<std::tuple<int, int, double>> edges;
    std::vector<int> vertices = graph.getVertices();

    for (int u : vertices) {
        for (int v : graph.getNeighbors(u)) {
            double weight = graph.getEdgeWeight(u, v);
            edges.push_back({u, v, weight});
        }
    }

    // Relax all edges V-1 times
    for (int i = 0; i < V - 1; i++) {
        bool anyChange = false;

        for (const auto& [u, v, weight] : edges) {
            // If u is not reachable, skip
            if (distances[u] == std::numeric_limits<double>::infinity()) {
                continue;
            }

            // Relaxation step
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                anyChange = true;
            }
        }

        // Early termination if no changes were made in this iteration
        if (!anyChange) {
            break;
        }
    }

    // Check for negative cycles
    for (const auto& [u, v, weight] : edges) {
        if (distances[u] != std::numeric_limits<double>::infinity() &&
            distances[u] + weight < distances[v]) {
            // Negative cycle detected
            return false;
        }
    }

    return true; // No negative cycles
}
```

### Bellman-Ford with an Example:

Consider this graph with a negative edge:

```
      2       -5
  A ----- B ----- C
  |       |       |
 4|       |1      |2
  |       |       |
  D ----- E ----- F
      -3      1
```

Starting from vertex A:

1. Initialize distances: A=0, B=∞, C=∞, D=∞, E=∞, F=∞
2. Iteration 1:
   - Relax A→B: B=2
   - Relax A→D: D=4
   - Relax B→C: C=-3
   - Relax B→E: E=3
   - Relax C→F: F=-1
   - Relax D→E: E=1 (improved from 3)
   - Relax E→F: F=2 (not an improvement)
3. Iteration 2:
   - Relax D→E: E=1 (no change)
   - Relax E→F: F=2 (not an improvement)
4. No changes in iteration 2, so we can stop early
5. Check for negative cycles: none found

Final distances from A: {A=0, B=2, C=-3, D=4, E=1, F=-1}

### Detecting and Handling Negative Cycles:

Negative cycles are problematic because they allow for paths of arbitrarily low weight. If a shortest path includes a negative cycle, you could traverse the cycle repeatedly to achieve an even "shorter" path.

The Bellman-Ford algorithm can detect negative cycles by performing one additional relaxation pass after the V-1 iterations. If any distance can still be improved, then a negative cycle exists.

```cpp
// Function to detect and find a negative cycle
std::vector<int> findNegativeCycle(const IGraph& graph,
                                  const std::vector<double>& distances,
                                  const std::vector<int>& previous) {
    int V = graph.getVertexCount();
    std::vector<int> vertices = graph.getVertices();

    // Find an edge that can still be relaxed after V-1 iterations
    for (int u : vertices) {
        for (int v : graph.getNeighbors(u)) {
            double weight = graph.getEdgeWeight(u, v);

            if (distances[u] != std::numeric_limits<double>::infinity() &&
                distances[u] + weight < distances[v]) {
                // Found an edge that can be relaxed
                // Trace back V vertices to find a vertex in the cycle
                std::vector<int> cycle;
                std::unordered_set<int> visited;

                // Start from any vertex in the relaxable edge
                int x = u;

                // Find a vertex that is part of the cycle
                for (int i = 0; i < V; i++) {
                    x = previous[x];

                    // If x is invalid or we've gone too far back
                    if (x == -1) {
                        return {}; // Cannot find a cycle
                    }
                }

                // Now x is part of the cycle, reconstruct the cycle
                int y = x;
                do {
                    cycle.push_back(y);
                    y = previous[y];
                } while (y != x);

                cycle.push_back(x); // Complete the cycle
                std::reverse(cycle.begin(), cycle.end());

                return cycle;
            }
        }
    }

    return {}; // No negative cycle found
}
```

## 🌐 Floyd-Warshall Algorithm

While Dijkstra's and Bellman-Ford solve the single-source shortest path problem, the Floyd-Warshall algorithm finds the shortest paths between all pairs of vertices in a graph.

### Key Characteristics of Floyd-Warshall:

- Solves the all-pairs shortest paths problem
- Works with positive and negative edge weights (but no negative cycles)
- Uses dynamic programming
- Simple implementation
- Time complexity: O(V³)
- Space complexity: O(V²)

### Floyd-Warshall Algorithm Implementation:

```cpp
void floydWarshall(const IGraph& graph,
                  std::vector<std::vector<double>>& distances,
                  std::vector<std::vector<int>>& next) {
    int V = graph.getVertexCount();
    std::vector<int> vertices = graph.getVertices();

    // Initialize distance matrix
    distances = std::vector<std::vector<double>>(
        V, std::vector<double>(V, std::numeric_limits<double>::infinity()));

    // Initialize next vertex matrix
    next = std::vector<std::vector<int>>(
        V, std::vector<int>(V, -1));

    // Initialize with direct edge weights
    for (int i = 0; i < V; i++) {
        distances[i][i] = 0; // Distance to self is 0

        for (int neighbor : graph.getNeighbors(vertices[i])) {
            int j = neighbor;
            double weight = graph.getEdgeWeight(vertices[i], j);
            distances[i][j] = weight;
            next[i][j] = j;
        }
    }

    // Main Floyd-Warshall algorithm
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (distances[i][k] != std::numeric_limits<double>::infinity() &&
                    distances[k][j] != std::numeric_limits<double>::infinity() &&
                    distances[i][k] + distances[k][j] < distances[i][j]) {

                    distances[i][j] = distances[i][k] + distances[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

// Function to reconstruct a path from the next vertex matrix
std::vector<int> reconstructPathFloydWarshall(
    const std::vector<std::vector<int>>& next, int source, int destination) {

    if (next[source][destination] == -1) {
        return {}; // No path exists
    }

    std::vector<int> path = {source};
    int at = source;

    while (at != destination) {
        at = next[at][destination];
        path.push_back(at);
    }

    return path;
}
```

### Floyd-Warshall With An Example:

Consider this simple graph:

```
      2       3
  A ----- B ----- C
  |               |
 6|               |1
  |               |
  D ------------- E
          4
```

Let's trace through the algorithm:

1. Initialize the distance matrix:

   ```
      A    B    C    D    E
   A  0    2    ∞    6    ∞
   B  2    0    3    ∞    ∞
   C  ∞    3    0    ∞    1
   D  6    ∞    ∞    0    4
   E  ∞    ∞    1    4    0
   ```

2. Consider each vertex as an intermediate:

   - With A as intermediate: No improvements
   - With B as intermediate:
     - Path A→C improves: A→B→C = 5
     - Path B→D improves: B→A→D = 8
   - With C as intermediate:
     - Path A→E improves: A→B→C→E = 6
     - Path B→E improves: B→C→E = 4
   - With D as intermediate:
     - Path A→E improves: A→D→E = 10 (not better than 6)
   - With E as intermediate:
     - Path A→C improves: A→D→E→C = 11 (not better than 5)
     - Path B→D improves: B→C→E→D = 8 (not better)
     - Path C→D improves: C→E→D = 5
     - Path D→C improves: D→E→C = 5

3. Final distance matrix:
   ```
      A    B    C    D    E
   A  0    2    5    6    6
   B  2    0    3    8    4
   C  5    3    0    5    1
   D  6    8    5    0    4
   E  5    7    1    4    0
   ```

### Applications of Floyd-Warshall:

1. **Transitive Closure**: Determine whether a path exists between every pair of vertices
2. **All-Pairs Shortest Paths**: Find the shortest path between any two vertices
3. **Network Routing**: Compute routing tables where paths between all nodes are needed
4. **Finding Maximum Flow Paths**: Used as a subroutine in some maximum flow algorithms
5. **Graph Analysis**: Detect cycles, compute betweenness centrality, etc.

## 🚀 Johnson's Algorithm

Johnson's algorithm is another approach to the all-pairs shortest paths problem. It's particularly efficient for sparse graphs where |E| is much less than |V²|.

### Key Characteristics of Johnson's Algorithm:

- Combines elements of Dijkstra's and Bellman-Ford algorithms
- More efficient than Floyd-Warshall for sparse graphs
- Handles negative edge weights
- Time complexity: O(V² log V + VE)
- Space complexity: O(V²)

### Johnson's Algorithm Implementation:

```cpp
bool johnsonsAlgorithm(const IGraph& graph,
                       std::vector<std::vector<double>>& distances) {
    int V = graph.getVertexCount();
    std::vector<int> vertices = graph.getVertices();

    // Create a copy of the graph with an additional vertex
    // connected to all other vertices with zero-weight edges
    auto augmentedGraph = std::make_unique<AdjacencyListGraph>(true, true);

    // Add the original vertices
    for (int v : vertices) {
        augmentedGraph->addVertex(v);
    }

    // Add a new source vertex
    int q = V; // New vertex ID
    augmentedGraph->addVertex(q);

    // Add edges from the original graph
    for (int u : vertices) {
        for (int v : graph.getNeighbors(u)) {
            double weight = graph.getEdgeWeight(u, v);
            augmentedGraph->addEdge(u, v, weight);
        }
    }

    // Add zero-weight edges from the new vertex to all others
    for (int v : vertices) {
        augmentedGraph->addEdge(q, v, 0);
    }

    // Run Bellman-Ford from the new source vertex to compute h values
    std::vector<double> h;
    std::vector<int> previous;
    if (!bellmanFord(*augmentedGraph, q, h, previous)) {
        return false; // Negative cycle detected
    }

    // Reweight the original graph
    auto reweightedGraph = std::make_unique<AdjacencyListGraph>(true, true);

    // Add vertices
    for (int v : vertices) {
        reweightedGraph->addVertex(v);
    }

    // Add reweighted edges
    for (int u : vertices) {
        for (int v : graph.getNeighbors(u)) {
            double originalWeight = graph.getEdgeWeight(u, v);
            double reweightedEdge = originalWeight + h[u] - h[v];
            reweightedGraph->addEdge(u, v, reweightedEdge);
        }
    }

    // Initialize the distance matrix
    distances = std::vector<std::vector<double>>(
        V, std::vector<double>(V, std::numeric_limits<double>::infinity()));

    // Run Dijkstra from each vertex
    for (int u : vertices) {
        std::vector<double> vertexDistances;
        std::vector<int> vertexPrevious = dijkstra(*reweightedGraph, u, vertexDistances);

        // Convert the reweighted distances back to original weights
        for (int v : vertices) {
            if (vertexDistances[v] != std::numeric_limits<double>::infinity()) {
                distances[u][v] = vertexDistances[v] - h[u] + h[v];
            }
        }
    }

    return true; // Success
}
```

## 🎯 Specialized Shortest Path Algorithms

Depending on the specific characteristics of a graph, several specialized algorithms can be more efficient than the general-purpose ones we've covered.

### 1. A\* Search Algorithm:

A\* (pronounced "A-star") is an informed search algorithm that uses a heuristic to guide its search. It's widely used in pathfinding and graph traversal.

```cpp
std::vector<int> aStarSearch(const IGraph& graph, int start, int goal,
                           std::function<double(int, int)> heuristic) {
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>> openSet;

    std::unordered_set<int> closedSet;
    std::vector<double> gScore(graph.getVertexCount(), std::numeric_limits<double>::infinity());
    std::vector<double> fScore(graph.getVertexCount(), std::numeric_limits<double>::infinity());
    std::vector<int> previous(graph.getVertexCount(), -1);

    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);
    openSet.push({fScore[start], start});

    while (!openSet.empty()) {
        int current = openSet.top().second;
        openSet.pop();

        if (current == goal) {
            // Reconstruct path
            std::vector<int> path;
            for (int at = goal; at != -1; at = previous[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (closedSet.find(current) != closedSet.end()) {
            continue;
        }

        closedSet.insert(current);

        for (int neighbor : graph.getNeighbors(current)) {
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            double tentativeGScore = gScore[current] + graph.getEdgeWeight(current, neighbor);

            if (tentativeGScore < gScore[neighbor]) {
                previous[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);
                openSet.push({fScore[neighbor], neighbor});
            }
        }
    }

    return {}; // No path found
}
```

### 2. Bidirectional Search:

This technique runs two simultaneous searches: one forward from the source and one backward from the destination.

```cpp
std::vector<int> bidirectionalSearch(const IGraph& graph, int source, int destination) {
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

    while (!forwardQueue.empty() && !backwardQueue.empty()) {
        // Expand forward search
        int currentForward = forwardQueue.front();
        forwardQueue.pop();

        for (int neighbor : graph.getNeighbors(currentForward)) {
            if (forwardVisited.find(neighbor) == forwardVisited.end()) {
                forwardQueue.push(neighbor);
                forwardVisited.insert(neighbor);
                forwardParent[neighbor] = currentForward;

                if (backwardVisited.find(neighbor) != backwardVisited.end()) {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1) {
            break;
        }

        // Expand backward search
        int currentBackward = backwardQueue.front();
        backwardQueue.pop();

        for (int neighbor : graph.getNeighbors(currentBackward)) {
            if (backwardVisited.find(neighbor) == backwardVisited.end()) {
                backwardQueue.push(neighbor);
                backwardVisited.insert(neighbor);
                backwardParent[neighbor] = currentBackward;

                if (forwardVisited.find(neighbor) != forwardVisited.end()) {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1) {
            break;
        }
    }

    if (meetingPoint == -1) {
        return {}; // No path found
    }

    // Reconstruct path
    std::vector<int> path;

    // Build path from source to meeting point
    for (int at = meetingPoint; at != -1; at = forwardParent[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    // Build path from meeting point to destination
    for (int at = backwardParent[meetingPoint]; at != -1; at = backwardParent[at]) {
        path.push_back(at);
    }

    return path;
}
```

### 3. Contraction Hierarchies:

This is a preprocessing-based technique that dramatically speeds up shortest path queries in static road networks.

```cpp
// Simplified representation of contraction hierarchies
class ContractionHierarchy {
private:
    struct ShortcutEdge {
        int to;
        double weight;
        int viaNode; // The bypassed node

        ShortcutEdge(int t, double w, int v) : to(t), weight(w), viaNode(v) {}
    };

    std::vector<std::vector<std::pair<int, double>>> upGraph; // Upward edges
    std::vector<std::vector<std::pair<int, double>>> downGraph; // Downward edges
    std::vector<int> nodeOrder; // Node importance

public:
    ContractionHierarchy(const IGraph& graph) {
        // Preprocessing phase - would compute node ordering and add shortcuts
        // This is a complex process and simplified here
    }

    std::vector<int> query(int source, int target) {
        // Bidirectional Dijkstra on the contracted graph
        // This is also simplified
        return {};
    }
};
```

## 🏠 Real-World Applications of Shortest Path Algorithms

Shortest path algorithms have countless applications across various domains:

### 1. Navigation Systems

```cpp
class NavigationSystem {
private:
    std::unique_ptr<IGraph> roadNetwork;
    std::unordered_map<int, GeoCoordinate> nodeLocations;

public:
    NavigationSystem(const std::string& mapDataFile) {
        // Load map data
        loadMapData(mapDataFile);
    }

    std::vector<int> findRoute(const GeoCoordinate& start, const GeoCoordinate& end) {
        // Find closest nodes to coordinates
        int startNode = findClosestNode(start);
        int endNode = findClosestNode(end);

        // Calculate route using A* with haversine distance as heuristic
        return aStarSearch(*roadNetwork, startNode, endNode,
                          [this, &end](int node, int goal) {
                              return haversineDistance(nodeLocations[node],
                                                      nodeLocations[goal]);
                          });
    }

    double estimateTravelTime(const std::vector<int>& route) {
        double time = 0.0;
        for (size_t i = 0; i < route.size() - 1; i++) {
            time += roadNetwork->getEdgeWeight(route[i], route[i+1]);
        }
        return time; // In minutes
    }
};
```

### 2. Network Routing Protocols

```cpp
class Router {
private:
    std::unique_ptr<IGraph> network;
    std::vector<std::vector<double>> distanceTable;
    std::vector<std::vector<int>> nextHopTable;

public:
    void updateRoutingTables() {
        // Use Floyd-Warshall to compute all-pairs shortest paths
        floydWarshall(*network, distanceTable, nextHopTable);
    }

    int getNextHop(int source, int destination) {
        return nextHopTable[source][destination];
    }

    void handleLinkFailure(int node1, int node2) {
        // Remove the link
        network->removeEdge(node1, node2);

        // Update routing tables
        updateRoutingTables();
    }
};
```

### 3. Social Network Analysis

```cpp
class SocialNetworkAnalyzer {
private:
    std::unique_ptr<IGraph> socialGraph;

public:
    double calculateAverageDistance() {
        std::vector<std::vector<double>> distances;
        std::vector<std::vector<int>> next;

        floydWarshall(*socialGraph, distances, next);

        double sum = 0.0;
        int count = 0;

        for (size_t i = 0; i < distances.size(); i++) {
            for (size_t j = 0; j < distances[i].size(); j++) {
                if (i != j && distances[i][j] != std::numeric_limits<double>::infinity()) {
                    sum += distances[i][j];
                    count++;
                }
            }
        }

        return count > 0 ? sum / count : 0.0;
    }

    bool isSixDegreesOfSeparation() {
        std::vector<std::vector<double>> distances;
        std::vector<std::vector<int>> next;

        floydWarshall(*socialGraph, distances, next);

        for (size_t i = 0; i < distances.size(); i++) {
            for (size_t j = 0; j < distances[i].size(); j++) {
                if (i != j && distances[i][j] > 6) {
                    return false;
                }
            }
        }

        return true;
    }
};
```

### 4. Circuit Design

```cpp
class CircuitRouter {
private:
    std::unique_ptr<IGraph> gridGraph;

public:
    std::vector<std::pair<int, int>> routeConnection(
        std::pair<int, int> start, std::pair<int, int> end) {

        // Convert grid coordinates to node IDs
        int startNode = coordToNode(start);
        int endNode = coordToNode(end);

        // Use A* search for routing
        auto path = aStarSearch(*gridGraph, startNode, endNode,
                              [this, &end](int node, int goal) {
                                  auto nodeCoord = nodeToCoord(node);
                                  auto goalCoord = nodeToCoord(goal);
                                  return manhattanDistance(nodeCoord, goalCoord);
                              });

        // Convert node IDs back to coordinates
        std::vector<std::pair<int, int>> route;
        for (int node : path) {
            route.push_back(nodeToCoord(node));
        }

        return route;
    }
};
```

### 5. Flight Planning

```cpp
class FlightPlanner {
private:
    std::unique_ptr<IGraph> airportNetwork;

    enum class CostMetric {
        DISTANCE,
        TIME,
        FUEL,
        PRICE
    };

public:
    std::vector<int> planRoute(int departureAirport, int arrivalAirport,
                             CostMetric metric) {

        // Adjust edge weights based on the selected metric
        adjustEdgeWeights(metric);

        // Run Dijkstra's algorithm
        std::vector<double> distances;
        auto previous = dijkstra(*airportNetwork, departureAirport, distances);

        // Reconstruct the path
        return reconstructPath(previous, departureAirport, arrivalAirport);
    }

    std::vector<int> planMultiCityRoute(const std::vector<int>& cities) {
        // Solve as a variant of the Traveling Salesman Problem
        // This is a simplification - would use more complex algorithms in practice

        std::vector<int> route;

        // Use Floyd-Warshall to compute all-pairs shortest paths
        std::vector<std::vector<double>> distances;
        std::vector<std::vector<int>> next;
        floydWarshall(*airportNetwork, distances, next);

        // Greedy approach: always go to the closest unvisited city
        int current = cities[0];
        route.push_back(current);

        std::unordered_set<int> visited = {current};

        while (visited.size() < cities.size()) {
            double minDist = std::numeric_limits<double>::infinity();
            int nextCity = -1;

            for (int city : cities) {
                if (visited.find(city) == visited.end() &&
                    distances[current][city] < minDist) {
                    minDist = distances[current][city];
                    nextCity = city;
                }
            }

            if (nextCity == -1) {
                break; // No reachable unvisited cities
            }

            // Add path from current to nextCity
            std::vector<int> subpath =
                reconstructPathFloydWarshall(next, current, nextCity);

            // Skip the first node as it's already in the route
            for (size_t i = 1; i < subpath.size(); i++) {
                route.push_back(subpath[i]);
            }

            current = nextCity;
            visited.insert(current);
        }

        return route;
    }
};
```

## 😄 Shortest Path Jokes and Analogies

1. **The GPS Joke**:
   "Why did the GPS get a speeding ticket? Because it was always looking for the shortest path, but never watching the speed limit!"

2. **The Relationship Advice Analogy**:
   "Dijkstra's algorithm is like a good relationship counselor: it finds the shortest path between two people, but only works if there's no negativity."

3. **The Bellman-Ford Persistence Joke**:
   "Bellman-Ford is that friend who will help you find your way home even when you're in a negative mood, but they'll keep asking 'are we there yet?' |V| - 1 times."

4. **The Floyd-Warshall Time Management Analogy**:
   "Using Floyd-Warshall for a small road trip is like hiring a team of accountants to split a lunch bill. It'll work, but the preprocessing time might be longer than the meal itself!"

5. **The Negative Cycle Diet Joke**:
   "I tried to use Dijkstra's algorithm for my diet plan, but it failed because of the negative cycles I kept creating between the fridge and the couch."

6. **The A\* Optimism Analogy**:
   "A\* search is like an optimistic traveler who not only knows where they've been but also has a good guess about how far they still need to go."

7. **The Algorithm Dating Analogy**:
   "Dating is like running different shortest path algorithms:
   - Dijkstra's: You date the most promising person first, but you might miss someone special with a rough first impression.
   - Bellman-Ford: You're willing to give everyone a chance, even those with some negativity, but it takes longer.
   - Floyd-Warshall: You meticulously analyze everyone's compatibility with everyone else before making a decision.
   - A\*: You use your intuition to guide you toward someone who seems like a good match."

## 🧩 LeetCode Practice Problems

### Easy:

#### 1. Network Delay Time (LeetCode #743)

**Problem:** You are given a network of n nodes, labeled from 1 to n. You are also given times, a list of travel times as directed edges times[i] = (ui, vi, wi), where ui is the source node, vi is the target node, and wi is the time it takes for a signal to travel from source to target.

We will send a signal from a given node k. Return the minimum time it takes for all the n nodes to receive the signal. If it is impossible for all the n nodes to receive the signal, return -1.

**Solution:**

```cpp
int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k) {
    // Create adjacency list
    std::vector<std::vector<std::pair<int, int>>> graph(n + 1);

    for (const auto& edge : times) {
        int u = edge[0];
        int v = edge[1];
        int w = edge[2];
        graph[u].push_back({v, w});
    }

    // Initialize distances
    std::vector<int> dist(n + 1, INT_MAX);
    dist[k] = 0;

    // Priority queue for Dijkstra's algorithm
    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;
    pq.push({0, k});

    while (!pq.empty()) {
        auto [currDist, u] = pq.top();
        pq.pop();

        if (currDist > dist[u]) continue;

        for (auto [v, w] : graph[u]) {
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    int maxTime = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) return -1;
        maxTime = std::max(maxTime, dist[i]);
    }

    return maxTime;
}
```

#### 2. Path With Minimum Effort (LeetCode #1631)

**Problem:** You are a hiker preparing for an upcoming hike. You are given heights, a 2D array of size rows x columns, where heights[row][col] represents the height of cell (row, col). You are situated in the top-left cell, (0, 0), and you hope to travel to the bottom-right cell, (rows-1, columns-1). You can move up, down, left, or right, and you wish to find a route that requires the minimum effort.

A route's effort is the maximum absolute difference in heights between two consecutive cells of the route.

Return the minimum effort required to travel from the top-left cell to the bottom-right cell.

**Solution:**

```cpp
int minimumEffortPath(std::vector<std::vector<int>>& heights) {
    int rows = heights.size();
    int cols = heights[0].size();

    // Define possible moves (up, right, down, left)
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};

    // Priority queue for Dijkstra's algorithm: {effort, {row, col}}
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                       std::vector<std::pair<int, std::pair<int, int>>>,
                       std::greater<std::pair<int, std::pair<int, int>>>> pq;

    // Initialize efforts array with maximum value
    std::vector<std::vector<int>> efforts(rows, std::vector<int>(cols, INT_MAX));
    efforts[0][0] = 0;

    pq.push({0, {0, 0}});

    while (!pq.empty()) {
        auto [effort, pos] = pq.top();
        int r = pos.first;
        int c = pos.second;
        pq.pop();

        // If we've reached the bottom-right cell, return the effort
        if (r == rows - 1 && c == cols - 1) {
            return effort;
        }

        // Skip if we've found a better path
        if (effort > efforts[r][c]) continue;

        // Try all four directions
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check if the new position is valid
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                // Calculate new effort
                int newEffort = std::max(effort, std::abs(heights[nr][nc] - heights[r][c]));

                // If we found a better path, update and push to queue
                if (newEffort < efforts[nr][nc]) {
                    efforts[nr][nc] = newEffort;
                    pq.push({newEffort, {nr, nc}});
                }
            }
        }
    }

    return -1; // Should not reach here if grid is connected
}
```

### Medium:

#### 1. Cheapest Flights Within K Stops (LeetCode #787)

**Problem:** There are n cities connected by some number of flights. You are given an array flights where flights[i] = [fromi, toi, pricei] indicates that there is a flight from city fromi to city toi with cost pricei.

You are also given three integers src, dst, and k, return the cheapest price from src to dst with at most k stops. If there is no such route, return -1.

**Solution:**

```cpp
int findCheapestPrice(int n, std::vector<std::vector<int>>& flights, int src, int dst, int k) {
    // Create adjacency list
    std::vector<std::vector<std::pair<int, int>>> graph(n);

    for (const auto& flight : flights) {
        int from = flight[0];
        int to = flight[1];
        int price = flight[2];
        graph[from].push_back({to, price});
    }

    // Array to keep track of minimum cost to reach each node
    std::vector<int> costs(n, INT_MAX);
    costs[src] = 0;

    // Queue for BFS: {node, cost, stops}
    std::queue<std::vector<int>> q;
    q.push({src, 0, 0});  // Start with source node

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        int node = curr[0];
        int cost = curr[1];
        int stops = curr[2];

        // If we've used too many stops, skip
        if (stops > k) continue;

        // Check all neighbors
        for (auto [neighbor, price] : graph[node]) {
            int newCost = cost + price;

            // If we've found a cheaper path with <= k stops
            // or if we're arriving with fewer stops than before
            if (newCost < costs[neighbor] || stops < k) {
                costs[neighbor] = std::min(costs[neighbor], newCost);
                q.push({neighbor, newCost, stops + 1});
            }
        }
    }

    return costs[dst] == INT_MAX ? -1 : costs[dst];
}
```

#### 2. Path with Maximum Probability (LeetCode #1514)

**Problem:** You are given an undirected weighted graph of n nodes (0-indexed), represented by an edge list where edges[i] = [a, b] is an undirected edge connecting the nodes a and b with a probability of success of traversing that edge succProb[i].

Given two nodes start and end, find the path with the maximum probability of success to go from start to end and return its success probability.

If there is no path from start to end, return 0. Your answer will be accepted if it differs from the correct answer by at most 1e-5.

**Solution:**

```cpp
double maxProbability(int n, std::vector<std::vector<int>>& edges,
                     std::vector<double>& succProb, int start, int end) {
    // Create adjacency list
    std::vector<std::vector<std::pair<int, double>>> graph(n);

    for (int i = 0; i < edges.size(); i++) {
        int a = edges[i][0];
        int b = edges[i][1];
        double prob = succProb[i];

        graph[a].push_back({b, prob});
        graph[b].push_back({a, prob});  // Undirected graph
    }

    // Array to store maximum probability to reach each node
    std::vector<double> maxProb(n, 0.0);
    maxProb[start] = 1.0;

    // Priority queue for Dijkstra's algorithm: {probability, node}
    // Note: Using negative probability because C++ priority_queue is a max heap
    std::priority_queue<std::pair<double, int>> pq;
    pq.push({1.0, start});

    while (!pq.empty()) {
        auto [prob, node] = pq.top();
        pq.pop();

        // If we've reached the end, return the probability
        if (node == end) {
            return prob;
        }

        // If we've found a better path, skip
        if (prob < maxProb[node]) continue;

        // Check all neighbors
        for (auto [neighbor, edgeProb] : graph[node]) {
            double newProb = prob * edgeProb;

            // If we've found a better path, update and push to queue
            if (newProb > maxProb[neighbor]) {
                maxProb[neighbor] = newProb;
                pq.push({newProb, neighbor});
            }
        }
    }

    return 0.0; // No path found
}
```

### Hard:

#### 1. Minimum Cost to Make at Least One Valid Path in a Grid (LeetCode #1368)

**Problem:** Given a m x n grid. Each cell of the grid has a sign pointing to one of the 4 directions: 'up', 'down', 'left' or 'right', which is represented by 1, 2, 3, and 4 respectively.

You can change the sign in any cell if needed, but it costs 1 to do so. You need to find the minimum cost to reach the bottom right cell (m-1, n-1) from the top left cell (0, 0) by following the signs.

**Solution:**

```cpp
int minCost(std::vector<std::vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();

    // Directions: right, left, down, up (matching the problem's encoding)
    const int dx[] = {0, 0, 0, 1, -1};
    const int dy[] = {0, 1, -1, 0, 0};

    // 0-1 BFS using a deque
    std::deque<std::vector<int>> deque;  // {row, col, cost}
    std::vector<std::vector<int>> costs(m, std::vector<int>(n, INT_MAX));

    deque.push_back({0, 0, 0});
    costs[0][0] = 0;

    while (!deque.empty()) {
        auto curr = deque.front();
        deque.pop_front();

        int row = curr[0];
        int col = curr[1];
        int cost = curr[2];

        // If we've reached the destination
        if (row == m - 1 && col == n - 1) {
            return cost;
        }

        // If we've found a better path, skip
        if (cost > costs[row][col]) continue;

        // Try all four directions
        for (int dir = 1; dir <= 4; dir++) {
            int nr = row + dx[dir];
            int nc = col + dy[dir];

            // Check if the new position is valid
            if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                int newCost = cost + (grid[row][col] == dir ? 0 : 1);

                if (newCost < costs[nr][nc]) {
                    costs[nr][nc] = newCost;

                    // If we don't need to change the sign, add to front of deque
                    if (grid[row][col] == dir) {
                        deque.push_front({nr, nc, newCost});
                    } else {
                        deque.push_back({nr, nc, newCost});
                    }
                }
            }
        }
    }

    return -1; // Should not reach here if grid is connected
}
```

#### 2. Swim in Rising Water (LeetCode #778)

**Problem:** On an N x N grid, each square grid[i][j] represents the elevation at that point (i,j).

Now rain starts to fall. At time t, the depth of the water everywhere is t. You can swim from a square to another 4-directionally adjacent square if and only if the elevation of both squares individually are at most t. You can swim infinite distance in zero time. Of course, you must stay within the boundaries of the grid during your swim.

You start at the top left square (0, 0). What is the least time until you can reach the bottom right square (N-1, N-1)?

**Solution:**

```cpp
int swimInWater(std::vector<std::vector<int>>& grid) {
    int n = grid.size();

    // Directions: up, right, down, left
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};

    // Priority queue for Dijkstra's algorithm: {time, {row, col}}
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                       std::vector<std::pair<int, std::pair<int, int>>>,
                       std::greater<std::pair<int, std::pair<int, int>>>> pq;

    // Initialize visited array
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));

    // Start from top-left corner
    pq.push({grid[0][0], {0, 0}});
    visited[0][0] = true;

    while (!pq.empty()) {
        auto [time, pos] = pq.top();
        int r = pos.first;
        int c = pos.second;
        pq.pop();

        // If we've reached the bottom-right corner
        if (r == n - 1 && c == n - 1) {
            return time;
        }

        // Try all four directions
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check if the new position is valid
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && !visited[nr][nc]) {
                // The time needed is the maximum of current time and elevation
                int newTime = std::max(time, grid[nr][nc]);

                visited[nr][nc] = true;
                pq.push({newTime, {nr, nc}});
            }
        }
    }

    return -1; // Should not reach here if grid is connected
}
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll be exploring **Minimum Spanning Trees (MSTs)** — a fundamental concept in graph theory that focuses on connecting all vertices with the minimum total edge weight. We'll dive into:

- Prim's Algorithm for building MSTs
- Kruskal's Algorithm using union-find data structure
- Applications in network design, clustering, and more
- Optimizations and practical implementations

_If shortest path algorithms help us find the most efficient route between specific points, minimum spanning trees help us connect all points in the most economical way possible. These algorithms are crucial for designing efficient networks, from telecommunications infrastructure to water supply systems. Join us tomorrow as we discover how to connect the dots with minimal resources!_

#DSAin45 #Day31 #ShortestPathAlgorithms #Dijkstra #BellmanFord #FloydWarshall #GraphAlgorithms #CPlusPlus #CompetitiveProgramming
