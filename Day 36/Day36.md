# Day 36: Dynamic Programming - Part 1 #DSAin45

## Introduction

Welcome to Day 36 of our #DSAin45 journey! Yesterday, we tackled challenging graph problems and explored powerful optimization techniques like network flow, bipartite matching, and the traveling salesman problem. Today, we begin our exploration of one of the most elegant and powerful algorithm design paradigms in computer science: Dynamic Programming (DP).

Dynamic Programming is a method for solving complex problems by breaking them down into simpler subproblems, solving each subproblem just once, and storing their solutions. It's particularly useful for optimization problems where we need to find the best solution among many possible options. The beauty of dynamic programming lies in its ability to dramatically reduce time complexity by avoiding redundant calculations—transforming exponential algorithms into polynomial ones.

Let's embark on this DP journey, which we'll continue tomorrow in Part 2 when we tackle more advanced problems!

## Core Concepts: Dynamic Programming Principles

### What is Dynamic Programming?

Dynamic Programming is an algorithmic paradigm that solves a complex problem by breaking it down into simpler subproblems and storing the results of these subproblems to avoid redundant computations. The term was coined by mathematician Richard Bellman in the 1950s.

For a problem to be suitable for DP, it must have two key properties:

1. **Optimal Substructure**: An optimal solution to the problem contains optimal solutions to its subproblems.
2. **Overlapping Subproblems**: The problem can be broken down into subproblems that are reused multiple times.

### The Dynamic Programming Process

The general approach to solving a problem with dynamic programming involves:

1. **Identify**: Determine if the problem has optimal substructure and overlapping subproblems.
2. **Define**: Formulate a recursive definition of the problem in terms of smaller subproblems.
3. **Compute**: Calculate the solution for all subproblems, typically in a bottom-up manner.
4. **Construct**: Build the solution to the original problem from the solutions to the subproblems.

### Memoization vs. Tabulation

There are two primary approaches to implement dynamic programming:

#### Memoization (Top-Down Approach)

Memoization is a top-down approach where we start from the original problem and recursively break it down into subproblems. To avoid redundant calculations, we store (or "memoize") the results of subproblems.

Characteristics:

- Uses recursion with a "memo" data structure (typically a hash map or array)
- Solves problems in a depth-first manner
- Computes only the needed subproblems
- Often easier to implement as it follows the natural recursive structure

```cpp
// Memoization example for Fibonacci
int fibMemo(int n, vector<int>& memo) {
    // Base cases
    if (n <= 1) return n;

    // If already computed, return stored result
    if (memo[n] != -1) return memo[n];

    // Compute and store result
    memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo);
    return memo[n];
}

int fibonacci(int n) {
    vector<int> memo(n+1, -1);
    return fibMemo(n, memo);
}
```

#### Tabulation (Bottom-Up Approach)

Tabulation is a bottom-up approach where we solve all subproblems first, starting from the smallest ones, and use them to build solutions for progressively larger problems.

Characteristics:

- Uses iteration and a "table" (typically an array or matrix)
- Solves problems in a breadth-first manner
- Computes all subproblems systematically
- Often more efficient in terms of memory management and stack overflow prevention

```cpp
// Tabulation example for Fibonacci
int fibonacci(int n) {
    if (n <= 1) return n;

    vector<int> dp(n+1);
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }

    return dp[n];
}
```

### When to Choose Memoization vs. Tabulation?

| Consideration             | Memoization                                              | Tabulation                                   |
| ------------------------- | -------------------------------------------------------- | -------------------------------------------- |
| Implementation complexity | Often simpler (follows recursive structure)              | May require careful ordering of computations |
| Memory usage              | Can be more efficient (computes only needed subproblems) | Stores solutions for all subproblems         |
| Stack overflow risk       | Higher (due to recursion)                                | Lower (iterative approach)                   |
| Speed                     | Potential overhead from recursion                        | Often faster due to lack of recursive calls  |
| Subproblem ordering       | Handled automatically                                    | Must be manually determined                  |

