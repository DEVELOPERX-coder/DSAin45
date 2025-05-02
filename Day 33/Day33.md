# Day 33: Topological Sort & Directed Acyclic Graphs (DAGs) #DSAin45

## Introduction

Welcome to Day 33 of our #DSAin45 challenge! Today, we're diving into a fascinating graph algorithm called **Topological Sorting** and the special family of graphs it works on: **Directed Acyclic Graphs (DAGs)**.

Imagine you're planning a semester's worth of courses, but many courses have prerequisites. How do you determine a valid order to take them? Or consider building a software project with multiple dependent modules—what's the correct sequence to compile them? These are perfect real-world examples of problems that topological sorting solves elegantly.

## What is a Directed Acyclic Graph (DAG)?

A Directed Acyclic Graph (DAG) is a directed graph that contains no cycles. In other words, it's impossible to start at a vertex, follow the directed edges, and return to the starting vertex.

Key properties of DAGs:

- All edges have a direction (they're "one-way streets")
- No cycles exist (you can't go in circles)
- At least one node has no incoming edges (called a "source")
- At least one node has no outgoing edges (called a "sink")
- Any finite directed acyclic graph has at least one topological ordering

DAGs are perfect for representing dependencies or precedence relationships, which is why they're so important in computer science.

## What is Topological Sorting?

Topological sorting arranges the vertices of a DAG in a linear order such that:

- For every directed edge (u → v), vertex u comes before vertex v in the ordering
- It's essentially an ordering of vertices that respects all dependency relationships

It's important to note that a DAG may have multiple valid topological orderings, not just one.

**Example**: If we have a graph with edges A → B, A → C, B → D, C → D, a valid topological sort would be [A, B, C, D] or [A, C, B, D] (both satisfy the rule that for any edge u → v, u appears before v).

## Algorithms for Topological Sorting

Let's explore the two main approaches for topological sorting:

### 1. DFS-based Topological Sort

The DFS-based approach uses depth-first search traversal with an additional stack:

1. Perform DFS traversal of the graph
2. When a vertex's DFS exploration is complete (all its neighbors have been visited), push it onto a stack
3. After all vertices are processed, pop elements from the stack to get the topological order

#### Pseudocode:

```
function topologicalSort(graph):
    n = number of vertices in graph
    visited = [false, false, ..., false] (size n)
    stack = empty stack

    for each vertex i in graph:
        if not visited[i]:
            dfsVisit(i, visited, stack, graph)

    return stack (from bottom to top)

function dfsVisit(vertex, visited, stack, graph):
    visited[vertex] = true

    for each neighbor in graph[vertex]:
        if not visited[neighbor]:
            dfsVisit(neighbor, visited, stack, graph)

    stack.push(vertex)
```

### 2. Kahn's Algorithm (BFS-based)

Kahn's algorithm takes a different approach, using in-degree counting:

1. Calculate in-degree (number of incoming edges) for each vertex
2. Enqueue all vertices with in-degree of 0 (sources)
3. While the queue is not empty:
   - Dequeue a vertex and add it to the result
   - For each neighbor of the dequeued vertex, decrease its in-degree by 1
   - If a neighbor's in-degree becomes 0, enqueue it
4. If the topological ordering doesn't include all vertices, the graph has a cycle

#### Pseudocode:

```
function kahnTopologicalSort(graph):
    n = number of vertices in graph
    inDegree = [0, 0, ..., 0] (size n)
    result = empty array

    // Calculate in-degrees
    for each vertex i in graph:
        for each neighbor j of vertex i:
            inDegree[j]++

    // Enqueue nodes with in-degree 0
    queue = empty queue
    for each vertex i in graph:
        if inDegree[i] == 0:
            queue.enqueue(i)

    // Process queue
    while queue is not empty:
        vertex = queue.dequeue()
        result.push(vertex)

        for each neighbor of vertex:
            inDegree[neighbor]--
            if inDegree[neighbor] == 0:
                queue.enqueue(neighbor)

    // Check if valid topological sort
    if result.size() == n:
        return result
    else:
        return "Graph has cycles"
```

## Time and Space Complexity

Both algorithms have efficient performance characteristics:

- **Time Complexity**: O(V + E) for both algorithms, where V is the number of vertices and E is the number of edges.
- **Space Complexity**: O(V) for both algorithms.

## Real-World Applications

Topological sorting has numerous practical applications:

1. **Task Scheduling**: Critical path analysis in project management, determining a sequence to complete tasks with dependencies.

2. **Build Systems**: Determining the order to compile modules in large software projects (like Make, Maven, or Gradle).

3. **Course Prerequisites**: Planning academic schedules where courses have prerequisites.

4. **Dependency Resolution**: Package managers need to resolve dependencies in the correct order.

5. **Data Processing Pipelines**: Determining the order of operations in ETL (Extract, Transform, Load) workflows.

6. **Compilation**: Ordering declarations and operations in programming language compilers.

## C++ Implementation

Let's implement both topological sorting algorithms in C++:

```cpp
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

class Graph {
private:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    // DFS helper function
    void topologicalSortUtil(int v, vector<bool>& visited, stack<int>& Stack) {
        // Mark the current node as visited
        visited[v] = true;

        // Recur for all adjacent vertices
        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                topologicalSortUtil(neighbor, visited, Stack);
            }
        }

        // Push current vertex to stack after all its neighbors are processed
        Stack.push(v);
    }

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add an edge to the graph
    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    // DFS-based topological sort
    vector<int> topologicalSortDFS() {
        stack<int> Stack;
        vector<bool> visited(V, false);

        // Call the recursive helper function for all vertices
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                topologicalSortUtil(i, visited, Stack);
            }
        }

        // Create the result array
        vector<int> result;
        while (!Stack.empty()) {
            result.push_back(Stack.top());
            Stack.pop();
        }

        return result;
    }

    // Detect if the graph has a cycle
    bool hasCycle() {
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);

        for (int i = 0; i < V; i++) {
            if (!visited[i] && hasCycleUtil(i, visited, recStack)) {
                return true;
            }
        }

        return false;
    }

    bool hasCycleUtil(int v, vector<bool>& visited, vector<bool>& recStack) {
        if (!visited[v]) {
            visited[v] = true;
            recStack[v] = true;

            for (int neighbor : adj[v]) {
                if (!visited[neighbor] && hasCycleUtil(neighbor, visited, recStack)) {
                    return true;
                } else if (recStack[neighbor]) {
                    return true;
                }
            }
        }

        recStack[v] = false; // Remove from recursion stack
        return false;
    }

    // Kahn's Algorithm (BFS-based) for topological sort
    vector<int> topologicalSortKahn() {
        vector<int> result;
        vector<int> inDegree(V, 0);

        // Calculate in-degrees
        for (int i = 0; i < V; i++) {
            for (int neighbor : adj[i]) {
                inDegree[neighbor]++;
            }
        }

        // Queue vertices with in-degree 0
        queue<int> q;
        for (int i = 0; i < V; i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }

        // Process queue
        int count = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            result.push_back(v);
            count++;

            // Update in-degrees of adjacent vertices
            for (int neighbor : adj[v]) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        // Check if graph has a cycle
        if (count != V) {
            cout << "Graph has a cycle, topological sort not possible" << endl;
            return {};
        }

        return result;
    }
};

// Utility function to print a vector
void printVector(const vector<int>& vec, const string& message) {
    cout << message << ": ";
    for (int i : vec) {
        cout << i << " ";
    }
    cout << endl;
}
```

## When Can't We Use Topological Sort?

Remember, topological sorting only works on Directed Acyclic Graphs. If a graph has any of these characteristics, topological sorting is not possible:

1. **Undirected Graphs**: All edges must have a direction.
2. **Graphs with Cycles**: A valid topological ordering cannot be created if you can travel in circles.

Think of it this way—if A depends on B, and B depends on C, but C also depends on A, we have a circular dependency (a cycle), and there's no valid order to arrange them.

## The "Layer-by-Layer" Perspective

One helpful way to think about a topological ordering is to visualize "layers" of vertices:

- Layer 0: Vertices with no dependencies (in-degree = 0)
- Layer 1: Vertices that depend only on Layer 0 vertices
- Layer 2: Vertices that depend on Layers 0 and 1
- And so on...

This is essentially what Kahn's algorithm calculates, and it can be useful for understanding parallelization potential in task scheduling.

## A Humorous Insight

Think of topological sorting as the ultimate "who goes first" algorithm for a group of fussy friends. Imagine you have friends who all have preferences about who should enter a room before them:

"I won't enter until Alex is already inside."
"I refuse to enter until both Jordan and Taylor are in the room."

Topological sorting tells you which order to send your friends in, so nobody has a meltdown! And if someone says "I won't enter until Alex is in, but Alex won't enter until I'm in"... well, that's a cycle, and you might need new friends!

## Practice Problems

Now that you understand topological sorting, try these LeetCode problems:

### Easy:

- [207. Course Schedule](https://leetcode.com/problems/course-schedule/): Determine if it's possible to finish all courses given prerequisites (cycle detection in a directed graph).

### Medium:

- [210. Course Schedule II](https://leetcode.com/problems/course-schedule-ii/): Return a valid ordering of courses to take based on prerequisites.
- [1136. Parallel Courses](https://leetcode.com/problems/parallel-courses/): Find the minimum number of semesters needed to take all courses.

### Hard:

- [269. Alien Dictionary](https://leetcode.com/problems/alien-dictionary/): Given a sorted dictionary of an alien language, derive the order of its characters.

## Preview of Tomorrow

Tomorrow, we'll dive into Advanced Graph Algorithms, where we'll explore strongly connected components, articulation points, bridges, and introduce flow networks. These algorithms help us analyze graph structures more deeply and solve complex problems like network reliability and maximum flow.

## Conclusion

Topological sorting is a powerful algorithm when dealing with dependency relationships. Whether you're scheduling tasks, resolving package dependencies, or planning a course schedule, understanding DAGs and topological sorting provides elegant solutions to these complex ordering problems.

Remember the key insight: topological sorting arranges vertices so that no vertex is processed before any of its prerequisites. This simple but powerful idea underlies many tools and systems we rely on daily.

Keep practicing with the LeetCode problems, and we'll see you tomorrow for more advanced graph algorithms!

#DSAin45 #Algorithms #GraphTheory #TopologicalSort #DAG #CPlusPlus #Programming
