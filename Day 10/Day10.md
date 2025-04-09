# 🔄 DAY 10: Deques & Priority Queues - The Swiss Army Knives of Data Structures #DSAin45

Welcome to Day 10! Yesterday we explored queues—where items patiently wait their turn in FIFO order. Today, we're diving into two powerful variations: Deques (Double-Ended Queues) and Priority Queues. These are the "advanced" versions of queues that bring additional superpowers to your data structure arsenal.

_If a queue is like waiting in line at a fast-food restaurant, a deque is like those fancy restaurants where you can enter and exit through both the front door and the back door. A priority queue is like a hospital emergency room, where the most urgent cases get treated first regardless of arrival time._

## 🤔 What Is a Deque (Double-Ended Queue)?

A deque (pronounced "deck") is a linear data structure that supports insertion and removal of elements from both ends. It combines the features of both stacks and queues, allowing you to use it as either or both simultaneously.

```
       ┌─────────┬─────────┬─────────┬─────────┐
 ↔ Add/| Element1 | Element2 | Element3 | Element4 | Add/ ↔
 Remove|         |         |         |         | Remove
       └─────────┴─────────┴─────────┴─────────┘
       Front                                 Rear
```

> **🧠 Fun Fact**: The term "deque" is a contraction of "double-ended queue." The name itself is somewhat of a computer science pun—taking "queue" and making it work at both ends!

## 📋 Deque Operations

Deques support operations at both ends:

- **push_front**: Add an element to the front
- **push_back**: Add an element to the back
- **pop_front**: Remove the front element
- **pop_back**: Remove the back element
- **front**: View the front element without removing it
- **back**: View the back element without removing it
- **isEmpty**: Check if the deque is empty
- **size**: Return the number of elements in the deque

_The beauty of a deque is its versatility: use push_front/pop_front for a stack, push_back/pop_front for a queue, or mix them all however you need. It's like having a multi-tool when everyone else is carrying a single wrench._

## 🏗️ Implementing a Deque in C++

There are multiple ways to implement a deque:

### 1. Using a Doubly Linked List

```cpp
#include <iostream>

template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* front_node;
    Node* back_node;
    size_t deque_size;

public:
    // Constructor
    Deque() : front_node(nullptr), back_node(nullptr), deque_size(0) {}

    // Destructor
    ~Deque() {
        clear();
    }

    // Push to the front
    void push_front(const T& value) {
        Node* new_node = new Node(value);

        if (isEmpty()) {
            front_node = back_node = new_node;
        } else {
            new_node->next = front_node;
            front_node->prev = new_node;
            front_node = new_node;
        }

        deque_size++;
    }

    // Push to the back
    void push_back(const T& value) {
        Node* new_node = new Node(value);

        if (isEmpty()) {
            front_node = back_node = new_node;
        } else {
            new_node->prev = back_node;
            back_node->next = new_node;
            back_node = new_node;
        }

        deque_size++;
    }

    // Pop from the front
    T pop_front() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }

        Node* temp = front_node;
        T value = temp->data;

        if (front_node == back_node) {
            // Only one element
            front_node = back_node = nullptr;
        } else {
            front_node = front_node->next;
            front_node->prev = nullptr;
        }

        delete temp;
        deque_size--;
        return value;
    }

    // Pop from the back
    T pop_back() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }

        Node* temp = back_node;
        T value = temp->data;

        if (front_node == back_node) {
            // Only one element
            front_node = back_node = nullptr;
        } else {
            back_node = back_node->prev;
            back_node->next = nullptr;
        }

        delete temp;
        deque_size--;
        return value;
    }

    // Get front element
    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return front_node->data;
    }

    // Get back element
    T back() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return back_node->data;
    }

    // Check if deque is empty
    bool isEmpty() const {
        return front_node == nullptr;
    }

    // Get the size of the deque
    size_t size() const {
        return deque_size;
    }

    // Clear the deque
    void clear() {
        while (!isEmpty()) {
            pop_front();
        }
    }
};
```

_A doubly linked list is perfect for a deque implementation—like a reversible jacket, it works well in both directions._

### 2. Using a Dynamic Array

