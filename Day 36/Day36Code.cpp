/**
 * @file dynamic_programming_part1.cpp
 * @brief Implementation of Dynamic Programming fundamentals and classic problems
 * @author DSAin45 Series
 * @date May 5, 2025
 *
 * This file contains comprehensive implementations of dynamic programming techniques:
 * - Fibonacci sequence with different approaches (naive, memoization, tabulation)
 * - Coin change problem (minimum coins and counting combinations)
 * - Climbing stairs problem and variations
 * - Longest increasing subsequence
 * - Benchmarking utilities for comparing algorithm performance
 *
 * Each implementation includes both naive and optimized solutions to demonstrate
 * the power of dynamic programming in improving time complexity.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <functional>
#include <numeric>
#include <random>
#include <fstream>

// Define constants and types
using std::cout;
using std::endl;
using std::string;
using std::vector;
const int INF = std::numeric_limits<int>::max();

/**
 * @brief Utility class for benchmarking and timing functions
 */
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::string name;

public:
    Timer(const std::string &name = "") : name(name)
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        stop();
    }

    double stop()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        double ms = duration / 1000.0;

        if (!name.empty())
        {
            std::cout << name << " took " << ms << " ms" << std::endl;
        }

        return ms;
    }
};

/**
 * @brief Class containing Fibonacci algorithms with different approaches
 */
class Fibonacci
{
public:
    /**
     * @brief Naive recursive implementation of Fibonacci
     * @param n The position in the Fibonacci sequence to compute
     * @return The nth Fibonacci number
     * @time O(2^n) - exponential time complexity
     * @space O(n) - call stack depth
     */
    static long long naive(int n)
    {
        if (n <= 1)
            return n;
        return naive(n - 1) + naive(n - 2);
    }

    /**
     * @brief Memoized (top-down) implementation of Fibonacci
     * @param n The position in the Fibonacci sequence to compute
     * @param memo Memoization array to store intermediate results
     * @return The nth Fibonacci number
     * @time O(n) - linear time complexity
     * @space O(n) - memoization array + call stack
     */
    static long long memoized(int n, vector<long long> &memo)
    {
        if (n <= 1)
            return n;

        if (memo[n] != -1)
            return memo[n];

        memo[n] = memoized(n - 1, memo) + memoized(n - 2, memo);
        return memo[n];
    }

    /**
     * @brief Wrapper for memoized implementation
     * @param n The position in the Fibonacci sequence to compute
     * @return The nth Fibonacci number
     */
    static long long memoized(int n)
    {
        vector<long long> memo(n + 1, -1);
        return memoized(n, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation of Fibonacci
     * @param n The position in the Fibonacci sequence to compute
     * @return The nth Fibonacci number
     * @time O(n) - linear time complexity
     * @space O(n) - tabulation array
     */
    static long long tabulation(int n)
    {
        if (n <= 1)
            return n;

        vector<long long> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;

        for (int i = 2; i <= n; i++)
        {
            dp[i] = dp[i - 1] + dp[i - 2];
        }

        return dp[n];
    }

    /**
     * @brief Space-optimized implementation of Fibonacci
     * @param n The position in the Fibonacci sequence to compute
     * @return The nth Fibonacci number
     * @time O(n) - linear time complexity
     * @space O(1) - constant space complexity
     */
    static long long optimized(int n)
    {
        if (n <= 1)
            return n;

        long long prev = 0;
        long long curr = 1;

        for (int i = 2; i <= n; i++)
        {
            long long next = prev + curr;
            prev = curr;
            curr = next;
        }

        return curr;
    }

    /**
     * @brief Matrix exponentiation implementation for Fibonacci
     * @param n The position in the Fibonacci sequence to compute
     * @return The nth Fibonacci number
     * @time O(log n) - logarithmic time complexity
     * @space O(1) - constant space complexity
     */
    static long long matrix_exponentiation(int n)
    {
        if (n <= 1)
            return n;

        // Base matrix [[1, 1], [1, 0]]
        vector<vector<long long>> base = {{1, 1}, {1, 0}};

        // Compute base^(n-1)
        auto result = matrix_power(base, n - 1);

        // F(n) is the element at position [0][0]
        return result[0][0];
    }

private:
    /**
     * @brief Helper function for matrix exponentiation
     * @param base The base matrix
     * @param exponent The exponent
     * @return The matrix after exponentiation
     */
    static vector<vector<long long>> matrix_power(const vector<vector<long long>> &base, int exponent)
    {
        if (exponent == 1)
            return base;
        if (exponent == 0)
            return {{1, 0}, {0, 1}}; // Identity matrix

        auto half_power = matrix_power(base, exponent / 2);
        auto result = matrix_multiply(half_power, half_power);

        if (exponent % 2 == 1)
        {
            result = matrix_multiply(result, base);
        }

        return result;
    }

    /**
     * @brief Helper function for matrix multiplication
     * @param a First 2x2 matrix
     * @param b Second 2x2 matrix
     * @return The product of the two matrices
     */
    static vector<vector<long long>> matrix_multiply(const vector<vector<long long>> &a,
                                                     const vector<vector<long long>> &b)
    {
        vector<vector<long long>> result(2, vector<long long>(2, 0));

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    result[i][j] += a[i][k] * b[k][j];
                }
            }
        }

        return result;
    }
};

/**
 * @brief Class containing Coin Change problem implementations
 */
class CoinChange
{
public:
    /**
     * @brief Naive recursive implementation to find minimum coins
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @return Minimum number of coins needed, or -1 if impossible
     * @time O(amount^n) where n is the number of coin denominations
     * @space O(amount) - recursion stack depth
     */
    static int minCoinsNaive(const vector<int> &coins, int amount)
    {
        // Base case: If amount is 0, no coins needed
        if (amount == 0)
            return 0;

        // Initialize result with a value larger than any valid answer
        int result = INF;

        // Try each coin denomination
        for (int coin : coins)
        {
            if (coin <= amount)
            {
                int subResult = minCoinsNaive(coins, amount - coin);

                // If a valid solution was found with this coin
                if (subResult != INF && subResult + 1 < result)
                {
                    result = subResult + 1;
                }
            }
        }

        return (result == INF) ? -1 : result;
    }

    /**
     * @brief Memoized (top-down) implementation to find minimum coins
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @param memo Memoization array to store intermediate results
     * @return Minimum number of coins needed, or -1 if impossible
     * @time O(amount * n) where n is the number of coin denominations
     * @space O(amount) - memoization array + recursion stack
     */
    static int minCoinsMemoized(const vector<int> &coins, int amount, vector<int> &memo)
    {
        // Base case: If amount is 0, no coins needed
        if (amount == 0)
            return 0;

        // If already computed, return stored result
        if (memo[amount] != -1)
            return memo[amount];

        // Initialize result with a value larger than any valid answer
        int result = INF;

        // Try each coin denomination
        for (int coin : coins)
        {
            if (coin <= amount)
            {
                int subResult = minCoinsMemoized(coins, amount - coin, memo);

                // If a valid solution was found with this coin
                if (subResult != INF && subResult + 1 < result)
                {
                    result = subResult + 1;
                }
            }
        }

        // Store and return the result
        memo[amount] = (result == INF) ? -1 : result;
        return memo[amount];
    }

