# 🔍 DAY 16: Searching Algorithms - Finding Needles in Haystacks #DSAin45

Welcome to Day 16 of our 45-day DSA journey! Today, we're diving into searching algorithms—the essential techniques for finding elements within our data structures. After learning about recursion yesterday, you're now equipped to understand the elegant recursive implementations of many of these searching methods.

_Like a detective with different investigative techniques for different cases, a skilled programmer knows which search algorithm to deploy based on the data and constraints at hand._

## 🎯 The Art of Searching

Searching is one of the most fundamental operations in computer science. Whether you're looking up a contact in your phone, finding a product on an e-commerce site, or querying a database, efficient search algorithms power these everyday experiences.

The key questions when choosing a searching algorithm are:

1. What is the structure of your data? (Sorted? Unsorted? Array? Linked list?)
2. How large is your dataset?
3. How frequent are search operations compared to insertions and deletions?
4. Is it a one-time search or will you search repeatedly?
5. What are your memory constraints?

## 📊 Linear Search: The Simple Detective

**Linear search** (also called sequential search) is the most straightforward searching algorithm. It checks each element in the data structure one by one until finding the target or exhausting the entire collection.

### Implementation

```cpp
int linearSearch(const std::vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i;  // Found the target at index i
        }
    }
    return -1;  // Target not found
}
```

### Characteristics

- **Time Complexity**: O(n) - in the worst case, we check all n elements
- **Space Complexity**: O(1) - requires only a constant amount of extra space
- **When to use**:
  - Unsorted data
  - Small datasets
  - When simplicity is more important than efficiency
  - When the element is likely to be near the beginning
  - When the data structure doesn't support random access (like linked lists)

### Linear Search in Linked Lists

When searching in a linked list, linear search is particularly suitable since we can't directly access arbitrary elements:

```cpp
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Linear search in a linked list
Node* linearSearchLinkedList(Node* head, int target) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == target) {
            return current;  // Found the target
        }
        current = current->next;
    }
    return nullptr;  // Target not found
}
```

### Variations and Optimizations

1. **Early Termination**: If you know your target must exist in the array, you can simplify the loop condition.

2. **Sentinel Linear Search**: Add the target at the end of the array to eliminate the boundary check in each iteration:

```cpp
int sentinelLinearSearch(std::vector<int> arr, int target) {
    int n = arr.size();

    // Save the last element and place target at the end
    int lastElement = arr[n-1];
    arr[n-1] = target;

    int i = 0;
    // No need to check array bounds
    while (arr[i] != target) {
        i++;
    }

    // Restore the last element
    arr[n-1] = lastElement;

    // Check if we found the target or reached the sentinel
    if (i < n-1 || arr[n-1] == target) {
        return i;
    }
    return -1;  // Target not found
}
```

3. **Ordered Linear Search**: If your array is sorted, you can terminate early when you encounter an element greater than the target.

```cpp
int orderedLinearSearch(const std::vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i;  // Found the target
        }
        if (arr[i] > target) {
            return -1;  // Target cannot be in the array
        }
    }
    return -1;  // Target not found
}
```

## 🔢 Binary Search: The Efficient Detective

**Binary search** is a much more efficient algorithm for finding elements in a sorted array. It repeatedly divides the search space in half, eliminating half of the remaining elements at each step.

_Think of binary search like finding a name in a phone book—you open to the middle, see if your target comes before or after, and then only search the relevant half._

### Implementation (Iterative)

```cpp
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;  // Avoid potential overflow

        // Check if target is present at mid
        if (arr[mid] == target) {
            return mid;
        }

        // If target is greater, ignore left half
        if (arr[mid] < target) {
            left = mid + 1;
        }
        // If target is smaller, ignore right half
        else {
            right = mid - 1;
        }
    }

    // Element not present
    return -1;
}
```

### Implementation (Recursive)

```cpp
int binarySearchRecursive(const std::vector<int>& arr, int target, int left, int right) {
    // Base case: element not found
    if (left > right) {
        return -1;
    }

    int mid = left + (right - left) / 2;

    // If the element is present at the middle
    if (arr[mid] == target) {
        return mid;
    }

    // If element is smaller than mid, search in the left subarray
    if (arr[mid] > target) {
        return binarySearchRecursive(arr, target, left, mid - 1);
    }

    // Else search in the right subarray
    return binarySearchRecursive(arr, target, mid + 1, right);
}

// Wrapper function
int binarySearchRecursive(const std::vector<int>& arr, int target) {
    return binarySearchRecursive(arr, target, 0, arr.size() - 1);
}
```

### Characteristics

