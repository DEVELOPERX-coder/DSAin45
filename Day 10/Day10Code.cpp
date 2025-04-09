// Day 10: Deques & Priority Queues - #DSAin45
// A comprehensive implementation with various applications

#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <cmath>
#include <chrono>
#include <functional>
#include <iomanip>

// ===== DEQUE IMPLEMENTATIONS =====

// 1. Deque Using Doubly Linked List
template <typename T>
class LinkedListDeque
{
private:
    struct Node
    {
        T data;
        Node *prev;
        Node *next;

        Node(const T &value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node *front_node;
    Node *back_node;
    size_t deque_size;

public:
    // Constructor
    LinkedListDeque() : front_node(nullptr), back_node(nullptr), deque_size(0)
    {
        std::cout << "Created a linked list based deque" << std::endl;
    }

    // Destructor - clean up memory
    ~LinkedListDeque()
    {
        std::cout << "Destroying linked list deque..." << std::endl;
        while (!isEmpty())
        {
            pop_front();
        }
    }

    // Add element to the front
    void push_front(const T &value)
    {
        Node *new_node = new Node(value);

        if (isEmpty())
        {
            front_node = back_node = new_node;
        }
        else
        {
            new_node->next = front_node;
            front_node->prev = new_node;
            front_node = new_node;
        }

        deque_size++;
        std::cout << "Pushed to front: " << value << std::endl;
    }

    // Add element to the back
    void push_back(const T &value)
    {
        Node *new_node = new Node(value);

        if (isEmpty())
        {
            front_node = back_node = new_node;
        }
        else
        {
            new_node->prev = back_node;
            back_node->next = new_node;
            back_node = new_node;
        }

        deque_size++;
        std::cout << "Pushed to back: " << value << std::endl;
    }

    // Remove element from the front
    T pop_front()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque underflow - Cannot pop from front of empty deque");
        }

        Node *temp = front_node;
        T value = temp->data;

        if (front_node == back_node)
        {
            // Only one element
            front_node = back_node = nullptr;
        }
        else
        {
            front_node = front_node->next;
            front_node->prev = nullptr;
        }

        delete temp;
        deque_size--;

        std::cout << "Popped from front: " << value << std::endl;
        return value;
    }

    // Remove element from the back
    T pop_back()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque underflow - Cannot pop from back of empty deque");
        }

        Node *temp = back_node;
        T value = temp->data;

        if (front_node == back_node)
        {
            // Only one element
            front_node = back_node = nullptr;
        }
        else
        {
            back_node = back_node->prev;
            back_node->next = nullptr;
        }

        delete temp;
        deque_size--;

        std::cout << "Popped from back: " << value << std::endl;
        return value;
    }

    // Get the front element
    T front() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque is empty - Cannot get front");
        }

        return front_node->data;
    }

    // Get the back element
    T back() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque is empty - Cannot get back");
        }

        return back_node->data;
    }

    // Check if deque is empty
    bool isEmpty() const
    {
        return front_node == nullptr;
    }

    // Get the number of elements
    size_t size() const
    {
        return deque_size;
    }

    // Display the deque (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Deque is empty" << std::endl;
            return;
        }

        std::cout << "Deque (front to back): ";
        Node *current = front_node;

        while (current)
        {
            std::cout << current->data << " ";
            current = current->next;
        }

        std::cout << std::endl;
    }
};

// 2. Deque Using Dynamic Array (Simplified Circular Array)
template <typename T>
class ArrayDeque
{
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t front_idx;
    size_t back_idx;
    size_t count;

public:
    // Constructor with specified capacity
    ArrayDeque(size_t size) : buffer(size),
                              capacity(size),
                              front_idx(0),
                              back_idx(0),
                              count(0)
    {
        std::cout << "Created an array-based deque with capacity " << size << std::endl;
    }

    // Add element to the front
    void push_front(const T &value)
    {
        if (isFull())
        {
            throw std::overflow_error("Deque overflow - Cannot push to front of full deque");
        }

        // Move front index backward (with wrap-around)
        front_idx = (front_idx + capacity - 1) % capacity;
        buffer[front_idx] = value;
        count++;

        std::cout << "Pushed to front: " << value << std::endl;
    }