    /**
     * @brief Wrapper for memoized implementation to find minimum coins
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @return Minimum number of coins needed, or -1 if impossible
     */
    static int minCoinsMemoized(const vector<int> &coins, int amount)
    {
        vector<int> memo(amount + 1, -1);
        return minCoinsMemoized(coins, amount, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation to find minimum coins
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @return Minimum number of coins needed, or -1 if impossible
     * @time O(amount * n) where n is the number of coin denominations
     * @space O(amount) - tabulation array
     */
    static int minCoinsTabulation(const vector<int> &coins, int amount)
    {
        // Initialize dp array with a value larger than any valid answer
        vector<int> dp(amount + 1, INF);

        // Base case: 0 coins needed to make amount 0
        dp[0] = 0;

        // Fill the dp array bottom-up
        for (int i = 1; i <= amount; i++)
        {
            // Try each coin denomination
            for (int coin : coins)
            {
                if (coin <= i && dp[i - coin] != INF)
                {
                    dp[i] = std::min(dp[i], dp[i - coin] + 1);
                }
            }
        }

        return (dp[amount] == INF) ? -1 : dp[amount];
    }

    /**
     * @brief Returns the actual coins used in the minimum coin solution
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @return Vector of coins used in the solution
     */
    static vector<int> getCoinsUsed(const vector<int> &coins, int amount)
    {
        vector<int> dp(amount + 1, INF);
        vector<int> coinUsed(amount + 1, -1);

        dp[0] = 0;

        for (int i = 1; i <= amount; i++)
        {
            for (int j = 0; j < coins.size(); j++)
            {
                if (coins[j] <= i && dp[i - coins[j]] != INF && dp[i - coins[j]] + 1 < dp[i])
                {
                    dp[i] = dp[i - coins[j]] + 1;
                    coinUsed[i] = j;
                }
            }
        }

        if (dp[amount] == INF)
            return {};

        vector<int> result;
        int remaining = amount;

        while (remaining > 0)
        {
            result.push_back(coins[coinUsed[remaining]]);
            remaining -= coins[coinUsed[remaining]];
        }

        return result;
    }

    /**
     * @brief Compute the number of ways to make change
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @return Number of different combinations that make up the amount
     * @time O(amount * n) where n is the number of coin denominations
     * @space O(amount) - tabulation array
     */
    static int countWays(const vector<int> &coins, int amount)
    {
        vector<int> dp(amount + 1, 0);

        // Base case: there is 1 way to make amount 0 (using no coins)
        dp[0] = 1;

        // For each coin, update the ways to make different amounts
        for (int coin : coins)
        {
            for (int i = coin; i <= amount; i++)
            {
                dp[i] += dp[i - coin];
            }
        }

        return dp[amount];
    }
};

/**
 * @brief Class containing Climbing Stairs problem implementations
 */
class ClimbingStairs
{
public:
    /**
     * @brief Naive recursive implementation for climbing stairs
     * @param n Number of steps in the staircase
     * @return Number of distinct ways to climb to the top
     * @time O(2^n) - exponential time complexity
     * @space O(n) - recursion stack depth
     */
    static int naive(int n)
    {
        if (n <= 2)
            return n;
        return naive(n - 1) + naive(n - 2);
    }

    /**
     * @brief Memoized (top-down) implementation for climbing stairs
     * @param n Number of steps in the staircase
     * @param memo Memoization array to store intermediate results
     * @return Number of distinct ways to climb to the top
     * @time O(n) - linear time complexity
     * @space O(n) - memoization array + recursion stack
     */
    static int memoized(int n, vector<int> &memo)
    {
        if (n <= 2)
            return n;

        if (memo[n] != -1)
            return memo[n];

        memo[n] = memoized(n - 1, memo) + memoized(n - 2, memo);
        return memo[n];
    }

    /**
     * @brief Wrapper for memoized implementation
     * @param n Number of steps in the staircase
     * @return Number of distinct ways to climb to the top
     */
    static int memoized(int n)
    {
        vector<int> memo(n + 1, -1);
        return memoized(n, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation for climbing stairs
     * @param n Number of steps in the staircase
     * @return Number of distinct ways to climb to the top
     * @time O(n) - linear time complexity
     * @space O(n) - tabulation array
     */
    static int tabulation(int n)
    {
        if (n <= 2)
            return n;

        vector<int> dp(n + 1);
        dp[1] = 1;
        dp[2] = 2;

        for (int i = 3; i <= n; i++)
        {
            dp[i] = dp[i - 1] + dp[i - 2];
        }

        return dp[n];
    }

    /**
     * @brief Space-optimized implementation for climbing stairs
     * @param n Number of steps in the staircase
     * @return Number of distinct ways to climb to the top
     * @time O(n) - linear time complexity
     * @space O(1) - constant space complexity
     */
    static int optimized(int n)
    {
        if (n <= 2)
            return n;

        int prev = 1;
        int curr = 2;

        for (int i = 3; i <= n; i++)
        {
            int next = prev + curr;
            prev = curr;
            curr = next;
        }

        return curr;
    }

    /**
     * @brief Generalized version: can take 1 to k steps at a time
     * @param n Number of steps in the staircase
     * @param k Maximum number of steps that can be climbed at once
     * @return Number of distinct ways to climb to the top
     * @time O(n * k) - linear time complexity w.r.t. n and k
     * @space O(n) - tabulation array
     */
    static int withVariableSteps(int n, int k)
    {
        vector<int> dp(n + 1, 0);
        dp[0] = 1; // Base case: one way to climb 0 steps

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= k && i - j >= 0; j++)
            {
                dp[i] += dp[i - j];
            }
        }

        return dp[n];
    }

    /**
     * @brief Variation: Min cost climbing stairs (each step has a cost)
     * @param cost Vector where cost[i] is the cost of the i-th step
     * @return Minimum cost to reach the top
     * @time O(n) - linear time complexity
     * @space O(1) - constant space complexity
     */
    static int minCost(const vector<int> &cost)
    {
        int n = cost.size();

        // Can start from either step 0 or step 1
        int prev = cost[0];
        int curr = cost[1];

        for (int i = 2; i < n; i++)
        {
            int next = cost[i] + std::min(prev, curr);
            prev = curr;
            curr = next;
        }

        // Last step to reach the top
        return std::min(prev, curr);
    }
};

/**
 * @brief Class containing Longest Increasing Subsequence implementations
 */
class LongestIncreasingSubsequence
{
public:
    /**
     * @brief Naive recursive implementation for LIS
     * @param nums Input array
     * @param curr Current index
     * @param prev Previous element index (-1 for first call)
     * @return Length of longest increasing subsequence
     * @time O(2^n) - exponential time complexity
     * @space O(n) - recursion stack depth
     */
    static int naiveRecursive(const vector<int> &nums, int curr, int prev)
    {
        // Base case: reached the end of array
        if (curr == nums.size())
            return 0;

        // Option 1: Don't include current element
        int exclude = naiveRecursive(nums, curr + 1, prev);

        // Option 2: Include current element if it forms an increasing subsequence
        int include = 0;
        if (prev == -1 || nums[curr] > nums[prev])
        {
            include = 1 + naiveRecursive(nums, curr + 1, curr);
        }

        // Return the better option
        return std::max(exclude, include);
    }

    /**
     * @brief Wrapper for naive recursive implementation
     * @param nums Input array
     * @return Length of longest increasing subsequence
     */
    static int naive(const vector<int> &nums)
    {
        return naiveRecursive(nums, 0, -1);
    }

    /**
     * @brief Memoized (top-down) implementation for LIS
     * @param nums Input array
     * @param curr Current index
     * @param prev Previous element index (-1 for first call)
     * @param memo 2D memoization array
     * @return Length of longest increasing subsequence
     * @time O(n^2) - quadratic time complexity
     * @space O(n^2) - memoization array + recursion stack
     */
    static int memoizedRecursive(const vector<int> &nums, int curr, int prev, vector<vector<int>> &memo)
    {
        // Base case: reached the end of array
        if (curr == nums.size())
            return 0;

        // If already computed, return stored result
        if (memo[curr][prev + 1] != -1)
            return memo[curr][prev + 1];

        // Option 1: Don't include current element
        int exclude = memoizedRecursive(nums, curr + 1, prev, memo);

        // Option 2: Include current element if it forms an increasing subsequence
        int include = 0;
        if (prev == -1 || nums[curr] > nums[prev])
        {
            include = 1 + memoizedRecursive(nums, curr + 1, curr, memo);
        }

        // Store and return the better option
        memo[curr][prev + 1] = std::max(exclude, include);
        return memo[curr][prev + 1];
    }

    /**
     * @brief Wrapper for memoized implementation
     * @param nums Input array
     * @return Length of longest increasing subsequence
     */
    static int memoized(const vector<int> &nums)
    {
        int n = nums.size();
        vector<vector<int>> memo(n, vector<int>(n + 1, -1));
        return memoizedRecursive(nums, 0, -1, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation for LIS
     * @param nums Input array
     * @return Length of longest increasing subsequence
     * @time O(n^2) - quadratic time complexity
     * @space O(n) - tabulation array
     */
    static int tabulation(const vector<int> &nums)
    {
        int n = nums.size();
        if (n == 0)
            return 0;

        // dp[i] = length of LIS ending at index i
        vector<int> dp(n, 1);

        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (nums[i] > nums[j])
                {
                    dp[i] = std::max(dp[i], dp[j] + 1);
                }
            }
        }

        return *std::max_element(dp.begin(), dp.end());
    }

    /**
     * @brief Binary search implementation for LIS
     * @param nums Input array
     * @return Length of longest increasing subsequence
     * @time O(n log n) - linearithmic time complexity
     * @space O(n) - auxiliary array
     */
    static int binarySearch(const vector<int> &nums)
    {
        int n = nums.size();
        if (n == 0)
            return 0;

        // tails[i] = smallest value that ends an increasing subsequence of length i+1
        vector<int> tails;

        for (int num : nums)
        {
            // Binary search to find the position to insert num
            auto it = std::lower_bound(tails.begin(), tails.end(), num);

            if (it == tails.end())
            {
                // If num is greater than all values in tails, extend the sequence
                tails.push_back(num);
            }
            else
            {
                // Replace the smallest value >= num
                *it = num;
            }
        }

        return tails.size();
    }

    /**
     * @brief Returns the actual longest increasing subsequence
     * @param nums Input array
     * @return Vector containing the longest increasing subsequence
     */
    static vector<int> getLIS(const vector<int> &nums)
    {
        int n = nums.size();
        if (n == 0)
            return {};

        // dp[i] = length of LIS ending at index i
        vector<int> dp(n, 1);
        // prev[i] = previous index in the LIS ending at i
        vector<int> prev(n, -1);

        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (nums[i] > nums[j] && dp[j] + 1 > dp[i])
                {
                    dp[i] = dp[j] + 1;
                    prev[i] = j;
                }
            }
        }

        // Find index with maximum LIS length
        int maxLength = 0;
        int lastIndex = -1;

        for (int i = 0; i < n; i++)
        {
            if (dp[i] > maxLength)
            {
                maxLength = dp[i];
                lastIndex = i;
            }
        }

        // Reconstruct the LIS
        vector<int> lis;
        while (lastIndex != -1)
        {
            lis.push_back(nums[lastIndex]);
            lastIndex = prev[lastIndex];
        }

        std::reverse(lis.begin(), lis.end());
        return lis;
    }
};

