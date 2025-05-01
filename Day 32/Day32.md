# 🌳 DAY 32: Minimum Spanning Trees - Connecting the Dots Efficiently #DSAin45

Welcome to Day 32 of our 45-day DSA journey! Yesterday, we explored shortest path algorithms that help us find the most efficient routes between specific points in a graph. Today, we'll focus on **Minimum Spanning Trees (MSTs)** — algorithms that help us connect all points in a graph with the minimum possible total edge weight.

_Imagine you're designing a network of roads to connect several cities, or laying cables to connect computers in a network. You want to ensure all points are connected while minimizing the total cost. This is precisely the problem that minimum spanning tree algorithms solve._

## 📌 Minimum Spanning Tree Fundamentals

A **spanning tree** of an undirected graph is a subgraph that includes all vertices of the original graph and is a tree (connected and acyclic). A **minimum spanning tree** is a spanning tree with the minimum possible total edge weight.

Key properties of a minimum spanning tree:

1. **Connects all vertices**: Every vertex in the original graph is included
2. **Contains no cycles**: It's a tree, so there are no loops
3. **Minimizes total edge weight**: The sum of all edge weights is minimal
4. **Has exactly n-1 edges**: Where n is the number of vertices
5. **May not be unique**: A graph can have multiple valid MSTs if it has edges with equal weights

MSTs have numerous applications in various fields:

- Network design (telecommunications, electrical, transportation, etc.)
- Cluster analysis in data mining
- Image segmentation in computer vision
- Approximation algorithms for NP-hard problems like the Traveling Salesman Problem

## 🔍 Prim's Algorithm

Prim's algorithm is a greedy approach to finding the MST. It starts from an arbitrary vertex and grows the MST one vertex at a time by always adding the lowest-weight edge that connects a vertex in the MST to a vertex outside the MST.

### Key Characteristics of Prim's Algorithm:

- Works similarly to Dijkstra's algorithm
- Builds the MST incrementally, one vertex at a time
- Maintains a priority queue of edges connecting the tree to vertices not yet in the tree
- Always selects the minimum-weight edge that doesn't create a cycle
- Time complexity: O(E log V) with a binary heap
- Space complexity: O(V + E)

### Prim's Algorithm Implementation:

```cpp
std::vector<std::pair<int, int>> primMST(const IGraph& graph, double& totalWeight) {
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
                        std::greater<std::tuple<double, int, int>>> pq;

    // Add all edges from startVertex to the priority queue
    for (int neighbor : graph.getNeighbors(startVertex)) {
        double weight = graph.getEdgeWeight(startVertex, neighbor);
        pq.push({weight, startVertex, neighbor});
    }

    // Process until all vertices are included in MST
    while (!pq.empty() && inMST.size() < n) {
        // Get the minimum weight edge
        auto [weight, from, to] = pq.top();
        pq.pop();

        // If the destination vertex is already in MST, skip
        if (inMST.find(to) != inMST.end()) {
            continue;
        }

        // Add the vertex to MST
        inMST.insert(to);

        // Add the edge to MST
        mstEdges.push_back({from, to});

        // Update total weight
        totalWeight += weight;

        // Add all edges from the newly added vertex
        for (int neighbor : graph.getNeighbors(to)) {
            // Only consider vertices not yet in MST
            if (inMST.find(neighbor) == inMST.end()) {
                double weight = graph.getEdgeWeight(to, neighbor);
                pq.push({weight, to, neighbor});
            }
        }
    }

    // Check if we have a spanning tree
    if (inMST.size() != n) {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}
```

### Prim's Algorithm With An Example:

Let's trace through the algorithm on this weighted graph:

```
    6       5
A ----- B ----- C
|       |       |
4|      |1      |2
|       |       |
D ----- E ----- F
    3       8
```

Starting from vertex A:

1. Add A to MST: MST = {A}
2. Add edges from A to priority queue: (A-B, 6), (A-D, 4)
3. Extract minimum edge (A-D, 4): Add D to MST, MST = {A, D}
4. Add edges from D to priority queue: (A-B, 6), (D-E, 3)
5. Extract minimum edge (D-E, 3): Add E to MST, MST = {A, D, E}
6. Add edges from E to priority queue: (A-B, 6), (E-B, 1), (E-F, 8)
7. Extract minimum edge (E-B, 1): Add B to MST, MST = {A, D, E, B}
8. Add edges from B to priority queue: (A-B, 6), (E-F, 8), (B-C, 5)
9. Extract minimum edge (B-C, 5): Add C to MST, MST = {A, D, E, B, C}
10. Add edges from C to priority queue: (A-B, 6), (E-F, 8), (C-F, 2)
11. Extract minimum edge (C-F, 2): Add F to MST, MST = {A, D, E, B, C, F}
12. All vertices are in MST, algorithm terminates

