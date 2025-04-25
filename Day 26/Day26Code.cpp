#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <random>
#include <functional>
#include <string>
#include <list>
#include <set>

/**
 * @brief Comprehensive Heap Sort & Applications Implementation for #DSAin45 Day 26
 *
 * This file provides a complete implementation of:
 * - Heap Sort (multiple variations)
 * - K-th Largest/Smallest Element Finding
 * - Top K Frequent Elements
 * - Median in a Data Stream
 * - Merge K Sorted Lists
 * - K Closest Points to Origin
 * - Performance benchmarks
 *
 * @author #DSAin45 Course
 * @date Day 26 of 45-Day DSA Challenge
 */

/****************************************************************************************
 * HEAP SORT IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Heapify a subtree rooted at node i in an array
 * This is the recursive version of heapify
 *
 * @param arr Array to heapify
 * @param n Size of the heap (not necessarily the whole array)
 * @param i Index of the subtree root
 */
template <typename T>
void heapify(std::vector<T> &arr, int n, int i)
{
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // Left child index
    int right = 2 * i + 2; // Right child index

    // Check if left child exists and is greater than root
    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }

    // Check if right child exists and is greater than current largest
    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }

    // If largest is not the root, swap and recursively heapify the affected subtree
    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

/**
 * @brief Heapify using iteration instead of recursion
 * This often performs better in practice due to less function call overhead
 *
 * @param arr Array to heapify
 * @param n Size of the heap
 * @param i Index of the subtree root
 */
template <typename T>
void heapifyIterative(std::vector<T> &arr, int n, int i)
{
    int largest;
    int currentIndex = i;

    while (true)
    {
        largest = currentIndex;
        int left = 2 * currentIndex + 1;
        int right = 2 * currentIndex + 2;

        // Check left child
        if (left < n && arr[left] > arr[largest])
        {
            largest = left;
        }

        // Check right child
        if (right < n && arr[right] > arr[largest])
        {
            largest = right;
        }

        // If largest is still the current index, we're done
        if (largest == currentIndex)
        {
            break;
        }

        // Otherwise, swap and continue
        std::swap(arr[currentIndex], arr[largest]);
        currentIndex = largest;
    }
}

/**
 * @brief Standard heap sort implementation using recursive heapify
 *
 * @param arr Array to sort
 */
template <typename T>
void heapSort(std::vector<T> &arr)
{
    int n = arr.size();

    // Build a max heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root (maximum) to end
        std::swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

/**
 * @brief Heap sort using iterative heapify
 *
 * @param arr Array to sort
 */
template <typename T>
void heapSortIterative(std::vector<T> &arr)
{
    int n = arr.size();

    // Build a max heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapifyIterative(arr, n, i);
    }

    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root (maximum) to end
        std::swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapifyIterative(arr, i, 0);
    }
}

/**
 * @brief Heap sort using STL heap operations
 *
 * @param arr Array to sort
 */
template <typename T>
void heapSortSTL(std::vector<T> &arr)
{
    // Convert vector to a heap in-place
    std::make_heap(arr.begin(), arr.end());

    // Pop each element from the heap and place it at the end
    for (auto it = arr.end(); it != arr.begin(); --it)
    {
        std::pop_heap(arr.begin(), it);
    }
}

/****************************************************************************************
 * K-TH LARGEST/SMALLEST ELEMENT IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Find the k-th largest element in an unsorted array using a min heap
 *
 * @param nums Input array
 * @param k Position (1-based) of the element to find
 * @return K-th largest element
 */
template <typename T>
T findKthLargest(const std::vector<T> &nums, int k)
{
    // Use a min heap to keep track of k largest elements
    std::priority_queue<T, std::vector<T>, std::greater<T>> minHeap;

    for (const T &num : nums)
    {
        minHeap.push(num);

        // If heap size exceeds k, remove the smallest element
        if (minHeap.size() > k)
        {
            minHeap.pop();
        }
    }

    // The top element is the k-th largest
    return minHeap.top();
}

