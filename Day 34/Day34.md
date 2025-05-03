# Day 34: Advanced Graph Algorithms #DSAin45

Welcome to Day 34 of our #DSAin45 journey! Today we're diving deeper into the fascinating world of graph algorithms. We've already covered traversals (BFS/DFS), shortest paths, minimum spanning trees, and topological sorting. Now, we're ready to explore three powerful advanced graph concepts:

1. Strongly Connected Components (SCCs)
2. Articulation Points and Bridges
3. Introduction to Flow Networks

These algorithms help us analyze connectivity properties of graphs and solve complex network problems. Let's dive in!

## Strongly Connected Components (SCCs)

### What are Strongly Connected Components?

A **strongly connected component** (SCC) is a maximal subset of vertices in a directed graph where every vertex is reachable from every other vertex within the same subset. In simpler terms, if you pick any two vertices in an SCC, there's always a path from one to the other and vice versa.

Think of SCCs as "tightly knit communities" within a larger social network:

- Within each community, everyone can reach everyone else
- Between communities, paths may only exist in one direction or not at all

### Kosaraju's Algorithm for Finding SCCs

Kosaraju's algorithm is an elegant method for finding all SCCs in a directed graph, with a linear time complexity of O(V+E). It uses two depth-first search (DFS) traversals.

The algorithm works in three simple steps:

1. Perform DFS on the original graph to compute finish times (or create a stack of vertices)
2. Create the transpose graph (reverse all edges)
3. Perform DFS on the transpose graph, starting with vertices in decreasing order of finish time

#### Pseudocode for Kosaraju's Algorithm:

```
function findSCCs(graph):
    // Step 1: Perform DFS and store vertices in order of finishing time
    stack = empty stack
    visited = [false, false, ..., false] (size |V|)

    for each vertex v in graph:
        if not visited[v]:
            fillOrder(v, visited, stack)

    // Step 2: Create transpose graph (reverse all edges)
    transpose = reverseGraph(graph)

    // Step 3: Process vertices in order of decreasing finish time
    // Reset visited array
    visited = [false, false, ..., false] (size |V|)

    while stack is not empty:
        v = stack.pop()
        if not visited[v]:
            // Start a new SCC
            currentSCC = []
            DFSUtil(v, visited, currentSCC, transpose)
            print or store currentSCC

function fillOrder(v, visited, stack):
    visited[v] = true

    for each neighbor w of v:
        if not visited[w]:
            fillOrder(w, visited, stack)

    stack.push(v)

function DFSUtil(v, visited, component, graph):
    visited[v] = true
    component.add(v)

    for each neighbor w of v:
        if not visited[w]:
            DFSUtil(w, visited, component, graph)
```

### Tarjan's Algorithm for Finding SCCs

While Kosaraju's algorithm is simpler to understand, Tarjan's algorithm is often preferred in practice as it requires only one DFS traversal. It uses the concept of low-link values to identify SCCs.

The key insights of Tarjan's algorithm:

- Assign each vertex a unique discovery time ("disc") during DFS
- Track the lowest reachable discovery time ("low") for each vertex
- Use a stack to keep track of vertices in the current SCC candidate
- When disc[v] == low[v], we've found an SCC

#### Pseudocode for Tarjan's Algorithm:

```
function tarjanSCC(graph):
    disc = [-1, -1, ..., -1] (size |V|)
    low = [-1, -1, ..., -1] (size |V|)
    onStack = [false, false, ..., false] (size |V|)
    stack = empty stack
    time = 0

    for each vertex v in graph:
        if disc[v] == -1:
            tarjanDFS(v, disc, low, onStack, stack, time, graph)

function tarjanDFS(u, disc, low, onStack, stack, time, graph):
    disc[u] = low[u] = time
    time = time + 1
    stack.push(u)
    onStack[u] = true

    for each neighbor v of u:
        if disc[v] == -1:  // v is not visited
            tarjanDFS(v, disc, low, onStack, stack, time, graph)
            low[u] = min(low[u], low[v])
        else if onStack[v]:  // Back edge to vertex in current SCC
            low[u] = min(low[u], disc[v])

    // If u is the root of SCC
    if disc[u] == low[u]:
        // Start a new SCC
        component = []
        while true:
            v = stack.pop()
            onStack[v] = false
            component.add(v)
            if v == u:
                break
        print or store component
```