```cpp
#include <iostream>
#include <vector>

template <typename T>
class ArrayDeque {
private:
    std::vector<T> elements;
    size_t front_index;
    size_t back_index;
    size_t deque_size;

public:
    // Constructor
    ArrayDeque() : front_index(0), back_index(0), deque_size(0) {}

    // Push to the front
    void push_front(const T& value) {
        // If empty or front_index is 0, resize the array
        if (elements.empty() || front_index == 0) {
            // Allocate more space at the beginning
            size_t new_capacity = std::max(size_t(1), elements.size() * 2);
            std::vector<T> new_elements(new_capacity);

            // Copy existing elements to the middle of the new array
            size_t new_front = new_capacity / 2;
            for (size_t i = 0; i < deque_size; i++) {
                new_elements[new_front + i] = elements[(front_index + i) % elements.size()];
            }

            elements = std::move(new_elements);
            front_index = new_front;
            back_index = front_index + deque_size;
        }

        front_index--;
        elements[front_index] = value;
        deque_size++;
    }

    // Push to the back
    void push_back(const T& value) {
        // If empty or back_index is at the end, resize the array
        if (elements.empty() || back_index == elements.size()) {
            // Allocate more space
            size_t new_capacity = std::max(size_t(1), elements.size() * 2);
            std::vector<T> new_elements(new_capacity);

            // Copy existing elements to the beginning of the new array
            for (size_t i = 0; i < deque_size; i++) {
                new_elements[i] = elements[(front_index + i) % elements.size()];
            }

            elements = std::move(new_elements);
            front_index = 0;
            back_index = deque_size;
        }

        elements[back_index] = value;
        back_index++;
        deque_size++;
    }

    // Pop from the front
    T pop_front() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }

        T value = elements[front_index];
        front_index++;
        deque_size--;

        // Reset indices if deque becomes empty
        if (isEmpty()) {
            front_index = back_index = 0;
        }

        return value;
    }

    // Pop from the back
    T pop_back() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }

        back_index--;
        T value = elements[back_index];
        deque_size--;

        // Reset indices if deque becomes empty
        if (isEmpty()) {
            front_index = back_index = 0;
        }

        return value;
    }

    // Get front element
    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return elements[front_index];
    }

    // Get back element
    T back() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return elements[back_index - 1];
    }

    // Check if deque is empty
    bool isEmpty() const {
        return deque_size == 0;
    }

    // Get the size of the deque
    size_t size() const {
        return deque_size;
    }
};
```

_An array-based deque implementation is trickier than it looks—like trying to build a two-way street on a one-way road. The key insight is clever index management and occasional resizing._

### 3. Using STL's `std::deque`

C++ provides a built-in deque implementation in the Standard Template Library:

```cpp
#include <iostream>
#include <deque>

int main() {
    std::deque<int> myDeque;

    // Add elements at both ends
    myDeque.push_front(10);
    myDeque.push_back(20);
    myDeque.push_front(5);
    myDeque.push_back(25);

    // Print the deque
    std::cout << "Deque contents: ";
    for (int num : myDeque) {
        std::cout << num << " ";
    }
    std::cout << std::endl;  // Output: 5 10 20 25

    // Access elements
    std::cout << "Front element: " << myDeque.front() << std::endl;  // 5
    std::cout << "Back element: " << myDeque.back() << std::endl;    // 25

    // Random access
    std::cout << "Element at index 1: " << myDeque[1] << std::endl;  // 10

    // Remove elements from both ends
    myDeque.pop_front();
    myDeque.pop_back();

    // Print the deque again
    std::cout << "Deque after pop operations: ";
    for (int num : myDeque) {
        std::cout << num << " ";
    }
    std::cout << std::endl;  // Output: 10 20

    return 0;
}
```

> **🧠 Fun Fact**: The STL's `std::deque` is typically implemented as a collection of fixed-size arrays with additional bookkeeping, providing a compromise between the constant time random access of arrays and the constant time insertion/deletion at ends of lists, while maintaining relatively efficient memory usage.

_Using STL's deque is like driving an automatic car instead of manually building the transmission—it handles the complex details so you can focus on solving the actual problem._

## ⚡ Deque Time & Space Complexity

A properly implemented deque should have the following complexities:

| Operation    | Time Complexity (Doubly Linked List) | Time Complexity (STL std::deque) |
| ------------ | ------------------------------------ | -------------------------------- |
| push_front() | O(1)                                 | O(1) amortized                   |
| push_back()  | O(1)                                 | O(1) amortized                   |
| pop_front()  | O(1)                                 | O(1) amortized                   |
| pop_back()   | O(1)                                 | O(1) amortized                   |
| front()      | O(1)                                 | O(1)                             |
| back()       | O(1)                                 | O(1)                             |
| at(i) / [i]  | O(n)                                 | O(1)                             |
| Space        | O(n)                                 | O(n)                             |

_Deques offer the best of both worlds: the efficiency of a stack or queue with more flexibility. It's like having a sports car that's also good at off-roading._

## 🌎 Applications of Deques

Deques shine in many scenarios due to their flexibility:

### 1. Sliding Window Problems

One of the most common applications of deques is solving sliding window maximum/minimum problems efficiently:

```cpp
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    std::deque<int> window;  // Stores indices of potentially maximum elements

    for (int i = 0; i < nums.size(); i++) {
        // Remove elements outside the current window
        while (!window.empty() && window.front() < i - k + 1) {
            window.pop_front();
        }

        // Remove smaller elements (they can't be maximum in current or future windows)
        while (!window.empty() && nums[window.back()] < nums[i]) {
            window.pop_back();
        }

        // Add current index
        window.push_back(i);

        // Add to result once we've processed the first k elements
        if (i >= k - 1) {
            result.push_back(nums[window.front()]);
        }
    }

    return result;
}
```

_The sliding window technique with a deque is like having a spotter at a race track who always knows who's in the lead among the last k runners—efficient and elegant._

### 2. Palindrome Checking

Deques are excellent for checking if a string is a palindrome:

```cpp
bool isPalindrome(const std::string& s) {
    std::deque<char> charDeque;

    // Add all characters to the deque (ignoring non-alphanumeric)
    for (char c : s) {
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

_Checking a palindrome with a deque is like having two people start at opposite ends of a tunnel and walking toward each other, checking if they see the same thing at each step._

### 3. Work Stealing Scheduling Algorithms

In parallel computing, deques are used in work-stealing schedulers:

```cpp
class WorkStealingScheduler {
private:
    std::vector<std::deque<Task>> workerQueues;
    int numWorkers;

public:
    WorkStealingScheduler(int n) : numWorkers(n), workerQueues(n) {}

    void addTask(int workerId, const Task& task) {
        workerQueues[workerId].push_back(task);
    }

    Task getTask(int workerId) {
        // Try to get a task from own queue
        if (!workerQueues[workerId].empty()) {
            Task task = workerQueues[workerId].front();
            workerQueues[workerId].pop_front();
            return task;
        }

        // Steal a task from another worker
        for (int i = 0; i < numWorkers; i++) {
            if (i != workerId && !workerQueues[i].empty()) {
                // Steal from the back (oldest task)
                Task task = workerQueues[i].back();
                workerQueues[i].pop_back();
                return task;
            }
        }

        // No tasks available
        return Task();
    }
};
```

_Work stealing with deques is like having multiple checkout lanes in a supermarket, where cashiers with no customers can "steal" customers from the back of other long lines—improving overall throughput._

### 4. Undo/Redo Functionality

Deques can be used to implement undo/redo features:

```cpp
class TextEditor {
private:
    std::string currentText;
    std::deque<std::string> undoStack;
    std::deque<std::string> redoStack;

public:
    void type(const std::string& text) {
        undoStack.push_front(currentText);
        redoStack.clear();  // Clear redo stack when new changes are made
        currentText += text;
    }

    void undo() {
        if (undoStack.empty()) {
            return;
        }

        redoStack.push_front(currentText);
        currentText = undoStack.front();
        undoStack.pop_front();
    }

    void redo() {
        if (redoStack.empty()) {
            return;
        }

        undoStack.push_front(currentText);
        currentText = redoStack.front();
        redoStack.pop_front();
    }

