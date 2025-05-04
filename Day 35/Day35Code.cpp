/**
 * @file graph_problems.cpp
 * @brief Implementation of classic graph problems and algorithms for Week 5 Challenge
 * @author DSAin45 Series
 * @date May 4, 2025
 *
 * This file contains comprehensive implementations of several graph algorithms:
 * - Ford-Fulkerson algorithm with Edmonds-Karp improvement for maximum flow
 * - Bipartite matching using flow networks
 * - Hungarian algorithm for assignment problems
 * - Traveling Salesman Problem (TSP) approaches
 * - Benchmarking utilities for comparing algorithm performance
 *
 * These implementations are designed for educational purposes with extensive
 * comments and debugging capabilities.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <chrono>
#include <random>
#include <iomanip>
#include <bitset>
#include <functional>

// Define constants
const int INF = std::numeric_limits<int>::max();

class GraphAlgorithms
{
public:
    /**
     * @brief Structure to represent a weighted edge in a graph
     */
    struct Edge
    {
        int from; // Source vertex
        int to;   // Destination vertex
        int cap;  // Capacity of the edge
        int flow; // Current flow along the edge
        int cost; // Cost per unit flow (for min-cost flow problems)

        Edge(int f, int t, int c, int co = 0)
            : from(f), to(t), cap(c), flow(0), cost(co) {}
    };

    /**
     * @brief Flow Network class for maximum flow problems
     */
    class FlowNetwork
    {
    private:
        int n;                             // Number of vertices
        std::vector<Edge> edges;           // List of all edges
        std::vector<std::vector<int>> adj; // Adjacency list of edge indices

    public:
        /**
         * @brief Construct a new Flow Network with n vertices
         * @param n Number of vertices
         */
        FlowNetwork(int n) : n(n)
        {
            adj.resize(n);
        }

        /**
         * @brief Add an edge to the flow network
         * @param from Source vertex
         * @param to Destination vertex
         * @param cap Capacity of the edge
         * @param cost Cost per unit flow (optional)
         * @return Index of the edge in the edges vector
         */
        int addEdge(int from, int to, int cap, int cost = 0)
        {
            // Add forward edge
            edges.emplace_back(from, to, cap, cost);
            adj[from].push_back(edges.size() - 1);

            // Add backward edge with zero capacity
            edges.emplace_back(to, from, 0, -cost);
            adj[to].push_back(edges.size() - 1);

            return edges.size() - 2; // Return index of the forward edge
        }

        /**
         * @brief Implement Ford-Fulkerson algorithm with Edmonds-Karp improvement
         * @param s Source vertex
         * @param t Sink vertex
         * @return Maximum flow from s to t
         */
        int maxFlow(int s, int t)
        {
            int flow = 0;

            while (true)
            {
                // Find an augmenting path using BFS
                std::vector<int> parent(n, -1);
                std::queue<int> q;
                q.push(s);
                parent[s] = -2; // Mark source as visited

                while (!q.empty() && parent[t] == -1)
                {
                    int u = q.front();
                    q.pop();

                    // Explore all adjacent edges
                    for (int eid : adj[u])
                    {
                        int v = edges[eid].to;

                        // If there's residual capacity and v is not visited
                        if (parent[v] == -1 && edges[eid].cap > edges[eid].flow)
                        {
                            parent[v] = eid;
                            q.push(v);
                        }
                    }
                }

                // If no augmenting path found, we're done
                if (parent[t] == -1)
                    break;

                // Find minimum residual capacity along the path
                int path_flow = INF;
                for (int v = t; v != s;)
                {
                    int eid = parent[v];
                    path_flow = std::min(path_flow, edges[eid].cap - edges[eid].flow);
                    v = edges[eid].from;
                }

                // Augment flow along the path
                for (int v = t; v != s;)
                {
                    int eid = parent[v];
                    edges[eid].flow += path_flow;
                    edges[eid ^ 1].flow -= path_flow; // Update reverse edge
                    v = edges[eid].from;
                }

                flow += path_flow;
            }

            return flow;
        }

        /**
         * @brief Implement Dinic's algorithm for maximum flow
         * @param s Source vertex
         * @param t Sink vertex
         * @return Maximum flow from s to t
         */
        int dinicMaxFlow(int s, int t)
        {
            int flow = 0;
            std::vector<int> level(n);
            std::vector<int> next(n); // For DFS, stores next edge to explore

            // BFS to construct level graph
            auto bfs = [&]() -> bool
            {
                std::fill(level.begin(), level.end(), -1);
                level[s] = 0;
                std::queue<int> q;
                q.push(s);

                while (!q.empty())
                {
                    int u = q.front();
                    q.pop();

                    for (int eid : adj[u])
                    {
                        Edge &e = edges[eid];
                        if (level[e.to] == -1 && e.flow < e.cap)
                        {
                            level[e.to] = level[u] + 1;
                            q.push(e.to);
                        }
                    }
                }

                return level[t] != -1; // Return if sink is reachable
            };

            // DFS to find blocking flow
            std::function<int(int, int)> dfs = [&](int u, int flow_limit) -> int
            {
                if (u == t)
                    return flow_limit;

                for (int &i = next[u]; i < adj[u].size(); i++)
                {
                    int eid = adj[u][i];
                    Edge &e = edges[eid];

                    if (level[e.to] == level[u] + 1 && e.flow < e.cap)
                    {
                        int pushed = dfs(e.to, std::min(flow_limit, e.cap - e.flow));

                        if (pushed > 0)
                        {
                            edges[eid].flow += pushed;
                            edges[eid ^ 1].flow -= pushed;
                            return pushed;
                        }
                    }
                }

                return 0;
            };

            // Main Dinic's algorithm loop
            while (bfs())
            {
                std::fill(next.begin(), next.end(), 0);
                int f;
                while ((f = dfs(s, INF)) > 0)
                {
                    flow += f;
                }
            }

            return flow;
        }

        /**
         * @brief Min-cost Max-flow algorithm using successive shortest path
         * @param s Source vertex
         * @param t Sink vertex
         * @return Pair (max flow, min cost)
         */
        std::pair<int, int> minCostMaxFlow(int s, int t)
        {
            int flow = 0;
            int cost = 0;
            std::vector<int> dist(n);
            std::vector<int> parent(n);
            std::vector<bool> inQueue(n);

            // Bellman-Ford with SPFA optimization
            auto findPath = [&]() -> bool
            {
                std::fill(dist.begin(), dist.end(), INF);
                std::fill(parent.begin(), parent.end(), -1);
                std::fill(inQueue.begin(), inQueue.end(), false);

                dist[s] = 0;
                std::queue<int> q;
                q.push(s);
                inQueue[s] = true;

                while (!q.empty())
                {
                    int u = q.front();
                    q.pop();
                    inQueue[u] = false;

                    for (int eid : adj[u])
                    {
                        Edge &e = edges[eid];

                        if (e.flow < e.cap && dist[e.to] > dist[u] + e.cost)
                        {
                            dist[e.to] = dist[u] + e.cost;
                            parent[e.to] = eid;

                            if (!inQueue[e.to])
                            {
                                q.push(e.to);
                                inQueue[e.to] = true;
                            }
                        }
                    }
                }

                return dist[t] != INF; // Return if there is a path
            };

            // Main algorithm loop
            while (findPath())
            {
                int path_flow = INF;

                // Find minimum residual capacity
                for (int v = t; v != s;)
                {
                    int eid = parent[v];
                    path_flow = std::min(path_flow, edges[eid].cap - edges[eid].flow);
                    v = edges[eid].from;
                }

                // Update flow and cost
                for (int v = t; v != s;)
                {
                    int eid = parent[v];
                    edges[eid].flow += path_flow;
                    edges[eid ^ 1].flow -= path_flow;
                    cost += path_flow * edges[eid].cost;
                    v = edges[eid].from;
                }

                flow += path_flow;
            }

            return {flow, cost};
        }

        /**
         * @brief Get the maximum flow in the network
         * @return Total flow out of the source
         */
        int getTotalFlow(int s)
        {
            int total = 0;
            for (int eid : adj[s])
            {
                if (edges[eid].from == s)
                {
                    total += edges[eid].flow;
                }
            }
            return total;
        }

        /**
         * @brief Print the current flow network
         */
        void printFlow()
        {
            std::cout << "Current Flow Network:\n";
            for (int i = 0; i < edges.size(); i += 2)
            {
                Edge &e = edges[i];
                if (e.cap > 0)
                { // Only print forward edges
                    std::cout << "Edge " << e.from << " -> " << e.to
                              << ": Flow " << e.flow << "/" << e.cap;
                    if (e.cost != 0)
                    {
                        std::cout << " (Cost: " << e.cost << ")";
                    }
                    std::cout << "\n";
                }
            }
        }
    };

    /**
     * @brief Implementation of the Hungarian algorithm for assignment problems
     * @param cost Cost matrix where cost[i][j] is the cost of assigning worker i to job j
     * @return Vector of assignments where result[i] = j means worker i is assigned to job j
     */
    std::vector<int> hungarianAlgorithm(const std::vector<std::vector<int>> &cost_matrix)
    {
        // Make a copy of the cost matrix as we'll modify it
        std::vector<std::vector<int>> cost = cost_matrix;
        int n = cost.size();

        // Ensure the matrix is square by adding dummy rows/columns if needed
        int max_dim = std::max(n, static_cast<int>(cost[0].size()));
        cost.resize(max_dim, std::vector<int>(max_dim, 0));
        for (int i = 0; i < n; i++)
        {
            cost[i].resize(max_dim, 0);
        }

        std::vector<int> lx(max_dim, 0);  // Labels for workers
        std::vector<int> ly(max_dim, 0);  // Labels for jobs
        std::vector<int> mx(max_dim, -1); // Current assignment for workers
        std::vector<int> my(max_dim, -1); // Current assignment for jobs

        // Step 1: Initial labeling
        for (int x = 0; x < max_dim; x++)
        {
            lx[x] = *std::max_element(cost[x].begin(), cost[x].end());
        }

        // Main loop of the Hungarian algorithm
        for (int root = 0; root < max_dim; root++)
        {
            // If worker already assigned, skip
            if (mx[root] != -1)
                continue;

            std::vector<bool> S(max_dim, false);  // Set of workers in the tree
            std::vector<bool> T(max_dim, false);  // Set of jobs in the tree
            std::vector<int> slack(max_dim, INF); // Slack for each job
            std::vector<int> slackx(max_dim, -1); // Workers that provide minimum slack
            std::vector<int> prev(max_dim, -1);   // For path augmentation

            S[root] = true;

            // Initialize slack for first worker
            for (int y = 0; y < max_dim; y++)
            {
                slack[y] = lx[root] + ly[y] - cost[root][y];
                slackx[y] = root;
            }

            int y = -1; // Current job

            // Find augmenting path
            while (true)
            {
                // Find job with minimum slack
                int delta = INF;
                y = -1;

                for (int j = 0; j < max_dim; j++)
                {
                    if (!T[j] && slack[j] < delta)
                    {
                        delta = slack[j];
                        y = j;
                    }
                }

                // Update dual variables
                for (int i = 0; i < max_dim; i++)
                {
                    if (S[i])
                        lx[i] -= delta;
                }
                for (int j = 0; j < max_dim; j++)
                {
                    if (T[j])
                        ly[j] += delta;
                    else
                        slack[j] -= delta;
                }

                // Add new job to T
                T[y] = true;

                // If job is not assigned, we've found an augmenting path
                if (my[y] == -1)
                    break;

                // Add the worker that is assigned to this job to S
                int x = my[y];
                S[x] = true;

                // Update slack for the new worker
                for (int j = 0; j < max_dim; j++)
                {
                    if (!T[j])
                    {
                        int new_slack = lx[x] + ly[j] - cost[x][j];
                        if (new_slack < slack[j])
                        {
                            slack[j] = new_slack;
                            slackx[j] = x;
                        }
                    }
                }
            }

            // Augment the matching along the found path
            while (y != -1)
            {
                int x = slackx[y];
                int next_y = mx[x];
                mx[x] = y;
                my[y] = x;
                y = next_y;
            }
        }

        // Extract the assignments for the original workers
        std::vector<int> result(n);
        for (int i = 0; i < n; i++)
        {
            result[i] = mx[i];
        }

        return result;
    }

    /**
     * @brief Solve maximum bipartite matching problem using flow network
     * @param graph Adjacency matrix where graph[i][j] = true if worker i can do job j
     * @return Maximum number of matches and the matching pairs
     */
    std::pair<int, std::vector<std::pair<int, int>>> maxBipartiteMatching(
        const std::vector<std::vector<bool>> &graph)
    {
        int n = graph.size();    // Number of workers
        int m = graph[0].size(); // Number of jobs

        // Create a flow network
        // n+m+2 vertices: n workers + m jobs + source + sink
        FlowNetwork network(n + m + 2);
        int source = n + m;
        int sink = n + m + 1;

        // Add edges from source to workers
        for (int i = 0; i < n; i++)
        {
            network.addEdge(source, i, 1);
        }

        // Add edges from workers to jobs they can do
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (graph[i][j])
                {
                    network.addEdge(i, n + j, 1);
                }
            }
        }

        // Add edges from jobs to sink
        for (int j = 0; j < m; j++)
        {
            network.addEdge(n + j, sink, 1);
        }

        // Find maximum flow
        int max_matches = network.maxFlow(source, sink);

        // Extract the matching
        std::vector<std::pair<int, int>> matches;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                // Check if there's flow on the edge worker->job
                // For each worker, find the job they're matched with
                if (graph[i][j])
                {
                    // Find the edge index in the FlowNetwork
                    for (size_t e = 0; e < network.edges.size(); e += 2)
                    {
                        if (network.edges[e].from == i && network.edges[e].to == n + j && network.edges[e].flow > 0)
                        {
                            matches.emplace_back(i, j);
                            break;
                        }
                    }
                }
            }
        }

        return {max_matches, matches};
    }

    /**
     * @brief Solve the Traveling Salesman Problem using dynamic programming (Held-Karp)
     * @param dist Distance matrix where dist[i][j] is the distance from city i to city j
     * @return Minimum distance and the optimal tour
     */
    std::pair<int, std::vector<int>> tspDP(const std::vector<std::vector<int>> &dist)
    {
        int n = dist.size();

        // dp[mask][i] = minimum distance of path that visits all vertices in mask and ends at i
        std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));

        // parent[mask][i] = previous vertex in the shortest path to dp[mask][i]
        std::vector<std::vector<int>> parent(1 << n, std::vector<int>(n, -1));

        // Base case: start at vertex 0
        dp[1][0] = 0; // Only vertex 0 is visited, ending at 0

        // Iterate through all subsets of vertices
        for (int mask = 1; mask < (1 << n); mask++)
        {
            // Skip if vertex 0 is not in the subset
            if (!(mask & 1))
                continue;

            // Try to add a new vertex to the path
            for (int u = 0; u < n; u++)
            {
                // Skip if u is not in the current subset
                if (!(mask & (1 << u)))
                    continue;

                // The subset without u
                int prev_mask = mask ^ (1 << u);

                // Skip if prev_mask is empty (only happens for u=0, mask=1)
                if (prev_mask == 0)
                    continue;

                // Find the best previous vertex
                for (int v = 0; v < n; v++)
                {
                    // Skip if v is not in prev_mask
                    if (!(prev_mask & (1 << v)))
                        continue;

                    // Relax the edge v->u
                    if (dp[prev_mask][v] + dist[v][u] < dp[mask][u])
                    {
                        dp[mask][u] = dp[prev_mask][v] + dist[v][u];
                        parent[mask][u] = v;
                    }
                }
            }
        }

        // Find the shortest Hamiltonian path that visits all vertices and returns to 0
        int min_dist = INF;
        int last_vertex = -1;

        for (int u = 1; u < n; u++)
        {
            if (dp[(1 << n) - 1][u] + dist[u][0] < min_dist)
            {
                min_dist = dp[(1 << n) - 1][u] + dist[u][0];
                last_vertex = u;
            }
        }

        // Reconstruct the path
        std::vector<int> path;
        int mask = (1 << n) - 1; // All vertices
        int u = last_vertex;

        while (u != -1)
        {
            path.push_back(u);
            int next_u = parent[mask][u];
            mask ^= (1 << u); // Remove u from the mask
            u = next_u;
        }

        // Add the starting vertex to complete the cycle
        path.push_back(0);

        // Reverse to get the correct order (starting from 0)
        std::reverse(path.begin(), path.end());

        return {min_dist, path};
    }

    /**
     * @brief Solve TSP using Nearest Neighbor heuristic (greedy approach)
     * @param dist Distance matrix
     * @return Approximate tour distance and path
     */
    std::pair<int, std::vector<int>> tspNearestNeighbor(const std::vector<std::vector<int>> &dist)
    {
        int n = dist.size();
        std::vector<int> path;
        std::vector<bool> visited(n, false);

        // Start from vertex 0
        int current = 0;
        path.push_back(current);
        visited[current] = true;

        // Greedily choose the nearest unvisited vertex
        for (int i = 1; i < n; i++)
        {
            int nearest = -1;
            int min_dist = INF;

            for (int j = 0; j < n; j++)
            {
                if (!visited[j] && dist[current][j] < min_dist)
                {
                    min_dist = dist[current][j];
                    nearest = j;
                }
            }

            current = nearest;
            path.push_back(current);
            visited[current] = true;
        }

        // Add the starting vertex to complete the cycle
        path.push_back(0);

        // Calculate the total distance
        int total_dist = 0;
        for (int i = 0; i < path.size() - 1; i++)
        {
            total_dist += dist[path[i]][path[i + 1]];
        }

        return {total_dist, path};
    }

    /**
     * @brief Solve TSP using 2-opt improvement heuristic
     * @param dist Distance matrix
     * @param initial_tour Initial tour to improve
     * @param max_iterations Maximum number of improvement iterations
     * @return Improved tour distance and path
     */
    std::pair<int, std::vector<int>> tsp2Opt(
        const std::vector<std::vector<int>> &dist,
        std::vector<int> initial_tour,
        int max_iterations = 1000)
    {
        int n = dist.size();

        // Calculate initial tour distance
        int current_distance = 0;
        for (int i = 0; i < n; i++)
        {
            current_distance += dist[initial_tour[i]][initial_tour[(i + 1) % n]];
        }

        bool improved = true;
        int iterations = 0;

        while (improved && iterations < max_iterations)
        {
            improved = false;
            iterations++;

            for (int i = 0; i < n - 2; i++)
            {
                for (int j = i + 2; j < n; j++)
                {
                    // Calculate the change in distance if we swap edges (i,i+1) and (j,j+1)
                    int old_distance = dist[initial_tour[i]][initial_tour[i + 1]] +
                                       dist[initial_tour[j]][initial_tour[(j + 1) % n]];
                    int new_distance = dist[initial_tour[i]][initial_tour[j]] +
                                       dist[initial_tour[i + 1]][initial_tour[(j + 1) % n]];

                    if (new_distance < old_distance)
                    {
                        // Reverse the segment between i+1 and j
                        std::reverse(initial_tour.begin() + i + 1, initial_tour.begin() + j + 1);

                        // Update the current distance
                        current_distance += (new_distance - old_distance);

                        improved = true;
                        break;
                    }
                }
                if (improved)
                    break;
            }
        }

        return {current_distance, initial_tour};
    }
};

