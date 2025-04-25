# 🔍 DAY 26: Heap Sort & Heap Applications - Advanced Priority Management #DSAin45

Welcome to Day 26 of our 45-day DSA journey! Yesterday, we explored the heap data structure and its fundamental operations. Today, we'll dive deeper into heap-based algorithms, with a focus on Heap Sort and powerful real-world applications of heaps.

_Imagine having the power to efficiently organize massive datasets, quickly identify the most important elements, or maintain running statistics in real-time as data flows in. That's the magic of heap-based algorithms: they leverage the elegant heap property to solve complex problems with remarkable efficiency._

## 📌 Heap Sort: Turning Heaps into Sorting Powerhouses

Heap Sort is an efficient comparison-based sorting algorithm that uses the heap data structure to sort elements. It works by first transforming the input array into a max heap (for ascending order) or min heap (for descending order), then repeatedly extracting the root element to build the sorted array.

### Key Advantages of Heap Sort:

1. **Consistent Performance**: O(n log n) time complexity in all cases (best, average, worst)
2. **In-Place Sorting**: Requires only O(1) auxiliary space
3. **No "Worst Case" Scenarios**: Unlike Quick Sort, which can degrade to O(n²)
4. **Efficient for Large Datasets**: Particularly when memory is a constraint

### The Heap Sort Algorithm

Here's a step-by-step breakdown of how Heap Sort works:

#### Step 1: Build a max heap from the input array

This transforms the unordered array into a heap structure, ensuring the max-heap property.

#### Step 2: Repeatedly extract the maximum element

- Swap the root (maximum element) with the last unsorted element
- Reduce the heap size by 1
- Heapify the root to maintain the heap property
- Repeat until all elements are sorted

### Heap Sort Implementation in C++

```cpp
void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Build a max heap (heapify all non-leaf nodes)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root (maximum) to end
        std::swap(arr[0], arr[i]);

        // Heapify the reduced heap
        heapify(arr, i, 0);
    }
}

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;        // Initialize largest as root
    int left = 2 * i + 1;   // Left child
    int right = 2 * i + 2;  // Right child

    // Check if left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // Check if right child is larger than current largest
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not the root, swap and continue heapifying
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
```

### 🔄 Heap Sort Analysis

Let's analyze the time and space complexity of Heap Sort:

| Phase            | Time Complexity | Explanation                                           |
| ---------------- | --------------- | ----------------------------------------------------- |
| Build Heap       | O(n)            | Building a heap from an array is O(n), not O(n log n) |
| Extract Elements | O(n log n)      | n elements, each requiring O(log n) time to heapify   |
| Overall          | O(n log n)      | Dominated by the extraction phase                     |

**Space Complexity**: O(1) - Heap Sort sorts in-place, using only a constant amount of extra space.

**Is Heap Sort Stable?** No, Heap Sort is not stable. Elements with equal keys may change their relative order during the sorting process.

### 🔄 Heap Sort vs. Other Sorting Algorithms

| Algorithm    | Best Case  | Average Case | Worst Case | Space    | Stable | Notes                           |
| ------------ | ---------- | ------------ | ---------- | -------- | ------ | ------------------------------- |
| Heap Sort    | O(n log n) | O(n log n)   | O(n log n) | O(1)     | No     | Consistent performance          |
| Quick Sort   | O(n log n) | O(n log n)   | O(n²)      | O(log n) | No     | Usually fastest in practice     |
| Merge Sort   | O(n log n) | O(n log n)   | O(n log n) | O(n)     | Yes    | Stable with guaranteed perf.    |
| Insertion S. | O(n)       | O(n²)        | O(n²)      | O(1)     | Yes    | Good for small or nearly sorted |
| Bubble Sort  | O(n)       | O(n²)        | O(n²)      | O(1)     | Yes    | Simple but inefficient          |

Heap Sort shines when:

- You need guaranteed O(n log n) performance
- Memory usage is a concern (advantage over Merge Sort)
- Stability is not required
- You want to avoid Quick Sort's worst-case behavior

## 📊 Powerful Heap Applications

Beyond sorting, heaps are the backbone of many elegant algorithmic solutions. Let's explore some powerful applications:

### 1. Finding the K-th Largest/Smallest Element

#### Problem:

Given an unsorted array of n elements, find the k-th largest (or smallest) element.

#### Naive Approach:

Sort the array (O(n log n)) and return the element at index n-k (for k-th largest).

#### Heap-Based Approach:

Use a min-heap of size k to find the k-th largest element:

```cpp
int findKthLargest(std::vector<int>& nums, int k) {
    // Use a min heap to keep track of the k largest elements
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int num : nums) {
        // Add element to the heap
        minHeap.push(num);

        // If heap size exceeds k, remove the smallest element
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    // The top element is the k-th largest
    return minHeap.top();
}
```

**Time Complexity**: O(n log k) - Much better than O(n log n) when k << n
**Space Complexity**: O(k)

### 2. Top K Frequent Elements

#### Problem:

Given an array of integers, find the k most frequent elements.

#### Solution:

Use a hash map to count frequencies, then use a min-heap to track the k most frequent elements:

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

**Time Complexity**: O(n log k)
**Space Complexity**: O(n + k)

### 3. Median in a Data Stream

#### Problem:

Design a data structure that supports:

- Adding integers to a data stream
- Finding the median of all elements seen so far

#### Solution:

Use two heaps - a max heap for the lower half and a min heap for the upper half:

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

This elegant solution maintains the following invariants:

1. Elements in maxHeap ≤ elements in minHeap
2. The difference in size between the heaps is at most 1
3. The median is either the top of maxHeap (if sizes differ) or the average of both tops (if sizes are equal)

**Time Complexity**: O(log n) per insertion, O(1) to find median
**Space Complexity**: O(n)

### 4. Merge K Sorted Lists

#### Problem:

Merge k sorted linked lists into one sorted list.

#### Solution:

Use a min heap to keep track of the current smallest element from each list:

```cpp
ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    // Compare function for the min heap
    auto comp = [](ListNode* a, ListNode* b) { return a->val > b->val; };

    // Min heap to track the smallest element from each list
    std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(comp)> minHeap(comp);

    // Add the first node from each list to the heap
    for (ListNode* list : lists) {
        if (list) {
            minHeap.push(list);
        }
    }

    // Dummy head for the result list
    ListNode dummy(0);
    ListNode* tail = &dummy;

    // Process nodes until the heap is empty
    while (!minHeap.empty()) {
        // Get the smallest node
        ListNode* node = minHeap.top();
        minHeap.pop();

        // Add to result list
        tail->next = node;
        tail = tail->next;

        // Add the next node from the same list to the heap
        if (node->next) {
            minHeap.push(node->next);
        }
    }

    return dummy.next;
}
```

**Time Complexity**: O(N log k), where N is the total number of nodes and k is the number of lists
**Space Complexity**: O(k)

### 5. K Closest Points to Origin

#### Problem:

Given an array of points in a 2D plane, find the k closest points to the origin (0, 0).

#### Solution:

Use a max heap to keep track of the k closest points:

```cpp
std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
    // Compare function based on distance from origin
    auto comp = [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
    };

    // Max heap to track k closest points
    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, decltype(comp)> maxHeap(comp);

    for (const auto& point : points) {
        maxHeap.push(point);
        if (maxHeap.size() > k) {
            maxHeap.pop();
        }
    }

    // Extract results
    std::vector<std::vector<int>> result;
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    return result;
}
```

**Time Complexity**: O(n log k)
**Space Complexity**: O(k)

## 🧠 The Two-Heap Pattern: Balancing Data for Efficiency

One of the most elegant heap application patterns is the "two heap" approach we saw in the median finder. This pattern involves:

1. **Divide and balance**: Split data into two parts with a specific property
2. **Heap selection**: Use appropriate heap types for each part (max/min)
3. **Maintain invariants**: Keep balance and order constraints during operations

This pattern works brilliantly for:

- **Median maintenance**: As we've seen
- **Sliding window problems**: Where elements enter and leave a range
- **Stream processing**: When data arrives continuously
- **Partial sorting**: When only a subset of ordering matters

