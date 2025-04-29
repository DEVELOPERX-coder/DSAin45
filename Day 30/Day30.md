# 🔍 DAY 30: Graph Traversals - Navigating Networks #DSAin45

Welcome to Day 30 of our 45-day DSA journey! Yesterday, we established the foundations of graph theory with basic graph representations and operations. Today, we're diving into **Graph Traversal Algorithms** — the fundamental techniques for exploring and processing every vertex in a graph.

_Imagine you're exploring an unfamiliar city without a map. How would you navigate it? Would you explore one neighborhood thoroughly before moving to the next, or would you explore all adjacent areas at each step? These two approaches mirror the two primary graph traversal strategies we'll be exploring today: Depth-First Search (DFS) and Breadth-First Search (BFS)._

## 📌 Graph Traversal Fundamentals

Graph traversal is the process of visiting (checking and/or updating) each vertex in a graph exactly once. Unlike tree traversal, graphs may contain cycles, which could cause an infinite loop. Therefore, we need to track the vertices we've already visited.

The two primary graph traversal algorithms are:

1. **Breadth-First Search (BFS)**: Explores all neighbors at the present depth before moving to vertices at the next depth level
2. **Depth-First Search (DFS)**: Explores as far as possible along each branch before backtracking

Both algorithms have their own strengths, weaknesses, and applications.

## 🌊 Breadth-First Search (BFS)

Breadth-First Search explores a graph level by level, visiting all neighbors of a vertex before moving on to the next level. It uses a **queue** data structure to keep track of the vertices to visit next.

### Key Characteristics of BFS:

- Uses a queue (FIFO - First In, First Out)
- Visits vertices in order of their distance from the source
- Guarantees the shortest path in unweighted graphs
- Requires more memory than DFS for storing the queue

### BFS Implementation:

```cpp
void BFS(const IGraph& graph, int startVertex) {
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex)) {
        return;
    }

    // Create a queue for BFS
    std::queue<int> queue;

    // Create a set to keep track of visited vertices
    std::unordered_set<int> visited;

    // Mark the start vertex as visited and enqueue it
    visited.insert(startVertex);
    queue.push(startVertex);

    std::cout << "BFS traversal starting from vertex " << startVertex << ": ";

    while (!queue.empty()) {
        // Dequeue a vertex from queue and print it
        int currentVertex = queue.front();
        queue.pop();
        std::cout << currentVertex << " ";

        // Get all adjacent vertices of the dequeued vertex
        // If an adjacent vertex has not been visited, mark it
        // visited and enqueue it
        for (int neighbor : graph.getNeighbors(currentVertex)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }

    std::cout << std::endl;
}
```

### BFS with Path Tracking:

To track the path from the start vertex to any other vertex, we can maintain a parent map:

```cpp
std::unordered_map<int, int> BFSWithPathTracking(const IGraph& graph, int startVertex) {
    std::unordered_map<int, int> parent;  // Maps each vertex to its parent in the BFS tree
    std::queue<int> queue;
    std::unordered_set<int> visited;

    // Mark the start vertex as visited and enqueue it
    visited.insert(startVertex);
    queue.push(startVertex);
    parent[startVertex] = -1;  // Start vertex has no parent

    while (!queue.empty()) {
        int currentVertex = queue.front();
        queue.pop();

        for (int neighbor : graph.getNeighbors(currentVertex)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
                parent[neighbor] = currentVertex;  // Set parent
            }
        }
    }

    return parent;
}

// Function to reconstruct path from start to end using parent map
std::vector<int> reconstructPath(const std::unordered_map<int, int>& parent, int start, int end) {
    std::vector<int> path;

    // Check if end was reached from start
    if (parent.find(end) == parent.end() && end != start) {
        return path;  // No path exists
    }

    // Start from the end and work backwards
    for (int at = end; at != -1; at = parent.at(at)) {
        path.push_back(at);
    }

    // Reverse path to get it from start to end
    std::reverse(path.begin(), path.end());

    return path;
}
```

### Applications of BFS:

1. **Shortest Path in Unweighted Graphs**: BFS finds the shortest path in terms of the number of edges.
2. **Level Order Traversal**: Used in tree algorithms to process nodes level by level.
3. **Connected Components**: Finds all vertices reachable from a given vertex.
4. **Web Crawlers**: Explore web pages level by level from a starting URL.
5. **Social Network Analysis**: Find all users within k connections of a user.
6. **Puzzle Solving**: Find the shortest sequence of moves to solve puzzles like Rubik's cube or sliding puzzles.

