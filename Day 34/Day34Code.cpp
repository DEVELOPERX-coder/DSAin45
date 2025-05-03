/**
 * Advanced Graph Algorithms - Day 34 of #DSAin45
 *
 * This file implements:
 * 1. Strongly Connected Components (Kosaraju's and Tarjan's algorithms)
 * 2. Articulation Points and Bridges
 * 3. Flow Networks (Ford-Fulkerson and Edmonds-Karp algorithms)
 *
 * Author: #DSAin45
 * Date: May 2025
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// ===================================================
// 1. STRONGLY CONNECTED COMPONENTS
// ===================================================

/**
 * DirectedGraph class for SCC algorithms
 */
class DirectedGraph
{
private:
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    // Kosaraju's Algorithm helpers

    // Fill order of vertices in stack according to their finishing times
    void fillOrder(int v, vector<bool> &visited, stack<int> &Stack)
    {
        visited[v] = true;

        // Recur for all adjacent vertices
        for (int neighbor : adj[v])
        {
            if (!visited[neighbor])
            {
                fillOrder(neighbor, visited, Stack);
            }
        }

        // Push current vertex to stack when all its adjacent vertices are processed
        Stack.push(v);
    }

    // DFS traversal to find components
    void DFSUtil(int v, vector<bool> &visited, vector<int> &component)
    {
        visited[v] = true;
        component.push_back(v);

        // Recur for all adjacent vertices
        for (int neighbor : adj[v])
        {
            if (!visited[neighbor])
            {
                DFSUtil(neighbor, visited, component);
            }
        }
    }

    // Tarjan's Algorithm helpers

    // Recursive function for Tarjan's SCC algorithm
    void tarjanSCCUtil(int u, vector<int> &disc, vector<int> &low,
                       stack<int> &st, vector<bool> &stackMember,
                       vector<vector<int>> &SCCs, int &time)
    {
        // Initialize discovery time and low value
        disc[u] = low[u] = ++time;
        st.push(u);
        stackMember[u] = true;

        // Go through all adjacent vertices
        for (int v : adj[u])
        {
            // If v is not visited yet, recur for it
            if (disc[v] == -1)
            {
                tarjanSCCUtil(v, disc, low, st, stackMember, SCCs, time);

                // Check if the subtree rooted with v has a connection to one of the ancestors of u
                low[u] = min(low[u], low[v]);
            }
            // If v is on stack, update low value of u
            else if (stackMember[v])
            {
                low[u] = min(low[u], disc[v]);
            }
        }

        // Head of SCC found, pop the stack and print SCC
        if (low[u] == disc[u])
        {
            vector<int> component;
            int w;
            do
            {
                w = st.top();
                st.pop();
                stackMember[w] = false;
                component.push_back(w);
            } while (w != u);

            SCCs.push_back(component);
        }
    }

public:
    // Constructor
    DirectedGraph(int vertices)
    {
        V = vertices;
        adj.resize(V);
    }

