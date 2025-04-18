# 🧩 DAY 19: Divide and Conquer Strategy - Breaking Problems into Manageable Pieces #DSAin45

Welcome to Day 19 of our 45-day DSA journey! Yesterday, we explored efficient sorting algorithms that break the O(n²) barrier. Today, we'll dive deeper into the fundamental paradigm that powers many of these algorithms: the Divide and Conquer strategy.

_Think of divide and conquer as the time-honored strategy of facing a formidable opponent: rather than confronting an overwhelming force head-on, divide it into smaller units that can be defeated individually, then combine these victories into an overall triumph._

## 📌 Understanding Divide and Conquer

Divide and Conquer is a algorithmic paradigm where a problem is broken down into smaller sub-problems that are similar to the original problem but simpler to solve. The solutions to these sub-problems are then combined to solve the original problem.

### The Three Steps of Divide and Conquer

1. **Divide**: Break the original problem into smaller subproblems.
2. **Conquer**: Solve the subproblems recursively. If the subproblems are small enough, solve them directly.
3. **Combine**: Merge the solutions of the subproblems to create a solution to the original problem.

### Visualization of the Paradigm

```
                        Original Problem
                             /   \
                            /     \
                           /       \
                Subproblem 1    Subproblem 2
                    / \             / \
                   /   \           /   \
            Sub-sub 1  Sub-sub 2  ...  ...
                  \     /          \   /
                   \   /            \ /
               Solution 1        Solution 2
                      \            /
                       \          /
                     Final Solution
```

### Key Characteristics

- **Recursive Nature**: Divide and conquer algorithms are typically implemented using recursion.
- **Non-overlapping Subproblems**: Unlike dynamic programming, the subproblems in divide and conquer usually don't overlap.
- **Independence**: Subproblems can often be solved independently of each other.
- **Combining Solutions**: The method for combining solutions is problem-specific and can range from simple to complex.

## 🧮 Mathematical Analysis of Divide and Conquer

### Recurrence Relations

Divide and conquer algorithms can be analyzed using recurrence relations. If T(n) represents the time complexity for a problem of size n, then a general form of the recurrence relation is:

```
T(n) = a * T(n/b) + f(n)
```

Where:

- `a` is the number of subproblems
- `b` is the factor by which the problem size is reduced
- `f(n)` is the cost of dividing the problem and combining the solutions

### Master Theorem

The Master Theorem provides a method to solve recurrence relations of the form above. It has three cases:

1. If f(n) = O(n^(log_b(a)-ε)) for some ε > 0, then T(n) = Θ(n^(log_b(a)))
2. If f(n) = Θ(n^(log_b(a))), then T(n) = Θ(n^(log_b(a)) \* log n)
3. If f(n) = Ω(n^(log_b(a)+ε)) for some ε > 0, and if a*f(n/b) ≤ c*f(n) for some c < 1 and sufficiently large n, then T(n) = Θ(f(n))

For example, in merge sort:

- a = 2 (two subproblems)
- b = 2 (size reduced by half)
- f(n) = Θ(n) (linear time to merge)

Thus: T(n) = 2T(n/2) + Θ(n)
Since log_b(a) = log_2(2) = 1, and f(n) = Θ(n^1), we're in case 2.
Therefore, T(n) = Θ(n log n), which matches our understanding of merge sort's time complexity.

## 🔍 Classic Divide and Conquer Algorithms

Let's examine some classic divide and conquer algorithms beyond the sorting algorithms we covered yesterday.

### Binary Search

While we've already covered binary search in Day 16, it's worth revisiting it as a prime example of divide and conquer.

**Divide**: Compare the target value with the middle element of the array.
**Conquer**: If the target matches the middle element, we're done. If the target is smaller, search the left subarray. If larger, search the right subarray.
**Combine**: Since we're just searching for an element, there's no need to combine results.

```cpp
int binarySearch(const std::vector<int>& arr, int target, int left, int right) {
    if (left > right) return -1; // Base case: element not found

    int mid = left + (right - left) / 2;

    if (arr[mid] == target)
        return mid; // Found the target
    else if (arr[mid] > target)
        return binarySearch(arr, target, left, mid - 1); // Search left half
    else
        return binarySearch(arr, target, mid + 1, right); // Search right half
}
```

**Time Complexity**: O(log n)
**Space Complexity**: O(log n) for the recursive call stack

