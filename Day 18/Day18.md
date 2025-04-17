# 🚀 DAY 18: Efficient Sorting Algorithms - Divide and Conquer #DSAin45

Welcome to Day 18 of our 45-day DSA journey! Yesterday, we explored basic sorting algorithms with O(n²) time complexity. Today, we're leveling up to more efficient sorting algorithms that achieve O(n log n) time complexity through clever divide-and-conquer strategies.

_If the basic sorting algorithms were like organizing a small bookshelf by hand, today's efficient algorithms are like coordinating a team of librarians to sort an entire library—more complex to orchestrate but dramatically faster for large collections._

## 🔍 The Need for Speed: Why O(n log n)?

Before diving into the algorithms, let's understand why O(n log n) is so important:

1. **Theoretical Lower Bound**: Under the comparison model, O(n log n) is the theoretical lower bound for sorting. You can't do better with a comparison-based sort.

2. **Practical Performance**: For large datasets, the difference between O(n²) and O(n log n) is enormous. For n = 1,000,000:

   - O(n²) = 1,000,000,000,000 operations
   - O(n log n) ≈ 20,000,000 operations (50,000 times faster!)

3. **Real-world Applications**: Most sorting in production systems uses these efficient algorithms, as they remain practical even as data scales.

## 🔄 Merge Sort: Divide, Sort, and Combine

Merge Sort epitomizes the divide-and-conquer paradigm with a simple but powerful approach:

1. **Divide**: Split the array into two halves
2. **Conquer**: Recursively sort both halves
3. **Combine**: Merge the sorted halves into a single sorted array

_Like a corporate merger where two organized companies combine operations by methodically integrating their sorted departments, one element at a time._

### Implementation

```cpp
void merge(std::vector<int>& arr, int left, int mid, int right) {
    // Sizes of the temporary arrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    std::vector<int> L(n1), R(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr
    int i = 0;    // Initial index of first subarray
    int j = 0;    // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        // Find the middle point
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Wrapper function
void mergeSort(std::vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}
```

### Step-by-Step Example

Let's trace Merge Sort on a small array: `[38, 27, 43, 3, 9, 82, 10]`

1. **Divide**: Split into `[38, 27, 43, 3]` and `[9, 82, 10]`

2. **Recursively sort first half**:

   - Split into `[38, 27]` and `[43, 3]`
   - Split `[38, 27]` into `[38]` and `[27]`
   - Merge `[38]` and `[27]` into `[27, 38]`
   - Split `[43, 3]` into `[43]` and `[3]`
   - Merge `[43]` and `[3]` into `[3, 43]`
   - Merge `[27, 38]` and `[3, 43]` into `[3, 27, 38, 43]`

3. **Recursively sort second half**:

   - Split into `[9, 82]` and `[10]`
   - Split `[9, 82]` into `[9]` and `[82]`
   - Merge `[9]` and `[82]` into `[9, 82]`
   - Merge `[9, 82]` and `[10]` into `[9, 10, 82]`

4. **Final merge**: Combine `[3, 27, 38, 43]` and `[9, 10, 82]` into `[3, 9, 10, 27, 38, 43, 82]`

### Analysis

- **Time Complexity**:
  - Best Case: O(n log n)
  - Average Case: O(n log n)
  - Worst Case: O(n log n)
- **Space Complexity**: O(n) - requires additional space for the temporary arrays
- **Stability**: Stable - equal elements maintain their relative order
- **Adaptivity**: Not adaptive - performance doesn't improve for partially sorted input

### Advantages and Disadvantages

**Advantages**:

- Guaranteed O(n log n) performance regardless of input
- Stable sorting algorithm
- Well-suited for linked lists with O(1) extra space
- Excellent for external sorting where data doesn't fit in memory

**Disadvantages**:

- Requires additional O(n) space
- Not adaptive to already-sorted data
- Has higher constant factors than Quick Sort in many implementations
- Not in-place, which can be a disadvantage for memory-constrained environments

## ⚡ Quick Sort: Pivoting to Success

Quick Sort is another divide-and-conquer algorithm with an elegantly simple concept but remarkable performance:

