# 🔄 DAY 10: Deques & Priority Queues - The Swiss Army Knives of Data Structures #DSAin45

Welcome to Day 10! Yesterday we explored queues—those orderly data structures that believe in fairness and FIFO principles. Today, we're leveling up with two powerful variations: Deques (Double-Ended Queues) and Priority Queues—the data structures that bring flexibility and importance ranking to our algorithmic toolkit.

_In data structures, as in life, sometimes you need more options than just 'get in line and wait your turn.' Deques let you cut both ends of the line, while priority queues ensure the VIPs always get served first._

## 🤔 What Is a Deque?

A deque (pronounced "deck") is a double-ended queue that allows insertion and removal of elements from both ends. Think of it as a more flexible data structure that combines the features of both stacks and queues.

```
             Front                       Back
   Insertion/Removal ◄─────────────► Insertion/Removal
                     ┌───┬───┬───┬───┐
                     │ A │ B │ C │ D │
                     └───┴───┴───┴───┘
```

> **🧠 Fun Fact**: The name "deque" is a shorthand for "double-ended queue." It's one of the most versatile container adaptors in the C++ STL, yet it's often underutilized compared to its siblings, vector and list.

## 🤔 What Is a Priority Queue?

A priority queue is a special type of queue where each element has a priority value, and elements with higher priorities are served before those with lower priorities, regardless of their order in the queue.

```
           Higher Priority             Lower Priority
           ┌───────────┐               ┌───────────┐
 Enqueue ─►│ Priority 1 │───►│───►│───►│ Priority 5 │─► Dequeue
           └───────────┘               └───────────┘
               VIP                        Economy
```

> **🧠 Fun Fact**: Priority queues are used in operating systems for process scheduling, where high-priority tasks (like system processes) need to execute before lower-priority ones (like background utilities), regardless of when they were initiated.

## 📋 Deque Operations

A deque supports the following operations:

- **push_front**: Add an element to the front
- **push_back**: Add an element to the back
- **pop_front**: Remove the front element
- **pop_back**: Remove the back element
- **front**: View the front element without removing it
- **back**: View the back element without removing it
- **isEmpty**: Check if the deque is empty
- **size**: Return the number of elements

_A deque is like a convertible car with both a front and back entrance—you can hop in or out from either end, giving you the ultimate flexibility in how you traverse the road of data._

## 📋 Priority Queue Operations

A priority queue typically supports these operations:

- **push**: Insert an element with a priority
- **pop**: Remove the highest (or lowest) priority element
- **top**: View the highest (or lowest) priority element
- **isEmpty**: Check if the priority queue is empty
- **size**: Return the number of elements

_A priority queue is like a hospital emergency room—the severity of your condition (priority) determines how quickly you'll be seen, not just when you arrived._

## 🏗️ Implementing a Deque in C++

### 1. Deque Using a Doubly Linked List

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

    // Destructor - clean up memory
    ~Deque() {
        while (!isEmpty()) {
            pop_front();
        }
    }

    // Add element to the front
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

    // Add element to the back
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

    // Remove element from the front
    void pop_front() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }

        Node* temp = front_node;

        if (front_node == back_node) {
            // Only one element
            front_node = back_node = nullptr;
        } else {
            front_node = front_node->next;
            front_node->prev = nullptr;
        }

        delete temp;
        deque_size--;
    }

    // Remove element from the back
    void pop_back() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }

        Node* temp = back_node;

        if (front_node == back_node) {
            // Only one element
            front_node = back_node = nullptr;
        } else {
            back_node = back_node->prev;
            back_node->next = nullptr;
        }

        delete temp;
        deque_size--;
    }

    // Get the front element
    T getFront() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }

        return front_node->data;
    }

    // Get the back element
    T getBack() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }

        return back_node->data;
    }

    // Check if deque is empty
    bool isEmpty() const {
        return front_node == nullptr;
    }

    // Get the number of elements
    size_t size() const {
        return deque_size;
    }
};
```

_Implementing a deque with a doubly linked list is like setting up a two-way street—traffic (data) can flow smoothly in both directions, with nodes acting as intersections connecting the paths._

### 2. Deque Using a Dynamic Array (Simplified Version)

```cpp
#include <iostream>
#include <vector>

