# Day 37: Dynamic Programming - Part 2 #DSAin45

## Introduction

Welcome to Day 37 of our #DSAin45 journey! Yesterday, we explored the fundamentals of Dynamic Programming (DP), discussing the core principles of overlapping subproblems and optimal substructure, the differences between memoization and tabulation approaches, and tackling classic problems like Fibonacci, Coin Change, and Climbing Stairs.

Today, we dive deeper into more complex DP problems and techniques. We'll focus on the legendary Knapsack problem, the Longest Common Subsequence (LCS), and other string-related DP problems. These problems are not only common in interviews and competitive programming but also have profound applications in fields ranging from resource allocation to computational biology.

As Richard Bellman, the father of dynamic programming, would say, we continue "making good decisions one at a time" through our journey of breaking down complex problems into manageable subproblems and building optimal solutions from the ground up.

## Core Concepts: Advanced DP Problems

### The 0/1 Knapsack Problem

The Knapsack problem is a classic optimization challenge that perfectly illustrates the power of dynamic programming.

**Problem Statement**: Given a set of items, each with a weight and a value, determine which items to include in a collection so that the total weight is less than or equal to a given limit (the capacity of the knapsack) and the total value is as large as possible.

In the 0/1 variant, each item can either be taken completely (1) or left behind (0) - no fractional items allowed.

#### Recursive Definition

Let's define our state:

- `dp[i][w]` = maximum value that can be obtained using the first `i` items and with a weight limit of `w`.

Base cases:

- `dp[0][w] = 0` for all `w` (no items means zero value)
- `dp[i][0] = 0` for all `i` (zero capacity means no items can be taken)

For `i > 0` and `w > 0`:

- If the weight of the i-th item is greater than the current weight limit:
  `dp[i][w] = dp[i-1][w]` (skip the item)
- Otherwise:
  `dp[i][w] = max(dp[i-1][w], dp[i-1][w - weights[i-1]] + values[i-1])` (max of including or excluding the item)

#### Implementation

**Approach 1: Recursive with Memoization**

```cpp
int knapsackRecursive(vector<int>& weights, vector<int>& values, int capacity, int n,
                     vector<vector<int>>& memo) {
    // Base case
    if (n == 0 || capacity == 0) {
        return 0;
    }

    // If result is already computed, return it
    if (memo[n][capacity] != -1) {
        return memo[n][capacity];
    }

    // Item is too heavy, skip it
    if (weights[n-1] > capacity) {
        memo[n][capacity] = knapsackRecursive(weights, values, capacity, n-1, memo);
        return memo[n][capacity];
    }

    // Max of including or excluding the item
    memo[n][capacity] = max(
        values[n-1] + knapsackRecursive(weights, values, capacity - weights[n-1], n-1, memo),
        knapsackRecursive(weights, values, capacity, n-1, memo)
    );

    return memo[n][capacity];
}

int knapsack(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> memo(n+1, vector<int>(capacity+1, -1));
    return knapsackRecursive(weights, values, capacity, n, memo);
}
```

**Approach 2: Tabulation (Bottom-Up)**

```cpp
int knapsack(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> dp(n+1, vector<int>(capacity+1, 0));

    // Fill the dp table
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i-1] <= w) {
                // Max of including or excluding the item
                dp[i][w] = max(
                    values[i-1] + dp[i-1][w - weights[i-1]],
                    dp[i-1][w]
                );
            } else {
                // Item is too heavy, skip it
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    return dp[n][capacity];
}
```

**Approach 3: Space-Optimized Tabulation**

```cpp
int knapsack(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<int> dp(capacity+1, 0);

    for (int i = 0; i < n; i++) {
        // Process in reverse to avoid using results from current iteration
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }

    return dp[capacity];
}
```

#### Tracing the Optimal Solution

To trace which items are included in the optimal solution:

```cpp
vector<int> getKnapsackItems(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> dp(n+1, vector<int>(capacity+1, 0));

    // First, compute the DP table
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i-1] <= w) {
                dp[i][w] = max(
                    values[i-1] + dp[i-1][w - weights[i-1]],
                    dp[i-1][w]
                );
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    // Backtrack to find the items
    vector<int> result;
    int w = capacity;

    for (int i = n; i > 0; i--) {
        // Check if this item is included
        if (dp[i][w] != dp[i-1][w]) {
            result.push_back(i-1); // Add the index of the item
            w -= weights[i-1];     // Reduce the capacity
        }
    }

    return result;
}
```

### Knapsack Variations

#### Unbounded Knapsack (Multiple Instances)