1. **Select**: Choose a 'pivot' element from the array
2. **Partition**: Rearrange the array so elements smaller than the pivot are on the left, and elements greater are on the right
3. **Recursively Sort**: Apply the algorithm to the sub-arrays on the left and right of the pivot

_Like organizing a classroom by selecting a height threshold (pivot), asking shorter students to move left and taller students to move right, then repeating the process in each group._

### Implementation

```cpp
int partition(std::vector<int>& arr, int low, int high) {
    // Select the rightmost element as pivot
    int pivot = arr[high];

    // Index of smaller element
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++; // Increment index of smaller element
            std::swap(arr[i], arr[j]);
        }
    }

    // Place pivot in its correct position
    std::swap(arr[i + 1], arr[high]);
    return i + 1; // Return the partitioning index
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partition the array and get pivot position
        int pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Wrapper function
void quickSort(std::vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}
```

### Step-by-Step Example

Let's trace Quick Sort on a small array: `[10, 7, 8, 9, 1, 5]`

1. **Choose pivot**: Pivot = 5 (last element)
2. **Partition**:
   - Elements <= 5: `[1]`
   - Elements > 5: `[10, 7, 8, 9]`
   - After partitioning: `[1, 5, 10, 7, 8, 9]` (pivot = 5 at index 1)
3. **Recursively sort left**: `[1]` (already sorted)
4. **Recursively sort right**: `[10, 7, 8, 9]`
   - Choose pivot: Pivot = 9
   - Partition: `[7, 8]` and `[10]`
   - After partitioning: `[7, 8, 9, 10]`
   - Recursively sort `[7, 8]`:
     - Choose pivot: Pivot = 8
     - Partition: `[7]` and `[]`
     - After partitioning: `[7, 8]`
5. **Final result**: `[1, 5, 7, 8, 9, 10]`

### Analysis

- **Time Complexity**:
  - Best Case: O(n log n) - when partitioning creates balanced splits
  - Average Case: O(n log n)
  - Worst Case: O(n²) - when partitioning creates highly unbalanced splits
- **Space Complexity**: O(log n) - for the recursive call stack
- **Stability**: Not stable - equal elements may change their relative order
- **Adaptivity**: Not inherently adaptive, but performs well on many types of input data

### Advantages and Disadvantages

**Advantages**:

- Often faster in practice than other O(n log n) algorithms due to cache efficiency
- In-place sorting with O(log n) extra space
- Works well with virtual memory systems
- Good locality of reference

**Disadvantages**:

- Worst case O(n²) time complexity
- Not stable
- Performance depends on pivot selection
- Vulnerable to stack overflow with deep recursion

### Quick Sort Optimizations

1. **Random Pivot Selection**: Reduces the chance of hitting the worst-case scenario

```cpp
int randomizedPartition(std::vector<int>& arr, int low, int high) {
    // Generate a random index between low and high
    int random = low + rand() % (high - low + 1);

    // Swap the random element with the high element
    std::swap(arr[random], arr[high]);

    // Call the standard partition method
    return partition(arr, low, high);
}
```

2. **Median-of-Three Pivot Selection**: Choose the median of the first, middle, and last elements as the pivot

```cpp
int medianOfThreePartition(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;

    // Sort low, mid, high
    if (arr[mid] < arr[low])
        std::swap(arr[mid], arr[low]);
    if (arr[high] < arr[low])
        std::swap(arr[high], arr[low]);
    if (arr[high] < arr[mid])
        std::swap(arr[high], arr[mid]);

    // Place pivot (median) at high-1
    std::swap(arr[mid], arr[high - 1]);

    // Use high-1 as pivot
    return partition(arr, low, high);
}
```

3. **Hybrid Approach with Insertion Sort**: Use QuickSort for large partitions and Insertion Sort for small ones