/**
 * @brief Utility class for benchmarking and comparing algorithms
 */
class BenchmarkUtils
{
public:
    /**
     * @brief Measure execution time of a function
     * @param func Function to measure
     * @return Execution time in milliseconds
     */
    template <typename Func>
    static double measureTime(Func func)
    {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        return duration.count();
    }

    /**
     * @brief Generate a random graph with n vertices and density probability of edges
     * @param n Number of vertices
     * @param density Probability of an edge between any two vertices [0,1]
     * @param max_capacity Maximum capacity of any edge
     * @return Adjacency matrix representation of the graph
     */
    static std::vector<std::vector<int>> generateRandomGraph(
        int n, double density, int max_capacity = 100)
    {
        std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        std::uniform_int_distribution<> cap_dis(1, max_capacity);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i != j && dis(gen) < density)
                {
                    graph[i][j] = cap_dis(gen);
                }
            }
        }

        return graph;
    }

    /**
     * @brief Generate a random bipartite graph
     * @param n Number of vertices in the first partition
     * @param m Number of vertices in the second partition
     * @param density Probability of an edge between partitions
     * @return Adjacency matrix (n x m) where graph[i][j] = true if there's an edge
     */
    static std::vector<std::vector<bool>> generateRandomBipartite(
        int n, int m, double density)
    {
        std::vector<std::vector<bool>> graph(n, std::vector<bool>(m, false));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (dis(gen) < density)
                {
                    graph[i][j] = true;
                }
            }
        }

        return graph;
    }

    /**
     * @brief Generate a random cost matrix for assignment problems
     * @param n Number of workers
     * @param m Number of jobs
     * @param max_cost Maximum cost value
     * @return Cost matrix
     */
    static std::vector<std::vector<int>> generateRandomCostMatrix(
        int n, int m, int max_cost = 100)
    {
        std::vector<std::vector<int>> cost(n, std::vector<int>(m));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, max_cost);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cost[i][j] = dis(gen);
            }
        }

        return cost;
    }

    /**
     * @brief Create a symmetric distance matrix for TSP
     * @param n Number of cities
     * @param max_dist Maximum distance between cities
     * @return Symmetric distance matrix
     */
    static std::vector<std::vector<int>> generateTSPInstance(int n, int max_dist = 100)
    {
        std::vector<std::vector<int>> dist(n, std::vector<int>(n, 0));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, max_dist);

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                dist[i][j] = dist[j][i] = dis(gen);
            }
        }

        return dist;
    }

    /**
     * @brief Compare different algorithms for the same problem
     * @param algorithms Vector of pairs (algorithm function, algorithm name)
     * @param num_runs Number of times to run each algorithm
     */
    template <typename Func>
    static void compareAlgorithms(
        const std::vector<std::pair<Func, std::string>> &algorithms,
        int num_runs = 10)
    {
        std::vector<double> avg_times(algorithms.size(), 0);

        for (int run = 0; run < num_runs; run++)
        {
            for (int i = 0; i < algorithms.size(); i++)
            {
                auto &[algo, name] = algorithms[i];
                double time = measureTime(algo);
                avg_times[i] += time;

                std::cout << "Run " << run + 1 << ", "
                          << name << ": " << time << " ms\n";
            }
        }

        std::cout << "\nAverage execution times:\n";
        for (int i = 0; i < algorithms.size(); i++)
        {
            avg_times[i] /= num_runs;
            std::cout << algorithms[i].second << ": "
                      << avg_times[i] << " ms\n";
        }
    }
};

