// Day 9: Queues - #DSAin45
// A comprehensive implementation with various queue applications

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <queue>
#include <stack>
#include <deque>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

// ===== QUEUE IMPLEMENTATIONS =====

// 1. Array-Based Queue Implementation (Simple)
template <typename T>
class SimpleArrayQueue
{
private:
    static const int MAX_SIZE = 10; // Small size for demonstration
    T elements[MAX_SIZE];
    int front;
    int rear;
    int count;

public:
    // Constructor
    SimpleArrayQueue() : front(0), rear(-1), count(0)
    {
        std::cout << "Created a simple array-based queue with capacity " << MAX_SIZE << std::endl;
    }

    // Enqueue - Add element to the rear
    void enqueue(const T &value)
    {
        if (isFull())
        {
            throw std::overflow_error("Queue overflow - Cannot enqueue to a full queue");
        }

        rear = (rear + 1) % MAX_SIZE;
        elements[rear] = value;
        count++;
        std::cout << "Enqueued: " << value << std::endl;
    }

    // Dequeue - Remove element from the front
    T dequeue()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue underflow - Cannot dequeue from an empty queue");
        }

        T value = elements[front];
        front = (front + 1) % MAX_SIZE;
        count--;

        std::cout << "Dequeued: " << value << std::endl;
        return value;
    }

    // Front - View the front element without removing
    T getFront() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty - Cannot get front");
        }

        return elements[front];
    }

    // Rear - View the rear element without removing
    T getRear() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty - Cannot get rear");
        }

        return elements[rear];
    }

    // Check if queue is empty
    bool isEmpty() const
    {
        return count == 0;
    }

    // Check if queue is full
    bool isFull() const
    {
        return count == MAX_SIZE;
    }

    // Get the number of elements
    int size() const
    {
        return count;
    }

    // Display the queue (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Queue is empty" << std::endl;
            return;
        }

        std::cout << "Queue (front to rear): ";
        int current = front;
        for (int i = 0; i < count; i++)
        {
            std::cout << elements[current] << " ";
            current = (current + 1) % MAX_SIZE;
        }
        std::cout << std::endl;
    }
};

// 2. Circular Queue Implementation (More efficient array-based implementation)
template <typename T>
class CircularQueue
{
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t front;
    size_t rear;
    size_t count;

public:
    // Constructor with specified capacity
    CircularQueue(size_t size) : buffer(size),
                                 capacity(size),
                                 front(0),
                                 rear(0),
                                 count(0)
    {
        std::cout << "Created a circular queue with capacity " << size << std::endl;
    }

    // Enqueue - Add element to the rear
    void enqueue(const T &value)
    {
        if (isFull())
        {
            throw std::overflow_error("Queue overflow - Cannot enqueue to a full queue");
        }

        buffer[rear] = value;
        rear = (rear + 1) % capacity;
        count++;
        std::cout << "Enqueued: " << value << std::endl;
    }

    // Dequeue - Remove element from the front
    T dequeue()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue underflow - Cannot dequeue from an empty queue");
        }

        T value = buffer[front];
        front = (front + 1) % capacity;
        count--;

        std::cout << "Dequeued: " << value << std::endl;
        return value;
    }

    // Front - View the front element without removing
    T getFront() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty - Cannot get front");
        }

        return buffer[front];
    }

    // Rear - View the element that will be rear after next enqueue
    T getRear() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty - Cannot get rear");
        }

        return buffer[(rear + capacity - 1) % capacity];
    }

    // Check if queue is empty
    bool isEmpty() const
    {
        return count == 0;
    }

    // Check if queue is full
    bool isFull() const
    {
        return count == capacity;
    }

    // Get the number of elements
    size_t size() const
    {
        return count;
    }

    // Display the queue (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Queue is empty" << std::endl;
            return;
        }

        std::cout << "Queue (front to rear): ";
        size_t current = front;
        for (size_t i = 0; i < count; i++)
        {
            std::cout << buffer[current] << " ";
            current = (current + 1) % capacity;
        }
        std::cout << std::endl;
    }
};