```cpp
void hybridQuickSort(std::vector<int>& arr, int low, int high) {
    while (low < high) {
        // Switch to insertion sort for small arrays
        if (high - low < 10) {
            insertionSort(arr, low, high);
            break;
        } else {
            int pi = partition(arr, low, high);

            // Recursively sort smaller partition
            // Tail call optimization: iterate on larger partition
            if (pi - low < high - pi) {
                hybridQuickSort(arr, low, pi - 1);
                low = pi + 1;
            } else {
                hybridQuickSort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}
```

## 🌳 Heap Sort: Leveraging Tree Structure

Heap Sort combines the tree structure of a binary heap with the in-place sorting approach:

1. **Build Heap**: Transform the input array into a max-heap
2. **Extract Max**: Swap the root (maximum element) with the last element and reduce heap size
3. **Restore Heap**: Heapify the root to maintain the max-heap property
4. **Repeat**: Steps 2-3 until the heap is empty

_Like organizing a sports tournament where the champion (maximum) is determined after each round, removed from competition, and the tournament is reorganized with the remaining participants._

### Implementation

```cpp
// To heapify a subtree rooted at node 'i'
void heapify(std::vector<int>& arr, int n, int i) {
    // Initialize largest as root
    int largest = i;
    int left = 2 * i + 1;  // Left child
    int right = 2 * i + 2; // Right child

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        std::swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        std::swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}
```

### Step-by-Step Example

Let's trace Heap Sort on a small array: `[4, 10, 3, 5, 1]`

1. **Build Max Heap**:

   - Initial array: `[4, 10, 3, 5, 1]`
   - After heapify: `[10, 5, 3, 4, 1]` (max heap)

2. **Extract the maximum element (10)**:

   - Swap `10` and `1`: `[1, 5, 3, 4, 10]`
   - Heapify the first 4 elements: `[5, 4, 3, 1]` and `[10]`

3. **Extract the maximum element (5)**:

   - Swap `5` and `1`: `[1, 4, 3, 5, 10]`
   - Heapify the first 3 elements: `[4, 1, 3]` and `[5, 10]`

4. **Extract the maximum element (4)**:

   - Swap `4` and `3`: `[3, 1, 4, 5, 10]`
   - Heapify the first 2 elements: `[3, 1]` and `[4, 5, 10]`

5. **Extract the maximum element (3)**:

   - Swap `3` and `1`: `[1, 3, 4, 5, 10]`

6. **Final sorted array**: `[1, 3, 4, 5, 10]`

### Analysis

- **Time Complexity**:
  - Best Case: O(n log n)
  - Average Case: O(n log n)
  - Worst Case: O(n log n)
- **Space Complexity**: O(1) - sorts in-place
- **Stability**: Not stable - equal elements may change their relative order
- **Adaptivity**: Not adaptive - performance doesn't improve for partially sorted input

### Advantages and Disadvantages

**Advantages**:

- Guaranteed O(n log n) performance
- In-place sorting with O(1) extra space
- Simple implementation
- Doesn't require the costly merge operation of Merge Sort
- Useful for implementing priority queues

**Disadvantages**:

- Not stable
- Usually slower than well-implemented Quick Sort
- Not cache-friendly due to poor locality
- Not adaptive to data characteristics

## 🔄 Tim Sort: The Best of Both Worlds

Tim Sort is a hybrid sorting algorithm derived from Merge Sort and Insertion Sort, designed to perform well on many real-world data sets. It was implemented by Tim Peters in 2002 for use in Python and is now used in programming languages like Java, Android, and many other standard libraries.

### Key Concepts:

1. **Run Identification**: Find naturally occurring runs (segments that are already sorted)
2. **Run Extension**: Extend runs using Insertion Sort for small segments
3. **Merge Strategy**: Merge runs using a modified Merge Sort approach with optimization techniques

_Like a skilled chef who uses different cooking techniques for different ingredients to create the perfect dish._

### Simplified Implementation Concept

```cpp
// This is a simplified conceptual version
void timSort(std::vector<int>& arr) {
    const int RUN = 32; // Size of sub-arrays to be sorted
    int n = arr.size();

    // Sort individual sub-arrays of size RUN
    for (int i = 0; i < n; i += RUN) {
        insertionSort(arr, i, std::min((i + RUN - 1), (n - 1)));
    }

    // Merge sub-arrays: start with size RUN, double size each pass
    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min((left + 2 * size - 1), (n - 1));

            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}
```