- **Time Complexity**: O(log n) - much faster than linear search for large datasets
- **Space Complexity**:
  - O(1) for iterative implementation
  - O(log n) for recursive implementation (due to the call stack)
- **When to use**:
  - Sorted arrays
  - Large datasets
  - Random access data structures
  - When search operations are frequent

### Cautions and Common Pitfalls

1. **Mid-Point Calculation**: The traditional `mid = (left + right) / 2` can cause integer overflow for large arrays. Use `mid = left + (right - left) / 2` instead.

2. **Loop Condition**: Be careful with the condition `left <= right` vs. `left < right`. The former allows checking the last element.

3. **Boundary Updates**: Ensure you set `left = mid + 1` and `right = mid - 1` to avoid infinite loops.

4. **Sorted Requirement**: Binary search only works on sorted data. If your data is unsorted, you'll need to sort it first (which is O(n log n)) or use linear search.

### Finding the Insertion Point

A common variation of binary search is finding where an element should be inserted in a sorted array:

```cpp
int binarySearchInsertPosition(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;  // Exact match found
        }

        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    // At this point, left > right
    // 'left' is the position where the element should be inserted
    return left;
}
```

## 🦘 Jump Search: The Skipping Detective

**Jump search** is a searching algorithm that works on sorted arrays. It jumps ahead by fixed steps and then performs a linear search when the target element is bracketed.

_Like skipping pages in a book until you find the chapter you're looking for, then reading page by page within that chapter._

### Implementation

```cpp
int jumpSearch(const std::vector<int>& arr, int target) {
    int n = arr.size();
    if (n == 0) return -1;

    // Finding the optimal jump size: sqrt(n)
    int step = sqrt(n);

    // Finding the block where the element is present
    int prev = 0;
    while (arr[std::min(step, n) - 1] < target) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) {
            return -1;  // Element not present
        }
    }

    // Linear search within the identified block
    while (arr[prev] < target) {
        prev++;
        if (prev == std::min(step, n)) {
            return -1;  // Element not present
        }
    }

    // If element is found
    if (arr[prev] == target) {
        return prev;
    }

    return -1;  // Element not present
}
```

### Characteristics

- **Time Complexity**: O(√n) - better than linear search but slower than binary search
- **Space Complexity**: O(1)
- **When to use**:
  - Sorted arrays
  - When binary search is too complex to implement
  - When the step size can be optimized for specific data distributions

### Optimization: Optimal Step Size

The optimal step size for Jump Search is √n, which minimizes the worst-case time complexity. However, depending on your data distribution, you might choose a different step size.

## 🔮 Interpolation Search: The Psychic Detective

**Interpolation search** improves upon binary search by estimating the position of the target value based on its value. It works best when the elements are uniformly distributed.

_If binary search is like opening a phone book in the middle, interpolation search is like estimating where a name might be based on alphabetical distribution—you'd look near the beginning for "Aaron" and near the end for "Zachary"._

### Implementation

```cpp
int interpolationSearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right && target >= arr[left] && target <= arr[right]) {
        // Avoid division by zero
        if (left == right) {
            if (arr[left] == target) return left;
            return -1;
        }

        // Estimate the position using interpolation formula
        int pos = left + ((double)(target - arr[left]) * (right - left)) /
                         (arr[right] - arr[left]);

        if (arr[pos] == target) {
            return pos;
        }

        if (arr[pos] < target) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }

    return -1;  // Element not present
}
```

### Characteristics

- **Time Complexity**:
  - Average case: O(log log n) for uniformly distributed data
  - Worst case: O(n) when the distribution is skewed
- **Space Complexity**: O(1)
- **When to use**:
  - Sorted arrays with uniformly distributed values
  - When you can make reasonable predictions about element locations

### Performance Considerations

Interpolation search can outperform binary search for uniformly distributed data, but it can degrade to linear search in the worst case. It also involves more complex calculations per iteration.

## 🌳 Binary Search Trees

A **Binary Search Tree (BST)** is a tree data structure where each node has at most two children (left and right), and for each node:

- All elements in the left subtree are less than the node's value
- All elements in the right subtree are greater than the node's value

This property makes searching in a BST efficient, using a similar principle to binary search.

### Implementation

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Search in a BST
TreeNode* searchBST(TreeNode* root, int target) {
    // Base cases: root is null or the target is found
    if (root == nullptr || root->val == target) {
        return root;
    }

    // If target is less than root's value, search in left subtree
    if (target < root->val) {
        return searchBST(root->left, target);
    }

    // If target is greater than root's value, search in right subtree
    return searchBST(root->right, target);
}