/**
 * @brief Real-world example: Traffic Flow Optimization
 */
class TrafficFlowOptimization
{
private:
    std::vector<std::vector<int>> roadNetwork; // Capacity of roads
    std::vector<std::string> locations;        // Names of intersections

public:
    /**
     * @brief Initialize a traffic flow optimization problem
     * @param locations Names of intersections
     * @param roadNetwork Capacity matrix for roads
     */
    TrafficFlowOptimization(
        const std::vector<std::string> &locs,
        const std::vector<std::vector<int>> &roads)
        : locations(locs), roadNetwork(roads) {}

    /**
     * @brief Find the maximum flow from source to destination
     * @param source_name Name of the source location
     * @param sink_name Name of the destination location
     * @return Maximum traffic flow and bottleneck edges
     */
    std::pair<int, std::vector<std::tuple<std::string, std::string, int>>>
    maximizeTrafficFlow(const std::string &source_name, const std::string &sink_name)
    {
        // Find indices of source and sink
        int source = -1, sink = -1;
        for (int i = 0; i < locations.size(); i++)
        {
            if (locations[i] == source_name)
                source = i;
            if (locations[i] == sink_name)
                sink = i;
        }

        if (source == -1 || sink == -1)
        {
            throw std::invalid_argument("Source or sink location not found");
        }

        // Create a flow network
        GraphAlgorithms::FlowNetwork network(locations.size());

        // Add edges to the network
        for (int i = 0; i < roadNetwork.size(); i++)
        {
            for (int j = 0; j < roadNetwork[i].size(); j++)
            {
                if (roadNetwork[i][j] > 0)
                {
                    network.addEdge(i, j, roadNetwork[i][j]);
                }
            }
        }

        // Find maximum flow
        int max_flow = network.maxFlow(source, sink);

        // Find bottleneck edges (saturated edges in the min-cut)
        std::vector<std::tuple<std::string, std::string, int>> bottlenecks;

        // A simple way to find bottlenecks is to look for edges that are at capacity
        for (int i = 0; i < network.edges.size(); i += 2)
        { // Skip backward edges
            auto &e = network.edges[i];
            if (e.flow == e.cap && e.cap > 0)
            {
                bottlenecks.emplace_back(locations[e.from], locations[e.to], e.cap);
            }
        }

        return {max_flow, bottlenecks};
    }

