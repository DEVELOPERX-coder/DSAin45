# 🔍 DAY 25: Heap Data Structure - Priority Management Made Efficient #DSAin45

Welcome to Day 25 of our 45-day DSA journey! Yesterday, we explored AVL Trees and their self-balancing properties. Today, we dive into Heaps, a specialized tree-based data structure that excels at priority management.

_Imagine a customer support system that always knows which issue is most urgent, or a task scheduler that can instantly identify the highest-priority job. That's the power of heaps: they maintain a partial ordering that allows us to efficiently find and remove the highest (or lowest) priority element, no matter how many items we're tracking._

## 📌 What is a Heap?

A Heap is a complete binary tree that satisfies the heap property:

- In a **Max Heap**, for any node N, the value of N is greater than or equal to the values of its children
- In a **Min Heap**, for any node N, the value of N is less than or equal to the values of its children

Unlike binary search trees, heaps don't maintain left-right ordering. Instead, they focus on maintaining the relationship between parents and children to ensure that the root is always the maximum (or minimum) element.

Additionally, heaps are complete binary trees, meaning:

1. All levels are filled except possibly the last level
2. The last level is filled from left to right

This completeness property makes heaps ideal for array-based implementation.

## 🔄 Max Heap vs Min Heap

Here's how max heaps and min heaps differ:

**Max Heap:**

```
       90
      /  \
    70    60
   / \    / \
  50 40  30 20
```

In this max heap, each parent node is greater than or equal to its children. The maximum element (90) is at the root.

**Min Heap:**

```
       10
      /  \
    30    20
   / \    / \
  50 40  35 25
```

In this min heap, each parent node is less than or equal to its children. The minimum element (10) is at the root.

## 🌟 Why Use Heaps?

Heaps offer several key advantages:

1. **Efficient Priority Access**: O(1) time to find the maximum/minimum element
2. **Fast Insertions**: O(log n) time to add a new element
3. **Efficient Deletions**: O(log n) time to remove the maximum/minimum element
4. **Memory Efficiency**: Can be implemented as arrays without pointers
5. **Foundation for Algorithms**: Enables efficient implementations of algorithms like Heap Sort, Dijkstra's, and more

## 💻 Array Representation of Heaps

One of the most elegant aspects of heaps is that they can be efficiently represented using arrays without needing explicit pointers, thanks to their complete binary tree structure.

For a node at index `i` in a 0-indexed array:

- Left Child: `2*i + 1`
- Right Child: `2*i + 2`
- Parent: `(i-1)/2` (integer division)

For a 1-indexed array:

- Left Child: `2*i`
- Right Child: `2*i + 1`
- Parent: `i/2` (integer division)

Let's see an example of how a max heap would be stored in a 0-indexed array:

```
Heap tree:
       90
      /  \
    70    60
   / \    / \
  50 40  30 20

Array representation: [90, 70, 60, 50, 40, 30, 20]
```

This array representation makes heaps very memory-efficient and cache-friendly.

## 🔨 Implementing a Max Heap in C++

Let's implement a Max Heap in C++:

```cpp
#include <vector>
#include <stdexcept>

template <typename T>
class MaxHeap {
private:
    std::vector<T> heap;

    // Helper functions for calculating indices
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // Helper function to maintain the heap property
    void heapifyUp(int index) {
        // If we reach the root, we're done
        if (index == 0) return;

        int p = parent(index);
        // If parent is smaller than the current node, swap and continue heapifying up
        if (heap[p] < heap[index]) {
            std::swap(heap[p], heap[index]);
            heapifyUp(p);
        }
    }

    // Helper function to maintain the heap property after deletion
    void heapifyDown(int index) {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        int size = heap.size();

        // Check if left child is larger than current node
        if (left < size && heap[left] > heap[largest]) {
            largest = left;
        }

        // Check if right child is larger than current largest
        if (right < size && heap[right] > heap[largest]) {
            largest = right;
        }

        // If largest isn't the current node, swap and continue heapifying down
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    // Constructor
    MaxHeap() {}

    // Build a heap from an existing array in O(n) time
    MaxHeap(const std::vector<T>& array) {
        heap = array;
        int n = heap.size();

        // Start from the last non-leaf node and heapify down
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    // Insert a new element into the heap
    void insert(T value) {
        // Add the new element to the end of the heap
        heap.push_back(value);

        // Maintain the heap property by heapifying up
        heapifyUp(heap.size() - 1);
    }

    // Remove and return the maximum element
    T extractMax() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        // The root contains the maximum element
        T max = heap[0];

        // Replace the root with the last element
        heap[0] = heap.back();
        heap.pop_back();

        // Maintain the heap property by heapifying down
        if (!heap.empty()) {
            heapifyDown(0);
        }

        return max;
    }

    // Get the maximum element without removing it
    T getMax() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    // Check if the heap is empty
    bool isEmpty() const {
        return heap.empty();
    }

    // Get the size of the heap
    int size() const {
        return heap.size();
    }

    // Print the heap array (for debugging)
    void print() const {
        for (const T& val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};
```

