# 🎟️ DAY 9: Queues - First In, First Out Data Heroes #DSAin45

Welcome to Day 9! Yesterday we explored stacks—the data structure that believes the last one to arrive should be the first one to leave (like a perfectly polite dinner guest). Today, we're diving into queues—the champions of fairness that follow the First-In-First-Out (FIFO) principle, just like the line at your favorite coffee shop.

_In programming, as in life, sometimes you need to wait your turn. Queues: because cutting in line is bad form in both algorithms and British supermarkets._

## 🤔 What Is a Queue?

A queue is a linear data structure that follows the First-In-First-Out (FIFO) principle. Elements are added at one end (rear/tail) and removed from the other end (front/head). Think of a queue as a line of people waiting for a movie ticket—the first person to join the line is the first one to get a ticket and leave.

```
       ┌─────────┬─────────┬─────────┬─────────┐
Enqueue│  Item 4  │  Item 3  │  Item 2  │  Item 1  │ Dequeue
 here →│  (Rear)  │         │         │ (Front) │→ here
       └─────────┴─────────┴─────────┴─────────┘
```

> **🧠 Fun Fact**: While Stacks date back to Turing in the 1940s, Queues grew in importance with time-sharing computer systems in the 1960s, where they became essential for managing multiple users' requests on a single machine. The first commercial operating systems used queues for printer management—a problem still solved with queues today!

## 📋 Queue Operations

Queues support four primary operations:

- **Enqueue (Push)**: Add an element to the rear of the queue
- **Dequeue (Pop)**: Remove the element at the front of the queue
- **Front (Peek)**: View the front element without removing it
- **isEmpty**: Check if the queue is empty

Additional common operations include:

- **Rear**: View the last element without removing it
- **Size**: Return the number of elements in the queue
- **isFull**: Check if the queue is full (for bounded queues)

_Queue operations are like a well-run deli counter—take a number when you arrive (enqueue), and they'll serve you in the exact order you came in (dequeue). No VIP treatment here!_

## 🏗️ Implementing a Queue in C++

There are multiple ways to implement a queue:

### 1. Using an Array with Two Pointers

```cpp
#include <iostream>

template <typename T>
class ArrayQueue {
private:
    static const int MAX_SIZE = 100;
    T elements[MAX_SIZE];
    int front;
    int rear;
    int count;  // Track number of elements

public:
    // Constructor
    ArrayQueue() : front(0), rear(-1), count(0) {}

    // Enqueue - Add element to the rear
    void enqueue(const T& value) {
        if (isFull()) {
            throw std::overflow_error("Queue overflow");
        }

        // Circular increment of rear
        rear = (rear + 1) % MAX_SIZE;
        elements[rear] = value;
        count++;
    }

    // Dequeue - Remove element from the front
    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue underflow");
        }

        T value = elements[front];

        // Circular increment of front
        front = (front + 1) % MAX_SIZE;
        count--;

        return value;
    }

    // Front - View the front element
    T getFront() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        return elements[front];
    }

    // Rear - View the rear element
    T getRear() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        return elements[rear];
    }

    // Check if queue is empty
    bool isEmpty() const {
        return count == 0;
    }

    // Check if queue is full
    bool isFull() const {
        return count == MAX_SIZE;
    }

    // Get the number of elements
    int size() const {
        return count;
    }
};
```

_This implementation secretly transforms a linear array into a circular one, like converting a straight road into a roundabout—no dead ends, just continuous flow. The circular approach ensures we don't waste array space._

### 2. Using a Linked List

```cpp
#include <iostream>

template <typename T>
class LinkedListQueue {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* front_node;  // For dequeue
    Node* rear_node;   // For enqueue
    int queue_size;

public:
    // Constructor
    LinkedListQueue() : front_node(nullptr), rear_node(nullptr), queue_size(0) {}

    // Destructor - clean up memory
    ~LinkedListQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue - Add element to the rear
    void enqueue(const T& value) {
        Node* new_node = new Node(value);

        if (isEmpty()) {
            // If queue is empty, both front and rear point to the new node
            front_node = rear_node = new_node;
        } else {
            // Add new node at the rear
            rear_node->next = new_node;
            rear_node = new_node;
        }

        queue_size++;
    }

    // Dequeue - Remove element from the front
    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue underflow");
        }

        Node* temp = front_node;
        T value = temp->data;

        front_node = front_node->next;

        // If the queue becomes empty, update rear
        if (front_node == nullptr) {
            rear_node = nullptr;
        }

        delete temp;
        queue_size--;

        return value;
    }

    // Front - View the front element
    T getFront() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        return front_node->data;
    }

    // Rear - View the rear element
    T getRear() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        return rear_node->data;
    }

    // Check if queue is empty
    bool isEmpty() const {
        return front_node == nullptr;
    }

    // Get the number of elements
    int size() const {
        return queue_size;
    }
};
```