### Time and Space Complexity:

- **Time Complexity**: O(V + E) where V is the number of vertices and E is the number of edges
  - In the worst case, we visit every vertex and edge exactly once
  - For adjacency matrix representation: O(V²)
- **Space Complexity**: O(V) for the queue and visited set
  - In the worst case, the queue might contain all vertices

## 🔍 Depth-First Search (DFS)

Depth-First Search explores a graph by going as deep as possible down a branch before backtracking. It can be implemented recursively using the system call stack or iteratively using an explicit stack data structure.

### Key Characteristics of DFS:

- Uses a stack (LIFO - Last In, First Out) or recursion
- Explores one branch to its full depth before backtracking
- May not find the shortest path
- More memory-efficient than BFS for deep graphs
- Useful for topological sorting, cycle detection, and connected components

### DFS Implementation (Recursive):

```cpp
void DFSRecursive(const IGraph& graph, int startVertex, std::unordered_set<int>& visited) {
    // Mark the current vertex as visited and print it
    visited.insert(startVertex);
    std::cout << startVertex << " ";

    // Recur for all adjacent vertices
    for (int neighbor : graph.getNeighbors(startVertex)) {
        if (visited.find(neighbor) == visited.end()) {
            DFSRecursive(graph, neighbor, visited);
        }
    }
}

// Wrapper function to handle disconnected graphs
void DFS(const IGraph& graph, int startVertex) {
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex)) {
        return;
    }

    std::unordered_set<int> visited;

    std::cout << "DFS traversal starting from vertex " << startVertex << ": ";
    DFSRecursive(graph, startVertex, visited);
    std::cout << std::endl;
}
```

### DFS Implementation (Iterative):

```cpp
void DFSIterative(const IGraph& graph, int startVertex) {
    // Check if the start vertex exists
    if (!graph.hasVertex(startVertex)) {
        return;
    }

    // Create a stack for DFS
    std::stack<int> stack;

    // Create a set to keep track of visited vertices
    std::unordered_set<int> visited;

    // Push the start vertex to the stack
    stack.push(startVertex);

    std::cout << "DFS traversal starting from vertex " << startVertex << ": ";

    while (!stack.empty()) {
        // Pop a vertex from stack
        int currentVertex = stack.top();
        stack.pop();

        // If the vertex is not visited, mark it as visited and print it
        if (visited.find(currentVertex) == visited.end()) {
            visited.insert(currentVertex);
            std::cout << currentVertex << " ";

            // Push all adjacent vertices to the stack (in reverse order to match recursive DFS)
            std::vector<int> neighbors = graph.getNeighbors(currentVertex);
            for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    stack.push(*it);
                }
            }
        }
    }

    std::cout << std::endl;
}
```

### Applications of DFS:

1. **Cycle Detection**: Used to detect cycles in a graph.
2. **Topological Sorting**: Orders vertices such that for every directed edge u→v, u comes before v in the ordering.
3. **Connected Components**: Finds all vertices in the same connected component.
4. **Maze Generation and Solving**: Creates perfect mazes and finds paths through them.
5. **Backtracking Algorithms**: Used in constraint satisfaction problems like Sudoku and N-Queens.
6. **Strongly Connected Components**: Finds strongly connected components in directed graphs (using Kosaraju's or Tarjan's algorithm).

### Time and Space Complexity:

- **Time Complexity**: O(V + E) where V is the number of vertices and E is the number of edges
  - Same as BFS, we visit every vertex and edge exactly once
  - For adjacency matrix representation: O(V²)
- **Space Complexity**:
  - For recursive implementation: O(V) for the call stack in the worst case
  - For iterative implementation: O(V) for the explicit stack and visited set

## 📊 BFS vs. DFS: When to Use Which?

Both BFS and DFS have O(V + E) time complexity, but they have different characteristics that make each more suitable for specific problems:

### Use BFS when:

1. **Finding Shortest Paths in Unweighted Graphs**: BFS guarantees the shortest path in terms of number of edges.
2. **Level by Level Processing**: You need to process all vertices at the same distance first.
3. **The Graph is Wide**: When the graph is wide but not deep, BFS is more memory-efficient.
4. **The Target is Likely Close to the Start**: BFS will find it faster.

### Use DFS when:

1. **Exhaustive Search is Required**: You need to explore all possible paths.
2. **Maze Solving**: When you want to explore one path completely before trying another.
3. **The Graph is Deep**: When the graph has long paths, DFS uses less memory.
4. **Detecting Cycles**: It's easier to detect cycles using DFS.
5. **Topological Sorting**: DFS is naturally suited for topological sorting of directed acyclic graphs.

## 🧮 Advanced Graph Traversal Techniques

Beyond basic BFS and DFS, there are several specialized traversal techniques:

### 1. Bidirectional BFS

Runs two BFS searches simultaneously: one from the start vertex and one from the end vertex. The search terminates when the two searches meet.

```cpp
std::vector<int> bidirectionalBFS(const IGraph& graph, int start, int end) {
    if (start == end) {
        return {start};  // Path is just the start/end vertex
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

    int meetingPoint = -1;  // Where the two searches meet

    while (!queueStart.empty() && !queueEnd.empty()) {
        // Expand forward search
        int currentStart = queueStart.front();
        queueStart.pop();

        for (int neighbor : graph.getNeighbors(currentStart)) {
            if (visitedStart.find(neighbor) == visitedStart.end()) {
                visitedStart.insert(neighbor);
                queueStart.push(neighbor);
                parentStart[neighbor] = currentStart;

                // Check if we've reached a vertex visited by the backward search
                if (visitedEnd.find(neighbor) != visitedEnd.end()) {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1) break;  // Searches have met

        // Expand backward search
        int currentEnd = queueEnd.front();
        queueEnd.pop();

        for (int neighbor : graph.getNeighbors(currentEnd)) {
            if (visitedEnd.find(neighbor) == visitedEnd.end()) {
                visitedEnd.insert(neighbor);
                queueEnd.push(neighbor);
                parentEnd[neighbor] = currentEnd;

                // Check if we've reached a vertex visited by the forward search
                if (visitedStart.find(neighbor) != visitedStart.end()) {
                    meetingPoint = neighbor;
                    break;
                }
            }
        }

        if (meetingPoint != -1) break;  // Searches have met
    }

    // If no meeting point was found, there's no path
    if (meetingPoint == -1) {
        return {};
    }

    // Reconstruct the path
    std::vector<int> path;

    // Add the path from start to meeting point
    for (int at = meetingPoint; at != -1; at = parentStart[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    // Add the path from meeting point to end (excluding meeting point)
    for (int at = parentEnd[meetingPoint]; at != -1; at = parentEnd[at]) {
        path.push_back(at);
    }

    return path;
}
```

### 2. Iterative Deepening Depth-First Search (IDDFS)

Combines the space efficiency of DFS with BFS's guarantee of finding the shortest path. It performs DFS with a depth limit, increasing the limit after each iteration.

```cpp
// DFS with depth limit
bool depthLimitedDFS(const IGraph& graph, int current, int target, int depthLimit,
                     std::unordered_set<int>& visited, std::vector<int>& path) {

    if (current == target) {
        path.push_back(current);
        return true;
    }

    if (depthLimit <= 0) {
        return false;
    }

    visited.insert(current);
    path.push_back(current);

    for (int neighbor : graph.getNeighbors(current)) {
        if (visited.find(neighbor) == visited.end()) {
            if (depthLimitedDFS(graph, neighbor, target, depthLimit - 1, visited, path)) {
                return true;
            }
        }
    }

    path.pop_back();  // Backtrack
    visited.erase(current);
    return false;
}

// Iterative Deepening DFS
std::vector<int> IDDFS(const IGraph& graph, int start, int target, int maxDepth) {
    for (int depth = 0; depth <= maxDepth; depth++) {
        std::unordered_set<int> visited;
        std::vector<int> path;

        if (depthLimitedDFS(graph, start, target, depth, visited, path)) {
            return path;
        }
    }

    return {};  // No path found within the maximum depth
}
```

### 3. A\* Search Algorithm

A\* is an informed search algorithm that uses a heuristic function to guide the search towards the target. It's commonly used for pathfinding in games and robotics.