In this variation, we can take any item multiple times.

```cpp
int unboundedKnapsack(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<int> dp(capacity+1, 0);

    for (int w = 1; w <= capacity; w++) {
        for (int i = 0; i < n; i++) {
            if (weights[i] <= w) {
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
    }

    return dp[capacity];
}
```

#### Subset Sum Problem

Given a set of non-negative integers, determine if there is a subset whose sum equals a given target.

```cpp
bool subsetSum(vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<bool>> dp(n+1, vector<bool>(target+1, false));

    // Base case: empty subset has sum 0
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= target; j++) {
            if (nums[i-1] <= j) {
                dp[i][j] = dp[i-1][j] || dp[i-1][j - nums[i-1]];
            } else {
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    return dp[n][target];
}
```

### Longest Common Subsequence (LCS)

The LCS problem asks: given two strings, find the length of their longest common subsequence (not necessarily contiguous).

#### Recursive Definition

Let's define our state:

- `dp[i][j]` = length of LCS of prefixes `s1[0...i-1]` and `s2[0...j-1]`

Base cases:

- `dp[0][j] = 0` for all `j` (empty first string has 0 LCS with any string)
- `dp[i][0] = 0` for all `i` (empty second string has 0 LCS with any string)

For `i > 0` and `j > 0`:

- If `s1[i-1] == s2[j-1]`:
  `dp[i][j] = dp[i-1][j-1] + 1` (characters match, extend LCS)
