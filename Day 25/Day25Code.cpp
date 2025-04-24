#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>
#include <ctime>
#include <random>
#include <iomanip>
#include <chrono>
#include <unordered_map>

/**
 * @brief Comprehensive Heap Implementation for #DSAin45 Day 25
 *
 * This file provides a complete implementation of:
 * - Max Heap
 * - Min Heap
 * - Priority Queue
 * - Heap Sort
 * - Real-world Heap Applications
 * - Performance benchmarks
 *
 * @author #DSAin45 Course
 * @date Day 25 of 45-Day DSA Challenge
 */

/****************************************************************************************
 * BINARY HEAP IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Max Heap Implementation
 * A binary heap that maintains the max-heap property (parent >= children)
 */
template <typename T>
class MaxHeap
{
private:
    std::vector<T> heap;

    /**
     * @brief Get parent index
     * @param i Child index
     * @return Parent index
     */
    int parent(int i)
    {
        return (i - 1) / 2;
    }

    /**
     * @brief Get left child index
     * @param i Parent index
     * @return Left child index
     */
    int leftChild(int i)
    {
        return 2 * i + 1;
    }

    /**
     * @brief Get right child index
     * @param i Parent index
     * @return Right child index
     */
    int rightChild(int i)
    {
        return 2 * i + 2;
    }

    /**
     * @brief Heapify up (fix heap property upward)
     * @param index Starting index for heapify operation
     */
    void heapifyUp(int index)
    {
        // If we reach the root, we're done
        if (index == 0)
            return;

        int p = parent(index);
        // If parent is smaller than the current node, swap and continue heapifying up
        if (heap[p] < heap[index])
        {
            std::swap(heap[p], heap[index]);
            heapifyUp(p);
        }
    }