// 3. Linked List Based Queue Implementation
template <typename T>
class LinkedListQueue
{
private:
    struct Node
    {
        T data;
        Node *next;

        Node(const T &value) : data(value), next(nullptr)
        {
            // std::cout << "Created node with value: " << value << std::endl;
        }
    };

    Node *front_node; // For dequeue operations
    Node *rear_node;  // For enqueue operations
    size_t queue_size;

public:
    // Constructor
    LinkedListQueue() : front_node(nullptr), rear_node(nullptr), queue_size(0)
    {
        std::cout << "Created a linked list based queue" << std::endl;
    }

    // Destructor - Clean up memory
    ~LinkedListQueue()
    {
        std::cout << "Destroying linked list queue..." << std::endl;
        while (!isEmpty())
        {
            dequeue();
        }
    }

    // Enqueue - Add element to the rear
    void enqueue(const T &value)
    {
        Node *new_node = new Node(value);

        if (isEmpty())
        {
            // If queue is empty, both front and rear point to the new node
            front_node = rear_node = new_node;
        }
        else
        {
            // Add new node at the rear
            rear_node->next = new_node;
            rear_node = new_node;
        }

        queue_size++;
        std::cout << "Enqueued: " << value << std::endl;
    }

    // Dequeue - Remove element from the front
    T dequeue()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue underflow - Cannot dequeue from an empty queue");
        }

        Node *temp = front_node;
        T value = temp->data;

        front_node = front_node->next;

        // If the queue becomes empty, update rear as well
        if (front_node == nullptr)
        {
            rear_node = nullptr;
        }

        delete temp;
        queue_size--;

        std::cout << "Dequeued: " << value << std::endl;
        return value;
    }

    // Front - View the front element without removing
    T getFront() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty - Cannot get front");
        }

        return front_node->data;
    }

    // Rear - View the rear element without removing
    T getRear() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty - Cannot get rear");
        }

        return rear_node->data;
    }

    // Check if queue is empty
    bool isEmpty() const
    {
        return front_node == nullptr;
    }

    // Get the number of elements
    size_t size() const
    {
        return queue_size;
    }

    // Display the queue (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Queue is empty" << std::endl;
            return;
        }

        std::cout << "Queue (front to rear): ";
        Node *current = front_node;

        while (current)
        {
            std::cout << current->data << " ";
            current = current->next;
        }

        std::cout << std::endl;
    }
};

// 4. STL Queue Demo
void stlQueueDemo()
{
    std::cout << "\n===== STL QUEUE DEMONSTRATION =====" << std::endl;

    std::queue<int> myQueue;

    // Enqueue elements
    std::cout << "Enqueuing elements: 10, 20, 30" << std::endl;
    myQueue.push(10);
    myQueue.push(20);
    myQueue.push(30);

    // View front and back elements
    std::cout << "Front element: " << myQueue.front() << std::endl;
    std::cout << "Rear element: " << myQueue.back() << std::endl;

    // Queue size
    std::cout << "Queue size: " << myQueue.size() << std::endl;

    // Dequeue elements
    std::cout << "Dequeuing all elements:" << std::endl;
    while (!myQueue.empty())
    {
        std::cout << "Dequeued: " << myQueue.front() << std::endl;
        myQueue.pop();
    }

    std::cout << "Queue is now empty: " << (myQueue.empty() ? "Yes" : "No") << std::endl;
}