Final MST edges: (A-D, 4), (D-E, 3), (E-B, 1), (B-C, 5), (C-F, 2)
Total weight: 4 + 3 + 1 + 5 + 2 = 15

### Optimizations for Prim's Algorithm:

1. **Lazy Implementation**: The code above is a lazy implementation that may add multiple edges for the same destination vertex. An eager implementation would maintain a priority queue of vertices instead of edges.

2. **Fibonacci Heap**: Using a Fibonacci heap for the priority queue can improve the theoretical time complexity to O(E + V log V).

3. **Dense Graphs**: For dense graphs, a simpler implementation that doesn't use a priority queue can be more efficient, with O(V²) time complexity.

## 🧩 Kruskal's Algorithm

Kruskal's algorithm is another greedy approach to finding the MST. It sorts all edges by weight and then iteratively adds the lowest-weight edge that doesn't create a cycle.

### Key Characteristics of Kruskal's Algorithm:

- Sorts all edges by weight
- Adds edges to the MST in order of increasing weight
- Uses a disjoint-set (union-find) data structure to efficiently check if adding an edge creates a cycle
- Time complexity: O(E log E) or O(E log V) since E is at most V²
- Space complexity: O(V + E)

### Disjoint-Set (Union-Find) Data Structure:

Kruskal's algorithm relies on the disjoint-set data structure to efficiently determine if adding an edge would create a cycle. Let's implement this first:

```cpp
class DisjointSet {
private:
    std::unordered_map<int, int> parent;  // Maps vertex to its parent
    std::unordered_map<int, int> rank;    // Maps vertex to its rank (approx. height)

public:
    DisjointSet(const std::vector<int>& vertices) {
        // Initialize each vertex as a separate set
        for (int v : vertices) {
            makeSet(v);
        }
    }

    void makeSet(int v) {
        parent[v] = v;  // Each vertex is its own parent initially
        rank[v] = 0;    // Initial rank is 0
    }

    int findSet(int v) {
        if (parent[v] != v) {
            // Path compression: make all nodes on the path point directly to the root
            parent[v] = findSet(parent[v]);
        }
        return parent[v];
    }

    void unionSets(int u, int v) {
        int rootU = findSet(u);
        int rootV = findSet(v);

        if (rootU != rootV) {
            // Union by rank: attach the shorter tree to the root of the taller tree
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;  // Increase the rank of rootU if both trees have the same rank
            }
        }
    }

    bool sameSet(int u, int v) {
        return findSet(u) == findSet(v);
    }
};
```

### Kruskal's Algorithm Implementation:

```cpp
std::vector<std::pair<int, int>> kruskalMST(const IGraph& graph, double& totalWeight) {
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Initialize result vector to store MST edges
    std::vector<std::pair<int, int>> mstEdges;

    // Initialize totalWeight
    totalWeight = 0.0;

    // Get all edges from the graph
    std::vector<std::tuple<double, int, int>> edges;

    for (int u : vertices) {
        for (int v : graph.getNeighbors(u)) {
            // For undirected graphs, add each edge only once
            if (!graph.isDirected() && u < v) {
                double weight = graph.getEdgeWeight(u, v);
                edges.push_back({weight, u, v});
            } else if (graph.isDirected()) {
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
    for (const auto& [weight, u, v] : edges) {
        // If adding this edge doesn't create a cycle, add it to the MST
        if (!ds.sameSet(u, v)) {
            ds.unionSets(u, v);
            mstEdges.push_back({u, v});
            totalWeight += weight;
        }
    }

    // Check if we have a spanning tree
    if (mstEdges.size() != n - 1) {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}
```

### Kruskal's Algorithm With An Example:

Let's use the same graph as before:

```
    6       5
A ----- B ----- C
|       |       |
4|      |1      |2
|       |       |
D ----- E ----- F
    3       8
```

Sorting all edges by weight:

1. (E-B, 1)
2. (C-F, 2)
3. (D-E, 3)
4. (A-D, 4)
5. (B-C, 5)
6. (A-B, 6)
7. (E-F, 8)

Applying Kruskal's algorithm:

1. Process (E-B, 1): Add to MST, MST = {(E-B, 1)}
2. Process (C-F, 2): Add to MST, MST = {(E-B, 1), (C-F, 2)}
3. Process (D-E, 3): Add to MST, MST = {(E-B, 1), (C-F, 2), (D-E, 3)}
4. Process (A-D, 4): Add to MST, MST = {(E-B, 1), (C-F, 2), (D-E, 3), (A-D, 4)}
5. Process (B-C, 5): Add to MST, MST = {(E-B, 1), (C-F, 2), (D-E, 3), (A-D, 4), (B-C, 5)}
6. Process (A-B, 6): Would create a cycle, so skip
7. Process (E-F, 8): Would create a cycle, so skip

Final MST edges: (E-B, 1), (C-F, 2), (D-E, 3), (A-D, 4), (B-C, 5)
Total weight: 1 + 2 + 3 + 4 + 5 = 15

Note that we get the same MST as with Prim's algorithm, although the edges were added in a different order.

### Optimizations for Kruskal's Algorithm:

1. **Edge Sorting**: For small graphs or graphs with integer weights within a small range, counting sort or radix sort can be more efficient than comparison-based sorting.

2. **Disjoint-Set Optimizations**: The union-find data structure with path compression and union by rank has almost constant time operations, making Kruskal's algorithm very efficient.

3. **Early Termination**: Once we've added n-1 edges to the MST, we can terminate the algorithm since a spanning tree on n vertices has exactly n-1 edges.

## 📊 Prim's vs. Kruskal's: When to Use Which?

Both algorithms correctly compute the minimum spanning tree, but they have different characteristics that make each more suitable for specific scenarios:

### Use Prim's Algorithm When:

1. **The Graph is Dense**: When E is close to V², Prim's algorithm with an adjacency matrix representation runs in O(V²) time, which is better than Kruskal's O(E log V).
2. **Memory is a Concern**: Prim's algorithm only needs to store the graph representation and a priority queue of at most V elements.
3. **Starting from a Specific Vertex**: If you need to grow the MST from a particular vertex.

### Use Kruskal's Algorithm When:

1. **The Graph is Sparse**: When E is much less than V², Kruskal's algorithm's O(E log E) time complexity is better.
2. **The Edges are Already Sorted**: If the edges are pre-sorted or can be sorted efficiently.
3. **Implementing a Parallel Algorithm**: Kruskal's algorithm is easier to parallelize since each edge can be processed independently.

## 🚀 Advanced MST Algorithms

While Prim's and Kruskal's algorithms are the most commonly used, there are several advanced algorithms for finding MSTs in special scenarios:

### 1. Borůvka's Algorithm:

Borůvka's algorithm is another approach to finding the MST, which is especially suitable for parallel computing. It works by repeatedly finding the minimum-weight edge out of each connected component.

```cpp
std::vector<std::pair<int, int>> boruvkaMST(const IGraph& graph, double& totalWeight) {
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Initialize result vector to store MST edges
    std::vector<std::pair<int, int>> mstEdges;

    // Initialize totalWeight
    totalWeight = 0.0;

    // Initialize disjoint-set data structure
    DisjointSet ds(vertices);

    // Continue until we have a single connected component
    while (mstEdges.size() < n - 1) {
        // For each component, find the lightest edge connecting it to another component
        std::unordered_map<int, std::tuple<double, int, int>> minEdges;

        // For each vertex
        for (int u : vertices) {
            int setU = ds.findSet(u);

            // For each of its neighbors
            for (int v : graph.getNeighbors(u)) {
                int setV = ds.findSet(v);

                // Skip if both vertices are in the same component
                if (setU == setV) {
                    continue;
                }

                double weight = graph.getEdgeWeight(u, v);

                // Update minimum edge for component setU if needed
                if (minEdges.find(setU) == minEdges.end() ||
                    std::get<0>(minEdges[setU]) > weight) {
                    minEdges[setU] = {weight, u, v};
                }
            }
        }

        // If no more edges can be added, break
        if (minEdges.empty()) {
            break;
        }

        // Add all minimum edges to the MST
        for (const auto& [set, edge] : minEdges) {
            auto [weight, u, v] = edge;

            // If adding this edge doesn't create a cycle, add it to the MST
            if (!ds.sameSet(u, v)) {
                ds.unionSets(u, v);
                mstEdges.push_back({u, v});
                totalWeight += weight;
            }
        }
    }

    // Check if we have a spanning tree
    if (mstEdges.size() != n - 1) {
        // Graph is not connected, no spanning tree exists
        mstEdges.clear();
        totalWeight = std::numeric_limits<double>::infinity();
    }

    return mstEdges;
}
```

### 2. Reverse-Delete Algorithm:

The reverse-delete algorithm is the logical opposite of Kruskal's algorithm. It starts with all edges and removes them in order of decreasing weight, as long as removing the edge doesn't disconnect the graph.