/**
 * @brief Find the k-th smallest element in an unsorted array using a max heap
 *
 * @param nums Input array
 * @param k Position (1-based) of the element to find
 * @return K-th smallest element
 */
template <typename T>
T findKthSmallest(const std::vector<T> &nums, int k)
{
    // Use a max heap to keep track of k smallest elements
    std::priority_queue<T> maxHeap;

    for (const T &num : nums)
    {
        maxHeap.push(num);

        // If heap size exceeds k, remove the largest element
        if (maxHeap.size() > k)
        {
            maxHeap.pop();
        }
    }

    // The top element is the k-th smallest
    return maxHeap.top();
}

/**
 * @brief Alternative implementation using quickselect algorithm (average O(n))
 *
 * @param nums Input array
 * @param k Position (1-based) of the element to find
 * @return K-th largest element
 */
template <typename T>
T findKthLargestQuickSelect(std::vector<T> &nums, int k)
{
    // Convert to 0-indexed position
    k = nums.size() - k;

    auto quickSelect = [&](int left, int right, auto &quickSelect) -> T
    {
        // Partition around a pivot
        T pivot = nums[right];
        int p = left;

        for (int i = left; i < right; i++)
        {
            if (nums[i] <= pivot)
            {
                std::swap(nums[i], nums[p]);
                p++;
            }
        }

        std::swap(nums[p], nums[right]);

        // Check if we found the k-th element
        if (p == k)
        {
            return nums[p];
        }
        else if (p > k)
        {
            return quickSelect(left, p - 1, quickSelect);
        }
        else
        {
            return quickSelect(p + 1, right, quickSelect);
        }
    };

    return quickSelect(0, nums.size() - 1, quickSelect);
}

/****************************************************************************************
 * TOP K FREQUENT ELEMENTS IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Find the k most frequent elements in an array
 *
 * @param nums Input array
 * @param k Number of most frequent elements to find
 * @return Vector of k most frequent elements
 */
template <typename T>
std::vector<T> topKFrequent(const std::vector<T> &nums, int k)
{
    // Count frequencies
    std::unordered_map<T, int> freq;
    for (const T &num : nums)
    {
        freq[num]++;
    }

    // Use min heap to track k most frequent elements
    using PairType = std::pair<T, int>; // (element, frequency)
    auto comp = [](const PairType &a, const PairType &b)
    {
        return a.second > b.second; // For min heap based on frequency
    };

    std::priority_queue<PairType, std::vector<PairType>, decltype(comp)> minHeap(comp);

    for (const auto &[num, count] : freq)
    {
        minHeap.push({num, count});
        if (minHeap.size() > k)
        {
            minHeap.pop();
        }
    }

    // Extract results
    std::vector<T> result;
    while (!minHeap.empty())
    {
        result.push_back(minHeap.top().first);
        minHeap.pop();
    }

    std::reverse(result.begin(), result.end()); // For decreasing frequency order
    return result;
}

/**
 * @brief Bucket sort approach for top k frequent elements
 * Often faster than heap approach when the frequency range is limited
 *
 * @param nums Input array
 * @param k Number of most frequent elements to find
 * @return Vector of k most frequent elements
 */
template <typename T>
std::vector<T> topKFrequentBucket(const std::vector<T> &nums, int k)
{
    // Count frequencies
    std::unordered_map<T, int> freq;
    for (const T &num : nums)
    {
        freq[num]++;
    }

    // Create buckets where bucket[i] holds elements with frequency i
    std::vector<std::vector<T>> buckets(nums.size() + 1);
    for (const auto &[num, count] : freq)
    {
        buckets[count].push_back(num);
    }

    // Extract results from buckets (highest frequency first)
    std::vector<T> result;
    for (int i = buckets.size() - 1; i >= 0 && result.size() < k; i--)
    {
        for (const T &num : buckets[i])
        {
            result.push_back(num);
            if (result.size() == k)
            {
                break;
            }
        }
    }

    return result;
}

