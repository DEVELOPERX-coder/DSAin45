# Real-World Applications of Deques & Priority Queues

## Deque Applications

### 1. Web Browser Navigation History

The back and forward buttons in web browsers are powered by a deque-like structure. New pages are added to the top as you browse, and you can move backward and forward through this history.

```cpp
class BrowserHistory {
private:
    std::deque<std::string> history;
    int currentIndex;

public:
    BrowserHistory(const std::string& homepage) {
        history.push_back(homepage);
        currentIndex = 0;
    }

    void visit(const std::string& url) {
        // Remove all forward history
        while (history.size() > currentIndex + 1) {
            history.pop_back();
        }

        // Add the new page
        history.push_back(url);
        currentIndex++;
    }

    std::string back(int steps) {
        int actualSteps = std::min(steps, currentIndex);
        currentIndex -= actualSteps;
        return history[currentIndex];
    }

    std::string forward(int steps) {
        int actualSteps = std::min(steps, (int)history.size() - currentIndex - 1);
        currentIndex += actualSteps;
        return history[currentIndex];
    }
};
```

### 2. Text Editor Undo/Redo Functionality

Most text editors use a deque-based approach for undo and redo operations.

```cpp
class TextEditor {
private:
    std::string currentText;
    std::deque<std::string> undoStack;
    std::deque<std::string> redoStack;

public:
    void type(const std::string& text) {
        undoStack.push_back(currentText);
        redoStack.clear(); // Clear redo history
        currentText += text;
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push_back(currentText);
            currentText = undoStack.back();
            undoStack.pop_back();
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push_back(currentText);
            currentText = redoStack.back();
            redoStack.pop_back();
        }
    }
};
```

### 3. Task Scheduling and Process Management

Operating systems use deques for various scheduling tasks.

```cpp
class TaskScheduler {
private:
    std::deque<Task> regularTasks;
    std::deque<Task> highPriorityTasks;

public:
    void addTask(const Task& task, bool isHighPriority = false) {
        if (isHighPriority) {
            highPriorityTasks.push_back(task);
        } else {
            regularTasks.push_back(task);
        }
    }

    Task getNextTask() {
        if (!highPriorityTasks.empty()) {
            Task task = highPriorityTasks.front();
            highPriorityTasks.pop_front();
            return task;
        } else if (!regularTasks.empty()) {
            Task task = regularTasks.front();
            regularTasks.pop_front();
            return task;
        }
        throw std::runtime_error("No tasks available");
    }

    void addUrgentTask(const Task& task) {
        // Add to front of high priority queue
        highPriorityTasks.push_front(task);
    }
};
```

### 4. Palindrome Checking

Deques are perfect for checking if a string is a palindrome.

```cpp
bool isPalindrome(const std::string& str) {
    std::deque<char> charDeque;

    // Push all characters to deque (ignoring non-alphanumeric)
    for (char c : str) {
        if (std::isalnum(c)) {
            charDeque.push_back(std::tolower(c));
        }
    }

    // Compare characters from both ends
    while (charDeque.size() > 1) {
        if (charDeque.front() != charDeque.back()) {
            return false;
        }
        charDeque.pop_front();
        charDeque.pop_back();
    }

    return true;
}
```

### 5. Sliding Window Maximum

A classic algorithmic problem efficiently solved using a deque.

```cpp
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    std::deque<int> dq; // Store indices of potential maximum elements

    for (int i = 0; i < nums.size(); i++) {
        // Remove elements outside the current window
        while (!dq.empty() && dq.front() < i - k + 1) {
            dq.pop_front();
        }

        // Remove smaller elements (they can't be maximum in current or future windows)
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }

        // Add current element
        dq.push_back(i);

        // Add to result if we've processed at least k elements
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }

    return result;
}
```

## Priority Queue Applications

### 1. Dijkstra's Shortest Path Algorithm

Finding the shortest path in a weighted graph.

```cpp
void dijkstra(const Graph& graph, int start, std::vector<int>& distances) {
    int n = graph.numVertices();
    distances.assign(n, INT_MAX);
    distances[start] = 0;

    // Min priority queue of (distance, vertex) pairs
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Skip outdated entries
        if (dist > distances[u]) continue;

        // Process all neighbors of u
        for (const auto& [v, weight] : graph.getNeighbors(u)) {
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                pq.push({distances[v], v});
            }
        }
    }
}
```

### 2. Huffman Coding for Data Compression

Building optimal prefix codes for lossless data compression.