_A linked list implementation of a queue is like a conga line at a party—people can join at the back (enqueue), leave from the front (dequeue), and the line stays intact no matter how many people come and go._

### 3. Using STL's `std::queue`

C++ provides a built-in queue implementation in the Standard Template Library:

```cpp
#include <iostream>
#include <queue>

int main() {
    std::queue<int> myQueue;

    // Enqueue elements
    myQueue.push(10);
    myQueue.push(20);
    myQueue.push(30);

    // View front and back elements
    std::cout << "Front element: " << myQueue.front() << std::endl;
    std::cout << "Rear element: " << myQueue.back() << std::endl;

    // Dequeue elements
    while (!myQueue.empty()) {
        std::cout << "Dequeuing: " << myQueue.front() << std::endl;
        myQueue.pop();
    }

    return 0;
}
```

_Using STL's queue is like using Uber Eats instead of cooking—convenient, reliable, and you don't have to clean up afterward._

> **🧠 Fun Fact**: Like `std::stack`, `std::queue` is also a container adapter in the STL. By default, it uses `std::deque` as its underlying container, but you can also configure it to use `std::list` (but not `std::vector`, since efficient queue operations require insertion at both ends).

## ⚡ Time & Space Complexity

A properly implemented queue should have the following complexities:

| Operation | Array-Based (Fixed) | Array-Based (Circular) | Linked List | STL std::queue |
| --------- | ------------------- | ---------------------- | ----------- | -------------- |
| Enqueue   | O(1) / O(n)\*       | O(1)                   | O(1)        | O(1) amortized |
| Dequeue   | O(n)                | O(1)                   | O(1)        | O(1)           |
| Front     | O(1)                | O(1)                   | O(1)        | O(1)           |
| Rear      | O(1)                | O(1)                   | O(1)\*\*    | O(1)           |
| isEmpty   | O(1)                | O(1)                   | O(1)        | O(1)           |
| Space     | O(n)                | O(n)                   | O(n)        | O(n)           |

_\* O(1) until array is full, then O(n) if we need to resize_  
_\*\* O(1) only if we maintain a tail pointer_

_Queue operations in a proper implementation should be like a drive-through window—quick service regardless of how long the line gets._

## 🔄 Types of Queues

Beyond the basic queue, several specialized queue types serve different purposes:

### 1. Circular Queue

A circular queue (also called a ring buffer) optimizes the array-based queue by connecting the front and rear in a circular manner, allowing us to reuse array spaces.

```
       ┌─────┬─────┬─────┬─────┬─────┬─────┐
       │  -  │  D  │  E  │  F  │  A  │  B  │
       └─────┴─────┴─────┴─────┴─────┴─────┘
                  ↑                 ↑
                 rear              front
```

Circular queues are implemented with modulo arithmetic to "wrap around" the array indices:

```cpp
rear = (rear + 1) % capacity;
front = (front + 1) % capacity;
```

_A circular queue is like a revolving sushi bar—empty spots keep coming back around, so there's no wasted space._

### 2. Priority Queue

A priority queue serves elements based on their priority rather than their arrival order. Elements with higher priority are dequeued before those with lower priority.

Implementation typically uses a heap data structure for O(log n) enqueue and dequeue operations:

```cpp
#include <iostream>
#include <queue>

int main() {
    // Max priority queue (largest element has highest priority)
    std::priority_queue<int> pq;

    pq.push(10);
    pq.push(30);
    pq.push(20);

    // Elements come out in order of priority (highest first)
    while (!pq.empty()) {
        std::cout << pq.top() << " ";  // Output: 30 20 10
        pq.pop();
    }

    return 0;
}
```

_A priority queue is like a hospital emergency room—the most urgent cases get treated first, regardless of arrival time._

### 3. Double-Ended Queue (Deque)

A deque (pronounced "deck") allows insertion and removal of elements from both ends, combining features of both stacks and queues.

```cpp
#include <iostream>
#include <deque>

int main() {
    std::deque<int> dq;

    // Insert at both ends
    dq.push_front(10);   // 10
    dq.push_back(20);    // 10, 20
    dq.push_front(5);    // 5, 10, 20
    dq.push_back(25);    // 5, 10, 20, 25

    // Access elements
    std::cout << "Front: " << dq.front() << std::endl;  // 5
    std::cout << "Back: " << dq.back() << std::endl;    // 25

    // Remove from both ends
    dq.pop_front();      // 10, 20, 25
    dq.pop_back();       // 10, 20

    return 0;
}
```