```cpp
std::vector<std::pair<int, int>> reverseDeleteMST(const IGraph& graph, double& totalWeight) {
    std::vector<int> vertices = graph.getVertices();
    int n = vertices.size();

    // Get all edges from the graph
    std::vector<std::tuple<double, int, int>> edges;

    for (int u : vertices) {
        for (int v : graph.getNeighbors(u)) {
            // For undirected graphs, add each edge only once
            if (!graph.isDirected() && u < v) {
                double weight = graph.getEdgeWeight(u, v);
                edges.push_back({weight, u, v});
            } else if (graph.isDirected()) {
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
    for (int v : vertices) {
        mstGraph->addVertex(v);
    }

    // Add all edges
    for (const auto& [weight, u, v] : edges) {
        mstGraph->addEdge(u, v, weight);
    }

    // Process edges in order of decreasing weight
    for (const auto& [weight, u, v] : edges) {
        // Temporarily remove the edge
        mstGraph->removeEdge(u, v);

        // Check if the graph is still connected
        bool connected = isConnected(*mstGraph);

        // If removing the edge disconnects the graph, add it back
        if (!connected) {
            mstGraph->addEdge(u, v, weight);
        }
    }

    // Extract the MST edges
    std::vector<std::pair<int, int>> mstEdges;
    totalWeight = 0.0;

    for (int u : vertices) {
        for (int v : mstGraph->getNeighbors(u)) {
            // For undirected graphs, add each edge only once
            if (u < v) {
                double weight = mstGraph->getEdgeWeight(u, v);
                mstEdges.push_back({u, v});
                totalWeight += weight;
            }
        }
    }

    return mstEdges;
}

// Helper function to check if a graph is connected
bool isConnected(const IGraph& graph) {
    std::vector<int> vertices = graph.getVertices();
    if (vertices.empty()) {
        return true;
    }

    std::unordered_set<int> visited;
    std::queue<int> queue;

    // Start BFS from the first vertex
    int start = vertices[0];
    visited.insert(start);
    queue.push(start);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int neighbor : graph.getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }

    // Check if all vertices are visited
    return visited.size() == vertices.size();
}
```

## 🌍 Real-World Applications of MSTs

Minimum Spanning Trees have numerous practical applications across various domains:

### 1. Network Design

```cpp
class NetworkDesigner {
private:
    std::unique_ptr<IGraph> networkGraph;

public:
    NetworkDesigner() {
        networkGraph = std::make_unique<AdjacencyListGraph>(false, true);
    }

    void addLocation(int locationId, double x, double y) {
        networkGraph->addVertex(locationId);
        // Store location coordinates for visualization
    }

    void addPotentialConnection(int loc1, int loc2, double cost) {
        networkGraph->addEdge(loc1, loc2, cost);
    }

    std::vector<std::pair<int, int>> designOptimalNetwork() {
        double totalCost;
        // Use Kruskal's algorithm for sparse graphs
        return kruskalMST(*networkGraph, totalCost);
    }

    double getNetworkCost(const std::vector<std::pair<int, int>>& connections) {
        double totalCost = 0.0;
        for (const auto& [from, to] : connections) {
            totalCost += networkGraph->getEdgeWeight(from, to);
        }
        return totalCost;
    }
};
```

### 2. Cluster Analysis in Data Mining

```cpp
class ClusterAnalyzer {
private:
    std::vector<std::vector<double>> dataPoints;
    std::unique_ptr<IGraph> similarityGraph;

public:
    ClusterAnalyzer(const std::vector<std::vector<double>>& data) {
        dataPoints = data;
        similarityGraph = std::make_unique<AdjacencyListGraph>(false, true);

        // Add vertices for each data point
        for (int i = 0; i < dataPoints.size(); i++) {
            similarityGraph->addVertex(i);
        }

        // Add edges with distances between data points
        for (int i = 0; i < dataPoints.size(); i++) {
            for (int j = i + 1; j < dataPoints.size(); j++) {
                double distance = euclideanDistance(dataPoints[i], dataPoints[j]);
                similarityGraph->addEdge(i, j, distance);
            }
        }
    }

    double euclideanDistance(const std::vector<double>& p1, const std::vector<double>& p2) {
        double sum = 0.0;
        for (size_t i = 0; i < p1.size(); i++) {
            double diff = p1[i] - p2[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }

    std::vector<std::vector<int>> createClusters(int k) {
        // Get the MST
        double totalWeight;
        auto mstEdges = kruskalMST(*similarityGraph, totalWeight);

        // Sort MST edges by weight (distance)
        std::vector<std::tuple<double, int, int>> sortedEdges;
        for (const auto& [u, v] : mstEdges) {
            double weight = similarityGraph->getEdgeWeight(u, v);
            sortedEdges.push_back({weight, u, v});
        }
        std::sort(sortedEdges.begin(), sortedEdges.end());

        // Remove the k-1 most expensive edges to create k clusters
        for (int i = 0; i < k - 1 && i < sortedEdges.size(); i++) {
            mstEdges.pop_back();
        }

        // Create clusters using the remaining MST edges
        DisjointSet ds(similarityGraph->getVertices());
        for (const auto& [u, v] : mstEdges) {
            ds.unionSets(u, v);
        }

        // Collect data points in each cluster
        std::unordered_map<int, std::vector<int>> clusterMap;
        for (int i = 0; i < dataPoints.size(); i++) {
            int clusterId = ds.findSet(i);
            clusterMap[clusterId].push_back(i);
        }

        // Convert to vector of clusters
        std::vector<std::vector<int>> clusters;
        for (const auto& [clusterId, points] : clusterMap) {
            clusters.push_back(points);
        }

        return clusters;
    }
};
```

