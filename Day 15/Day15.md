# 🔄 DAY 15: Recursion Fundamentals - The Art of Self-Reference #DSAin45

Welcome to Week 4 of our 45-day DSA journey! After building our foundation with data structures in the first three weeks, we're now ready to tackle more advanced algorithmic concepts, starting with recursion—a powerful technique that forms the backbone of numerous elegant algorithms.

_Like the mythical Ouroboros serpent eating its own tail, recursion creates solutions by referring back to itself, allowing complex problems to be solved through self-reference._

## 🧠 Understanding Recursion

**Recursion** is a problem-solving approach where a function calls itself to solve smaller instances of the same problem until reaching a simple enough case (base case) that can be solved directly.

Every recursive solution consists of two essential parts:

1. **Base Case(s)**: The terminating scenario(s) that can be answered without recursion
2. **Recursive Case(s)**: The scenario(s) where the function calls itself with a simpler version of the problem

### The Key to Understanding Recursion

_The key to understanding recursion is trusting that your function will correctly solve the smaller subproblems without worrying about how it does it._

Think of recursion as a form of "wishful thinking" or "divine intervention":

```
To solve Problem(n):
  If n is simple enough (base case):
    Solve directly
  Else:
    Assume you ALREADY have a magical solution for Problem(n-1)
    Use that solution to construct the solution for Problem(n)
```

## 📚 The Call Stack

When your program calls a function, the computer creates a new **stack frame** that stores:

- The function's parameters
- Local variables
- The return address (where to continue after the function completes)

This frame gets pushed onto a region of memory called the **call stack**. When the function finishes, its frame is popped off the stack, and execution resumes from the return address.

With recursion, we have multiple frames of the same function on the stack simultaneously:

```cpp
int factorial(int n) {
    // Base case
    if (n <= 1) return 1;

    // Recursive case
    return n * factorial(n - 1);
}
```

When calculating `factorial(5)`, the call stack would look like:

```
// Initial call
factorial(5)
  // Creates recursive call
  factorial(4)
    // Creates recursive call
    factorial(3)
      // Creates recursive call
      factorial(2)
        // Creates recursive call
        factorial(1)
          // Base case reached - returns 1
        // Returns 1 * 2 = 2
      // Returns 2 * 3 = 6
    // Returns 6 * 4 = 24
  // Returns 24 * 5 = 120
```

### Stack Frame Visualization

Each stack frame for `factorial(n)` contains:

- Parameter `n`
- Return address
- Space for local variables

As we make recursive calls, the stack grows:

| Stack (grows downward) | n   | Return Value  |
| ---------------------- | --- | ------------- |
| factorial(1) [Base]    | 1   | 1             |
| factorial(2)           | 2   | 2 \* 1 = 2    |
| factorial(3)           | 3   | 3 \* 2 = 6    |
| factorial(4)           | 4   | 4 \* 6 = 24   |
| factorial(5) [Initial] | 5   | 5 \* 24 = 120 |

Once we hit the base case, the stack unwinds, calculating return values along the way.

## 🧮 Simple Recursion Examples

### Example 1: Factorial

```cpp
int factorial(int n) {
    // Base case
    if (n <= 1) {
        return 1;
    }

    // Recursive case
    return n * factorial(n - 1);
}
```

### Example 2: Fibonacci Sequence