## 🔨 Implementing a Min Heap in C++

The implementation of a Min Heap is very similar to that of a Max Heap, with key comparisons reversed:

```cpp
template <typename T>
class MinHeap {
private:
    std::vector<T> heap;

    // Helper functions for calculating indices
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // Helper function to maintain the heap property
    void heapifyUp(int index) {
        // If we reach the root, we're done
        if (index == 0) return;

        int p = parent(index);
        // If parent is larger than the current node, swap and continue heapifying up
        if (heap[p] > heap[index]) {
            std::swap(heap[p], heap[index]);
            heapifyUp(p);
        }
    }

    // Helper function to maintain the heap property after deletion
    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        int size = heap.size();

        // Check if left child is smaller than current node
        if (left < size && heap[left] < heap[smallest]) {
            smallest = left;
        }

        // Check if right child is smaller than current smallest
        if (right < size && heap[right] < heap[smallest]) {
            smallest = right;
        }

        // If smallest isn't the current node, swap and continue heapifying down
        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    // Constructor
    MinHeap() {}

    // Build a heap from an existing array in O(n) time
    MinHeap(const std::vector<T>& array) {
        heap = array;
        int n = heap.size();

        // Start from the last non-leaf node and heapify down
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    // Insert a new element into the heap
    void insert(T value) {
        // Add the new element to the end of the heap
        heap.push_back(value);

        // Maintain the heap property by heapifying up
        heapifyUp(heap.size() - 1);
    }

    // Remove and return the minimum element
    T extractMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        // The root contains the minimum element
        T min = heap[0];

        // Replace the root with the last element
        heap[0] = heap.back();
        heap.pop_back();

        // Maintain the heap property by heapifying down
        if (!heap.empty()) {
            heapifyDown(0);
        }

        return min;
    }

    // Get the minimum element without removing it
    T getMin() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    // Check if the heap is empty
    bool isEmpty() const {
        return heap.empty();
    }

    // Get the size of the heap
    int size() const {
        return heap.size();
    }

    // Print the heap array (for debugging)
    void print() const {
        for (const T& val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};
```

## 🔄 Core Heap Operations

Let's explore the key operations of a heap:

### 1. Insertion - O(log n)

Inserting a new element in a heap involves:

1. Add the element to the end of the heap
2. Heapify Up: Compare with parent and swap if necessary
3. Repeat step 2 until the heap property is satisfied

```cpp
void insert(T value) {
    heap.push_back(value);
    heapifyUp(heap.size() - 1);
}
```

### 2. Extract Max/Min - O(log n)

Removing the maximum (or minimum) element:

1. Save the root (max/min element)
2. Replace the root with the last element in the heap
3. Remove the last element
4. Heapify Down: Compare with children and swap with largest/smallest child if necessary
5. Repeat step 4 until the heap property is satisfied

```cpp
T extractMax() {
    if (heap.empty()) {
        throw std::runtime_error("Heap is empty");
    }

    T max = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    return max;
}
```

### 3. Peeking - O(1)

Getting the maximum/minimum element without removing it:

```cpp
T getMax() const {
    if (heap.empty()) {
        throw std::runtime_error("Heap is empty");
    }
    return heap[0];
}
```

### 4. Heap Construction - O(n)

Building a heap from an existing array:

```cpp
MaxHeap(const std::vector<T>& array) {
    heap = array;
    int n = heap.size();

    // Start from the last non-leaf node and heapify down
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDown(i);
    }
}
```