template <typename T>
class ArrayDeque {
private:
    std::vector<T> elements;

public:
    // Add element to the front
    void push_front(const T& value) {
        elements.insert(elements.begin(), value);
    }

    // Add element to the back
    void push_back(const T& value) {
        elements.push_back(value);
    }

    // Remove element from the front
    void pop_front() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }
        elements.erase(elements.begin());
    }

    // Remove element from the back
    void pop_back() {
        if (isEmpty()) {
            throw std::underflow_error("Deque underflow");
        }
        elements.pop_back();
    }

    // Get the front element
    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return elements.front();
    }

    // Get the back element
    T back() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return elements.back();
    }

    // Check if deque is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Get the number of elements
    size_t size() const {
        return elements.size();
    }
};
```

_This array-based deque implementation is like a line of people who constantly have to shuffle forward whenever someone new joins at the front—not the most efficient system in a crowded venue!_

### 3. Using STL's `std::deque`

C++ provides a built-in deque implementation in the Standard Template Library:

```cpp
#include <iostream>
#include <deque>

int main() {
    std::deque<int> dq;

    // Add elements at both ends
    dq.push_front(10); // [10]
    dq.push_back(20);  // [10, 20]
    dq.push_front(5);  // [5, 10, 20]
    dq.push_back(25);  // [5, 10, 20, 25]

    // Access elements
    std::cout << "Front element: " << dq.front() << std::endl; // 5
    std::cout << "Back element: " << dq.back() << std::endl;   // 25

    // Random access
    std::cout << "Element at index 2: " << dq[2] << std::endl; // 20

    // Remove from both ends
    dq.pop_front();    // [10, 20, 25]
    dq.pop_back();     // [10, 20]

    // Size
    std::cout << "Size: " << dq.size() << std::endl; // 2

    return 0;
}
```

_Using STL's deque is like having a valet service for your car—they'll park it and retrieve it from either end of the lot, and you don't have to worry about the implementation details._

> **🧠 Fun Fact**: Unlike `std::vector`, which stores elements in a single contiguous memory block, `std::deque` typically uses multiple fixed-size arrays connected together. This allows efficient insertion and deletion at both ends without requiring reallocation of the entire structure.

## 🏗️ Implementing a Priority Queue in C++

### 1. Priority Queue Using a Heap

```cpp
#include <iostream>
#include <vector>
#include <functional> // For std::greater

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> heap;
    Compare comp;

    // Helper functions for maintaining heap property
    size_t parent(size_t i) { return (i - 1) / 2; }
    size_t leftChild(size_t i) { return 2 * i + 1; }
    size_t rightChild(size_t i) { return 2 * i + 2; }

    void heapifyUp(size_t i) {
        while (i > 0 && comp(heap[parent(i)], heap[i])) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(size_t i) {
        size_t maxIndex = i;
        size_t left = leftChild(i);

        if (left < heap.size() && comp(heap[maxIndex], heap[left])) {
            maxIndex = left;
        }

        size_t right = rightChild(i);
        if (right < heap.size() && comp(heap[maxIndex], heap[right])) {
            maxIndex = right;
        }

        if (i != maxIndex) {
            std::swap(heap[i], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }

public:
    // Constructor
    PriorityQueue() {}

    // Add element to the priority queue
    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    // Remove the highest priority element
    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("Priority queue underflow");
        }

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    // Get the highest priority element
    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Priority queue is empty");
        }

        return heap[0];
    }

    // Check if priority queue is empty
    bool isEmpty() const {
        return heap.empty();
    }

    // Get the number of elements
    size_t size() const {
        return heap.size();
    }
};
```

_A heap-based priority queue is like a company hierarchy—the CEO (highest priority) sits at the top, with direct reports below, and each level has lower priority than the one above it._

### 2. Priority Queue Using an Ordered Array (Simple but Inefficient)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class SimpleArrayPriorityQueue {
private:
    std::vector<T> elements;
    bool is_min_pq;

public:
    // Constructor (default is a max priority queue)
    SimpleArrayPriorityQueue(bool min_queue = false) : is_min_pq(min_queue) {}

    // Add element to the priority queue
    void push(const T& value) {
        elements.push_back(value);

        if (is_min_pq) {
            // For min priority queue (smaller values have higher priority)
            std::sort(elements.begin(), elements.end());
        } else {
            // For max priority queue (larger values have higher priority)
            std::sort(elements.begin(), elements.end(), std::greater<T>());
        }
    }

    // Remove the highest priority element
    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("Priority queue underflow");
        }

        elements.erase(elements.begin());
    }

    // Get the highest priority element
    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Priority queue is empty");
        }

        return elements.front();
    }

    // Check if priority queue is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Get the number of elements
    size_t size() const {
        return elements.size();
    }
};
```