/****************************************************************************************
 * MEDIAN FINDER IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Class for finding the median from a data stream
 * Uses two heaps to efficiently track the median element
 */
class MedianFinder
{
private:
    // Max heap for the lower half of elements (less than or equal to median)
    std::priority_queue<int> maxHeap;

    // Min heap for the upper half of elements (greater than median)
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

public:
    /**
     * @brief Add a number to the data structure
     *
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
     * @brief Find the median of all elements added so far
     *
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

    /**
     * @brief Get the current size of the data structure
     *
     * @return Number of elements
     */
    int size() const
    {
        return maxHeap.size() + minHeap.size();
    }

    /**
     * @brief Check if the data structure is empty
     *
     * @return True if empty, false otherwise
     */
    bool empty() const
    {
        return maxHeap.empty() && minHeap.empty();
    }

    /**
     * @brief Reset the data structure
     */
    void clear()
    {
        // Create new empty heaps (no direct clear method for priority_queue)
        maxHeap = std::priority_queue<int>();
        minHeap = std::priority_queue<int, std::vector<int>, std::greater<int>>();
    }

    /**
     * @brief Get the current elements in the lower half (max heap)
     *
     * @return Vector of elements in the max heap
     */
    std::vector<int> getLowerHalf() const
    {
        std::vector<int> result;
        std::priority_queue<int> tempHeap = maxHeap; // Create a copy

        while (!tempHeap.empty())
        {
            result.push_back(tempHeap.top());
            tempHeap.pop();
        }

        return result;
    }

    /**
     * @brief Get the current elements in the upper half (min heap)
     *
     * @return Vector of elements in the min heap
     */
    std::vector<int> getUpperHalf() const
    {
        std::vector<int> result;
        std::priority_queue<int, std::vector<int>, std::greater<int>> tempHeap = minHeap; // Create a copy

        while (!tempHeap.empty())
        {
            result.push_back(tempHeap.top());
            tempHeap.pop();
        }

        return result;
    }
};

/**
 * @brief Function to find the median of a sliding window
 *
 * @param nums Input array
 * @param k Window size
 * @return Vector of medians for each window
 */
std::vector<double> medianSlidingWindow(const std::vector<int> &nums, int k)
{
    // Use multiset for efficient insertion, removal and access to median
    std::multiset<int> window(nums.begin(), nums.begin() + k);
    auto mid = std::next(window.begin(), k / 2);
    std::vector<double> medians;

    // Process each window
    for (int i = k; i <= nums.size(); i++)
    {
        // Calculate median from current window
        medians.push_back(k % 2 == 0 ? (*mid + *std::prev(mid)) / 2.0 : *mid);

        if (i == nums.size())
            break;

        // Add the incoming element
        window.insert(nums[i]);

        // Update the median iterator
        if (nums[i] < *mid)
            mid--;

        // Remove the outgoing element
        if (nums[i - k] <= *mid)
            mid++;
        window.erase(window.find(nums[i - k]));
    }

    return medians;
}

/****************************************************************************************
 * MERGE K SORTED LISTS IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Definition for singly-linked list node
 */
struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/**
 * @brief Merge k sorted linked lists
 *
 * @param lists Vector of list heads
 * @return Head of merged list
 */
ListNode *mergeKLists(std::vector<ListNode *> &lists)
{
    // Comparator for the min heap
    auto comp = [](ListNode *a, ListNode *b)
    { return a->val > b->val; };

    // Min heap to track the smallest element from each list
    std::priority_queue<ListNode *, std::vector<ListNode *>, decltype(comp)> minHeap(comp);

    // Add the first node from each list to the heap
    for (ListNode *list : lists)
    {
        if (list)
        {
            minHeap.push(list);
        }
    }

    // Dummy head for the result list
    ListNode dummy(0);
    ListNode *tail = &dummy;

    // Process nodes until the heap is empty
    while (!minHeap.empty())
    {
        // Get the smallest node
        ListNode *node = minHeap.top();
        minHeap.pop();

        // Add to result list
        tail->next = node;
        tail = tail->next;

        // Add the next node from the same list to the heap
        if (node->next)
        {
            minHeap.push(node->next);
        }
    }

    return dummy.next;
}