    std::string getText() const {
        return currentText;
    }
};
```

_The undo/redo pattern is like time travel for your document—deques provide the perfect vehicle for journeying back and forth through your editing history._

## 🏆 Priority Queues: When Order Is Determined by Importance

Now, let's dive into priority queues—data structures where elements have associated priorities and higher-priority elements are served before lower-priority ones.

### What Is a Priority Queue?

A priority queue is an abstract data type similar to a regular queue or stack, with an additional property: each element has a "priority" associated with it. Elements with higher priority are served before elements with lower priority.

```
  Priority: 5    Priority: 3    Priority: 7    Priority: 1
  +--------+    +--------+    +--------+    +--------+
  |  Item  |    |  Item  |    |  Item  |    |  Item  |
  +--------+    +--------+    +--------+    +--------+
      |             |             |             |
      V             V             V             V
  Dequeued in order: Item with Priority 7, Item with Priority 5, Item with Priority 3, Item with Priority 1
```

> **🧠 Fun Fact**: Priority queues are the core data structure behind Dijkstra's algorithm for finding shortest paths in graphs, Huffman coding for data compression, and even modern operating system schedulers!

### Implementing a Priority Queue

The most efficient implementation of a priority queue uses a data structure called a heap (typically a binary heap). A heap is a specialized tree-based structure that satisfies the heap property.

#### Using STL's `std::priority_queue`

C++ provides a built-in priority queue implementation:

```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <string>

int main() {
    // Max priority queue (default): largest value has highest priority
    std::priority_queue<int> maxPQ;

    maxPQ.push(10);
    maxPQ.push(30);
    maxPQ.push(20);
    maxPQ.push(5);

    std::cout << "Max Priority Queue (descending order):" << std::endl;
    while (!maxPQ.empty()) {
        std::cout << maxPQ.top() << " ";  // Output: 30 20 10 5
        maxPQ.pop();
    }
    std::cout << std::endl;

    // Min priority queue: smallest value has highest priority
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;

    minPQ.push(10);
    minPQ.push(30);
    minPQ.push(20);
    minPQ.push(5);

    std::cout << "Min Priority Queue (ascending order):" << std::endl;
    while (!minPQ.empty()) {
        std::cout << minPQ.top() << " ";  // Output: 5 10 20 30
        minPQ.pop();
    }
    std::cout << std::endl;

    // Custom comparator for a complex object
    struct Patient {
        std::string name;
        int priority;  // Lower number means higher priority

        Patient(const std::string& n, int p) : name(n), priority(p) {}

        // For std::priority_queue, we need to define operator<
        bool operator<(const Patient& other) const {
            // This is reversed because std::priority_queue is a max-heap by default
            return priority > other.priority;
        }
    };

    std::priority_queue<Patient> emergencyRoom;

    emergencyRoom.push(Patient("Alice", 3));  // Regular
    emergencyRoom.push(Patient("Bob", 1));    // Critical
    emergencyRoom.push(Patient("Charlie", 2));  // Urgent
    emergencyRoom.push(Patient("Dave", 4));   // Non-urgent

    std::cout << "Emergency Room Queue (by priority):" << std::endl;
    while (!emergencyRoom.empty()) {
        Patient p = emergencyRoom.top();
        std::cout << p.name << " (Priority: " << p.priority << ")" << std::endl;
        emergencyRoom.pop();
    }

    return 0;
}
```

_The STL priority queue is like a smart bouncer who always knows who to let into the club next based on your custom VIP ranking system._

### Time & Space Complexity of Priority Queue Operations

| Operation | Time Complexity | Space Complexity |
| --------- | --------------- | ---------------- |
| insert    | O(log n)        | O(n)             |
| remove    | O(log n)        | O(n)             |
| peek      | O(1)            | O(n)             |

_Priority queues sacrifice the O(1) insertion and deletion of regular queues for the ability to always grab the highest-priority element. It's a trade-off worth making for many problems._

### Priority Queue Applications

Priority queues are ubiquitous in computing:

#### 1. Dijkstra's Shortest Path Algorithm

```cpp
void dijkstra(const Graph& graph, int source, std::vector<int>& distances) {
    int n = graph.numVertices();
    distances.assign(n, INT_MAX);
    distances[source] = 0;

    // Min priority queue: {distance, vertex}
    std::priority_queue<std::pair<int, int>,
                       std::vector<std::pair<int, int>>,
                       std::greater<std::pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Skip outdated entries
        if (dist > distances[u]) continue;

        // Explore neighbors
        for (const auto& [v, weight] : graph.getNeighbors(u)) {
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                pq.push({distances[v], v});
            }
        }
    }
}
```

_Dijkstra's algorithm with a priority queue is like a smart GPS system that always explores the most promising routes first—minimizing wasted effort._

#### 2. Huffman Coding (Data Compression)

```cpp
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f, Node* l = nullptr, Node* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}

    bool operator>(const Node& other) const {
        return freq > other.freq;
    }
};