/**
 * @brief Class containing Triangle problem implementation
 * Given a triangle array, find the minimum path sum from top to bottom.
 */
class Triangle
{
public:
    /**
     * @brief Memoized (top-down) implementation for Triangle problem
     * @param triangle Triangle represented as a vector of vectors
     * @param i Current row
     * @param j Current column
     * @param memo Memoization array
     * @return Minimum path sum from top to the current position
     * @time O(n^2) where n is the number of rows
     * @space O(n^2) - memoization array + recursion stack
     */
    static int memoizedRecursive(const vector<vector<int>> &triangle, int i, int j,
                                 vector<vector<int>> &memo)
    {
        // Base case: reached bottom row
        if (i == triangle.size())
            return 0;

        // If already computed, return stored result
        if (memo[i][j] != -1)
            return memo[i][j];

        // Choose the minimum of the two possible next positions
        int pathSum = triangle[i][j] + std::min(
                                           memoizedRecursive(triangle, i + 1, j, memo),
                                           memoizedRecursive(triangle, i + 1, j + 1, memo));

        // Store and return the result
        memo[i][j] = pathSum;
        return pathSum;
    }

    /**
     * @brief Wrapper for memoized implementation
     * @param triangle Triangle represented as a vector of vectors
     * @return Minimum path sum from top to bottom
     */
    static int memoized(const vector<vector<int>> &triangle)
    {
        int n = triangle.size();
        vector<vector<int>> memo(n, vector<int>(n, -1));
        return memoizedRecursive(triangle, 0, 0, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation for Triangle problem
     * @param triangle Triangle represented as a vector of vectors
     * @return Minimum path sum from top to bottom
     * @time O(n^2) where n is the number of rows
     * @space O(n^2) - tabulation array
     */
    static int tabulation(const vector<vector<int>> &triangle)
    {
        int n = triangle.size();

        // dp[i][j] = minimum path sum from top to position (i,j)
        vector<vector<int>> dp = triangle;

        // Start from the second last row and build up
        for (int i = n - 2; i >= 0; i--)
        {
            for (int j = 0; j <= i; j++)
            {
                dp[i][j] += std::min(dp[i + 1][j], dp[i + 1][j + 1]);
            }
        }

        return dp[0][0];
    }

    /**
     * @brief Space-optimized implementation for Triangle problem
     * @param triangle Triangle represented as a vector of vectors
     * @return Minimum path sum from top to bottom
     * @time O(n^2) where n is the number of rows
     * @space O(n) - single row for dp computation
     */
    static int optimized(const vector<vector<int>> &triangle)
    {
        int n = triangle.size();

        // Initialize dp with the bottom row
        vector<int> dp = triangle[n - 1];

        // Work our way up
        for (int i = n - 2; i >= 0; i--)
        {
            for (int j = 0; j <= i; j++)
            {
                dp[j] = triangle[i][j] + std::min(dp[j], dp[j + 1]);
            }
        }

        return dp[0];
    }
};

/**
 * @brief Class containing Maximum Subarray implementation
 * Finding the contiguous subarray with the largest sum.
 */
class MaximumSubarray
{
public:
    /**
     * @brief Naive approach checking all subarrays
     * @param nums Input array
     * @return Maximum sum of a contiguous subarray
     * @time O(n^3) - cubic time complexity
     * @space O(1) - constant space complexity
     */
    static int bruteForce(const vector<int> &nums)
    {
        int n = nums.size();
        int maxSum = nums[0];

        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                int sum = 0;
                for (int k = i; k <= j; k++)
                {
                    sum += nums[k];
                }
                maxSum = std::max(maxSum, sum);
            }
        }

        return maxSum;
    }