This operation is actually O(n), not O(n log n) as might be expected. The mathematical proof is complex, but it leverages the fact that most nodes are near the bottom of the heap and require very few swaps.

## 📊 Time and Space Complexity Analysis

Let's analyze the time and space complexity for heap operations:

| Operation       | Time Complexity | Space Complexity     |
| --------------- | --------------- | -------------------- |
| Insert          | O(log n)        | O(1)                 |
| Extract Max/Min | O(log n)        | O(1)                 |
| Get Max/Min     | O(1)            | O(1)                 |
| Build Heap      | O(n)            | O(n)                 |
| Heapify Up/Down | O(log n)        | O(log n) - recursion |

The logarithmic time complexity for many operations makes heaps very efficient for priority-based applications.

## 🔢 Heapsort Algorithm

Heapsort is an efficient comparison-based sorting algorithm that uses a heap data structure:

```cpp
void heapSort(std::vector<int>& arr) {
    // Build a max heap
    MaxHeap<int> maxHeap(arr);

    // Extract elements from the heap in order
    for (int i = arr.size() - 1; i >= 0; i--) {
        arr[i] = maxHeap.extractMax();
    }
}
```

Or more efficiently, without using a separate heap class:

```cpp
void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Build a max heap in-place
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        std::swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
```

**Analysis of Heapsort:**

- Time Complexity: O(n log n) - Both in worst, average, and best cases
- Space Complexity: O(1) - Since it's an in-place sort
- Not stable (equal elements may change relative order)
- Very efficient for large datasets

## 🛠️ Priority Queue using Heap

One of the most common applications of heaps is implementing a priority queue:

```cpp
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> heap;
    Compare comp;

    // Helper functions similar to MaxHeap/MinHeap
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void heapifyUp(int index) {
        if (index == 0) return;

        int p = parent(index);
        if (comp(heap[p], heap[index])) {
            std::swap(heap[p], heap[index]);
            heapifyUp(p);
        }
    }

    void heapifyDown(int index) {
        int highestPriority = index;
        int left = leftChild(index);
        int right = rightChild(index);
        int size = heap.size();

        if (left < size && comp(heap[highestPriority], heap[left])) {
            highestPriority = left;
        }

        if (right < size && comp(heap[highestPriority], heap[right])) {
            highestPriority = right;
        }

        if (highestPriority != index) {
            std::swap(heap[index], heap[highestPriority]);
            heapifyDown(highestPriority);
        }
    }

public:
    PriorityQueue() {}

    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    T top() const {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0];
    }

    void pop() {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    bool empty() const {
        return heap.empty();
    }

    int size() const {
        return heap.size();
    }
};
```

This implementation allows custom comparators, enabling both max and min priority queues.

### Using C++ Standard Library

C++ also offers a built-in priority queue in the Standard Library:

```cpp
#include <queue>
#include <iostream>

int main() {
    // Max priority queue (default)
    std::priority_queue<int> maxPQ;
    maxPQ.push(10);
    maxPQ.push(30);
    maxPQ.push(20);

    while (!maxPQ.empty()) {
        std::cout << maxPQ.top() << " ";  // Output: 30 20 10
        maxPQ.pop();
    }
    std::cout << std::endl;

    // Min priority queue using comparison function
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;
    minPQ.push(10);
    minPQ.push(30);
    minPQ.push(20);

    while (!minPQ.empty()) {
        std::cout << minPQ.top() << " ";  // Output: 10 20 30
        minPQ.pop();
    }
    std::cout << std::endl;

    return 0;
}
```

## 📡 Advanced Heap Operations

Heaps can support additional operations with varying levels of efficiency:

### 1. Decrease Key / Increase Key - O(log n)

Changing the priority of an element:

```cpp
// For MinHeap - Decrease key
void decreaseKey(int index, T newValue) {
    if (newValue > heap[index]) {
        throw std::invalid_argument("New value is greater than current value");
    }

    heap[index] = newValue;
    heapifyUp(index);
}

// For MaxHeap - Increase key
void increaseKey(int index, T newValue) {
    if (newValue < heap[index]) {
        throw std::invalid_argument("New value is less than current value");
    }

    heap[index] = newValue;
    heapifyUp(index);
}
```

Note: These operations require knowing the index of the element to modify, which is not typically provided by a standard heap interface. In practice, this would need additional data structures to track element positions.