void buildHuffmanTree(const std::string& text) {
    // Count frequency of each character
    std::unordered_map<char, int> freqMap;
    for (char c : text) {
        freqMap[c]++;
    }

    // Create leaf nodes and add them to the priority queue
    std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> pq;
    for (const auto& [ch, freq] : freqMap) {
        pq.push(new Node(ch, freq));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        // Extract the two nodes with lowest frequency
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // Create new internal node with these two as children
        // and with frequency equal to the sum of both
        pq.push(new Node('\0', left->freq + right->freq, left, right));
    }

    // The remaining node is the root of the Huffman tree
    Node* root = pq.top();

    // Generate Huffman codes by traversing the tree
    // (Code not shown for brevity)
}
```

_Huffman coding with a priority queue is like a compression artist who knows exactly which characters to encode with fewer bits based on their frequency._

#### 3. Process Scheduling in Operating Systems

```cpp
struct Process {
    int id;
    int priority;
    int burstTime;

    Process(int i, int p, int b) : id(i), priority(p), burstTime(b) {}

    bool operator<(const Process& other) const {
        return priority < other.priority;
    }
};

void scheduleProcesses(const std::vector<Process>& processes) {
    std::priority_queue<Process> scheduler;

    // Add all processes to the scheduler
    for (const Process& p : processes) {
        scheduler.push(p);
    }

    std::cout << "Process execution order:" << std::endl;
    while (!scheduler.empty()) {
        Process current = scheduler.top();
        scheduler.pop();

        std::cout << "Executing Process " << current.id
                  << " (Priority: " << current.priority
                  << ", Burst Time: " << current.burstTime << ")" << std::endl;

        // In a real scheduler, we would handle time slices,
        // process states, etc.
    }
}
```

_OS schedulers using priority queues are like efficient project managers who always know which task needs attention next._

#### 4. Event-Driven Simulation

```cpp
struct Event {
    double time;
    std::string type;
    int entityId;

    Event(double t, const std::string& tp, int id)
        : time(t), type(tp), entityId(id) {}

    bool operator<(const Event& other) const {
        return time > other.time;  // Earlier time has higher priority
    }
};

class Simulation {
private:
    std::priority_queue<Event> eventQueue;
    double currentTime;

public:
    Simulation() : currentTime(0.0) {}

    void scheduleEvent(const Event& event) {
        eventQueue.push(event);
    }

    void run() {
        while (!eventQueue.empty()) {
            Event current = eventQueue.top();
            eventQueue.pop();

            currentTime = current.time;

            std::cout << "Time " << currentTime
                      << ": Processing event '" << current.type
                      << "' for entity " << current.entityId << std::endl;

            // Process the event and potentially generate new events
            // (specific logic depends on the simulation)
        }
    }
};
```

_Event-driven simulations with priority queues are like having a perfect timeline of the future, always knowing exactly which event happens next._

## 🧩 Comparing Deques and Priority Queues

| Feature                 | Deque                              | Priority Queue                     |
| ----------------------- | ---------------------------------- | ---------------------------------- |
| Ordering                | Based on insertion position        | Based on priority                  |
| Main Operations         | push_front/back, pop_front/back    | push, pop, top                     |
| Implementation          | Doubly linked list, dynamic arrays | Heap (binary, Fibonacci, etc.)     |
| Access Pattern          | Both ends                          | Highest priority element only      |
| Time Complexity: Insert | O(1)                               | O(log n)                           |
| Time Complexity: Delete | O(1)                               | O(log n)                           |
| Best Used For           | Double-ended access patterns       | Always processing "most important" |

_Deques and priority queues are like different tools in your toolbox—each has specific scenarios where it shines. Choose wisely based on your access patterns._

## 🎭 Common Problems and Solutions

### 1. Balancing Deque Size

When implementing a deque with a dynamic array, it's important to maintain the balance of the array. If all elements shift to one end, we may want to rebalance:

```cpp
void rebalance() {
    if (isEmpty()) return;

    std::vector<T> new_elements(elements.size());
    size_t new_front = elements.size() / 4;

    // Copy elements to the rebalanced position
    for (size_t i = 0; i < deque_size; i++) {
        new_elements[new_front + i] = elements[(front_index + i) % elements.size()];
    }

    elements = std::move(new_elements);
    front_index = new_front;
    back_index = front_index + deque_size;
}
```

_Rebalancing a deque array is like reorganizing your bookshelf when all the books pile up on one side—it recenters everything for more efficient future access._

### 2. Combining Priority Queue with Timestamps for FIFO Tie-Breaking

Sometimes we want a priority queue that uses FIFO order for items with the same priority:

```cpp
template <typename T>
class FifoPriorityQueue {
private:
    struct Item {
        T data;
        int priority;
        int timestamp;