/**
 * @brief Create a linked list from a vector
 *
 * @param values Vector of values
 * @return Head of created list
 */
ListNode *createList(const std::vector<int> &values)
{
    ListNode dummy(0);
    ListNode *tail = &dummy;

    for (int val : values)
    {
        tail->next = new ListNode(val);
        tail = tail->next;
    }

    return dummy.next;
}

/**
 * @brief Print a linked list
 *
 * @param head Head of the list
 */
void printList(ListNode *head)
{
    ListNode *curr = head;
    while (curr)
    {
        std::cout << curr->val;
        if (curr->next)
        {
            std::cout << " -> ";
        }
        curr = curr->next;
    }
    std::cout << std::endl;
}

/**
 * @brief Delete a linked list to free memory
 *
 * @param head Head of the list
 */
void deleteList(ListNode *head)
{
    while (head)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

/**
 * @brief Alternative implementation to merge k sorted arrays (not lists)
 *
 * @param arrays Vector of sorted arrays
 * @return Merged sorted array
 */
std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>> &arrays)
{
    // Custom structure to track elements in the heap
    struct HeapNode
    {
        int value;        // The value
        int arrayIndex;   // Which array it comes from
        int elementIndex; // Position in that array

        // Comparator for the min heap
        bool operator>(const HeapNode &other) const
        {
            return value > other.value;
        }
    };

    // Min heap to track smallest elements
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<>> minHeap;

    // Add the first element from each array
    for (int i = 0; i < arrays.size(); i++)
    {
        if (!arrays[i].empty())
        {
            minHeap.push({arrays[i][0], i, 0});
        }
    }

    std::vector<int> result;

    // Process elements until heap is empty
    while (!minHeap.empty())
    {
        // Get the smallest element
        HeapNode node = minHeap.top();
        minHeap.pop();

        // Add to result
        result.push_back(node.value);

        // Add the next element from the same array (if any)
        if (node.elementIndex + 1 < arrays[node.arrayIndex].size())
        {
            minHeap.push({arrays[node.arrayIndex][node.elementIndex + 1],
                          node.arrayIndex,
                          node.elementIndex + 1});
        }
    }

    return result;
}

/****************************************************************************************
 * K CLOSEST POINTS IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief 2D point with x,y coordinates
 */
struct Point
{
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}

    // Distance from origin (squared to avoid square root)
    int distanceSquared() const
    {
        return x * x + y * y;
    }
};

/**
 * @brief Find k closest points to the origin
 *
 * @param points Vector of points
 * @param k Number of closest points to find
 * @return Vector of k closest points
 */
