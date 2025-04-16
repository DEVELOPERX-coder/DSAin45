# 🔄 DAY 17: Basic Sorting Algorithms - Building the Foundations #DSAin45

Welcome to Day 17 of our 45-day DSA journey! After exploring searching algorithms yesterday, we're now diving into the fundamental world of sorting. Today, we'll focus on the basic sorting algorithms that form the foundation of algorithmic thinking.

_Like learning scales before playing a concerto or mastering knife skills before cooking a gourmet meal, understanding basic sorting algorithms builds the foundation for more sophisticated computational thinking._

## 📊 The Art of Sorting

Sorting is one of the most fundamental operations in computer science. Whether it's organizing a list of names alphabetically, arranging products by price, or preparing data for efficient searching, sorting algorithms power countless applications in our digital world.

Why are sorting algorithms so important?

1. **They enable efficient searching**: Many searching algorithms (like binary search) require sorted data.
2. **They facilitate data analysis**: Sorted data reveals patterns and makes statistical calculations easier.
3. **They improve user experience**: Users expect data to be presented in logical order.
4. **They optimize other algorithms**: Many algorithms perform better when input is sorted.
5. **They teach algorithmic thinking**: Sorting algorithms demonstrate core computer science principles.

## 🫧 Bubble Sort: The Simplest Approach

Bubble Sort is often the first sorting algorithm introduced to students, and for good reason. It's conceptually simple: repeatedly step through the list, compare adjacent elements, and swap them if they're in the wrong order.

_Like bubbles rising to the surface of water, the largest elements "bubble up" to the end of the array in each pass._

### Algorithm Description

1. Start at the beginning of the array
2. Compare adjacent elements and swap them if they're in the wrong order
3. Move to the next pair of adjacent elements and repeat
4. After completing one pass, the largest element will be at the end
5. Repeat the process for the remaining elements (excluding sorted ones)
6. Continue until no more swaps are needed

### Implementation

```cpp
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        // Last i elements are already sorted, so we don't need to check them
        for (int j = 0; j < n - i - 1; j++) {
            // If the current element is greater than the next element, swap them
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no swapping occurred in this pass, array is sorted
        if (!swapped) {
            break;
        }
    }
}
```

### Step-by-Step Example

Let's trace the Bubble Sort algorithm with a small array: `[5, 3, 8, 4, 2]`

Pass 1:

- Compare 5 and 3: Swap → `[3, 5, 8, 4, 2]`
- Compare 5 and 8: No swap → `[3, 5, 8, 4, 2]`
- Compare 8 and 4: Swap → `[3, 5, 4, 8, 2]`
- Compare 8 and 2: Swap → `[3, 5, 4, 2, 8]`
- End of pass 1: Largest element (8) is now at the end

Pass 2:

- Compare 3 and 5: No swap → `[3, 5, 4, 2, 8]`
- Compare 5 and 4: Swap → `[3, 4, 5, 2, 8]`
- Compare 5 and 2: Swap → `[3, 4, 2, 5, 8]`
- End of pass 2: Second largest element (5) is in its correct position

Pass 3:

- Compare 3 and 4: No swap → `[3, 4, 2, 5, 8]`
- Compare 4 and 2: Swap → `[3, 2, 4, 5, 8]`
- End of pass 3: Third largest element (4) is in its correct position

Pass 4:

- Compare 3 and 2: Swap → `[2, 3, 4, 5, 8]`
- End of pass 4: Array is now sorted

### Analysis

- **Time Complexity**:
  - Best Case: O(n) - when the array is already sorted (with the optimization to break if no swaps)
  - Average Case: O(n²)
  - Worst Case: O(n²) - when the array is sorted in reverse order
- **Space Complexity**: O(1) - only requires a constant amount of extra space
- **Stability**: Stable - equal elements maintain their relative order

### Advantages and Disadvantages

**Advantages**:

- Simple to implement and understand
- Requires minimal extra space
- Stable sorting algorithm
- Works well for small datasets
- Performs well when array is nearly sorted