```cpp
int fibonacci(int n) {
    // Base cases
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // Recursive case
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

### Example 3: Sum of Array Elements

```cpp
int sumArray(const std::vector<int>& arr, int index = 0) {
    // Base case
    if (index >= arr.size()) {
        return 0;
    }

    // Recursive case
    return arr[index] + sumArray(arr, index + 1);
}
```

## 🔄 Recursion vs. Iteration

Many problems can be solved using either recursion or iteration. Consider the pros and cons of each:

### Factorial: Recursive vs. Iterative

**Recursive Solution**:

```cpp
int factorialRecursive(int n) {
    if (n <= 1) return 1;
    return n * factorialRecursive(n - 1);
}
```

**Iterative Solution**:

```cpp
int factorialIterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
```

### Comparison

| Aspect              | Recursion                                                    | Iteration                               |
| ------------------- | ------------------------------------------------------------ | --------------------------------------- |
| Code Simplicity     | Often more elegant and closer to the mathematical definition | Sometimes more verbose                  |
| Memory Usage        | Uses more memory (call stack)                                | Typically uses less memory              |
| Performance         | May be slower due to function call overhead                  | Generally faster                        |
| Stack Overflow Risk | Yes, with deep recursion                                     | Rare                                    |
| Problem Suitability | Trees, graphs, divide & conquer                              | Linear processes, loop-based operations |

_Recursion is like a sophisticated Swiss watch—elegant, beautiful, but with more moving parts that can break. Iteration is like a digital watch—perhaps less elegant but often more practical._

## 🌲 Types of Recursion

### 1. Linear Recursion (Single Recursion)

A function makes a single recursive call in each execution. Examples: factorial, sum of array.

```cpp
int linearRecursion(int n) {
    if (n <= 0) return baseValue;
    return operation(n, linearRecursion(n-1));
}
```

### 2. Binary Recursion (Multiple Recursion)

A function makes two recursive calls in each execution. Example: Fibonacci.

```cpp
int binaryRecursion(int n) {
    if (base case) return baseValue;
    return operation(binaryRecursion(n-1), binaryRecursion(n-2));
}
```

### 3. Tail Recursion

A special form where the recursive call is the last operation in the function. It can be optimized by compilers.

```cpp
int tailFactorial(int n, int accumulator = 1) {
    if (n <= 1) return accumulator;
    return tailFactorial(n - 1, n * accumulator);
}
```

### 4. Mutual Recursion

Two or more functions call each other in a cycle.

```cpp
bool isEven(int n) {
    if (n == 0) return true;
    return isOdd(n - 1);
}