### Example: Sliding Window Median

```cpp
std::vector<double> medianSlidingWindow(std::vector<int>& nums, int k) {
    std::multiset<int> window(nums.begin(), nums.begin() + k);
    auto mid = std::next(window.begin(), k / 2);
    std::vector<double> medians;

    for (int i = k; i <= nums.size(); i++) {
        // Calculate median from current window
        medians.push_back(k % 2 == 0 ? (*mid + *std::prev(mid)) / 2.0 : *mid);

        if (i == nums.size()) break;

        // Add the incoming element
        window.insert(nums[i]);

        // Update the median iterator
        if (nums[i] < *mid) mid--;

        // Remove the outgoing element
        if (nums[i-k] <= *mid) mid++;
        window.erase(window.find(nums[i-k]));
    }

    return medians;
}
```

## 💻 Using C++ STL for Heap Operations

C++ provides efficient heap operations through the Standard Template Library:

### 1. Priority Queue

```cpp
// Max priority queue (default)
std::priority_queue<int> maxPQ;

// Min priority queue using comparator
std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;

// Custom comparator for complex objects
auto comp = [](const MyClass& a, const MyClass& b) { return a.value > b.value; };
std::priority_queue<MyClass, std::vector<MyClass>, decltype(comp)> customPQ(comp);
```

### 2. Heap Algorithms

```cpp
std::vector<int> v = {1, 3, 5, 7, 9, 2, 4, 6, 8, 0};

// Turn vector into a heap
std::make_heap(v.begin(), v.end());  // Default: max-heap

// Add element to heap
v.push_back(10);
std::push_heap(v.begin(), v.end());

// Remove largest element
std::pop_heap(v.begin(), v.end());
int largest = v.back();
v.pop_back();

// Sort heap
std::sort_heap(v.begin(), v.end());  // Now v is sorted
```

## 📝 Heap Sort Implementation Strategies

### 1. Classic Recursive Implementation

This is the standard implementation we've already seen, using recursive heapify.

### 2. Iterative Implementation

An iterative version can be more efficient in practice by avoiding function call overhead:

```cpp
void heapifyIterative(std::vector<int>& arr, int n, int i) {
    int largest = i;

    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        largest = i;

        if (left < n && arr[left] > arr[largest])
            largest = left;

        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest == i)
            break;

        std::swap(arr[i], arr[largest]);
        i = largest;
    }
}
```

### 3. Bottom-Up Heap Construction

A more efficient way to build a heap from scratch:

```cpp
void buildHeapBottomUp(std::vector<int>& arr) {
    int n = arr.size();

    // Start from the last non-leaf node and heapify down
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}
```

This approach uses O(n) time rather than the naive O(n log n) because most heapify operations are performed on nodes near the bottom of the tree, which require fewer swaps.

## 😄 Heap Jokes and Analogies

1. **The Olympic Podium Analogy**:
   "If sorting algorithms were Olympic events, Heap Sort would be the consistent bronze medalist. It doesn't have Quick Sort's blazing speed or Merge Sort's elegant stability, but it never has a bad day and never runs out of memory during the ceremony!"

2. **The Restaurant Analogy**:
   "Using a heap to find the median is like running a perfectly balanced restaurant with two waiting areas. You maintain a 'not-so-hungry' area (max heap) and a 'very-hungry' area (min heap), making sure they're balanced in size. When someone asks for the median hunger level, you just check the hungriest person in the 'not-so-hungry' area. That's O(1) service!"

3. **The Gardening Joke**:
   "I tried to explain heapsort to my gardener. I told him to arrange the plants by height, but he kept insisting the parent plants should always be taller than their children. I had to explain that's exactly what a max heap is. He replied, 'So just like my family tree then!'"

4. **The Traffic Management Analogy**:
   "A heap is like a well-designed highway system. The largest elements always have an express lane straight to the top, while smaller ones may need to take a few exits and detours along the way. And the beauty is, no matter how busy the highway gets, the travel time only increases logarithmically!"

5. **The Dating App Joke**:
   "Heap Sort is like a dating app that always shows you the most compatible match first. It doesn't care how many people are in the system; it just efficiently reorganizes them after each match. Unfortunately, like the algorithm, your matches might not stay in stable order!"