    /**
     * @brief Simulate a road closure and recalculate maximum flow
     * @param closed_road_start Start of the closed road
     * @param closed_road_end End of the closed road
     * @return New maximum flow after road closure
     */
    int simulateRoadClosure(const std::string &closed_road_start,
                            const std::string &closed_road_end,
                            const std::string &source_name,
                            const std::string &sink_name)
    {
        // Find indices
        int start_idx = -1, end_idx = -1;
        for (int i = 0; i < locations.size(); i++)
        {
            if (locations[i] == closed_road_start)
                start_idx = i;
            if (locations[i] == closed_road_end)
                end_idx = i;
        }

        if (start_idx == -1 || end_idx == -1)
        {
            throw std::invalid_argument("Road endpoints not found");
        }

        // Create a modified road network with the closed road
        auto modified_network = roadNetwork;
        modified_network[start_idx][end_idx] = 0;

        // Recalculate maximum flow
        TrafficFlowOptimization modified_simulation(locations, modified_network);
        auto [new_flow, _] = modified_simulation.maximizeTrafficFlow(source_name, sink_name);

        return new_flow;
    }
};

/**
 * @brief Real-world example: Job Assignment Optimization
 */
class JobAssignmentOptimization
{
private:
    std::vector<std::string> workers;
    std::vector<std::string> jobs;
    std::vector<std::vector<int>> costs;

public:
    /**
     * @brief Initialize a job assignment problem
     * @param workers Names of workers
     * @param jobs Names of jobs
     * @param costs Cost matrix where costs[i][j] is the cost of assigning worker i to job j
     */
    JobAssignmentOptimization(
        const std::vector<std::string> &w,
        const std::vector<std::string> &j,
        const std::vector<std::vector<int>> &c)
        : workers(w), jobs(j), costs(c) {}