// Iterative version
TreeNode* searchBSTIterative(TreeNode* root, int target) {
    while (root != nullptr && root->val != target) {
        if (target < root->val) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}
```

### Characteristics

- **Time Complexity**:
  - Average case: O(log n) for balanced trees
  - Worst case: O(n) for degenerate trees (essentially a linked list)
- **Space Complexity**:
  - O(log n) for recursive implementation due to the call stack
  - O(1) for iterative implementation
- **When to use**:
  - When you need a dynamic data structure that supports efficient search, insertion, and deletion
  - When the data is sorted and frequently updated

### Balanced BSTs

To ensure O(log n) performance, BSTs need to be balanced. Common balanced BST implementations include:

1. **AVL Trees**: Self-balancing BSTs where the heights of the two child subtrees differ by at most one
2. **Red-Black Trees**: Self-balancing BSTs with coloring properties that ensure balance
3. **B-Trees**: Self-balancing trees with multiple children per node, commonly used in databases and file systems

## 🔍 Exponential Search: The Expansive Detective

**Exponential search** combines the idea of jumping ahead with binary search. It works by exponentially increasing the range to be searched until a range containing the target is found, then applying binary search within that range.

_Like starting with small steps, doubling your stride each time until you overshoot, then using binary search to pinpoint the exact location._

### Implementation

```cpp
int exponentialSearch(const std::vector<int>& arr, int target) {
    int n = arr.size();
    if (n == 0) return -1;

    // If target is the first element
    if (arr[0] == target) {
        return 0;
    }

    // Find the range for binary search
    int i = 1;
    while (i < n && arr[i] <= target) {
        i *= 2;  // Double i each time
    }

    // Apply binary search in the found range
    int left = i / 2;
    int right = std::min(i, n - 1);

    return binarySearchRecursive(arr, target, left, right);
}
```

### Characteristics

- **Time Complexity**: O(log n)
- **Space Complexity**: O(log n) if using recursive binary search
- **When to use**:
  - Sorted arrays with unbounded or unknown size
  - When the target is likely to be near the beginning of the array

## 🏆 Searching Algorithm Comparison

Let's compare our searching algorithms across various dimensions:

| Algorithm            | Time Complexity (Avg) | Time Complexity (Worst) | Space Complexity | Pre-requisites  | Use Case                                 |
| -------------------- | --------------------- | ----------------------- | ---------------- | --------------- | ---------------------------------------- |
| Linear Search        | O(n)                  | O(n)                    | O(1)             | None            | Unsorted data, small datasets            |
| Binary Search        | O(log n)              | O(log n)                | O(1)             | Sorted array    | Large sorted datasets, frequent searches |
| Jump Search          | O(√n)                 | O(√n)                   | O(1)             | Sorted array    | Medium-sized sorted datasets             |
| Interpolation Search | O(log log n)          | O(n)                    | O(1)             | Sorted, uniform | Uniformly distributed data               |
| Exponential Search   | O(log n)              | O(log n)                | O(log n)         | Sorted array    | Unbounded arrays, target near beginning  |
| BST Search           | O(log n)              | O(n)                    | O(log n) / O(1)  | BST structure   | Dynamic data with frequent updates       |

### Performance Visualization

Imagine searching in an array of 1,000,000 elements. The average number of comparisons required:

- **Linear Search**: 500,000 (half the elements on average)
- **Binary Search**: ~20 (log₂ of 1,000,000)
- **Jump Search**: ~1,000 (square root of 1,000,000)
- **Interpolation Search**: ~5 (log log of 1,000,000, for uniform data)

This illustrates why sophisticated search algorithms are critical for large datasets!

## 🧩 Real-World Applications

1. **Database Indexing**: B-Trees and variations power the indexes in nearly all relational database systems, enabling fast lookups.

2. **Search Engines**: Complex variations of search algorithms power web search engines, handling massive datasets with sophisticated ranking.

3. **Autocomplete**: Trie data structures (which we'll see later) combined with searching algorithms power autocomplete features.

4. **IP Routing**: Routers use specialized search algorithms to quickly find the next hop for an IP packet.

5. **Spell Checkers**: Edit distance algorithms combined with efficient search power spell-checking features.

6. **Compression Algorithms**: Huffman coding and other compression techniques rely on efficient tree search algorithms.

7. **Geolocation and Maps**: Quad-trees and other spatial data structures use specialized search algorithms for location-based services.

## 😄 Searching Algorithm Jokes and Analogies

1. **The Librarian Analogy**:

   - Linear Search is like a librarian who starts at the first shelf and checks each book until finding yours.
   - Binary Search is like a librarian who starts in the middle, determines which half contains your book, and repeats.
   - Interpolation Search is like an experienced librarian who knows "H" authors are likely to be about 40% through the shelves.

2. **The Dating Analogy**:

   - Linear Search: Going on a date with every single person in your city until you find "the one."
   - Binary Search: Using a matchmaking service that narrows down your options by half with each round of speed dating.
   - BST: Setting up a dating pyramid scheme where each person introduces you to two more people who match your criteria better.

3. **The Programmer's Curse**:
   "A programmer spends 20 minutes implementing binary search to save 5 seconds of runtime, then wastes 2 hours debugging because they forgot the array needs to be sorted first."

4. **Linear vs. Binary**:
   "Linear search is like eating a foot-long sandwich one bite at a time. Binary search is like cutting the sandwich in half repeatedly until you find the piece with the pickle."

5. **Interpolation Search Optimism**:
   "Interpolation search is the optimist of the algorithm world. It assumes the data is perfectly distributed and is devastated when it encounters a skewed dataset."

6. **The Stack Overflow Joke**:
   "How many search algorithms does it take to find a bug? None. The bug will find you when your recursive binary search causes a stack overflow."

## 🚀 LeetCode Practice Problems

### Easy:

1. **Binary Search** (LeetCode #704)

   - Given a sorted array of integers and a target value, return the index if the target is found. If not, return -1.

   ```cpp
   int search(vector<int>& nums, int target) {
       int left = 0;
       int right = nums.size() - 1;

       while (left <= right) {
           int mid = left + (right - left) / 2;

           if (nums[mid] == target) {
               return mid;
           }

           if (nums[mid] < target) {
               left = mid + 1;
           } else {
               right = mid - 1;
           }
       }

       return -1;
   }
   ```

2. **Search Insert Position** (LeetCode #35)

   - Given a sorted array of distinct integers and a target value, return the index if found. If not, return the index where it would be if it were inserted in order.

   ```cpp
   int searchInsert(vector<int>& nums, int target) {
       int left = 0;
       int right = nums.size() - 1;

       while (left <= right) {
           int mid = left + (right - left) / 2;

           if (nums[mid] == target) {
               return mid;
           }

           if (nums[mid] < target) {
               left = mid + 1;
           } else {
               right = mid - 1;
           }
       }

       return left;
   }
   ```

3. **First Bad Version** (LeetCode #278)

   - You have n versions of a product and you want to find the first bad one, which causes all subsequent versions to be bad.

   ```cpp
   int firstBadVersion(int n) {
       int left = 1;
       int right = n;

       while (left < right) {
           int mid = left + (right - left) / 2;

           if (isBadVersion(mid)) {
               right = mid;
           } else {
               left = mid + 1;
           }
       }

       return left;
   }
   ```

### Medium:

1. **Find First and Last Position of Element in Sorted Array** (LeetCode #34)

   - Given a sorted array with possibly duplicate elements, find the start and end positions of a given target value.

   ```cpp
   vector<int> searchRange(vector<int>& nums, int target) {
       int first = findFirst(nums, target);
       int last = findLast(nums, target);
       return {first, last};
   }

   int findFirst(vector<int>& nums, int target) {
       int left = 0;
       int right = nums.size() - 1;
       int result = -1;

       while (left <= right) {
           int mid = left + (right - left) / 2;

           if (nums[mid] >= target) {
               right = mid - 1;
               if (nums[mid] == target) {
                   result = mid;
               }
           } else {
               left = mid + 1;
           }
       }

       return result;
   }

   int findLast(vector<int>& nums, int target) {
       int left = 0;
       int right = nums.size() - 1;
       int result = -1;

       while (left <= right) {
           int mid = left + (right - left) / 2;

           if (nums[mid] <= target) {
               left = mid + 1;
               if (nums[mid] == target) {
                   result = mid;
               }
           } else {
               right = mid - 1;
           }
       }

       return result;
   }
   ```

2. **Search in Rotated Sorted Array** (LeetCode #33)

   - Search in a sorted array that has been rotated at some pivot, with O(log n) runtime.

   ```cpp
   int search(vector<int>& nums, int target) {
       int left = 0;
       int right = nums.size() - 1;

       while (left <= right) {
           int mid = left + (right - left) / 2;

           if (nums[mid] == target) {
               return mid;
           }

           // Check if the left side is sorted
           if (nums[left] <= nums[mid]) {
               // Check if target is in the left sorted portion
               if (nums[left] <= target && target < nums[mid]) {
                   right = mid - 1;
               } else {
                   left = mid + 1;
               }
           }
           // Right side is sorted
           else {
               // Check if target is in the right sorted portion
               if (nums[mid] < target && target <= nums[right]) {
                   left = mid + 1;
               } else {
                   right = mid - 1;
               }
           }
       }

       return -1;
   }
   ```

3. **Find Peak Element** (LeetCode #162)

   - Find a peak element where a peak is an element greater than its neighbors.

   ```cpp
   int findPeakElement(vector<int>& nums) {
       int left = 0;
       int right = nums.size() - 1;

       while (left < right) {
           int mid = left + (right - left) / 2;

           if (nums[mid] > nums[mid + 1]) {
               // Peak is in the left half (including mid)
               right = mid;
           } else {
               // Peak is in the right half
               left = mid + 1;
           }
       }

       return left;
   }
   ```

### Hard:

1. **Median of Two Sorted Arrays** (LeetCode #4)

   - Find the median of two sorted arrays with O(log(m+n)) complexity.

   ```cpp
   double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
       // Ensure nums1 is the smaller array for simplicity
       if (nums1.size() > nums2.size()) {
           return findMedianSortedArrays(nums2, nums1);
       }

       int x = nums1.size();
       int y = nums2.size();
       int low = 0;
       int high = x;

       while (low <= high) {
           int partitionX = (low + high) / 2;
           int partitionY = (x + y + 1) / 2 - partitionX;

           // If partitionX is 0, use -INFINITY for maxX
           // If partitionX is x, use INFINITY for minX
           int maxX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
           int minX = (partitionX == x) ? INT_MAX : nums1[partitionX];

           int maxY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
           int minY = (partitionY == y) ? INT_MAX : nums2[partitionY];

           if (maxX <= minY && maxY <= minX) {
               // Found correct partition
               // Check if the combined array length is odd or even
               if ((x + y) % 2 != 0) {
                   return std::max(maxX, maxY);
               } else {
                   return (std::max(maxX, maxY) + std::min(minX, minY)) / 2.0;
               }
           } else if (maxX > minY) {
               // Move toward the left in nums1
               high = partitionX - 1;
           } else {
               // Move toward the right in nums1
               low = partitionX + 1;
           }
       }

       // If we reach here, the input arrays were not sorted
       throw std::invalid_argument("Input arrays must be sorted");
   }
   ```

2. **Search in a 2D Matrix II** (LeetCode #240)

   - Search in a 2D matrix where each row and each column is sorted in ascending order.

   ```cpp
   bool searchMatrix(vector<vector<int>>& matrix, int target) {
       if (matrix.empty() || matrix[0].empty()) {
           return false;
       }

       int rows = matrix.size();
       int cols = matrix[0].size();

       // Start from the top-right corner
       int row = 0;
       int col = cols - 1;

       while (row < rows && col >= 0) {
           if (matrix[row][col] == target) {
               return true;
           } else if (matrix[row][col] > target) {
               // Current value is larger, move left
               col--;
           } else {
               // Current value is smaller, move down
               row++;
           }
       }

       return false;
   }
   ```

3. **Split Array Largest Sum** (LeetCode #410)

   - Split an array into m subarrays such that the largest sum of any subarray is minimized.

   ```cpp
   int splitArray(vector<int>& nums, int m) {
       int left = *max_element(nums.begin(), nums.end());
       int right = accumulate(nums.begin(), nums.end(), 0);

       // Binary search on the result (the minimum largest subarray sum)
       while (left < right) {
           int mid = left + (right - left) / 2;

           // Check if it's possible to split the array with max sum 'mid'
           if (canSplit(nums, m, mid)) {
               right = mid;
           } else {
               left = mid + 1;
           }
       }

       return left;
   }

   bool canSplit(vector<int>& nums, int m, int maxSum) {
       int subarrays = 1;
       int currentSum = 0;

       for (int num : nums) {
           if (currentSum + num > maxSum) {
               subarrays++;
               currentSum = num;

               if (subarrays > m) {
                   return false;
               }
           } else {
               currentSum += num;
           }
       }

       return true;
   }
   ```

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive into **Basic Sorting Algorithms**. We'll explore classic sorting techniques like:

- Bubble sort
- Selection sort
- Insertion sort
- And understand their implementations, time complexities, and use cases

While today we explored how to find elements in data structures, tomorrow we'll learn how to arrange those elements in an ordered manner—a fundamental skill for any programmer's toolkit!

_Think of today's searching as finding your favorite book in a library, and tomorrow's sorting as organizing the entire library for easier searches in the future._

#DSAin45 #Day16 #SearchingAlgorithms #BinarySearch #DataStructures #Algorithms #CPlusPlus #CodingInterview