### 2. Delete at Index - O(log n)

Removing an element from an arbitrary position:

```cpp
void deleteAt(int index) {
    if (index < 0 || index >= heap.size()) {
        throw std::out_of_range("Index out of range");
    }

    // Replace with last element
    heap[index] = heap.back();
    heap.pop_back();

    if (index < heap.size()) {
        // Try heapify up and down
        heapifyUp(index);
        heapifyDown(index);
    }
}
```

### 3. Merge Heaps - O(n)

Combining two heaps:

```cpp
void merge(const MaxHeap<T>& other) {
    // Add all elements from other heap
    for (const T& value : other.heap) {
        heap.push_back(value);
    }

    // Rebuild the heap
    for (int i = heap.size() / 2 - 1; i >= 0; i--) {
        heapifyDown(i);
    }
}
```

## 🧮 Heap Applications

Heaps are incredibly useful data structures with numerous applications:

### 1. Priority Queues

As we've seen, heaps are the backbone of priority queue implementations, which are used in:

- Task scheduling in operating systems
- Job sequencing in batch processing
- Event-driven simulation systems
- Bandwidth management in network traffic

### 2. Graph Algorithms

Many graph algorithms rely on heaps:

- Dijkstra's algorithm for shortest paths
- Prim's algorithm for minimum spanning trees
- A\* search algorithm for pathfinding

### 3. K-th Element Problems

Heaps efficiently solve problems involving k-th smallest/largest elements:

- Finding the k largest elements in an array
- Finding the k closest points to a given point
- Median maintenance in a data stream

### 4. External Sorting

When sorting large files that don't fit in memory, heaps help manage chunks efficiently.

### Two-Heap Pattern

Some problems can be elegantly solved using two heaps:

**Example: Finding the median in a stream of numbers**

```cpp
class MedianFinder {
private:
    std::priority_queue<int> maxHeap;                             // Lower half elements
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;  // Upper half elements

public:
    void addNum(int num) {
        // Add to appropriate heap
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
        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        } else {
            return maxHeap.top();
        }
    }
};
```

## 🔍 Advanced Heap Variants

While the binary heap is the most common implementation, several variants offer different trade-offs:

### 1. Binomial Heap

A collection of binomial trees that allows efficient merging of heaps:

- Union operation: O(log n)
- Basic operations (insert, extract min): O(log n)
- Excellent for merging priority queues

### 2. Fibonacci Heap

An advanced heap structure with amortized time complexity advantages:

- Insert: O(1) amortized
- Decrease key: O(1) amortized
- Extract min: O(log n) amortized
- Used theoretically in advanced graph algorithms

### 3. d-ary Heap

A generalization of binary heaps where each node has d children:

- Extract min: O(d log_d n)
- Decrease key: O(log_d n)
- Can be more efficient for certain applications like Dijkstra's algorithm

### 4. Leftist Heap and Skew Heap

Self-adjusting binary trees optimized for merging operations.

## 😄 Heap Jokes and Analogies

1. **The Nightclub Analogy**:
   "If data structures were nightclubs, a heap would be the exclusive VIP lounge where the highest-priority guest (or the lowest, depending on your heap type) always gets immediate service. Everyone else has to wait their turn based on their priority level, but the bouncers are constantly reorganizing the line to make sure it runs efficiently!"

2. **The Queue-Jumping Joke**:
   "Why did the element get kicked out of the priority queue? It couldn't maintain its heap-property behavior!"

3. **The Management Analogy**:
   "A max heap is like a perfectly organized corporation where the CEO is always the highest-paid person, every manager earns more than their direct reports, but there's no specified ordering among siblings at the same level. Similarly, a min heap is like a company where the intern always gets paid the least!"

4. **The Family Tree Joke**:
   "In a max heap family tree, parents are always more important than their children. It's the only family structure where 'my dad is bigger than your dad' is guaranteed by mathematical proof!"

5. **The Sorting Comparison Joke**:
   "Quicksort, Mergesort, and Heapsort walk into a bar. The bartender says 'I'll serve you in O(n log n) time.' Quicksort replies, 'But I might need O(n²) in the worst case!' Heapsort proudly states, 'I'm consistently O(n log n) no matter what,' while Mergesort mutters, 'Yeah, but you need extra space...'"