    /**
     * @brief Find the optimal assignment of workers to jobs
     * @return Total cost and assignment pairs
     */
    std::pair<int, std::vector<std::pair<std::string, std::string>>>
    findOptimalAssignment()
    {
        GraphAlgorithms algo;
        auto assignments = algo.hungarianAlgorithm(costs);

        // Calculate the total cost
        int total_cost = 0;
        std::vector<std::pair<std::string, std::string>> pairs;

        for (int i = 0; i < workers.size(); i++)
        {
            int job = assignments[i];
            if (job < jobs.size())
            { // Ensure job is valid
                total_cost += costs[i][job];
                pairs.emplace_back(workers[i], jobs[job]);
            }
        }

        return {total_cost, pairs};
    }

    /**
     * @brief Find the best job for a specific worker
     * @param worker_name Name of the worker
     * @return Best job and its cost
     */
    std::pair<std::string, int> findBestJobForWorker(const std::string &worker_name)
    {
        // Find worker index
        int worker_idx = -1;
        for (int i = 0; i < workers.size(); i++)
        {
            if (workers[i] == worker_name)
            {
                worker_idx = i;
                break;
            }
        }

        if (worker_idx == -1)
        {
            throw std::invalid_argument("Worker not found");
        }

        // Find the job with minimum cost
        int best_job_idx = 0;
        int min_cost = costs[worker_idx][0];

        for (int j = 1; j < jobs.size(); j++)
        {
            if (costs[worker_idx][j] < min_cost)
            {
                min_cost = costs[worker_idx][j];
                best_job_idx = j;
            }
        }

        return {jobs[best_job_idx], min_cost};
    }
};