bool isOdd(int n) {
    if (n == 0) return false;
    return isEven(n - 1);
}
```

## 🔍 Common Recursive Patterns

### 1. Divide and Conquer

Break a problem into smaller, more manageable subproblems, solve them recursively, then combine the results.

Example: Merge Sort

```cpp
std::vector<int> mergeSort(std::vector<int>& arr) {
    // Base case: single element is already sorted
    if (arr.size() <= 1) return arr;

    // Divide
    int mid = arr.size() / 2;
    std::vector<int> left(arr.begin(), arr.begin() + mid);
    std::vector<int> right(arr.begin() + mid, arr.end());

    // Conquer (recursively sort subarrays)
    left = mergeSort(left);
    right = mergeSort(right);

    // Combine (merge sorted subarrays)
    return merge(left, right);
}
```

### 2. Backtracking

Build a solution incrementally, abandoning a partial solution as soon as it's determined invalid.

Example: N-Queens Problem (simplified)

```cpp
bool solveNQueens(std::vector<int>& board, int col, int n) {
    // Base case: all queens placed
    if (col >= n) return true;

    // Try placing a queen in each row of the current column
    for (int row = 0; row < n; row++) {
        if (isSafe(board, row, col, n)) {
            // Place queen
            board[row * n + col] = 1;

            // Recursively try to place queens in remaining columns
            if (solveNQueens(board, col + 1, n))
                return true;

            // If placing queen here didn't work, remove it (backtrack)
            board[row * n + col] = 0;
        }
    }

    // Couldn't place a queen in any row of this column
    return false;
}
```

### 3. Dynamic Programming (Memoization)

Cache previously computed results to avoid redundant calculations.

Example: Fibonacci with Memoization

```cpp
int fibMemoized(int n, std::unordered_map<int, int>& memo) {
    // Check memo first
    if (memo.find(n) != memo.end()) {
        return memo[n];
    }

    // Base cases
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // Recursively compute and store result
    memo[n] = fibMemoized(n - 1, memo) + fibMemoized(n - 2, memo);
    return memo[n];
}
```

## 🚧 Recursion Pitfalls and How to Avoid Them

### 1. Stack Overflow

Deep recursion can exceed the call stack size limit.

**Prevention**:

- Ensure your base case is reachable
- Consider tail recursion
- For very deep recursion, convert to iteration

```cpp
// Dangerous - may cause stack overflow with large n
void recursiveFunction(int n) {
    if (n < 0) return; // Base case unreachable for positive n
    recursiveFunction(n);
}
```

### 2. Redundant Computation

Naïve implementations can repeat calculations exponentially.

**Prevention**:

- Use memoization
- Consider iterative approaches for simple problems

```cpp
// Inefficient - many redundant calculations
int fibNaive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibNaive(n - 1) + fibNaive(n - 2); // Both branches recompute overlapping values
}
```

### 3. Infinite Recursion

Missing or incorrect base case leads to infinite recursion.

**Prevention**:

- Always explicitly define and test base cases
- Ensure recursive steps move toward the base case

```cpp
// Infinite recursion - no base case
int badFactorial(int n) {
    return n * badFactorial(n - 1);
}
```

## 💡 Tail Recursion Optimization

Tail recursion is a special case where the recursive call is the last operation in the function. Modern compilers can optimize tail-recursive functions to avoid creating new stack frames, essentially converting them to iteration.

### Non-Tail-Recursive vs. Tail-Recursive Factorial

**Non-Tail-Recursive**:

```cpp
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1); // Must wait for recursive call to complete before multiplying
}
```

**Tail-Recursive**:

```cpp
int factorialTail(int n, int accumulator = 1) {
    if (n <= 1) return accumulator;
    return factorialTail(n - 1, n * accumulator); // Final action is the recursive call
}
```

In the tail-recursive version, we pass the partial result as an accumulator parameter, so no computation needs to be done after the recursive call returns.

### Optimization Process

When a compiler optimizes tail recursion:

1. It transforms the recursion into a loop
2. It reuses the same stack frame
3. It updates parameters in place

```cpp
// Compiler might transform this
int factorialTail(int n, int accumulator = 1) {
    if (n <= 1) return accumulator;
    return factorialTail(n - 1, n * accumulator);
}

// Into something like this
int factorialTailOptimized(int n, int accumulator = 1) {
    while (n > 1) {
        accumulator = n * accumulator;
        n = n - 1;
    }
    return accumulator;
}
```

Note: C++ does not guarantee tail-call optimization like some functional languages do. Always test on your specific compiler and platform if you're relying on this optimization.

## 🧩 Solving Complex Problems with Recursion

Let's tackle some more complex problems to demonstrate recursive problem-solving:

### 1. Binary Tree Traversal

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// In-order traversal (left, root, right)
void inorderTraversal(TreeNode* root, std::vector<int>& result) {
    if (!root) return;

    inorderTraversal(root->left, result);  // Left
    result.push_back(root->val);           // Root
    inorderTraversal(root->right, result); // Right
}

// Pre-order traversal (root, left, right)
void preorderTraversal(TreeNode* root, std::vector<int>& result) {
    if (!root) return;

    result.push_back(root->val);           // Root
    preorderTraversal(root->left, result); // Left
    preorderTraversal(root->right, result); // Right
}

// Post-order traversal (left, right, root)
void postorderTraversal(TreeNode* root, std::vector<int>& result) {
    if (!root) return;

    postorderTraversal(root->left, result);  // Left
    postorderTraversal(root->right, result); // Right
    result.push_back(root->val);             // Root
}
```

### 2. Tower of Hanoi

The classic recursive problem: Move all disks from one rod to another, using a third rod as auxiliary, following these rules:

- Only one disk can be moved at a time
- A larger disk cannot be placed on top of a smaller disk