6. **The Playground Analogy**:
   "A heap is like a playground slide where the child at the top is either the heaviest (max-heap) or lightest (min-heap). When a new child arrives, they start at the bottom and bump their way up if they're heavier/lighter than their parent, causing a cascade of children sliding down until everyone finds their proper place."

7. **The Coffee Shop Joke**:
   "I tried to implement a priority queue at my coffee shop, but the customers kept complaining about being heapified every time someone new walked in."

8. **The Vegetable Garden Analogy**:
   "Think of a binary heap as a very peculiar vegetable garden. In a max heap garden, the largest vegetable is always at the top of the mound, with smaller ones arranged below it. When you harvest the top vegetable, you replace it with one from the bottom row and let it find its proper place by trickling down."

## 🧩 Common Heap Problems and Solutions

### 1. **K Closest Points to Origin** (LeetCode #973)

Given an array of points where points[i] = [xi, yi] and an integer k, return the k closest points to the origin (0, 0).

```cpp
std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
    auto comp = [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
    };

    // Use max heap to keep k closest points
    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, decltype(comp)> maxHeap(comp);

    for (const auto& point : points) {
        maxHeap.push(point);
        if (maxHeap.size() > k) {
            maxHeap.pop();
        }
    }

    std::vector<std::vector<int>> result;
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    return result;
}
```

### 2. **Last Stone Weight** (LeetCode #1046)

You are given an array of integers stones where stones[i] is the weight of the ith stone. On each turn, we choose the two heaviest stones and smash them together. The result of this smash is either both stones are destroyed, or the heavier stone remains with reduced weight.

```cpp
int lastStoneWeight(std::vector<int>& stones) {
    // Use a max heap to always get the two heaviest stones
    std::priority_queue<int> maxHeap(stones.begin(), stones.end());

    while (maxHeap.size() > 1) {
        int stone1 = maxHeap.top();
        maxHeap.pop();
        int stone2 = maxHeap.top();
        maxHeap.pop();

        if (stone1 != stone2) {
            maxHeap.push(stone1 - stone2);
        }
    }

    return maxHeap.empty() ? 0 : maxHeap.top();
}
```

### 3. **Kth Largest Element in an Array** (LeetCode #215)

Find the kth largest element in an unsorted array.

```cpp
int findKthLargest(std::vector<int>& nums, int k) {
    // Use a min heap to keep track of k largest elements
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    return minHeap.top();
}
```

## 🚀 LeetCode Practice Problems

### Easy:

1. **Last Stone Weight** (LeetCode #1046)

   - We've already covered this above.

2. **Kth Largest Element in a Stream** (LeetCode #703)

   ```cpp
   class KthLargest {
   private:
       std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
       int k;

   public:
       KthLargest(int k, std::vector<int>& nums) : k(k) {
           for (int num : nums) {
               add(num);
           }
       }

       int add(int val) {
           minHeap.push(val);
           if (minHeap.size() > k) {
               minHeap.pop();
           }
           return minHeap.top();
       }
   };
   ```

3. **Is Binary Tree a Heap** (GeeksforGeeks)

   ```cpp
   bool isHeap(TreeNode* root) {
       if (!root) return true;

       std::queue<TreeNode*> q;
       q.push(root);
       bool nullSeen = false;

       while (!q.empty()) {
           TreeNode* node = q.front();
           q.pop();

           // If we've seen a null node, all subsequent nodes should be null
           if (node->left) {
               if (nullSeen || node->left->val >= node->val) {
                   return false;
               }
               q.push(node->left);
           } else {
               nullSeen = true;
           }

           if (node->right) {
               if (nullSeen || node->right->val >= node->val) {
                   return false;
               }
               q.push(node->right);
           } else {
               nullSeen = true;
           }
       }

       return true;
   }
   ```

### Medium:

1. **Kth Largest Element in an Array** (LeetCode #215)

   - We've already covered this above.

2. **Top K Frequent Elements** (LeetCode #347)

   ```cpp
   std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
       // Count frequencies
       std::unordered_map<int, int> freq;
       for (int num : nums) {
           freq[num]++;
       }

       // Use min heap to track k most frequent elements
       using PII = std::pair<int, int>; // (frequency, number)
       auto comp = [](const PII& a, const PII& b) { return a.first > b.first; };
       std::priority_queue<PII, std::vector<PII>, decltype(comp)> minHeap(comp);

       for (const auto& [num, count] : freq) {
           minHeap.push({count, num});
           if (minHeap.size() > k) {
               minHeap.pop();
           }
       }

       // Extract results
       std::vector<int> result;
       while (!minHeap.empty()) {
           result.push_back(minHeap.top().second);
           minHeap.pop();
       }

       return result;
   }
   ```

3. **Merge K Sorted Lists** (LeetCode #23)

   ```cpp
   ListNode* mergeKLists(std::vector<ListNode*>& lists) {
       auto comp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
       std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(comp)> minHeap(comp);

       // Add the first node from each list
       for (ListNode* list : lists) {
           if (list) {
               minHeap.push(list);
           }
       }

       ListNode dummy(0);
       ListNode* tail = &dummy;

       while (!minHeap.empty()) {
           ListNode* node = minHeap.top();
           minHeap.pop();

           tail->next = node;
           tail = tail->next;

           if (node->next) {
               minHeap.push(node->next);
           }
       }

       return dummy.next;
   }
   ```

### Hard:

1. **Find Median from Data Stream** (LeetCode #295)

   ```cpp
   class MedianFinder {
   private:
       std::priority_queue<int> maxHeap;                             // Lower half elements
       std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;  // Upper half elements

   public:
       MedianFinder() {}

       void addNum(int num) {
           // Add to appropriate heap
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
           if (maxHeap.size() == minHeap.size()) {
               return (maxHeap.top() + minHeap.top()) / 2.0;
           } else {
               return maxHeap.top();
           }
       }
   };
   ```

2. **Sliding Window Median** (LeetCode #480)

   ```cpp
   std::vector<double> medianSlidingWindow(std::vector<int>& nums, int k) {
       std::multiset<int> window(nums.begin(), nums.begin() + k);
       auto mid = std::next(window.begin(), k / 2);
       std::vector<double> medians;

       for (int i = k; i <= nums.size(); i++) {
           // Get median from the current window
           medians.push_back(k % 2 == 0 ? (*mid + *std::prev(mid)) / 2.0 : *mid);

           if (i == nums.size()) break;

           // Insert the next element
           window.insert(nums[i]);

           // Adjust mid iterator
           if (nums[i] < *mid) {
               mid--;
           }

           // Remove the element going out of the window
           if (nums[i - k] <= *mid) {
               mid++;
           }

           auto it = window.find(nums[i - k]);
           window.erase(it);
       }

       return medians;
   }
   ```

3. **IPO** (LeetCode #502)

   ```cpp
   int findMaximizedCapital(int k, int w, std::vector<int>& profits, std::vector<int>& capital) {
       int n = profits.size();

       // Create pairs of (capital, profit)
       std::vector<std::pair<int, int>> projects;
       for (int i = 0; i < n; i++) {
           projects.push_back({capital[i], profits[i]});
       }

       // Sort by capital required
       std::sort(projects.begin(), projects.end());

       // Max heap to store projects we can afford
       std::priority_queue<int> maxHeap;

       int i = 0;
       while (k > 0) {
           // Add all projects we can currently afford to the max heap
           while (i < n && projects[i].first <= w) {
               maxHeap.push(projects[i].second);
               i++;
           }

           if (maxHeap.empty()) break;

           // Take the most profitable project
           w += maxHeap.top();
           maxHeap.pop();
           k--;
       }

       return w;
   }
   ```

## 🔮 Tomorrow's Preview

Tomorrow, we'll explore Heap Sort in more detail and dive into other applications of Heap data structures. We'll learn:

- Advanced implementations of Heap Sort
- Optimizations and variations
- Heap operations in different contexts
- Using heaps to solve complex problems
- Performance benchmarking of heap-based solutions against other approaches

_If heaps are like priority management systems that always keep the most important element at the top, heap sort is like an efficient file organizer that uses this property to sort your documents. By repeatedly extracting the highest-priority item and putting it in its final position, heap sort manages to sort an array with the reliability of merge sort but without the extra space overhead._

Stay tuned for our journey into the world of heap-based algorithms!

#DSAin45 #Day25 #Heaps #PriorityQueues #DataStructures #CPlusPlus #BinaryHeap #HeapSort #Algorithms #CodingInterview