_A deque is like having both VIP and standard entrances to a venue—people can come and go from either side, offering maximum flexibility._

## 🌎 Real-World Applications of Queues

Queues are foundational in many systems and algorithms:

### 1. Process Scheduling

Operating systems use queues to manage processes waiting for CPU time:

```cpp
struct Process {
    int id;
    int burst_time;
    int priority;
};

std::queue<Process> ready_queue;  // Processes ready to execute
std::queue<Process> io_queue;     // Processes waiting for I/O operations
```

_Process scheduling queues are like multiple checkout lines at a supermarket—different lines for different types of customers, all waiting for service._

### 2. Printer Spooling

Print jobs are managed using queues to handle multiple requests in order:

```cpp
struct PrintJob {
    int user_id;
    std::string document_name;
    int pages;
    int priority;
};

std::queue<PrintJob> print_queue;
```

_The printer queue is the original computing queue—they've been keeping your documents in order since the dawn of office computing._

### 3. Breadth-First Search (BFS)

BFS uses a queue to explore nodes level by level in a graph:

```cpp
void BFS(Graph& graph, int start_vertex) {
    std::vector<bool> visited(graph.numVertices(), false);
    std::queue<int> q;

    visited[start_vertex] = true;
    q.push(start_vertex);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Process current vertex
        std::cout << "Visiting vertex: " << current << std::endl;

        // Enqueue all adjacent unvisited vertices
        for (int adjacent : graph.getAdjacent(current)) {
            if (!visited[adjacent]) {
                visited[adjacent] = true;
                q.push(adjacent);
            }
        }
    }
}
```

_BFS with a queue is like exploring a maze by checking all paths of length 1, then all paths of length 2, and so on—methodical and thorough._

### 4. Message Queues in Distributed Systems

Message queues facilitate communication between distributed system components:

```cpp
// Producer code
MessageQueue mq("orders");
mq.send({"order_id": 12345, "product": "widget", "quantity": 10});

// Consumer code
Message msg = mq.receive();  // Blocks until a message is available
processOrder(msg);
```

_Message queues in distributed systems are like postal services—they ensure messages get delivered reliably, even if the sender and receiver aren't available at the same time._

### 5. Web Servers

Web servers use queues to manage incoming requests:

```cpp
std::queue<HttpRequest> request_queue;

// In the request handler thread
while (true) {
    HttpRequest request = request_queue.front();
    request_queue.pop();
    processRequest(request);
}
```

_Web server request queues are like ticket lines at popular attractions—they prevent server overload when traffic spikes and ensure all visitors get served eventually._

## 🎭 Classic Queue Problems

### 1. Implementing a Stack Using Queues

Yes, it's possible to implement a stack using just queues, though it's a bit inefficient:

```cpp
#include <iostream>
#include <queue>

template <typename T>
class StackUsingQueues {
private:
    std::queue<T> q1;
    std::queue<T> q2;

public:
    void push(const T& value) {
        // Add new element to q2
        q2.push(value);

        // Move all elements from q1 to q2
        while (!q1.empty()) {
            q2.push(q1.front());
            q1.pop();
        }

        // Swap q1 and q2, making q1 the main queue
        std::swap(q1, q2);
    }

    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack underflow");
        }

        T value = q1.front();
        q1.pop();
        return value;
    }

    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }

        return q1.front();
    }

    bool isEmpty() const {
        return q1.empty();
    }

    int size() const {
        return q1.size();
    }
};
```

_Implementing a stack with queues is like making a right turn by making three left turns—it works, but it's not the most direct approach._

### 2. Implementing a Queue Using Stacks

The reverse problem is also interesting—implementing a queue using only stacks:

```cpp
#include <iostream>
#include <stack>

template <typename T>
class QueueUsingStacks {
private:
    std::stack<T> s1;  // For enqueue
    std::stack<T> s2;  // For dequeue

public:
    void enqueue(const T& value) {
        s1.push(value);
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue underflow");
        }

        // If s2 is empty, transfer all elements from s1 to s2
        if (s2.empty()) {
            while (!s1.empty()) {
                s2.push(s1.top());
                s1.pop();
            }
        }

        T value = s2.top();
        s2.pop();
        return value;
    }

    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }

        // Make sure s2 has the element at the front of queue
        if (s2.empty()) {
            // This is a bit of a hack since we're modifying a const method
            // In a real implementation, we'd pre-compute this or use another approach
            std::stack<T>& s1_nonconst = const_cast<std::stack<T>&>(s1);
            std::stack<T> temp_s2;

            while (!s1_nonconst.empty()) {
                temp_s2.push(s1_nonconst.top());
                s1_nonconst.pop();
            }

            T front_value = temp_s2.top();

            // Restore s1
            while (!temp_s2.empty()) {
                s1_nonconst.push(temp_s2.top());
                temp_s2.pop();
            }

            return front_value;
        }

        return s2.top();
    }

    bool isEmpty() const {
        return s1.empty() && s2.empty();
    }

    int size() const {
        return s1.size() + s2.size();
    }
};
```