    // Add element to the back
    void push_back(const T &value)
    {
        if (isFull())
        {
            throw std::overflow_error("Deque overflow - Cannot push to back of full deque");
        }

        buffer[back_idx] = value;
        back_idx = (back_idx + 1) % capacity;
        count++;

        std::cout << "Pushed to back: " << value << std::endl;
    }

    // Remove element from the front
    T pop_front()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque underflow - Cannot pop from front of empty deque");
        }

        T value = buffer[front_idx];
        front_idx = (front_idx + 1) % capacity;
        count--;

        std::cout << "Popped from front: " << value << std::endl;
        return value;
    }

    // Remove element from the back
    T pop_back()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque underflow - Cannot pop from back of empty deque");
        }

        // Move back index backward (with wrap-around)
        back_idx = (back_idx + capacity - 1) % capacity;
        T value = buffer[back_idx];
        count--;

        std::cout << "Popped from back: " << value << std::endl;
        return value;
    }

    // Get the front element
    T front() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque is empty - Cannot get front");
        }

        return buffer[front_idx];
    }

    // Get the back element
    T back() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Deque is empty - Cannot get back");
        }

        return buffer[(back_idx + capacity - 1) % capacity];
    }

    // Check if deque is empty
    bool isEmpty() const
    {
        return count == 0;
    }

    // Check if deque is full
    bool isFull() const
    {
        return count == capacity;
    }

    // Get the number of elements
    size_t size() const
    {
        return count;
    }

    // Display the deque (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Deque is empty" << std::endl;
            return;
        }

        std::cout << "Deque (front to back): ";
        size_t current = front_idx;
        for (size_t i = 0; i < count; i++)
        {
            std::cout << buffer[current] << " ";
            current = (current + 1) % capacity;
        }
        std::cout << std::endl;
    }
};

// 3. STL Deque Demo
void stlDequeDemo()
{
    std::cout << "\n===== STL DEQUE DEMONSTRATION =====" << std::endl;

    std::deque<int> dq;

    // Add elements at both ends
    std::cout << "Adding elements at both ends:" << std::endl;
    dq.push_front(10);
    std::cout << "After push_front(10): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.push_back(20);
    std::cout << "After push_back(20): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.push_front(5);
    std::cout << "After push_front(5): ";
    for (int n : dq)
        std::cout << n << " ";
    std::cout << std::endl;

    dq.push_back(25);
    std::cout << "After push_back(25): ";
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

    // Size and empty status
    std::cout << "Size: " << dq.size() << std::endl;
    std::cout << "Is empty? " << (dq.empty() ? "Yes" : "No") << std::endl;
}

// ===== PRIORITY QUEUE IMPLEMENTATIONS =====

// 1. Priority Queue Using Binary Heap
template <typename T, typename Compare = std::less<T>>
class HeapPriorityQueue
{
private:
    std::vector<T> heap;
    Compare comp;

    // Helper functions for maintaining heap property
    size_t parent(size_t i) { return (i - 1) / 2; }
    size_t leftChild(size_t i) { return 2 * i + 1; }
    size_t rightChild(size_t i) { return 2 * i + 2; }

    void heapifyUp(size_t i)
    {
        // Move element up the heap until heap property is restored
        while (i > 0 && comp(heap[parent(i)], heap[i]))
        {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(size_t i)
    {
        size_t maxIndex = i;
        size_t left = leftChild(i);

        // Check if left child should be max
        if (left < heap.size() && comp(heap[maxIndex], heap[left]))
        {
            maxIndex = left;
        }

        size_t right = rightChild(i);
        // Check if right child should be max
        if (right < heap.size() && comp(heap[maxIndex], heap[right]))
        {
            maxIndex = right;
        }

        // If i is not the max, swap and continue heapifying down
        if (i != maxIndex)
        {
            std::swap(heap[i], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }

public:
    // Constructor
    HeapPriorityQueue() {}

    // Add element to the priority queue
    void push(const T &value)
    {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
        std::cout << "Pushed: " << value << std::endl;
    }

    // Remove the highest priority element
    T pop()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Priority queue underflow - Cannot pop from empty queue");
        }

        T top_value = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty())
        {
            heapifyDown(0);
        }

        std::cout << "Popped: " << top_value << std::endl;
        return top_value;
    }

    // Get the highest priority element
    T top() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Priority queue is empty - Cannot get top");
        }