        Item(const T& d, int p, int t) : data(d), priority(p), timestamp(t) {}

        bool operator<(const Item& other) const {
            if (priority != other.priority) {
                return priority < other.priority;
            }
            return timestamp > other.timestamp;  // Earlier timestamp wins
        }
    };

    std::priority_queue<Item> pq;
    int currentTimestamp;

public:
    FifoPriorityQueue() : currentTimestamp(0) {}

    void push(const T& item, int priority) {
        pq.push(Item(item, priority, currentTimestamp++));
    }

    T pop() {
        T result = pq.top().data;
        pq.pop();
        return result;
    }

    bool empty() const {
        return pq.empty();
    }
};
```

_A FIFO-prioritized queue is like a coffee shop where people with the same rewards level get served in the order they arrived—preserving fairness within priority tiers._

### 3. Multi-Element Peek with Deques

Sometimes we need to look at multiple elements at the ends of a deque:

```cpp
template <typename T>
std::vector<T> peekFront(const std::deque<T>& dq, size_t n) {
    std::vector<T> result;
    result.reserve(n);

    for (size_t i = 0; i < n && i < dq.size(); i++) {
        result.push_back(dq[i]);
    }

    return result;
}

template <typename T>
std::vector<T> peekBack(const std::deque<T>& dq, size_t n) {
    std::vector<T> result;
    result.reserve(n);

    size_t size = dq.size();
    for (size_t i = 0; i < n && i < size; i++) {
        result.push_back(dq[size - 1 - i]);
    }

    std::reverse(result.begin(), result.end());
    return result;
}
```

_Multi-element peeking is like window shopping—looking at several items without removing them from the display._

## 🚀 Advanced Applications

### 1. Work-Stealing Task Schedulers

Modern parallel computing frameworks use deques for work-stealing:

```cpp
class Worker {
private:
    std::deque<Task> tasks;
    std::mutex mutex;

public:
    void addTask(const Task& task) {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push_back(task);
    }

    Task getTask() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) {
            return Task();  // Empty task
        }

        Task task = tasks.front();
        tasks.pop_front();
        return task;
    }

    Task stealTask() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) {
            return Task();  // Empty task
        }

        // Steal from the back
        Task task = tasks.back();
        tasks.pop_back();
        return task;
    }
};
```

_Work stealing is like a dynamic team where members who finish their work early help those who have more to do—maximizing throughput._

### 2. A\* Pathfinding Algorithm

A\* uses a priority queue to efficiently find optimal paths:

```cpp
struct Node {
    int x, y;
    int g_cost;  // Cost from start
    int h_cost;  // Heuristic (estimated cost to goal)
    Node* parent;

    Node(int x, int y, int g, int h, Node* p = nullptr)
        : x(x), y(y), g_cost(g), h_cost(h), parent(p) {}

    int f_cost() const {
        return g_cost + h_cost;
    }

    bool operator<(const Node& other) const {
        return f_cost() > other.f_cost();  // Lower f_cost has higher priority
    }
};