_Implementing a queue with stacks is like serving customers at a single-person bank teller—when a new person needs service, everyone already in line moves to another line in reverse order._

### 3. Sliding Window Maximum

Given a large array and a size `k`, find the maximum element in each contiguous subarray of size `k`:

```cpp
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    std::deque<int> dq;  // Stores indices of potential maximum elements

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

_The sliding window maximum problem is like finding the tallest person in a group photo where people keep shuffling in and out from the sides—using a deque gives us an efficient way to track potential maximums._

### 4. Task Scheduler

Given a list of tasks and a cooldown period, determine the minimum time needed to complete all tasks:

```cpp
int minimumTime(const std::vector<char>& tasks, int n) {
    // Count frequency of each task
    std::unordered_map<char, int> freq;
    for (char task : tasks) {
        freq[task]++;
    }

    // Find the maximum frequency
    int max_freq = 0;
    for (const auto& pair : freq) {
        max_freq = std::max(max_freq, pair.second);
    }

    // Count how many tasks have the maximum frequency
    int max_count = 0;
    for (const auto& pair : freq) {
        if (pair.second == max_freq) {
            max_count++;
        }
    }

    // Calculate the result using the formula
    // The idea is to schedule the most frequent tasks first, with cooling periods in between
    int result = std::max(
        (int)tasks.size(),  // Lower bound: at least as many slots as tasks
        (max_freq - 1) * (n + 1) + max_count  // Upper bound: based on max frequency tasks
    );

    return result;
}
```

_The task scheduler problem is like a juggler with multiple balls—some balls (tasks) need to stay in the air longer before being caught again, and we need to find the most efficient juggling pattern._

## 🚀 Advanced Queue Techniques

### 1. Circular Buffer / Ring Buffer

A circular buffer optimizes memory usage when elements are added and removed in a FIFO manner:

```cpp
template <typename T>
class CircularBuffer {
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t size_;
    size_t head;  // Read position
    size_t tail;  // Write position

public:
    CircularBuffer(size_t capacity) :
        buffer(capacity),
        capacity(capacity),
        size_(0),
        head(0),
        tail(0) {}

    bool push(const T& item) {
        if (isFull()) {
            return false;  // Buffer full
        }

        buffer[tail] = item;
        tail = (tail + 1) % capacity;
        size_++;
        return true;
    }

    bool pop(T& item) {
        if (isEmpty()) {
            return false;  // Buffer empty
        }

        item = buffer[head];
        head = (head + 1) % capacity;
        size_--;
        return true;
    }

    bool isEmpty() const {
        return size_ == 0;
    }

    bool isFull() const {
        return size_ == capacity;
    }

    size_t size() const {
        return size_;
    }
};
```

_A circular buffer is the conveyor belt of programming—items go around and around, with space being reused as items are consumed._

### 2. Blocking Queue for Producer-Consumer Pattern

A thread-safe queue that blocks when attempting to dequeue from an empty queue:

```cpp
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    size_t capacity;

public:
    BlockingQueue(size_t capacity = SIZE_MAX) : capacity(capacity) {}

    void enqueue(T item) {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait until queue has space
        not_full.wait(lock, [this] { return queue.size() < capacity; });

        queue.push(std::move(item));

        // Notify that queue is not empty
        not_empty.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait until queue has at least one item
        not_empty.wait(lock, [this] { return !queue.empty(); });

        T item = std::move(queue.front());
        queue.pop();

        // Notify that queue is not full
        not_full.notify_one();

        return item;
    }

    bool try_enqueue(T item, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex);

        if (!not_full.wait_for(lock, timeout, [this] { return queue.size() < capacity; })) {
            return false;  // Timeout occurred
        }

        queue.push(std::move(item));
        not_empty.notify_one();
        return true;
    }

    bool try_dequeue(T& item, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex);

        if (!not_empty.wait_for(lock, timeout, [this] { return !queue.empty(); })) {
            return false;  // Timeout occurred
        }

        item = std::move(queue.front());
        queue.pop();
        not_full.notify_one();
        return true;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};
