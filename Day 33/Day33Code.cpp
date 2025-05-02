/**
 * Topological Sort and Directed Acyclic Graphs (DAGs) - Day 33 of #DSAin45
 *
 * This file implements:
 * 1. A Graph class with DAG operations
 * 2. DFS-based topological sorting
 * 3. Kahn's algorithm (BFS-based) for topological sorting
 * 4. Cycle detection
 * 5. Real-world examples and applications
 *
 * Author: #DSAin45
 * Date: May 2025
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;

// Generic Graph class for Directed Acyclic Graphs
template <typename T>
class DAG
{
private:
    // Adjacency list representation with custom vertex type
    unordered_map<T, vector<T>> adj;

    // Helper function for DFS topological sort
    void dfsTopSortUtil(const T &vertex, unordered_map<T, bool> &visited,
                        unordered_map<T, bool> &recStack, vector<T> &result)
    {
        // Mark current node as visited and part of recursion stack
        visited[vertex] = true;
        recStack[vertex] = true;

        // Recur for all adjacent vertices
        for (const T &neighbor : adj[vertex])
        {
            // If neighbor is not visited, process it
            if (!visited[neighbor])
            {
                dfsTopSortUtil(neighbor, visited, recStack, result);
            }
            // If this vertex is being processed in current recursion, there's a cycle
            else if (recStack[neighbor])
            {
                throw runtime_error("Graph contains a cycle, topological sort not possible");
            }
        }

        // Remove current vertex from recursion stack and add to result
        recStack[vertex] = false;
        result.push_back(vertex);
    }

public:
    // Add a vertex to the graph
    void addVertex(const T &vertex)
    {
        if (adj.find(vertex) == adj.end())
        {
            adj[vertex] = vector<T>();
        }
    }

    // Add a directed edge from source to destination
    void addEdge(const T &src, const T &dest)
    {
        // Add vertices if they don't exist
        addVertex(src);
        addVertex(dest);

        // Add the edge
        adj[src].push_back(dest);
    }

    // Get all vertices in the graph
    vector<T> getVertices() const
    {
        vector<T> vertices;
        for (const auto &pair : adj)
        {
            vertices.push_back(pair.first);
        }
        return vertices;
    }

    // DFS-based topological sort
    vector<T> topologicalSortDFS()
    {
        vector<T> result;
        unordered_map<T, bool> visited;
        unordered_map<T, bool> recStack; // For cycle detection

        // Initialize visited map
        for (const auto &pair : adj)
        {
            visited[pair.first] = false;
            recStack[pair.first] = false;
        }

        try
        {
            // Process all unvisited vertices
            for (const auto &pair : adj)
            {
                if (!visited[pair.first])
                {
                    dfsTopSortUtil(pair.first, visited, recStack, result);
                }
            }

            // Reverse the result to get correct topological order
            reverse(result.begin(), result.end());
            return result;
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
            return {};
        }
    }

    // Kahn's algorithm (BFS-based) for topological sort
    vector<T> topologicalSortKahn()
    {
        vector<T> result;

        // Calculate in-degree for each vertex
        unordered_map<T, int> inDegree;
        for (const auto &pair : adj)
        {
            // Initialize in-degree to 0 for all vertices
            inDegree[pair.first] = 0;
        }

        // Calculate in-degrees
        for (const auto &pair : adj)
        {
            for (const T &neighbor : pair.second)
            {
                inDegree[neighbor]++;
            }
        }

        // Queue vertices with in-degree 0
        queue<T> q;
        for (const auto &pair : inDegree)
        {
            if (pair.second == 0)
            {
                q.push(pair.first);
            }
        }

        // Process vertices in topological order
        int visitedCount = 0;

        while (!q.empty())
        {
            T current = q.front();
            q.pop();
            result.push_back(current);
            visitedCount++;

            // Reduce in-degree of all adjacent vertices
            for (const T &neighbor : adj[current])
            {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0)
                {
                    q.push(neighbor);
                }
            }
        }

        // Check if we visited all vertices
        if (visitedCount != adj.size())
        {
            cout << "Graph contains a cycle, topological sort not possible" << endl;
            return {};
        }

        return result;
    }

    // Detect if graph has a cycle
    bool hasCycle()
    {
        unordered_map<T, bool> visited;
        unordered_map<T, bool> recStack;

        // Initialize visited and recursion stack
        for (const auto &pair : adj)
        {
            visited[pair.first] = false;
            recStack[pair.first] = false;
        }

        // Check for cycle starting from each unvisited vertex
        for (const auto &pair : adj)
        {
            if (!visited[pair.first])
            {
                if (hasCycleUtil(pair.first, visited, recStack))
                {
                    return true;
                }
            }
        }

        return false;
    }

    // Utility function for cycle detection
    bool hasCycleUtil(const T &vertex, unordered_map<T, bool> &visited,
                      unordered_map<T, bool> &recStack)
    {
        // Mark current node as visited and add to recursion stack
        visited[vertex] = true;
        recStack[vertex] = true;

        // Check all adjacent vertices
        for (const T &neighbor : adj[vertex])
        {
            // If neighbor is not visited, check if it leads to a cycle
            if (!visited[neighbor])
            {
                if (hasCycleUtil(neighbor, visited, recStack))
                {
                    return true;
                }
            }
            // If neighbor is in recursion stack, we found a cycle
            else if (recStack[neighbor])
            {
                return true;
            }
        }

        // Remove vertex from recursion stack
        recStack[vertex] = false;
        return false;
    }

    // Calculate longest path lengths (useful for scheduling)
    unordered_map<T, int> longestPathLengths()
    {
        if (hasCycle())
        {
            throw runtime_error("Cannot calculate longest paths in a cyclic graph");
        }

        // Get topological ordering
        vector<T> topOrder = topologicalSortKahn();

        // Initialize distances
        unordered_map<T, int> dist;
        for (const auto &vertex : topOrder)
        {
            dist[vertex] = 0;
        }

        // Process vertices in topological order
        for (const T &vertex : topOrder)
        {
            // For each adjacent vertex, update its distance if needed
            for (const T &neighbor : adj[vertex])
            {
                if (dist[neighbor] < dist[vertex] + 1)
                {
                    dist[neighbor] = dist[vertex] + 1;
                }
            }
        }

        return dist;
    }

    // Find all sources (vertices with no incoming edges)
    vector<T> findSources()
    {
        unordered_map<T, bool> hasIncoming;

        // Initialize all vertices as not having incoming edges
        for (const auto &pair : adj)
        {
            hasIncoming[pair.first] = false;
        }

        // Mark vertices that have incoming edges
        for (const auto &pair : adj)
        {
            for (const T &neighbor : pair.second)
            {
                hasIncoming[neighbor] = true;
            }
        }

        // Collect vertices with no incoming edges
        vector<T> sources;
        for (const auto &pair : hasIncoming)
        {
            if (!pair.second)
            {
                sources.push_back(pair.first);
            }
        }

        return sources;
    }

    // Find all sinks (vertices with no outgoing edges)
    vector<T> findSinks()
    {
        vector<T> sinks;

        for (const auto &pair : adj)
        {
            if (pair.second.empty())
            {
                sinks.push_back(pair.first);
            }
        }

        return sinks;
    }

    // Print adjacency list representation of the graph
    void printGraph()
    {
        cout << "Graph Adjacency List:" << endl;
        for (const auto &pair : adj)
        {
            cout << pair.first << " -> ";
            for (const T &neighbor : pair.second)
            {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Calculate the minimum number of "layers" needed (critical path length)
    int criticalPathLength()
    {
        unordered_map<T, int> pathLengths = longestPathLengths();

        int maxLength = 0;
        for (const auto &pair : pathLengths)
        {
            maxLength = max(maxLength, pair.second);
        }

        return maxLength + 1; // +1 because length 0 is the first layer
    }

    // Group vertices by layers (useful for parallel processing)
    vector<vector<T>> groupByLayers()
    {
        unordered_map<T, int> pathLengths = longestPathLengths();

        int maxLayer = 0;
        for (const auto &pair : pathLengths)
        {
            maxLayer = max(maxLayer, pair.second);
        }

        // Create layer groups
        vector<vector<T>> layers(maxLayer + 1);
        for (const auto &pair : pathLengths)
        {
            layers[pair.second].push_back(pair.first);
        }

        return layers;
    }
};

// Utility function to print a vector
template <typename T>
void printVector(const vector<T> &vec, const string &message)
{
    cout << message << ": ";
    for (const T &item : vec)
    {
        cout << item << " ";
    }
    cout << endl;
}

// Example 1: Course Prerequisites
void coursePrerequisitesExample()
{
    cout << "\n--------- Example: Course Prerequisites ---------\n";

    DAG<string> courseGraph;

    // Add courses and their prerequisites
    courseGraph.addEdge("Data Structures", "Algorithms");
    courseGraph.addEdge("Calculus I", "Calculus II");
    courseGraph.addEdge("Calculus II", "Differential Equations");
    courseGraph.addEdge("Programming Basics", "Data Structures");
    courseGraph.addEdge("Programming Basics", "OOP");
    courseGraph.addEdge("OOP", "Software Engineering");
    courseGraph.addEdge("Data Structures", "Databases");

    // Print the graph structure
    courseGraph.printGraph();

    cout << "\nChecking if curriculum has circular dependencies: ";
    if (courseGraph.hasCycle())
    {
        cout << "Yes, circular dependencies found!" << endl;
    }
    else
    {
        cout << "No circular dependencies detected." << endl;

        // Get valid course sequence using both algorithms
        cout << "\nValid course sequences:\n";

        auto start = chrono::high_resolution_clock::now();
        vector<string> dfsOrder = courseGraph.topologicalSortDFS();
        auto end = chrono::high_resolution_clock::now();
        double dfsTime = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;

        start = chrono::high_resolution_clock::now();
        vector<string> kahnOrder = courseGraph.topologicalSortKahn();
        end = chrono::high_resolution_clock::now();
        double kahnTime = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;

        printVector(dfsOrder, "DFS-based ordering");
        cout << "DFS execution time: " << dfsTime << " ms\n";

        printVector(kahnOrder, "Kahn's algorithm ordering");
        cout << "Kahn's algorithm execution time: " << kahnTime << " ms\n";

        // Find starting courses (no prerequisites)
        vector<string> startingCourses = courseGraph.findSources();
        printVector(startingCourses, "\nStarting courses (no prerequisites)");

        // Find final courses (no dependent courses)
        vector<string> finalCourses = courseGraph.findSinks();
        printVector(finalCourses, "Final courses (no dependent courses)");

        // Calculate minimum semesters needed
        int minSemesters = courseGraph.criticalPathLength();
        cout << "\nMinimum number of semesters needed: " << minSemesters << endl;

        // Group courses by semester
        vector<vector<string>> semesterGroups = courseGraph.groupByLayers();
        cout << "\nSemester-by-semester course plan:\n";
        for (int i = 0; i < semesterGroups.size(); i++)
        {
            cout << "Semester " << (i + 1) << ": ";
            for (const string &course : semesterGroups[i])
            {
                cout << course << ", ";
            }
            cout << endl;
        }
    }
}

// Example 2: Build System Dependencies
void buildSystemExample()
{
    cout << "\n--------- Example: Build System Dependencies ---------\n";

    DAG<string> buildGraph;

    // Add files and their dependencies
    buildGraph.addEdge("main.cpp", "main.o");
    buildGraph.addEdge("utils.cpp", "utils.o");
    buildGraph.addEdge("network.cpp", "network.o");
    buildGraph.addEdge("main.o", "app");
    buildGraph.addEdge("utils.o", "app");
    buildGraph.addEdge("network.o", "app");
    buildGraph.addEdge("utils.h", "main.cpp");
    buildGraph.addEdge("utils.h", "utils.cpp");
    buildGraph.addEdge("network.h", "network.cpp");
    buildGraph.addEdge("network.h", "main.cpp");

    // Print the dependency graph
    buildGraph.printGraph();

    cout << "\nChecking if build system has circular dependencies: ";
    if (buildGraph.hasCycle())
    {
        cout << "Yes, circular dependencies found!" << endl;
    }
    else
    {
        cout << "No circular dependencies detected." << endl;

        // Get build order
        vector<string> buildOrder = buildGraph.topologicalSortKahn();
        printVector(buildOrder, "\nValid build order");

        // Find source files (no dependencies)
        vector<string> sourceFiles = buildGraph.findSources();
        printVector(sourceFiles, "\nSource files (no dependencies)");

        // Find target files (no dependents)
        vector<string> targetFiles = buildGraph.findSinks();
        printVector(targetFiles, "Target files (final build artifacts)");

        // Group by build phases
        vector<vector<string>> buildPhases = buildGraph.groupByLayers();
        cout << "\nParallelizable build phases:\n";
        for (int i = 0; i < buildPhases.size(); i++)
        {
            cout << "Phase " << (i + 1) << ": ";
            for (const string &file : buildPhases[i])
            {
                cout << file << ", ";
            }
            cout << endl;
        }
    }
}

// Example 3: Project Task Scheduling
void projectSchedulingExample()
{
    cout << "\n--------- Example: Project Task Scheduling ---------\n";

    DAG<string> projectGraph;

    // Add tasks and their dependencies
    projectGraph.addEdge("Requirements", "Design");
    projectGraph.addEdge("Design", "Implementation");
    projectGraph.addEdge("Design", "Database Setup");
    projectGraph.addEdge("Implementation", "Unit Testing");
    projectGraph.addEdge("Database Setup", "Integration Testing");
    projectGraph.addEdge("Unit Testing", "Integration Testing");
    projectGraph.addEdge("Integration Testing", "System Testing");
    projectGraph.addEdge("System Testing", "Deployment");
    projectGraph.addEdge("Deployment", "Maintenance");

    // Print the project dependency graph
    projectGraph.printGraph();

    // Get project schedule
    vector<string> schedule = projectGraph.topologicalSortKahn();
    printVector(schedule, "\nProject schedule (task order)");

    // Calculate critical path and slack
    int projectDuration = projectGraph.criticalPathLength();
    cout << "\nMinimum project duration (in phases): " << projectDuration << endl;

    // Group tasks that can be done in parallel
    vector<vector<string>> phases = projectGraph.groupByLayers();
    cout << "\nProject phases with parallel tasks:\n";
    for (int i = 0; i < phases.size(); i++)
    {
        cout << "Phase " << (i + 1) << ": ";
        for (const string &task : phases[i])
        {
            cout << task << ", ";
        }
        cout << endl;
    }
}

// Main function - demonstrate DAG and topological sorting
int main()
{
    cout << "===== Day 33: Topological Sort & Directed Acyclic Graphs (DAGs) =====" << endl;

    // Run examples
    coursePrerequisitesExample();
    buildSystemExample();
    projectSchedulingExample();

    // Demonstrate detecting cycles
    cout << "\n--------- Example: Detecting Cycles ---------\n";

    DAG<int> cycleGraph;
    cycleGraph.addEdge(1, 2);
    cycleGraph.addEdge(2, 3);
    cycleGraph.addEdge(3, 4);
    cycleGraph.addEdge(4, 1); // This creates a cycle!

    cout << "Graph with cycle: ";
    if (cycleGraph.hasCycle())
    {
        cout << "Cycle detected!" << endl;

        try
        {
            // This will throw an exception
            cycleGraph.topologicalSortDFS();
        }
        catch (const exception &e)
        {
            cout << "DFS topological sort: " << e.what() << endl;
        }

        // This will print an error message and return empty vector
        vector<int> kahnResult = cycleGraph.topologicalSortKahn();
        if (kahnResult.empty())
        {
            cout << "Kahn's algorithm correctly detected cycle" << endl;
        }
    }
    else
    {
        cout << "No cycle detected (this shouldn't happen)" << endl;
    }

    cout << "\n===== End of Day 33 Examples =====" << endl;

    return 0;
}