**Disadvantages**:

- Inefficient for large datasets
- Has a time complexity of O(n²)
- Performs many swaps which can be expensive

## 🔍 Selection Sort: Finding the Minimum

Selection Sort takes a different approach: instead of repeatedly swapping adjacent elements, it repeatedly selects the minimum element from the unsorted portion and puts it at the beginning.

_Like selecting the smallest card from your hand and placing it in a new sorted pile._

### Algorithm Description

1. Divide the array into two parts: sorted (initially empty) and unsorted (initially the entire array)
2. Find the minimum element in the unsorted part
3. Swap it with the first element of the unsorted part
4. Expand the sorted part by one element (moving the boundary between sorted and unsorted)
5. Repeat until the entire array is sorted

### Implementation

```cpp
void selectionSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        // Find the minimum element in the unsorted part
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // Swap the found minimum element with the first element of the unsorted part
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}
```

### Step-by-Step Example

Let's trace the Selection Sort algorithm with a small array: `[5, 3, 8, 4, 2]`

Pass 1:

- Find minimum in positions 0-4: value 2 at position 4
- Swap positions 0 and 4: `[2, 3, 8, 4, 5]`
- Sorted portion: `[2]`, Unsorted portion: `[3, 8, 4, 5]`

Pass 2:

- Find minimum in positions 1-4: value 3 at position 1
- No swap needed (already at position 1)
- Sorted portion: `[2, 3]`, Unsorted portion: `[8, 4, 5]`

Pass 3:

- Find minimum in positions 2-4: value 4 at position 3
- Swap positions 2 and 3: `[2, 3, 4, 8, 5]`
- Sorted portion: `[2, 3, 4]`, Unsorted portion: `[8, 5]`

Pass 4:

- Find minimum in positions 3-4: value 5 at position 4
- Swap positions 3 and 4: `[2, 3, 4, 5, 8]`
- Sorted portion: `[2, 3, 4, 5]`, Unsorted portion: `[8]`

With only one element left in the unsorted portion, the array is now sorted.

### Analysis

- **Time Complexity**:
  - Best Case: O(n²) - even if the array is already sorted, we still need to find the minimum
  - Average Case: O(n²)
  - Worst Case: O(n²)
- **Space Complexity**: O(1) - only requires a constant amount of extra space
- **Stability**: Not stable - equal elements may change their relative order

### Advantages and Disadvantages

**Advantages**:

- Simple to implement and understand
- Performs well on small arrays
- Makes the minimum number of swaps (at most n-1 swaps)
- Works well when memory writes are expensive
- In-place sorting algorithm (requires no extra space)

**Disadvantages**:

- Inefficient for large datasets
- Always performs O(n²) comparisons even if the array is already sorted
- Not stable (relative ordering of equal elements may change)

## 📥 Insertion Sort: Building a Sorted Sequence

Insertion Sort builds the sorted array one element at a time, similar to how people often sort playing cards in their hands.

_Imagine sorting a hand of cards: you pick up one card at a time and insert it into its proper position among the cards you're already holding._

### Algorithm Description

1. Start with the second element (consider the first element as already sorted)
2. Compare it with the elements in the sorted part, moving backward
3. Shift elements greater than the key to the right
4. Insert the key at its correct position in the sorted part
5. Repeat for all elements in the array

### Implementation

```cpp
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        int key = arr[i];  // Current element to be inserted
        int j = i - 1;     // Index of the last element in the sorted part

        // Move elements greater than key one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        // Insert the key at its correct position
        arr[j + 1] = key;
    }
}
```

### Step-by-Step Example

Let's trace the Insertion Sort algorithm with a small array: `[5, 3, 8, 4, 2]`

Starting with `[5]` as the sorted part and `[3, 8, 4, 2]` as the unsorted part:

Step 1: Consider element 3

- Compare 3 with 5: 3 < 5, so insert 3 before 5
- Array: `[3, 5, 8, 4, 2]`
- Sorted part: `[3, 5]`, Unsorted part: `[8, 4, 2]`