// 5. Priority Queue Demo
void priorityQueueDemo()
{
    std::cout << "\n===== PRIORITY QUEUE DEMONSTRATION =====" << std::endl;

    // Max heap (default)
    std::priority_queue<int> maxPQ;

    std::cout << "Adding elements to max priority queue: 30, 10, 50, 20" << std::endl;
    maxPQ.push(30);
    maxPQ.push(10);
    maxPQ.push(50);
    maxPQ.push(20);

    std::cout << "Elements in max priority queue (dequeuing in priority order):" << std::endl;
    while (!maxPQ.empty())
    {
        std::cout << maxPQ.top() << " ";
        maxPQ.pop();
    }
    std::cout << std::endl;

    // Min heap (using comparison function)
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;

    std::cout << "Adding elements to min priority queue: 30, 10, 50, 20" << std::endl;
    minPQ.push(30);
    minPQ.push(10);
    minPQ.push(50);
    minPQ.push(20);

    std::cout << "Elements in min priority queue (dequeuing in priority order):" << std::endl;
    while (!minPQ.empty())
    {
        std::cout << minPQ.top() << " ";
        minPQ.pop();
    }
    std::cout << std::endl;

    // Priority queue with custom type
    struct Patient
    {
        std::string name;
        int priority; // Lower number means higher priority

        // Compare patients based on priority
        bool operator>(const Patient &other) const
        {
            return priority > other.priority;
        }
    };

    std::priority_queue<Patient, std::vector<Patient>, std::greater<Patient>> emergencyRoom;

    emergencyRoom.push({"Alice", 3});   // Standard
    emergencyRoom.push({"Bob", 1});     // Critical
    emergencyRoom.push({"Charlie", 2}); // Urgent
    emergencyRoom.push({"Dave", 4});    // Minor

    std::cout << "Emergency room queue (processing patients by priority):" << std::endl;
    while (!emergencyRoom.empty())
    {
        Patient p = emergencyRoom.top();
        std::cout << "Treating patient: " << p.name << " (Priority: " << p.priority << ")" << std::endl;
        emergencyRoom.pop();
    }
}

// 6. Deque (Double-Ended Queue) Demo
void dequeDemo()
{
    std::cout << "\n===== DEQUE DEMONSTRATION =====" << std::endl;

    std::deque<int> dq;

    // Insert at both ends
    std::cout << "Adding elements at both ends" << std::endl;
    dq.push_front(10);
    std::cout << "push_front(10): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.push_back(20);
    std::cout << "push_back(20): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.push_front(5);
    std::cout << "push_front(5): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.push_back(25);
    std::cout << "push_back(25): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    // Access elements
    std::cout << "Front element: " << dq.front() << std::endl;
    std::cout << "Back element: " << dq.back() << std::endl;

    // Random access
    std::cout << "Element at index 2: " << dq[2] << std::endl;

    // Remove from both ends
    dq.pop_front();
    std::cout << "After pop_front(): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.pop_back();
    std::cout << "After pop_back(): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    // Insert in the middle
    dq.insert(dq.begin() + 1, 15);
    std::cout << "After insert(15) at index 1: ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;
}

// ===== QUEUE APPLICATIONS =====

// 1. Implementing Stack using Queue
class StackUsingQueue
{
private:
    std::queue<int> q1;
    std::queue<int> q2;

public:
    // Push - O(n) time complexity
    void push(int value)
    {
        // Add new element to q2
        q2.push(value);

        // Move all elements from q1 to q2
        while (!q1.empty())
        {
            q2.push(q1.front());
            q1.pop();
        }

        // Swap q1 and q2, making q1 the main queue with most recent element at front
        std::swap(q1, q2);

        std::cout << "Pushed (to stack): " << value << std::endl;
    }

    // Pop - O(1) time complexity
    int pop()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Stack is empty");
        }

        int value = q1.front();
        q1.pop();

        std::cout << "Popped (from stack): " << value << std::endl;
        return value;
    }

    // Top - O(1) time complexity
    int top() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Stack is empty");
        }

        return q1.front();
    }

    // Check if stack is empty
    bool isEmpty() const
    {
        return q1.empty();
    }

    // Get the number of elements
    int size() const
    {
        return q1.size();
    }
};