### 3. Image Segmentation

```cpp
class ImageSegmenter {
private:
    std::vector<std::vector<int>> image; // Pixel values
    std::unique_ptr<IGraph> pixelGraph;
    int width, height;

public:
    ImageSegmenter(const std::vector<std::vector<int>>& img) {
        image = img;
        height = image.size();
        width = height > 0 ? image[0].size() : 0;

        pixelGraph = std::make_unique<AdjacencyListGraph>(false, true);

        // Add vertices for each pixel
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixelId = y * width + x;
                pixelGraph->addVertex(pixelId);
            }
        }

        // Add edges between adjacent pixels
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixelId = y * width + x;

                // Connect to right neighbor
                if (x + 1 < width) {
                    int neighborId = y * width + (x + 1);
                    double diff = std::abs(image[y][x] - image[y][x + 1]);
                    pixelGraph->addEdge(pixelId, neighborId, diff);
                }

                // Connect to bottom neighbor
                if (y + 1 < height) {
                    int neighborId = (y + 1) * width + x;
                    double diff = std::abs(image[y][x] - image[y + 1][x]);
                    pixelGraph->addEdge(pixelId, neighborId, diff);
                }
            }
        }
    }

    std::vector<std::vector<int>> segmentImage(int numSegments) {
        // Get the MST
        double totalWeight;
        auto mstEdges = kruskalMST(*pixelGraph, totalWeight);

        // Sort MST edges by weight (pixel difference)
        std::vector<std::tuple<double, int, int>> sortedEdges;
        for (const auto& [u, v] : mstEdges) {
            double weight = pixelGraph->getEdgeWeight(u, v);
            sortedEdges.push_back({weight, u, v});
        }
        std::sort(sortedEdges.begin(), sortedEdges.end(), std::greater<std::tuple<double, int, int>>());

        // Remove the numSegments-1 most expensive edges
        std::vector<std::pair<int, int>> edgesToRemove;
        for (int i = 0; i < numSegments - 1 && i < sortedEdges.size(); i++) {
            auto [weight, u, v] = sortedEdges[i];
            edgesToRemove.push_back({u, v});
        }

        // Create segments using the removed edges
        DisjointSet ds(pixelGraph->getVertices());
        for (const auto& [u, v] : mstEdges) {
            // Skip edges that should be removed
            if (std::find(edgesToRemove.begin(), edgesToRemove.end(), std::make_pair(u, v)) == edgesToRemove.end() &&
                std::find(edgesToRemove.begin(), edgesToRemove.end(), std::make_pair(v, u)) == edgesToRemove.end()) {
                ds.unionSets(u, v);
            }
        }

        // Create segmentation result
        std::vector<std::vector<int>> segmentation(height, std::vector<int>(width));
        std::unordered_map<int, int> segmentIds;
        int nextSegmentId = 0;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixelId = y * width + x;
                int setId = ds.findSet(pixelId);

                // Assign segment ID
                if (segmentIds.find(setId) == segmentIds.end()) {
                    segmentIds[setId] = nextSegmentId++;
                }

                segmentation[y][x] = segmentIds[setId];
            }
        }

        return segmentation;
    }
};
```

### 4. Approximating the Traveling Salesman Problem