std::vector<Point> kClosestPoints(const std::vector<Point> &points, int k)
{
    // Max heap to keep the k closest points
    auto comp = [](const Point &a, const Point &b)
    {
        return a.distanceSquared() < b.distanceSquared();
    };

    std::priority_queue<Point, std::vector<Point>, decltype(comp)> maxHeap(comp);

    // Process all points
    for (const Point &point : points)
    {
        maxHeap.push(point);
        if (maxHeap.size() > k)
        {
            maxHeap.pop();
        }
    }

    // Extract results
    std::vector<Point> result;
    while (!maxHeap.empty())
    {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    std::reverse(result.begin(), result.end()); // To return in ascending order of distance
    return result;
}

/**
 * @brief Overload to work with vector representation of points
 *
 * @param points Vector of points represented as [x, y]
 * @param k Number of closest points to find
 * @return Vector of k closest points in the same format
 */
std::vector<std::vector<int>> kClosestPoints(const std::vector<std::vector<int>> &points, int k)
{
    // Convert to Point structs
    std::vector<Point> pointStructs;
    for (const auto &p : points)
    {
        pointStructs.emplace_back(p[0], p[1]);
    }

    // Use the other function
    std::vector<Point> closestPointStructs = kClosestPoints(pointStructs, k);

    // Convert back to vector representation
    std::vector<std::vector<int>> result;
    for (const Point &p : closestPointStructs)
    {
        result.push_back({p.x, p.y});
    }

    return result;
}

/****************************************************************************************
 * UTILITY FUNCTIONS & BENCHMARKING
 ****************************************************************************************/

/**
 * @brief Generate a random vector of integers
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
 * @brief Generate a random sorted vector of integers
 *
 * @param size Size of the vector
 * @param min Minimum value
 * @param max Maximum value
 * @return Random sorted vector
 */
std::vector<int> generateRandomSortedVector(int size, int min = 1, int max = 1000)
{
    std::vector<int> vec = generateRandomVector(size, min, max);
    std::sort(vec.begin(), vec.end());
    return vec;
}

/**
 * @brief Generate a vector of random 2D points
 *
 * @param size Number of points
 * @param min Minimum coordinate value
 * @param max Maximum coordinate value
 * @return Vector of points
 */
std::vector<Point> generateRandomPoints(int size, int min = -100, int max = 100)
{
    std::vector<Point> points;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    for (int i = 0; i < size; i++)
    {
        points.emplace_back(dist(gen), dist(gen));
    }

    return points;
}

/**
 * @brief Print a vector
 *
 * @param vec Vector to print
 * @param label Optional label
 * @param limit Maximum elements to print (0 for all)
 */
template <typename T>
void printVector(const std::vector<T> &vec, const std::string &label = "", int limit = 0)
{
    if (!label.empty())
    {
        std::cout << label << ": ";
    }

    std::cout << "[";

    int count = 0;
    for (const T &val : vec)
    {
        if (limit > 0 && count >= limit)
        {
            std::cout << "...";
            break;
        }

        std::cout << val;

        count++;
        if (count < vec.size() && (limit == 0 || count < limit))
        {
            std::cout << ", ";
        }
    }

    std::cout << "]" << std::endl;
}

/**
 * @brief Print a Point object
 *
 * @param p Point to print
 */
std::ostream &operator<<(std::ostream &os, const Point &p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

/**
 * @brief Benchmark different heap sort implementations
 *
 * @param sizes Vector of array sizes to test
 * @param iterations Number of iterations for each size
 */
void benchmarkHeapSort(const std::vector<int> &sizes, int iterations = 5)
{
    std::cout << "===== Heap Sort Benchmarks =====" << std::endl;
    std::cout << std::setw(10) << "Size"
              << std::setw(15) << "Recursive (ms)"
              << std::setw(15) << "Iterative (ms)"
              << std::setw(15) << "STL (ms)"
              << std::endl;

    for (int size : sizes)
    {
        double totalTimeRecursive = 0;
        double totalTimeIterative = 0;
        double totalTimeSTL = 0;

        for (int i = 0; i < iterations; i++)
        {
            auto data = generateRandomVector(size);

            // Recursive heap sort
            auto dataRecursive = data;
            auto startRecursive = std::chrono::high_resolution_clock::now();
            heapSort(dataRecursive);
            auto endRecursive = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationRecursive = endRecursive - startRecursive;
            totalTimeRecursive += durationRecursive.count();

            // Iterative heap sort
            auto dataIterative = data;
            auto startIterative = std::chrono::high_resolution_clock::now();
            heapSortIterative(dataIterative);
            auto endIterative = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationIterative = endIterative - startIterative;
            totalTimeIterative += durationIterative.count();

            // STL heap sort
            auto dataSTL = data;
            auto startSTL = std::chrono::high_resolution_clock::now();
            heapSortSTL(dataSTL);
            auto endSTL = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationSTL = endSTL - startSTL;
            totalTimeSTL += durationSTL.count();
        }

        std::cout << std::setw(10) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeRecursive / iterations)
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeIterative / iterations)
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeSTL / iterations)
                  << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark k-th element finding approaches
 *
 * @param sizes Vector of array sizes to test
 * @param iterations Number of iterations for each size
 */
void benchmarkKthElement(const std::vector<int> &sizes, int iterations = 5)
{
    std::cout << "===== K-th Element Finding Benchmarks =====" << std::endl;
    std::cout << std::setw(10) << "Size"
              << std::setw(15) << "Heap (ms)"
              << std::setw(15) << "QuickSelect (ms)"
              << std::endl;

    for (int size : sizes)
    {
        double totalTimeHeap = 0;
        double totalTimeQuickSelect = 0;

        for (int i = 0; i < iterations; i++)
        {
            auto data = generateRandomVector(size);
            int k = size / 2; // Find the median

            // Heap approach
            auto startHeap = std::chrono::high_resolution_clock::now();
            auto resultHeap = findKthLargest(data, k);
            auto endHeap = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationHeap = endHeap - startHeap;
            totalTimeHeap += durationHeap.count();

            // QuickSelect approach
            auto dataQuickSelect = data; // Need to copy as it modifies the array
            auto startQuickSelect = std::chrono::high_resolution_clock::now();
            auto resultQuickSelect = findKthLargestQuickSelect(dataQuickSelect, k);
            auto endQuickSelect = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationQuickSelect = endQuickSelect - startQuickSelect;
            totalTimeQuickSelect += durationQuickSelect.count();
        }

        std::cout << std::setw(10) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeHeap / iterations)
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeQuickSelect / iterations)
                  << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Benchmark top-k frequent elements approaches
 *
 * @param sizes Vector of array sizes to test
 * @param iterations Number of iterations for each size
 */
void benchmarkTopKFrequent(const std::vector<int> &sizes, int iterations = 5)
{
    std::cout << "===== Top-K Frequent Elements Benchmarks =====" << std::endl;
    std::cout << std::setw(10) << "Size"
              << std::setw(15) << "Heap (ms)"
              << std::setw(15) << "Bucket (ms)"
              << std::endl;

    for (int size : sizes)
    {
        double totalTimeHeap = 0;
        double totalTimeBucket = 0;

        for (int i = 0; i < iterations; i++)
        {
            // Create data with some repeated elements
            auto data = generateRandomVector(size, 1, size / 10);
            int k = 5; // Find top 5 frequent elements

            // Heap approach
            auto startHeap = std::chrono::high_resolution_clock::now();
            auto resultHeap = topKFrequent(data, k);
            auto endHeap = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationHeap = endHeap - startHeap;
            totalTimeHeap += durationHeap.count();

            // Bucket approach
            auto startBucket = std::chrono::high_resolution_clock::now();
            auto resultBucket = topKFrequentBucket(data, k);
            auto endBucket = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> durationBucket = endBucket - startBucket;
            totalTimeBucket += durationBucket.count();
        }

        std::cout << std::setw(10) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeHeap / iterations)
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTimeBucket / iterations)
                  << std::endl;
    }

    std::cout << std::endl;
}