// 2. Implementing Queue using Stack
class QueueUsingStack
{
private:
    std::stack<int> s1; // For enqueue
    std::stack<int> s2; // For dequeue

public:
    // Enqueue - O(1) time complexity
    void enqueue(int value)
    {
        s1.push(value);
        std::cout << "Enqueued (via stacks): " << value << std::endl;
    }

    // Dequeue - Amortized O(1) time complexity
    int dequeue()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty");
        }

        // If s2 is empty, transfer all elements from s1 to s2
        if (s2.empty())
        {
            while (!s1.empty())
            {
                s2.push(s1.top());
                s1.pop();
            }
        }

        int value = s2.top();
        s2.pop();

        std::cout << "Dequeued (via stacks): " << value << std::endl;
        return value;
    }

    // Front - Get front element
    int front()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Queue is empty");
        }

        // If s2 is empty, transfer all elements from s1 to s2
        if (s2.empty())
        {
            while (!s1.empty())
            {
                s2.push(s1.top());
                s1.pop();
            }
        }

        return s2.top();
    }

    // Check if queue is empty
    bool isEmpty() const
    {
        return s1.empty() && s2.empty();
    }

    // Get the number of elements
    int size() const
    {
        return s1.size() + s2.size();
    }
};

// 3. Breadth-First Search using Queue
class Graph
{
private:
    int V;                             // Number of vertices
    std::vector<std::vector<int>> adj; // Adjacency list

public:
    Graph(int vertices) : V(vertices), adj(vertices) {}

    // Add an edge to the graph
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
    }

    // BFS traversal from a given source vertex
    void BFS(int start)
    {
        // Mark all vertices as not visited
        std::vector<bool> visited(V, false);

        // Create a queue for BFS
        std::queue<int> q;

        // Mark the source vertex as visited and enqueue it
        visited[start] = true;
        q.push(start);

        std::cout << "BFS Traversal starting from vertex " << start << ": ";

        while (!q.empty())
        {
            // Dequeue a vertex from queue and print it
            int current = q.front();
            std::cout << current << " ";
            q.pop();

            // Get all adjacent vertices of the dequeued vertex
            // If an adjacent vertex is not visited, mark it as visited and enqueue it
            for (int adjacent : adj[current])
            {
                if (!visited[adjacent])
                {
                    visited[adjacent] = true;
                    q.push(adjacent);
                }
            }
        }
        std::cout << std::endl;
    }
};

// 4. Sliding Window Maximum using Deque
std::vector<int> maxSlidingWindow(const std::vector<int> &nums, int k)
{
    std::vector<int> result;
    std::deque<int> dq; // Store indices of potential maximum elements

    for (int i = 0; i < nums.size(); i++)
    {
        // Remove elements outside the current window
        while (!dq.empty() && dq.front() < i - k + 1)
        {
            dq.pop_front();
        }

        // Remove smaller elements (they can't be maximum in current or future windows)
        while (!dq.empty() && nums[dq.back()] < nums[i])
        {
            dq.pop_back();
        }

        // Add current element
        dq.push_back(i);

        // Add to result if we've processed at least k elements
        if (i >= k - 1)
        {
            result.push_back(nums[dq.front()]);
        }
    }

    return result;
}

// 5. Level Order Traversal of Binary Tree using Queue
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

std::vector<std::vector<int>> levelOrderTraversal(TreeNode *root)
{
    std::vector<std::vector<int>> result;

    if (!root)
    {
        return result;
    }

    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty())
    {
        int levelSize = q.size();
        std::vector<int> currentLevel;

        for (int i = 0; i < levelSize; i++)
        {
            TreeNode *node = q.front();
            q.pop();

            currentLevel.push_back(node->val);

            if (node->left)
            {
                q.push(node->left);
            }

            if (node->right)
            {
                q.push(node->right);
            }
        }

        result.push_back(currentLevel);
    }

    return result;
}