```cpp
class TSPApproximator {
private:
    std::unique_ptr<IGraph> graph;
    std::unordered_map<int, std::pair<double, double>> coordinates;

public:
    TSPApproximator() {
        graph = std::make_unique<AdjacencyListGraph>(false, true);
    }

    void addCity(int cityId, double x, double y) {
        graph->addVertex(cityId);
        coordinates[cityId] = {x, y};

        // Add edges to all existing cities
        for (const auto& [existingCity, _] : coordinates) {
            if (existingCity != cityId) {
                double distance = euclideanDistance(x, y, coordinates[existingCity].first, coordinates[existingCity].second);
                graph->addEdge(cityId, existingCity, distance);
            }
        }
    }

    double euclideanDistance(double x1, double y1, double x2, double y2) {
        double dx = x1 - x2;
        double dy = y1 - y2;
        return std::sqrt(dx * dx + dy * dy);
    }

    std::vector<int> approximateTSPSolution() {
        // Step 1: Find MST
        double mstWeight;
        auto mstEdges = primMST(*graph, mstWeight);

        // Step 2: Convert MST to a multigraph by doubling each edge
        auto multiGraph = std::make_unique<AdjacencyListGraph>(true, true);

        for (int cityId : graph->getVertices()) {
            multiGraph->addVertex(cityId);
        }

        for (const auto& [u, v] : mstEdges) {
            double weight = graph->getEdgeWeight(u, v);
            multiGraph->addEdge(u, v, weight);
            multiGraph->addEdge(v, u, weight);
        }

        // Step 3: Find an Eulerian circuit
        std::vector<int> eulerianCircuit = findEulerianCircuit(*multiGraph);

        // Step 4: Create Hamiltonian circuit by skipping repeated vertices
        std::vector<int> tspRoute;
        std::unordered_set<int> visited;

        for (int city : eulerianCircuit) {
            if (visited.find(city) == visited.end()) {
                tspRoute.push_back(city);
                visited.insert(city);
            }
        }

        // Complete the circuit
        if (!tspRoute.empty()) {
            tspRoute.push_back(tspRoute[0]);
        }

        return tspRoute;
    }

    std::vector<int> findEulerianCircuit(const IGraph& g) {
        // Implementation of Hierholzer's algorithm for Eulerian circuit
        // This is a simplified version and may not handle all edge cases

        std::vector<int> circuit;
        std::vector<int> vertices = g.getVertices();

        if (vertices.empty()) {
            return circuit;
        }

        // Start at any vertex
        int start = vertices[0];

        // Create a working copy of the graph
        auto workGraph = std::make_unique<AdjacencyListGraph>(true, true);

        for (int v : vertices) {
            workGraph->addVertex(v);
        }

        for (int u : vertices) {
            for (int v : g.getNeighbors(u)) {
                double weight = g.getEdgeWeight(u, v);
                workGraph->addEdge(u, v, weight);
            }
        }

        // Start with a single vertex in the circuit
        circuit.push_back(start);
        int currentVertex = start;

        while (true) {
            std::vector<int> neighbors = workGraph->getNeighbors(currentVertex);

            if (neighbors.empty()) {
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

    double getRouteCost(const std::vector<int>& route) {
        double totalCost = 0.0;

        for (size_t i = 0; i < route.size() - 1; i++) {
            int from = route[i];
            int to = route[i + 1];
            totalCost += graph->getEdgeWeight(from, to);
        }

        return totalCost;
    }
};
```

## 😄 Minimum Spanning Tree Jokes and Analogies

1. **The Party Planner Analogy**:
   "Prim's algorithm is like planning a party where you start with one friend, and each time you invite the closest friend of anyone already invited. Kruskal's algorithm is like planning a party by just inviting the closest pairs of friends, regardless of who knows whom, as long as you don't create cliques."

2. **The Budget Project Manager Joke**:
   "Why was the MST algorithm hired as a project manager? Because it always finds a way to connect all the stakeholders with minimal overhead!"

3. **The Dating App Analogy**:
   "Kruskal's algorithm is like a dating app that matches people based solely on compatibility scores, without caring if they share any connections. Prim's algorithm is more like traditional matchmaking, where you start with one person and expand through their social circle."

4. **The Road Construction Joke**:
   "The transportation department tried to use Prim's algorithm to plan a new highway system, but they kept getting stuck at local minimum traffic jams."

5. **The Office Network Analogy**:
   "MST algorithms are like figuring out the minimum number of office gossip channels needed to ensure everyone gets the latest news. Ideally, you want just enough connections for information to reach everyone, but not so many that you're overwhelmed with redundant stories about Bob's new haircut."

6. **The Utility Company Dilemma**:
   "If a tree falls in the forest and nobody hears it, did it really fall? If an MST algorithm eliminates a high-weight edge and no other algorithm sees it, is it really a minimum spanning tree?"