    /**
     * @brief Heapify down (fix heap property downward)
     * @param index Starting index for heapify operation
     */
    void heapifyDown(int index)
    {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        int size = heap.size();

        // Check if left child is larger than current node
        if (left < size && heap[left] > heap[largest])
        {
            largest = left;
        }

        // Check if right child is larger than current largest
        if (right < size && heap[right] > heap[largest])
        {
            largest = right;
        }

        // If largest isn't the current node, swap and continue heapifying down
        if (largest != index)
        {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    /**
     * @brief Default constructor
     */
    MaxHeap() {}

    /**
     * @brief Construct a new Max Heap from an array
     * @param array Input array
     */
    MaxHeap(const std::vector<T> &array)
    {
        heap = array;
        int n = heap.size();

        // Start from the last non-leaf node and heapify down
        // This is an O(n) operation rather than O(n log n)
        for (int i = n / 2 - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }

    /**
     * @brief Insert a new element into the heap
     * @param value Value to insert
     */
    void insert(T value)
    {
        // Add the new element to the end of the heap
        heap.push_back(value);

        // Maintain the heap property by heapifying up
        heapifyUp(heap.size() - 1);
    }

    /**
     * @brief Remove and return the maximum element
     * @return Maximum element
     */
    T extractMax()
    {
        if (heap.empty())
        {
            throw std::runtime_error("Heap is empty");
        }

        // The root contains the maximum element
        T max = heap[0];

        // Replace the root with the last element
        heap[0] = heap.back();
        heap.pop_back();

        // Maintain the heap property by heapifying down
        if (!heap.empty())
        {
            heapifyDown(0);
        }

        return max;
    }

    /**
     * @brief Get the maximum element without removing it
     * @return Maximum element
     */
    T getMax() const
    {
        if (heap.empty())
        {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    /**
     * @brief Check if the heap is empty
     * @return True if empty, false otherwise
     */
    bool isEmpty() const
    {
        return heap.empty();
    }

    /**
     * @brief Get the size of the heap
     * @return Number of elements in the heap
     */
    int size() const
    {
        return heap.size();
    }

    /**
     * @brief Access the internal vector
     * @return Heap vector
     */
    const std::vector<T> &getHeapArray() const
    {
        return heap;
    }

    /**
     * @brief Print the heap array (for debugging)
     */
    void print() const
    {
        std::cout << "Heap Array: ";
        for (const T &val : heap)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    /**
     * @brief Increase the key (priority) of an element
     * @param index Index of the element
     * @param newValue New value (must be greater than current)
     */
    void increaseKey(int index, T newValue)
    {
        if (index < 0 || index >= heap.size())
        {
            throw std::out_of_range("Index out of range");
        }

        if (newValue < heap[index])
        {
            throw std::invalid_argument("New value is less than current value");
        }

        heap[index] = newValue;
        heapifyUp(index);
    }

    /**
     * @brief Delete element at a specific index
     * @param index Index to delete from
     */
    void deleteAt(int index)
    {
        if (index < 0 || index >= heap.size())
        {
            throw std::out_of_range("Index out of range");
        }

        // Increase key to infinity (or a very large value)
        increaseKey(index, std::numeric_limits<T>::max());

        // Extract max will remove the element with the highest priority
        extractMax();
    }

    /**
     * @brief Merge with another Max Heap
     * @param other Heap to merge with
     */
    void merge(const MaxHeap<T> &other)
    {
        // Add all elements from other heap
        for (const T &value : other.heap)
        {
            heap.push_back(value);
        }

        // Rebuild the heap (O(n) operation)
        int n = heap.size();
        for (int i = n / 2 - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }
};

/**
 * @brief Min Heap Implementation
 * A binary heap that maintains the min-heap property (parent <= children)
 */
template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;

    /**
     * @brief Get parent index
     * @param i Child index
     * @return Parent index
     */
    int parent(int i)
    {
        return (i - 1) / 2;
    }

    /**
     * @brief Get left child index
     * @param i Parent index
     * @return Left child index
     */
    int leftChild(int i)
    {
        return 2 * i + 1;
    }

    /**
     * @brief Get right child index
     * @param i Parent index
     * @return Right child index
     */
    int rightChild(int i)
    {
        return 2 * i + 2;
    }

    /**
     * @brief Heapify up (fix heap property upward)
     * @param index Starting index for heapify operation
     */
    void heapifyUp(int index)
    {
        // If we reach the root, we're done
        if (index == 0)
            return;

        int p = parent(index);
        // If parent is larger than the current node, swap and continue heapifying up
        if (heap[p] > heap[index])
        {
            std::swap(heap[p], heap[index]);
            heapifyUp(p);
        }
    }

    /**
     * @brief Heapify down (fix heap property downward)
     * @param index Starting index for heapify operation
     */
    void heapifyDown(int index)
    {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        int size = heap.size();

        // Check if left child is smaller than current node
        if (left < size && heap[left] < heap[smallest])
        {
            smallest = left;
        }

        // Check if right child is smaller than current smallest
        if (right < size && heap[right] < heap[smallest])
        {
            smallest = right;
        }

        // If smallest isn't the current node, swap and continue heapifying down
        if (smallest != index)
        {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    /**
     * @brief Default constructor
     */
    MinHeap() {}

    /**
     * @brief Construct a new Min Heap from an array
     * @param array Input array
     */
    MinHeap(const std::vector<T> &array)
    {
        heap = array;
        int n = heap.size();

        // Start from the last non-leaf node and heapify down
        // This is an O(n) operation rather than O(n log n)
        for (int i = n / 2 - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }

    /**
     * @brief Insert a new element into the heap
     * @param value Value to insert
     */
    void insert(T value)
    {
        // Add the new element to the end of the heap
        heap.push_back(value);

        // Maintain the heap property by heapifying up
        heapifyUp(heap.size() - 1);
    }

    /**
     * @brief Remove and return the minimum element
     * @return Minimum element
     */
    T extractMin()
    {
        if (heap.empty())
        {
            throw std::runtime_error("Heap is empty");
        }

        // The root contains the minimum element
        T min = heap[0];

        // Replace the root with the last element
        heap[0] = heap.back();
        heap.pop_back();

        // Maintain the heap property by heapifying down
        if (!heap.empty())
        {
            heapifyDown(0);
        }

        return min;
    }

    /**
     * @brief Get the minimum element without removing it
     * @return Minimum element
     */
    T getMin() const
    {
        if (heap.empty())
        {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    /**
     * @brief Check if the heap is empty
     * @return True if empty, false otherwise
     */
    bool isEmpty() const
    {
        return heap.empty();
    }

    /**
     * @brief Get the size of the heap
     * @return Number of elements in the heap
     */
    int size() const
    {
        return heap.size();
    }

    /**
     * @brief Access the internal vector
     * @return Heap vector
     */
    const std::vector<T> &getHeapArray() const
    {
        return heap;
    }

    /**
     * @brief Print the heap array (for debugging)
     */
    void print() const
    {
        std::cout << "Heap Array: ";
        for (const T &val : heap)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    /**
     * @brief Decrease the key (priority) of an element
     * @param index Index of the element
     * @param newValue New value (must be less than current)
     */
    void decreaseKey(int index, T newValue)
    {
        if (index < 0 || index >= heap.size())
        {
            throw std::out_of_range("Index out of range");
        }

        if (newValue > heap[index])
        {
            throw std::invalid_argument("New value is greater than current value");
        }

        heap[index] = newValue;
        heapifyUp(index);
    }

    /**
     * @brief Delete element at a specific index
     * @param index Index to delete from
     */
    void deleteAt(int index)
    {
        if (index < 0 || index >= heap.size())
        {
            throw std::out_of_range("Index out of range");
        }

        // Decrease key to negative infinity (or a very small value)
        decreaseKey(index, std::numeric_limits<T>::min());

        // Extract min will remove the element with the highest priority
        extractMin();
    }

    /**
     * @brief Merge with another Min Heap
     * @param other Heap to merge with
     */
    void merge(const MinHeap<T> &other)
    {
        // Add all elements from other heap
        for (const T &value : other.heap)
        {
            heap.push_back(value);
        }

        // Rebuild the heap (O(n) operation)
        int n = heap.size();
        for (int i = n / 2 - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }
};

/****************************************************************************************
 * PRIORITY QUEUE IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Custom Priority Queue Implementation
 * Flexible priority queue that can be either a max or min priority queue
 * based on the provided comparator
 */
template <typename T, typename Compare = std::less<T>>
class PriorityQueue
{
private:
    std::vector<T> heap;
    Compare comp;

    /**
     * @brief Get parent index
     * @param i Child index
     * @return Parent index
     */
    int parent(int i)
    {
        return (i - 1) / 2;
    }

    /**
     * @brief Get left child index
     * @param i Parent index
     * @return Left child index
     */
    int leftChild(int i)
    {
        return 2 * i + 1;
    }

    /**
     * @brief Get right child index
     * @param i Parent index
     * @return Right child index
     */
    int rightChild(int i)
    {
        return 2 * i + 2;
    }

    /**
     * @brief Heapify up (fix heap property upward)
     * @param index Starting index for heapify operation
     */
    void heapifyUp(int index)
    {
        if (index == 0)
            return;

        int p = parent(index);
        if (comp(heap[p], heap[index]))
        {
            std::swap(heap[p], heap[index]);
            heapifyUp(p);
        }
    }

    /**
     * @brief Heapify down (fix heap property downward)
     * @param index Starting index for heapify operation
     */
    void heapifyDown(int index)
    {
        int highestPriority = index;
        int left = leftChild(index);
        int right = rightChild(index);
        int size = heap.size();

        if (left < size && comp(heap[highestPriority], heap[left]))
        {
            highestPriority = left;
        }

        if (right < size && comp(heap[highestPriority], heap[right]))
        {
            highestPriority = right;
        }

        if (highestPriority != index)
        {
            std::swap(heap[index], heap[highestPriority]);
            heapifyDown(highestPriority);
        }
    }

public:
    /**
     * @brief Default constructor
     */
    PriorityQueue() {}

    /**
     * @brief Constructor with custom comparator
     * @param comparator Comparison function
     */
    PriorityQueue(Compare comparator) : comp(comparator) {}

    /**
     * @brief Add an element to the priority queue
     * @param value Value to add
     */
    void push(const T &value)
    {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    /**
     * @brief Get the highest priority element
     * @return Highest priority element
     */
    T top() const
    {
        if (heap.empty())
        {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0];
    }

    /**
     * @brief Remove the highest priority element
     */
    void pop()
    {
        if (heap.empty())
        {
            throw std::runtime_error("Priority queue is empty");
        }

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty())
        {
            heapifyDown(0);
        }
    }

    /**
     * @brief Check if priority queue is empty
     * @return True if empty, false otherwise
     */
    bool empty() const
    {
        return heap.empty();
    }

    /**
     * @brief Get the size of the priority queue
     * @return Number of elements
     */
    int size() const
    {
        return heap.size();
    }
};

/****************************************************************************************
 * HEAP SORT IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Heap Sort Algorithm
 * Implementation of the Heap Sort using our MaxHeap class
 *
 * @param arr Array to sort
 */
template <typename T>
void heapSort(std::vector<T> &arr)
{
    // Build a max heap
    MaxHeap<T> maxHeap(arr);

    // Extract elements from the heap in order
    for (int i = arr.size() - 1; i >= 0; i--)
    {
        arr[i] = maxHeap.extractMax();
    }
}

/**
 * @brief Efficient in-place Heap Sort implementation
 *
 * @param arr Array to sort
 */
template <typename T>
void heapSortInPlace(std::vector<T> &arr)
{
    int n = arr.size();

    // Build a max heap (in-place)
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    // Extract elements one by one
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root to end
        std::swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

/**
 * @brief Heapify function for in-place heap sort
 *
 * @param arr Array being sorted
 * @param n Size of the heap
 * @param i Index to heapify from
 */
template <typename T>
void heapify(std::vector<T> &arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Check if left child is larger than root
    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }

    // Check if right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }

    // If largest is not root
    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

/****************************************************************************************
 * REAL-WORLD HEAP APPLICATIONS
 ****************************************************************************************/

/**
 * @brief Finding k-th smallest element using a max heap
 *
 * @param arr Input array
 * @param k k-th position (1-based)
 * @return k-th smallest element
 */
template <typename T>
T findKthSmallest(const std::vector<T> &arr, int k)
{
    if (k <= 0 || k > arr.size())
    {
        throw std::invalid_argument("k is out of range");
    }

    // Use a max heap to keep track of k smallest elements
    MaxHeap<T> maxHeap;

    for (const T &num : arr)
    {
        // If we have fewer than k elements, just add
        if (maxHeap.size() < k)
        {
            maxHeap.insert(num);
        }
        // If current element is smaller than the largest in the heap,
        // replace the largest with the current
        else if (num < maxHeap.getMax())
        {
            maxHeap.extractMax();
            maxHeap.insert(num);
        }
    }

    // The largest element in the heap is the k-th smallest
    return maxHeap.getMax();
}

/**
 * @brief Finding k-th largest element using a min heap
 *
 * @param arr Input array
 * @param k k-th position (1-based)
 * @return k-th largest element
 */
template <typename T>
T findKthLargest(const std::vector<T> &arr, int k)
{
    if (k <= 0 || k > arr.size())
    {
        throw std::invalid_argument("k is out of range");
    }

    // Use a min heap to keep track of k largest elements
    MinHeap<T> minHeap;

    for (const T &num : arr)
    {
        // If we have fewer than k elements, just add
        if (minHeap.size() < k)
        {
            minHeap.insert(num);
        }
        // If current element is larger than the smallest in the heap,
        // replace the smallest with the current
        else if (num > minHeap.getMin())
        {
            minHeap.extractMin();
            minHeap.insert(num);
        }
    }

    // The smallest element in the heap is the k-th largest
    return minHeap.getMin();
}

/**
 * @brief Top K frequent elements
 *
 * @param nums Input array
 * @param k Number of frequent elements to find
 * @return Vector of k most frequent elements
 */
std::vector<int> topKFrequent(const std::vector<int> &nums, int k)
{
    // Count frequencies
    std::unordered_map<int, int> freq;
    for (int num : nums)
    {
        freq[num]++;
    }

    // Create min heap using custom comparator to order by frequency
    auto comp = [](const std::pair<int, int> &a, const std::pair<int, int> &b)
    {
        return a.second > b.second; // For min heap based on frequency
    };

    PriorityQueue<std::pair<int, int>, decltype(comp)> pq(comp);

    // Add elements to the priority queue
    for (const auto &[num, count] : freq)
    {
        pq.push({num, count});
        if (pq.size() > k)
        {
            pq.pop();
        }
    }

    // Extract results
    std::vector<int> result;
    while (!pq.empty())
    {
        result.push_back(pq.top().first);
        pq.pop();
    }

    std::reverse(result.begin(), result.end()); // To get them in descending order
    return result;
}

/**
 * @brief MedianFinder class to find the median from a data stream
 * Uses two heaps to track the median efficiently
 */
class MedianFinder
{
private:
    // Max heap for the lower half (numbers less than the median)
    std::priority_queue<int> maxHeap;

    // Min heap for the upper half (numbers greater than the median)
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

public:
    /**
     * @brief Default constructor
     */
    MedianFinder() {}

    /**
     * @brief Add a number to the data structure
     * @param num Number to add
     */
    void addNum(int num)
    {
        // Add to appropriate heap
        if (maxHeap.empty() || num <= maxHeap.top())
        {
            maxHeap.push(num);
        }
        else
        {
            minHeap.push(num);
        }

        // Balance heaps
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

    /**
     * @brief Find the median of all numbers seen so far
     * @return Median value
     */
    double findMedian()
    {
        if (maxHeap.size() == minHeap.size())
        {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
        else
        {
            return maxHeap.top();
        }
    }
};

/**
 * @brief Merge K sorted arrays using a min heap
 *
 * @param arrays Vector of sorted arrays
 * @return Single sorted array containing all elements
 */
std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>> &arrays)
{
    // Custom comparator for the min heap
    struct ComparePair
    {
        bool operator()(const std::tuple<int, int, int> &a, const std::tuple<int, int, int> &b)
        {
            return std::get<0>(a) > std::get<0>(b); // Compare values (min heap)
        }
    };

    // Min heap of tuples: (value, array_index, element_index)
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, ComparePair> minHeap;

    // Initialize the min heap with the first element from each array
    for (int i = 0; i < arrays.size(); i++)
    {
        if (!arrays[i].empty())
        {
            minHeap.push({arrays[i][0], i, 0});
        }
    }

    std::vector<int> result;

    // Process the min heap
    while (!minHeap.empty())
    {
        auto [value, arrayIndex, elementIndex] = minHeap.top();
        minHeap.pop();

        // Add the smallest element to the result
        result.push_back(value);

        // Move to the next element in that array
        elementIndex++;
        if (elementIndex < arrays[arrayIndex].size())
        {
            minHeap.push({arrays[arrayIndex][elementIndex], arrayIndex, elementIndex});
        }
    }

    return result;
}

/**
 * @brief Dijkstra's algorithm for shortest paths using a priority queue
 *
 * @param graph Adjacency list representation of the graph
 * @param start Starting vertex
 * @param V Number of vertices
 * @return Vector of shortest distances from start to all other vertices
 */
std::vector<int> dijkstra(const std::vector<std::vector<std::pair<int, int>>> &graph, int start, int V)
{
    // Distance vector
    std::vector<int> dist(V, INT_MAX);
    dist[start] = 0;

    // Min priority queue (distance, vertex)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        pq;

    pq.push({0, start});

    while (!pq.empty())
    {
        auto [distance, u] = pq.top();
        pq.pop();

        // If the popped vertex is processed already, skip
        if (distance > dist[u])
            continue;

        // Process all neighbors
        for (const auto &[v, weight] : graph[u])
        {
            // If there is a shorter path to v through u
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

/****************************************************************************************
 * BENCHMARKING & UTILITIES
 ****************************************************************************************/

/**
 * @brief Generate a vector of random integers
 *
 * @param size Size of the vector
 * @param min Minimum value
 * @param max Maximum value
 * @return Random vector
 */
std::vector<int> generateRandomVector(int size, int min = 1, int max = 1000)
{
    std::vector<int> vec(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    for (int i = 0; i < size; i++)
    {
        vec[i] = dist(gen);
    }

    return vec;
}

/**
 * @brief Time the execution of heap operations
 *
 * @param size Size of the test data
 */
void benchmarkHeapOperations(int size)
{
    std::cout << "===== Heap Operations Benchmark =====" << std::endl;
    std::cout << "Data size: " << size << " elements" << std::endl
              << std::endl;

    auto randomData = generateRandomVector(size);

    // Benchmark Max Heap construction
    auto startMaxHeap = std::chrono::high_resolution_clock::now();
    MaxHeap<int> maxHeap(randomData);
    auto endMaxHeap = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msMaxHeap = endMaxHeap - startMaxHeap;
    std::cout << "Max Heap construction time: " << msMaxHeap.count() << " ms" << std::endl;

    // Benchmark Min Heap construction
    auto startMinHeap = std::chrono::high_resolution_clock::now();
    MinHeap<int> minHeap(randomData);
    auto endMinHeap = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msMinHeap = endMinHeap - startMinHeap;
    std::cout << "Min Heap construction time: " << msMinHeap.count() << " ms" << std::endl;

    // Benchmark insertion
    maxHeap = MaxHeap<int>(); // Reset
    auto startInsert = std::chrono::high_resolution_clock::now();
    for (int val : randomData)
    {
        maxHeap.insert(val);
    }
    auto endInsert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msInsert = endInsert - startInsert;
    std::cout << "One-by-one insertion time: " << msInsert.count() << " ms" << std::endl;

    // Benchmark extraction
    auto startExtract = std::chrono::high_resolution_clock::now();
    while (!maxHeap.isEmpty())
    {
        maxHeap.extractMax();
    }
    auto endExtract = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msExtract = endExtract - startExtract;
    std::cout << "Element extraction time: " << msExtract.count() << " ms" << std::endl;

    // Benchmark heap sort
    auto startHeapSort = std::chrono::high_resolution_clock::now();
    std::vector<int> sortData = randomData;
    heapSort(sortData);
    auto endHeapSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msHeapSort = endHeapSort - startHeapSort;
    std::cout << "Heap Sort time: " << msHeapSort.count() << " ms" << std::endl;

    // Benchmark in-place heap sort
    auto startInPlace = std::chrono::high_resolution_clock::now();
    sortData = randomData;
    heapSortInPlace(sortData);
    auto endInPlace = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msInPlace = endInPlace - startInPlace;
    std::cout << "In-place Heap Sort time: " << msInPlace.count() << " ms" << std::endl;

    // Benchmark std::sort for comparison
    auto startStdSort = std::chrono::high_resolution_clock::now();
    sortData = randomData;
    std::sort(sortData.begin(), sortData.end());
    auto endStdSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> msStdSort = endStdSort - startStdSort;
    std::cout << "std::sort time: " << msStdSort.count() << " ms" << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Verify the correctness of heap operations
 */
void verifyHeapOperations()
{
    std::cout << "===== Heap Operations Verification =====" << std::endl;

    // Test data
    std::vector<int> testData = {4, 10, 3, 5, 1, 7, 9};

    // Max Heap test
    std::cout << "Max Heap Test:" << std::endl;
    MaxHeap<int> maxHeap;
    for (int val : testData)
    {
        maxHeap.insert(val);
        std::cout << "After inserting " << val << ": ";
        maxHeap.print();
    }

    std::cout << "Extracting elements: ";
    while (!maxHeap.isEmpty())
    {
        std::cout << maxHeap.extractMax() << " ";
    }
    std::cout << std::endl
              << std::endl;

    // Min Heap test
    std::cout << "Min Heap Test:" << std::endl;
    MinHeap<int> minHeap;
    for (int val : testData)
    {
        minHeap.insert(val);
        std::cout << "After inserting " << val << ": ";
        minHeap.print();
    }

    std::cout << "Extracting elements: ";
    while (!minHeap.isEmpty())
    {
        std::cout << minHeap.extractMin() << " ";
    }
    std::cout << std::endl
              << std::endl;

    // Heap Sort test
    std::cout << "Heap Sort Test:" << std::endl;
    std::vector<int> sortData = testData;
    std::cout << "Before sorting: ";
    for (int val : sortData)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    heapSort(sortData);

    std::cout << "After sorting: ";
    for (int val : sortData)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl
              << std::endl;

    // Priority Queue test
    std::cout << "Priority Queue Test:" << std::endl;
    PriorityQueue<int> maxPQ; // Default is max priority queue
    for (int val : testData)
    {
        maxPQ.push(val);
    }

    std::cout << "Max Priority Queue elements: ";
    while (!maxPQ.empty())
    {
        std::cout << maxPQ.top() << " ";
        maxPQ.pop();
    }
    std::cout << std::endl;

    PriorityQueue<int, std::greater<int>> minPQ; // Min priority queue
    for (int val : testData)
    {
        minPQ.push(val);
    }

    std::cout << "Min Priority Queue elements: ";
    while (!minPQ.empty())
    {
        std::cout << minPQ.top() << " ";
        minPQ.pop();
    }
    std::cout << std::endl
              << std::endl;
}

/**
 * @brief Demonstrate real-world heap applications
 */
void demonstrateHeapApplications()
{
    std::cout << "===== Heap Applications Demonstration =====" << std::endl;

    // Test data
    std::vector<int> testData = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    // K-th smallest/largest element
    int k = 3;
    std::cout << "Test data: ";
    for (int val : testData)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << k << "-th smallest element: " << findKthSmallest(testData, k) << std::endl;
    std::cout << k << "-th largest element: " << findKthLargest(testData, k) << std::endl;

    // Top K frequent
    std::vector<int> freqData = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4};
    k = 2;
    std::cout << "\nFrequency data: ";
    for (int val : freqData)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Top " << k << " frequent elements: ";
    std::vector<int> topK = topKFrequent(freqData, k);
    for (int val : topK)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Median finder
    std::cout << "\nMedian finder demonstration:" << std::endl;
    MedianFinder medianFinder;
    std::vector<int> stream = {2, 3, 4, 8, 1, 5};

    for (int i = 0; i < stream.size(); i++)
    {
        medianFinder.addNum(stream[i]);
        std::cout << "After adding " << stream[i] << ", median is: " << medianFinder.findMedian() << std::endl;
    }

    // Merge K sorted arrays
    std::cout << "\nMerge K sorted arrays demonstration:" << std::endl;
    std::vector<std::vector<int>> sortedArrays = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}};

    std::vector<int> merged = mergeKSortedArrays(sortedArrays);
    std::cout << "Merged array: ";
    for (int val : merged)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Dijkstra's algorithm (simple graph)
    std::cout << "\nDijkstra's algorithm demonstration:" << std::endl;
    int V = 5; // Number of vertices
    std::vector<std::vector<std::pair<int, int>>> graph(V);

    // Add edges (u, v, weight)
    auto addEdge = [&graph](int u, int v, int w)
    {
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // For undirected graph
    };

    addEdge(0, 1, 4);
    addEdge(0, 2, 1);
    addEdge(1, 2, 2);
    addEdge(1, 3, 5);
    addEdge(2, 3, 8);
    addEdge(2, 4, 10);
    addEdge(3, 4, 2);

    int startVertex = 0;
    std::vector<int> shortestPaths = dijkstra(graph, startVertex, V);

    std::cout << "Shortest distances from vertex " << startVertex << ":" << std::endl;
    for (int i = 0; i < V; i++)
    {
        std::cout << "To vertex " << i << ": " << shortestPaths[i] << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Demonstrate heap visualization (ASCII art)
 */
void visualizeHeap(const std::vector<int> &heapArray)
{
    if (heapArray.empty())
    {
        std::cout << "Heap is empty" << std::endl;
        return;
    }

    int height = static_cast<int>(std::log2(heapArray.size())) + 1;
    int width = (1 << height) - 1;

    // Create a matrix to represent the tree
    std::vector<std::vector<std::string>> tree(height, std::vector<std::string>(width, " "));

    std::function<void(int, int, int, int)> fillTree = [&](int node, int depth, int left, int right)
    {
        if (node >= heapArray.size())
            return;

        int mid = (left + right) / 2;
        tree[depth][mid] = std::to_string(heapArray[node]);

        // Fill left and right subtrees
        fillTree(2 * node + 1, depth + 1, left, mid);
        fillTree(2 * node + 2, depth + 1, mid + 1, right);
    };

    fillTree(0, 0, 0, width - 1);

    // Print the tree
    std::cout << "Heap Visualization:" << std::endl;
    for (const auto &row : tree)
    {
        for (const auto &cell : row)
        {
            if (cell == " ")
            {
                std::cout << "  ";
            }
            else
            {
                std::cout << cell << " ";
            }
        }
        std::cout << std::endl;
    }

    // Print array representation
    std::cout << "Array representation: [";
    for (size_t i = 0; i < heapArray.size(); i++)
    {
        std::cout << heapArray[i];
        if (i < heapArray.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

/**
 * @brief Main function for heap demonstration
 */
int main()
{
    std::cout << "======= Heap Data Structure Implementation =======" << std::endl;
    std::cout << "Day 25 of #DSAin45" << std::endl
              << std::endl;

    // Verify basic heap operations
    verifyHeapOperations();

    // Demonstrate heap applications
    demonstrateHeapApplications();

    // Visualize a heap
    std::vector<int> heapForVisualization = {90, 70, 60, 50, 40, 30, 20};
    visualizeHeap(heapForVisualization);
    std::cout << std::endl;

    // Benchmark heap operations (optional)
    int benchmarkSize = 100000; // Adjust based on your system
    benchmarkHeapOperations(benchmarkSize);

    std::cout << "======= End of Heap Implementation =======" << std::endl;
    return 0;
}