```cpp
void towerOfHanoi(int n, char source, char auxiliary, char target) {
    // Base case: no disk to move
    if (n == 0) return;

    // Move n-1 disks from source to auxiliary (using target as auxiliary)
    towerOfHanoi(n - 1, source, target, auxiliary);

    // Move the largest disk from source to target
    std::cout << "Move disk " << n << " from " << source << " to " << target << std::endl;

    // Move n-1 disks from auxiliary to target (using source as auxiliary)
    towerOfHanoi(n - 1, auxiliary, source, target);
}
```

### 3. Generating Permutations

Generate all possible permutations of a given array:

```cpp
void generatePermutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result) {
    // Base case: reached the end of array
    if (start == nums.size() - 1) {
        result.push_back(nums);
        return;
    }

    // Try each element at the current position
    for (int i = start; i < nums.size(); i++) {
        // Swap to put nums[i] at position start
        std::swap(nums[start], nums[i]);

        // Recursively generate permutations for the rest of the array
        generatePermutations(nums, start + 1, result);

        // Backtrack - restore array to original state
        std::swap(nums[start], nums[i]);
    }
}
```

## 🚀 LeetCode Practice Problems

### Easy:

1. **Reverse String** (LeetCode #344)

   - Write a function that reverses a string using recursion.

   ```cpp
   void reverseString(std::vector<char>& s, int left = 0, int right = -1) {
       if (right == -1) right = s.size() - 1; // Initialize right if not set

       // Base case: pointers meet in the middle
       if (left >= right) return;

       // Swap characters and recurse
       std::swap(s[left], s[right]);
       reverseString(s, left + 1, right - 1);
   }
   ```

2. **Power of Three** (LeetCode #326)

   - Determine if an integer is a power of three.

   ```cpp
   bool isPowerOfThree(int n) {
       // Base cases
       if (n <= 0) return false;
       if (n == 1) return true;

       // Check if divisible by 3
       if (n % 3 != 0) return false;

       // Recursive case
       return isPowerOfThree(n / 3);
   }
   ```

3. **Merge Two Sorted Lists** (LeetCode #21)

   - Merge two sorted linked lists recursively.

   ```cpp
   ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
       // Base cases
       if (!l1) return l2;
       if (!l2) return l1;

       // Recursive case
       if (l1->val <= l2->val) {
           l1->next = mergeTwoLists(l1->next, l2);
           return l1;
       } else {
           l2->next = mergeTwoLists(l1, l2->next);
           return l2;
       }
   }
   ```

### Medium:

1. **Generate Parentheses** (LeetCode #22)

   - Generate all valid combinations of n pairs of parentheses.

   ```cpp
   void backtrack(std::vector<std::string>& result, std::string current, int open, int close, int max) {
       // Base case: string is complete
       if (current.length() == max * 2) {
           result.push_back(current);
           return;
       }

       // Add open parenthesis if we haven't used all
       if (open < max) {
           backtrack(result, current + "(", open + 1, close, max);
       }

       // Add close parenthesis if it won't create an invalid sequence
       if (close < open) {
           backtrack(result, current + ")", open, close + 1, max);
       }
   }

   std::vector<std::string> generateParenthesis(int n) {
       std::vector<std::string> result;
       backtrack(result, "", 0, 0, n);
       return result;
   }
   ```

2. **Subsets** (LeetCode #78)

   - Generate all possible subsets of a set of distinct integers.

   ```cpp
   void generateSubsets(std::vector<int>& nums, int index, std::vector<int>& current, std::vector<std::vector<int>>& result) {
       // Base case: reached the end of nums
       if (index == nums.size()) {
           result.push_back(current);
           return;
       }

       // Don't include nums[index]
       generateSubsets(nums, index + 1, current, result);

       // Include nums[index]
       current.push_back(nums[index]);
       generateSubsets(nums, index + 1, current, result);
       current.pop_back(); // Backtrack
   }

   std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
       std::vector<std::vector<int>> result;
       std::vector<int> current;
       generateSubsets(nums, 0, current, result);
       return result;
   }
   ```

### Hard:

1. **N-Queens** (LeetCode #51)

   - Place N queens on an N×N chessboard so no two queens attack each other.

   ```cpp
   bool isSafe(std::vector<std::string>& board, int row, int col, int n) {
       // Check row to the left
       for (int i = 0; i < col; i++) {
           if (board[row][i] == 'Q') return false;
       }

       // Check upper diagonal
       for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
           if (board[i][j] == 'Q') return false;
       }

       // Check lower diagonal
       for (int i = row, j = col; i < n && j >= 0; i++, j--) {
           if (board[i][j] == 'Q') return false;
       }

       return true;
   }

   void solveNQueensBacktrack(std::vector<std::string>& board, int col, int n, std::vector<std::vector<std::string>>& solutions) {
       // Base case: all queens placed
       if (col == n) {
           solutions.push_back(board);
           return;
       }

       // Try placing queen in each row of the current column
       for (int row = 0; row < n; row++) {
           if (isSafe(board, row, col, n)) {
               // Place queen
               board[row][col] = 'Q';

               // Recursively place remaining queens
               solveNQueensBacktrack(board, col + 1, n, solutions);

               // Backtrack
               board[row][col] = '.';
           }
       }
   }

   std::vector<std::vector<std::string>> solveNQueens(int n) {
       std::vector<std::vector<std::string>> solutions;
       std::vector<std::string> board(n, std::string(n, '.'));

       solveNQueensBacktrack(board, 0, n, solutions);

       return solutions;
   }
   ```

2. **Regular Expression Matching** (LeetCode #10)

   - Implement regular expression matching with support for '.' and '\*'.

   ```cpp
   bool isMatch(std::string s, std::string p) {
       // Base case: empty pattern
       if (p.empty()) return s.empty();

       // Check first character match
       bool firstMatch = !s.empty() && (p[0] == s[0] || p[0] == '.');

       // If pattern has a '*', we have two options:
       // 1. Ignore the pattern character and its '*' (zero occurrence)
       // 2. Use the pattern character for the current string character (if they match)
       if (p.length() >= 2 && p[1] == '*') {
           return isMatch(s, p.substr(2)) || // Case 1: Ignore pattern char and '*'
                 (firstMatch && isMatch(s.substr(1), p)); // Case 2: Use pattern char
       } else {
           // No '*', just check if first chars match and recurse
           return firstMatch && isMatch(s.substr(1), p.substr(1));
       }
   }
   ```

## 🧠 Recursion Jokes & Analogies

1. **The Recursion Definition**:
   To understand recursion, you must first understand recursion.

2. **The Programmer's Dilemma**:
   Why do programmers confuse recursion and iteration?
   Because both involve doing the same thing over and over, but recursion means someone else does it for you.

3. **The Programmer's Prayer**:
   "Please, God, let my base case be reachable and my recursive step move toward it."

4. **The Manager Analogy**:
   Recursion is like a manager who breaks a task into smaller tasks, delegates most of it to their clone, and then integrates the results with their small contribution.

5. **The Russian Doll Analogy**:
   Recursion is like a set of Russian nested dolls. You keep opening dolls until you find the smallest one (the base case), then you put them back together as you return.

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive into **Searching Algorithms**. We'll explore various techniques for finding elements in data structures, including:

- Linear search for arrays and linked lists
- Binary search for sorted arrays
- Jump search and interpolation search
- Binary search trees
- Comparison of search algorithm efficiencies

Get ready to discover how efficient searching can dramatically improve your program's performance when dealing with large datasets!

_Think of today's recursion as learning to create a map, while tomorrow's searching algorithms will teach you to navigate that map efficiently._

#DSAin45 #Day15 #Recursion #CPlusPlus #DataStructures #Algorithms #CodingInterview