7. **The Family Reunion Analogy**:
   "Finding the MST of your family connections is like planning a family reunion with just enough relatives to maintain the family ties but not so many that you have to deal with all the awkward conversations."

## 🧩 LeetCode Practice Problems

### Easy:

#### 1. Min Cost to Connect All Points (LeetCode #1584)

**Problem:** You are given an array points representing integer coordinates of some points on a 2D-plane, where points[i] = [xi, yi]. The cost of connecting two points [xi, yi] and [xj, yj] is the Manhattan distance between them: |xi - xj| + |yi - yj|. Return the minimum cost to make all points connected.

**Solution:**

```cpp
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();

    // Create a complete graph using Manhattan distances
    std::vector<std::tuple<int, int, int>> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int distance = abs(points[i][0] - points[j][0]) + abs(points[i][1] - points[j][1]);
            edges.push_back({distance, i, j});
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    // Initialize disjoint-set data structure
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    // Find function with path compression
    std::function<int(int)> find = [&](int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    // Union function with rank
    auto unionSets = [&](int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false;
        }

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }

        return true;
    };

    // Kruskal's algorithm
    int minCost = 0;
    int edgesAdded = 0;

    for (const auto& [weight, u, v] : edges) {
        if (unionSets(u, v)) {
            minCost += weight;
            edgesAdded++;

            if (edgesAdded == n - 1) {
                break;
            }
        }
    }

    return minCost;
}
```

#### 2. Find Redundant Connection (LeetCode #684)

**Problem:** In this problem, a tree is an undirected graph that is connected and has no cycles. You are given a graph that started as a tree with n nodes labeled from 1 to n, with one additional edge added. The added edge has two different vertices chosen from 1 to n, and was not an edge that already existed. Return an edge that can be removed so that the resulting graph is a tree of n nodes. If there are multiple answers, return the answer that occurs last in the input.

**Solution:**

```cpp
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size();

    // Initialize disjoint-set data structure
    std::vector<int> parent(n + 1);
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }

    // Find function with path compression
    std::function<int(int)> find = [&](int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    // Union function
    auto unionSets = [&](int x, int y) {
        parent[find(x)] = find(y);
    };

    // Process edges one by one
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];

        // If vertices are already in the same set, this edge creates a cycle
        if (find(u) == find(v)) {
            return edge;
        }

        // Otherwise, union the sets
        unionSets(u, v);
    }

    // No redundant connection found (shouldn't happen given problem constraints)
    return {};
}
```

### Medium:

#### 1. Connecting Cities With Minimum Cost (LeetCode #1135 - Premium)

**Problem:** There are n cities labeled from 1 to n. You are given the integer n and an array connections where connections[i] = [xi, yi, costi] indicates that the cost of connecting city xi and city yi (bidirectional connection) is costi. Return the minimum cost to connect all the n cities such that there is at least one path between each pair of cities. If it is impossible to connect all the n cities, return -1.

**Solution:**

```cpp
int minimumCost(int n, vector<vector<int>>& connections) {
    // Sort connections by cost
    std::sort(connections.begin(), connections.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[2] < b[2];
             });

    // Initialize disjoint-set data structure
    std::vector<int> parent(n + 1);
    std::vector<int> rank(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }

    // Find function with path compression
    std::function<int(int)> find = [&](int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    // Union function with rank
    auto unionSets = [&](int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false;
        }

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }

        return true;
    };

    // Kruskal's algorithm
    int minCost = 0;
    int edgesAdded = 0;

    for (const auto& conn : connections) {
        int u = conn[0];
        int v = conn[1];
        int cost = conn[2];

        if (unionSets(u, v)) {
            minCost += cost;
            edgesAdded++;

            if (edgesAdded == n - 1) {
                break;
            }
        }
    }

    // Check if all cities are connected
    return edgesAdded == n - 1 ? minCost : -1;
}
```

#### 2. Network Delay Time (LeetCode #743)

**Problem:** You are given a network of n nodes, labeled from 1 to n. You are also given a list of travel times as directed edges times[i] = (ui, vi, wi), where ui is the source node, vi is the target node, and wi is the time it takes for a signal to travel from source to target. We will send a signal from a given node k. Return the minimum time it takes for all the n nodes to receive the signal. If it is impossible for all the n nodes to receive the signal, return -1.

**Solution:**