/**
 * @brief Real-world example: Network Routing
 */
class NetworkRouting
{
private:
    std::vector<std::string> nodes;
    std::vector<std::vector<int>> bandwidthMatrix;
    std::vector<std::vector<int>> latencyMatrix;

public:
    /**
     * @brief Initialize a network routing problem
     * @param nodes Names of network nodes
     * @param bandwidth Bandwidth matrix between nodes
     * @param latency Latency matrix between nodes
     */
    NetworkRouting(
        const std::vector<std::string> &n,
        const std::vector<std::vector<int>> &b,
        const std::vector<std::vector<int>> &l)
        : nodes(n), bandwidthMatrix(b), latencyMatrix(l) {}

    /**
     * @brief Find the maximum bandwidth path between two nodes
     * @param source_name Source node name
     * @param dest_name Destination node name
     * @return Maximum bandwidth and the path
     */
    std::pair<int, std::vector<std::string>>
    maximumBandwidthPath(const std::string &source_name, const std::string &dest_name)
    {
        // Find indices
        int source = -1, dest = -1;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i] == source_name)
                source = i;
            if (nodes[i] == dest_name)
                dest = i;
        }

        if (source == -1 || dest == -1)
        {
            throw std::invalid_argument("Source or destination node not found");
        }

        int n = nodes.size();
        std::vector<int> maxBandwidth(n, 0);
        std::vector<int> parent(n, -1);
        std::vector<bool> visited(n, false);

        // Initialize bandwidth for source
        maxBandwidth[source] = std::numeric_limits<int>::max();

        for (int i = 0; i < n; i++)
        {
            // Find the unvisited node with maximum bandwidth
            int u = -1;
            int max_bw = 0;

            for (int j = 0; j < n; j++)
            {
                if (!visited[j] && maxBandwidth[j] > max_bw)
                {
                    max_bw = maxBandwidth[j];
                    u = j;
                }
            }

            if (u == -1 || u == dest)
                break;

            visited[u] = true;

            // Update bandwidths of adjacent nodes
            for (int v = 0; v < n; v++)
            {
                if (!visited[v] && bandwidthMatrix[u][v] > 0)
                {
                    int new_bw = std::min(maxBandwidth[u], bandwidthMatrix[u][v]);
                    if (new_bw > maxBandwidth[v])
                    {
                        maxBandwidth[v] = new_bw;
                        parent[v] = u;
                    }
                }
            }
        }

        // No path found
        if (maxBandwidth[dest] == 0)
        {
            return {0, {}};
        }

        // Reconstruct the path
        std::vector<std::string> path;
        for (int v = dest; v != -1; v = parent[v])
        {
            path.push_back(nodes[v]);
        }
        std::reverse(path.begin(), path.end());

        return {maxBandwidth[dest], path};
    }

    /**
     * @brief Find the lowest latency path between two nodes
     * @param source_name Source node name
     * @param dest_name Destination node name
     * @param min_bandwidth Minimum required bandwidth
     * @return Lowest latency and the path
     */
    std::pair<int, std::vector<std::string>>
    lowestLatencyPath(const std::string &source_name,
                      const std::string &dest_name,
                      int min_bandwidth = 0)
    {
        // Find indices
        int source = -1, dest = -1;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i] == source_name)
                source = i;
            if (nodes[i] == dest_name)
                dest = i;
        }

        if (source == -1 || dest == -1)
        {
            throw std::invalid_argument("Source or destination node not found");
        }

        int n = nodes.size();
        std::vector<int> dist(n, INF);
        std::vector<int> parent(n, -1);
        std::vector<bool> visited(n, false);

        // Initialize distance for source
        dist[source] = 0;

        for (int i = 0; i < n; i++)
        {
            // Find the unvisited node with minimum distance
            int u = -1;
            int min_dist = INF;

            for (int j = 0; j < n; j++)
            {
                if (!visited[j] && dist[j] < min_dist)
                {
                    min_dist = dist[j];
                    u = j;
                }
            }

            if (u == -1 || u == dest)
                break;

            visited[u] = true;

            // Update distances of adjacent nodes
            for (int v = 0; v < n; v++)
            {
                if (!visited[v] && latencyMatrix[u][v] != INF &&
                    bandwidthMatrix[u][v] >= min_bandwidth)
                {
                    if (dist[u] + latencyMatrix[u][v] < dist[v])
                    {
                        dist[v] = dist[u] + latencyMatrix[u][v];
                        parent[v] = u;
                    }
                }
            }
        }

        // No path found
        if (dist[dest] == INF)
        {
            return {INF, {}};
        }

        // Reconstruct the path
        std::vector<std::string> path;
        for (int v = dest; v != -1; v = parent[v])
        {
            path.push_back(nodes[v]);
        }
        std::reverse(path.begin(), path.end());

        return {dist[dest], path};
    }
};

/**
 * @brief Main demonstration function
 */