### Strassen's Matrix Multiplication

The naive algorithm for multiplying two n×n matrices takes O(n³) time. Strassen's algorithm reduces this to approximately O(n^2.81).

**Divide**: Split each matrix into four n/2 × n/2 submatrices.
**Conquer**: Instead of the 8 multiplications required by the standard approach, use 7 cleverly designed multiplications.
**Combine**: Combine these 7 results to get the four quarters of the final matrix.

```cpp
// (Simplified version of Strassen's Algorithm)
std::vector<std::vector<int>> strassenMultiply(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) {

    int n = A.size();

    // Base case
    if (n <= 2) {
        // Perform standard matrix multiplication for small matrices
        std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }

    // Divide matrices into quarters
    int newSize = n / 2;
    std::vector<std::vector<int>> A11(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> A12(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> A21(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> A22(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B11(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B12(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B21(newSize, std::vector<int>(newSize));
    std::vector<std::vector<int>> B22(newSize, std::vector<int>(newSize));

    // Fill submatrices
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // Compute 7 products using divide and conquer
    auto M1 = strassenMultiply(matrixAdd(A11, A22), matrixAdd(B11, B22));
    auto M2 = strassenMultiply(matrixAdd(A21, A22), B11);
    auto M3 = strassenMultiply(A11, matrixSubtract(B12, B22));
    auto M4 = strassenMultiply(A22, matrixSubtract(B21, B11));
    auto M5 = strassenMultiply(matrixAdd(A11, A12), B22);
    auto M6 = strassenMultiply(matrixSubtract(A21, A11), matrixAdd(B11, B12));
    auto M7 = strassenMultiply(matrixSubtract(A12, A22), matrixAdd(B21, B22));

    // Combine results
    auto C11 = matrixAdd(matrixSubtract(matrixAdd(M1, M4), M5), M7);
    auto C12 = matrixAdd(M3, M5);
    auto C21 = matrixAdd(M2, M4);
    auto C22 = matrixAdd(matrixSubtract(matrixAdd(M1, M3), M2), M6);

    // Combine four quarters into a single result matrix
    std::vector<std::vector<int>> C(n, std::vector<int>(n));
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }

    return C;
}

// Helper function to add two matrices
std::vector<std::vector<int>> matrixAdd(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) {

    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}

// Helper function to subtract two matrices
std::vector<std::vector<int>> matrixSubtract(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) {

    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }

    return C;
}
```

**Time Complexity**: O(n^log₂(7)) ≈ O(n^2.81)
**Space Complexity**: O(n²) for storing the matrices

### Closest Pair of Points Problem

Given a set of points in 2D space, find the pair of points with the smallest Euclidean distance between them.

**Divide**: Sort points by x-coordinate, then divide the set into two equal-sized subsets.
**Conquer**: Recursively find the closest pairs in each subset.
**Combine**: Find the closest pair with one point in each subset. This requires special handling of points near the dividing line.