## Classic DP Problems

### The Fibonacci Sequence

The Fibonacci sequence is defined as: F(n) = F(n-1) + F(n-2) with base cases F(0) = 0 and F(1) = 1.

A naive recursive approach would lead to exponential time complexity:

```cpp
int fibNaive(int n) {
    if (n <= 1) return n;
    return fibNaive(n-1) + fibNaive(n-2);
}
```

This has a time complexity of O(2^n) because each call branches into two recursive calls, creating a binary tree of height n.

With dynamic programming, we can reduce this to O(n):

```cpp
// Space-optimized tabulation
int fibonacci(int n) {
    if (n <= 1) return n;

    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}
```

This optimization reduces the space complexity from O(n) to O(1) by only storing the two most recent Fibonacci numbers.

### Coin Change Problem

**Problem**: Given a set of coin denominations and a target amount, find the minimum number of coins needed to make that amount.

This is a classic DP problem because:

1. It has optimal substructure: The minimum coins to make amount n using the first k coin types depends on optimal solutions to smaller subproblems.
2. It has overlapping subproblems: Many recursive calls compute the same subproblems.

#### Recursive Definition

Let dp[i] be the minimum number of coins needed to make amount i.

Base case: dp[0] = 0 (zero coins needed to make amount 0)

For i > 0, dp[i] = min(dp[i-coin] + 1) for all coins that don't exceed i.

#### Implementation with Tabulation

```cpp
int coinChange(vector<int>& coins, int amount) {
    // Initialize dp array with a value larger than any possible solution
    vector<int> dp(amount + 1, amount + 1);

    // Base case: 0 coins needed to make amount 0
    dp[0] = 0;

    // Fill the dp array bottom-up
    for (int i = 1; i <= amount; i++) {
        // Try each coin denomination
        for (int coin : coins) {
            if (coin <= i) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }

    // If dp[amount] is still amount+1, it means no solution exists
    return dp[amount] > amount ? -1 : dp[amount];
}
```

Time complexity: O(amount \* n), where n is the number of coin denominations.
Space complexity: O(amount).

### Climbing Stairs Problem

**Problem**: You are climbing a staircase with n steps. Each time you can climb either 1 or 2 steps. How many distinct ways can you climb to the top?

This problem has a recursive structure: the number of ways to reach step n is the sum of the number of ways to reach step n-1 and step n-2 (since those are the steps from which you can reach step n).

#### Recursive Definition

Let dp[i] be the number of ways to reach step i.

Base cases:

- dp[1] = 1 (one way to reach step 1)
- dp[2] = 2 (two ways to reach step 2: 1+1 or 2)

For i > 2, dp[i] = dp[i-1] + dp[i-2]

#### Implementation with Tabulation

```cpp
int climbStairs(int n) {
    if (n <= 2) return n;

    vector<int> dp(n+1);
    dp[1] = 1;
    dp[2] = 2;

    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }

    return dp[n];
}
```

Space-optimized version:

```cpp
int climbStairs(int n) {
    if (n <= 2) return n;

    int prev = 1;
    int curr = 2;

    for (int i = 3; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}
```

Time complexity: O(n)
Space complexity: O(1) for the optimized version

## Visual Explanations of Algorithms

### The Fibonacci Sequence Visualization

To understand the power of dynamic programming, let's visualize the computation of the Fibonacci sequence:

#### Naive Recursive Approach

```
                      fib(5)
                    /        \
               fib(4)         fib(3)
              /      \        /     \
        fib(3)        fib(2)  fib(2) fib(1)
        /    \        /    \  /    \
    fib(2)  fib(1) fib(1) fib(0) fib(1) fib(0)
    /    \
fib(1) fib(0)
```

Notice how fib(3) is computed twice, fib(2) is computed three times, and fib(1) is computed five times. This redundancy leads to exponential time complexity.