### Applications of Strongly Connected Components

1. **Social Network Analysis**: Identifying communities where information can flow freely
2. **Compiler Design**: Identifying cyclic dependencies for optimization
3. **URL Analysis**: Finding groups of pages that link to each other (like in PageRank)
4. **Circuit Design**: Identifying feedback paths in electronic circuits
5. **Game Theory**: Finding stable states in game dynamics

## Articulation Points and Bridges

### What are Articulation Points and Bridges?

In an undirected graph:

- An **articulation point** (or cut vertex) is a vertex whose removal increases the number of connected components in the graph
- A **bridge** (or cut edge) is an edge whose removal increases the number of connected components in the graph

Think of these as "weak points" in a network:

- Articulation points are like key intersections in a road network
- Bridges are like crucial roads connecting different regions

Identifying these critical elements helps in analyzing network reliability and designing resilient systems.

### Finding Articulation Points Using Tarjan's Algorithm

We can find articulation points efficiently using a modified DFS approach that tracks discovery times and low values (similar to Tarjan's SCC algorithm):

#### Pseudocode for Finding Articulation Points:

```
function findArticulationPoints(graph):
    n = number of vertices
    disc = [-1, -1, ..., -1] (size n)
    low = [-1, -1, ..., -1] (size n)
    parent = [-1, -1, ..., -1] (size n)
    articulation_points = empty set
    time = 0

    for each vertex v in graph:
        if disc[v] == -1:
            dfsArticulationPoints(v, disc, low, parent, articulation_points, time, graph)

    return articulation_points

function dfsArticulationPoints(u, disc, low, parent, articulation_points, time, graph):
    children = 0
    disc[u] = low[u] = time
    time = time + 1

    for each neighbor v of u:
        if disc[v] == -1:  // v is not visited
            parent[v] = u
            children = children + 1
            dfsArticulationPoints(v, disc, low, parent, articulation_points, time, graph)

            // Update low value of u
            low[u] = min(low[u], low[v])

            // Case 1: u is root of DFS tree and has multiple children
            if parent[u] == -1 and children > 1:
                articulation_points.add(u)

            // Case 2: u is not root and low value of one of its children is >= discovery value of u
            if parent[u] != -1 and low[v] >= disc[u]:
                articulation_points.add(u)

        else if v != parent[u]:  // Back edge
            low[u] = min(low[u], disc[v])
```

### Finding Bridges Using a Similar Approach

Bridges can be found using a similar DFS-based approach:

#### Pseudocode for Finding Bridges:

```
function findBridges(graph):
    n = number of vertices
    disc = [-1, -1, ..., -1] (size n)
    low = [-1, -1, ..., -1] (size n)
    parent = [-1, -1, ..., -1] (size n)
    bridges = empty list
    time = 0

    for each vertex v in graph:
        if disc[v] == -1:
            dfsBridges(v, disc, low, parent, bridges, time, graph)

    return bridges

function dfsBridges(u, disc, low, parent, bridges, time, graph):
    disc[u] = low[u] = time
    time = time + 1

    for each neighbor v of u:
        if disc[v] == -1:  // v is not visited
            parent[v] = u
            dfsBridges(v, disc, low, parent, bridges, time, graph)

            // Update low value of u
            low[u] = min(low[u], low[v])

            // If the lowest vertex reachable from subtree under v is below u, then u-v is a bridge
            if low[v] > disc[u]:
                bridges.add((u, v))

        else if v != parent[u]:  // Back edge
            low[u] = min(low[u], disc[v])
```

### Applications of Articulation Points and Bridges

1. **Network Reliability**: Identifying critical nodes/links in communication networks
2. **Transportation Planning**: Finding critical intersections/roads in a transportation network
3. **Supply Chain Analysis**: Identifying vulnerabilities in supply chains
4. **Biconnected Components**: Finding subgraphs with no articulation points (more resilient components)
5. **Circuit Design**: Identifying critical components in electrical circuits

## Introduction to Flow Networks

### What are Flow Networks?

A **flow network** is a directed graph where:

- Each edge has a capacity (maximum amount of flow it can handle)
- There's a source vertex (where flow originates)
- There's a sink vertex (where flow terminates)
- Flow is conserved at all vertices except source and sink (what flows in = what flows out)

Flow networks model many real-world scenarios involving transportation, routing, or allocation of resources.

### The Ford-Fulkerson Algorithm & Maximum Flow Problem

The **maximum flow problem** asks: what's the maximum amount of flow that can be sent from source to sink without exceeding any edge capacity?

The **Ford-Fulkerson algorithm** solves this problem using the concept of augmenting paths:

1. Start with zero flow on all edges
2. While there's an augmenting path from source to sink:
   - Find the bottleneck capacity along this path
   - Increase flow along the path by this bottleneck capacity
   - Update residual capacities of edges and add reverse edges
3. When no more augmenting paths exist, we've found the maximum flow

#### Pseudocode for Ford-Fulkerson Algorithm:

```
function fordFulkerson(graph, source, sink):
    // Initialize flow to 0
    flow = 0

    // Create residual graph
    residualGraph = copy of graph with all flows set to 0

    // While there's an augmenting path
    while path = findAugmentingPath(residualGraph, source, sink) exists:
        // Find bottleneck capacity
        bottleneck = minimum capacity along path

        // Update flow value
        flow = flow + bottleneck

        // Update residual capacities
        for each edge (u, v) in path:
            residualGraph[u][v] -= bottleneck  // Forward edge
            residualGraph[v][u] += bottleneck  // Reverse edge

    return flow

function findAugmentingPath(residualGraph, source, sink):
    // Can use BFS or DFS to find a path
    // BFS typically preferred for Edmonds-Karp implementation
    // Return the path if found, or null if no path exists
```

### Edmonds-Karp Algorithm

The **Edmonds-Karp algorithm** is an implementation of Ford-Fulkerson that uses BFS to find augmenting paths. This ensures that the algorithm runs in O(V·E²) time, making it more efficient for dense graphs.

### Applications of Flow Networks

1. **Transportation Networks**: Modeling capacity of roads, railroads, or air traffic routes
2. **Communication Networks**: Analyzing data flow capacity in computer networks
3. **Bipartite Matching**: Solving assignment problems (e.g., workers to jobs)
4. **Image Segmentation**: Using min-cut/max-flow for separating image regions
5. **Project Scheduling**: Handling resource allocation in project management
6. **Sports Tournament Scheduling**: Determining if certain outcomes are possible

## C++ Implementation

Let's implement these advanced graph algorithms in C++:

### Strongly Connected Components (Kosaraju's Algorithm)

```cpp
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Graph {
private:
    int V;
    vector<vector<int>> adj;

    // Fills Stack with vertices in order of finishing times
    void fillOrder(int v, vector<bool>& visited, stack<int>& Stack) {
        visited[v] = true;

        for (int i : adj[v]) {
            if (!visited[i]) {
                fillOrder(i, visited, Stack);
            }
        }

        Stack.push(v);
    }

    // DFS starting from v
    void DFSUtil(int v, vector<bool>& visited, vector<int>& component) {
        visited[v] = true;
        component.push_back(v);

        for (int i : adj[v]) {
            if (!visited[i]) {
                DFSUtil(i, visited, component);
            }
        }
    }

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    // Returns transpose (or reverse) of this graph
    Graph getTranspose() {
        Graph g(V);
        for (int v = 0; v < V; v++) {
            for (int i : adj[v]) {
                g.addEdge(i, v);
            }
        }
        return g;
    }

    // Find all strongly connected components
    vector<vector<int>> findSCCs() {
        vector<vector<int>> SCCs;
        stack<int> Stack;

        // Mark all vertices as not visited (for first DFS)
        vector<bool> visited(V, false);

        // Fill vertices in stack according to their finishing times
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                fillOrder(i, visited, Stack);
            }
        }

        // Create a reversed graph
        Graph gr = getTranspose();

        // Mark all vertices as not visited (for second DFS)
        fill(visited.begin(), visited.end(), false);

        // Process all vertices in order defined by Stack
        while (!Stack.empty()) {
            int v = Stack.top();
            Stack.pop();

            if (!visited[v]) {
                vector<int> component;
                gr.DFSUtil(v, visited, component);
                SCCs.push_back(component);
            }
        }

        return SCCs;
    }
};
```

### Articulation Points and Bridges

```cpp
#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Graph {
private:
    int V;
    vector<vector<int>> adj;

    void APUtil(int u, vector<bool>& visited, vector<int>& disc, vector<int>& low,
                vector<int>& parent, set<int>& articulationPoints) {
        static int time = 0;
        int children = 0;

        visited[u] = true;
        disc[u] = low[u] = ++time;

        for (int v : adj[u]) {
            if (!visited[v]) {
                children++;
                parent[v] = u;
                APUtil(v, visited, disc, low, parent, articulationPoints);

                // Check if subtree rooted with v has a connection to one of the ancestors of u
                low[u] = min(low[u], low[v]);

                // Case 1: u is root of DFS tree and has two or more children
                if (parent[u] == -1 && children > 1) {
                    articulationPoints.insert(u);
                }

                // Case 2: u is not root and low value of one of its children is more than or equal to discovery value of u
                if (parent[u] != -1 && low[v] >= disc[u]) {
                    articulationPoints.insert(u);
                }
            }
            else if (v != parent[u]) {
                // Update low value of u for parent function calls
                low[u] = min(low[u], disc[v]);
            }
        }
    }

    void bridgeUtil(int u, vector<bool>& visited, vector<int>& disc, vector<int>& low,
                   vector<int>& parent, vector<pair<int, int>>& bridges) {
        static int time = 0;

        visited[u] = true;
        disc[u] = low[u] = ++time;

        for (int v : adj[u]) {
            if (!visited[v]) {
                parent[v] = u;
                bridgeUtil(v, visited, disc, low, parent, bridges);

                // Check if the subtree rooted with v has a connection to one of the ancestors of u
                low[u] = min(low[u], low[v]);

                // If the lowest vertex reachable from subtree under v is below u in DFS tree, then u-v is a bridge
                if (low[v] > disc[u]) {
                    bridges.push_back({u, v});
                }
            }
            else if (v != parent[u]) {
                // Update low value of u for parent function calls
                low[u] = min(low[u], disc[v]);
            }
        }
    }

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v); // Undirected graph
    }

    // Find all articulation points
    set<int> findArticulationPoints() {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<int> parent(V, -1);
        vector<bool> visited(V, false);
        set<int> articulationPoints;

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                APUtil(i, visited, disc, low, parent, articulationPoints);
            }
        }

        return articulationPoints;
    }

    // Find all bridges
    vector<pair<int, int>> findBridges() {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<int> parent(V, -1);
        vector<bool> visited(V, false);
        vector<pair<int, int>> bridges;

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                bridgeUtil(i, visited, disc, low, parent, bridges);
            }
        }

        return bridges;
    }
};
```

### Ford-Fulkerson Algorithm for Maximum Flow

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

class FlowNetwork {
private:
    int V;
    vector<vector<int>> residualGraph;

    // Returns true if there is a path from source to sink in the residual graph
    bool bfs(int source, int sink, vector<int>& parent) {
        vector<bool> visited(V, false);
        queue<int> q;

        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && residualGraph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return visited[sink];
    }

public:
    FlowNetwork(int V) {
        this->V = V;
        residualGraph.resize(V, vector<int>(V, 0));
    }

    void addEdge(int u, int v, int capacity) {
        residualGraph[u][v] = capacity;
    }

    // Returns the maximum flow from source to sink
    int fordFulkerson(int source, int sink) {
        vector<int> parent(V);
        int maxFlow = 0;

        // Augment the flow while there is a path from source to sink
        while (bfs(source, sink, parent)) {
            // Find minimum residual capacity of the edges along the path
            int pathFlow = numeric_limits<int>::max();
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, residualGraph[u][v]);
            }

            // Update residual capacities of the edges and reverse edges along the path
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= pathFlow;
                residualGraph[v][u] += pathFlow; // Add reverse edge
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }
};
```

## Time and Space Complexity Analysis

| Algorithm                   | Time Complexity | Space Complexity |
| --------------------------- | --------------- | ---------------- |
| Kosaraju's Algorithm (SCCs) | O(V + E)        | O(V)             |
| Tarjan's Algorithm (SCCs)   | O(V + E)        | O(V)             |
| Finding Articulation Points | O(V + E)        | O(V)             |
| Finding Bridges             | O(V + E)        | O(V)             |
| Ford-Fulkerson (Max Flow)   | O(E \* maxFlow) | O(V^2)           |
| Edmonds-Karp (Max Flow)     | O(V \* E^2)     | O(V^2)           |

## Real-World Applications and Examples

### Strongly Connected Components

1. **Web Pages Analysis**: Google's PageRank algorithm uses SCCs to identify groups of pages that link to each other, helping to determine page importance.

2. **Social Network Communities**: Identifying clusters of users who interact frequently with each other.

```
Example: Consider a Twitter-like social network:
- People in the same SCC can all see each other's content through follows
- Content cannot escape an SCC if there are no outgoing follows
- This helps identify echo chambers and content distribution patterns
```

3. **Compiler Optimization**: Finding circular dependencies in code modules.

### Articulation Points and Bridges

1. **Network Reliability Assessment**: Identifying critical nodes that, if removed, would disconnect parts of the network.

```
Example: In a telecommunications network:
- Articulation points represent critical routers/switches
- Bridges represent critical links between network segments
- Network engineers can use this information to add redundant paths
```

2. **Vulnerability Analysis**: Finding critical points in infrastructure networks.

3. **Biology**: Analyzing protein interaction networks to find critical proteins.

### Flow Networks

1. **Transportation Systems**: Analyzing maximum throughput in road, rail, or air traffic networks.

```
Example: Urban traffic flow:
- Roads are edges with capacity based on lanes and speed limits
- Intersections are vertices
- Maximum flow shows the theoretical maximum traffic throughput
- Min-cut identifies bottlenecks that should be addressed first
```

2. **Assignment Problems**: Matching workers to jobs, students to dorms, etc.

3. **Image Segmentation**: Using min-cut/max-flow to separate foreground from background.

## The Graph Algorithm Joke Book

**Strongly Connected Components**:
"I tried to break up with my partner, but we're in the same strongly connected component. No matter how I try to distance myself, there's always a path back to me!"

**Articulation Points**:
"Why did the vertex become a therapist? It was good at keeping graph components together!"

**Flow Networks**:
"What did the source vertex say to the sink? 'Everything that leaves me eventually comes to you!'"

## Practice Problems from LeetCode

### Easy:

- [997. Find the Town Judge](https://leetcode.com/problems/find-the-town-judge/): Similar to analyzing in-degree and out-degree in a graph.

### Medium:

- [1192. Critical Connections in a Network](https://leetcode.com/problems/critical-connections-in-a-network/): Find all bridges in an undirected graph.
- [1557. Minimum Number of Vertices to Reach All Nodes](https://leetcode.com/problems/minimum-number-of-vertices-to-reach-all-nodes/): Related to connectivity concepts.

### Hard:

- [1568. Minimum Number of Days to Disconnect Island](https://leetcode.com/problems/minimum-number-of-days-to-disconnect-island/): Involves finding articulation points.
- [2360. Longest Cycle in a Graph](https://leetcode.com/problems/longest-cycle-in-a-graph/): Related to SCCs.

## Preview of Day 35

Tomorrow, we'll apply all the graph algorithms we've learned to solve classic graph problems. We'll work on optimization techniques, investigate real-world graph applications, and participate in a graph algorithm challenge to test our understanding. Get ready to put your knowledge into practice!

## Conclusion

Advanced graph algorithms allow us to analyze complex network properties and solve sophisticated problems. Strongly connected components help us identify tightly linked communities, articulation points and bridges reveal network vulnerabilities, and flow networks enable us to optimize resource allocation and throughput.

These algorithms form the backbone of many modern applications in transportation, communication, social network analysis, and more. By understanding how to identify and leverage these graph properties, you'll be equipped to tackle a wide range of problems in computer science and beyond.

Keep practicing, and see you tomorrow for our weekly challenge and graph problem-solving session!

#DSAin45 #Algorithms #GraphTheory #CPlusPlus #Programming #SCC #FlowNetworks