```cpp
// Note: This implementation assumes the graph vertices are 2D points with x,y coordinates

// Heuristic function (Manhattan distance)
double heuristic(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<int> AStarSearch(const IGraph& graph, int start, int goal,
                             const std::unordered_map<int, Point>& positions) {

    // Priority queue for A* - pair of (f-value, vertex)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>> openSet;

    std::unordered_set<int> closedSet;
    std::unordered_map<int, int> parent;
    std::unordered_map<int, double> gScore;  // Cost from start to current
    std::unordered_map<int, double> fScore;  // Estimated total cost

    // Initialize start node
    gScore[start] = 0;
    fScore[start] = heuristic(positions.at(start), positions.at(goal));
    openSet.push({fScore[start], start});
    parent[start] = -1;

    while (!openSet.empty()) {
        int current = openSet.top().second;
        openSet.pop();

        if (current == goal) {
            // Reconstruct path
            std::vector<int> path;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(current);

        for (int neighbor : graph.getNeighbors(current)) {
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;  // Skip already evaluated vertices
            }

            // Calculate tentative g score
            double edgeWeight = graph.getEdgeWeight(current, neighbor);
            double tentativeGScore = gScore[current] + edgeWeight;

            // If this path is better than the previous one
            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                parent[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(positions.at(neighbor), positions.at(goal));

                // Add to open set if not already there
                openSet.push({fScore[neighbor], neighbor});
            }
        }
    }

    return {};  // No path found
}
```

## 🛠️ Real-World Applications of Graph Traversal

Graph traversal algorithms are ubiquitous in software systems and real-world applications:

### 1. Social Network Analysis

```cpp
// Find all friends within k degrees of separation
std::vector<int> friendsWithinKDegrees(const IGraph& socialNetwork, int user, int k) {
    std::queue<std::pair<int, int>> queue;  // Pair of (user, distance)
    std::unordered_set<int> visited;
    std::vector<int> result;

    queue.push({user, 0});
    visited.insert(user);

    while (!queue.empty()) {
        auto [currentUser, distance] = queue.front();
        queue.pop();

        // If within k degrees, add to result
        if (distance > 0 && distance <= k) {
            result.push_back(currentUser);
        }

        // Stop expanding if we've reached k degrees
        if (distance >= k) {
            continue;
        }

        // Process neighbors (friends)
        for (int friend_ : socialNetwork.getNeighbors(currentUser)) {
            if (visited.find(friend_) == visited.end()) {
                visited.insert(friend_);
                queue.push({friend_, distance + 1});
            }
        }
    }

    return result;
}
```

### 2. Web Crawling

```cpp
// Simple web crawler using BFS
void webCrawl(const std::string& startUrl, int maxPages) {
    std::queue<std::string> queue;
    std::unordered_set<std::string> visited;

    queue.push(startUrl);
    visited.insert(startUrl);

    int pageCount = 0;

    while (!queue.empty() && pageCount < maxPages) {
        std::string currentUrl = queue.front();
        queue.pop();

        // Download and process the page
        std::cout << "Crawling: " << currentUrl << std::endl;
        pageCount++;

        // Extract links from the page (simplified)
        std::vector<std::string> links = extractLinks(currentUrl);

        for (const std::string& link : links) {
            if (visited.find(link) == visited.end()) {
                visited.insert(link);
                queue.push(link);
            }
        }
    }
}
```

### 3. Maze Solving

```cpp
// Solve a maze using DFS
bool solveMaze(const std::vector<std::vector<int>>& maze,
               const Point& start, const Point& end,
               std::vector<std::vector<bool>>& visited,
               std::vector<Point>& path) {

    int rows = maze.size();
    int cols = maze[0].size();

    // Check if current position is valid
    if (start.x < 0 || start.x >= rows || start.y < 0 || start.y >= cols ||
        maze[start.x][start.y] == 1 || visited[start.x][start.y]) {
        return false;
    }

    // Mark current cell as visited and add to path
    visited[start.x][start.y] = true;
    path.push_back(start);

    // Check if we've reached the end
    if (start.x == end.x && start.y == end.y) {
        return true;
    }

    // Try all four directions
    const int dx[] = {1, 0, -1, 0};
    const int dy[] = {0, 1, 0, -1};

    for (int i = 0; i < 4; i++) {
        Point nextPoint = {start.x + dx[i], start.y + dy[i]};

        if (solveMaze(maze, nextPoint, end, visited, path)) {
            return true;
        }
    }

    // If no direction leads to the end, backtrack
    path.pop_back();
    return false;
}
```