        return heap[0];
    }

    // Check if priority queue is empty
    bool isEmpty() const
    {
        return heap.empty();
    }

    // Get the number of elements
    size_t size() const
    {
        return heap.size();
    }

    // Display the heap (for debugging)
    void display() const
    {
        if (isEmpty())
        {
            std::cout << "Priority queue is empty" << std::endl;
            return;
        }

        std::cout << "Priority queue elements (in heap order): ";
        for (const T &value : heap)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

// 2. STL Priority Queue Demo
void stlPriorityQueueDemo()
{
    std::cout << "\n===== STL PRIORITY QUEUE DEMONSTRATION =====" << std::endl;

    // Max priority queue (default)
    std::cout << "Max Priority Queue:" << std::endl;
    std::priority_queue<int> maxPQ;

    std::cout << "Adding elements: 30, 10, 50, 20" << std::endl;
    maxPQ.push(30);
    maxPQ.push(10);
    maxPQ.push(50);
    maxPQ.push(20);

    std::cout << "Elements in max priority queue (in order of removal): ";
    while (!maxPQ.empty())
    {
        std::cout << maxPQ.top() << " ";
        maxPQ.pop();
    }
    std::cout << std::endl;

    // Min priority queue using comparison function
    std::cout << "\nMin Priority Queue:" << std::endl;
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;

    std::cout << "Adding elements: 30, 10, 50, 20" << std::endl;
    minPQ.push(30);
    minPQ.push(10);
    minPQ.push(50);
    minPQ.push(20);

    std::cout << "Elements in min priority queue (in order of removal): ";
    while (!minPQ.empty())
    {
        std::cout << minPQ.top() << " ";
        minPQ.pop();
    }
    std::cout << std::endl;

    // Priority queue with custom type
    std::cout << "\nPriority Queue with Custom Type:" << std::endl;
    struct Task
    {
        std::string name;
        int priority; // Lower number means higher priority

        // Comparison operator for priority queue
        bool operator<(const Task &other) const
        {
            return priority > other.priority; // For a min priority queue
        }
    };

    std::priority_queue<Task> taskQueue;

    taskQueue.push({"Parse log files", 3});
    taskQueue.push({"Handle user request", 1});
    taskQueue.push({"Update database", 2});
    taskQueue.push({"Send notification", 4});

    std::cout << "Tasks in order of priority:" << std::endl;
    while (!taskQueue.empty())
    {
        Task t = taskQueue.top();
        std::cout << "  Priority " << t.priority << ": " << t.name << std::endl;
        taskQueue.pop();
    }
}

// ===== APPLICATIONS =====

// 1. Palindrome Checker Using Deque
bool isPalindrome(const std::string &str)
{
    std::deque<char> charDeque;

    // Push all characters to deque (ignoring non-alphanumeric)
    for (char c : str)
    {
        if (std::isalnum(c))
        {
            charDeque.push_back(std::tolower(c));
        }
    }

    std::cout << "String after preprocessing: ";
    for (char c : charDeque)
    {
        std::cout << c;
    }
    std::cout << std::endl;

    // Compare characters from both ends
    while (charDeque.size() > 1)
    {
        char front = charDeque.front();
        charDeque.pop_front();

        char back = charDeque.back();
        charDeque.pop_back();

        if (front != back)
        {
            return false;
        }
    }

    return true;
}

// 2. Browser History Implementation Using Deque
class BrowserHistory
{
private:
    std::deque<std::string> history;
    size_t current_index;

public:
    BrowserHistory(const std::string &homepage) : current_index(0)
    {
        history.push_back(homepage);
        std::cout << "Browser opened with homepage: " << homepage << std::endl;
    }

    void visit(const std::string &url)
    {
        // Remove all forward history
        while (history.size() > current_index + 1)
        {
            history.pop_back();
        }

        // Add the new URL
        history.push_back(url);
        current_index++;

        std::cout << "Visited: " << url << std::endl;
    }

    std::string back(int steps)
    {
        int actual_steps = std::min(static_cast<int>(steps), static_cast<int>(current_index));
        current_index -= actual_steps;

        std::cout << "Moving back " << actual_steps << " step(s) to: " << history[current_index] << std::endl;
        return history[current_index];
    }

    std::string forward(int steps)
    {
        int max_forward = history.size() - current_index - 1;
        int actual_steps = std::min(steps, max_forward);
        current_index += actual_steps;

        std::cout << "Moving forward " << actual_steps << " step(s) to: " << history[current_index] << std::endl;
        return history[current_index];
    }

    std::string getCurrentPage() const
    {
        return history[current_index];
    }

    void displayHistory() const
    {
        std::cout << "Browser History:" << std::endl;
        for (size_t i = 0; i < history.size(); i++)
        {
            std::cout << "  " << i << ": " << history[i];
            if (i == current_index)
            {
                std::cout << " (current)";
            }
            std::cout << std::endl;
        }
    }
};

// 3. Sliding Window Maximum Using Deque
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

// 4. Merge K Sorted Arrays Using Priority Queue
std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>> &arrays)
{
    // Element in priority queue: {value, array_index, element_index}
    struct Element
    {
        int value;
        int array_idx;
        int element_idx;

        bool operator>(const Element &other) const
        {
            return value > other.value; // Min priority queue
        }
    };

    // Min heap to keep track of the smallest elements
    std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

    // Initialize with the first element from each array
    for (int i = 0; i < arrays.size(); i++)
    {
        if (!arrays[i].empty())
        {
            pq.push({arrays[i][0], i, 0});
        }
    }

    std::vector<int> result;

    // Process elements in order of increasing value
    while (!pq.empty())
    {
        Element current = pq.top();
        pq.pop();

        // Add to result
        result.push_back(current.value);

        // Move to next element in the same array
        if (current.element_idx + 1 < arrays[current.array_idx].size())
        {
            pq.push({arrays[current.array_idx][current.element_idx + 1],
                     current.array_idx,
                     current.element_idx + 1});
        }
    }

    return result;
}