_This array-based priority queue is like having to completely reorganize your bookshelf (sorting the entire array) every time you add a new book—functional but terribly inefficient for large collections._

### 3. Using STL's `std::priority_queue`

C++ provides a built-in priority queue implementation in the Standard Template Library:

```cpp
#include <iostream>
#include <queue>

int main() {
    // Max priority queue (default)
    std::priority_queue<int> maxPQ;

    // Add elements
    maxPQ.push(30);
    maxPQ.push(10);
    maxPQ.push(50);
    maxPQ.push(20);

    // Remove and print elements (will come out in descending order: 50, 30, 20, 10)
    std::cout << "Max Priority Queue elements: ";
    while (!maxPQ.empty()) {
        std::cout << maxPQ.top() << " ";
        maxPQ.pop();
    }
    std::cout << std::endl;

    // Min priority queue (using comparison function)
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;

    // Add elements
    minPQ.push(30);
    minPQ.push(10);
    minPQ.push(50);
    minPQ.push(20);

    // Remove and print elements (will come out in ascending order: 10, 20, 30, 50)
    std::cout << "Min Priority Queue elements: ";
    while (!minPQ.empty()) {
        std::cout << minPQ.top() << " ";
        minPQ.pop();
    }
    std::cout << std::endl;

    return 0;
}
```

_Using STL's priority_queue is like having a personal assistant who automatically puts your most important tasks at the top of your to-do list, regardless of when you added them._

> **🧠 Fun Fact**: The `std::priority_queue` container adapter in the STL is implemented using a binary heap data structure, which provides logarithmic time complexity for insertions and deletions while keeping the highest priority element always accessible in constant time.

## ⚡ Time & Space Complexity

### Deque Operations

| Operation     | Doubly Linked List | Dynamic Array    | STL std::deque |
| ------------- | ------------------ | ---------------- | -------------- |
| push_front    | O(1)               | O(n)             | O(1) amortized |
| push_back     | O(1)               | O(1) amortized\* | O(1) amortized |
| pop_front     | O(1)               | O(n)             | O(1)           |
| pop_back      | O(1)               | O(1)             | O(1)           |
| front/back    | O(1)               | O(1)             | O(1)           |
| Random Access | O(n)               | O(1)             | O(1)           |
| Space         | O(n)               | O(n)             | O(n)           |

_\* O(1) amortized for vector push_back due to occasional reallocation_

_STL's deque gives you the best of both worlds—constant-time operations at both ends and constant-time random access, like having a teleporter that can instantly take you to any part of a line._

### Priority Queue Operations

| Operation | Binary Heap | Ordered Array | STL priority_queue |
| --------- | ----------- | ------------- | ------------------ |
| push      | O(log n)    | O(n)          | O(log n)           |
| pop       | O(log n)    | O(1)          | O(log n)           |
| top       | O(1)        | O(1)          | O(1)               |
| Space     | O(n)        | O(n)          | O(n)               |

_The binary heap implementation strikes an excellent balance, like finding the express lane in a crowded store—you might spend a little time finding it (O(log n) for insertion), but checking out is instant (O(1) for top operation)._

## 🌎 Real-World Applications

### Deque Applications

1. **Undo/Redo Functionality**

```cpp
class TextEditor {
private:
    std::string current_text;
    std::deque<std::string> undo_stack;
    std::deque<std::string> redo_stack;

public:
    void type(const std::string& text) {
        undo_stack.push_back(current_text);
        redo_stack.clear();  // Clear redo history when new action is performed
        current_text += text;
    }

    void undo() {
        if (!undo_stack.empty()) {
            redo_stack.push_back(current_text);
            current_text = undo_stack.back();
            undo_stack.pop_back();
        }
    }

    void redo() {
        if (!redo_stack.empty()) {
            undo_stack.push_back(current_text);
            current_text = redo_stack.back();
            redo_stack.pop_back();
        }
    }

    std::string getText() const {
        return current_text;
    }
};
```