    // Add a directed edge
    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
    }

    // Get transpose of the graph (reverse all edges)
    DirectedGraph getTranspose()
    {
        DirectedGraph g(V);
        for (int v = 0; v < V; v++)
        {
            for (int neighbor : adj[v])
            {
                g.addEdge(neighbor, v);
            }
        }
        return g;
    }

    // Print adjacency list representation of graph
    void printGraph()
    {
        for (int v = 0; v < V; v++)
        {
            cout << "Vertex " << v << " -> ";
            for (int neighbor : adj[v])
            {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Kosaraju's algorithm to find strongly connected components
    vector<vector<int>> kosarajuSCC()
    {
        vector<vector<int>> SCCs;

        // Create a stack for DFS
        stack<int> Stack;

        // Mark all vertices as not visited (for first DFS)
        vector<bool> visited(V, false);

        // Fill vertices in stack according to their finishing times
        for (int i = 0; i < V; i++)
        {
            if (!visited[i])
            {
                fillOrder(i, visited, Stack);
            }
        }

        // Create a reversed graph
        DirectedGraph gr = getTranspose();

        // Mark all vertices as not visited (for second DFS)
        fill(visited.begin(), visited.end(), false);

        // Process all vertices in order defined by Stack
        while (!Stack.empty())
        {
            int v = Stack.top();
            Stack.pop();

            // If not visited, it's a new SCC
            if (!visited[v])
            {
                vector<int> component;
                gr.DFSUtil(v, visited, component);
                SCCs.push_back(component);
            }
        }

        return SCCs;
    }

    // Tarjan's algorithm to find strongly connected components
    vector<vector<int>> tarjanSCC()
    {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<bool> stackMember(V, false);
        stack<int> st;
        vector<vector<int>> SCCs;
        int time = 0;

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++)
        {
            if (disc[i] == -1)
            {
                tarjanSCCUtil(i, disc, low, st, stackMember, SCCs, time);
            }
        }

        return SCCs;
    }

    // Check if the graph has a cycle
    bool hasCycle()
    {
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);

        for (int i = 0; i < V; i++)
        {
            if (!visited[i])
            {
                if (hasCycleUtil(i, visited, recStack))
                {
                    return true;
                }
            }
        }

        return false;
    }

    // Utility function for cycle detection
    bool hasCycleUtil(int v, vector<bool> &visited, vector<bool> &recStack)
    {
        if (!visited[v])
        {
            visited[v] = true;
            recStack[v] = true;

            for (int neighbor : adj[v])
            {
                if (!visited[neighbor] && hasCycleUtil(neighbor, visited, recStack))
                {
                    return true;
                }
                else if (recStack[neighbor])
                {
                    return true;
                }
            }
        }

        recStack[v] = false;
        return false;
    }

    // Return the condensation graph (SCCs as single nodes)
    pair<DirectedGraph, vector<vector<int>>> getCondensationGraph()
    {
        // Find all SCCs
        vector<vector<int>> SCCs = kosarajuSCC();

        // Create a map from original vertex to SCC id
        vector<int> sccId(V, -1);
        for (int i = 0; i < SCCs.size(); i++)
        {
            for (int v : SCCs[i])
            {
                sccId[v] = i;
            }
        }

        // Create the condensation graph
        DirectedGraph condensation(SCCs.size());
        set<pair<int, int>> condensedEdges;

        // Add edges between SCCs
        for (int v = 0; v < V; v++)
        {
            for (int neighbor : adj[v])
            {
                if (sccId[v] != sccId[neighbor])
                {
                    condensedEdges.insert({sccId[v], sccId[neighbor]});
                }
            }
        }

        // Add collected edges to the condensation graph
        for (auto &edge : condensedEdges)
        {
            condensation.addEdge(edge.first, edge.second);
        }

        return {condensation, SCCs};
    }
};

// ===================================================
// 2. ARTICULATION POINTS AND BRIDGES
// ===================================================

/**
 * UndirectedGraph class for articulation points and bridges
 */
class UndirectedGraph
{
private:
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    // Recursive function to find articulation points
    void APUtil(int u, vector<bool> &visited, vector<int> &disc,
                vector<int> &low, vector<int> &parent, set<int> &artPoints)
    {
        static int time = 0;
        int children = 0;

        // Mark the current node as visited
        visited[u] = true;

        // Initialize discovery time and low value
        disc[u] = low[u] = ++time;

        // Go through all neighbors
        for (int v : adj[u])
        {
            // If v is not visited yet
            if (!visited[v])
            {
                children++;
                parent[v] = u;

                // Recursive call
                APUtil(v, visited, disc, low, parent, artPoints);

                // Check if the subtree rooted with v has a connection to one of the ancestors of u
                low[u] = min(low[u], low[v]);

                // Case 1: u is root of DFS tree and has two or more children
                if (parent[u] == -1 && children > 1)
                {
                    artPoints.insert(u);
                }

                // Case 2: u is not root and low value of one of its children is more than or equal to discovery value of u
                if (parent[u] != -1 && low[v] >= disc[u])
                {
                    artPoints.insert(u);
                }
            }
            // Update low value of u for parent function calls
            else if (v != parent[u])
            {
                low[u] = min(low[u], disc[v]);
            }
        }
    }