- Otherwise:
  `dp[i][j] = max(dp[i-1][j], dp[i][j-1])` (characters don't match, take best of two options)

#### Implementation

**Approach 1: Recursive with Memoization**

```cpp
int lcsRecursive(string& s1, string& s2, int m, int n, vector<vector<int>>& memo) {
    // Base case
    if (m == 0 || n == 0) {
        return 0;
    }

    // If result is already computed, return it
    if (memo[m][n] != -1) {
        return memo[m][n];
    }

    // If characters match, extend the LCS
    if (s1[m-1] == s2[n-1]) {
        memo[m][n] = 1 + lcsRecursive(s1, s2, m-1, n-1, memo);
    } else {
        // Characters don't match, take the maximum of two options
        memo[m][n] = max(
            lcsRecursive(s1, s2, m-1, n, memo),
            lcsRecursive(s1, s2, m, n-1, memo)
        );
    }

    return memo[m][n];
}

int longestCommonSubsequence(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> memo(m+1, vector<int>(n+1, -1));
    return lcsRecursive(s1, s2, m, n, memo);
}
```

**Approach 2: Tabulation (Bottom-Up)**

```cpp
int longestCommonSubsequence(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }

    return dp[m][n];
}
```

**Approach 3: Space-Optimized Tabulation**

```cpp
int longestCommonSubsequence(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();

    // Ensure s1 is the shorter string to optimize space
    if (m > n) {
        swap(s1, s2);
        swap(m, n);
    }

    vector<int> prev(n+1, 0);
    vector<int> curr(n+1, 0);

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                curr[j] = prev[j-1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j-1]);
            }
        }
        prev = curr;
    }

    return prev[n];
}
```

#### Printing the LCS

To actually find one of the possible LCS strings:

```cpp
string printLCS(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    // Fill the dp table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }

    // Backtrack to find the LCS
    string lcs;
    int i = m, j = n;

    while (i > 0 && j > 0) {
        if (s1[i-1] == s2[j-1]) {
            lcs = s1[i-1] + lcs;
            i--; j--;
        } else if (dp[i-1][j] > dp[i][j-1]) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}
```

### Longest Common Substring

Unlike LCS, the longest common substring must be contiguous.

```cpp
int longestCommonSubstring(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));
    int result = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                result = max(result, dp[i][j]);
            } else {
                dp[i][j] = 0; // Reset if characters don't match
            }
        }
    }

    return result;
}
```

### Edit Distance

The Edit Distance problem asks: given two strings, find the minimum number of operations (insert, delete, replace) required to convert one string to another.

#### Recursive Definition

Let's define our state:

- `dp[i][j]` = minimum operations to convert prefix `s1[0...i-1]` to prefix `s2[0...j-1]`

Base cases:

- `dp[0][j] = j` for all `j` (converting empty string to string of length j requires j insertions)
- `dp[i][0] = i` for all `i` (converting string of length i to empty string requires i deletions)

For `i > 0` and `j > 0`:

- If `s1[i-1] == s2[j-1]`:
  `dp[i][j] = dp[i-1][j-1]` (no operation needed for this character)
- Otherwise:
  `dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])` (minimum of delete, insert, replace)

#### Implementation

```cpp
int minDistance(string word1, string word2) {
    int m = word1.length();
    int n = word2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    // Base cases
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i; // Converting to empty string
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j; // Converting from empty string
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i-1] == word2[j-1]) {
                dp[i][j] = dp[i-1][j-1]; // No operation needed
            } else {
                dp[i][j] = 1 + min({
                    dp[i-1][j],   // Delete
                    dp[i][j-1],   // Insert
                    dp[i-1][j-1]  // Replace
                });
            }
        }
    }

    return dp[m][n];
}
```

## Visual Explanations of Algorithms

### 0/1 Knapsack Visualization

Let's visualize the Knapsack problem with a concrete example:

**Items:**

1. Weight: 1, Value: 1
2. Weight: 3, Value: 4
3. Weight: 4, Value: 5
4. Weight: 5, Value: 7

**Capacity:** 7

First, we initialize our DP table:

```
      Capacity (w)
      0 1 2 3 4 5 6 7
I 0 | 0 0 0 0 0 0 0 0
t 1 | 0 ? ? ? ? ? ? ?
e 2 | 0 ? ? ? ? ? ? ?
m 3 | 0 ? ? ? ? ? ? ?
s 4 | 0 ? ? ? ? ? ? ?
```

Now, we fill the table step by step:

For item 1 (weight=1, value=1):

```
      0 1 2 3 4 5 6 7
    | 0 0 0 0 0 0 0 0
1(1)| 0 1 1 1 1 1 1 1
    | 0 ? ? ? ? ? ? ?
    | 0 ? ? ? ? ? ? ?
    | 0 ? ? ? ? ? ? ?
```

For item 2 (weight=3, value=4):

```
      0 1 2 3 4 5 6 7
    | 0 0 0 0 0 0 0 0
1(1)| 0 1 1 1 1 1 1 1
3(4)| 0 1 1 4 5 5 5 5
    | 0 ? ? ? ? ? ? ?
    | 0 ? ? ? ? ? ? ?
```

For item 3 (weight=4, value=5):

```
      0 1 2 3 4 5 6 7
    | 0 0 0 0 0 0 0 0
1(1)| 0 1 1 1 1 1 1 1
3(4)| 0 1 1 4 5 5 5 5
4(5)| 0 1 1 4 5 6 9 9
    | 0 ? ? ? ? ? ? ?
```

For item 4 (weight=5, value=7):

```
      0 1 2 3 4 5 6 7
    | 0 0 0 0 0 0 0 0
1(1)| 0 1 1 1 1 1 1 1
3(4)| 0 1 1 4 5 5 5 5
4(5)| 0 1 1 4 5 6 9 9
5(7)| 0 1 1 4 5 7 8 11
```

The maximum value we can achieve with capacity 7 is 11, which is in the bottom-right cell of our table.

To find which items are included:

1. Start at dp[4][7] = 11
2. Is dp[4][7] != dp[3][7]? Yes (11 != 9), so we include item 4
3. Remaining capacity = 7 - 5 = 2
4. Is dp[3][2] != dp[2][2]? No (1 == 1), so we don't include item 3
5. Is dp[2][2] != dp[1][2]? No (1 == 1), so we don't include item 2
6. Is dp[1][2] != dp[0][2]? Yes (1 != 0), so we include item 1
7. Remaining capacity = 2 - 1 = 1
8. Is dp[0][1] != dp[0][0]? No (0 == 0), we're done

So we include items 1 and 4 for a value of 1 + 7 = 8. (Wait, that's not 11... let me trace again.)

Ah, I made an error in my trace. Let's redo:

1. Start at dp[4][7] = 11
2. Is dp[4][7] != dp[3][7]? Yes (11 != 9), so we include item 4
3. Remaining capacity = 7 - 5 = 2
4. Is dp[3][2] != dp[2][2]? No (1 == 1), so we don't include item 3
5. Is dp[2][2] != dp[1][2]? No (1 == 1), so we don't include item 2
6. Is dp[1][2] != dp[0][2]? Yes (1 != 0), so we include item 1
7. Remaining capacity = 2 - 1 = 1
8. We've reached the end

So we include items 1 and 4 for a value of 1 + 7 = 8.

Wait, this still doesn't match 11. Let me revisit the table filling:

For dp[4][7], we consider:

- Not taking item 4: dp[3][7] = 9
- Taking item 4: dp[3][7-5] + 7 = dp[3][2] + 7 = 1 + 7 = 8

So dp[4][7] should be max(9, 8) = 9, not 11. Let me correct the table:

```
      0 1 2 3 4 5 6 7
    | 0 0 0 0 0 0 0 0
1(1)| 0 1 1 1 1 1 1 1
3(4)| 0 1 1 4 5 5 5 5
4(5)| 0 1 1 4 5 6 6 9
5(7)| 0 1 1 4 5 7 8 9
```

Now, to find the included items:

1. Start at dp[4][7] = 9
2. Is dp[4][7] != dp[3][7]? No (9 == 9), so we don't include item 4
3. Is dp[3][7] != dp[2][7]? Yes (9 != 5), so we include item 3
4. Remaining capacity = 7 - 4 = 3
5. Is dp[2][3] != dp[1][3]? Yes (4 != 1), so we include item 2
6. Remaining capacity = 3 - 3 = 0
7. We've reached capacity 0, so we're done

So we include items 2 and 3 for a value of 4 + 5 = 9. This matches our final answer.

### Longest Common Subsequence Visualization

Let's consider two strings: s1 = "ABCBDAB" and s2 = "BDCABA".

First, initialize our DP table:

```
      B D C A B A
    | 0 0 0 0 0 0
A   | 0 ? ? ? ? ?
B   | 0 ? ? ? ? ?
C   | 0 ? ? ? ? ?
B   | 0 ? ? ? ? ?
D   | 0 ? ? ? ? ?
A   | 0 ? ? ? ? ?
B   | 0 ? ? ? ? ?
```

Fill in the table using our recurrence relation:

```
      B D C A B A
    | 0 0 0 0 0 0
A   | 0 0 0 0 1 1
B   | 0 1 1 1 1 2
C   | 0 1 1 2 2 2
B   | 0 1 1 2 2 3
D   | 0 1 2 2 2 3
A   | 0 1 2 2 3 3
B   | 0 1 2 2 3 4
```

The length of the LCS is 4, found in the bottom-right cell.

To find the actual LCS:

1. Start at dp[7][6] = 4
2. s1[7-1] == s2[6-1]? No (B != A), so move left
3. dp[7][5] == 3, s1[7-1] == s2[5-1]? Yes (B == B), add 'B' to LCS, move diagonally
4. dp[6][4] == 3, s1[6-1] == s2[4-1]? Yes (A == A), add 'A' to LCS, move diagonally
5. dp[5][3] == 2, s1[5-1] == s2[3-1]? No (D != C), compare dp[4][3] vs dp[5][2]
6. dp[4][3] > dp[5][2], so move up
7. dp[4][3] == 2, s1[4-1] == s2[3-1]? No (B != C), compare dp[3][3] vs dp[4][2]
8. dp[3][3] == 2, s1[3-1] == s2[3-1]? Yes (C == C), add 'C' to LCS, move diagonally
9. dp[2][2] == 1, s1[2-1] == s2[2-1]? No (B != D), compare dp[1][2] vs dp[2][1]
10. They're equal, let's move left
11. dp[2][1] == 1, s1[2-1] == s2[1-1]? Yes (B == B), add 'B' to LCS, move diagonally
12. We've reached dp[1][0] = 0, we're done

The LCS is "BCAB" (reading backward from our trace).

### Edit Distance Visualization

Let's visualize the Edit Distance between s1 = "horse" and s2 = "ros".

Initialize our DP table:

```
      r o s
    | 0 1 2 3
h   | 1 ? ? ?
o   | 2 ? ? ?
r   | 3 ? ? ?
s   | 4 ? ? ?
e   | 5 ? ? ?
```

Fill in the table:

```
      r o s
    | 0 1 2 3
h   | 1 1 2 3
o   | 2 2 1 2
r   | 3 2 2 2
s   | 4 3 3 2
e   | 5 4 4 3
```

The minimum edit distance is 3, found in the bottom-right cell.

To trace the operations:

1. dp[5][3] = 3, s1[5-1] != s2[3-1], so we take min of:
   - Delete (dp[4][3] = 2) + 1 = 3
   - Insert (dp[5][2] = 4) + 1 = 5
   - Replace (dp[4][2] = 3) + 1 = 4
     We choose delete, removing 'e' from "horse"
2. dp[4][3] = 2, s1[4-1] == s2[3-1], so no operation needed
3. dp[3][2] = 2, s1[3-1] != s2[2-1], so we take min of:
   - Delete (dp[2][2] = 1) + 1 = 2
   - Insert (dp[3][1] = 2) + 1 = 3
   - Replace (dp[2][1] = 2) + 1 = 3
     We choose delete, removing 'r' from "hor"
4. dp[2][2] = 1, s1[2-1] == s2[2-1], so no operation needed
5. dp[1][1] = 1, s1[1-1] != s2[1-1], so we take min of:
   - Delete (dp[0][1] = 1) + 1 = 2
   - Insert (dp[1][0] = 1) + 1 = 2
   - Replace (dp[0][0] = 0) + 1 = 1
     We choose replace, changing 'h' to 'r'

So the operations are:

1. Replace 'h' with 'r'
2. No change for 'o'
3. Delete 'r'
4. No change for 's'
5. Delete 'e'

This gives us 3 operations to transform "horse" to "ros".

## Time & Space Complexity Analysis

### 0/1 Knapsack Problem

**Recursive Approach (without memoization):**

- Time Complexity: O(2^n), where n is the number of items
- Space Complexity: O(n) for the recursion stack

This is because for each item, we have two choices: include it or exclude it, resulting in a binary tree of depth n.

**Memoized Approach:**

- Time Complexity: O(n \* W), where n is the number of items and W is the capacity
- Space Complexity: O(n \* W) for the memoization table + O(n) for the recursion stack

We fill each cell in the memoization table at most once, and there are n \* W cells.

**Tabulation Approach:**

- Time Complexity: O(n \* W)
- Space Complexity: O(n \* W) for the DP table

**Space-Optimized Approach:**

- Time Complexity: O(n \* W)
- Space Complexity: O(W) for the 1D DP array

The mathematical insight here is that we're only ever using the previous row of the DP table, so we can optimize to just use a single row and update it in-place.

### Longest Common Subsequence

**Recursive Approach (without memoization):**

- Time Complexity: O(2^(m+n)), where m and n are the lengths of the strings
- Space Complexity: O(m + n) for the recursion stack

**Memoized Approach:**

- Time Complexity: O(m \* n)
- Space Complexity: O(m \* n) for the memoization table + O(m + n) for the recursion stack

**Tabulation Approach:**

- Time Complexity: O(m \* n)
- Space Complexity: O(m \* n) for the DP table

**Space-Optimized Approach:**

- Time Complexity: O(m \* n)
- Space Complexity: O(min(m, n)) for the 1D arrays

Again, we optimize by observing that we only need the previous row to compute the current row.

### Edit Distance

**Tabulation Approach:**

- Time Complexity: O(m \* n), where m and n are the lengths of the strings
- Space Complexity: O(m \* n) for the DP table

**Space-Optimized Approach:**

- Time Complexity: O(m \* n)
- Space Complexity: O(min(m, n))

The mathematical insight is similar to the LCS space optimization.

## C++ Implementation

Here's a detailed implementation of all the algorithms discussed:

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Knapsack with memoization
int knapsackMemo(vector<int>& weights, vector<int>& values, int capacity, int n,
                vector<vector<int>>& memo) {
    // Base case
    if (n == 0 || capacity == 0) {
        return 0;
    }

    // If result is already computed
    if (memo[n][capacity] != -1) {
        return memo[n][capacity];
    }

    // If item is too heavy
    if (weights[n-1] > capacity) {
        memo[n][capacity] = knapsackMemo(weights, values, capacity, n-1, memo);
    } else {
        // Max of including or excluding the item
        memo[n][capacity] = max(
            values[n-1] + knapsackMemo(weights, values, capacity - weights[n-1], n-1, memo),
            knapsackMemo(weights, values, capacity, n-1, memo)
        );
    }

    return memo[n][capacity];
}

int knapsack(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> memo(n+1, vector<int>(capacity+1, -1));
    return knapsackMemo(weights, values, capacity, n, memo);
}

// Knapsack with tabulation
int knapsackDP(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> dp(n+1, vector<int>(capacity+1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i-1] <= w) {
                dp[i][w] = max(
                    values[i-1] + dp[i-1][w - weights[i-1]],
                    dp[i-1][w]
                );
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    return dp[n][capacity];
}

// Knapsack with space optimization
int knapsackOptimized(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<int> dp(capacity+1, 0);

    for (int i = 0; i < n; i++) {
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }

    return dp[capacity];
}

// Get items included in knapsack solution
vector<int> getKnapsackItems(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> dp(n+1, vector<int>(capacity+1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i-1] <= w) {
                dp[i][w] = max(
                    values[i-1] + dp[i-1][w - weights[i-1]],
                    dp[i-1][w]
                );
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    // Backtrack to find items
    vector<int> result;
    int w = capacity;

    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            result.push_back(i-1);
            w -= weights[i-1];
        }
    }

    return result;
}

// Unbounded Knapsack
int unboundedKnapsack(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<int> dp(capacity+1, 0);

    for (int w = 1; w <= capacity; w++) {
        for (int i = 0; i < n; i++) {
            if (weights[i] <= w) {
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
    }

    return dp[capacity];
}

// Subset Sum
bool subsetSum(vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<bool>> dp(n+1, vector<bool>(target+1, false));

    // Base case: empty subset can make sum 0
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= target; j++) {
            if (nums[i-1] <= j) {
                dp[i][j] = dp[i-1][j] || dp[i-1][j - nums[i-1]];
            } else {
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    return dp[n][target];
}

// Longest Common Subsequence - Memoization
int lcsRecursive(string& s1, string& s2, int m, int n, vector<vector<int>>& memo) {
    if (m == 0 || n == 0) {
        return 0;
    }

    if (memo[m][n] != -1) {
        return memo[m][n];
    }

    if (s1[m-1] == s2[n-1]) {
        memo[m][n] = 1 + lcsRecursive(s1, s2, m-1, n-1, memo);
    } else {
        memo[m][n] = max(
            lcsRecursive(s1, s2, m-1, n, memo),
            lcsRecursive(s1, s2, m, n-1, memo)
        );
    }

    return memo[m][n];
}

int longestCommonSubsequence(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> memo(m+1, vector<int>(n+1, -1));
    return lcsRecursive(s1, s2, m, n, memo);
}

// Longest Common Subsequence - Tabulation
int lcsDP(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }

    return dp[m][n];
}

// Longest Common Subsequence - Space Optimized
int lcsOptimized(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();

    // Ensure s1 is the shorter string
    if (m > n) {
        swap(s1, s2);
        swap(m, n);
    }

    vector<int> prev(n+1, 0);
    vector<int> curr(n+1, 0);

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                curr[j] = prev[j-1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j-1]);
            }
        }
        prev = curr;
    }

    return prev[n];
}