### Analysis

- **Time Complexity**:
  - Best Case: O(n) - when array is already sorted
  - Average Case: O(n log n)
  - Worst Case: O(n log n)
- **Space Complexity**: O(n) - for temporary arrays during merging
- **Stability**: Stable - equal elements maintain their relative order
- **Adaptivity**: Adaptive - performs better on partially sorted arrays

### Advantages

- Best real-world performance for a wide range of inputs
- Stable sorting algorithm
- Adaptive to real data patterns
- Minimizes comparisons by using galloping mode
- Used in production systems for many programming languages

## ⚔️ Comparing Efficient Sorting Algorithms

Let's compare our efficient sorting algorithms across multiple dimensions:

| Algorithm  | Best Case  | Average Case | Worst Case | Space    | Stable | In-Place | Notes                             |
| ---------- | ---------- | ------------ | ---------- | -------- | ------ | -------- | --------------------------------- |
| Merge Sort | O(n log n) | O(n log n)   | O(n log n) | O(n)     | Yes    | No       | Reliable, good for linked lists   |
| Quick Sort | O(n log n) | O(n log n)   | O(n²)      | O(log n) | No     | Yes      | Fast in practice, pivot-dependent |
| Heap Sort  | O(n log n) | O(n log n)   | O(n log n) | O(1)     | No     | Yes      | Consistent, poor cache locality   |
| Tim Sort   | O(n)       | O(n log n)   | O(n log n) | O(n)     | Yes    | No       | Hybrid, excellent on real data    |

### When to Use Each Algorithm

**Merge Sort**:

- When stability is required
- When guaranteed O(n log n) performance is needed
- When working with linked lists
- When external sorting is required

**Quick Sort**:

- When average-case performance is more important than worst-case
- When memory usage is a concern
- When in-place sorting is required
- When working with arrays that fit in memory

**Heap Sort**:

- When guaranteed worst-case performance is required
- When memory usage is a concern
- When implementing a priority queue
- When simplicity of implementation is valued

**Tim Sort**:

- When working with real-world data that might have some ordering
- When stability is required
- When adaptive behavior can provide benefits
- In general-purpose sorting libraries

## 🌎 Real-World Applications

These efficient sorting algorithms power countless systems we use daily:

1. **Merge Sort** is used in:

   - External sorting of large files
   - Sorting linked lists
   - Inversion count problems
   - Stable sorting implementations
   - Custom sort functions for objects with complex comparison logic

2. **Quick Sort** is used in:

   - C++'s `std::sort()` (often a hybrid version)
   - Implementations where average-case performance is critical
   - Systems with tight memory constraints
   - Numerical computation libraries

3. **Heap Sort** is used in:

   - Priority queues and scheduling algorithms
   - Finding the k largest/smallest elements
   - Graph algorithms like Dijkstra's and Prim's
   - Systems where worst-case guarantees are important

4. **Tim Sort** is used in:
   - Python's built-in `sort()` and `sorted()`
   - Java's `Arrays.sort()` and `Collections.sort()`
   - Android platform's sorting implementations
   - Modern browsers for JavaScript's `Array.sort()`

## 😄 Sorting Algorithm Jokes and Analogies

1. **The Merge Sort Wedding Planner**:
   "A Merge Sort wedding planner would have the bride and groom each sort their guest lists separately, then efficiently combine them without duplicates. And unlike Quick Sort, it guarantees no last-minute chaos!"

2. **The Quick Sort Chef**:
   "Quick Sort is like a chef who organizes ingredients by picking one item as a reference, then putting smaller items to the left and larger items to the right. Fast and efficient, but if you choose the wrong reference ingredient, dinner might take all night!"

3. **The Heap Sort Gym**:
   "Heap Sort is like a gym where the strongest person is always at the top of the leader board, but as soon as they win a competition, they're moved to the 'champions' section and the next strongest takes their place."