    // Recursive function to find bridges
    void bridgeUtil(int u, vector<bool> &visited, vector<int> &disc,
                    vector<int> &low, vector<int> &parent, vector<pair<int, int>> &bridges)
    {
        static int time = 0;

        // Mark the current node as visited
        visited[u] = true;

        // Initialize discovery time and low value
        disc[u] = low[u] = ++time;

        // Go through all neighbors
        for (int v : adj[u])
        {
            // If v is not visited yet
            if (!visited[v])
            {
                parent[v] = u;

                // Recursive call
                bridgeUtil(v, visited, disc, low, parent, bridges);

                // Check if the subtree rooted with v has a connection to one of the ancestors of u
                low[u] = min(low[u], low[v]);

                // If the lowest vertex reachable from subtree under v is below u in DFS tree, then u-v is a bridge
                if (low[v] > disc[u])
                {
                    bridges.push_back({u, v});
                }
            }
            // Update low value of u for parent function calls
            else if (v != parent[u])
            {
                low[u] = min(low[u], disc[v]);
            }
        }
    }

    // DFS traversal to find biconnected components
    void BCCUtil(int u, vector<int> &disc, vector<int> &low,
                 stack<pair<int, int>> &st, vector<vector<int>> &BCCs)
    {
        static int time = 0;

        // Initialize discovery time and low value
        disc[u] = low[u] = ++time;
        int children = 0;

        // Go through all neighbors
        for (int v : adj[u])
        {
            // If v is not visited yet
            if (disc[v] == -1)
            {
                children++;

                // Store the edge in stack
                st.push({u, v});
                BCCUtil(v, disc, low, st, BCCs);

                // Check if the subtree rooted with v has a connection to one of the ancestors of u
                low[u] = min(low[u], low[v]);

                // If u is an articulation point, pop all edges from stack till u-v
                if ((disc[u] == 1 && children > 1) || (disc[u] > 1 && low[v] >= disc[u]))
                {
                    vector<int> component;
                    set<int> vertices;

                    while (!st.empty() && !(st.top().first == u && st.top().second == v))
                    {
                        vertices.insert(st.top().first);
                        vertices.insert(st.top().second);
                        st.pop();
                    }

                    if (!st.empty())
                    {
                        vertices.insert(st.top().first);
                        vertices.insert(st.top().second);
                        st.pop();
                    }

                    // Convert set to vector
                    component.assign(vertices.begin(), vertices.end());
                    BCCs.push_back(component);
                }
            }
            // If v is already visited and not parent of u
            else if (v != low[u] && disc[v] < disc[u])
            {
                low[u] = min(low[u], disc[v]);
                st.push({u, v});
            }
        }
    }

public:
    // Constructor
    UndirectedGraph(int vertices)
    {
        V = vertices;
        adj.resize(V);
    }

    // Add an undirected edge
    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    // Print adjacency list representation of graph
    void printGraph()
    {
        for (int v = 0; v < V; v++)
        {
            cout << "Vertex " << v << " -> ";
            for (int neighbor : adj[v])
            {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Find all articulation points
    set<int> findArticulationPoints()
    {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<int> parent(V, -1);
        vector<bool> visited(V, false);
        set<int> artPoints;

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++)
        {
            if (!visited[i])
            {
                APUtil(i, visited, disc, low, parent, artPoints);
            }
        }

        return artPoints;
    }

    // Find all bridges
    vector<pair<int, int>> findBridges()
    {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<int> parent(V, -1);
        vector<bool> visited(V, false);
        vector<pair<int, int>> bridges;

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++)
        {
            if (!visited[i])
            {
                bridgeUtil(i, visited, disc, low, parent, bridges);
            }
        }

        return bridges;
    }

    // Find all biconnected components
    vector<vector<int>> findBiconnectedComponents()
    {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        stack<pair<int, int>> st;
        vector<vector<int>> BCCs;

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++)
        {
            if (disc[i] == -1)
            {
                BCCUtil(i, disc, low, st, BCCs);

                // If stack is not empty, then one more BCC exists
                vector<int> component;
                set<int> vertices;

                while (!st.empty())
                {
                    vertices.insert(st.top().first);
                    vertices.insert(st.top().second);
                    st.pop();
                }

                if (!vertices.empty())
                {
                    component.assign(vertices.begin(), vertices.end());
                    BCCs.push_back(component);
                }
            }
        }

        return BCCs;
    }

    // Check if removing a vertex would disconnect the graph
    bool isArticulationPoint(int vertex)
    {
        set<int> artPoints = findArticulationPoints();
        return artPoints.find(vertex) != artPoints.end();
    }