### 4. Network Routing

```cpp
// Simplified routing table computation using BFS
std::unordered_map<int, int> computeRoutingTable(const IGraph& network, int sourceRouter) {
    std::unordered_map<int, int> routingTable;  // Destination -> Next hop
    std::queue<int> queue;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;

    queue.push(sourceRouter);
    visited.insert(sourceRouter);
    parent[sourceRouter] = -1;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int neighbor : network.getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
                parent[neighbor] = current;
            }
        }
    }

    // Compute next hop for each destination
    for (const auto& [dest, p] : parent) {
        if (dest == sourceRouter) continue;

        // Find the first hop from source
        int current = dest;
        while (parent[current] != sourceRouter && parent[current] != -1) {
            current = parent[current];
        }

        // If reachable, add to routing table
        if (parent[current] == sourceRouter) {
            routingTable[dest] = current;
        }
    }

    return routingTable;
}
```

## 😄 Graph Traversal Jokes and Analogies

1. **The Party Exploration Analogy**:
   "BFS is like the social butterfly at a party who briefly chats with everyone in the room before deciding who to have a deeper conversation with. DFS is like the deep conversationalist who gets stuck talking to the first interesting person they meet for the entire party."

2. **The Dating Strategy Joke**:
   "BFS dating strategy: Go on one date with everyone in the city before deciding on a second date. DFS dating strategy: Date someone, then their friends, then their friends' friends, before ever going back to your original dating pool."

3. **The Maze Solver Analogy**:
   "DFS is like a maze solver who just keeps following the left wall until they find the exit or return to the start. BFS is like a maze solver who sends a scout to check each possible turn before deciding which path to explore further."

4. **The Stack Overflow Joke**:
   "Why did the recursive DFS function go to therapy? It had deep-seated issues with its call stack."

5. **The Queue vs Stack Life Philosophy**:
   "BFS uses a queue and processes things in the order they arrive—like a responsible adult. DFS uses a stack and always does the newest thing first—like a procrastinating student."

## 🧩 LeetCode Practice Problems

### Easy:

#### 1. Number of Islands (LeetCode #200)

**Problem:** Given an m x n 2D binary grid grid which represents a map of '1's (land) and '0's (water), return the number of islands. An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically.

**Solution:**

```cpp
int numIslands(std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int islands = 0;

    // DFS to mark all connected land cells as visited
    std::function<void(int, int)> dfs = [&](int r, int c) {
        // Check bounds and if it's a land cell
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] != '1') {
            return;
        }

        // Mark as visited by changing '1' to '2'
        grid[r][c] = '2';

        // Visit all 4 adjacent cells
        dfs(r+1, c);
        dfs(r-1, c);
        dfs(r, c+1);
        dfs(r, c-1);
    };

    // Count islands by starting DFS from each unvisited land cell
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '1') {
                islands++;
                dfs(r, c);
            }
        }
    }

    // Restore the grid (optional)
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '2') {
                grid[r][c] = '1';
            }
        }
    }

    return islands;
}
```

#### 2. Flood Fill (LeetCode #733)

**Problem:** An image is represented by an m x n integer grid image where image[i][j] represents the pixel value of the image. You are given three integers sr, sc, and color. Perform a flood fill starting from the pixel image[sr][sc] with the new color. Change the color of that pixel, plus any pixels connected 4-directionally to the same color as the starting pixel.

**Solution:**

```cpp
std::vector<std::vector<int>> floodFill(std::vector<std::vector<int>>& image, int sr, int sc, int color) {
    int rows = image.size();
    int cols = image[0].size();
    int originalColor = image[sr][sc];

    // If the starting pixel is already the new color, return the image as is
    if (originalColor == color) {
        return image;
    }

    // DFS to fill connected pixels
    std::function<void(int, int)> dfs = [&](int r, int c) {
        // Check bounds and if it's the original color
        if (r < 0 || r >= rows || c < 0 || c >= cols || image[r][c] != originalColor) {
            return;
        }

        // Change color
        image[r][c] = color;

        // Recursively fill adjacent pixels
        dfs(r+1, c);
        dfs(r-1, c);
        dfs(r, c+1);
        dfs(r, c-1);
    };

    dfs(sr, sc);
    return image;
}
```