_Using deques for undo/redo is like having a time machine for your document—jump back and forth through edit history with ease._

2. **Web Browser Navigation History**

```cpp
class BrowserHistory {
private:
    std::deque<std::string> history;
    int current_index;

public:
    BrowserHistory(const std::string& homepage) : current_index(0) {
        history.push_back(homepage);
    }

    void visit(const std::string& url) {
        // Remove all forward history when visiting a new page
        while (history.size() > current_index + 1) {
            history.pop_back();
        }

        history.push_back(url);
        current_index++;
    }

    std::string back(int steps) {
        int actual_steps = std::min(steps, current_index);
        current_index -= actual_steps;
        return history[current_index];
    }

    std::string forward(int steps) {
        int max_forward = history.size() - current_index - 1;
        int actual_steps = std::min(steps, max_forward);
        current_index += actual_steps;
        return history[current_index];
    }

    std::string getCurrentPage() const {
        return history[current_index];
    }
};
```

_Browser history navigation is like a personalized tour guide for your web journeys—keeping track of where you've been and letting you revisit sites with ease._

3. **Sliding Window Problems**

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

_Using a deque for sliding window problems is like having an efficient security system that continuously monitors just the right section of footage, automatically discarding irrelevant frames._

### Priority Queue Applications

1. **Dijkstra's Shortest Path Algorithm**

```cpp
void dijkstra(const std::vector<std::vector<std::pair<int, int>>>& graph, int start, std::vector<int>& distances) {
    int n = graph.size();
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
        for (const auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            // Relaxation step
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                pq.push({distances[v], v});
            }
        }
    }
}
```

_Dijkstra's algorithm with a priority queue is like having a GPS that always explores the most promising routes first, ensuring you find the shortest path without wasting time on lengthy detours._

2. **Huffman Coding for Data Compression**

```cpp
struct HuffmanNode {
    char data;
    unsigned freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

void generateCodes(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root) return;

    // Found a leaf node
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

std::unordered_map<char, std::string> buildHuffmanTree(const std::string& text) {
    // Count frequency of each character
    std::unordered_map<char, unsigned> freq;
    for (char c : text) {
        freq[c]++;
    }

    // Create a min priority queue
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto& pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build Huffman Tree by combining the two nodes with lowest frequency
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        // Create a new internal node with frequency equal to the sum
        // of the two nodes, and with the two nodes as children
        HuffmanNode* newNode = new HuffmanNode('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    // The remaining node is the root of the Huffman tree
    HuffmanNode* root = pq.top();

    // Generate Huffman codes
    std::unordered_map<char, std::string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    return huffmanCodes;
}
```

_Huffman coding with a priority queue is like having a savvy packing expert who gives smaller codes to frequently used items and longer codes to rarely used ones—optimizing your storage space._

3. **Event-Driven Simulation**

```cpp
struct Event {
    double time;
    int type;
    // Additional data...

    Event(double t, int ty) : time(t), type(ty) {}

    // Higher priority for earlier events
    bool operator<(const Event& other) const {
        return time > other.time;
    }
};

class Simulation {
private:
    std::priority_queue<Event> events;
    double current_time;

public:
    Simulation() : current_time(0) {}

    void scheduleEvent(double time, int type) {
        events.push(Event(time, type));
    }

    void run() {
        while (!events.empty()) {
            Event current = events.top();
            events.pop();

            // Update simulation time
            current_time = current.time;

            // Process the event based on its type
            processEvent(current);
        }
    }

    void processEvent(const Event& event) {
        switch (event.type) {
            case 1: // Arrival event
                // Process arrival
                // Maybe schedule a departure event
                break;
            case 2: // Departure event
                // Process departure
                break;
            // Other event types
        }
    }
};
```

_An event-driven simulation using a priority queue is like a perfect stage manager for a complex production—ensuring every actor appears at exactly the right moment, maintaining the chronological flow regardless of when events were scheduled._