```cpp
// Structure to represent a point
struct Point {
    double x, y;

    // Constructor
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // Utility function to calculate squared distance
    double distanceSquared(const Point& p) const {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }
};

// Comparator for sorting points by x-coordinate
bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

// Comparator for sorting points by y-coordinate
bool compareY(const Point& a, const Point& b) {
    return a.y < b.y;
}

// Brute force method to find closest pair (used for small subsets)
std::pair<Point, Point> bruteForce(std::vector<Point>& points) {
    int n = points.size();
    double minDist = std::numeric_limits<double>::max();
    std::pair<Point, Point> closestPair;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = points[i].distanceSquared(points[j]);
            if (dist < minDist) {
                minDist = dist;
                closestPair = {points[i], points[j]};
            }
        }
    }

    return closestPair;
}

// Find the closest pair of points
std::pair<Point, Point> closestPair(std::vector<Point>& pointsX, std::vector<Point>& pointsY) {
    int n = pointsX.size();

    // Base case: use brute force for small sets
    if (n <= 3) {
        return bruteForce(pointsX);
    }

    // Divide the points into left and right halves
    int mid = n / 2;
    Point midPoint = pointsX[mid];

    std::vector<Point> leftX(pointsX.begin(), pointsX.begin() + mid);
    std::vector<Point> rightX(pointsX.begin() + mid, pointsX.end());

    std::vector<Point> leftY, rightY;
    for (const Point& p : pointsY) {
        if (p.x <= midPoint.x) {
            leftY.push_back(p);
        } else {
            rightY.push_back(p);
        }
    }

    // Recursively find the closest pairs in each half
    std::pair<Point, Point> leftPair = closestPair(leftX, leftY);
    std::pair<Point, Point> rightPair = closestPair(rightX, rightY);

    // Find the minimum distance from each half
    double leftDist = leftPair.first.distanceSquared(leftPair.second);
    double rightDist = rightPair.first.distanceSquared(rightPair.second);
    double minDist = std::min(leftDist, rightDist);
    std::pair<Point, Point> closestPair = (leftDist <= rightDist) ? leftPair : rightPair;

    // Find points in the strip around the middle line
    std::vector<Point> strip;
    for (const Point& p : pointsY) {
        if (std::abs(p.x - midPoint.x) < std::sqrt(minDist)) {
            strip.push_back(p);
        }
    }

    // Check for closer points in the strip
    for (int i = 0; i < strip.size(); i++) {
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < std::sqrt(minDist); j++) {
            double dist = strip[i].distanceSquared(strip[j]);
            if (dist < minDist) {
                minDist = dist;
                closestPair = {strip[i], strip[j]};
            }
        }
    }

    return closestPair;
}

// Main function to find closest pair
std::pair<Point, Point> findClosestPair(std::vector<Point>& points) {
    // Sort points by x and y coordinates
    std::vector<Point> pointsX = points;
    std::sort(pointsX.begin(), pointsX.end(), compareX);

    std::vector<Point> pointsY = points;
    std::sort(pointsY.begin(), pointsY.end(), compareY);

    return closestPair(pointsX, pointsY);
}
```

**Time Complexity**: O(n log n), dominated by the initial sorting
**Space Complexity**: O(n log n) for the recursive call stack and temporary arrays

### The Karatsuba Algorithm for Integer Multiplication

The standard algorithm for multiplying two n-digit numbers takes O(n²) time. Karatsuba's algorithm reduces this to approximately O(n^1.585).

**Divide**: Split each number into two halves.
**Conquer**: Perform three multiplications instead of four (as in the standard approach).
**Combine**: Combine the three results to get the final product.

```cpp
// Karatsuba algorithm for integer multiplication
long long karatsuba(long long x, long long y) {
    // Base case
    if (x < 10 || y < 10) {
        return x * y;
    }

    // Calculate the number of digits
    long long n = std::max(
        (long long)std::to_string(x).length(),
        (long long)std::to_string(y).length()
    );
    long long half = n / 2;

    // Split the digits
    long long power = (long long)std::pow(10, half);
    long long a = x / power;
    long long b = x % power;
    long long c = y / power;
    long long d = y % power;

    // Recursive steps (only 3 multiplications instead of 4)
    long long ac = karatsuba(a, c);
    long long bd = karatsuba(b, d);
    long long abcd = karatsuba(a + b, c + d) - ac - bd;

    // Combine results
    return ac * (long long)std::pow(10, 2 * half) + abcd * power + bd;
}
```

**Time Complexity**: O(n^log₂(3)) ≈ O(n^1.585)
**Space Complexity**: O(log n) for the recursive call stack

### Fast Fourier Transform (FFT)

The Fast Fourier Transform is a divide and conquer algorithm used to compute the discrete Fourier transform (DFT) of a sequence. It's widely used in signal processing and can also be applied to multiply large polynomials efficiently.

**Divide**: Split the sequence into even and odd indexed elements.
**Conquer**: Recursively compute the DFT of each half.
**Combine**: Combine the results using the butterfly operation.

While a complete implementation of FFT is beyond the scope of this document, it's worth mentioning as one of the most important applications of the divide and conquer paradigm.

**Time Complexity**: O(n log n), a significant improvement over the O(n²) naive approach
**Applications**: Signal processing, image processing, polynomial multiplication, convolution, etc.

## 🔄 Recursion vs. Iteration in Divide and Conquer

Most divide and conquer algorithms are naturally expressed using recursion, but they can often be rewritten in an iterative form.

### Advantages of Recursion

- Clearer expression of the divide and conquer paradigm
- Often leads to more elegant and readable code
- Directly maps to the mathematical recurrences