6. **The Line-Cutting Analogy**:
   "Finding the top-K elements with a heap is like managing a VIP section with limited space. When a new VIP arrives, they're allowed in, but if the section is full, the least important VIP gets kicked out. The bouncer (heap) makes this decision in logarithmic time, no matter how many celebrities show up!"

## 🧩 LeetCode Practice Problems

### Easy:

#### 1. Last Stone Weight (LeetCode #1046)

**Problem:** You have a collection of stones, each with a positive integer weight. Each turn, we choose the two heaviest stones and smash them together. The result is either both stones are destroyed, or the heavier stone remains with reduced weight.

**Solution:**

```cpp
int lastStoneWeight(std::vector<int>& stones) {
    // Max heap to always get the two heaviest stones
    std::priority_queue<int> maxHeap(stones.begin(), stones.end());

    while (maxHeap.size() > 1) {
        int stone1 = maxHeap.top(); maxHeap.pop();
        int stone2 = maxHeap.top(); maxHeap.pop();

        if (stone1 != stone2) {
            maxHeap.push(stone1 - stone2);
        }
    }

    return maxHeap.empty() ? 0 : maxHeap.top();
}
```

#### 2. Relative Ranks (LeetCode #506)

**Problem:** Given an array of scores in competitions, return the relative ranks as strings ("Gold Medal", "Silver Medal", "Bronze Medal", or "4", "5", etc.)

**Solution:**

```cpp
std::vector<std::string> findRelativeRanks(std::vector<int>& score) {
    int n = score.size();
    std::vector<std::string> result(n);

    // Pair scores with their original indices
    std::vector<std::pair<int, int>> pairs;
    for (int i = 0; i < n; i++) {
        pairs.push_back({score[i], i});
    }

    // Sort in descending order
    std::sort(pairs.rbegin(), pairs.rend());

    // Assign ranks
    for (int i = 0; i < n; i++) {
        int originalIndex = pairs[i].second;

        if (i == 0) {
            result[originalIndex] = "Gold Medal";
        } else if (i == 1) {
            result[originalIndex] = "Silver Medal";
        } else if (i == 2) {
            result[originalIndex] = "Bronze Medal";
        } else {
            result[originalIndex] = std::to_string(i + 1);
        }
    }

    return result;
}
```

### Medium:

#### 1. K Closest Points to Origin (LeetCode #973)

**Problem:** Find the k closest points to the origin (0, 0) from a list of points.

**Solution:** Already discussed in the applications section.

#### 2. Sort Characters By Frequency (LeetCode #451)

**Problem:** Sort a string by decreasing frequency of characters.

**Solution:**

```cpp
std::string frequencySort(std::string s) {
    // Count frequencies
    std::unordered_map<char, int> freq;
    for (char c : s) {
        freq[c]++;
    }

    // Create max heap based on frequency
    std::priority_queue<std::pair<int, char>> maxHeap;
    for (auto [c, count] : freq) {
        maxHeap.push({count, c});
    }

    // Build result string
    std::string result;
    while (!maxHeap.empty()) {
        auto [count, c] = maxHeap.top();
        maxHeap.pop();
        result.append(count, c);
    }

    return result;
}
```

#### 3. Task Scheduler (LeetCode #621)

**Problem:** Given a list of tasks and a cooldown period n, find the least time needed to complete all tasks.

**Solution:**

```cpp
int leastInterval(std::vector<char>& tasks, int n) {
    // Count task frequencies
    std::unordered_map<char, int> freq;
    for (char task : tasks) {
        freq[task]++;
    }

    // Use max heap to process most frequent tasks first
    std::priority_queue<int> maxHeap;
    for (auto [task, count] : freq) {
        maxHeap.push(count);
    }

    int cycles = 0;
    while (!maxHeap.empty()) {
        // Tasks to process in this cycle
        std::vector<int> temp;
        for (int i = 0; i <= n; i++) {
            if (!maxHeap.empty()) {
                temp.push_back(maxHeap.top() - 1);
                maxHeap.pop();
            }
        }

        // Add tasks that still need processing back to the heap
        for (int count : temp) {
            if (count > 0) {
                maxHeap.push(count);
            }
        }

        // Calculate cycle time
        cycles += maxHeap.empty() ? temp.size() : n + 1;
    }

    return cycles;
}
```