### Medium:

#### 1. Pacific Atlantic Water Flow (LeetCode #417)

**Problem:** There is an m x n rectangular island that borders the Pacific Ocean and the Atlantic Ocean. The island is partitioned into a grid of square cells. Rain water can flow to neighboring cells directly north, south, east, and west if the neighboring cell's height is less than or equal to the current cell's height. Water can flow from any cell adjacent to an ocean into the ocean. Return a list of grid coordinates where water can flow to both the Pacific and Atlantic oceans.

**Solution:**

```cpp
std::vector<std::vector<int>> pacificAtlantic(std::vector<std::vector<int>>& heights) {
    if (heights.empty() || heights[0].empty()) {
        return {};
    }

    int rows = heights.size();
    int cols = heights[0].size();

    std::vector<std::vector<bool>> pacificReachable(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<bool>> atlanticReachable(rows, std::vector<bool>(cols, false));

    // DFS function
    std::function<void(int, int, std::vector<std::vector<bool>>&, int)> dfs =
        [&](int r, int c, std::vector<std::vector<bool>>& reachable, int prevHeight) {
            // Check bounds and if already visited
            if (r < 0 || r >= rows || c < 0 || c >= cols ||
                reachable[r][c] || heights[r][c] < prevHeight) {
                return;
            }

            // Mark as reachable
            reachable[r][c] = true;

            // Visit all 4 adjacent cells
            dfs(r+1, c, reachable, heights[r][c]);
            dfs(r-1, c, reachable, heights[r][c]);
            dfs(r, c+1, reachable, heights[r][c]);
            dfs(r, c-1, reachable, heights[r][c]);
        };

    // Start DFS from Pacific edges (left and top borders)
    for (int r = 0; r < rows; r++) {
        dfs(r, 0, pacificReachable, heights[r][0]);
    }
    for (int c = 0; c < cols; c++) {
        dfs(0, c, pacificReachable, heights[0][c]);
    }

    // Start DFS from Atlantic edges (right and bottom borders)
    for (int r = 0; r < rows; r++) {
        dfs(r, cols - 1, atlanticReachable, heights[r][cols - 1]);
    }
    for (int c = 0; c < cols; c++) {
        dfs(rows - 1, c, atlanticReachable, heights[rows - 1][c]);
    }

    // Find cells that can reach both oceans
    std::vector<std::vector<int>> result;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (pacificReachable[r][c] && atlanticReachable[r][c]) {
                result.push_back({r, c});
            }
        }
    }

    return result;
}
```

#### 2. Course Schedule II (LeetCode #210)

**Problem:** There are a total of numCourses courses you have to take, labeled from 0 to numCourses-1. You are given an array prerequisites where prerequisites[i] = [ai, bi] indicates that you must take course bi first if you want to take course ai. Return the ordering of courses you should take to finish all courses. If it is impossible to finish all courses, return an empty array.

**Solution:**

```cpp
std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
    // Build adjacency list
    std::vector<std::vector<int>> graph(numCourses);
    std::vector<int> inDegree(numCourses, 0);

    for (const auto& prereq : prerequisites) {
        int course = prereq[0];
        int prerequisite = prereq[1];
        graph[prerequisite].push_back(course);
        inDegree[course]++;
    }

    // Kahn's Algorithm (BFS-based topological sort)
    std::queue<int> queue;

    // Add all vertices with in-degree 0 to the queue
    for (int i = 0; i < numCourses; i++) {
        if (inDegree[i] == 0) {
            queue.push(i);
        }
    }

    std::vector<int> result;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        result.push_back(current);

        for (int neighbor : graph[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                queue.push(neighbor);
            }
        }
    }

    // If we couldn't include all courses, there's a cycle
    if (result.size() != numCourses) {
        return {};
    }

    return result;
}
```

### Hard:

#### 1. Word Ladder (LeetCode #127)

**Problem:** A transformation sequence from beginWord to endWord is a sequence of words such that each word has exactly one letter different from the previous word. Return the length of the shortest transformation sequence from beginWord to endWord, or 0 if no such sequence exists.