/****************************************************************************************
 * DEMO FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Demonstrate heap sort
 */
void demoHeapSort()
{
    std::cout << "===== Heap Sort Demonstration =====" << std::endl;

    // Create random array
    std::vector<int> arr = generateRandomVector(15, 1, 100);
    printVector(arr, "Original array");

    // Sort using heap sort
    std::vector<int> sortedArr = arr;
    heapSort(sortedArr);
    printVector(sortedArr, "Sorted array");

    // Verify against STL sort
    std::vector<int> stlSortedArr = arr;
    std::sort(stlSortedArr.begin(), stlSortedArr.end());

    bool sortCorrect = (sortedArr == stlSortedArr);
    std::cout << "Sort correctness: " << (sortCorrect ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Demonstrate finding k-th largest/smallest element
 */
void demoKthElement()
{
    std::cout << "===== K-th Element Finding Demonstration =====" << std::endl;

    // Create random array
    std::vector<int> arr = generateRandomVector(15, 1, 100);
    printVector(arr, "Original array");

    // Find 3rd largest element
    int k = 3;
    int kthLargest = findKthLargest(arr, k);
    std::cout << k << "rd largest element: " << kthLargest << std::endl;

    // Find 3rd smallest element
    int kthSmallest = findKthSmallest(arr, k);
    std::cout << k << "rd smallest element: " << kthSmallest << std::endl;

    // Verify against sorted array
    std::vector<int> sortedArr = arr;
    std::sort(sortedArr.begin(), sortedArr.end());

    bool largestCorrect = (kthLargest == sortedArr[sortedArr.size() - k]);
    bool smallestCorrect = (kthSmallest == sortedArr[k - 1]);

    std::cout << "K-th largest correctness: " << (largestCorrect ? "PASS" : "FAIL") << std::endl;
    std::cout << "K-th smallest correctness: " << (smallestCorrect ? "PASS" : "FAIL") << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Demonstrate finding top-k frequent elements
 */
void demoTopKFrequent()
{
    std::cout << "===== Top-K Frequent Elements Demonstration =====" << std::endl;

    // Create array with repeating elements
    std::vector<int> arr = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 7, 8, 9, 9, 9, 9};
    printVector(arr, "Original array");

    // Count frequencies manually for verification
    std::unordered_map<int, int> freq;
    for (int num : arr)
    {
        freq[num]++;
    }

    std::cout << "Frequencies:" << std::endl;
    for (const auto &[num, count] : freq)
    {
        std::cout << "  " << num << ": " << count << std::endl;
    }

    // Find top 3 frequent elements
    int k = 3;
    std::vector<int> topK = topKFrequent(arr, k);

    std::cout << "Top " << k << " frequent elements: ";
    for (int i = 0; i < topK.size(); i++)
    {
        std::cout << topK[i];
        if (i < topK.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Using bucket approach
    std::vector<int> topKBucket = topKFrequentBucket(arr, k);

    std::cout << "Top " << k << " frequent elements (bucket): ";
    for (int i = 0; i < topKBucket.size(); i++)
    {
        std::cout << topKBucket[i];
        if (i < topKBucket.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Demonstrate median finding
 */
void demoMedianFinder()
{
    std::cout << "===== Median Finding Demonstration =====" << std::endl;

    MedianFinder medianFinder;

    // Add numbers one by one
    std::vector<int> nums = {41, 35, 62, 5, 97, 108};

    for (int num : nums)
    {
        medianFinder.addNum(num);

        std::cout << "After adding " << num << ":" << std::endl;

        // Print current lower half (max heap)
        std::vector<int> lowerHalf = medianFinder.getLowerHalf();
        std::cout << "  Lower half (max heap): ";
        for (int i = 0; i < lowerHalf.size(); i++)
        {
            std::cout << lowerHalf[i];
            if (i < lowerHalf.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;

        // Print current upper half (min heap)
        std::vector<int> upperHalf = medianFinder.getUpperHalf();
        std::cout << "  Upper half (min heap): ";
        for (int i = 0; i < upperHalf.size(); i++)
        {
            std::cout << upperHalf[i];
            if (i < upperHalf.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;

        // Print current median
        std::cout << "  Current median: " << medianFinder.findMedian() << std::endl;
        std::cout << std::endl;
    }

    // Verify against sorted array
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end());

    double expectedMedian;
    if (sortedNums.size() % 2 == 0)
    {
        // Even number of elements
        expectedMedian = (sortedNums[sortedNums.size() / 2 - 1] + sortedNums[sortedNums.size() / 2]) / 2.0;
    }
    else
    {
        // Odd number of elements
        expectedMedian = sortedNums[sortedNums.size() / 2];
    }

    std::cout << "Final median: " << medianFinder.findMedian() << std::endl;
    std::cout << "Expected median: " << expectedMedian << std::endl;
    std::cout << "Correctness: " << (medianFinder.findMedian() == expectedMedian ? "PASS" : "FAIL") << std::endl;

    // Demonstrate sliding window median
    std::cout << "\nSliding Window Median (window size 3):" << std::endl;
    std::vector<double> slidingMedians = medianSlidingWindow(nums, 3);

    std::cout << "Numbers: ";
    for (int i = 0; i < nums.size(); i++)
    {
        std::cout << nums[i];
        if (i < nums.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Sliding Medians: ";
    for (int i = 0; i < slidingMedians.size(); i++)
    {
        std::cout << slidingMedians[i];
        if (i < slidingMedians.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Demonstrate merging k sorted lists
 */
void demoMergeKLists()
{
    std::cout << "===== Merge K Sorted Lists Demonstration =====" << std::endl;

    // Create sorted lists
    std::vector<std::vector<int>> listValues = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}};

    std::vector<ListNode *> lists;
    for (const auto &values : listValues)
    {
        lists.push_back(createList(values));
    }

    // Print input lists
    std::cout << "Input Lists:" << std::endl;
    for (int i = 0; i < lists.size(); i++)
    {
        std::cout << "List " << (i + 1) << ": ";
        printList(lists[i]);
    }

    // Merge lists
    ListNode *mergedList = mergeKLists(lists);

    // Print merged list
    std::cout << "Merged List: ";
    printList(mergedList);

    // Cleanup
    for (ListNode *list : lists)
    {
        deleteList(list);
    }
    deleteList(mergedList);

    // Also demonstrate merging k sorted arrays
    std::cout << "\nMerge K Sorted Arrays:" << std::endl;
    std::vector<int> mergedArray = mergeKSortedArrays(listValues);

    std::cout << "Merged Array: ";
    for (int i = 0; i < mergedArray.size(); i++)
    {
        std::cout << mergedArray[i];
        if (i < mergedArray.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Demonstrate finding k closest points
 */
void demoKClosestPoints()
{
    std::cout << "===== K Closest Points Demonstration =====" << std::endl;

    // Create random points
    std::vector<Point> points = {
        {1, 3}, {-2, 2}, {5, 8}, {0, 1}, {-1, -1}, {3, -3}, {-2, -4}, {1, 1}, {3, 4}, {-4, 4}};

    // Print all points with distances
    std::cout << "All Points (with distances to origin):" << std::endl;
    for (const Point &p : points)
    {
        std::cout << p << " - Distance: " << std::sqrt(p.distanceSquared()) << std::endl;
    }

    // Find 3 closest points
    int k = 3;
    std::vector<Point> closestPoints = kClosestPoints(points, k);

    // Print k closest points
    std::cout << "\nThe " << k << " closest points to origin:" << std::endl;
    for (const Point &p : closestPoints)
    {
        std::cout << p << " - Distance: " << std::sqrt(p.distanceSquared()) << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Run all benchmarks
 */
void runBenchmarks()
{
    std::vector<int> sizes = {1000, 10000, 100000};

    benchmarkHeapSort(sizes);
    benchmarkKthElement(sizes);
    benchmarkTopKFrequent(sizes);
}

/**
 * @brief Main function with demonstrations of heap applications
 */
int main()
{
    std::cout << "===== DAY 26: Heap Sort & Heap Applications =====" << std::endl;
    std::cout << "Part of #DSAin45 Course" << std::endl
              << std::endl;

    // Run demonstrations
    demoHeapSort();
    demoKthElement();
    demoTopKFrequent();
    demoMedianFinder();
    demoMergeKLists();
    demoKClosestPoints();

    // Run benchmarks (comment out if not needed)
    // runBenchmarks();

    std::cout << "===== End of DAY 26 Demonstrations =====" << std::endl;

    return 0;
}