std::vector<std::pair<int, int>> findPath(Grid& grid, Point start, Point goal) {
    std::priority_queue<Node> openSet;
    std::unordered_set<std::pair<int, int>> closedSet;

    openSet.push(Node(start.x, start.y, 0, heuristic(start, goal)));

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        // If we've reached the goal
        if (current.x == goal.x && current.y == goal.y) {
            // Reconstruct path
            std::vector<std::pair<int, int>> path;
            // ... (code to reconstruct path from current.parent)
            return path;
        }

        closedSet.insert({current.x, current.y});

        // For each neighbor
        for (const auto& [nx, ny] : getNeighbors(current.x, current.y)) {
            if (closedSet.find({nx, ny}) != closedSet.end() || !grid.isWalkable(nx, ny)) {
                continue;
            }

            int new_g = current.g_cost + 1;  // Assume cost of 1 to move

            openSet.push(Node(nx, ny, new_g, heuristic({nx, ny}, goal), &current));
        }
    }

    return {};  // No path found
}
```

_A\* pathfinding is like a smart explorer who always checks the most promising directions first—using a priority queue to focus on paths likely to reach the goal._

### 3. LRU Cache with O(1) Operations

Combining a hash map and a deque to create an efficient LRU (Least Recently Used) cache:

```cpp
template <typename Key, typename Value>
class LRUCache {
private:
    int capacity;
    std::list<std::pair<Key, Value>> items;  // std::list is a doubly linked list, similar to deque
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    Value get(const Key& key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            throw std::out_of_range("Key not found");
        }

        // Move to the front (most recently used)
        items.splice(items.begin(), items, it->second);
        return it->second->second;
    }

    void put(const Key& key, const Value& value) {
        auto it = cache.find(key);

        // Key exists, update value and move to front
        if (it != cache.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }

        // Cache is full, remove least recently used item
        if (items.size() == capacity) {
            Key oldKey = items.back().first;
            items.pop_back();
            cache.erase(oldKey);
        }

        // Add new item to the front
        items.push_front({key, value});
        cache[key] = items.begin();
    }
};
```

_An LRU cache is like a bookshelf where you always put recently used books at one end, and when the shelf gets full, you remove books from the other end—maintaining quick access to frequently used items._

## 📚 LeetCode Practice Problems

To reinforce today's concepts, try these LeetCode problems:

### Deque Problems:

**Easy:**

- #933: Number of Recent Calls
- #346: Moving Average from Data Stream
- #1700: Number of Students Unable to Eat Lunch

**Medium:**

- #641: Design Circular Deque
- #239: Sliding Window Maximum
- #1438: Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit

**Hard:**

- #1499: Max Value of Equation
- #1425: Constrained Subsequence Sum

### Priority Queue Problems:

**Medium:**

- #215: Kth Largest Element in an Array
- #347: Top K Frequent Elements
- #973: K Closest Points to Origin
- #1046: Last Stone Weight

**Hard:**

- #23: Merge k Sorted Lists
- #295: Find Median from Data Stream
- #630: Course Schedule III
- #1354: Construct Target Array With Multiple Sums

_These LeetCode problems are the fitness center for your deque and priority queue muscles—challenging at first, but they'll make you stronger with each repetition._

## 😂 Deque and Priority Queue Jokes

- Why is a deque always welcome at parties? Because it's so versatile—it can join from the front or back, and leave from either end too!

- Priority queue dating advice: It doesn't matter when you arrived on the dating scene—what matters is how high your priority is.

- What do you call a priority queue with messed-up priorities? A random data structure.

- I told my deque it needed to lose weight, so it started doing push_fronts and pop_backs. Programmers' aerobics!

- Why did the priority queue go to therapy? It had trouble prioritizing its own needs.

## 🔮 Tomorrow's Preview:

Tomorrow we'll dive into hash tables—perhaps the most versatile and widely used data structure in modern programming. We'll explore how they achieve near-constant time lookups through the magic of hashing, how to handle collisions, and why they're the backbone of modern languages' associative array implementations.

_Hash tables are like librarians with incredible memory—they can tell you exactly where to find any book in constant time, without even checking the catalog!_

#DSAin45 #Day10 #Deque #PriorityQueue #CPlusPlus #DataStructures #CodingInterview #FIFO