// Print the Longest Common Subsequence
string printLCS(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }

    // Backtrack to find the LCS
    string lcs;
    int i = m, j = n;

    while (i > 0 && j > 0) {
        if (s1[i-1] == s2[j-1]) {
            lcs = s1[i-1] + lcs;
            i--; j--;
        } else if (dp[i-1][j] > dp[i][j-1]) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}

// Longest Common Substring
int longestCommonSubstring(string s1, string s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));
    int result = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                result = max(result, dp[i][j]);
            }
        }
    }

    return result;
}

// Edit Distance
int minDistance(string word1, string word2) {
    int m = word1.length();
    int n = word2.length();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    // Base cases
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i-1] == word2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = 1 + min({
                    dp[i-1][j],   // Delete
                    dp[i][j-1],   // Insert
                    dp[i-1][j-1]  // Replace
                });
            }
        }
    }

    return dp[m][n];
}

// Main function with examples
int main() {
    // 0/1 Knapsack example
    vector<int> weights = {1, 3, 4, 5};
    vector<int> values = {1, 4, 5, 7};
    int capacity = 7;

    cout << "0/1 Knapsack Maximum Value: " << knapsackDP(weights, values, capacity) << endl;

    vector<int> selected = getKnapsackItems(weights, values, capacity);
    cout << "Selected items: ";
    for (int idx : selected) {
        cout << idx << " ";
    }
    cout << endl;

    // LCS example
    string s1 = "ABCBDAB";
    string s2 = "BDCABA";

    cout << "Length of LCS: " << lcsDP(s1, s2) << endl;
    cout << "LCS: " << printLCS(s1, s2) << endl;

    // Edit Distance example
    string word1 = "horse";
    string word2 = "ros";

    cout << "Edit Distance: " << minDistance(word1, word2) << endl;

    return 0;
}
```

## Edge Cases & Optimizations

### Edge Cases in Dynamic Programming

1. **Empty Inputs**: Always handle cases where one or both inputs are empty:

   - Knapsack: Empty items list or zero capacity
   - LCS/Edit Distance: Empty string(s)

2. **Negative Values**: In knapsack problems, we typically assume non-negative weights and values.

3. **Large Inputs**: Be careful about time and space complexity with large inputs:

   - In the knapsack problem, if the weights are large but the number of items is small, consider a different DP formulation using the sum of values as the state.
   - In string problems, if one string is much longer than the other, make sure to use the shorter string for the inner loop in space-optimized versions.

4. **Overflow**: For large inputs, watch out for integer overflow:
   - In the knapsack problem, if the sum of values is large, use `long long` instead of `int`.
   - In string problems, if the strings are very long, be careful about the DP table size.

### Performance Optimizations

1. **Space Optimization**:

   - Knapsack: Reduce from 2D array to 1D array by updating in reverse order.
   - LCS/Edit Distance: Use two rows instead of the full matrix.

2. **Precomputation**:

   - If you need to solve similar problems multiple times, precompute and reuse results.

3. **Pruning**:

   - In recursive solutions, add additional checks to prune branches that cannot lead to optimal solutions.

4. **Bitmask DP**:

   - For problems with small state spaces, consider using bitmasks to represent states, which can be more efficient.

5. **Branch and Bound**:

   - In knapsack, sort items by value/weight ratio and use greedy choices to establish bounds for pruning.

6. **Custom Data Structures**:
   - Use sparse tables or segment trees for problems involving range queries.

## Real-world Applications

### 1. Resource Allocation in Finance and Economics

The knapsack problem directly models portfolio optimization, where:

- Items are potential investments
- Values are expected returns
- Weights are investment costs or risks
- Capacity is the total budget or risk tolerance

For example, hedge funds use sophisticated DP algorithms to select optimal combinations of assets, considering expected returns, volatility, and correlations, while respecting investment constraints.

### 2. DNA Sequence Alignment in Bioinformatics

The LCS and Edit Distance algorithms are fundamental to bioinformatics:

- Genome sequencing uses DP to align DNA fragments
- Protein matching uses LCS to find structural similarities
- Evolutionary distance between species is calculated using Edit Distance

BLAST (Basic Local Alignment Search Tool), one of the most widely used bioinformatics tools, employs optimized versions of sequence alignment algorithms to rapidly search genetic databases.

### 3. Natural Language Processing

String-related DP algorithms power numerous NLP applications:

- Spell checking uses Edit Distance to find the closest match to misspelled words
- Machine translation leverages LCS for alignment between languages
- Text summarization uses DP to find common passages across multiple documents
- Plagiarism detection compares document similarity using string matching algorithms

Google's translation services initially used statistical models heavily dependent on DP-based alignment algorithms before transitioning to neural approaches.

### 4. Inventory Management and Supply Chain Optimization

Variants of the knapsack problem are used to:

- Optimize warehouse storage by selecting which products to stock
- Determine optimal shipment configurations to maximize value while minimizing shipping costs
- Schedule manufacturing to maximize resource utilization

Amazon's fulfillment centers use DP-based algorithms to optimize picking routes, inventory placement, and packing decisions, handling millions of items daily.

### 5. Computer Graphics and Image Processing

DP algorithms are essential in graphics:

- Image resizing with content-aware techniques (seam carving)
- Texture synthesis for generating realistic textures
- Optimal triangulation for 3D meshes

Adobe Photoshop's "Content-Aware Fill" and "Content-Aware Scale" features use DP algorithms to intelligently modify images while preserving important visual elements.

## Humorous Elements

### The Knapsack Thief's Dilemma

A thief breaks into a tech conference and finds a room full of gadgets. He pulls out his knapsack and starts sweating:

"I can take the iPhone worth $1000 that weighs 0.5 pounds, or the Pixel worth $800 that weighs 0.4 pounds, or the Galaxy worth $900 that weighs 0.45 pounds... but I can only carry 2 pounds total! How do I maximize my loot?"

The thief scratches his head for hours, drawing tables and filling in numbers.

Meanwhile, a programmer walks by and sees his work: "Ah, I see you're implementing bottom-up dynamic programming for the 0/1 knapsack problem! Your space complexity could be optimized though..."

The thief looks up, confused: "I'm just trying to steal some phones, man."

The programmer: "You're about to steal from a tech conference. The least you could do is use efficient algorithms!"

### The LCS Dating App

New dating app pitch: "LCSMatch"

"We don't just match people with common interests - we find the Longest Common Subsequence of your life experiences! Whether your lives intersected in the right order is all that matters."

Marketing slogan: "Because sometimes, it's not about what you have in common, but the order in which you experienced it."

User testimonial: "My previous boyfriend and I both loved cooking, traveling, and dogs, but the sequence was all wrong! With LCSMatch, I found someone whose life experiences align perfectly with mine. Our overlapping life events form an optimal subsequence!"

Programmer critique: "The matching algorithm runs in O(n²) time where n is the length of your dating history. Users with extensive dating histories may experience delays."

### The Edit Distance Relationship Counseling

A couple visits a relationship counselor who specializes in "Edit Distance Therapy."

Counselor: "Today we're going to calculate the minimum number of operations needed to transform your relationship into a healthy one."

Husband: "Operations?"

Counselor: "Yes! Insertions of date nights, deletions of bad habits, and substitutions of arguments with constructive conversations."

Wife: "I think we need at least 20 operations."

Husband: "That many?"

Counselor: "Let's create a dynamic programming table to find the optimal solution. The good news is, even the most distant relationships can be transformed with the right sequence of edits!"

Husband whispers to wife: "I think I prefer our previous counselor who just told us to communicate better."

Wife: "True, but this one has a whiteboard with colored markers. It's much more scientific."

## Practice Problems

### Easy: Partition Equal Subset Sum (LeetCode 416)

**Problem**: Given a non-empty array of positive integers nums, determine if the array can be partitioned into two subsets such that the sum of elements in both subsets is equal.

**Approach Hint**:

- This is a variation of the Subset Sum problem
- The problem is equivalent to: Can we find a subset whose sum is exactly half of the total sum?
- If the total sum is odd, we can immediately return false
- Create a DP table where dp[i][j] = true if we can make sum j using first i elements
- Final answer is dp[n][sum/2]
- Can be optimized to use 1D DP array

### Medium: Coin Change 2 (LeetCode 518)

**Problem**: You are given coins of different denominations and a total amount of money. Find the number of combinations that make up that amount.

**Approach Hint**:

- Let dp[i][j] be the number of ways to make amount j using first i coins
- Base case: dp[i][0] = 1 (one way to make amount 0 - use no coins)
- If coin_i <= j, then dp[i][j] = dp[i-1][j] + dp[i][j-coin_i]
- Otherwise, dp[i][j] = dp[i-1][j]
- The order of loops matters! To count combinations (not permutations), outer loop should be coins
- Can be optimized to 1D array: dp[j] += dp[j - coin]

### Hard: Distinct Subsequences (LeetCode 115)

**Problem**: Given two strings s and t, find the number of distinct subsequences of s which equals t.

**Approach Hint**:

- Let dp[i][j] = number of ways to form t[0...j-1] using s[0...i-1]
- Base cases:
  - dp[i][0] = 1 (one way to form empty string - use no characters)
  - dp[0][j] = 0 for j > 0 (can't form non-empty string from empty string)
- If s[i-1] == t[j-1], then dp[i][j] = dp[i-1][j-1] + dp[i-1][j]
- Otherwise, dp[i][j] = dp[i-1][j]
- Be careful about integer overflow for large strings
- The recurrence relation can be understood as: either use s[i-1] to match t[j-1], or don't use it

## Preview of Day 38

Tomorrow, we'll shift our focus to Greedy Algorithms, which always make the locally optimal choice at each stage. We'll cover:

- Principles of greedy algorithms and when they work
- Proof techniques for greedy algorithms
- Standard greedy problems like activity selection and Huffman coding
- Comparison with dynamic programming approaches
- Cases where greedy approaches fail

Greedy algorithms are often simpler and more efficient than DP, but they don't always find the optimal solution. We'll learn how to identify when a greedy approach is guaranteed to work and when we need the more comprehensive approach of dynamic programming.

## Conclusion

Today, we've delved deeper into Dynamic Programming, exploring complex problems like the Knapsack problem and string-related DP algorithms. These problems showcase the true power of breaking down a complex problem into overlapping subproblems and building the optimal solution from the ground up.

The key insights from today's lesson:

1. The 0/1 Knapsack problem exemplifies resource allocation with constraints and serves as a foundation for many optimization problems.

2. String-related DP problems like Longest Common Subsequence and Edit Distance have profound applications in fields from bioinformatics to natural language processing.

3. Space optimization techniques can drastically reduce the memory requirements of DP solutions, often from O(n²) to O(n).

4. Backtracking through the DP table allows us to reconstruct the optimal solution, not just find its value.

5. Real-world applications of these DP techniques span across finance, biology, language processing, and many other fields.

Dynamic Programming is not just a theoretical tool; it's a practical approach to solving complex optimization problems that appear in various domains. By mastering these techniques, you're equipped to tackle a wide range of algorithmic challenges efficiently.

As we continue our #DSAin45 journey, remember that the principles we've learned in DP - breaking down problems, storing intermediate results, and building optimal solutions - will serve as foundations for many other algorithmic paradigms.

#DSAin45 #DynamicProgramming #Algorithms #Knapsack #LongestCommonSubsequence #EditDistance #CPlusPlus #ProblemSolving