### Hard:

#### 1. Find Median from Data Stream (LeetCode #295)

**Problem:** Design a data structure that supports adding integers and finding the median.

**Solution:** Already discussed in the applications section.

#### 2. Sliding Window Median (LeetCode #480)

**Problem:** Find the median in each sliding window of size k from an array.

**Solution:** We showed an implementation using multiset earlier. Here's a heap-based solution:

```cpp
std::vector<double> medianSlidingWindow(std::vector<int>& nums, int k) {
    std::priority_queue<int> maxHeap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    std::unordered_map<int, int> delayed; // For "lazy" removal

    std::vector<double> result;

    for (int i = 0; i < nums.size(); i++) {
        // Add new element
        if (maxHeap.empty() || nums[i] <= maxHeap.top()) {
            maxHeap.push(nums[i]);
        } else {
            minHeap.push(nums[i]);
        }

        // Remove element outside the window
        if (i >= k) {
            int removed = nums[i - k];
            delayed[removed]++;

            // Clean up the heaps if needed
            while (!maxHeap.empty() && delayed[maxHeap.top()] > 0) {
                delayed[maxHeap.top()]--;
                maxHeap.pop();
            }

            while (!minHeap.empty() && delayed[minHeap.top()] > 0) {
                delayed[minHeap.top()]--;
                minHeap.pop();
            }
        }

        // Balance heaps
        while (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }

        while (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }

        // Calculate median if we have a full window
        if (i >= k - 1) {
            if (k % 2 == 0) {
                result.push_back((double)maxHeap.top() * 0.5 + (double)minHeap.top() * 0.5);
            } else {
                result.push_back(maxHeap.top());
            }
        }
    }

    return result;
}
```

#### 3. Swim in Rising Water (LeetCode #778)

**Problem:** Find the least time to reach the bottom right of a grid where each cell has a height and the water level rises by 1 each time unit.

**Solution:**

```cpp
int swimInWater(std::vector<std::vector<int>>& grid) {
    int n = grid.size();

    // Min heap to store (time, row, col)
    std::priority_queue<std::tuple<int, int, int>,
                        std::vector<std::tuple<int, int, int>>,
                        std::greater<>> minHeap;

    // Visited cells
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));

    // Directions for movement
    std::vector<std::pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    minHeap.push({grid[0][0], 0, 0});
    visited[0][0] = true;

    while (!minHeap.empty()) {
        auto [time, row, col] = minHeap.top();
        minHeap.pop();

        // If we reached the destination
        if (row == n - 1 && col == n - 1) {
            return time;
        }

        // Try all four directions
        for (auto [dr, dc] : dirs) {
            int newRow = row + dr;
            int newCol = col + dc;

            if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < n &&
                !visited[newRow][newCol]) {

                visited[newRow][newCol] = true;
                int newTime = std::max(time, grid[newRow][newCol]);
                minHeap.push({newTime, newRow, newCol});
            }
        }
    }

    return -1; // Should never reach here
}
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll explore Trie Data Structure, a specialized tree-based data structure that's incredibly efficient for string operations. We'll learn:

- How tries efficiently store and search strings
- Implementing a trie from scratch in C++
- Fast prefix matching and autocomplete applications
- When to use tries instead of hash tables
- Optimizing tries for memory efficiency
- Real-world applications like auto-suggestion and spell checking

_If heaps are like priority management systems that always know what's most important, tries are like language experts that can complete your sentences faster than you can type them. By organizing letters in a tree where paths represent words, tries make searching for patterns, prefixes, and even entire dictionaries blazingly fast—capabilities we'll explore in depth tomorrow._

Stay tuned for our journey into the fascinating world of tries!

#DSAin45 #Day26 #HeapSort #Algorithms #CPlusPlus #DataStructures #TopKElements #MedianFinding #PriorityQueue #CodingInterview