4. **The Sorting Algorithm Bar**:
   "At the Sorting Algorithm Bar, Merge Sort orders drinks carefully and methodically, Quick Sort serves them blazingly fast but occasionally drops a tray, Heap Sort maintains perfect order but seems robotic, and Tim Sort somehow anticipates what you want before you order it."

5. **The Quick Sort Pessimist**:
   "A Quick Sort pessimist always expects the pivot to be the smallest or largest element. An optimist believes the pivot will always be the median. A realist picks a random pivot and hopes for the best."

## 🧠 Advanced Topics and Optimizations

### Parallelizing Merge Sort

Merge Sort is naturally suited for parallelization:

```cpp
void parallelMergeSort(std::vector<int>& arr, int threads = 4) {
    // Create a thread pool
    std::vector<std::thread> threadPool;

    // Size of sub-arrays for each thread
    int segmentSize = arr.size() / threads;

    // Launch threads to sort segments in parallel
    for (int i = 0; i < threads; i++) {
        int left = i * segmentSize;
        int right = (i == threads - 1) ? arr.size() - 1 : (i + 1) * segmentSize - 1;

        threadPool.push_back(std::thread([&arr, left, right]() {
            mergeSort(arr, left, right);
        }));
    }

    // Wait for all threads to complete
    for (auto& thread : threadPool) {
        thread.join();
    }

    // Merge the sorted segments
    for (int size = segmentSize; size < arr.size(); size *= 2) {
        for (int left = 0; left < arr.size(); left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, static_cast<int>(arr.size()) - 1);

            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}
```

### Introspective Sort

Introspective Sort (IntroSort) is a hybrid sorting algorithm that provides both the fast average performance of Quick Sort and the worst-case O(n log n) runtime of Heap Sort:

```cpp
void introSort(std::vector<int>& arr, int depthLimit) {
    int n = arr.size();

    // If the array is small, use insertion sort
    if (n <= 16) {
        insertionSort(arr, 0, n - 1);
        return;
    }

    // If depth limit is 0, switch to heap sort
    if (depthLimit == 0) {
        heapSort(arr);
        return;
    }

    // Use quick sort with median-of-three pivot
    int pivot = medianOfThreePartition(arr, 0, n - 1);

    // Recursively sort sub-arrays
    introSort(std::vector<int>(arr.begin(), arr.begin() + pivot), depthLimit - 1);
    introSort(std::vector<int>(arr.begin() + pivot + 1, arr.end()), depthLimit - 1);
}

// Wrapper function
void introSort(std::vector<int>& arr) {
    int depthLimit = 2 * log2(arr.size());
    introSort(arr, depthLimit);
}
```

### External Sorting

When data doesn't fit in memory, external sorting techniques are used, often based on Merge Sort:

1. **Divide**: Split the file into chunks that fit in memory
2. **Sort**: Sort each chunk using an in-memory algorithm (e.g., Quick Sort)
3. **Save**: Write sorted chunks back to disk
4. **Merge**: Merge sorted chunks using a k-way merge

This approach is used in database systems, large file processing, and big data frameworks.

## 🚀 LeetCode Practice Problems

### Easy:

1. **Merge Sorted Array** (LeetCode #88)

   - Merge two sorted arrays where the first array has enough space at the end.

   ```cpp
   void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
       int i = m - 1;  // Last element in nums1
       int j = n - 1;  // Last element in nums2
       int k = m + n - 1;  // Last position in merged array

       // Start from the end and work backwards
       while (i >= 0 && j >= 0) {
           if (nums1[i] > nums2[j]) {
               nums1[k--] = nums1[i--];
           } else {
               nums1[k--] = nums2[j--];
           }
       }

       // If there are remaining elements in nums2
       while (j >= 0) {
           nums1[k--] = nums2[j--];
       }
   }
   ```

2. **Sort Array By Parity** (LeetCode #905)

   - Given an array of integers, return an array with even integers first, then odd integers.

   ```cpp
   vector<int> sortArrayByParity(vector<int>& A) {
       int left = 0;
       int right = A.size() - 1;

       while (left < right) {
           // Find an odd number from the left
           while (left < right && A[left] % 2 == 0) {
               left++;
           }

           // Find an even number from the right
           while (left < right && A[right] % 2 == 1) {
               right--;
           }

           // Swap the odd and even numbers
           if (left < right) {
               swap(A[left], A[right]);
           }
       }

       return A;
   }
   ```

3. **Intersection of Two Arrays** (LeetCode #349)

   - Find the intersection of two arrays (elements that appear in both).

   ```cpp
   vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
       // Sort both arrays
       sort(nums1.begin(), nums1.end());
       sort(nums2.begin(), nums2.end());

       vector<int> result;
       int i = 0, j = 0;

       while (i < nums1.size() && j < nums2.size()) {
           if (nums1[i] < nums2[j]) {
               i++;
           } else if (nums1[i] > nums2[j]) {
               j++;
           } else {
               // Found a common element
               if (result.empty() || result.back() != nums1[i]) {
                   result.push_back(nums1[i]);
               }
               i++;
               j++;
           }
       }

       return result;
   }
   ```

### Medium:

1. **Sort Colors** (LeetCode #75)

   - Sort an array with only three unique values (0, 1, and 2) in-place.

   ```cpp
   void sortColors(vector<int>& nums) {
       int left = 0;        // For 0's
       int mid = 0;         // For 1's
       int right = nums.size() - 1;  // For 2's

       while (mid <= right) {
           if (nums[mid] == 0) {
               swap(nums[left], nums[mid]);
               left++;
               mid++;
           } else if (nums[mid] == 1) {
               mid++;
           } else { // nums[mid] == 2
               swap(nums[mid], nums[right]);
               right--;
           }
       }
   }
   ```

2. **Kth Largest Element in an Array** (LeetCode #215)

   - Find the kth largest element in an unsorted array using Quick Select.

   ```cpp
   int findKthLargest(vector<int>& nums, int k) {
       // Quick select algorithm
       int left = 0;
       int right = nums.size() - 1;
       k = nums.size() - k;  // Convert to kth smallest

       while (left < right) {
           int pivotIndex = partition(nums, left, right);

           if (pivotIndex < k) {
               left = pivotIndex + 1;
           } else if (pivotIndex > k) {
               right = pivotIndex - 1;
           } else {
               break;
           }
       }

       return nums[k];
   }

   int partition(vector<int>& nums, int left, int right) {
       int pivot = nums[right];
       int i = left;

       for (int j = left; j < right; j++) {
           if (nums[j] <= pivot) {
               swap(nums[i], nums[j]);
               i++;
           }
       }

       swap(nums[i], nums[right]);
       return i;
   }
   ```

3. **Merge Intervals** (LeetCode #56)

   - Merge overlapping intervals.

   ```cpp
   vector<vector<int>> merge(vector<vector<int>>& intervals) {
       if (intervals.empty()) return {};

       // Sort intervals by start time
       sort(intervals.begin(), intervals.end(),
           [](const vector<int>& a, const vector<int>& b) {
               return a[0] < b[0];
           });

       vector<vector<int>> merged;
       merged.push_back(intervals[0]);

       for (int i = 1; i < intervals.size(); i++) {
           if (merged.back()[1] >= intervals[i][0]) {
               // Merge overlapping intervals
               merged.back()[1] = max(merged.back()[1], intervals[i][1]);
           } else {
               // Add new interval
               merged.push_back(intervals[i]);
           }
       }

       return merged;
   }
   ```

### Hard:

1. **Count of Smaller Numbers After Self** (LeetCode #315)

   - For each element, count the number of smaller elements to its right.

   ```cpp
   vector<int> countSmaller(vector<int>& nums) {
       int n = nums.size();
       vector<int> result(n, 0);
       vector<pair<int, int>> indexed;

       // Create pairs of (value, original index)
       for (int i = 0; i < n; i++) {
           indexed.push_back({nums[i], i});
       }

       // Modified merge sort
       mergeSortAndCount(indexed, 0, n - 1, result);

       return result;
   }

   void mergeSortAndCount(vector<pair<int, int>>& arr, int left, int right, vector<int>& result) {
       if (left >= right) return;

       int mid = left + (right - left) / 2;
       mergeSortAndCount(arr, left, mid, result);
       mergeSortAndCount(arr, mid + 1, right, result);

       // Count smaller elements and merge
       int j = mid + 1;
       for (int i = left; i <= mid; i++) {
           while (j <= right && arr[j].first < arr[i].first) {
               j++;
           }
           result[arr[i].second] += (j - (mid + 1));
       }

       // Merge the two sorted halves
       inplace_merge(arr.begin() + left, arr.begin() + mid + 1, arr.begin() + right + 1);
   }
   ```

2. **Reverse Pairs** (LeetCode #493)

   - Count the number of important reverse pairs: pairs (i, j) where i < j and nums[i] > 2\*nums[j].

   ```cpp
   int reversePairs(vector<int>& nums) {
       return mergeSort(nums, 0, nums.size() - 1);
   }

   int mergeSort(vector<int>& nums, int left, int right) {
       if (left >= right) return 0;

       int mid = left + (right - left) / 2;
       int count = mergeSort(nums, left, mid) + mergeSort(nums, mid + 1, right);

       // Count reverse pairs
       int j = mid + 1;
       for (int i = left; i <= mid; i++) {
           while (j <= right && nums[i] > 2LL * nums[j]) {
               j++;
           }
           count += (j - (mid + 1));
       }

       // Merge the two sorted arrays
       vector<int> merged(right - left + 1);
       int i = left, k = mid + 1, m = 0;

       while (i <= mid && k <= right) {
           if (nums[i] <= nums[k]) {
               merged[m++] = nums[i++];
           } else {
               merged[m++] = nums[k++];
           }
       }

       while (i <= mid) merged[m++] = nums[i++];
       while (k <= right) merged[m++] = nums[k++];

       for (int p = 0; p < merged.size(); p++) {
           nums[left + p] = merged[p];
       }

       return count;
   }
   ```

3. **Maximum Gap** (LeetCode #164)

   - Given an unsorted array, find the maximum gap between successive elements in the sorted form.

   ```cpp
   int maximumGap(vector<int>& nums) {
       if (nums.size() < 2) return 0;

       // Bucket sort approach
       int n = nums.size();
       int minVal = *min_element(nums.begin(), nums.end());
       int maxVal = *max_element(nums.begin(), nums.end());

       // Edge case: all elements are the same
       if (minVal == maxVal) return 0;

       // Bucket size and count
       int bucketSize = max(1, (maxVal - minVal) / (n - 1));
       int bucketCount = (maxVal - minVal) / bucketSize + 1;

       // Initialize buckets
       vector<pair<int, int>> buckets(bucketCount, {-1, -1}); // {min, max}

       // Fill buckets
       for (int num : nums) {
           int idx = (num - minVal) / bucketSize;
           if (buckets[idx].first == -1) {
               buckets[idx].first = buckets[idx].second = num;
           } else {
               buckets[idx].first = min(buckets[idx].first, num);
               buckets[idx].second = max(buckets[idx].second, num);
           }
       }

       // Find max gap
       int maxGap = 0;
       int prevBucketMax = minVal;

       for (auto& bucket : buckets) {
           if (bucket.first == -1) continue; // Skip empty buckets

           maxGap = max(maxGap, bucket.first - prevBucketMax);
           prevBucketMax = bucket.second;
       }

       return maxGap;
   }
   ```

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive into **Divide and Conquer Strategy**. We'll explore:

- The fundamental principles of the divide and conquer paradigm
- How to break down complex problems into simpler subproblems
- Applications beyond sorting (like the closest pair problem, Strassen's matrix multiplication, etc.)
- Analysis techniques for divide and conquer algorithms

_If today's sorting algorithms were examples of divide and conquer in action, tomorrow we'll explore the broader toolkit that this powerful paradigm provides for solving a wide range of computational problems._

#DSAin45 #Day18 #SortingAlgorithms #MergeSort #QuickSort #HeapSort #TimSort #DivideAndConquer #CPlusPlus #DataStructures #Algorithms #CodingInterview