// 6. Multi-producer, Multi-consumer Queue (Thread-safe)
template <typename T>
class ThreadSafeQueue
{
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable not_empty;

public:
    // Enqueue an item
    void enqueue(T item)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(std::move(item));
        }
        not_empty.notify_one();
    }

    // Dequeue an item (blocks if queue is empty)
    T dequeue()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this]
                       { return !queue.empty(); });

        T item = std::move(queue.front());
        queue.pop();
        return item;
    }

    // Try to dequeue with timeout
    bool try_dequeue(T &item, std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lock(mutex);

        if (!not_empty.wait_for(lock, timeout, [this]
                                { return !queue.empty(); }))
        {
            return false; // Timeout occurred
        }

        item = std::move(queue.front());
        queue.pop();
        return true;
    }

    // Check if queue is empty
    bool isEmpty() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

    // Get queue size
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }
};

// 7. Task Scheduler
int leastInterval(const std::vector<char> &tasks, int n)
{
    // Count task frequencies
    std::unordered_map<char, int> freq;
    for (char task : tasks)
    {
        freq[task]++;
    }

    // Find the maximum frequency
    int maxFreq = 0;
    for (const auto &pair : freq)
    {
        maxFreq = std::max(maxFreq, pair.second);
    }

    // Count how many tasks have the maximum frequency
    int maxCount = 0;
    for (const auto &pair : freq)
    {
        if (pair.second == maxFreq)
        {
            maxCount++;
        }
    }

    // Calculate the minimum time needed
    int slots = (maxFreq - 1) * (n + 1) + maxCount;
    return std::max(static_cast<int>(tasks.size()), slots);
}

// 8. Circular Buffer Implementation
template <typename T>
class CircularBuffer
{
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t head; // Read position
    size_t tail; // Write position
    bool full;

public:
    CircularBuffer(size_t size) : buffer(size),
                                  capacity(size),
                                  head(0),
                                  tail(0),
                                  full(false)
    {
        std::cout << "Created circular buffer with capacity " << size << std::endl;
    }

    size_t size() const
    {
        if (full)
        {
            return capacity;
        }
        if (tail >= head)
        {
            return tail - head;
        }
        return capacity - (head - tail);
    }

    bool isEmpty() const
    {
        return !full && (head == tail);
    }

    bool isFull() const
    {
        return full;
    }

    bool write(const T &item)
    {
        if (full)
        {
            return false; // Buffer is full
        }

        buffer[tail] = item;
        tail = (tail + 1) % capacity;

        // Check if buffer is now full
        if (tail == head)
        {
            full = true;
        }

        return true;
    }

    bool read(T &item)
    {
        if (isEmpty())
        {
            return false; // Buffer is empty
        }

        item = buffer[head];
        head = (head + 1) % capacity;

        // Buffer is definitely not full now
        full = false;

        return true;
    }

    // Display the buffer contents for debugging
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Buffer is empty" << std::endl;
            return;
        }

        std::cout << "Buffer contents: ";
        size_t count = size();
        size_t current = head;

        for (size_t i = 0; i < count; i++)
        {
            std::cout << buffer[current] << " ";
            current = (current + 1) % capacity;
        }
        std::cout << std::endl;
    }
};