Step 2: Consider element 8

- Compare 8 with 5: 8 > 5, so keep 8 after 5
- Array: `[3, 5, 8, 4, 2]`
- Sorted part: `[3, 5, 8]`, Unsorted part: `[4, 2]`

Step 3: Consider element 4

- Compare 4 with 8: 4 < 8, so continue
- Compare 4 with 5: 4 < 5, so continue
- Compare 4 with 3: 4 > 3, so insert 4 after 3
- Array: `[3, 4, 5, 8, 2]`
- Sorted part: `[3, 4, 5, 8]`, Unsorted part: `[2]`

Step 4: Consider element 2

- Compare 2 with 8: 2 < 8, so continue
- Compare 2 with 5: 2 < 5, so continue
- Compare 2 with 4: 2 < 4, so continue
- Compare 2 with 3: 2 < 3, so insert 2 before 3
- Array: `[2, 3, 4, 5, 8]`
- Sorted part: `[2, 3, 4, 5, 8]`, Unsorted part: `[]`

The array is now fully sorted.

### Analysis

- **Time Complexity**:
  - Best Case: O(n) - when the array is already sorted
  - Average Case: O(n²)
  - Worst Case: O(n²) - when the array is sorted in reverse order
- **Space Complexity**: O(1) - only requires a constant amount of extra space
- **Stability**: Stable - equal elements maintain their relative order

### Advantages and Disadvantages

**Advantages**:

- Simple implementation
- Efficient for small datasets
- More efficient than Bubble Sort and Selection Sort in practice
- Adaptive: O(n) time when array is nearly sorted
- Stable sorting algorithm
- In-place sorting algorithm
- Online algorithm (can sort a list as it receives it)

**Disadvantages**:

- Inefficient for large datasets
- Has a time complexity of O(n²) in the worst case

## 🔁 Binary Insertion Sort: An Optimization

Binary Insertion Sort is an optimization of Insertion Sort that uses binary search to find the insertion point, reducing the number of comparisons but not the number of shifts.

_Like using a more efficient method to find where to insert a new card in your hand, but still needing to make room for it._

### Implementation

```cpp
void binaryInsertionSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int left = 0;
        int right = i - 1;

        // Binary search to find insertion point
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] > key) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        // Shift elements to make room for key
        for (int j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }

        // Insert the key at the correct position
        arr[left] = key;
    }
}
```

### Analysis

- **Time Complexity**:
  - Best Case: O(n log n) for comparisons, but still O(n²) for shifts
  - Average Case: O(n log n) for comparisons, O(n²) for shifts
  - Worst Case: O(n log n) for comparisons, O(n²) for shifts
- **Space Complexity**: O(1)
- **Stability**: Stable

While this optimization reduces the number of comparisons, the number of shifts remains the same, so the asymptotic time complexity is still O(n²).

## ⚔️ Comparing Basic Sorting Algorithms

Let's compare our three basic sorting algorithms across multiple dimensions:

| Algorithm      | Best Case | Average Case | Worst Case | Space | Stable | Adaptive | In-Place |
| -------------- | --------- | ------------ | ---------- | ----- | ------ | -------- | -------- |
| Bubble Sort    | O(n)      | O(n²)        | O(n²)      | O(1)  | Yes    | Yes      | Yes      |
| Selection Sort | O(n²)     | O(n²)        | O(n²)      | O(1)  | No     | No       | Yes      |
| Insertion Sort | O(n)      | O(n²)        | O(n²)      | O(1)  | Yes    | Yes      | Yes      |

**Key terms explained**:

- **Stable**: Preserves the relative order of equal elements
- **Adaptive**: Performs better when the input is partially sorted
- **In-Place**: Requires only a constant amount of extra space

### When to Use Each Algorithm

**Bubble Sort**:

- When the array is small
- When the array is nearly sorted
- When simplicity is more important than efficiency
- When you need a stable sort with minimal extra space

**Selection Sort**:

- When memory writes are expensive (it performs the minimum number of swaps: O(n))
- When simplicity is more important than efficiency
- When auxiliary memory is limited

**Insertion Sort**:

- When the array is small
- When the array is nearly sorted
- When incoming data is being sorted in real-time (online algorithm)
- When you need a simple, stable, adaptive sort

## 🌎 Real-World Applications

Despite their simplicity and inefficiency for large datasets, these basic sorting algorithms have practical applications:

1. **Insertion Sort** is used in:

   - V8 JavaScript engine for sorting small arrays
   - Hybrid sorting algorithms (like Tim Sort, used in Python and Java) for small subarrays
   - Card sorting by hand (humans naturally use this algorithm)
   - Online sorting when elements arrive one at a time

2. **Bubble Sort** is used in:

   - Educational contexts to teach sorting concepts
   - Graphics algorithms to detect very small errors in nearly-sorted arrays

3. **Selection Sort** is used in:

   - Memory-constrained systems due to its minimal swaps
   - Systems where writing to memory is significantly more expensive than reading

4. **All basic sorts** are used in:
   - Systems where code simplicity and maintainability are more important than speed
   - Embedded systems with limited resources
   - Sorting very small arrays where more complex algorithms have higher overhead

## 😄 Sorting Algorithm Jokes and Analogies

1. **The Bubble Sort Joke**:
   "Why did the bubble sort break up with the quick sort? It found quick sort's partitioning too divisive."

2. **The Selection Sort Analogy**:
   "Selection sort is like shopping at a store where you have to pick the cheapest item, buy it, then go back to the store for the next cheapest... until you've bought everything on your list."

3. **The Insertion Sort Card Analogy**:
   "Insertion sort is exactly how most people organize their hand in a card game—pick up one card at a time and insert it into the right position."

4. **The Algorithm Comparison**:
   "Bubble sort, selection sort, and insertion sort are like economy cars. They'll get you there eventually, but don't expect to win any races against algorithms driving sports cars like merge sort and quick sort."

5. **The Computer Science Professor Joke**:
   "A computer science professor asks a student: 'What's special about bubble sort?' The student replies, 'It performs better when the input is already sorted.' The professor says, 'So does every other sorting algorithm!'"

## 🧠 Optimizations and Variations

### Cocktail Shaker Sort (Bidirectional Bubble Sort)

An optimization of Bubble Sort that traverses the array in both directions, reducing the turtle problem (small values near the end taking a long time to "bubble" to their correct positions).

```cpp
void cocktailShakerSort(std::vector<int>& arr) {
    bool swapped = true;
    int start = 0;
    int end = arr.size() - 1;

    while (swapped) {
        // Reset swapped flag for forward pass
        swapped = false;

        // Forward pass (like bubble sort)
        for (int i = start; i < end; i++) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        // If nothing moved, array is sorted
        if (!swapped) break;

        // Reset swapped flag for backward pass
        swapped = false;

        // Decrement end as largest element is now at end
        end--;

        // Backward pass (bubble smallest element to the front)
        for (int i = end - 1; i >= start; i--) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        // Increment start as smallest element is now at start
        start++;
    }
}
```

### Shell Sort

An extension of Insertion Sort that allows exchanges of elements that are far apart, producing partially sorted arrays that are then efficiently sorted with insertion sort.

```cpp
void shellSort(std::vector<int>& arr) {
    int n = arr.size();

    // Start with a large gap, then reduce it
    for (int gap = n/2; gap > 0; gap /= 2) {
        // Perform insertion sort for each gap size
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            // Shift elements that are gap positions apart
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }

            // Put temp in its correct location
            arr[j] = temp;
        }
    }
}
```

Shell Sort is significantly more efficient than the basic sorting algorithms, with a time complexity between O(n log n) and O(n²) depending on the gap sequence used.

## 🚀 LeetCode Practice Problems

### Easy:

1. **Sort Array By Parity** (LeetCode #905)

   - Given an array of integers, return an array such that even integers appear first, then odd integers.

   ```cpp
   vector<int> sortArrayByParity(vector<int>& nums) {
       int i = 0;
       int j = nums.size() - 1;

       while (i < j) {
           // Find an odd number from the left
           while (i < j && nums[i] % 2 == 0) {
               i++;
           }

           // Find an even number from the right
           while (i < j && nums[j] % 2 == 1) {
               j--;
           }

           // Swap them
           if (i < j) {
               std::swap(nums[i], nums[j]);
               i++;
               j--;
           }
       }

       return nums;
   }
   ```

2. **Sort Colors** (LeetCode #75)

   - Given an array with objects of three colors (0, 1, and 2), sort them in-place.

   ```cpp
   void sortColors(vector<int>& nums) {
       int low = 0;        // For 0's
       int mid = 0;        // For 1's
       int high = nums.size() - 1;  // For 2's

       while (mid <= high) {
           if (nums[mid] == 0) {
               std::swap(nums[low], nums[mid]);
               low++;
               mid++;
           } else if (nums[mid] == 1) {
               mid++;
           } else { // nums[mid] == 2
               std::swap(nums[mid], nums[high]);
               high--;
           }
       }
   }
   ```

3. **Merge Sorted Array** (LeetCode #88)

   - Merge two sorted arrays where the first array has enough space at the end.

   ```cpp
   void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
       int i = m - 1;  // Index for nums1
       int j = n - 1;  // Index for nums2
       int k = m + n - 1;  // Index for merged array

       // Start from the end and work backwards
       while (i >= 0 && j >= 0) {
           if (nums1[i] > nums2[j]) {
               nums1[k--] = nums1[i--];
           } else {
               nums1[k--] = nums2[j--];
           }
       }

       // Copy remaining elements from nums2 (if any)
       while (j >= 0) {
           nums1[k--] = nums2[j--];
       }
   }
   ```

### Medium:

1. **Sort an Array** (LeetCode #912)

   - Sort an array of integers (an opportunity to implement one of the basic sorts).

   ```cpp
   vector<int> sortArray(vector<int>& nums) {
       // Let's use insertion sort for this example
       int n = nums.size();

       for (int i = 1; i < n; i++) {
           int key = nums[i];
           int j = i - 1;

           while (j >= 0 && nums[j] > key) {
               nums[j + 1] = nums[j];
               j--;
           }

           nums[j + 1] = key;
       }

       return nums;
   }
   ```

2. **Wiggle Sort** (LeetCode #280)

   - Given an unsorted array, reorder it in-place such that nums[0] ≤ nums[1] ≥ nums[2] ≤ nums[3]...

   ```cpp
   void wiggleSort(vector<int>& nums) {
       for (int i = 0; i < nums.size() - 1; i++) {
           if ((i % 2 == 0 && nums[i] > nums[i+1]) ||
               (i % 2 == 1 && nums[i] < nums[i+1])) {
               std::swap(nums[i], nums[i+1]);
           }
       }
   }
   ```

3. **Pancake Sorting** (LeetCode #969)

   - Sort an array by doing a series of pancake flips.

   ```cpp
   vector<int> pancakeSort(vector<int>& arr) {
       vector<int> result;
       int n = arr.size();

       for (int i = n; i > 0; i--) {
           // Find the position of the maximum value in the unsorted part
           int maxIdx = 0;
           for (int j = 0; j < i; j++) {
               if (arr[j] > arr[maxIdx]) {
                   maxIdx = j;
               }
           }

           // If the max is already at the end of the unsorted part, continue
           if (maxIdx == i - 1) continue;

           // First flip: bring max to the beginning (if it's not already there)
           if (maxIdx != 0) {
               // Flip from 0 to maxIdx
               reverse(arr.begin(), arr.begin() + maxIdx + 1);
               result.push_back(maxIdx + 1);
           }

           // Second flip: bring max to its final position
           reverse(arr.begin(), arr.begin() + i);
           result.push_back(i);
       }

       return result;
   }
   ```

### Hard:

1. **First Missing Positive** (LeetCode #41)

   - Find the smallest missing positive integer in an unsorted array.

   ```cpp
   int firstMissingPositive(vector<int>& nums) {
       int n = nums.size();

       // Basic idea: ensure that nums[i] = i+1 for all valid positions
       for (int i = 0; i < n; i++) {
           // Keep swapping until we get a number that's out of range or already in correct position
           while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
               std::swap(nums[i], nums[nums[i] - 1]);
           }
       }

       // Find the first position where the number doesn't match the index
       for (int i = 0; i < n; i++) {
           if (nums[i] != i + 1) {
               return i + 1;
           }
       }

       // If all numbers are in correct positions, the missing number is n+1
       return n + 1;
   }
   ```

2. **Maximum Gap** (LeetCode #164)

   - Given an unsorted array, find the maximum gap between successive elements in the sorted form.

   ```cpp
   int maximumGap(vector<int>& nums) {
       if (nums.size() < 2) return 0;

       // Use bucket sort concept for linear time
       int n = nums.size();
       int minVal = *min_element(nums.begin(), nums.end());
       int maxVal = *max_element(nums.begin(), nums.end());

       // Edge case
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
       int prevMax = minVal;
       for (auto& bucket : buckets) {
           if (bucket.first == -1) continue; // Skip empty buckets

           maxGap = max(maxGap, bucket.first - prevMax);
           prevMax = bucket.second;
       }

       return maxGap;
   }
   ```

3. **Count of Smaller Numbers After Self** (LeetCode #315)

   - For each element, count how many smaller elements appear after it in the array.

   ```cpp
   vector<int> countSmaller(vector<int>& nums) {
       int n = nums.size();
       vector<int> result(n, 0);
       vector<pair<int, int>> indexed; // {value, original index}

       // Create indexed array
       for (int i = 0; i < n; i++) {
           indexed.push_back({nums[i], i});
       }

       // Merge sort with counting
       mergeSortCount(indexed, 0, n - 1, result);

       return result;
   }

   void mergeSortCount(vector<pair<int, int>>& arr, int left, int right, vector<int>& result) {
       if (left >= right) return;

       int mid = left + (right - left) / 2;
       mergeSortCount(arr, left, mid, result);
       mergeSortCount(arr, mid + 1, right, result);
       merge(arr, left, mid, right, result);
   }

   void merge(vector<pair<int, int>>& arr, int left, int mid, int right, vector<int>& result) {
       vector<pair<int, int>> temp(right - left + 1);

       int i = left;
       int j = mid + 1;
       int k = 0;

       int rightCount = 0; // Count of elements from right side that are smaller

       while (i <= mid && j <= right) {
           if (arr[j].first < arr[i].first) {
               // Element from right side is smaller
               rightCount++;
               temp[k++] = arr[j++];
           } else {
               // Element from left side is smaller or equal
               // Update count for this element
               result[arr[i].second] += rightCount;
               temp[k++] = arr[i++];
           }
       }

       // Handle remaining elements in left half
       while (i <= mid) {
           result[arr[i].second] += rightCount;
           temp[k++] = arr[i++];
       }

       // Handle remaining elements in right half
       while (j <= right) {
           temp[k++] = arr[j++];
       }

       // Copy back to original array
       for (i = 0; i < k; i++) {
           arr[left + i] = temp[i];
       }
   }
   ```

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive into **Efficient Sorting Algorithms**. We'll explore:

- Merge Sort: A divide-and-conquer algorithm that ensures O(n log n) time complexity
- Quick Sort: Another divide-and-conquer approach with excellent average-case performance
- Heap Sort: A comparison-based sort that uses the heap data structure
- Comparison of these algorithms and their practical applications

_If today's algorithms were the reliable family sedans of the sorting world, tomorrow's algorithms are the high-performance sports cars—faster, more efficient, but also a bit more complex under the hood._

#DSAin45 #Day17 #SortingAlgorithms #CPlusPlus #DataStructures #Algorithms #BubbleSort #SelectionSort #InsertionSort #CodingInterview