#### Memoized Approach

With memoization, once we compute fib(k), we store it and reuse it whenever needed:

```
Step 1: Compute fib(5)
  └─ Need fib(4) and fib(3)
     └─ First, compute fib(4)
        └─ Need fib(3) and fib(2)
           └─ First, compute fib(3)
              └─ Need fib(2) and fib(1)
                 └─ First, compute fib(2)
                    └─ Need fib(1) and fib(0)
                       └─ fib(1) = 1 (base case)
                       └─ fib(0) = 0 (base case)
                    └─ Store: fib(2) = 1
                 └─ fib(1) = 1 (base case)
              └─ Store: fib(3) = 2
           └─ Reuse: fib(2) = 1
        └─ Store: fib(4) = 3
     └─ Reuse: fib(3) = 2
  └─ Store: fib(5) = 5
```

Notice how fib(3) and fib(2) are computed only once and then reused. This reduces the time complexity to O(n).

### Coin Change Problem Visualization

Let's visualize the dynamic programming approach for the coin change problem with denominations [1, 3, 4, 5] and target amount 7:

First, we initialize our DP table:

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 ∞ ∞ ∞ ∞ ∞ ∞ ∞
```

Then, we fill the table by considering each coin for each amount:

For amount = 1:

- Use a 1-coin: dp[1] = dp[1-1] + 1 = dp[0] + 1 = 1

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 ∞ ∞ ∞ ∞ ∞ ∞
```

For amount = 2:

- Use a 1-coin: dp[2] = dp[2-1] + 1 = dp[1] + 1 = 2

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 2 ∞ ∞ ∞ ∞ ∞
```

For amount = 3:

- Use a 1-coin: dp[3] = dp[3-1] + 1 = dp[2] + 1 = 3
- Use a 3-coin: dp[3] = dp[3-3] + 1 = dp[0] + 1 = 1
- Take the minimum: dp[3] = min(3, 1) = 1

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 2 1 ∞ ∞ ∞ ∞
```

Continuing this process:

For amount = 4:

- Use a 1-coin: dp[4] = dp[4-1] + 1 = dp[3] + 1 = 2
- Use a 3-coin: dp[4] = dp[4-3] + 1 = dp[1] + 1 = 2
- Use a 4-coin: dp[4] = dp[4-4] + 1 = dp[0] + 1 = 1
- Take the minimum: dp[4] = min(2, 2, 1) = 1

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 2 1 1 ∞ ∞ ∞
```

For amount = 5:

- Use a 1-coin: dp[5] = dp[5-1] + 1 = dp[4] + 1 = 2
- Use a 3-coin: dp[5] = dp[5-3] + 1 = dp[2] + 1 = 3
- Use a 4-coin: dp[5] = dp[5-4] + 1 = dp[1] + 1 = 2
- Use a 5-coin: dp[5] = dp[5-5] + 1 = dp[0] + 1 = 1
- Take the minimum: dp[5] = min(2, 3, 2, 1) = 1

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 2 1 1 1 ∞ ∞
```

For amount = 6:

- Use a 1-coin: dp[6] = dp[6-1] + 1 = dp[5] + 1 = 2
- Use a 3-coin: dp[6] = dp[6-3] + 1 = dp[3] + 1 = 2
- Use a 4-coin: dp[6] = dp[6-4] + 1 = dp[2] + 1 = 3
- Use a 5-coin: dp[6] = dp[6-5] + 1 = dp[1] + 1 = 2
- Take the minimum: dp[6] = min(2, 2, 3, 2) = 2

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 2 1 1 1 2 ∞
```

For amount = 7:

- Use a 1-coin: dp[7] = dp[7-1] + 1 = dp[6] + 1 = 3
- Use a 3-coin: dp[7] = dp[7-3] + 1 = dp[4] + 1 = 2
- Use a 4-coin: dp[7] = dp[7-4] + 1 = dp[3] + 1 = 2
- Use a 5-coin: dp[7] = dp[7-5] + 1 = dp[2] + 1 = 3
- Take the minimum: dp[7] = min(3, 2, 2, 3) = 2

```
Amount:    0 1 2 3 4 5 6 7
Min Coins: 0 1 2 1 1 1 2 2
```

Final result: To make amount 7, we need a minimum of 2 coins.

## Time & Space Complexity Analysis

### Fibonacci Sequence

**Naive Recursive Approach**:

- Time Complexity: O(2^n)
- Space Complexity: O(n) for the recursion stack

The time complexity is exponential because for each n, the algorithm makes two recursive calls, creating a binary tree of depth n.

**Dynamic Programming Approach**:

- Time Complexity: O(n)
- Space Complexity: O(n) for memoization or tabulation, O(1) for optimized tabulation

With DP, we compute each Fibonacci number exactly once, resulting in linear time complexity. The optimized version keeps only the last two numbers, reducing space complexity to constant.

Mathematical Analysis:
For the recursive approach, T(n) = T(n-1) + T(n-2) + O(1), which solves to approximately T(n) = O(φ^n), where φ ≈ 1.618 is the golden ratio. This is actually Θ(1.618^n), a tighter bound than O(2^n), but still exponential.

For the DP approach, we do constant work n times, so T(n) = O(n).

### Coin Change Problem

**Naive Recursive Approach**:

- Time Complexity: O(m^n), where m is the number of coin denominations and n is the amount
- Space Complexity: O(n) for the recursion stack

This is because at each step, we have m choices (one for each coin denomination), and the depth of the recursion tree is potentially n (if we use 1-unit coins).

**Dynamic Programming Approach**:

- Time Complexity: O(n\*m), where n is the amount and m is the number of coin denominations
- Space Complexity: O(n) for the dp array

With DP, we perform m operations for each of the n+1 amounts, resulting in O(n\*m) time complexity.

Mathematical Analysis:
For each amount from 0 to n, we consider m coin denominations, leading to n\*m operations. Each operation involves simple arithmetic and comparison, which takes constant time.

### Climbing Stairs Problem

**Naive Recursive Approach**:

- Time Complexity: O(2^n)
- Space Complexity: O(n) for the recursion stack

Similar to Fibonacci, each step branches into two recursive calls.

**Dynamic Programming Approach**:

- Time Complexity: O(n)
- Space Complexity: O(n) for tabulation, O(1) for optimized tabulation

We compute each step exactly once, resulting in linear time complexity.

## C++ Implementation

Let's implement these algorithms with detailed comments:

### Fibonacci Sequence

```cpp
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Naive recursive approach
int fibNaive(int n) {
    if (n <= 1) return n;
    return fibNaive(n-1) + fibNaive(n-2);
}

// Top-down memoization approach
int fibMemo(int n, vector<int>& memo) {
    // Base cases
    if (n <= 1) return n;

    // If already computed, return stored result
    if (memo[n] != -1) return memo[n];

    // Compute and store result
    memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo);
    return memo[n];
}

int fibonacciMemo(int n) {
    vector<int> memo(n+1, -1);
    return fibMemo(n, memo);
}

// Bottom-up tabulation approach
int fibonacciTab(int n) {
    if (n <= 1) return n;

    vector<int> dp(n+1);
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }

    return dp[n];
}