## 🎭 Advanced Techniques & Variations

### 1. Double-Ended Priority Queue

A double-ended priority queue (DEPQ) allows efficient access, insertion, and deletion of both minimum and maximum elements:

```cpp
template <typename T>
class DoubleEndedPQ {
private:
    std::vector<T> min_heap; // For finding minimum
    std::vector<T> max_heap; // For finding maximum
    std::unordered_map<T, size_t> min_positions; // Track positions in min_heap
    std::unordered_map<T, size_t> max_positions; // Track positions in max_heap

    // Heapify functions for min and max heaps
    // ...

public:
    void insert(const T& value) {
        // Insert into both heaps
        min_heap.push_back(value);
        max_heap.push_back(value);

        // Update position maps and heapify
        min_positions[value] = min_heap.size() - 1;
        max_positions[value] = max_heap.size() - 1;

        heapifyUpMin(min_heap.size() - 1);
        heapifyUpMax(max_heap.size() - 1);
    }

    T extractMin() {
        // Extract minimum element
        // ...
    }

    T extractMax() {
        // Extract maximum element
        // ...
    }

    // Other operations...
};
```

_A double-ended priority queue is like having both a "Most Wanted" and "Least Wanted" list that you can update efficiently—keeping track of both extremes without having to search through the entire collection._

### 2. Indexed Priority Queue

An indexed priority queue allows updating the priority of elements already in the queue:

```cpp
template <typename T>
class IndexedPQ {
private:
    std::vector<T> heap;
    std::vector<int> indices; // Map external indices to heap positions
    std::vector<int> reverse; // Map heap positions to external indices
    int heap_size;

    // Heapify functions
    // ...

public:
    IndexedPQ(int max_size) : heap(max_size + 1), indices(max_size + 1, -1),
                             reverse(max_size + 1, -1), heap_size(0) {}

    void insert(int i, const T& value) {
        // Insert element with external index i
        // ...
    }

    void update(int i, const T& value) {
        // Update priority of element with external index i
        // ...
    }

    void remove(int i) {
        // Remove element with external index i
        // ...
    }

    // Other operations...
};
```

_An indexed priority queue is like having a dynamic restaurant reservation system—you can change the size of your party or your arrival time, and the hostess automatically updates your place in line._

### 3. Min-Max Heap

A min-max heap allows finding both the minimum and maximum elements efficiently using a single data structure:

```cpp
template <typename T>
class MinMaxHeap {
private:
    std::vector<T> heap;

    // Helper functions
    bool isMinLevel(int i) { return (int)log2(i + 1) % 2 == 0; }

    // Heapify functions
    // ...

public:
    void insert(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    T extractMin() {
        // Extract minimum element (always at index 0)
        // ...
    }

    T extractMax() {
        // Find and extract maximum element (at level 1 or among leaves)
        // ...
    }

    // Other operations...
};
```

_A min-max heap is like a cleverly designed building with both the CEO (maximum) and the receptionist (minimum) easily accessible, despite having completely opposite roles in the organizational hierarchy._

## 🎮 Classic Problems & Solutions