```cpp
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    // Dijkstra's algorithm for shortest paths

    // Create adjacency list
    std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
    for (const auto& time : times) {
        int u = time[0];
        int v = time[1];
        int w = time[2];
        graph[u].push_back({v, w});
    }

    // Distance array
    std::vector<int> dist(n + 1, INT_MAX);
    dist[k] = 0;

    // Priority queue for Dijkstra (min-heap): (distance, node)
    std::priority_queue<std::pair<int, int>,
                       std::vector<std::pair<int, int>>,
                       std::greater<std::pair<int, int>>> pq;
    pq.push({0, k});

    // Processed set
    std::unordered_set<int> processed;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (processed.find(u) != processed.end()) {
            continue;
        }

        processed.insert(u);

        for (const auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    // Find the maximum distance (time for the signal to reach all nodes)
    int maxTime = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) {
            return -1; // Some node is unreachable
        }
        maxTime = std::max(maxTime, dist[i]);
    }

    return maxTime;
}
```

### Hard:

#### 1. Swim in Rising Water (LeetCode #778)

**Problem:** You are given an n x n integer matrix grid where each value grid[i][j] represents the elevation at that point (i, j). The rain starts to fall. At time t, the depth of the water everywhere is t. You can swim from a square to another 4-directionally adjacent square if and only if the elevation of both squares individually are at most t. You can swim infinite distances in zero time. Of course, you must stay within the boundaries of the grid during your swim. Return the least time until you can reach the bottom right square (n - 1, n - 1) if you start at the top left square (0, 0).

**Solution:**

```cpp
int swimInWater(vector<vector<int>>& grid) {
    int n = grid.size();

    // Create a list of all cells with their elevations
    std::vector<std::tuple<int, int, int>> cells; // (elevation, row, col)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cells.push_back({grid[i][j], i, j});
        }
    }

    // Sort cells by elevation
    std::sort(cells.begin(), cells.end());

    // Initialize disjoint-set data structure
    std::vector<int> parent(n * n);
    for (int i = 0; i < n * n; i++) {
        parent[i] = i;
    }

    // Find function with path compression
    std::function<int(int)> find = [&](int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    // Union function
    auto unionSets = [&](int x, int y) {
        parent[find(x)] = find(y);
    };

    // Direction arrays for 4-directional movement
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};

    // Process cells in order of increasing elevation
    for (const auto& [elevation, r, c] : cells) {
        int cellIndex = r * n + c;

        // Check adjacent cells
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Skip out of bounds
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
                continue;
            }

            int neighborIndex = nr * n + nc;

            // Only union with cells that have already been processed (elevation <= current)
            if (grid[nr][nc] <= elevation) {
                unionSets(cellIndex, neighborIndex);
            }
        }

        // Check if top-left and bottom-right are connected
        if (find(0) == find(n * n - 1)) {
            return elevation;
        }
    }

    return -1; // Should not reach here given problem constraints
}
```

#### 2. Minimum Cost to Connect Sticks (LeetCode #1167 - Premium)

**Problem:** You have some number of sticks with positive integer lengths. These lengths are given as an array sticks, where sticks[i] is the length of the ith stick. You can connect any two sticks of lengths x and y into one stick by paying a cost of x + y. You must connect all the sticks until there is only one stick remaining. Return the minimum cost of connecting all the sticks.

**Solution:**

```cpp
int connectSticks(vector<int>& sticks) {
    // Use a min-heap to always connect the two shortest sticks
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq(sticks.begin(), sticks.end());

    int totalCost = 0;

    // While there is more than one stick
    while (pq.size() > 1) {
        // Get the two shortest sticks
        int first = pq.top();
        pq.pop();

        int second = pq.top();
        pq.pop();

        // Combine them and add to the heap
        int cost = first + second;
        totalCost += cost;
        pq.push(cost);
    }

    return totalCost;
}
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll be exploring **Topological Sort & Directed Acyclic Graphs (DAGs)** — an essential concept for dependency resolution and scheduling problems. We'll dive into:

- The properties of Directed Acyclic Graphs
- Topological sorting algorithms (DFS-based and Kahn's algorithm)
- Applications in scheduling, build systems, and dependency resolution
- Detecting cycles in directed graphs
- Efficient implementations and optimizations

_If minimum spanning trees help us connect points with minimal total cost, topological sorting helps us understand and respect dependencies between tasks. Whether you're scheduling course prerequisites, managing software build processes, or resolving dependencies in a complex system, topological sorting provides a systematic way to handle these relationships. Join us tomorrow as we explore how to arrange tasks in a valid sequence that respects all dependencies!_

#DSAin45 #Day32 #MinimumSpanningTree #Kruskal #Prim #GraphAlgorithms #CPlusPlus #NetworkDesign