**Solution:**

```cpp
int ladderLength(std::string beginWord, std::string endWord, std::vector<std::string>& wordList) {
    // Create a set for O(1) lookups
    std::unordered_set<std::string> wordSet(wordList.begin(), wordList.end());

    // Check if endWord is in the dictionary
    if (wordSet.find(endWord) == wordSet.end()) {
        return 0;
    }

    // BFS
    std::queue<std::string> queue;
    queue.push(beginWord);
    int level = 1;  // Start with level 1 (beginWord)

    while (!queue.empty()) {
        int size = queue.size();

        // Process all words at the current level
        for (int i = 0; i < size; i++) {
            std::string currentWord = queue.front();
            queue.pop();

            // Try all possible one-letter transformations
            for (int j = 0; j < currentWord.length(); j++) {
                char originalChar = currentWord[j];

                // Try replacing with all letters
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == originalChar) continue;

                    currentWord[j] = c;

                    // If we've reached the end word, return the level + 1
                    if (currentWord == endWord) {
                        return level + 1;
                    }

                    // If the transformed word is in the dictionary, add it to the queue
                    if (wordSet.find(currentWord) != wordSet.end()) {
                        queue.push(currentWord);
                        wordSet.erase(currentWord);  // Remove to avoid cycles
                    }
                }

                // Restore the original character
                currentWord[j] = originalChar;
            }
        }

        level++;  // Move to the next level
    }

    return 0;  // No transformation sequence found
}
```

#### 2. Alien Dictionary (LeetCode #269 - Premium)

**Problem:** There is a new alien language that uses the English alphabet. Given a list of words sorted lexicographically by the rules of this new language, derive the order of the letters in this language.

**Solution:**

```cpp
std::string alienOrder(std::vector<std::string>& words) {
    // Build graph (adjacency list)
    std::unordered_map<char, std::unordered_set<char>> graph;
    std::unordered_map<char, int> inDegree;

    // Initialize graph with all letters
    for (const auto& word : words) {
        for (char c : word) {
            graph[c] = std::unordered_set<char>();
            inDegree[c] = 0;
        }
    }

    // Build graph from word order
    for (int i = 0; i < words.size() - 1; i++) {
        std::string word1 = words[i];
        std::string word2 = words[i + 1];

        // Check for invalid input (word2 is a prefix of word1)
        if (word1.size() > word2.size() && word1.substr(0, word2.size()) == word2) {
            return "";
        }

        // Find the first different character
        for (int j = 0; j < std::min(word1.size(), word2.size()); j++) {
            if (word1[j] != word2[j]) {
                // word1[j] comes before word2[j]
                if (graph[word1[j]].find(word2[j]) == graph[word1[j]].end()) {
                    graph[word1[j]].insert(word2[j]);
                    inDegree[word2[j]]++;
                }
                break;
            }
        }
    }

    // Topological sort using Kahn's algorithm (BFS)
    std::queue<char> queue;
    for (const auto& pair : inDegree) {
        if (pair.second == 0) {
            queue.push(pair.first);
        }
    }

    std::string result;
    while (!queue.empty()) {
        char current = queue.front();
        queue.pop();
        result += current;

        for (char neighbor : graph[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                queue.push(neighbor);
            }
        }
    }

    // Check if we included all characters (if not, there's a cycle)
    if (result.size() != inDegree.size()) {
        return "";
    }

    return result;
}
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll be diving into **Shortest Path Algorithms**, which are specialized graph algorithms for finding the most efficient routes between vertices. We'll explore:

- Dijkstra's Algorithm for single-source shortest paths
- Bellman-Ford Algorithm for graphs with negative weights
- Floyd-Warshall Algorithm for all-pairs shortest paths
- Applications in route planning, network routing, and more
- Optimizations and practical implementations

_If graph traversals are about exploring and visiting every part of a network, shortest path algorithms are about finding the most efficient routes through that network. These algorithms are what power your GPS navigation, optimize network traffic, route internet packets, and solve countless other optimization problems. Join us tomorrow as we discover how to navigate graphs in the most efficient way possible!_

#DSAin45 #Day30 #GraphTraversals #BFS #DFS #DataStructures #Algorithms #CPlusPlus #NetworkAnalysis #PathFinding