int main()
{
    // Set up console output formatting
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "========================\n";
    std::cout << "Graph Problems Showcase\n";
    std::cout << "========================\n\n";

    std::cout << "1. Maximum Flow Demonstration\n";
    std::cout << "-----------------------------\n";

    // Create a flow network for demonstration
    GraphAlgorithms::FlowNetwork network(6);

    // Add edges (from, to, capacity)
    network.addEdge(0, 1, 10);
    network.addEdge(0, 2, 5);
    network.addEdge(1, 2, 2);
    network.addEdge(1, 3, 6);
    network.addEdge(2, 3, 3);
    network.addEdge(2, 4, 8);
    network.addEdge(3, 4, 10);
    network.addEdge(3, 5, 12);
    network.addEdge(4, 5, 7);

    std::cout << "Initial network:\n";
    network.printFlow();

    int max_flow = network.maxFlow(0, 5);

    std::cout << "\nAfter computing maximum flow:\n";
    network.printFlow();
    std::cout << "\nMaximum flow value: " << max_flow << "\n\n";

    std::cout << "2. Bipartite Matching Demonstration\n";
    std::cout << "----------------------------------\n";

    // Create a bipartite graph for job assignment
    std::vector<std::vector<bool>> bpGraph = {
        {true, true, false, false, false},
        {false, true, true, false, false},
        {false, false, true, true, false},
        {false, false, false, true, true},
        {true, false, false, false, true}};

    GraphAlgorithms algo;
    auto [max_matches, matches] = algo.maxBipartiteMatching(bpGraph);

    std::cout << "Maximum number of matches: " << max_matches << "\n";
    std::cout << "Matchings:\n";
    for (const auto &[worker, job] : matches)
    {
        std::cout << "Worker " << worker << " -> Job " << job << "\n";
    }
    std::cout << "\n";

    std::cout << "3. Hungarian Algorithm Demonstration\n";
    std::cout << "-----------------------------------\n";

    // Create a cost matrix for assignment problem
    std::vector<std::vector<int>> costMatrix = {
        {5, 3, 8, 9, 3},
        {7, 2, 4, 7, 8},
        {6, 1, 6, 5, 4},
        {2, 9, 4, 2, 7},
        {5, 4, 2, 9, 8}};

    auto assignments = algo.hungarianAlgorithm(costMatrix);

    int total_cost = 0;
    std::cout << "Optimal assignments:\n";
    for (int i = 0; i < assignments.size(); i++)
    {
        int j = assignments[i];
        total_cost += costMatrix[i][j];
        std::cout << "Worker " << i << " -> Job " << j
                  << " (Cost: " << costMatrix[i][j] << ")\n";
    }
    std::cout << "Total cost: " << total_cost << "\n\n";

    std::cout << "4. TSP Demonstration\n";
    std::cout << "-------------------\n";

    // Create a distance matrix for small TSP example
    std::vector<std::vector<int>> distMatrix = {
        {0, 20, 42, 35, 29},
        {20, 0, 30, 34, 25},
        {42, 30, 0, 12, 15},
        {35, 34, 12, 0, 18},
        {29, 25, 15, 18, 0}};

    // Solve with dynamic programming
    auto [dp_dist, dp_path] = algo.tspDP(distMatrix);

    std::cout << "TSP solution using dynamic programming:\n";
    std::cout << "Minimum distance: " << dp_dist << "\n";
    std::cout << "Optimal path: ";
    for (int city : dp_path)
    {
        std::cout << city << " -> ";
    }
    std::cout << dp_path[0] << "\n\n";

    // Solve with nearest neighbor heuristic
    auto [nn_dist, nn_path] = algo.tspNearestNeighbor(distMatrix);

    std::cout << "TSP solution using nearest neighbor heuristic:\n";
    std::cout << "Distance: " << nn_dist << "\n";
    std::cout << "Path: ";
    for (int city : nn_path)
    {
        std::cout << city << " -> ";
    }
    std::cout << nn_path[0] << "\n\n";

    // Improve with 2-opt
    auto [improved_dist, improved_path] = algo.tsp2Opt(distMatrix, nn_path);

    std::cout << "TSP solution after 2-opt improvement:\n";
    std::cout << "Improved distance: " << improved_dist << "\n";
    std::cout << "Improved path: ";
    for (int city : improved_path)
    {
        std::cout << city << " -> ";
    }
    std::cout << improved_path[0] << "\n\n";

    std::cout << "5. Real-world Example: Traffic Flow\n";
    std::cout << "-----------------------------------\n";

    // Create a traffic network
    std::vector<std::string> locations = {
        "Downtown", "Suburb", "Airport", "Mall", "Industrial", "Highway"};

    std::vector<std::vector<int>> roadCapacity = {
        {0, 1000, 800, 0, 0, 0},
        {0, 0, 500, 700, 0, 0},
        {0, 0, 0, 0, 900, 0},
        {0, 0, 0, 0, 600, 500},
        {0, 0, 0, 0, 0, 1200},
        {0, 0, 0, 0, 0, 0}};

    TrafficFlowOptimization traffic(locations, roadCapacity);
    auto [max_traffic, bottlenecks] = traffic.maximizeTrafficFlow("Downtown", "Highway");

    std::cout << "Maximum traffic flow: " << max_traffic << " vehicles/hour\n";
    std::cout << "Bottlenecks:\n";
    for (const auto &[from, to, cap] : bottlenecks)
    {
        std::cout << "Road from " << from << " to " << to
                  << " (Capacity: " << cap << " vehicles/hour)\n";
    }

    // Simulate road closure
    int new_flow = traffic.simulateRoadClosure("Airport", "Industrial", "Downtown", "Highway");
    std::cout << "\nAfter closing Airport-to-Industrial road:\n";
    std::cout << "New maximum flow: " << new_flow << " vehicles/hour\n";
    std::cout << "Flow reduction: " << max_traffic - new_flow << " vehicles/hour\n\n";

    std::cout << "6. Real-world Example: Job Assignment\n";
    std::cout << "-------------------------------------\n";

    // Create a job assignment problem
    std::vector<std::string> workers = {
        "Alice", "Bob", "Charlie", "Dave", "Eve"};

    std::vector<std::string> jobs = {
        "Software Dev", "QA Testing", "UI Design", "Data Analysis", "DevOps"};

    std::vector<std::vector<int>> skillCosts = {
        {10, 20, 5, 15, 25}, // Alice's costs for each job
        {15, 5, 20, 25, 10}, // Bob's costs
        {25, 10, 15, 5, 20}, // Charlie's costs
        {5, 15, 20, 25, 10}, // Dave's costs
        {20, 10, 25, 15, 5}  // Eve's costs
    };

    JobAssignmentOptimization jobAssign(workers, jobs, skillCosts);
    auto [total_cost, assignments_pairs] = jobAssign.findOptimalAssignment();

    std::cout << "Optimal job assignments (lower cost = better skill):\n";
    for (const auto &[worker, job] : assignments_pairs)
    {
        std::cout << worker << " assigned to " << job << "\n";
    }
    std::cout << "Total cost: " << total_cost << "\n\n";

    auto [best_job, cost] = jobAssign.findBestJobForWorker("Alice");
    std::cout << "Best job for Alice: " << best_job << " (Cost: " << cost << ")\n\n";

    std::cout << "7. Real-world Example: Network Routing\n";
    std::cout << "--------------------------------------\n";

    // Create a network routing problem
    std::vector<std::string> nodes = {
        "NYC", "LA", "Chicago", "Houston", "Miami", "Seattle"};

    // Bandwidth matrix (Mbps)
    std::vector<std::vector<int>> bandwidth = {
        {0, 100, 200, 50, 80, 0},
        {100, 0, 60, 120, 0, 150},
        {200, 60, 0, 180, 100, 80},
        {50, 120, 180, 0, 200, 0},
        {80, 0, 100, 200, 0, 50},
        {0, 150, 80, 0, 50, 0}};

    // Latency matrix (ms)
    std::vector<std::vector<int>> latency = {
        {0, 40, 20, 30, 25, INF},
        {40, 0, 35, 25, INF, 15},
        {20, 35, 0, 15, 30, 40},
        {30, 25, 15, 0, 10, INF},
        {25, INF, 30, 10, 0, 45},
        {INF, 15, 40, INF, 45, 0}};

    NetworkRouting routing(nodes, bandwidth, latency);

    auto [max_bw, bw_path] = routing.maximumBandwidthPath("NYC", "Seattle");
    std::cout << "Maximum bandwidth path from NYC to Seattle: "
              << max_bw << " Mbps\n";
    std::cout << "Path: ";
    for (int i = 0; i < bw_path.size(); i++)
    {
        std::cout << bw_path[i];
        if (i < bw_path.size() - 1)
            std::cout << " -> ";
    }
    std::cout << "\n\n";

    auto [min_latency, lat_path] = routing.lowestLatencyPath("NYC", "Seattle", 50);
    std::cout << "Lowest latency path from NYC to Seattle (min 50 Mbps): "
              << min_latency << " ms\n";
    std::cout << "Path: ";
    for (int i = 0; i < lat_path.size(); i++)
    {
        std::cout << lat_path[i];
        if (i < lat_path.size() - 1)
            std::cout << " -> ";
    }
    std::cout << "\n\n";

    std::cout << "8. Algorithm Benchmarking\n";
    std::cout << "-------------------------\n";

    // Compare Ford-Fulkerson vs. Dinic's algorithm
    std::cout << "Comparing max flow algorithms on random graphs:\n";

    // Generate random graphs of different sizes
    for (int n : {10, 20, 50})
    {
        std::cout << "\nGraph with " << n << " vertices:\n";

        auto random_graph = BenchmarkUtils::generateRandomGraph(n, 0.3);

        GraphAlgorithms::FlowNetwork ff_network(n);
        GraphAlgorithms::FlowNetwork dinic_network(n);

        // Add edges to both networks
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (random_graph[i][j] > 0)
                {
                    ff_network.addEdge(i, j, random_graph[i][j]);
                    dinic_network.addEdge(i, j, random_graph[i][j]);
                }
            }
        }

        // Measure performance
        double ff_time = BenchmarkUtils::measureTime([&]()
                                                     { ff_network.maxFlow(0, n - 1); });

        double dinic_time = BenchmarkUtils::measureTime([&]()
                                                        { dinic_network.dinicMaxFlow(0, n - 1); });

        std::cout << "Ford-Fulkerson (Edmonds-Karp): " << ff_time << " ms\n";
        std::cout << "Dinic's algorithm: " << dinic_time << " ms\n";
        std::cout << "Speedup: " << ff_time / dinic_time << "x\n";
    }

    std::cout << "\nComparing TSP algorithms on random instance:\n";

    // Generate a random TSP instance
    auto tsp_instance = BenchmarkUtils::generateTSPInstance(10);

    double dp_time = BenchmarkUtils::measureTime([&]()
                                                 { algo.tspDP(tsp_instance); });

    double nn_time = BenchmarkUtils::measureTime([&]()
                                                 { algo.tspNearestNeighbor(tsp_instance); });

    std::cout << "Dynamic Programming: " << dp_time << " ms\n";
    std::cout << "Nearest Neighbor: " << nn_time << " ms\n";
    std::cout << "Speedup: " << dp_time / nn_time << "x\n";

    // Compare solution quality
    auto [dp_opt, _] = algo.tspDP(tsp_instance);
    auto [nn_opt, nn_p] = algo.tspNearestNeighbor(tsp_instance);
    auto [opt_2_opt, _2] = algo.tsp2Opt(tsp_instance, nn_p);

    std::cout << "DP solution (optimal): " << dp_opt << "\n";
    std::cout << "Nearest Neighbor solution: " << nn_opt
              << " (error: " << (nn_opt - dp_opt) * 100.0 / dp_opt << "%)\n";
    std::cout << "2-opt improved solution: " << opt_2_opt
              << " (error: " << (opt_2_opt - dp_opt) * 100.0 / dp_opt << "%)\n";

    std::cout << "\n==========================\n";
    std::cout << "End of Graph Problems Demo\n";
    std::cout << "==========================\n";

    return 0;
}