```cpp
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    // Create a min priority queue
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto& pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build Huffman Tree
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        // Create a new internal node with frequency equal to the sum
        HuffmanNode* newNode = new HuffmanNode('\0', left->freq + right->freq, left, right);
        pq.push(newNode);
    }

    return pq.top(); // Root of Huffman tree
}
```

### 3. A\* Pathfinding Algorithm

Used in games and navigation systems.

```cpp
Path aStar(const Grid& grid, Point start, Point goal) {
    // Priority queue ordered by f-value (g + h)
    std::priority_queue<Node, std::vector<Node>, CompareNodes> openSet;
    std::unordered_set<Point> closedSet;

    openSet.push({start, 0, heuristic(start, goal)});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.point == goal) {
            return reconstructPath(current);
        }

        if (closedSet.find(current.point) != closedSet.end()) {
            continue;
        }

        closedSet.insert(current.point);

        for (Point neighbor : grid.getNeighbors(current.point)) {
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            int gScore = current.g + 1; // Cost to move to neighbor
            int fScore = gScore + heuristic(neighbor, goal);

            openSet.push({neighbor, gScore, fScore, &current});
        }
    }

    return {}; // No path found
}
```

### 4. Process Scheduling in Operating Systems

Determining which process runs next on the CPU.

```cpp
class ProcessScheduler {
private:
    std::priority_queue<Process, std::vector<Process>, CompareProcessPriority> readyQueue;

public:
    void addProcess(const Process& process) {
        readyQueue.push(process);
    }

    Process getNextProcess() {
        if (readyQueue.empty()) {
            throw std::runtime_error("No processes available");
        }

        Process next = readyQueue.top();
        readyQueue.pop();
        return next;
    }
};
```

### 5. Event-Driven Simulation

Discrete event simulations for modeling complex systems.

```cpp
class EventSimulation {
private:
    // Priority queue ordered by event time
    std::priority_queue<Event, std::vector<Event>, CompareEventTime> eventQueue;
    double currentTime;

public:
    EventSimulation() : currentTime(0.0) {}

    void scheduleEvent(double time, const EventData& data) {
        eventQueue.push({time, data});
    }

    void run() {
        while (!eventQueue.empty()) {
            Event e = eventQueue.top();
            eventQueue.pop();

            // Update simulation time
            currentTime = e.time;

            // Process the event
            processEvent(e);
        }
    }
};
```

### 6. Median Maintenance

Finding the running median of a stream of numbers.

```cpp
class MedianFinder {
private:
    std::priority_queue<int> maxHeap; // Lower half
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // Upper half

public:
    void addNum(int num) {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        // Balance heaps
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        } else {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }
};
```

### 7. Network Packet Processing

Routers use priority queues to determine which packets to process first.

```cpp
class PacketRouter {
private:
    std::priority_queue<Packet, std::vector<Packet>, ComparePacketPriority> queue;

public:
    void receivePacket(const Packet& packet) {
        queue.push(packet);
    }

    void processPackets(int maxPackets) {
        int count = 0;
        while (!queue.empty() && count < maxPackets) {
            Packet p = queue.top();
            queue.pop();

            // Process the packet
            routePacket(p);

            count++;
        }
    }
};
```

## Industry-Specific Applications

### Banking and Finance

- **Transaction Processing**: Priority queues ensure high-value transactions or time-sensitive trades are processed first.
- **Fraud Detection**: Suspicious activities are prioritized for immediate investigation.

### Healthcare

- **Emergency Room Triage**: Patients are treated based on severity, not arrival time.
- **Organ Transplant Lists**: Multiple factors determine priority for receiving limited donor organs.

### Telecommunications

- **Call Centers**: VIP customers or emergency calls are prioritized in the queue.
- **Network Traffic Management**: Quality of Service (QoS) implementations prioritize certain types of packets.

### Web Services

- **Request Throttling**: High-priority API requests are processed before lower-priority ones when under heavy load.
- **Content Delivery Networks**: Prioritize which content to cache based on popularity.

### Transportation and Logistics

- **Air Traffic Control**: Landing slots are assigned based on various priority factors.
- **Package Delivery**: Premium shipping options get prioritized handling.

### Operating Systems

- **I/O Request Scheduling**: Disk and network I/O requests are scheduled using variations of priority queues.
- **Memory Management**: Page replacement algorithms often use priority-based structures.

### Artificial Intelligence

- **Game AI**: Decision trees use priority queues to evaluate the most promising moves first.
- **Machine Learning**: Implementations of gradient descent often prioritize updating certain weights first.