// Space-optimized tabulation approach
int fibonacciOpt(int n) {
    if (n <= 1) return n;

    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

// Benchmark function to compare different implementations
void benchmarkFibonacci(int n) {
    cout << "Benchmarking Fibonacci implementations for n = " << n << endl;

    auto start = high_resolution_clock::now();
    int result;

    if (n <= 40) {  // Naive approach is too slow for large n
        start = high_resolution_clock::now();
        result = fibNaive(n);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Naive recursive: " << result << " (Time: " << duration.count() << " ms)" << endl;
    } else {
        cout << "Naive recursive: Skipped (too slow for n = " << n << ")" << endl;
    }

    start = high_resolution_clock::now();
    result = fibonacciMemo(n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Memoization: " << result << " (Time: " << duration.count() << " ms)" << endl;

    start = high_resolution_clock::now();
    result = fibonacciTab(n);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Tabulation: " << result << " (Time: " << duration.count() << " ms)" << endl;

    start = high_resolution_clock::now();
    result = fibonacciOpt(n);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Optimized tabulation: " << result << " (Time: " << duration.count() << " ms)" << endl;
}
```

### Coin Change Problem

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

// Naive recursive approach
int coinChangeNaive(vector<int>& coins, int amount, int index = 0) {
    // Base cases
    if (amount == 0) return 0;
    if (amount < 0 || index >= coins.size()) return -1;

    // Try including the current coin
    int include = coinChangeNaive(coins, amount - coins[index], index);

    // Try excluding the current coin
    int exclude = coinChangeNaive(coins, amount, index + 1);

    // Calculate the result
    if (include == -1 && exclude == -1) return -1;
    if (include == -1) return exclude;
    if (exclude == -1) return include + 1;

    return min(include + 1, exclude);
}

// Top-down memoization approach
int coinChangeMemo(vector<int>& coins, int amount, vector<vector<int>>& memo, int index = 0) {
    // Base cases
    if (amount == 0) return 0;
    if (amount < 0 || index >= coins.size()) return -1;

    // If already computed, return stored result
    if (memo[index][amount] != -2) return memo[index][amount];

    // Try including the current coin
    int include = coinChangeMemo(coins, amount - coins[index], memo, index);

    // Try excluding the current coin
    int exclude = coinChangeMemo(coins, amount, memo, index + 1);

    // Calculate and store the result
    int result;
    if (include == -1 && exclude == -1) result = -1;
    else if (include == -1) result = exclude;
    else if (exclude == -1) result = include + 1;
    else result = min(include + 1, exclude);

    memo[index][amount] = result;
    return result;
}

int coinChangeWithMemo(vector<int>& coins, int amount) {
    // Initialize memo array with -2 (to distinguish from -1 which means no solution)
    vector<vector<int>> memo(coins.size(), vector<int>(amount + 1, -2));
    return coinChangeMemo(coins, amount, memo);
}

// Bottom-up tabulation approach
int coinChangeTab(vector<int>& coins, int amount) {
    // Initialize dp array with a value larger than any possible solution
    vector<int> dp(amount + 1, amount + 1);

    // Base case: 0 coins needed to make amount 0
    dp[0] = 0;

    // Fill the dp array bottom-up
    for (int i = 1; i <= amount; i++) {
        // Try each coin denomination
        for (int coin : coins) {
            if (coin <= i) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }

    // If dp[amount] is still amount+1, it means no solution exists
    return dp[amount] > amount ? -1 : dp[amount];
}

// Function to reconstruct the coins used in the solution
vector<int> getCoinsUsed(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    vector<int> coinUsed(amount + 1, -1);

    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int j = 0; j < coins.size(); j++) {
            int coin = coins[j];
            if (coin <= i && dp[i - coin] + 1 < dp[i]) {
                dp[i] = dp[i - coin] + 1;
                coinUsed[i] = j;
            }
        }
    }

    if (dp[amount] > amount) return {};

    // Reconstruct the solution
    vector<int> result;
    int remaining = amount;
    while (remaining > 0) {
        result.push_back(coins[coinUsed[remaining]]);
        remaining -= coins[coinUsed[remaining]];
    }

    return result;
}
```

### Climbing Stairs Problem

```cpp
#include <iostream>
#include <vector>
using namespace std;

// Naive recursive approach
int climbStairsNaive(int n) {
    if (n <= 2) return n;
    return climbStairsNaive(n-1) + climbStairsNaive(n-2);
}

// Top-down memoization approach
int climbStairsMemo(int n, vector<int>& memo) {
    if (n <= 2) return n;

    if (memo[n] != -1) return memo[n];

    memo[n] = climbStairsMemo(n-1, memo) + climbStairsMemo(n-2, memo);
    return memo[n];
}

int climbStairsWithMemo(int n) {
    vector<int> memo(n+1, -1);
    return climbStairsMemo(n, memo);
}

// Bottom-up tabulation approach
int climbStairsTab(int n) {
    if (n <= 2) return n;

    vector<int> dp(n+1);
    dp[1] = 1;
    dp[2] = 2;

    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }

    return dp[n];
}

// Space-optimized tabulation approach
int climbStairsOpt(int n) {
    if (n <= 2) return n;

    int prev = 1;
    int curr = 2;

    for (int i = 3; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

// Generalized version: can take 1 to k steps at a time
int climbStairsGeneralized(int n, int k) {
    vector<int> dp(n+1, 0);
    dp[0] = 1;  // Base case: one way to climb 0 steps

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k && i - j >= 0; j++) {
            dp[i] += dp[i - j];
        }
    }

    return dp[n];
}
```

## Edge Cases & Optimizations

### Edge Cases in Dynamic Programming

1. **Base Cases**: Ensure proper handling of base cases, especially for recursive implementations. For example:

   - Fibonacci: Handle n = 0 and n = 1
   - Coin Change: Handle amount = the value of each coin denomination
   - Climbing Stairs: Handle n = 1 and n = 2

2. **Empty Inputs**: Be careful with empty sets or zero values. For example:

   - Coin Change: Handle empty coins array or amount = 0
   - Knapsack Problem: Handle empty items array or capacity = 0

3. **Negative Values**: Some problems might involve negative values, which can affect our recursive definitions. For example:

   - Coin Change: What if negative amounts are allowed?
   - Maximum Subarray Sum: How to handle all-negative arrays?

4. **Overflow**: For large inputs, ensure that the result doesn't overflow the data type. Consider using long long or BigInteger.
   - Fibonacci: Values grow exponentially, leading to overflow for n > 46 with 32-bit integers
   - Combinations problems: Results can grow factorially

### Performance Optimizations

1. **Space Optimization**: Many DP problems can have their space complexity reduced from O(n) to O(1) or O(k) by only keeping track of the last few states. For example:

   - Fibonacci: Only need to store the last two numbers
   - Climbing Stairs: Only need to store the last two counts

2. **State Reduction**: Sometimes, the DP state can be simplified:

   - 2D DP tables can be reduced to 1D if the current state only depends on the previous row
   - 3D DP tables can be reduced to 2D using similar techniques

3. **Precomputation**: For repeated calls with different parameters, precompute the DP table once and reuse it:

   - Precompute Fibonacci numbers up to a maximum expected n
   - Precompute binomial coefficients for combination problems

4. **Avoid Recursion Stack Overflow**: For large inputs, prefer iterative approaches:

   - Convert recursive memoization to iterative tabulation
   - Use tail recursion optimization where supported

5. **Custom Data Structures**: Use appropriate data structures for the specific problem:
   - Sparse Tables for Range Minimum Query
   - Segment Trees for dynamic range queries

Example of space optimization for Coin Change:

```cpp
// Space-optimized coin change
// Only works when we need to find the minimum number of coins, not the actual coins used
int coinChangeOptimized(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;

    for (int coin : coins) {
        for (int i = coin; i <= amount; i++) {
            dp[i] = min(dp[i], dp[i - coin] + 1);
        }
    }

    return dp[amount] > amount ? -1 : dp[amount];
}
```

## Real-world Applications

### 1. Sequence Alignment in Bioinformatics

Dynamic programming is crucial in bioinformatics for aligning DNA, RNA, or protein sequences. The Needleman-Wunsch and Smith-Waterman algorithms use DP to find the optimal alignment between two biological sequences.

This has applications in:

- Identifying evolutionary relationships between species
- Finding gene mutations associated with diseases
- Protein structure prediction and drug discovery

Example: The BLAST (Basic Local Alignment Search Tool) algorithm uses dynamic programming concepts to rapidly search for similarities between a query sequence and a database of sequences, enabling researchers to identify gene functions across species.

### 2. Resource Allocation in Economics

DP is widely used in economic models for optimal resource allocation problems:

- **Knapsack Problems**: Deciding how to allocate limited resources (like budget) across different investments
- **Production Planning**: Determining the optimal schedule for manufacturing to minimize costs
- **Portfolio Optimization**: Distributing investments across assets to maximize returns while managing risk

Example: A manufacturing company uses dynamic programming to optimize their production schedule, deciding which products to manufacture, when to manufacture them, and how much inventory to maintain, saving millions in annual costs.

### 3. Computer Graphics and Image Processing

DP algorithms are fundamental in computer graphics:

- **Seam Carving**: Content-aware image resizing using dynamic programming to find optimal paths through an image
- **Image Segmentation**: Partitioning an image into meaningful regions using DP-based algorithms
- **Texture Synthesis**: Generating textures by finding optimal matches between small patches

Example: Adobe Photoshop's content-aware fill feature uses DP-based algorithms to seamlessly fill in removed areas of an image by analyzing surrounding pixels, a technique that has revolutionized photo editing.

### 4. Natural Language Processing

DP is essential in various NLP tasks:

- **Speech Recognition**: Hidden Markov Models with DP for finding the most likely sequence of words
- **Parsing**: Cocke-Younger-Kasami (CYK) algorithm for parsing context-free grammars
- **Machine Translation**: Dynamic programming for alignment models in statistical machine translation

Example: Google Translate initially used statistical machine translation models with DP-based alignment algorithms to map words and phrases between languages, processing billions of translation requests daily before the advent of neural models.

### 5. Operations Research and Logistics

DP optimizes complex logistics operations:

- **Shortest Path Problems**: Finding optimal routes for delivery vehicles
- **Network Flow Optimization**: Managing supply chains efficiently
- **Inventory Management**: Determining optimal ordering policies

Example: Amazon uses dynamic programming algorithms to optimize their fulfillment center operations, determining which warehouses should fulfill which orders to minimize shipping costs and delivery times, handling millions of packages daily.

## Humorous Elements

### The DP Detective Story

Imagine a detective trying to solve the Fibonacci Mystery:

_Detective Recursive_ approaches a crime scene: "I'll solve this case by investigating every single clue, even if I've seen it before!"

_Detective Memoization_ shows up: "Rookie mistake. I take notes on each clue I investigate. If I see the same clue again, I just check my notebook instead of reinvestigating."

_Detective Tabulation_ arrives: "Amateurs. I work systematically from the simplest clues to the most complex ones, building my case file as I go. By the time I reach the main mystery, most of it is already solved!"

The moral: A good detective, like a good algorithm, knows when to reuse previous work!

### The Dynamic Programming Diet Plan

A computer scientist goes on a diet:

Day 1: "I'll try every possible combination of foods to maximize nutrition while minimizing calories. This will take forever!"

Day 2: "Wait, I can use memoization! I'll record the optimal meal plans for different calorie targets, so I don't recalculate them."

Day 3: "Even better, I'll use tabulation! I'll start by finding the optimal meal for 100 calories, then 200, and so on."

Day 30: "My dynamic programming diet is a success! I've lost weight and gained O(n) efficiency!"

His friend asks: "What about just eating vegetables and exercising?"

The computer scientist: "That's a greedy algorithm. It might work in practice, but it's not provably optimal!"

### The Recursive Restaurant

At "The Recursive Restaurant," a customer orders a dish:

Waiter: "I'll need to ask the chef."

Chef: "To make this dish, I need to know how to make a simpler version of it first."

Sous Chef: "To make that simpler dish, I need to know how to make an even simpler version."

Intern: "To make that even simpler dish..."

Customer: "My food will never arrive!"

Manager: "We fixed that with our new 'Memoization Menu'! Now we write down recipes for dishes we've made before."

Customer, impressed: "So, you went from exponential to linear serving time?"

Manager: "Yes! And our 'Tabulation Table Service' is even faster—we prepare all simpler dishes in advance!"

## Practice Problems

### Easy: Minimum Cost Climbing Stairs (LeetCode 746)

**Problem**: You are given an integer array cost where cost[i] is the cost of i-th step on a staircase. Once you pay the cost, you can either climb one or two steps. You can either start from the step with index 0, or the step with index 1. Return the minimum cost to reach the top of the floor.

**Approach Hint**:

- Define dp[i] as the minimum cost to reach the i-th step
- Base cases: dp[0] = cost[0], dp[1] = cost[1]
- For i >= 2: dp[i] = cost[i] + min(dp[i-1], dp[i-2])
- The answer is min(dp[n-1], dp[n-2]) where n is the length of the cost array
- You can optimize the space complexity to O(1) by keeping track of only the last two steps

### Medium: Unique Paths (LeetCode 62)

**Problem**: A robot is located at the top-left corner of a m×n grid. The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid. How many possible unique paths are there?

**Approach Hint**:

- Define dp[i][j] as the number of ways to reach cell (i, j)
- Base cases: dp[0][j] = 1 for all j, dp[i][0] = 1 for all i
- For i, j > 0: dp[i][j] = dp[i-1][j] + dp[i][j-1]
- The answer is dp[m-1][n-1]
- Consider space optimization: You only need to keep track of the current row and the previous row

### Hard: Coin Change 2 (LeetCode 518)

**Problem**: You are given an integer array coins representing coins of different denominations and an integer amount representing a total amount of money. Return the number of combinations that make up that amount.

**Approach Hint**:

- Define dp[i] as the number of ways to make up amount i
- Base case: dp[0] = 1 (one way to make zero: use no coins)
- For each coin, for each amount from coin value to target amount:
  dp[amount] += dp[amount - coin]
- The order of the loops matters! To count combinations (not permutations), iterate through coins first
- Be careful with integer overflow for large inputs

## Preview of Day 37

Tomorrow, we'll dive deeper into Dynamic Programming with Part 2, covering:

- The classic Knapsack problem and its variations
- Longest Common Subsequence (LCS) and other string-related DP problems
- Matrix Chain Multiplication
- Advanced optimization techniques
- Even more real-world applications

Get ready to tackle these more complex DP challenges that build on the foundations we established today!

## Conclusion

Dynamic Programming is a powerful technique that transforms exponential algorithms into polynomial ones by storing and reusing solutions to overlapping subproblems. Today, we've explored the core principles of DP, the different approaches (memoization vs. tabulation), and solved classic problems like Fibonacci, Coin Change, and Climbing Stairs.

The key insights from today's lesson are:

1. Identify problems with optimal substructure and overlapping subproblems
2. Formulate a recursive relation to express the solution in terms of smaller subproblems
3. Choose between top-down memoization or bottom-up tabulation based on the specific problem
4. Consider space optimizations when possible
5. Be mindful of edge cases and potential performance pitfalls

As you practice, you'll develop an intuition for recognizing problems suitable for DP and choosing the right approach. Dynamic Programming isn't just a theoretical concept—it's a practical tool used in countless real-world applications, from bioinformatics to finance, from computer graphics to natural language processing.

Tomorrow, we'll build on these foundations to tackle more complex DP problems. The journey continues!

#DSAin45 #DynamicProgramming #Algorithms #Memoization #Tabulation #CPlusPlus #ProblemSolving