// 5. Find Median from Data Stream Using Two Heaps
class MedianFinder
{
private:
    std::priority_queue<int> maxHeap;                                      // Stores the smaller half
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // Stores the larger half

public:
    MedianFinder()
    {
        std::cout << "Created a median finder" << std::endl;
    }

    void addNum(int num)
    {
        // First element goes to maxHeap
        if (maxHeap.empty())
        {
            maxHeap.push(num);
            return;
        }

        // Decide which heap to put the new number
        if (num <= maxHeap.top())
        {
            maxHeap.push(num);
        }
        else
        {
            minHeap.push(num);
        }

        // Balance the heaps
        balanceHeaps();

        std::cout << "Added number: " << num << ", current median: " << findMedian() << std::endl;
    }

    double findMedian()
    {
        // If unequal size, the median is the top of the larger heap
        if (maxHeap.size() > minHeap.size())
        {
            return maxHeap.top();
        }
        else if (minHeap.size() > maxHeap.size())
        {
            return minHeap.top();
        }
        else
        {
            // If equal size, the median is the average of the two tops
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }

    void displayHeaps() const
    {
        std::cout << "Max Heap (smaller half): ";
        displayHeap(maxHeap);

        std::cout << "Min Heap (larger half): ";
        displayHeap(minHeap);
    }

private:
    template <typename PQ>
    void displayHeap(PQ pq) const
    {
        if (pq.empty())
        {
            std::cout << "empty";
        }
        else
        {
            // Note: This destroys the heap, only for display purposes
            while (!pq.empty())
            {
                std::cout << pq.top() << " ";
                pq.pop();
            }
        }
        std::cout << std::endl;
    }

    void balanceHeaps()
    {
        // Ensure the size difference is not more than 1
        if (maxHeap.size() > minHeap.size() + 1)
        {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
        else if (minHeap.size() > maxHeap.size())
        {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
};

// 6. Task Scheduler with Cooldown Using Priority Queue
int leastInterval(const std::vector<char> &tasks, int n)
{
    // Count frequency of each task
    std::unordered_map<char, int> freq;
    for (char task : tasks)
    {
        freq[task]++;
    }

    // Create max heap based on frequency
    std::priority_queue<int> pq;
    for (const auto &pair : freq)
    {
        pq.push(pair.second);
    }

    int cycles = 0;

    while (!pq.empty())
    {
        // Try to execute as many different tasks as possible in one cycle
        std::vector<int> temp;
        int time = 0;

        // Execute up to n+1 different tasks
        for (int i = 0; i <= n; i++)
        {
            if (!pq.empty())
            {
                temp.push_back(pq.top() - 1); // Execute one instance
                pq.pop();
                time++;
            }
        }

        // Put tasks with remaining instances back into the heap
        for (int count : temp)
        {
            if (count > 0)
            {
                pq.push(count);
            }
        }

        // Add time for this cycle
        cycles += pq.empty() ? time : n + 1;
    }

    return cycles;
}

// ===== EXAMPLE USE CASES =====

// Event-Driven Simulation Example
void eventSimulationExample()
{
    std::cout << "\n===== EVENT-DRIVEN SIMULATION EXAMPLE =====" << std::endl;

    struct Event
    {
        double time;
        std::string description;

        // Higher priority for earlier events
        bool operator>(const Event &other) const
        {
            return time > other.time;
        }
    };

    // Create a min priority queue for events based on time
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> eventQueue;

    // Schedule some events
    eventQueue.push({10.5, "Customer arrives"});
    eventQueue.push({5.3, "Server startup"});
    eventQueue.push({15.7, "Data backup"});
    eventQueue.push({8.2, "Network check"});
    eventQueue.push({12.1, "User login"});

    std::cout << "Processing events in chronological order:" << std::endl;
    double current_time = 0.0;

    while (!eventQueue.empty())
    {
        Event e = eventQueue.top();
        eventQueue.pop();

        // Update simulation time
        current_time = e.time;

        // Process the event
        std::cout << "Time " << std::fixed << std::setprecision(1) << current_time
                  << ": " << e.description << std::endl;

        // In a real simulation, this might generate new events
        if (e.description == "Customer arrives" && current_time < 20.0)
        {
            // Schedule another customer arrival
            double next_arrival = current_time + (rand() % 10) + 5.0;
            eventQueue.push({next_arrival, "Customer arrives"});
            std::cout << "  Scheduled next customer arrival at time " << std::fixed
                      << std::setprecision(1) << next_arrival << std::endl;
        }
    }
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "===== DAY 10: DEQUES & PRIORITY QUEUES DEMONSTRATION =====" << std::endl;

    // ===== LINKED LIST DEQUE DEMO =====
    std::cout << "\n===== LINKED LIST DEQUE DEMO =====" << std::endl;
    LinkedListDeque<int> llDeque;

    try
    {
        std::cout << "Is empty? " << (llDeque.isEmpty() ? "Yes" : "No") << std::endl;

        llDeque.push_front(10);
        llDeque.push_back(20);
        llDeque.push_front(5);
        llDeque.push_back(25);

        llDeque.display();

        std::cout << "Front element: " << llDeque.front() << std::endl;
        std::cout << "Back element: " << llDeque.back() << std::endl;
        std::cout << "Deque size: " << llDeque.size() << std::endl;

        llDeque.pop_front();
        llDeque.pop_back();
        llDeque.display();
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== ARRAY DEQUE DEMO =====
    std::cout << "\n===== ARRAY DEQUE DEMO =====" << std::endl;
    ArrayDeque<int> arrayDeque(5);

    try
    {
        std::cout << "Is empty? " << (arrayDeque.isEmpty() ? "Yes" : "No") << std::endl;

        arrayDeque.push_back(10);
        arrayDeque.push_back(20);
        arrayDeque.push_front(5);
        arrayDeque.push_front(0);

        arrayDeque.display();

        std::cout << "Front element: " << arrayDeque.front() << std::endl;
        std::cout << "Back element: " << arrayDeque.back() << std::endl;
        std::cout << "Deque size: " << arrayDeque.size() << std::endl;
        std::cout << "Is full? " << (arrayDeque.isFull() ? "Yes" : "No") << std::endl;

        arrayDeque.pop_front();
        arrayDeque.pop_back();
        arrayDeque.display();

        // Try to add more elements
        arrayDeque.push_back(30);
        arrayDeque.push_front(-5);
        arrayDeque.display();

        std::cout << "Is full? " << (arrayDeque.isFull() ? "Yes" : "No") << std::endl;

        // This should cause an overflow exception
        // arrayDeque.push_back(40);
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== STL DEQUE DEMO =====
    stlDequeDemo();

    // ===== HEAP PRIORITY QUEUE DEMO =====
    std::cout << "\n===== HEAP PRIORITY QUEUE DEMO =====" << std::endl;

    // Max priority queue (default comparison)
    HeapPriorityQueue<int> maxPQ;

    try
    {
        std::cout << "Is empty? " << (maxPQ.isEmpty() ? "Yes" : "No") << std::endl;

        maxPQ.push(30);
        maxPQ.push(10);
        maxPQ.push(50);
        maxPQ.push(20);

        maxPQ.display();

        std::cout << "Top element: " << maxPQ.top() << std::endl;
        std::cout << "Priority queue size: " << maxPQ.size() << std::endl;

        maxPQ.pop();
        maxPQ.pop();
        maxPQ.display();
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Min priority queue (using greater comparison)
    std::cout << "\nMin Priority Queue:" << std::endl;
    HeapPriorityQueue<int, std::greater<int>> minPQ;

    try
    {
        minPQ.push(30);
        minPQ.push(10);
        minPQ.push(50);
        minPQ.push(20);

        minPQ.display();

        std::cout << "Top element: " << minPQ.top() << std::endl;

        minPQ.pop();
        minPQ.pop();
        minPQ.display();
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // ===== STL PRIORITY QUEUE DEMO =====
    stlPriorityQueueDemo();

    // ===== APPLICATIONS =====

    // 1. Palindrome Checker
    std::cout << "\n===== PALINDROME CHECKER DEMO =====" << std::endl;
    std::string palindrome1 = "A man, a plan, a canal: Panama";
    std::string palindrome2 = "race a car";

    std::cout << "Is \"" << palindrome1 << "\" a palindrome? "
              << (isPalindrome(palindrome1) ? "Yes" : "No") << std::endl;
    std::cout << "Is \"" << palindrome2 << "\" a palindrome? "
              << (isPalindrome(palindrome2) ? "Yes" : "No") << std::endl;

    // 2. Browser History
    std::cout << "\n===== BROWSER HISTORY DEMO =====" << std::endl;
    BrowserHistory browser("https://www.homepage.com");

    browser.visit("https://www.google.com");
    browser.visit("https://www.leetcode.com");
    browser.visit("https://www.github.com");

    browser.displayHistory();

    browser.back(1);
    browser.back(1);
    browser.forward(1);
    browser.visit("https://www.newpage.com");

    browser.displayHistory();
    std::cout << "Current page: " << browser.getCurrentPage() << std::endl;

    // 3. Sliding Window Maximum
    std::cout << "\n===== SLIDING WINDOW MAXIMUM DEMO =====" << std::endl;
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;

    std::cout << "Array: ";
    for (int num : nums)
        std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Window size: " << k << std::endl;

    std::vector<int> result = maxSlidingWindow(nums, k);

    std::cout << "Maximum values in each window: ";
    for (int max : result)
        std::cout << max << " ";
    std::cout << std::endl;

    // 4. Merge K Sorted Arrays
    std::cout << "\n===== MERGE K SORTED ARRAYS DEMO =====" << std::endl;
    std::vector<std::vector<int>> arrays = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9, 10}};

    std::cout << "Input arrays:" << std::endl;
    for (const auto &arr : arrays)
    {
        std::cout << "  ";
        for (int num : arr)
            std::cout << num << " ";
        std::cout << std::endl;
    }

    std::vector<int> merged = mergeKSortedArrays(arrays);

    std::cout << "Merged array: ";
    for (int num : merged)
        std::cout << num << " ";
    std::cout << std::endl;

    // 5. Find Median from Data Stream
    std::cout << "\n===== MEDIAN FINDER DEMO =====" << std::endl;
    MedianFinder medianFinder;

    medianFinder.addNum(41);
    medianFinder.addNum(35);
    medianFinder.addNum(62);
    medianFinder.addNum(5);
    medianFinder.addNum(97);
    medianFinder.addNum(108);

    std::cout << "Final median: " << medianFinder.findMedian() << std::endl;
    medianFinder.displayHeaps();

    // 6. Task Scheduler
    std::cout << "\n===== TASK SCHEDULER DEMO =====" << std::endl;
    std::vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'B'};
    int cooldown = 2;

    std::cout << "Tasks: ";
    for (char task : tasks)
        std::cout << task << " ";
    std::cout << std::endl;
    std::cout << "Cooldown period: " << cooldown << std::endl;

    int minTime = leastInterval(tasks, cooldown);
    std::cout << "Minimum time needed: " << minTime << " units" << std::endl;

    // ===== EVENT SIMULATION EXAMPLE =====
    eventSimulationExample();

    std::cout << "\n===== END OF DEMONSTRATION =====" << std::endl;

    return 0;
}