```

_A blocking queue is like a restaurant kitchen—chefs (producers) prepare dishes and put them in the pickup area, while servers (consumers) take them out when ready. If there's nothing to pick up, servers wait; if the pickup area is full, chefs pause cooking._

### 3. Multilevel Queue / Multilevel Feedback Queue

Used in operating systems for process scheduling:

```cpp
class MultilevelQueue {
private:
    std::vector<std::queue<Process>> levels;
    int current_level;

public:
    MultilevelQueue(int num_levels) : levels(num_levels), current_level(0) {}

    void addProcess(const Process& process, int level) {
        if (level >= 0 && level < levels.size()) {
            levels[level].push(process);
        }
    }

    Process getNextProcess() {
        // Check each level, starting from the highest priority (lowest index)
        for (int i = 0; i < levels.size(); i++) {
            if (!levels[i].empty()) {
                Process next = levels[i].front();
                levels[i].pop();
                return next;
            }
        }

        throw std::runtime_error("No processes available");
    }
};
```

_A multilevel feedback queue is like a multi-lane highway where vehicles are sorted by type—emergency vehicles in the fast lane, trucks in the slow lane, and cars in between, with occasional lane changes based on behavior._

## 📊 Comparing Queue Implementations

| Feature           | Array-Based Queue      | Linked List Queue   | Circular Queue       | STL `std::queue`  |
| ----------------- | ---------------------- | ------------------- | -------------------- | ----------------- |
| Memory Efficiency | Fixed size or resizing | Dynamic size        | Fixed but efficient  | Dynamic (adapter) |
| Enqueue Time      | O(1)\* / O(n)\*\*      | O(1)                | O(1)                 | O(1) amortized    |
| Dequeue Time      | O(n) / O(1)\*\*\*      | O(1)                | O(1)                 | O(1)              |
| Access Front/Back | O(1)                   | O(1)                | O(1)                 | O(1)              |
| Implementation    | Moderate               | Straightforward     | Trickiest            | Easiest           |
| Best Use Case     | Small, fixed capacity  | Unknown, large size | Fixed size, frequent | General purpose   |

_\* For array with rear tracking_  
_\*\* If array resize needed_  
_\*\*\* For circular array_

_Choosing the right queue implementation is like choosing transportation—cars for flexibility, trains for efficiency, bikes for simplicity, and walking when you're not in a hurry. Each has its place._

> **🧠 Fun Fact**: The term "queue" comes from the British English word for a line of people waiting for something. This is also why "queue" has five letters but you only pronounce the first one!

## 😂 Queue Jokes

- Why don't programmers like to get in lines? Because they prefer queues—at least they come with built-in methods to know when they'll be done waiting.

- I tried to tell my friend a joke about queues, but he said he'd have to wait for everyone who came before him to get it first.

- What's a queue's favorite song? "I'll Be There For You" by The Rembrandts... but only after everyone who arrived first is served.

- Why did the linked list decide to retire? It wanted a data structure with more balance—so it became a binary search tree with constant-time operations at both ends.

## 📚 LeetCode Practice Problems

To reinforce today's queue concepts, try these LeetCode problems:

**Easy:**

- #225: Implement Stack using Queues
- #232: Implement Queue using Stacks
- #933: Number of Recent Calls
- #1700: Number of Students Unable to Eat Lunch
- #387: First Unique Character in a String

**Medium:**

- #622: Design Circular Queue
- #641: Design Circular Deque
- #950: Reveal Cards In Increasing Order
- #649: Dota2 Senate
- #1438: Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit

**Hard:**

- #239: Sliding Window Maximum
- #358: Rearrange String k Distance Apart
- #862: Shortest Subarray with Sum at Least K
- #1425: Constrained Subsequence Sum
- #1499: Max Value of Equation

_LeetCode queue problems are like waiting at the DMV—confusing at first, often frustrating, but eventually you'll get what you came for... hopefully._

## 🔮 Tomorrow's Preview:

Tomorrow we'll explore the Deque (Double-Ended Queue) in greater depth—a versatile data structure that can function as both a stack and a queue, offering insertion and deletion at both ends. We'll see how this flexibility enables efficient solutions to problems that would be challenging with simpler data structures.

_Deques are like Swiss Army knives in the cutlery drawer—not always necessary, but impressively versatile when you need them._

#DSAin45 #Day9 #Queues #CPlusPlus #DataStructures #CodingInterview #FIFO