// Producer-consumer example with the thread-safe queue
void producerConsumerExample()
{
    ThreadSafeQueue<int> q;
    bool done = false;

    // Producer thread
    std::thread producer([&q, &done]()
                         {
        for (int i = 0; i < 10; i++) {
            q.enqueue(i);
            std::cout << "Produced: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        done = true; });

    // Consumer thread
    std::thread consumer([&q, &done]()
                         {
        while (!done || !q.isEmpty()) {
            int item;
            if (q.try_dequeue(item, std::chrono::milliseconds(200))) {
                std::cout << "Consumed: " << item << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        } });

    producer.join();
    consumer.join();
}

// Main function with examples
int main()
{
    std::cout << "===== DAY 9: QUEUES DEMONSTRATION =====" << std::endl;

    // ===== SIMPLE ARRAY QUEUE DEMO =====
    std::cout << "\n===== SIMPLE ARRAY QUEUE DEMO =====" << std::endl;
    SimpleArrayQueue<int> simpleQueue;

    try
    {
        std::cout << "Is empty? " << (simpleQueue.isEmpty() ? "Yes" : "No") << std::endl;

        simpleQueue.enqueue(10);
        simpleQueue.enqueue(20);
        simpleQueue.enqueue(30);

        simpleQueue.display();

        std::cout << "Queue size: " << simpleQueue.size() << std::endl;
        std::cout << "Front element: " << simpleQueue.getFront() << std::endl;
        std::cout << "Rear element: " << simpleQueue.getRear() << std::endl;

        simpleQueue.dequeue();
        simpleQueue.display();

        simpleQueue.enqueue(40);
        simpleQueue.display();

        std::cout << "Is full? " << (simpleQueue.isFull() ? "Yes" : "No") << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== CIRCULAR QUEUE DEMO =====
    std::cout << "\n===== CIRCULAR QUEUE DEMO =====" << std::endl;
    CircularQueue<int> circularQueue(5);

    try
    {
        std::cout << "Is empty? " << (circularQueue.isEmpty() ? "Yes" : "No") << std::endl;

        circularQueue.enqueue(10);
        circularQueue.enqueue(20);
        circularQueue.enqueue(30);
        circularQueue.enqueue(40);

        circularQueue.display();

        std::cout << "Queue size: " << circularQueue.size() << std::endl;
        std::cout << "Front element: " << circularQueue.getFront() << std::endl;
        std::cout << "Rear element: " << circularQueue.getRear() << std::endl;

        circularQueue.dequeue();
        circularQueue.dequeue();
        circularQueue.display();

        circularQueue.enqueue(50);
        circularQueue.enqueue(60);
        circularQueue.display();

        // This should fill the queue
        std::cout << "Is full? " << (circularQueue.isFull() ? "Yes" : "No") << std::endl;

        // Try to enqueue when queue is full
        // circularQueue.enqueue(70); // This would throw an exception
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== LINKED LIST QUEUE DEMO =====
    std::cout << "\n===== LINKED LIST QUEUE DEMO =====" << std::endl;
    LinkedListQueue<int> llQueue;

    try
    {
        std::cout << "Is empty? " << (llQueue.isEmpty() ? "Yes" : "No") << std::endl;

        llQueue.enqueue(10);
        llQueue.enqueue(20);
        llQueue.enqueue(30);

        llQueue.display();

        std::cout << "Queue size: " << llQueue.size() << std::endl;
        std::cout << "Front element: " << llQueue.getFront() << std::endl;
        std::cout << "Rear element: " << llQueue.getRear() << std::endl;

        llQueue.dequeue();
        llQueue.display();

        llQueue.enqueue(40);
        llQueue.display();
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== STL QUEUE DEMO =====
    stlQueueDemo();

    // ===== PRIORITY QUEUE DEMO =====
    priorityQueueDemo();

    // ===== DEQUE DEMO =====
    dequeDemo();

    // ===== STACK USING QUEUE DEMO =====
    std::cout << "\n===== STACK USING QUEUE DEMO =====" << std::endl;
    StackUsingQueue stackUsingQueue;

    try
    {
        stackUsingQueue.push(10);
        stackUsingQueue.push(20);
        stackUsingQueue.push(30);

        std::cout << "Stack size: " << stackUsingQueue.size() << std::endl;
        std::cout << "Top element: " << stackUsingQueue.top() << std::endl;

        stackUsingQueue.pop();
        stackUsingQueue.pop();

        std::cout << "After two pops, top element: " << stackUsingQueue.top() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== QUEUE USING STACK DEMO =====
    std::cout << "\n===== QUEUE USING STACK DEMO =====" << std::endl;
    QueueUsingStack queueUsingStack;

    try
    {
        queueUsingStack.enqueue(10);
        queueUsingStack.enqueue(20);
        queueUsingStack.enqueue(30);

        std::cout << "Queue size: " << queueUsingStack.size() << std::endl;
        std::cout << "Front element: " << queueUsingStack.front() << std::endl;

        queueUsingStack.dequeue();
        queueUsingStack.dequeue();

        std::cout << "After two dequeues, front element: " << queueUsingStack.front() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== BFS USING QUEUE DEMO =====
    std::cout << "\n===== BFS USING QUEUE DEMO =====" << std::endl;

    // Create a graph
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    g.BFS(0);

    // ===== SLIDING WINDOW MAXIMUM DEMO =====
    std::cout << "\n===== SLIDING WINDOW MAXIMUM DEMO =====" << std::endl;
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;

    std::vector<int> maxValues = maxSlidingWindow(nums, k);

    std::cout << "Array: ";
    for (int num : nums)
        std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Sliding window size: " << k << std::endl;
    std::cout << "Maximum values in each window: ";
    for (int max : maxValues)
        std::cout << max << " ";
    std::cout << std::endl;

    // ===== BINARY TREE LEVEL ORDER TRAVERSAL DEMO =====
    std::cout << "\n===== BINARY TREE LEVEL ORDER TRAVERSAL DEMO =====" << std::endl;

    // Create a binary tree
    TreeNode *root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);

    std::vector<std::vector<int>> levels = levelOrderTraversal(root);

    std::cout << "Level order traversal:" << std::endl;
    for (size_t i = 0; i < levels.size(); i++)
    {
        std::cout << "Level " << i << ": ";
        for (int val : levels[i])
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Clean up tree memory
    delete root->right->right;
    delete root->right->left;
    delete root->right;
    delete root->left;
    delete root;

    // ===== TASK SCHEDULER DEMO =====
    std::cout << "\n===== TASK SCHEDULER DEMO =====" << std::endl;
    std::vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'B'};
    int cooldown = 2;

    int minTime = leastInterval(tasks, cooldown);

    std::cout << "Tasks: ";
    for (char task : tasks)
        std::cout << task << " ";
    std::cout << std::endl;

    std::cout << "Cooldown period: " << cooldown << std::endl;
    std::cout << "Minimum time needed: " << minTime << " units" << std::endl;

    // ===== CIRCULAR BUFFER DEMO =====
    std::cout << "\n===== CIRCULAR BUFFER DEMO =====" << std::endl;
    CircularBuffer<int> buffer(5);

    std::cout << "Is empty? " << (buffer.isEmpty() ? "Yes" : "No") << std::endl;

    buffer.write(10);
    buffer.write(20);
    buffer.write(30);
    buffer.display();

    int value;
    buffer.read(value);
    std::cout << "Read value: " << value << std::endl;
    buffer.display();

    buffer.write(40);
    buffer.write(50);
    buffer.write(60);
    buffer.display();

    std::cout << "Is full? " << (buffer.isFull() ? "Yes" : "No") << std::endl;

    while (!buffer.isEmpty())
    {
        buffer.read(value);
        std::cout << "Read value: " << value << std::endl;
    }

    std::cout << "Is empty? " << (buffer.isEmpty() ? "Yes" : "No") << std::endl;

    // ===== PRODUCER-CONSUMER DEMO (uncomment to run) =====
    // Note: This creates threads and may not be suitable for all environments
    // std::cout << "\n===== PRODUCER-CONSUMER DEMO =====" << std::endl;
    // producerConsumerExample();

    std::cout << "\n===== END OF DEMONSTRATION =====" << std::endl;

    return 0;
}