    /**
     * @brief Slightly optimized approach with prefix sums
     * @param nums Input array
     * @return Maximum sum of a contiguous subarray
     * @time O(n^2) - quadratic time complexity
     * @space O(1) - constant space complexity
     */
    static int prefixSum(const vector<int> &nums)
    {
        int n = nums.size();
        int maxSum = nums[0];

        for (int i = 0; i < n; i++)
        {
            int sum = 0;
            for (int j = i; j < n; j++)
            {
                sum += nums[j];
                maxSum = std::max(maxSum, sum);
            }
        }

        return maxSum;
    }

    /**
     * @brief Kadane's algorithm - dynamic programming approach
     * @param nums Input array
     * @return Maximum sum of a contiguous subarray
     * @time O(n) - linear time complexity
     * @space O(1) - constant space complexity
     */
    static int kadane(const vector<int> &nums)
    {
        int n = nums.size();
        int currentSum = nums[0];
        int maxSum = nums[0];

        for (int i = 1; i < n; i++)
        {
            // Either start a new subarray or extend the existing one
            currentSum = std::max(nums[i], currentSum + nums[i]);
            maxSum = std::max(maxSum, currentSum);
        }

        return maxSum;
    }

    /**
     * @brief Returns the actual maximum subarray
     * @param nums Input array
     * @return Pair containing the maximum sum and the subarray indices
     */
    static std::pair<int, std::pair<int, int>> getMaxSubarray(const vector<int> &nums)
    {
        int n = nums.size();
        int currentSum = nums[0];
        int maxSum = nums[0];
        int start = 0;
        int end = 0;
        int tempStart = 0;

        for (int i = 1; i < n; i++)
        {
            if (nums[i] > currentSum + nums[i])
            {
                // Start a new subarray
                currentSum = nums[i];
                tempStart = i;
            }
            else
            {
                // Extend the existing subarray
                currentSum = currentSum + nums[i];
            }

            if (currentSum > maxSum)
            {
                maxSum = currentSum;
                start = tempStart;
                end = i;
            }
        }

        return {maxSum, {start, end}};
    }
};

/**
 * @brief Class containing 0/1 Knapsack implementation
 * Classic DP problem: selecting items with weights and values to maximize value within weight constraint
 */
class Knapsack
{
public:
    /**
     * @brief Naive recursive implementation
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @param n Number of items to consider
     * @return Maximum value that can be put in knapsack
     * @time O(2^n) - exponential time complexity
     * @space O(n) - recursion stack depth
     */
    static int naiveRecursive(const vector<int> &weights, const vector<int> &values,
                              int capacity, int n)
    {
        // Base case: no items or no capacity
        if (n == 0 || capacity == 0)
            return 0;

        // If weight of nth item is more than capacity, exclude it
        if (weights[n - 1] > capacity)
        {
            return naiveRecursive(weights, values, capacity, n - 1);
        }

        // Return the maximum of two cases:
        // 1. nth item included
        // 2. nth item excluded
        return std::max(
            values[n - 1] + naiveRecursive(weights, values, capacity - weights[n - 1], n - 1),
            naiveRecursive(weights, values, capacity, n - 1));
    }

    /**
     * @brief Wrapper for naive recursive implementation
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be put in knapsack
     */
    static int naive(const vector<int> &weights, const vector<int> &values, int capacity)
    {
        return naiveRecursive(weights, values, capacity, weights.size());
    }

    /**
     * @brief Memoized (top-down) implementation
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @param n Number of items to consider
     * @param memo Memoization table
     * @return Maximum value that can be put in knapsack
     * @time O(n * capacity) - polynomial time complexity
     * @space O(n * capacity) - memoization table + recursion stack
     */
    static int memoizedRecursive(const vector<int> &weights, const vector<int> &values,
                                 int capacity, int n, vector<vector<int>> &memo)
    {
        // Base case: no items or no capacity
        if (n == 0 || capacity == 0)
            return 0;

        // If already computed, return stored result
        if (memo[n][capacity] != -1)
            return memo[n][capacity];

        // If weight of nth item is more than capacity, exclude it
        if (weights[n - 1] > capacity)
        {
            memo[n][capacity] = memoizedRecursive(weights, values, capacity, n - 1, memo);
            return memo[n][capacity];
        }

        // Return the maximum of two cases:
        // 1. nth item included
        // 2. nth item excluded
        memo[n][capacity] = std::max(
            values[n - 1] + memoizedRecursive(weights, values, capacity - weights[n - 1], n - 1, memo),
            memoizedRecursive(weights, values, capacity, n - 1, memo));

        return memo[n][capacity];
    }