### 1. Palindrome Checker Using Deque

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
        char front = charDeque.front();
        charDeque.pop_front();

        char back = charDeque.back();
        charDeque.pop_back();

        if (front != back) {
            return false;
        }
    }

    return true;
}
```

_Using a deque for palindrome checking is like having two readers start from opposite ends of a book and comparing notes—if they always read the same character, it's a palindrome._

### 2. Task Scheduler with Cooldown Using Priority Queue

```cpp
int leastInterval(const std::vector<char>& tasks, int n) {
    // Count frequency of each task
    std::unordered_map<char, int> freq;
    for (char task : tasks) {
        freq[task]++;
    }

    // Create max heap based on frequency
    std::priority_queue<int> pq;
    for (const auto& pair : freq) {
        pq.push(pair.second);
    }

    int cycles = 0;

    while (!pq.empty()) {
        // Try to execute as many different tasks as possible in one cycle
        std::vector<int> temp;
        int time = 0;

        // Execute up to n+1 different tasks
        for (int i = 0; i <= n; i++) {
            if (!pq.empty()) {
                temp.push_back(pq.top() - 1); // Execute one instance
                pq.pop();
                time++;
            }
        }

        // Put tasks with remaining instances back into the heap
        for (int count : temp) {
            if (count > 0) {
                pq.push(count);
            }
        }

        // Add time for this cycle
        cycles += pq.empty() ? time : n + 1;
    }

    return cycles;
}
```

_The task scheduler solution is like managing a team of specialists who need recovery time between tasks—you prioritize the workers with the most assignments while respecting their required rest periods._

### 3. Merge K Sorted Lists Using Priority Queue

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

struct Compare {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;
    }
};

ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    // Min heap to keep track of the smallest elements
    std::priority_queue<ListNode*, std::vector<ListNode*>, Compare> pq;

    // Add the first element of each list to the heap
    for (ListNode* list : lists) {
        if (list) {
            pq.push(list);
        }
    }

    ListNode dummy(0); // Dummy head
    ListNode* tail = &dummy;

    // Process nodes in order of increasing value
    while (!pq.empty()) {
        // Get the smallest element
        ListNode* current = pq.top();
        pq.pop();

        // Add to result list
        tail->next = current;
        tail = tail->next;

        // If there are more elements in this list, add the next one to the heap
        if (current->next) {
            pq.push(current->next);
        }
    }

    return dummy.next;
}
```

_Merging k sorted lists with a priority queue is like having multiple sorted paper stacks that you want to combine—you always take the smallest top sheet from any stack, resulting in a perfectly sorted final pile._

### 4. Find Median from Data Stream Using Two Heaps

```cpp
class MedianFinder {
private:
    std::priority_queue<int> maxHeap; // For the smaller half
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // For the larger half

public:
    void addNum(int num) {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        // Balance the heaps
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

_Using two heaps to find the running median is like having two teams that maintain perfect balance—one team keeps track of the smaller half of numbers, the other the larger half, with their captains (the heap tops) always revealing the median._

## 😂 Deque & Priority Queue Jokes

- Why did the data structure go to therapy? It had double-ended issues—couldn't decide whether to be a stack or a queue, so it became a deque instead.

- How do you know if a programmer has used a priority queue? They always say "Hold on, I have something more important to finish first."

- I tried to explain the concept of a priority queue to my cat. Now it expects premium treatment before all other pets in the house.

- My friend claimed he could implement a deque in constant time. I told him that's a push from both ends!

- Priority queue motto: "It's not about when you arrived, it's about how important you are."

- What's a deque's favorite movie? "Front to Back to the Future"

## 📚 LeetCode Practice Problems

To reinforce today's concepts, try these LeetCode problems:

### Deque Problems

**Easy:**

- #933: Number of Recent Calls
- #1700: Number of Students Unable to Eat Lunch
- #1047: Remove All Adjacent Duplicates In String

**Medium:**

- #239: Sliding Window Maximum
- #641: Design Circular Deque
- #1438: Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit

**Hard:**

- #1499: Max Value of Equation
- #862: Shortest Subarray with Sum at Least K

### Priority Queue Problems

**Easy:**

- #703: Kth Largest Element in a Stream
- #1046: Last Stone Weight

**Medium:**

- #215: Kth Largest Element in an Array
- #347: Top K Frequent Elements
- #973: K Closest Points to Origin
- #1642: Furthest Building You Can Reach

**Hard:**

- #23: Merge k Sorted Lists
- #295: Find Median from Data Stream
- #1834: Single-Threaded CPU
- #1851: Minimum Interval to Include Each Query

_These LeetCode problems are like training for a marathon—they might be challenging, but they strengthen your algorithmic muscles and prepare you for real-world coding challenges._

## 🔮 Tomorrow's Preview:

Tomorrow we'll explore Hash Tables - the data structures that use the magic of hashing to provide near-constant time lookups. We'll delve into hash functions, collision resolution techniques, and implementation strategies to build efficient key-value stores.

_Hash tables are like having a librarian with photographic memory—they can tell you exactly where to find any book in the library in (almost) constant time, regardless of how many books there are._

#DSAin45 #Day10 #Deques #PriorityQueues #CPlusPlus #DataStructures #CodingInterview