### Advantages of Iteration

- Avoids potential stack overflow for large inputs
- Generally more efficient (no function call overhead)
- More straightforward to analyze in terms of space complexity

### Example: Recursive vs. Iterative Binary Search

Recursive approach (as shown earlier):

```cpp
int binarySearch(const std::vector<int>& arr, int target, int left, int right) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;

    if (arr[mid] == target)
        return mid;
    else if (arr[mid] > target)
        return binarySearch(arr, target, left, mid - 1);
    else
        return binarySearch(arr, target, mid + 1, right);
}
```

Iterative approach:

```cpp
int binarySearchIterative(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
            return mid;
        else if (arr[mid] > target)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return -1;
}
```

Both approaches have O(log n) time complexity, but the iterative version has O(1) space complexity, while the recursive version uses O(log n) stack space.

## 🌎 Real-World Applications of Divide and Conquer

The divide and conquer paradigm extends far beyond academic examples. Here are some real-world applications:

### 1. Database Systems

- **Query Optimization**: Breaking complex queries into simpler subqueries
- **Index Structures**: B-trees and other search structures used in databases
- **Join Algorithms**: Merge-join and other join algorithms

### 2. Graphics and Image Processing

- **Rendering Complex Scenes**: Dividing scenes into manageable regions (e.g., quadtrees, octrees)
- **Image Compression**: Algorithms like JPEG use divide and conquer principles
- **Ray Tracing**: Breaking the scene into voxels or other spatial divisions

### 3. Machine Learning and Data Analysis

- **Decision Trees**: Recursively partitioning the feature space
- **K-means Clustering**: Recursive bisection methods
- **Principal Component Analysis**: Using eigendecomposition algorithms

### 4. Parallel Computing

- **MapReduce Framework**: Dividing large data processing tasks
- **Parallel Sorting Algorithms**: Distributed merge sort, parallel quicksort
- **Task Scheduling**: Dividing work among multiple processors

### 5. Telecommunications

- **Coding and Compression**: Huffman coding and other algorithms
- **Signal Processing**: Fast Fourier Transform for spectral analysis
- **Network Routing**: Hierarchical routing algorithms

## 🧠 Optimization Techniques for Divide and Conquer

### 1. Memoization

For divide and conquer problems where the same subproblems are solved multiple times, memoization can dramatically improve performance:

```cpp
std::unordered_map<int, int> memo;

int fibonacciMemoized(int n) {
    if (n <= 1) return n;

    // Check if we've already computed this value
    if (memo.find(n) != memo.end())
        return memo[n];

    // Otherwise, compute and store the result
    memo[n] = fibonacciMemoized(n - 1) + fibonacciMemoized(n - 2);
    return memo[n];
}
```

### 2. Tail Recursion Optimization

Rewriting recursive functions to use tail recursion can enable compiler optimizations:

```cpp
int factorialTail(int n, int accumulator = 1) {
    if (n <= 1) return accumulator;
    return factorialTail(n - 1, n * accumulator);
}
```

### 3. Hybrid Approaches

Combining divide and conquer with other techniques can lead to superior performance:

- Using insertion sort for small subarrays in merge sort/quicksort
- Combining divide and conquer with dynamic programming
- Switching between recursive and iterative approaches based on input size

## 😄 Divide and Conquer Jokes and Analogies

1. **The Recursive Restaurant**:
   "I asked the waiter at the Recursive Restaurant how they make their famous soup. He said, 'First, we make a smaller pot of the same soup, then we add a few ingredients.' When I asked how they make the smaller pot, he gave me the exact same answer."

2. **The Divide and Conquer Diet Plan**:
   "My new divide and conquer diet plan: cut the cake in half, eat one half, then recursively apply the same strategy to the remaining piece. Unfortunately, the time complexity is still O(entire cake)."

3. **The Binary Search First Date**:
   "Dating with binary search: start in the middle of your list of potential partners, eliminate half based on compatibility, and repeat until you find your match or end up alone with a runtime of O(log n) heartbreaks."

4. **The Merge Sort Family Reunion**:
   "At the Merge Sort family reunion, everyone splits into smaller and smaller groups until each person is alone, then gradually re-assembles in a perfectly ordered line for the group photo. Takes forever, but the picture is always sorted!"