    /**
     * @brief Wrapper for memoized implementation
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be put in knapsack
     */
    static int memoized(const vector<int> &weights, const vector<int> &values, int capacity)
    {
        int n = weights.size();
        vector<vector<int>> memo(n + 1, vector<int>(capacity + 1, -1));
        return memoizedRecursive(weights, values, capacity, n, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be put in knapsack
     * @time O(n * capacity) - polynomial time complexity
     * @space O(n * capacity) - tabulation table
     */
    static int tabulation(const vector<int> &weights, const vector<int> &values, int capacity)
    {
        int n = weights.size();
        vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

        // Build table dp[][] in bottom-up manner
        for (int i = 1; i <= n; i++)
        {
            for (int w = 1; w <= capacity; w++)
            {
                if (weights[i - 1] <= w)
                {
                    dp[i][w] = std::max(
                        values[i - 1] + dp[i - 1][w - weights[i - 1]],
                        dp[i - 1][w]);
                }
                else
                {
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }

        return dp[n][capacity];
    }

    /**
     * @brief Space-optimized tabulation implementation
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be put in knapsack
     * @time O(n * capacity) - polynomial time complexity
     * @space O(capacity) - single row for dp computation
     */
    static int optimized(const vector<int> &weights, const vector<int> &values, int capacity)
    {
        int n = weights.size();
        vector<int> dp(capacity + 1, 0);

        for (int i = 0; i < n; i++)
        {
            for (int w = capacity; w >= weights[i]; w--)
            {
                dp[w] = std::max(dp[w], values[i] + dp[w - weights[i]]);
            }
        }

        return dp[capacity];
    }

    /**
     * @brief Returns the items selected in the optimal solution
     * @param weights Vector of item weights
     * @param values Vector of item values
     * @param capacity Knapsack capacity
     * @return Vector of indices of selected items
     */
    static vector<int> getSelectedItems(const vector<int> &weights, const vector<int> &values, int capacity)
    {
        int n = weights.size();
        vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

        // Build table dp[][] in bottom-up manner
        for (int i = 1; i <= n; i++)
        {
            for (int w = 1; w <= capacity; w++)
            {
                if (weights[i - 1] <= w)
                {
                    dp[i][w] = std::max(
                        values[i - 1] + dp[i - 1][w - weights[i - 1]],
                        dp[i - 1][w]);
                }
                else
                {
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }

        // Backtrack to find selected items
        vector<int> selectedItems;
        int w = capacity;

        for (int i = n; i > 0 && w > 0; i--)
        {
            // If the result comes from including the current item
            if (dp[i][w] != dp[i - 1][w])
            {
                selectedItems.push_back(i - 1);
                w -= weights[i - 1];
            }
        }

        return selectedItems;
    }
};

/**
 * @brief Class for generating and benchmarking DP examples
 */
class DynamicProgrammingExamples
{
public:
    /**
     * @brief Generate random array of specified size
     * @param size Size of the array
     * @param min Minimum value
     * @param max Maximum value
     * @return Random integer array
     */
    static vector<int> generateRandomArray(int size, int min, int max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);

        vector<int> arr(size);
        for (int i = 0; i < size; i++)
        {
            arr[i] = dis(gen);
        }

        return arr;
    }

    /**
     * @brief Generate random triangle of specified size
     * @param rows Number of rows in the triangle
     * @param min Minimum value
     * @param max Maximum value
     * @return Random triangle
     */
    static vector<vector<int>> generateRandomTriangle(int rows, int min, int max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);

        vector<vector<int>> triangle(rows);
        for (int i = 0; i < rows; i++)
        {
            triangle[i].resize(i + 1);
            for (int j = 0; j <= i; j++)
            {
                triangle[i][j] = dis(gen);
            }
        }

        return triangle;
    }

    /**
     * @brief Generate random knapsack problem
     * @param items Number of items
     * @param maxWeight Maximum weight of any item
     * @param maxValue Maximum value of any item
     * @return Tuple of weights, values, and capacity
     */
    static std::tuple<vector<int>, vector<int>, int> generateRandomKnapsack(
        int items, int maxWeight, int maxValue)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> weightDis(1, maxWeight);
        std::uniform_int_distribution<> valueDis(1, maxValue);

        vector<int> weights(items);
        vector<int> values(items);

        for (int i = 0; i < items; i++)
        {
            weights[i] = weightDis(gen);
            values[i] = valueDis(gen);
        }

        int totalWeight = std::accumulate(weights.begin(), weights.end(), 0);
        int capacity = totalWeight / 2;

        return {weights, values, capacity};
    }

    /**
     * @brief Benchmark Fibonacci implementations
     */
    static void benchmarkFibonacci()
    {
        cout << "=== Fibonacci Benchmarks ===" << endl;

        for (int n : {10, 20, 30, 40})
        {
            cout << "n = " << n << ":" << endl;

            if (n <= 30)
            { // Naive is too slow for larger n
                {
                    Timer timer("  Naive recursive");
                    long long result = Fibonacci::naive(n);
                    cout << "    Result: " << result << endl;
                }
            }

            {
                Timer timer("  Memoized (top-down)");
                long long result = Fibonacci::memoized(n);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Tabulation (bottom-up)");
                long long result = Fibonacci::tabulation(n);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Space-optimized");
                long long result = Fibonacci::optimized(n);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Matrix exponentiation");
                long long result = Fibonacci::matrix_exponentiation(n);
                cout << "    Result: " << result << endl;
            }

            cout << endl;
        }
    }

    /**
     * @brief Benchmark Coin Change implementations
     */
    static void benchmarkCoinChange()
    {
        cout << "=== Coin Change Benchmarks ===" << endl;

        vector<vector<int>> testCases = {
            {1, 2, 5},         // Common US coins
            {1, 3, 4, 5},      // Mix of coins
            {2, 5, 10, 20, 50} // European coins
        };

        vector<int> amounts = {11, 30, 100};

        for (int i = 0; i < testCases.size(); i++)
        {
            cout << "Coin set " << (i + 1) << ": ";
            for (int coin : testCases[i])
            {
                cout << coin << " ";
            }
            cout << endl;

            for (int amount : amounts)
            {
                cout << "  Amount = " << amount << ":" << endl;

                if (amount <= 30)
                { // Naive is too slow for larger amounts
                    {
                        Timer timer("    Naive recursive");
                        int result = CoinChange::minCoinsNaive(testCases[i], amount);
                        cout << "      Result: " << result << endl;
                    }
                }

                {
                    Timer timer("    Memoized (top-down)");
                    int result = CoinChange::minCoinsMemoized(testCases[i], amount);
                    cout << "      Result: " << result << endl;
                }

                {
                    Timer timer("    Tabulation (bottom-up)");
                    int result = CoinChange::minCoinsTabulation(testCases[i], amount);
                    cout << "      Result: " << result << endl;
                }

                {
                    Timer timer("    Coins used");
                    vector<int> coinsUsed = CoinChange::getCoinsUsed(testCases[i], amount);
                    cout << "      Coins: ";
                    for (int coin : coinsUsed)
                    {
                        cout << coin << " ";
                    }
                    cout << endl;
                }

                {
                    Timer timer("    Count ways");
                    int result = CoinChange::countWays(testCases[i], amount);
                    cout << "      Number of ways: " << result << endl;
                }

                cout << endl;
            }
        }
    }

    /**
     * @brief Benchmark Climbing Stairs implementations
     */
    static void benchmarkClimbingStairs()
    {
        cout << "=== Climbing Stairs Benchmarks ===" << endl;

        for (int n : {10, 20, 30, 40})
        {
            cout << "n = " << n << ":" << endl;

            if (n <= 30)
            { // Naive is too slow for larger n
                {
                    Timer timer("  Naive recursive");
                    int result = ClimbingStairs::naive(n);
                    cout << "    Result: " << result << endl;
                }
            }

            {
                Timer timer("  Memoized (top-down)");
                int result = ClimbingStairs::memoized(n);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Tabulation (bottom-up)");
                int result = ClimbingStairs::tabulation(n);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Space-optimized");
                int result = ClimbingStairs::optimized(n);
                cout << "    Result: " << result << endl;
            }

            cout << endl;
        }

        cout << "Variable steps (1-3):" << endl;
        for (int n : {10, 20, 30})
        {
            Timer timer("  n = " + std::to_string(n));
            int result = ClimbingStairs::withVariableSteps(n, 3);
            cout << "    Result: " << result << endl;
        }

        cout << "\nMin cost climbing stairs:" << endl;
        vector<int> cost = {10, 15, 20, 17, 1, 5, 27, 50, 30};
        {
            Timer timer("  Cost: 10,15,20,17,1,5,27,50,30");
            int result = ClimbingStairs::minCost(cost);
            cout << "    Result: " << result << endl;
        }

        cout << endl;
    }

    /**
     * @brief Benchmark LIS implementations
     */
    static void benchmarkLIS()
    {
        cout << "=== Longest Increasing Subsequence Benchmarks ===" << endl;

        vector<vector<int>> testCases = {
            {10, 9, 2, 5, 3, 7, 101, 18},
            {0, 1, 0, 3, 2, 3},
            {7, 7, 7, 7, 7, 7, 7},
            generateRandomArray(20, 1, 100)};

        for (int i = 0; i < testCases.size(); i++)
        {
            cout << "Test case " << (i + 1) << ": ";
            for (int j = 0; j < std::min(10, static_cast<int>(testCases[i].size())); j++)
            {
                cout << testCases[i][j] << " ";
            }
            if (testCases[i].size() > 10)
                cout << "...";
            cout << endl;

            if (testCases[i].size() <= 20)
            { // Naive is too slow for larger arrays
                {
                    Timer timer("  Naive recursive");
                    int result = LongestIncreasingSubsequence::naive(testCases[i]);
                    cout << "    Result: " << result << endl;
                }
            }

            {
                Timer timer("  Memoized (top-down)");
                int result = LongestIncreasingSubsequence::memoized(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Tabulation (bottom-up)");
                int result = LongestIncreasingSubsequence::tabulation(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Binary search");
                int result = LongestIncreasingSubsequence::binarySearch(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Get LIS");
                vector<int> lis = LongestIncreasingSubsequence::getLIS(testCases[i]);
                cout << "    LIS: ";
                for (int num : lis)
                {
                    cout << num << " ";
                }
                cout << endl;
            }

            cout << endl;
        }
    }

    /**
     * @brief Benchmark Triangle implementations
     */
    static void benchmarkTriangle()
    {
        cout << "=== Triangle Benchmarks ===" << endl;

        vector<vector<vector<int>>> testCases = {
            {{2},
             {3, 4},
             {6, 5, 7},
             {4, 1, 8, 3}},
            generateRandomTriangle(10, 1, 100),
            generateRandomTriangle(20, 1, 100)};

        for (int i = 0; i < testCases.size(); i++)
        {
            cout << "Triangle " << (i + 1) << " (size " << testCases[i].size() << "):" << endl;

            {
                Timer timer("  Memoized (top-down)");
                int result = Triangle::memoized(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Tabulation (bottom-up)");
                int result = Triangle::tabulation(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Space-optimized");
                int result = Triangle::optimized(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            cout << endl;
        }
    }

    /**
     * @brief Benchmark Maximum Subarray implementations
     */
    static void benchmarkMaximumSubarray()
    {
        cout << "=== Maximum Subarray Benchmarks ===" << endl;

        vector<vector<int>> testCases = {
            {-2, 1, -3, 4, -1, 2, 1, -5, 4},
            {5, 4, -1, 7, 8},
            {-1, -2, -3, -4, -5},
            generateRandomArray(50, -50, 50)};

        for (int i = 0; i < testCases.size(); i++)
        {
            cout << "Test case " << (i + 1) << ": ";
            for (int j = 0; j < std::min(10, static_cast<int>(testCases[i].size())); j++)
            {
                cout << testCases[i][j] << " ";
            }
            if (testCases[i].size() > 10)
                cout << "...";
            cout << endl;

            if (testCases[i].size() <= 20)
            { // Brute force is too slow for larger arrays
                {
                    Timer timer("  Brute force");
                    int result = MaximumSubarray::bruteForce(testCases[i]);
                    cout << "    Result: " << result << endl;
                }
            }

            {
                Timer timer("  Prefix sum");
                int result = MaximumSubarray::prefixSum(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Kadane's algorithm");
                int result = MaximumSubarray::kadane(testCases[i]);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Get max subarray");
                auto [sum, indices] = MaximumSubarray::getMaxSubarray(testCases[i]);
                cout << "    Result: " << sum << " (from index " << indices.first
                     << " to " << indices.second << ")" << endl;
            }

            cout << endl;
        }
    }

    /**
     * @brief Benchmark Knapsack implementations
     */
    static void benchmarkKnapsack()
    {
        cout << "=== 0/1 Knapsack Benchmarks ===" << endl;

        vector<std::tuple<vector<int>, vector<int>, int>> testCases = {
            // Small test case
            {{1, 2, 3, 5}, {1, 6, 10, 16}, 7},
            // Medium test case
            {{10, 20, 30, 40, 50}, {60, 100, 120, 140, 150}, 100},
            // Generated test case
            generateRandomKnapsack(10, 50, 100)};

        for (int i = 0; i < testCases.size(); i++)
        {
            auto &[weights, values, capacity] = testCases[i];

            cout << "Test case " << (i + 1) << ":" << endl;
            cout << "  Weights: ";
            for (int w : weights)
                cout << w << " ";
            cout << endl;

            cout << "  Values: ";
            for (int v : values)
                cout << v << " ";
            cout << endl;

            cout << "  Capacity: " << capacity << endl;

            if (weights.size() <= 20)
            { // Naive is too slow for larger inputs
                {
                    Timer timer("  Naive recursive");
                    int result = Knapsack::naive(weights, values, capacity);
                    cout << "    Result: " << result << endl;
                }
            }

            {
                Timer timer("  Memoized (top-down)");
                int result = Knapsack::memoized(weights, values, capacity);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Tabulation (bottom-up)");
                int result = Knapsack::tabulation(weights, values, capacity);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Space-optimized");
                int result = Knapsack::optimized(weights, values, capacity);
                cout << "    Result: " << result << endl;
            }

            {
                Timer timer("  Get selected items");
                vector<int> selected = Knapsack::getSelectedItems(weights, values, capacity);
                cout << "    Selected items: ";
                for (int idx : selected)
                {
                    cout << idx << " ";
                }
                cout << endl;

                // Verify solution
                int totalWeight = 0;
                int totalValue = 0;
                for (int idx : selected)
                {
                    totalWeight += weights[idx];
                    totalValue += values[idx];
                }
                cout << "    Total weight: " << totalWeight << " (capacity: " << capacity << ")" << endl;
                cout << "    Total value: " << totalValue << endl;
            }

            cout << endl;
        }
    }

    /**
     * @brief Generate and save visualization data for DP examples
     */
    static void generateVisualizationData()
    {
        // Fibonacci visualization data
        std::ofstream fibFile("fibonacci_visualization.csv");
        fibFile << "n,Naive,Memoized,Tabulation,Optimized,MatrixExponentiation\n";

        for (int n = 1; n <= 20; n++)
        {
            fibFile << n << ",";

            // Naive (skip for large n to avoid timeout)
            if (n <= 15)
            {
                Timer timer;
                long long result = Fibonacci::naive(n);
                double time = timer.stop();
                fibFile << time;
            }
            else
            {
                fibFile << "N/A";
            }
            fibFile << ",";

            // Memoized
            {
                Timer timer;
                long long result = Fibonacci::memoized(n);
                double time = timer.stop();
                fibFile << time;
            }
            fibFile << ",";

            // Tabulation
            {
                Timer timer;
                long long result = Fibonacci::tabulation(n);
                double time = timer.stop();
                fibFile << time;
            }
            fibFile << ",";

            // Optimized
            {
                Timer timer;
                long long result = Fibonacci::optimized(n);
                double time = timer.stop();
                fibFile << time;
            }
            fibFile << ",";

            // Matrix Exponentiation
            {
                Timer timer;
                long long result = Fibonacci::matrix_exponentiation(n);
                double time = timer.stop();
                fibFile << time;
            }
            fibFile << "\n";
        }

        // Coin change visualization data
        vector<int> coins = {1, 2, 5};
        std::ofstream coinFile("coin_change_visualization.csv");
        coinFile << "amount,naive,memoized,tabulation\n";

        for (int amount = 1; amount <= 20; amount++)
        {
            coinFile << amount << ",";

            // Naive
            {
                Timer timer;
                int result = CoinChange::minCoinsNaive(coins, amount);
                double time = timer.stop();
                coinFile << time;
            }
            coinFile << ",";

            // Memoized
            {
                Timer timer;
                int result = CoinChange::minCoinsMemoized(coins, amount);
                double time = timer.stop();
                coinFile << time;
            }
            coinFile << ",";

            // Tabulation
            {
                Timer timer;
                int result = CoinChange::minCoinsTabulation(coins, amount);
                double time = timer.stop();
                coinFile << time;
            }
            coinFile << "\n";
        }
    }
};

/**
 * @brief Main function with examples and benchmarks
 */
int main()
{
    std::cout << "Dynamic Programming - Part 1" << std::endl;
    std::cout << "===========================" << std::endl
              << std::endl;

    // Demonstrate Fibonacci with different approaches
    std::cout << "Fibonacci Examples:" << std::endl;
    std::cout << "-----------------" << std::endl;

    int n = 10;
    std::cout << "Computing Fibonacci(" << n << "):" << std::endl;

    // Naive recursive
    {
        Timer timer("Naive recursive");
        long long result = Fibonacci::naive(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Memoized
    {
        Timer timer("Memoized (top-down)");
        long long result = Fibonacci::memoized(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Tabulation
    {
        Timer timer("Tabulation (bottom-up)");
        long long result = Fibonacci::tabulation(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Optimized
    {
        Timer timer("Space-optimized");
        long long result = Fibonacci::optimized(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Matrix exponentiation
    {
        Timer timer("Matrix exponentiation");
        long long result = Fibonacci::matrix_exponentiation(n);
        std::cout << "  Result: " << result << std::endl;
    }

    std::cout << std::endl;

    // Demonstrate Coin Change
    std::cout << "Coin Change Examples:" << std::endl;
    std::cout << "-------------------" << std::endl;

    vector<int> coins = {1, 2, 5};
    int amount = 11;

    std::cout << "Minimum coins to make " << amount << " using [1, 2, 5]:" << std::endl;

    // Naive recursive
    {
        Timer timer("Naive recursive");
        int result = CoinChange::minCoinsNaive(coins, amount);
        std::cout << "  Result: " << result << std::endl;
    }

    // Memoized
    {
        Timer timer("Memoized (top-down)");
        int result = CoinChange::minCoinsMemoized(coins, amount);
        std::cout << "  Result: " << result << std::endl;
    }

    // Tabulation
    {
        Timer timer("Tabulation (bottom-up)");
        int result = CoinChange::minCoinsTabulation(coins, amount);
        std::cout << "  Result: " << result << std::endl;
    }

    // Get the coins used
    {
        Timer timer("Get coins used");
        vector<int> coinsUsed = CoinChange::getCoinsUsed(coins, amount);
        std::cout << "  Coins used: ";
        for (int coin : coinsUsed)
        {
            std::cout << coin << " ";
        }
        std::cout << std::endl;
    }

    // Count ways
    {
        Timer timer("Count ways");
        int result = CoinChange::countWays(coins, amount);
        std::cout << "  Number of ways: " << result << std::endl;
    }

    std::cout << std::endl;

    // Demonstrate Climbing Stairs
    std::cout << "Climbing Stairs Examples:" << std::endl;
    std::cout << "----------------------" << std::endl;

    n = 10;
    std::cout << "Distinct ways to climb " << n << " stairs:" << std::endl;

    // Naive recursive
    {
        Timer timer("Naive recursive");
        int result = ClimbingStairs::naive(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Memoized
    {
        Timer timer("Memoized (top-down)");
        int result = ClimbingStairs::memoized(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Tabulation
    {
        Timer timer("Tabulation (bottom-up)");
        int result = ClimbingStairs::tabulation(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // Optimized
    {
        Timer timer("Space-optimized");
        int result = ClimbingStairs::optimized(n);
        std::cout << "  Result: " << result << std::endl;
    }

    // With variable steps
    {
        int k = 3; // Can take 1, 2, or 3 steps at a time
        Timer timer("Variable steps (1-3)");
        int result = ClimbingStairs::withVariableSteps(n, k);
        std::cout << "  Result with 1-" << k << " steps: " << result << std::endl;
    }

    // Min cost climbing stairs
    {
        vector<int> cost = {10, 15, 20};
        Timer timer("Min cost");
        int result = ClimbingStairs::minCost(cost);
        std::cout << "  Minimum cost: " << result << std::endl;
    }

    std::cout << std::endl;

    // Demonstrate Longest Increasing Subsequence
    std::cout << "Longest Increasing Subsequence Examples:" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    std::cout << "LIS in [10, 9, 2, 5, 3, 7, 101, 18]:" << std::endl;

    // Naive
    {
        Timer timer("Naive recursive");
        int result = LongestIncreasingSubsequence::naive(nums);
        std::cout << "  Result: " << result << std::endl;
    }

    // Memoized
    {
        Timer timer("Memoized (top-down)");
        int result = LongestIncreasingSubsequence::memoized(nums);
        std::cout << "  Result: " << result << std::endl;
    }

    // Tabulation
    {
        Timer timer("Tabulation (bottom-up)");
        int result = LongestIncreasingSubsequence::tabulation(nums);
        std::cout << "  Result: " << result << std::endl;
    }

    // Binary search
    {
        Timer timer("Binary search");
        int result = LongestIncreasingSubsequence::binarySearch(nums);
        std::cout << "  Result: " << result << std::endl;
    }

    // Get the LIS
    {
        Timer timer("Get LIS");
        vector<int> lis = LongestIncreasingSubsequence::getLIS(nums);
        std::cout << "  LIS: ";
        for (int num : lis)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // Demonstrate Triangle
    std::cout << "Triangle Examples:" << std::endl;
    std::cout << "-----------------" << std::endl;

    vector<vector<int>> triangle = {
        {2},
        {3, 4},
        {6, 5, 7},
        {4, 1, 8, 3}};

    std::cout << "Minimum path sum in triangle:" << std::endl;
    std::cout << "  2" << std::endl;
    std::cout << " 3 4" << std::endl;
    std::cout << "6 5 7" << std::endl;
    std::cout << "4 1 8 3" << std::endl;

    // Memoized
    {
        Timer timer("Memoized (top-down)");
        int result = Triangle::memoized(triangle);
        std::cout << "  Result: " << result << std::endl;
    }

    // Tabulation
    {
        Timer timer("Tabulation (bottom-up)");
        int result = Triangle::tabulation(triangle);
        std::cout << "  Result: " << result << std::endl;
    }

    // Optimized
    {
        Timer timer("Space-optimized");
        int result = Triangle::optimized(triangle);
        std::cout << "  Result: " << result << std::endl;
    }

    std::cout << std::endl;

    // Demonstrate Maximum Subarray
    std::cout << "Maximum Subarray Examples:" << std::endl;
    std::cout << "-------------------------" << std::endl;

    vector<int> array = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    std::cout << "Maximum subarray in [-2, 1, -3, 4, -1, 2, 1, -5, 4]:" << std::endl;

    // Brute force
    {
        Timer timer("Brute force");
        int result = MaximumSubarray::bruteForce(array);
        std::cout << "  Result: " << result << std::endl;
    }

    // Prefix sum
    {
        Timer timer("Prefix sum");
        int result = MaximumSubarray::prefixSum(array);
        std::cout << "  Result: " << result << std::endl;
    }

    // Kadane's algorithm
    {
        Timer timer("Kadane's algorithm");
        int result = MaximumSubarray::kadane(array);
        std::cout << "  Result: " << result << std::endl;
    }

    // Get the subarray
    {
        Timer timer("Get max subarray");
        auto [sum, indices] = MaximumSubarray::getMaxSubarray(array);
        std::cout << "  Result: " << sum << " (from index " << indices.first
                  << " to " << indices.second << ")" << std::endl;

        std::cout << "  Subarray: ";
        for (int i = indices.first; i <= indices.second; i++)
        {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // Demonstrate 0/1 Knapsack
    std::cout << "0/1 Knapsack Examples:" << std::endl;
    std::cout << "---------------------" << std::endl;

    vector<int> weights = {1, 2, 3, 5};
    vector<int> values = {1, 6, 10, 16};
    int capacity = 7;

    std::cout << "Knapsack with capacity " << capacity << ":" << std::endl;
    std::cout << "  Weights: ";
    for (int w : weights)
        std::cout << w << " ";
    std::cout << std::endl;

    std::cout << "  Values: ";
    for (int v : values)
        std::cout << v << " ";
    std::cout << std::endl;

    // Naive
    {
        Timer timer("Naive recursive");
        int result = Knapsack::naive(weights, values, capacity);
        std::cout << "  Result: " << result << std::endl;
    }

    // Memoized
    {
        Timer timer("Memoized (top-down)");
        int result = Knapsack::memoized(weights, values, capacity);
        std::cout << "  Result: " << result << std::endl;
    }

    // Tabulation
    {
        Timer timer("Tabulation (bottom-up)");
        int result = Knapsack::tabulation(weights, values, capacity);
        std::cout << "  Result: " << result << std::endl;
    }

    // Optimized
    {
        Timer timer("Space-optimized");
        int result = Knapsack::optimized(weights, values, capacity);
        std::cout << "  Result: " << result << std::endl;
    }

    // Get selected items
    {
        Timer timer("Get selected items");
        vector<int> selected = Knapsack::getSelectedItems(weights, values, capacity);
        std::cout << "  Selected items: ";
        for (int idx : selected)
        {
            std::cout << idx << " ";
        }
        std::cout << std::endl;

        // Verify solution
        int totalWeight = 0;
        int totalValue = 0;
        for (int idx : selected)
        {
            totalWeight += weights[idx];
            totalValue += values[idx];
        }
        std::cout << "  Total weight: " << totalWeight << " (capacity: " << capacity << ")" << std::endl;
        std::cout << "  Total value: " << totalValue << std::endl;
    }

    std::cout << std::endl;

    // Run comprehensive benchmarks if requested
    char runBenchmarks;
    std::cout << "Run comprehensive benchmarks? (y/n): ";
    std::cin >> runBenchmarks;

    if (tolower(runBenchmarks) == 'y')
    {
        DynamicProgrammingExamples::benchmarkFibonacci();
        DynamicProgrammingExamples::benchmarkCoinChange();
        DynamicProgrammingExamples::benchmarkClimbingStairs();
        DynamicProgrammingExamples::benchmarkLIS();
        DynamicProgrammingExamples::benchmarkTriangle();
        DynamicProgrammingExamples::benchmarkMaximumSubarray();
        DynamicProgrammingExamples::benchmarkKnapsack();
        DynamicProgrammingExamples::generateVisualizationData();
    }

    return 0;
}