    // Check if removing an edge would disconnect the graph
    bool isBridge(int u, int v)
    {
        vector<pair<int, int>> bridges = findBridges();
        return find(bridges.begin(), bridges.end(), make_pair(u, v)) != bridges.end() ||
               find(bridges.begin(), bridges.end(), make_pair(v, u)) != bridges.end();
    }

    // Return the resilience score (higher is better)
    // Defined as: 1 - (num_articulation_points / num_vertices)
    double getResilienceScore()
    {
        set<int> artPoints = findArticulationPoints();
        return 1.0 - (double)artPoints.size() / V;
    }
};

// ===================================================
// 3. FLOW NETWORKS
// ===================================================

/**
 * FlowNetwork class for maximum flow algorithms
 */
class FlowNetwork
{
private:
    int V;                        // Number of vertices
    vector<vector<int>> capacity; // Capacity of each edge
    vector<vector<int>> flow;     // Flow of each edge

    // Returns true if there is a path from source to sink in residual graph
    // Also fills parent[] to store the path
    bool bfs(int source, int sink, vector<int> &parent)
    {
        // Create a visited array and mark all vertices as not visited
        vector<bool> visited(V, false);

        // Create a queue, enqueue source vertex and mark source vertex as visited
        queue<int> q;
        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        // Standard BFS loop
        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            // Check all adjacent vertices of u
            for (int v = 0; v < V; v++)
            {
                // If vertex v is not visited and there is residual capacity
                if (!visited[v] && capacity[u][v] - flow[u][v] > 0)
                {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        // If we reached sink in BFS, return true, else return false
        return visited[sink];
    }

    // Returns true if there is a path from source to sink in residual graph
    // Uses DFS instead of BFS
    bool dfs(int u, int sink, vector<bool> &visited, vector<int> &parent)
    {
        // Mark the current node as visited
        visited[u] = true;

        // If we reached sink, return true
        if (u == sink)
            return true;

        // Check all adjacent vertices of u
        for (int v = 0; v < V; v++)
        {
            // If vertex v is not visited and there is residual capacity
            if (!visited[v] && capacity[u][v] - flow[u][v] > 0)
            {
                parent[v] = u;

                // If DFS from v to sink returns true, we found a path
                if (dfs(v, sink, visited, parent))
                {
                    return true;
                }
            }
        }

        return false;
    }

public:
    // Constructor
    FlowNetwork(int vertices)
    {
        V = vertices;
        capacity.resize(V, vector<int>(V, 0));
        flow.resize(V, vector<int>(V, 0));
    }

    // Add a directed edge with given capacity
    void addEdge(int u, int v, int cap)
    {
        capacity[u][v] = cap;
    }

    // Print the flow network
    void printNetwork()
    {
        cout << "Capacity Matrix:" << endl;
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                cout << setw(4) << capacity[i][j] << " ";
            }
            cout << endl;
        }

        cout << "\nFlow Matrix:" << endl;
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                cout << setw(4) << flow[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Ford-Fulkerson algorithm for maximum flow
    int fordFulkerson(int source, int sink)
    {
        int maxFlow = 0;
        vector<int> parent(V);
        vector<bool> visited(V);

        // While there is a path from source to sink
        while (bfs(source, sink, parent))
        {
            // Find the maximum flow through the path found
            int pathFlow = numeric_limits<int>::max();

            // Find the minimum residual capacity of the edges along the path
            for (int v = sink; v != source; v = parent[v])
            {
                int u = parent[v];
                pathFlow = min(pathFlow, capacity[u][v] - flow[u][v]);
            }

            // Update residual capacities of the edges and reverse edges along the path
            for (int v = sink; v != source; v = parent[v])
            {
                int u = parent[v];
                flow[u][v] += pathFlow;
                flow[v][u] -= pathFlow; // Reverse edge
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    // Edmonds-Karp algorithm for maximum flow (uses BFS for finding paths)
    int edmondsKarp(int source, int sink)
    {
        // Same as Ford-Fulkerson but always uses BFS
        return fordFulkerson(source, sink);
    }

    // Dinic's algorithm for maximum flow (faster than Edmonds-Karp)
    int dinic(int source, int sink)
    {
        int maxFlow = 0;

        while (true)
        {
            // Construct level graph using BFS
            vector<int> level(V, -1);
            level[source] = 0;
            queue<int> q;
            q.push(source);

            while (!q.empty())
            {
                int u = q.front();
                q.pop();

                for (int v = 0; v < V; v++)
                {
                    if (level[v] < 0 && capacity[u][v] - flow[u][v] > 0)
                    {
                        level[v] = level[u] + 1;
                        q.push(v);
                    }
                }
            }

            // If we can't reach sink, we're done
            if (level[sink] < 0)
                break;

            // Use DFS to find blocking flows
            vector<int> ptr(V, 0); // Current edge pointers

            function<int(int, int)> sendFlow = [&](int u, int flowSoFar) -> int
            {
                if (u == sink)
                    return flowSoFar;

                for (int &i = ptr[u]; i < V; i++)
                {
                    int v = i;

                    if (level[v] == level[u] + 1 && capacity[u][v] - flow[u][v] > 0)
                    {
                        int currFlow = min(flowSoFar, capacity[u][v] - flow[u][v]);
                        int tempFlow = sendFlow(v, currFlow);

                        if (tempFlow > 0)
                        {
                            flow[u][v] += tempFlow;
                            flow[v][u] -= tempFlow;
                            return tempFlow;
                        }
                    }
                }

                return 0;
            };

            // Send flow while possible
            while (int addedFlow = sendFlow(source, numeric_limits<int>::max()))
            {
                maxFlow += addedFlow;
            }
        }

        return maxFlow;
    }

    // Find the min-cut edges
    vector<pair<int, int>> getMinCutEdges(int source)
    {
        vector<pair<int, int>> minCutEdges;
        vector<bool> visited(V, false);

        // Perform DFS from source
        function<void(int)> dfs = [&](int u)
        {
            visited[u] = true;

            for (int v = 0; v < V; v++)
            {
                if (!visited[v] && capacity[u][v] - flow[u][v] > 0)
                {
                    dfs(v);
                }
            }
        };

        dfs(source);

        // Find all edges from visited to not visited vertices
        for (int u = 0; u < V; u++)
        {
            if (visited[u])
            {
                for (int v = 0; v < V; v++)
                {
                    if (!visited[v] && capacity[u][v] > 0)
                    {
                        minCutEdges.push_back({u, v});
                    }
                }
            }
        }

        return minCutEdges;
    }

    // Get the current flow matrix
    vector<vector<int>> getFlowMatrix()
    {
        return flow;
    }

    // Reset all flows to zero
    void resetFlow()
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                flow[i][j] = 0;
            }
        }
    }
};

// ===================================================
// UTILITY FUNCTIONS
// ===================================================

// Print vectors nicely
template <typename T>
void printVector(const vector<T> &vec, const string &label = "Vector")
{
    cout << label << ": ";
    for (const T &item : vec)
    {
        cout << item << " ";
    }
    cout << endl;
}

// Print vector of vectors
template <typename T>
void printVectorOfVectors(const vector<vector<T>> &vec, const string &label = "Vector of Vectors")
{
    cout << label << ":" << endl;
    for (int i = 0; i < vec.size(); i++)
    {
        cout << i << ": ";
        for (const T &item : vec[i])
        {
            cout << item << " ";
        }
        cout << endl;
    }
}

// Print set
template <typename T>
void printSet(const set<T> &s, const string &label = "Set")
{
    cout << label << ": ";
    for (const T &item : s)
    {
        cout << item << " ";
    }
    cout << endl;
}

// Print pairs
template <typename T1, typename T2>
void printPairs(const vector<pair<T1, T2>> &pairs, const string &label = "Pairs")
{
    cout << label << ":" << endl;
    for (const auto &p : pairs)
    {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
}

// Timing function
template <typename Func>
double timeFunction(Func func)
{
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
}

// ===================================================
// EXAMPLES
// ===================================================

/**
 * Example demonstrating strongly connected components
 */
void demonstrateSCC()
{
    cout << "\n===== STRONGLY CONNECTED COMPONENTS =====\n";

    // Create a directed graph
    DirectedGraph g(8);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 0);
    g.addEdge(2, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);
    g.addEdge(6, 4);
    g.addEdge(6, 7);

    cout << "Graph representation:" << endl;
    g.printGraph();

    // Find SCCs using Kosaraju's algorithm
    auto kosarajuTime = timeFunction([&]
                                     {
         vector<vector<int>> sccs = g.kosarajuSCC();
         cout << "\nStrongly Connected Components (Kosaraju's algorithm):" << endl;
         printVectorOfVectors(sccs, "SCCs"); });

    // Find SCCs using Tarjan's algorithm
    auto tarjanTime = timeFunction([&]
                                   {
         vector<vector<int>> sccs = g.tarjanSCC();
         cout << "\nStrongly Connected Components (Tarjan's algorithm):" << endl;
         printVectorOfVectors(sccs, "SCCs"); });

    cout << "\nPerformance comparison:" << endl;
    cout << "Kosaraju's algorithm: " << kosarajuTime << " ms" << endl;
    cout << "Tarjan's algorithm: " << tarjanTime << " ms" << endl;

    // Get condensation graph
    auto [condensation, SCCs] = g.getCondensationGraph();
    cout << "\nCondensation graph (SCCs as single nodes):" << endl;
    condensation.printGraph();

    cout << "\nSCC mapping:" << endl;
    for (int i = 0; i < SCCs.size(); i++)
    {
        cout << "SCC " << i << ": ";
        for (int v : SCCs[i])
        {
            cout << v << " ";
        }
        cout << endl;
    }

    // Real-world SCC example: Web analysis
    cout << "\nReal-world example - Web page communities:" << endl;
    DirectedGraph web(6);
    web.addEdge(0, 1); // A links to B
    web.addEdge(1, 2); // B links to C
    web.addEdge(2, 0); // C links to A
    web.addEdge(3, 4); // D links to E
    web.addEdge(4, 5); // E links to F
    web.addEdge(5, 3); // F links to D
    web.addEdge(2, 3); // C links to D
    web.addEdge(1, 5); // B links to F

    vector<vector<int>> webSCCs = web.kosarajuSCC();
    cout << "Web communities:" << endl;
    printVectorOfVectors(webSCCs, "Communities");

    cout << "Interpretation: Pages within the same community all link to each other (directly or indirectly),\n"
         << "forming tightly connected subgroups in the web graph."
         << endl;
}

/**
 * Example demonstrating articulation points and bridges
 */
void demonstrateArticulationPointsAndBridges()
{
    cout << "\n===== ARTICULATION POINTS AND BRIDGES =====\n";

    // Create an undirected graph
    UndirectedGraph g(7);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);

    cout << "Graph representation:" << endl;
    g.printGraph();

    // Find articulation points
    auto apTime = timeFunction([&]
                               {
         set<int> artPoints = g.findArticulationPoints();
         printSet(artPoints, "\nArticulation Points"); });

    // Find bridges
    auto bridgeTime = timeFunction([&]
                                   {
         vector<pair<int, int>> bridges = g.findBridges();
         printPairs(bridges, "\nBridges"); });

    cout << "\nPerformance:" << endl;
    cout << "Articulation Points: " << apTime << " ms" << endl;
    cout << "Bridges: " << bridgeTime << " ms" << endl;

    // Find biconnected components
    vector<vector<int>> bccs = g.findBiconnectedComponents();
    printVectorOfVectors(bccs, "\nBiconnected Components");

    // Calculate resilience score
    double resilienceScore = g.getResilienceScore();
    cout << "\nNetwork Resilience Score: " << resilienceScore << endl;
    cout << "Interpretation: A score closer to 1 means higher resilience (fewer critical points)." << endl;

    // Real-world example: Network reliability
    cout << "\nReal-world example - Computer Network:" << endl;
    UndirectedGraph network(8);
    network.addEdge(0, 1); // Server connections
    network.addEdge(1, 2);
    network.addEdge(2, 3);
    network.addEdge(3, 4);
    network.addEdge(4, 5);
    network.addEdge(5, 0);
    network.addEdge(0, 6);
    network.addEdge(3, 7);

    set<int> criticalServers = network.findArticulationPoints();
    printSet(criticalServers, "Critical Servers (Articulation Points)");

    vector<pair<int, int>> criticalLinks = network.findBridges();
    printPairs(criticalLinks, "Critical Links (Bridges)");

    cout << "Interpretation: Critical servers and links should have backup systems to prevent network failure."
         << endl;
}

/**
 * Example demonstrating flow networks
 */
void demonstrateFlowNetworks()
{
    cout << "\n===== FLOW NETWORKS =====\n";

    // Create a flow network
    FlowNetwork network(6);

    // Add edges with capacities
    network.addEdge(0, 1, 16);
    network.addEdge(0, 2, 13);
    network.addEdge(1, 2, 10);
    network.addEdge(1, 3, 12);
    network.addEdge(2, 1, 4);
    network.addEdge(2, 4, 14);
    network.addEdge(3, 2, 9);
    network.addEdge(3, 5, 20);
    network.addEdge(4, 3, 7);
    network.addEdge(4, 5, 4);

    cout << "Flow Network before flow calculation:" << endl;
    network.printNetwork();

    // Find maximum flow using Ford-Fulkerson
    int source = 0;
    int sink = 5;
    auto ffTime = timeFunction([&]
                               {
         network.resetFlow();
         int maxFlow = network.fordFulkerson(source, sink);
         cout << "\nMaximum flow from " << source << " to " << sink 
              << " using Ford-Fulkerson: " << maxFlow << endl; });

    // Find maximum flow using Edmonds-Karp
    auto ekTime = timeFunction([&]
                               {
         network.resetFlow();
         int maxFlow = network.edmondsKarp(source, sink);
         cout << "Maximum flow from " << source << " to " << sink 
              << " using Edmonds-Karp: " << maxFlow << endl; });

    cout << "\nPerformance comparison:" << endl;
    cout << "Ford-Fulkerson: " << ffTime << " ms" << endl;
    cout << "Edmonds-Karp: " << ekTime << " ms" << endl;

    // Find min-cut
    network.resetFlow();
    network.fordFulkerson(source, sink);
    vector<pair<int, int>> minCut = network.getMinCutEdges(source);
    printPairs(minCut, "\nMin-Cut Edges");

    cout << "\nFlow Network after flow calculation:" << endl;
    network.printNetwork();

    // Real-world example: Traffic network
    cout << "\nReal-world example - Traffic Network:" << endl;
    FlowNetwork traffic(6);
    // 0: Residential Area, 1-3: Intersections, 4: Downtown, 5: Industrial Area
    traffic.addEdge(0, 1, 5); // Cars per minute
    traffic.addEdge(0, 2, 8);
    traffic.addEdge(1, 2, 3);
    traffic.addEdge(1, 3, 4);
    traffic.addEdge(2, 3, 2);
    traffic.addEdge(2, 5, 4);
    traffic.addEdge(3, 4, 7);
    traffic.addEdge(3, 5, 3);
    traffic.addEdge(4, 5, 2);

    // Maximum traffic flow from residential to industrial area
    int trafficFlow = traffic.fordFulkerson(0, 5);
    cout << "Maximum traffic flow from residential to industrial area: "
         << trafficFlow << " cars per minute" << endl;

    // Find bottlenecks
    vector<pair<int, int>> bottlenecks = traffic.getMinCutEdges(0);
    printPairs(bottlenecks, "Traffic Bottlenecks (Min-Cut)");

    cout << "Interpretation: These bottlenecks represent roads that are operating at full capacity and\n"
         << "limit the overall traffic flow. Upgrading these roads would increase throughput."
         << endl;
}

// ===================================================
// MAIN FUNCTION
// ===================================================

int main()
{
    cout << "=====================================================" << endl;
    cout << "DAY 34: ADVANCED GRAPH ALGORITHMS" << endl;
    cout << "=====================================================" << endl;

    // Demonstrate all algorithms
    demonstrateSCC();
    demonstrateArticulationPointsAndBridges();
    demonstrateFlowNetworks();

    cout << "\n===== ASSIGNMENT FOR DAY 34 =====" << endl;
    cout << "1. Implement Tarjan's algorithm for finding strongly connected components" << endl;
    cout << "2. Find articulation points in a social network" << endl;
    cout << "3. Solve a maximum flow problem for a resource allocation scenario" << endl;
    cout << "4. Analyze the runtime performance of different algorithms" << endl;

    return 0;
}