5. **The Karatsuba Calculator**:
   "My calculator uses the Karatsuba algorithm. It doesn't give me the answer any faster, but it looks really impressive when it shows all the recursive steps on the display."

6. **The Programmer's Mountain Climbing Strategy**:
   "How programmers climb mountains: divide the mountain into smaller peaks, conquer each one recursively, then combine the experience into one epic LinkedIn post."

7. **The Divide and Conquer Relationship Counselor**:
   "Our relationship counselor uses the divide and conquer approach: separate all our problems into smaller issues, solve each one individually, then merge the solutions. Works great until we hit the O(n²) emotional complexity."

8. **The Binary Search Tree**:
   "A binary search tree walks into a bar. The bartender asks, 'What'll you have?' The tree says, 'I'll have what the left subtree is having if it's less than me, otherwise what the right subtree is having.'"

## 🚀 LeetCode Practice Problems

### Easy:

1. **Power of Two** (LeetCode #231)

   - Determine if a given integer is a power of two.

   ```cpp
   bool isPowerOfTwo(int n) {
       if (n <= 0) return false;

       // A power of two has only one bit set in its binary representation
       // n & (n-1) clears the rightmost set bit
       return (n & (n-1)) == 0;
   }
   ```

2. **Maximum Subarray** (LeetCode #53)

   - Find the contiguous subarray with the largest sum.

   ```cpp
   int maxSubArray(vector<int>& nums) {
       return maxSubArrayDivideConquer(nums, 0, nums.size() - 1);
   }

   int maxSubArrayDivideConquer(vector<int>& nums, int left, int right) {
       if (left == right) return nums[left];

       int mid = left + (right - left) / 2;

       // Find maximum subarray sum in left half
       int leftMax = maxSubArrayDivideConquer(nums, left, mid);

       // Find maximum subarray sum in right half
       int rightMax = maxSubArrayDivideConquer(nums, mid + 1, right);

       // Find maximum subarray sum that crosses the midpoint
       int crossMax = maxCrossingSum(nums, left, mid, right);

       // Return the maximum of the three
       return std::max({leftMax, rightMax, crossMax});
   }

   int maxCrossingSum(vector<int>& nums, int left, int mid, int right) {
       // Find maximum sum starting from mid and going left
       int leftSum = 0;
       int maxLeftSum = INT_MIN;
       for (int i = mid; i >= left; i--) {
           leftSum += nums[i];
           maxLeftSum = std::max(maxLeftSum, leftSum);
       }

       // Find maximum sum starting from mid+1 and going right
       int rightSum = 0;
       int maxRightSum = INT_MIN;
       for (int i = mid + 1; i <= right; i++) {
           rightSum += nums[i];
           maxRightSum = std::max(maxRightSum, rightSum);
       }

       // Return sum of the two halves
       return maxLeftSum + maxRightSum;
   }
   ```

3. **Majority Element** (LeetCode #169)

   - Find the majority element (appears more than n/2 times) in an array.

   ```cpp
   int majorityElement(vector<int>& nums) {
       return majorityElementDivideConquer(nums, 0, nums.size() - 1);
   }

   int majorityElementDivideConquer(vector<int>& nums, int left, int right) {
       // Base case: single element
       if (left == right) return nums[left];

       // Divide the array into two halves
       int mid = left + (right - left) / 2;
       int leftMajority = majorityElementDivideConquer(nums, left, mid);
       int rightMajority = majorityElementDivideConquer(nums, mid + 1, right);

       // If both halves agree on the majority element
       if (leftMajority == rightMajority) return leftMajority;

       // Count occurrences of the two candidates
       int leftCount = 0, rightCount = 0;
       for (int i = left; i <= right; i++) {
           if (nums[i] == leftMajority) leftCount++;
           else if (nums[i] == rightMajority) rightCount++;
       }

       // Return the element with more occurrences
       return leftCount > rightCount ? leftMajority : rightMajority;
   }
   ```

### Medium:

1. **Kth Largest Element in an Array** (LeetCode #215)

   - Find the kth largest element in an unsorted array.

   ```cpp
   int findKthLargest(vector<int>& nums, int k) {
       // Convert to 0-indexed
       return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
   }

   int quickSelect(vector<int>& nums, int left, int right, int k) {
       if (left == right) return nums[left];

       // Choose a random pivot to avoid worst-case scenario
       int pivotIndex = left + rand() % (right - left + 1);
       pivotIndex = partition(nums, left, right, pivotIndex);

       if (k == pivotIndex) return nums[k];
       else if (k < pivotIndex) return quickSelect(nums, left, pivotIndex - 1, k);
       else return quickSelect(nums, pivotIndex + 1, right, k);
   }

   int partition(vector<int>& nums, int left, int right, int pivotIndex) {
       int pivotValue = nums[pivotIndex];

       // Move pivot to the end
       swap(nums[pivotIndex], nums[right]);

       // Move all elements smaller than pivot to the left
       int storeIndex = left;
       for (int i = left; i < right; i++) {
           if (nums[i] < pivotValue) {
               swap(nums[storeIndex], nums[i]);
               storeIndex++;
           }
       }

       // Move pivot to its final place
       swap(nums[storeIndex], nums[right]);
       return storeIndex;
   }
   ```

2. **Different Ways to Add Parentheses** (LeetCode #241)

   - Given a string of numbers and operators, compute all possible results by adding parentheses.

   ```cpp
   vector<int> diffWaysToCompute(string expression) {
       vector<int> results;
       bool isNumber = true;

       // Check if the input is just a number
       for (char c : expression) {
           if (!isdigit(c)) {
               isNumber = false;
               break;
           }
       }

       // Base case: just a number
       if (isNumber) {
           results.push_back(stoi(expression));
           return results;
       }

       // Recursive case
       for (int i = 0; i < expression.size(); i++) {
           char c = expression[i];
           if (c == '+' || c == '-' || c == '*') {
               // Split the expression at this operator
               vector<int> leftResults = diffWaysToCompute(expression.substr(0, i));
               vector<int> rightResults = diffWaysToCompute(expression.substr(i + 1));

               // Combine the results from left and right
               for (int left : leftResults) {
                   for (int right : rightResults) {
                       if (c == '+') results.push_back(left + right);
                       else if (c == '-') results.push_back(left - right);
                       else if (c == '*') results.push_back(left * right);
                   }
               }
           }
       }

       return results;
   }
   ```

3. **Search a 2D Matrix II** (LeetCode #240)

   - Search for a target value in a 2D matrix where each row and column is sorted.

   ```cpp
   bool searchMatrix(vector<vector<int>>& matrix, int target) {
       // Start from the top-right corner
       int row = 0;
       int col = matrix[0].size() - 1;

       while (row < matrix.size() && col >= 0) {
           if (matrix[row][col] == target) {
               return true;
           } else if (matrix[row][col] > target) {
               // The current column is too large, move left
               col--;
           } else {
               // The current row is too small, move down
               row++;
           }
       }

       return false;
   }
   ```

### Hard:

1. **Count of Smaller Numbers After Self** (LeetCode #315)

   - Given an array, count for each element how many smaller elements appear to its right.

   ```cpp
   vector<int> countSmaller(vector<int>& nums) {
       vector<int> indices(nums.size());
       for (int i = 0; i < nums.size(); i++) {
           indices[i] = i;
       }

       vector<int> result(nums.size(), 0);
       mergeSort(nums, indices, result, 0, nums.size() - 1);

       return result;
   }

   void mergeSort(vector<int>& nums, vector<int>& indices, vector<int>& result, int start, int end) {
       if (start >= end) return;

       int mid = start + (end - start) / 2;
       mergeSort(nums, indices, result, start, mid);
       mergeSort(nums, indices, result, mid + 1, end);

       // Count smaller elements and merge
       merge(nums, indices, result, start, mid, end);
   }

   void merge(vector<int>& nums, vector<int>& indices, vector<int>& result, int start, int mid, int end) {
       vector<int> temp(end - start + 1);
       int left = start, right = mid + 1;
       int idx = 0;

       while (left <= mid && right <= end) {
           if (nums[indices[left]] <= nums[indices[right]]) {
               // If element on left is smaller or equal, it has 'right - (mid + 1)' smaller elements on the right
               result[indices[left]] += right - (mid + 1);
               temp[idx++] = indices[left++];
           } else {
               temp[idx++] = indices[right++];
           }
       }

       // Handle remaining elements
       while (left <= mid) {
           result[indices[left]] += right - (mid + 1);
           temp[idx++] = indices[left++];
       }
       while (right <= end) {
           temp[idx++] = indices[right++];
       }

       // Copy back to indices
       for (int i = 0; i < temp.size(); i++) {
           indices[start + i] = temp[i];
       }
   }
   ```

2. **Median of Two Sorted Arrays** (LeetCode #4)

   - Find the median of two sorted arrays.

   ```cpp
   double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
       // Ensure nums1 is the smaller array
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

           // If partitionX is 0, there's nothing on the left side
           int maxX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
           // If partitionX is x, there's nothing on the right side
           int minX = (partitionX == x) ? INT_MAX : nums1[partitionX];

           int maxY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
           int minY = (partitionY == y) ? INT_MAX : nums2[partitionY];

           if (maxX <= minY && maxY <= minX) {
               // We have the correct partition
               // Now find the median based on the total length of the arrays
               if ((x + y) % 2 == 0) {
                   return (std::max(maxX, maxY) + std::min(minX, minY)) / 2.0;
               } else {
                   return std::max(maxX, maxY);
               }
           } else if (maxX > minY) {
               // Move toward the left in nums1
               high = partitionX - 1;
           } else {
               // Move toward the right in nums1
               low = partitionX + 1;
           }
       }

       // If we reach here, the input arrays are not sorted
       throw invalid_argument("Input arrays are not sorted");
   }
   ```

3. **The Skyline Problem** (LeetCode #218)

   - Given a set of buildings, output the skyline formed by these buildings.

   ```cpp
   vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
       vector<vector<int>> result;

       // Edge case
       if (buildings.empty()) return result;

       // Use divide and conquer
       return divideAndConquer(buildings, 0, buildings.size() - 1);
   }

   vector<vector<int>> divideAndConquer(vector<vector<int>>& buildings, int start, int end) {
       // Base case: single building
       if (start == end) {
           vector<vector<int>> skyline;
           skyline.push_back({buildings[start][0], buildings[start][2]});
           skyline.push_back({buildings[start][1], 0});
           return skyline;
       }

       // Divide the buildings into two halves
       int mid = start + (end - start) / 2;
       auto skyline1 = divideAndConquer(buildings, start, mid);
       auto skyline2 = divideAndConquer(buildings, mid + 1, end);

       // Merge the two skylines
       return mergeSkylines(skyline1, skyline2);
   }

   vector<vector<int>> mergeSkylines(vector<vector<int>>& skyline1, vector<vector<int>>& skyline2) {
       vector<vector<int>> result;
       int h1 = 0, h2 = 0; // Current heights
       int i = 0, j = 0;   // Current indices

       while (i < skyline1.size() && j < skyline2.size()) {
           int x, height;

           // Choose the point with smaller x-coordinate
           if (skyline1[i][0] < skyline2[j][0]) {
               x = skyline1[i][0];
               h1 = skyline1[i][1];
               i++;
           } else if (skyline1[i][0] > skyline2[j][0]) {
               x = skyline2[j][0];
               h2 = skyline2[j][1];
               j++;
           } else {
               x = skyline1[i][0];
               h1 = skyline1[i][1];
               h2 = skyline2[j][1];
               i++; j++;
           }

           // Calculate the max height at this point
           height = std::max(h1, h2);

           // Add to result if the height changes
           if (result.empty() || result.back()[1] != height) {
               result.push_back({x, height});
           }
       }

       // Add remaining points from skyline1
       while (i < skyline1.size()) {
           result.push_back(skyline1[i]);
           i++;
       }

       // Add remaining points from skyline2
       while (j < skyline2.size()) {
           result.push_back(skyline2[j]);
           j++;
       }

       return result;
   }
   ```

## 🔮 Tomorrow's Preview

Tomorrow, we'll explore the fascinating world of **Dynamic Programming**. We'll learn:

- How dynamic programming differs from divide and conquer
- Top-down (memoization) vs. bottom-up (tabulation) approaches
- How to recognize problems suitable for dynamic programming
- Classic dynamic programming problems and solutions
- Optimizing dynamic programming solutions for space and time

_If divide and conquer is about breaking problems into independent subproblems, dynamic programming is about breaking them into overlapping subproblems and avoiding redundant work._

#DSAin45 #Day19 #DivideAndConquer #AlgorithmicParadigms #CPlusPlus #DataStructures #Algorithms #RecursiveAlgorithms #MasterTheorem #BinarySearch #CodingInterview
