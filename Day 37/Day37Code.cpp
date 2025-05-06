/**
 * @file dynamic_programming_part2.cpp
 * @brief Implementation of advanced Dynamic Programming algorithms and problems
 * @author DSAin45 Series
 * @date May 6, 2025
 *
 * This file contains comprehensive implementations of advanced DP problems:
 * - Knapsack problem and its variations
 * - Longest Common Subsequence (LCS) and related string problems
 * - Edit Distance and string transformation algorithms
 * - Matrix Chain Multiplication
 * - Optimal Binary Search Tree
 *
 * Each implementation includes different approaches (naive, memoized, tabulation)
 * along with space optimizations and solution reconstruction techniques.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <functional>
#include <random>

// Define constants
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
 * @brief Class containing Knapsack problem implementations
 */
class Knapsack
{
public:
    /**
     * @brief Naive recursive implementation of 0/1 Knapsack
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @param n Number of items
     * @return Maximum value that can be obtained
     * @time O(2^n) - exponential time complexity
     * @space O(n) - recursion stack depth
     */
    static int naiveRecursive(const std::vector<int> &weights, const std::vector<int> &values,
                              int capacity, int n)
    {
        // Base case: no items or no capacity
        if (n == 0 || capacity == 0)
        {
            return 0;
        }

        // If item is too heavy, skip it
        if (weights[n - 1] > capacity)
        {
            return naiveRecursive(weights, values, capacity, n - 1);
        }

        // Return the maximum of including or excluding the item
        return std::max(
            values[n - 1] + naiveRecursive(weights, values, capacity - weights[n - 1], n - 1),
            naiveRecursive(weights, values, capacity, n - 1));
    }

    /**
     * @brief Wrapper for naive recursive implementation
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     */
    static int naive(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        return naiveRecursive(weights, values, capacity, weights.size());
    }

    /**
     * @brief Memoized recursive implementation of 0/1 Knapsack
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @param n Number of items
     * @param memo Memoization table
     * @return Maximum value that can be obtained
     * @time O(n * capacity) - polynomial time complexity
     * @space O(n * capacity) - memoization table + recursion stack
     */
    static int memoizedRecursive(const std::vector<int> &weights, const std::vector<int> &values,
                                 int capacity, int n, std::vector<std::vector<int>> &memo)
    {
        // Base case: no items or no capacity
        if (n == 0 || capacity == 0)
        {
            return 0;
        }

        // If result is already computed, return it
        if (memo[n][capacity] != -1)
        {
            return memo[n][capacity];
        }

        // If item is too heavy, skip it
        if (weights[n - 1] > capacity)
        {
            memo[n][capacity] = memoizedRecursive(weights, values, capacity, n - 1, memo);
        }
        else
        {
            // Return the maximum of including or excluding the item
            memo[n][capacity] = std::max(
                values[n - 1] + memoizedRecursive(weights, values, capacity - weights[n - 1], n - 1, memo),
                memoizedRecursive(weights, values, capacity, n - 1, memo));
        }

        return memo[n][capacity];
    }

    /**
     * @brief Wrapper for memoized recursive implementation
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     */
    static int memoized(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1));
        return memoizedRecursive(weights, values, capacity, n, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation of 0/1 Knapsack
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     * @time O(n * capacity) - polynomial time complexity
     * @space O(n * capacity) - tabulation table
     */
    static int tabulation(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

        // Fill the dp table in bottom-up manner
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
     * @brief Space-optimized tabulation implementation of 0/1 Knapsack
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     * @time O(n * capacity) - polynomial time complexity
     * @space O(capacity) - optimized to use only 1D array
     */
    static int spaceOptimized(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<int> dp(capacity + 1, 0);

        // Process each item
        for (int i = 0; i < n; i++)
        {
            // Process each capacity in reverse to avoid using items multiple times
            for (int w = capacity; w >= weights[i]; w--)
            {
                dp[w] = std::max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }

        return dp[capacity];
    }

    /**
     * @brief Get the items selected in the optimal 0/1 Knapsack solution
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Vector of indices of the selected items
     * @time O(n * capacity) - polynomial time complexity
     * @space O(n * capacity) - tabulation table
     */
    static std::vector<int> getSelectedItems(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

        // Fill the dp table
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
        std::vector<int> selectedItems;
        int w = capacity;

        for (int i = n; i > 0 && w > 0; i--)
        {
            // Check if this item is included (value changed from previous row)
            if (dp[i][w] != dp[i - 1][w])
            {
                selectedItems.push_back(i - 1);
                w -= weights[i - 1];
            }
        }

        // Reverse to get items in original order
        std::reverse(selectedItems.begin(), selectedItems.end());
        return selectedItems;
    }

    /**
     * @brief Print the DP table for educational purposes
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     */
    static void printDPTable(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

        // Fill the dp table
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

        // Print the table
        std::cout << "Dynamic Programming Table for 0/1 Knapsack:\n";
        std::cout << "Weights: ";
        for (int w : weights)
            std::cout << w << " ";
        std::cout << "\nValues: ";
        for (int v : values)
            std::cout << v << " ";
        std::cout << "\nCapacity: " << capacity << "\n\n";

        // Print column headers
        std::cout << "      ";
        for (int w = 0; w <= capacity; w++)
        {
            std::cout << std::setw(4) << w;
        }
        std::cout << "\n------";
        for (int w = 0; w <= capacity; w++)
        {
            std::cout << "----";
        }
        std::cout << "\n";

        // Print table rows
        for (int i = 0; i <= n; i++)
        {
            if (i == 0)
            {
                std::cout << "     |";
            }
            else
            {
                std::cout << "W:" << std::setw(2) << weights[i - 1] << " |";
            }

            for (int w = 0; w <= capacity; w++)
            {
                std::cout << std::setw(4) << dp[i][w];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    /**
     * @brief Unbounded Knapsack (items can be taken multiple times)
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     * @time O(n * capacity) - polynomial time complexity
     * @space O(capacity) - only need 1D array
     */
    static int unboundedKnapsack(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<int> dp(capacity + 1, 0);

        // Fill the dp table
        for (int w = 1; w <= capacity; w++)
        {
            for (int i = 0; i < n; i++)
            {
                if (weights[i] <= w)
                {
                    dp[w] = std::max(dp[w], dp[w - weights[i]] + values[i]);
                }
            }
        }

        return dp[capacity];
    }

    /**
     * @brief Get items selected in unbounded knapsack
     * @param weights Array of item weights
     * @param values Array of item values
     * @param capacity Knapsack capacity
     * @return Vector with the count of each item taken
     */
    static std::vector<int> getUnboundedKnapsackItems(const std::vector<int> &weights, const std::vector<int> &values, int capacity)
    {
        int n = weights.size();
        std::vector<int> dp(capacity + 1, 0);
        std::vector<int> choice(capacity + 1, -1);

        // Fill the dp table and track choices
        for (int w = 1; w <= capacity; w++)
        {
            for (int i = 0; i < n; i++)
            {
                if (weights[i] <= w && dp[w - weights[i]] + values[i] > dp[w])
                {
                    dp[w] = dp[w - weights[i]] + values[i];
                    choice[w] = i;
                }
            }
        }

        // Backtrack to find item counts
        std::vector<int> itemCounts(n, 0);
        int remainingCapacity = capacity;

        while (remainingCapacity > 0 && choice[remainingCapacity] != -1)
        {
            int item = choice[remainingCapacity];
            itemCounts[item]++;
            remainingCapacity -= weights[item];
        }

        return itemCounts;
    }
};

/**
 * @brief Class containing Knapsack problem variations
 */
class KnapsackVariations
{
public:
    /**
     * @brief Subset Sum Problem: Is there a subset with given sum?
     * @param nums Array of integers
     * @param target Target sum
     * @return True if there exists a subset with sum equal to target
     * @time O(n * target) - polynomial time complexity
     * @space O(target) - optimized to use only 1D array
     */
    static bool subsetSum(const std::vector<int> &nums, int target)
    {
        int n = nums.size();
        std::vector<bool> dp(target + 1, false);
        dp[0] = true; // Empty subset can make sum 0

        for (int num : nums)
        {
            for (int j = target; j >= num; j--)
            {
                dp[j] = dp[j] || dp[j - num];
            }
        }

        return dp[target];
    }

    /**
     * @brief Partition Equal Subset Sum: Can array be divided into two equal sum subsets?
     * @param nums Array of integers
     * @return True if array can be partitioned into two equal sum subsets
     * @time O(n * sum) - polynomial time complexity
     * @space O(sum) - optimized to use only 1D array
     */
    static bool canPartition(std::vector<int> &nums)
    {
        int sum = 0;
        for (int num : nums)
        {
            sum += num;
        }

        // If sum is odd, cannot have two equal partitions
        if (sum % 2 != 0)
        {
            return false;
        }

        return subsetSum(nums, sum / 2);
    }

    /**
     * @brief Minimum Subset Sum Difference: Find min diff between two subset sums
     * @param nums Array of integers
     * @return Minimum difference between the sum of two subsets
     */
    static int minimumSubsetSumDifference(const std::vector<int> &nums)
    {
        int total_sum = 0;
        for (int num : nums)
        {
            total_sum += num;
        }

        int n = nums.size();
        bool dp[n + 1][total_sum + 1];

        // Initialize base cases
        for (int i = 0; i <= n; i++)
        {
            dp[i][0] = true;
        }
        for (int j = 1; j <= total_sum; j++)
        {
            dp[0][j] = false;
        }

        // Fill dp table
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= total_sum; j++)
            {
                dp[i][j] = dp[i - 1][j];
                if (nums[i - 1] <= j)
                {
                    dp[i][j] = dp[i][j] || dp[i - 1][j - nums[i - 1]];
                }
            }
        }

        // Find the largest sum <= total_sum/2 that is achievable
        int half_sum = total_sum / 2;
        while (half_sum >= 0 && !dp[n][half_sum])
        {
            half_sum--;
        }

        // Return the minimum difference
        return total_sum - 2 * half_sum;
    }

    /**
     * @brief Count number of subsets with given sum
     * @param nums Array of integers
     * @param target Target sum
     * @return Number of subsets with sum equal to target
     */
    static int countSubsetsWithSum(const std::vector<int> &nums, int target)
    {
        int n = nums.size();
        std::vector<int> dp(target + 1, 0);
        dp[0] = 1; // Empty subset can make sum 0

        for (int num : nums)
        {
            for (int j = target; j >= num; j--)
            {
                dp[j] += dp[j - num];
            }
        }

        return dp[target];
    }

    /**
     * @brief Target Sum: Assign + or - to each number to reach target
     * @param nums Array of integers
     * @param target Target sum
     * @return Number of ways to assign + or - to reach target
     */
    static int findTargetSumWays(std::vector<int> &nums, int target)
    {
        int total = 0;
        for (int num : nums)
        {
            total += num;
        }

        // If target is beyond the range of possible sums or
        // (total + target) is odd, return 0
        if (std::abs(target) > total || (total + target) % 2 != 0)
        {
            return 0;
        }

        // Let P be sum of elements with + sign, N be sum with - sign
        // P + N = total
        // P - N = target
        // 2P = total + target
        // P = (total + target) / 2

        int subsetSum = (total + target) / 2;
        return countSubsetsWithSum(nums, subsetSum);
    }

    /**
     * @brief Coin Change: Minimum number of coins to make amount
     * @param coins Array of coin denominations
     * @param amount Target amount
     * @return Minimum number of coins needed, or -1 if impossible
     */
    static int coinChange(std::vector<int> &coins, int amount)
    {
        std::vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;

        for (int coin : coins)
        {
            for (int i = coin; i <= amount; i++)
            {
                dp[i] = std::min(dp[i], dp[i - coin] + 1);
            }
        }

        return dp[amount] > amount ? -1 : dp[amount];
    }

    /**
     * @brief Coin Change 2: Number of ways to make amount
     * @param coins Array of coin denominations
     * @param amount Target amount
     * @return Number of ways to make amount
     */
    static int coinChange2(std::vector<int> &coins, int amount)
    {
        std::vector<int> dp(amount + 1, 0);
        dp[0] = 1;

        for (int coin : coins)
        {
            for (int i = coin; i <= amount; i++)
            {
                dp[i] += dp[i - coin];
            }
        }

        return dp[amount];
    }

    /**
     * @brief Rod Cutting: Maximize profit by cutting a rod
     * @param prices Array of prices for different rod lengths
     * @param n Length of the rod
     * @return Maximum profit achievable
     */
    static int rodCutting(std::vector<int> &prices, int n)
    {
        std::vector<int> dp(n + 1, 0);

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= i; j++)
            {
                dp[i] = std::max(dp[i], prices[j - 1] + dp[i - j]);
            }
        }

        return dp[n];
    }

    /**
     * @brief Get the optimal rod cutting solution
     * @param prices Array of prices for different rod lengths
     * @param n Length of the rod
     * @return Vector of cut lengths in the optimal solution
     */
    static std::vector<int> getRodCuttingSolution(std::vector<int> &prices, int n)
    {
        std::vector<int> dp(n + 1, 0);
        std::vector<int> cut(n + 1, 0);

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= i; j++)
            {
                if (prices[j - 1] + dp[i - j] > dp[i])
                {
                    dp[i] = prices[j - 1] + dp[i - j];
                    cut[i] = j;
                }
            }
        }

        // Reconstruct the solution
        std::vector<int> result;
        int remaining = n;

        while (remaining > 0)
        {
            result.push_back(cut[remaining]);
            remaining -= cut[remaining];
        }

        return result;
    }
};

/**
 * @brief Class containing Longest Common Subsequence (LCS) implementations
 */
class LongestCommonSubsequence
{
public:
    /**
     * @brief Naive recursive implementation of LCS
     * @param s1 First string
     * @param s2 Second string
     * @param m Length of first string
     * @param n Length of second string
     * @return Length of LCS
     * @time O(2^(m+n)) - exponential time complexity
     * @space O(m+n) - recursion stack depth
     */
    static int naiveRecursive(const std::string &s1, const std::string &s2, int m, int n)
    {
        // Base case: if either string is empty
        if (m == 0 || n == 0)
        {
            return 0;
        }

        // If last characters match, include it in LCS
        if (s1[m - 1] == s2[n - 1])
        {
            return 1 + naiveRecursive(s1, s2, m - 1, n - 1);
        }

        // Otherwise, try both possibilities and take max
        return std::max(
            naiveRecursive(s1, s2, m - 1, n),
            naiveRecursive(s1, s2, m, n - 1));
    }

    /**
     * @brief Wrapper for naive recursive implementation
     * @param s1 First string
     * @param s2 Second string
     * @return Length of LCS
     */
    static int naive(const std::string &s1, const std::string &s2)
    {
        return naiveRecursive(s1, s2, s1.length(), s2.length());
    }

    /**
     * @brief Memoized recursive implementation of LCS
     * @param s1 First string
     * @param s2 Second string
     * @param m Length of first string
     * @param n Length of second string
     * @param memo Memoization table
     * @return Length of LCS
     * @time O(m*n) - polynomial time complexity
     * @space O(m*n) - memoization table + recursion stack
     */
    static int memoizedRecursive(const std::string &s1, const std::string &s2, int m, int n,
                                 std::vector<std::vector<int>> &memo)
    {
        // Base case: if either string is empty
        if (m == 0 || n == 0)
        {
            return 0;
        }

        // If result is already computed, return it
        if (memo[m][n] != -1)
        {
            return memo[m][n];
        }

        // If last characters match, include it in LCS
        if (s1[m - 1] == s2[n - 1])
        {
            memo[m][n] = 1 + memoizedRecursive(s1, s2, m - 1, n - 1, memo);
        }
        else
        {
            // Otherwise, try both possibilities and take max
            memo[m][n] = std::max(
                memoizedRecursive(s1, s2, m - 1, n, memo),
                memoizedRecursive(s1, s2, m, n - 1, memo));
        }

        return memo[m][n];
    }

    /**
     * @brief Wrapper for memoized recursive implementation
     * @param s1 First string
     * @param s2 Second string
     * @return Length of LCS
     */
    static int memoized(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> memo(m + 1, std::vector<int>(n + 1, -1));
        return memoizedRecursive(s1, s2, m, n, memo);
    }

    /**
     * @brief Tabulation (bottom-up) implementation of LCS
     * @param s1 First string
     * @param s2 Second string
     * @return Length of LCS
     * @time O(m*n) - polynomial time complexity
     * @space O(m*n) - tabulation table
     */
    static int tabulation(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        return dp[m][n];
    }

    /**
     * @brief Space-optimized implementation of LCS
     * @param s1 First string
     * @param s2 Second string
     * @return Length of LCS
     * @time O(m*n) - polynomial time complexity
     * @space O(min(m,n)) - optimized to use only 2 rows
     */
    static int spaceOptimized(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();

        // Make sure s1 is the shorter string to optimize space
        if (m > n)
        {
            return spaceOptimized(s2, s1);
        }

        std::vector<int> prev(n + 1, 0);
        std::vector<int> curr(n + 1, 0);

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    curr[j] = prev[j - 1] + 1;
                }
                else
                {
                    curr[j] = std::max(prev[j], curr[j - 1]);
                }
            }
            // Swap for next iteration
            prev = curr;
        }

        return prev[n];
    }

    /**
     * @brief Print the actual Longest Common Subsequence
     * @param s1 First string
     * @param s2 Second string
     * @return The LCS string
     */
    static std::string printLCS(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Fill the dp table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Backtrack to find LCS
        std::string lcs;
        int i = m, j = n;

        while (i > 0 && j > 0)
        {
            // If current characters match
            if (s1[i - 1] == s2[j - 1])
            {
                lcs = s1[i - 1] + lcs;
                i--;
                j--;
            }
            // Otherwise, move in the direction of larger value
            else if (dp[i - 1][j] > dp[i][j - 1])
            {
                i--;
            }
            else
            {
                j--;
            }
        }

        return lcs;
    }

    /**
     * @brief Print the DP table for educational purposes
     * @param s1 First string
     * @param s2 Second string
     */
    static void printDPTable(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Fill the dp table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Print the table
        std::cout << "Dynamic Programming Table for LCS:\n";
        std::cout << "s1: " << s1 << "\n";
        std::cout << "s2: " << s2 << "\n\n";

        // Print column headers
        std::cout << "    |   |";
        for (int j = 0; j < n; j++)
        {
            std::cout << std::setw(3) << s2[j];
        }
        std::cout << "\n----+---+";
        for (int j = 0; j < n; j++)
        {
            std::cout << "---";
        }
        std::cout << "\n";

        // Print table rows
        for (int i = 0; i <= m; i++)
        {
            if (i == 0)
            {
                std::cout << "    |";
            }
            else
            {
                std::cout << " " << s1[i - 1] << "  |";
            }

            for (int j = 0; j <= n; j++)
            {
                std::cout << std::setw(3) << dp[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    /**
     * @brief Find length of Longest Common Substring
     * @param s1 First string
     * @param s2 Second string
     * @return Length of longest common substring
     */
    static int longestCommonSubstring(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        int result = 0;

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    result = std::max(result, dp[i][j]);
                }
                // No else case needed - default is 0
            }
        }

        return result;
    }

    /**
     * @brief Find the Longest Common Substring
     * @param s1 First string
     * @param s2 Second string
     * @return The longest common substring
     */
    static std::string printLongestCommonSubstring(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        int maxLength = 0;
        int endIndex = 0;

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;

                    if (dp[i][j] > maxLength)
                    {
                        maxLength = dp[i][j];
                        endIndex = i - 1;
                    }
                }
            }
        }

        if (maxLength == 0)
        {
            return "";
        }

        return s1.substr(endIndex - maxLength + 1, maxLength);
    }

    /**
     * @brief Find length of Shortest Common Supersequence
     * @param s1 First string
     * @param s2 Second string
     * @return Length of shortest common supersequence
     */
    static int shortestCommonSupersequence(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();

        // Length of SCS = m + n - length of LCS
        return m + n - tabulation(s1, s2);
    }

    /**
     * @brief Find the Shortest Common Supersequence
     * @param s1 First string
     * @param s2 Second string
     * @return The shortest common supersequence
     */
    static std::string printShortestCommonSupersequence(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Fill the LCS dp table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Build the SCS
        std::string result;
        int i = m, j = n;

        while (i > 0 && j > 0)
        {
            // If current characters match, add once to result
            if (s1[i - 1] == s2[j - 1])
            {
                result = s1[i - 1] + result;
                i--;
                j--;
            }
            // Otherwise, add character from string with larger dp value
            else if (dp[i - 1][j] > dp[i][j - 1])
            {
                result = s1[i - 1] + result;
                i--;
            }
            else
            {
                result = s2[j - 1] + result;
                j--;
            }
        }

        // Add remaining characters
        while (i > 0)
        {
            result = s1[i - 1] + result;
            i--;
        }
        while (j > 0)
        {
            result = s2[j - 1] + result;
            j--;
        }

        return result;
    }

    /**
     * @brief Find the length of Longest Palindromic Subsequence
     * @param s Input string
     * @return Length of LPS
     */
    static int longestPalindromicSubsequence(const std::string &s)
    {
        // LPS(s) = LCS(s, reverse(s))
        std::string reversedS = s;
        std::reverse(reversedS.begin(), reversedS.end());
        return tabulation(s, reversedS);
    }

    /**
     * @brief Find the Longest Palindromic Subsequence
     * @param s Input string
     * @return The LPS string
     */
    static std::string printLongestPalindromicSubsequence(const std::string &s)
    {
        std::string reversedS = s;
        std::reverse(reversedS.begin(), reversedS.end());
        return printLCS(s, reversedS);
    }

    /**
     * @brief Check if string s is a subsequence of string t
     * @param s First string
     * @param t Second string
     * @return True if s is a subsequence of t
     */
    static bool isSubsequence(const std::string &s, const std::string &t)
    {
        int m = s.length();
        int n = t.length();

        if (m > n)
            return false;

        int i = 0, j = 0;
        while (i < m && j < n)
        {
            if (s[i] == t[j])
            {
                i++;
            }
            j++;
        }

        return i == m;
    }

    /**
     * @brief Count distinct subsequences
     * @param s Input string
     * @return Number of distinct subsequences
     */
    static int countDistinctSubsequences(const std::string &s)
    {
        int n = s.length();
        std::vector<int> dp(n + 1);
        dp[0] = 1; // Empty string is a valid subsequence

        std::unordered_map<char, int> lastOccurrence;

        for (int i = 1; i <= n; i++)
        {
            dp[i] = 2 * dp[i - 1]; // Double the count (take or don't take)

            // If character has appeared before, subtract duplicates
            if (lastOccurrence.find(s[i - 1]) != lastOccurrence.end())
            {
                dp[i] -= dp[lastOccurrence[s[i - 1]] - 1];
            }

            lastOccurrence[s[i - 1]] = i;
        }

        return dp[n];
    }
};

/**
 * @brief Class containing Edit Distance implementations
 */
class EditDistance
{
public:
    /**
     * @brief Calculate minimum edit distance between two strings
     * @param word1 First string
     * @param word2 Second string
     * @return Minimum edit distance
     * @time O(m*n) - polynomial time complexity
     * @space O(m*n) - tabulation table
     */
    static int minDistance(const std::string &word1, const std::string &word2)
    {
        int m = word1.length();
        int n = word2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Base cases: empty string to non-empty string
        for (int i = 0; i <= m; i++)
        {
            dp[i][0] = i; // Delete i characters
        }
        for (int j = 0; j <= n; j++)
        {
            dp[0][j] = j; // Insert j characters
        }

        // Fill the table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (word1[i - 1] == word2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1]; // No operation needed
                }
                else
                {
                    dp[i][j] = 1 + std::min({
                                       dp[i - 1][j],    // Delete
                                       dp[i][j - 1],    // Insert
                                       dp[i - 1][j - 1] // Replace
                                   });
                }
            }
        }

        return dp[m][n];
    }

    /**
     * @brief Space-optimized edit distance
     * @param word1 First string
     * @param word2 Second string
     * @return Minimum edit distance
     * @time O(m*n) - polynomial time complexity
     * @space O(min(m,n)) - optimized to use only 2 rows
     */
    static int minDistanceOptimized(const std::string &word1, const std::string &word2)
    {
        int m = word1.length();
        int n = word2.length();

        // Ensure word1 is shorter to optimize space
        if (m > n)
        {
            return minDistanceOptimized(word2, word1);
        }

        std::vector<int> prev(m + 1);
        std::vector<int> curr(m + 1);

        // Base case
        for (int i = 0; i <= m; i++)
        {
            prev[i] = i;
        }

        // Fill the table
        for (int j = 1; j <= n; j++)
        {
            curr[0] = j; // Base case

            for (int i = 1; i <= m; i++)
            {
                if (word1[i - 1] == word2[j - 1])
                {
                    curr[i] = prev[i - 1];
                }
                else
                {
                    curr[i] = 1 + std::min({
                                      prev[i],     // Delete
                                      curr[i - 1], // Insert
                                      prev[i - 1]  // Replace
                                  });
                }
            }

            prev = curr;
        }

        return prev[m];
    }

    /**
     * @brief Get the edit operations to transform one string to another
     * @param word1 First string
     * @param word2 Second string
     * @return Vector of operations (Insert, Delete, Replace, Keep)
     */
    static std::vector<std::string> getEditOperations(const std::string &word1, const std::string &word2)
    {
        int m = word1.length();
        int n = word2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Base cases
        for (int i = 0; i <= m; i++)
        {
            dp[i][0] = i;
        }
        for (int j = 0; j <= n; j++)
        {
            dp[0][j] = j;
        }

        // Fill the table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (word1[i - 1] == word2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = 1 + std::min({
                                       dp[i - 1][j],    // Delete
                                       dp[i][j - 1],    // Insert
                                       dp[i - 1][j - 1] // Replace
                                   });
                }
            }
        }

        // Backtrack to find operations
        std::vector<std::string> operations;
        int i = m, j = n;

        while (i > 0 || j > 0)
        {
            if (i > 0 && j > 0 && word1[i - 1] == word2[j - 1])
            {
                operations.push_back("Keep " + std::string(1, word1[i - 1]));
                i--;
                j--;
            }
            else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + 1)
            {
                operations.push_back("Replace " + std::string(1, word1[i - 1]) + " with " + std::string(1, word2[j - 1]));
                i--;
                j--;
            }
            else if (i > 0 && dp[i][j] == dp[i - 1][j] + 1)
            {
                operations.push_back("Delete " + std::string(1, word1[i - 1]));
                i--;
            }
            else
            {
                operations.push_back("Insert " + std::string(1, word2[j - 1]));
                j--;
            }
        }

        // Reverse to get operations in correct order
        std::reverse(operations.begin(), operations.end());
        return operations;
    }

    /**
     * @brief Print the DP table for educational purposes
     * @param word1 First string
     * @param word2 Second string
     */
    static void printDPTable(const std::string &word1, const std::string &word2)
    {
        int m = word1.length();
        int n = word2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Base cases
        for (int i = 0; i <= m; i++)
        {
            dp[i][0] = i;
        }
        for (int j = 0; j <= n; j++)
        {
            dp[0][j] = j;
        }

        // Fill the table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (word1[i - 1] == word2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = 1 + std::min({
                                       dp[i - 1][j],    // Delete
                                       dp[i][j - 1],    // Insert
                                       dp[i - 1][j - 1] // Replace
                                   });
                }
            }
        }

        // Print the table
        std::cout << "Dynamic Programming Table for Edit Distance:\n";
        std::cout << "word1: " << word1 << "\n";
        std::cout << "word2: " << word2 << "\n\n";

        // Print column headers
        std::cout << "    |   |";
        for (int j = 0; j < n; j++)
        {
            std::cout << std::setw(3) << word2[j];
        }
        std::cout << "\n----+---+";
        for (int j = 0; j < n; j++)
        {
            std::cout << "---";
        }
        std::cout << "\n";

        // Print table rows
        for (int i = 0; i <= m; i++)
        {
            if (i == 0)
            {
                std::cout << "    |";
            }
            else
            {
                std::cout << " " << word1[i - 1] << "  |";
            }

            for (int j = 0; j <= n; j++)
            {
                std::cout << std::setw(3) << dp[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    /**
     * @brief Check if two strings are one edit distance apart
     * @param s First string
     * @param t Second string
     * @return True if edit distance is exactly 1
     */
    static bool isOneEditDistance(const std::string &s, const std::string &t)
    {
        return minDistance(s, t) == 1;
    }

    /**
     * @brief Calculate Levenshtein distance (same as Edit Distance)
     */
    static int levenshteinDistance(const std::string &word1, const std::string &word2)
    {
        return minDistance(word1, word2);
    }

    /**
     * @brief Calculate Damerau-Levenshtein distance (allows transposition)
     */
    static int damerauLevenshteinDistance(const std::string &word1, const std::string &word2)
    {
        int m = word1.length();
        int n = word2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Base cases
        for (int i = 0; i <= m; i++)
        {
            dp[i][0] = i;
        }
        for (int j = 0; j <= n; j++)
        {
            dp[0][j] = j;
        }

        // Fill the table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (word1[i - 1] == word2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = 1 + std::min({
                                       dp[i - 1][j],    // Delete
                                       dp[i][j - 1],    // Insert
                                       dp[i - 1][j - 1] // Replace
                                   });

                    // Check for transposition (swap adjacent characters)
                    if (i > 1 && j > 1 && word1[i - 1] == word2[j - 2] && word1[i - 2] == word2[j - 1])
                    {
                        dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + 1);
                    }
                }
            }
        }

        return dp[m][n];
    }
};

/**
 * @brief Class containing Matrix Chain Multiplication implementations
 */
class MatrixChainMultiplication
{
public:
    /**
     * @brief Calculate minimum number of scalar multiplications
     * @param dims Array of matrix dimensions
     * @return Minimum number of multiplications
     * @time O(n^3) - cubic time complexity
     * @space O(n^2) - quadratic space for dp table
     */
    static int matrixChainOrder(const std::vector<int> &dims)
    {
        int n = dims.size() - 1; // Number of matrices
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // l is chain length
        for (int l = 2; l <= n; l++)
        {
            for (int i = 0; i < n - l + 1; i++)
            {
                int j = i + l - 1;
                dp[i][j] = INF;

                // Try different partitioning positions
                for (int k = i; k < j; k++)
                {
                    int cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                    dp[i][j] = std::min(dp[i][j], cost);
                }
            }
        }

        return dp[0][n - 1];
    }

    /**
     * @brief Recursive with memoization approach for matrix chain
     * @param dims Array of matrix dimensions
     * @param i Start index
     * @param j End index
     * @param memo Memoization table
     * @return Minimum number of multiplications
     */
    static int matrixChainMemoized(const std::vector<int> &dims, int i, int j,
                                   std::vector<std::vector<int>> &memo)
    {
        // Base case: single matrix
        if (i == j)
        {
            return 0;
        }

        // If already computed, return stored result
        if (memo[i][j] != -1)
        {
            return memo[i][j];
        }

        memo[i][j] = INF;

        // Try all possible partitioning positions
        for (int k = i; k < j; k++)
        {
            int cost = matrixChainMemoized(dims, i, k, memo) +
                       matrixChainMemoized(dims, k + 1, j, memo) +
                       dims[i] * dims[k + 1] * dims[j + 1];

            memo[i][j] = std::min(memo[i][j], cost);
        }

        return memo[i][j];
    }

    /**
     * @brief Wrapper for memoized approach
     * @param dims Array of matrix dimensions
     * @return Minimum number of multiplications
     */
    static int matrixChainMemoized(const std::vector<int> &dims)
    {
        int n = dims.size() - 1;
        std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
        return matrixChainMemoized(dims, 0, n - 1, memo);
    }

    /**
     * @brief Get the optimal parenthesization
     * @param dims Array of matrix dimensions
     * @return String with optimal parenthesization
     */
    static std::string getOptimalParenthesization(const std::vector<int> &dims)
    {
        int n = dims.size() - 1;
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> bracket(n, std::vector<int>(n, 0));

        // l is chain length
        for (int l = 2; l <= n; l++)
        {
            for (int i = 0; i < n - l + 1; i++)
            {
                int j = i + l - 1;
                dp[i][j] = INF;

                for (int k = i; k < j; k++)
                {
                    int cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                    if (cost < dp[i][j])
                    {
                        dp[i][j] = cost;
                        bracket[i][j] = k;
                    }
                }
            }
        }

        // Recursively print optimal parenthesization
        std::function<std::string(int, int)> printParenthesis = [&](int i, int j) -> std::string
        {
            if (i == j)
            {
                return "M" + std::to_string(i + 1);
            }

            return "(" + printParenthesis(i, bracket[i][j]) + " * " +
                   printParenthesis(bracket[i][j] + 1, j) + ")";
        };

        return printParenthesis(0, n - 1);
    }

    /**
     * @brief Print the DP table for educational purposes
     * @param dims Array of matrix dimensions
     */
    static void printDPTable(const std::vector<int> &dims)
    {
        int n = dims.size() - 1;
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // l is chain length
        for (int l = 2; l <= n; l++)
        {
            for (int i = 0; i < n - l + 1; i++)
            {
                int j = i + l - 1;
                dp[i][j] = INF;

                for (int k = i; k < j; k++)
                {
                    int cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                    dp[i][j] = std::min(dp[i][j], cost);
                }
            }
        }

        // Print matrix dimensions
        std::cout << "Matrix Dimensions:\n";
        for (int i = 0; i < n; i++)
        {
            std::cout << "Matrix " << i + 1 << ": " << dims[i] << "x" << dims[i + 1] << "\n";
        }
        std::cout << "\n";

        // Print the table
        std::cout << "Dynamic Programming Table for Matrix Chain Multiplication:\n";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (j < i)
                {
                    std::cout << std::setw(10) << "";
                }
                else
                {
                    std::cout << std::setw(10) << (dp[i][j] == INF ? "INF" : std::to_string(dp[i][j]));
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};

/**
 * @brief Class containing Optimal Binary Search Tree implementations
 */
class OptimalBST
{
public:
    /**
     * @brief Calculate cost of optimal BST
     * @param keys Array of keys in sorted order
     * @param freq Array of key frequencies
     * @return Minimum cost of BST
     * @time O(n^3) - cubic time complexity
     * @space O(n^2) - quadratic space for dp table
     */
    static int optimalBST(const std::vector<int> &keys, const std::vector<int> &freq)
    {
        int n = keys.size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // Initialize for single nodes
        for (int i = 0; i < n; i++)
        {
            dp[i][i] = freq[i];
        }

        // Fill table for different lengths
        for (int l = 2; l <= n; l++)
        {
            for (int i = 0; i <= n - l; i++)
            {
                int j = i + l - 1;
                dp[i][j] = INF;

                // Calculate sum of frequencies in range [i,j]
                int sum = 0;
                for (int k = i; k <= j; k++)
                {
                    sum += freq[k];
                }

                // Try each key as root
                for (int r = i; r <= j; r++)
                {
                    int left_cost = (r > i) ? dp[i][r - 1] : 0;
                    int right_cost = (r < j) ? dp[r + 1][j] : 0;
                    int cost = left_cost + right_cost + sum;

                    dp[i][j] = std::min(dp[i][j], cost);
                }
            }
        }

        return dp[0][n - 1];
    }

    /**
     * @brief Optimized implementation with precomputed sums
     * @param keys Array of keys in sorted order
     * @param freq Array of key frequencies
     * @return Minimum cost of BST
     * @time O(n^3) - cubic time complexity
     * @space O(n^2) - quadratic space for dp table
     */
    static int optimalBSTOptimized(const std::vector<int> &keys, const std::vector<int> &freq)
    {
        int n = keys.size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // Precompute prefix sums
        std::vector<int> prefix_sum(n + 1, 0);
        for (int i = 0; i < n; i++)
        {
            prefix_sum[i + 1] = prefix_sum[i] + freq[i];
        }

        // Initialize for single nodes
        for (int i = 0; i < n; i++)
        {
            dp[i][i] = freq[i];
        }

        // Fill table for different lengths
        for (int l = 2; l <= n; l++)
        {
            for (int i = 0; i <= n - l; i++)
            {
                int j = i + l - 1;
                dp[i][j] = INF;

                // Calculate sum of frequencies in range [i,j]
                int sum = prefix_sum[j + 1] - prefix_sum[i];

                // Try each key as root
                for (int r = i; r <= j; r++)
                {
                    int left_cost = (r > i) ? dp[i][r - 1] : 0;
                    int right_cost = (r < j) ? dp[r + 1][j] : 0;
                    int cost = left_cost + right_cost + sum;

                    dp[i][j] = std::min(dp[i][j], cost);
                }
            }
        }

        return dp[0][n - 1];
    }

    /**
     * @brief Get the root structure of the optimal BST
     * @param keys Array of keys in sorted order
     * @param freq Array of key frequencies
     * @return Root indices for each subproblem
     */
    static std::vector<std::vector<int>> getOptimalBSTStructure(const std::vector<int> &keys, const std::vector<int> &freq)
    {
        int n = keys.size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> root(n, std::vector<int>(n, 0));

        // Precompute prefix sums
        std::vector<int> prefix_sum(n + 1, 0);
        for (int i = 0; i < n; i++)
        {
            prefix_sum[i + 1] = prefix_sum[i] + freq[i];
        }

        // Initialize for single nodes
        for (int i = 0; i < n; i++)
        {
            dp[i][i] = freq[i];
            root[i][i] = i;
        }

        // Fill table for different lengths
        for (int l = 2; l <= n; l++)
        {
            for (int i = 0; i <= n - l; i++)
            {
                int j = i + l - 1;
                dp[i][j] = INF;

                // Calculate sum of frequencies in range [i,j]
                int sum = prefix_sum[j + 1] - prefix_sum[i];

                // Try each key as root
                for (int r = i; r <= j; r++)
                {
                    int left_cost = (r > i) ? dp[i][r - 1] : 0;
                    int right_cost = (r < j) ? dp[r + 1][j] : 0;
                    int cost = left_cost + right_cost + sum;

                    if (cost < dp[i][j])
                    {
                        dp[i][j] = cost;
                        root[i][j] = r;
                    }
                }
            }
        }

        return root;
    }

    /**
     * @brief Print the optimal BST structure
     * @param keys Array of keys in sorted order
     * @param freq Array of key frequencies
     */
    static void printOptimalBST(const std::vector<int> &keys, const std::vector<int> &freq)
    {
        std::vector<std::vector<int>> root = getOptimalBSTStructure(keys, freq);

        std::function<void(int, int, int, std::string)> printBST =
            [&](int i, int j, int level, std::string prefix)
        {
            if (i > j)
                return;

            int r = root[i][j];
            std::cout << prefix << "Key: " << keys[r] << " (Freq: " << freq[r] << ")\n";

            if (i <= r - 1)
            {
                printBST(i, r - 1, level + 1, prefix + "  L: ");
            }
            else
            {
                std::cout << prefix << "  L: NULL\n";
            }

            if (r + 1 <= j)
            {
                printBST(r + 1, j, level + 1, prefix + "  R: ");
            }
            else
            {
                std::cout << prefix << "  R: NULL\n";
            }
        };

        std::cout << "Optimal Binary Search Tree Structure:\n";
        printBST(0, keys.size() - 1, 0, "");
    }
};

/**
 * @brief Class containing Palindrome related DP problems
 */
class PalindromeDP
{
public:
    /**
     * @brief Check if substring is palindrome
     * @param s Input string
     * @param i Start index
     * @param j End index
     * @return True if s[i...j] is palindrome
     */
    static bool isPalindrome(const std::string &s, int i, int j)
    {
        while (i < j)
        {
            if (s[i] != s[j])
                return false;
            i++;
            j--;
        }
        return true;
    }

    /**
     * @brief Precompute all palindrome substrings
     * @param s Input string
     * @return 2D array where dp[i][j] = true if s[i...j] is palindrome
     */
    static std::vector<std::vector<bool>> precomputePalindromes(const std::string &s)
    {
        int n = s.length();
        std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));

        // Single characters are palindromes
        for (int i = 0; i < n; i++)
        {
            dp[i][i] = true;
        }

        // Check for palindromes of length 2
        for (int i = 0; i < n - 1; i++)
        {
            dp[i][i + 1] = (s[i] == s[i + 1]);
        }

        // Check for palindromes of length > 2
        for (int len = 3; len <= n; len++)
        {
            for (int i = 0; i <= n - len; i++)
            {
                int j = i + len - 1;
                dp[i][j] = (s[i] == s[j]) && dp[i + 1][j - 1];
            }
        }

        return dp;
    }

    /**
     * @brief Find the longest palindromic substring
     * @param s Input string
     * @return Longest palindromic substring
     */
    static std::string longestPalindromicSubstring(const std::string &s)
    {
        int n = s.length();
        if (n == 0)
            return "";

        int start = 0, max_len = 1;

        std::vector<std::vector<bool>> dp = precomputePalindromes(s);

        // Find the longest palindrome
        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                if (dp[i][j] && j - i + 1 > max_len)
                {
                    max_len = j - i + 1;
                    start = i;
                }
            }
        }

        return s.substr(start, max_len);
    }

    /**
     * @brief Count all palindromic substrings
     * @param s Input string
     * @return Number of palindromic substrings
     */
    static int countPalindromicSubstrings(const std::string &s)
    {
        int n = s.length();
        int count = 0;

        std::vector<std::vector<bool>> dp = precomputePalindromes(s);

        // Count all palindromes
        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                if (dp[i][j])
                {
                    count++;
                }
            }
        }

        return count;
    }

    /**
     * @brief Minimum cuts needed to partition string into palindromes
     * @param s Input string
     * @return Minimum number of cuts
     */
    static int palindromePartitioning(const std::string &s)
    {
        int n = s.length();

        // Precompute palindromes
        std::vector<std::vector<bool>> isPal = precomputePalindromes(s);

        // dp[i] = minimum cuts for s[0...i]
        std::vector<int> dp(n, INF);

        for (int i = 0; i < n; i++)
        {
            // If s[0...i] is palindrome, no cuts needed
            if (isPal[0][i])
            {
                dp[i] = 0;
            }
            else
            {
                // Try all possible partitioning points
                for (int j = 0; j < i; j++)
                {
                    if (isPal[j + 1][i])
                    {
                        dp[i] = std::min(dp[i], dp[j] + 1);
                    }
                }
            }
        }

        return dp[n - 1];
    }

    /**
     * @brief Get all palindrome partitioning of a string
     * @param s Input string
     * @return All possible palindrome partitioning
     */
    static std::vector<std::vector<std::string>> palindromePartitioningAll(const std::string &s)
    {
        int n = s.length();
        std::vector<std::vector<bool>> isPal = precomputePalindromes(s);
        std::vector<std::vector<std::string>> result;
        std::vector<std::string> current;

        std::function<void(int)> backtrack = [&](int start)
        {
            if (start >= n)
            {
                result.push_back(current);
                return;
            }

            for (int end = start; end < n; end++)
            {
                if (isPal[start][end])
                {
                    current.push_back(s.substr(start, end - start + 1));
                    backtrack(end + 1);
                    current.pop_back();
                }
            }
        };

        backtrack(0);
        return result;
    }
};

/**
 * @brief Generate examples and benchmarks for the different algorithms
 */
class DPExamples
{
public:
    /**
     * @brief Run examples for Knapsack problem
     */
    static void knapsackExamples()
    {
        std::cout << "===== 0/1 Knapsack Problem =====\n\n";

        std::vector<int> weights = {1, 3, 4, 5};
        std::vector<int> values = {1, 4, 5, 7};
        int capacity = 7;

        std::cout << "Weights: ";
        for (int w : weights)
            std::cout << w << " ";
        std::cout << "\nValues: ";
        for (int v : values)
            std::cout << v << " ";
        std::cout << "\nCapacity: " << capacity << "\n\n";

        // Print DP table for educational purposes
        Knapsack::printDPTable(weights, values, capacity);

        // Naive recursive solution (may be slow for large inputs)
        {
            Timer timer("Naive recursive approach");
            int max_value = Knapsack::naive(weights, values, capacity);
            std::cout << "Maximum value: " << max_value << "\n";
        }

        // Memoized solution
        {
            Timer timer("Memoized approach");
            int max_value = Knapsack::memoized(weights, values, capacity);
            std::cout << "Maximum value: " << max_value << "\n";
        }

        // Tabulation solution
        {
            Timer timer("Tabulation approach");
            int max_value = Knapsack::tabulation(weights, values, capacity);
            std::cout << "Maximum value: " << max_value << "\n";
        }

        // Space-optimized solution
        {
            Timer timer("Space-optimized approach");
            int max_value = Knapsack::spaceOptimized(weights, values, capacity);
            std::cout << "Maximum value: " << max_value << "\n";
        }

        // Get selected items
        std::vector<int> selected = Knapsack::getSelectedItems(weights, values, capacity);
        std::cout << "\nSelected items: ";
        for (int idx : selected)
        {
            std::cout << "Item " << idx + 1 << " (Weight: " << weights[idx]
                      << ", Value: " << values[idx] << ")";
            if (&idx != &selected.back())
                std::cout << ", ";
        }
        std::cout << "\n";

        // Calculate total weight and value
        int total_weight = 0, total_value = 0;
        for (int idx : selected)
        {
            total_weight += weights[idx];
            total_value += values[idx];
        }

        std::cout << "Total weight: " << total_weight << " / " << capacity << "\n";
        std::cout << "Total value: " << total_value << "\n\n";

        // Unbounded knapsack
        std::cout << "===== Unbounded Knapsack Problem =====\n\n";

        {
            Timer timer("Unbounded knapsack");
            int max_value = Knapsack::unboundedKnapsack(weights, values, capacity);
            std::cout << "Maximum value: " << max_value << "\n";
        }

        std::vector<int> item_counts = Knapsack::getUnboundedKnapsackItems(weights, values, capacity);
        std::cout << "\nItems taken:\n";

        total_weight = 0;
        total_value = 0;

        for (int i = 0; i < item_counts.size(); i++)
        {
            if (item_counts[i] > 0)
            {
                std::cout << item_counts[i] << "x Item " << i + 1
                          << " (Weight: " << weights[i] << ", Value: " << values[i] << ")\n";
                total_weight += weights[i] * item_counts[i];
                total_value += values[i] * item_counts[i];
            }
        }

        std::cout << "Total weight: " << total_weight << " / " << capacity << "\n";
        std::cout << "Total value: " << total_value << "\n\n";

        // Knapsack variations
        std::cout << "===== Knapsack Variations =====\n\n";

        // Subset Sum
        std::vector<int> nums = {2, 3, 7, 8, 10};
        int target = 11;

        std::cout << "Subset Sum Problem:\n";
        std::cout << "Array: ";
        for (int num : nums)
            std::cout << num << " ";
        std::cout << "\nTarget: " << target << "\n";

        {
            Timer timer("Subset Sum");
            bool canMakeSum = KnapsackVariations::subsetSum(nums, target);
            std::cout << "Can make sum " << target << ": " << (canMakeSum ? "Yes" : "No") << "\n";
        }

        // Partition Equal Subset Sum
        std::vector<int> partition_nums = {1, 5, 11, 5};

        std::cout << "\nPartition Equal Subset Sum Problem:\n";
        std::cout << "Array: ";
        for (int num : partition_nums)
            std::cout << num << " ";
        std::cout << "\n";

        {
            Timer timer("Partition Equal Subset Sum");
            bool canPartition = KnapsackVariations::canPartition(partition_nums);
            std::cout << "Can partition equally: " << (canPartition ? "Yes" : "No") << "\n";
        }

        // Minimum Subset Sum Difference
        std::vector<int> diff_nums = {3, 1, 4, 2, 2, 1};

        std::cout << "\nMinimum Subset Sum Difference Problem:\n";
        std::cout << "Array: ";
        for (int num : diff_nums)
            std::cout << num << " ";
        std::cout << "\n";

        {
            Timer timer("Minimum Subset Sum Difference");
            int min_diff = KnapsackVariations::minimumSubsetSumDifference(diff_nums);
            std::cout << "Minimum difference: " << min_diff << "\n";
        }

        // Coin Change
        std::vector<int> coins = {1, 2, 5};
        int amount = 11;

        std::cout << "\nCoin Change Problem:\n";
        std::cout << "Coins: ";
        for (int coin : coins)
            std::cout << coin << " ";
        std::cout << "\nAmount: " << amount << "\n";

        {
            Timer timer("Coin Change");
            int min_coins = KnapsackVariations::coinChange(coins, amount);
            std::cout << "Minimum number of coins: " << min_coins << "\n";
        }

        // Coin Change 2
        std::cout << "\nCoin Change 2 Problem:\n";

        {
            Timer timer("Coin Change 2");
            int ways = KnapsackVariations::coinChange2(coins, amount);
            std::cout << "Number of ways: " << ways << "\n";
        }

        // Rod Cutting
        std::vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20};
        int rod_length = 8;

        std::cout << "\nRod Cutting Problem:\n";
        std::cout << "Prices: ";
        for (int i = 0; i < prices.size(); i++)
        {
            std::cout << "Length " << i + 1 << " = $" << prices[i];
            if (i < prices.size() - 1)
                std::cout << ", ";
        }
        std::cout << "\nRod length: " << rod_length << "\n";

        {
            Timer timer("Rod Cutting");
            int max_profit = KnapsackVariations::rodCutting(prices, rod_length);
            std::cout << "Maximum profit: $" << max_profit << "\n";
        }

        std::vector<int> cuts = KnapsackVariations::getRodCuttingSolution(prices, rod_length);
        std::cout << "Optimal cuts: ";
        for (int cut : cuts)
        {
            std::cout << cut;
            if (&cut != &cuts.back())
                std::cout << " + ";
        }
        std::cout << " = " << rod_length << "\n\n";
    }

    /**
     * @brief Run examples for Longest Common Subsequence
     */
    static void lcsExamples()
    {
        std::cout << "===== Longest Common Subsequence =====\n\n";

        std::string s1 = "ABCBDAB";
        std::string s2 = "BDCABA";

        std::cout << "String 1: " << s1 << "\n";
        std::cout << "String 2: " << s2 << "\n\n";

        // Print DP table for educational purposes
        LongestCommonSubsequence::printDPTable(s1, s2);

        // Naive recursive solution (may be slow for long strings)
        {
            Timer timer("Naive recursive approach");
            int length = LongestCommonSubsequence::naive(s1, s2);
            std::cout << "LCS length: " << length << "\n";
        }

        // Memoized solution
        {
            Timer timer("Memoized approach");
            int length = LongestCommonSubsequence::memoized(s1, s2);
            std::cout << "LCS length: " << length << "\n";
        }

        // Tabulation solution
        {
            Timer timer("Tabulation approach");
            int length = LongestCommonSubsequence::tabulation(s1, s2);
            std::cout << "LCS length: " << length << "\n";
        }

        // Space-optimized solution
        {
            Timer timer("Space-optimized approach");
            int length = LongestCommonSubsequence::spaceOptimized(s1, s2);
            std::cout << "LCS length: " << length << "\n";
        }

        // Print the LCS
        std::string lcs = LongestCommonSubsequence::printLCS(s1, s2);
        std::cout << "Longest Common Subsequence: " << lcs << "\n\n";

        // Longest Common Substring
        std::cout << "===== Longest Common Substring =====\n\n";

        std::string s3 = "ABCDEF";
        std::string s4 = "XBCYEF";

        std::cout << "String 1: " << s3 << "\n";
        std::cout << "String 2: " << s4 << "\n\n";

        {
            Timer timer("Longest Common Substring");
            int length = LongestCommonSubsequence::longestCommonSubstring(s3, s4);
            std::cout << "Longest Common Substring length: " << length << "\n";
        }

        std::string lcsubstr = LongestCommonSubsequence::printLongestCommonSubstring(s3, s4);
        std::cout << "Longest Common Substring: " << lcsubstr << "\n\n";

        // Shortest Common Supersequence
        std::cout << "===== Shortest Common Supersequence =====\n\n";

        std::string s5 = "AGGTAB";
        std::string s6 = "GXTXAYB";

        std::cout << "String 1: " << s5 << "\n";
        std::cout << "String 2: " << s6 << "\n\n";

        {
            Timer timer("Shortest Common Supersequence");
            int length = LongestCommonSubsequence::shortestCommonSupersequence(s5, s6);
            std::cout << "SCS length: " << length << "\n";
        }

        std::string scs = LongestCommonSubsequence::printShortestCommonSupersequence(s5, s6);
        std::cout << "Shortest Common Supersequence: " << scs << "\n\n";

        // Longest Palindromic Subsequence
        std::cout << "===== Longest Palindromic Subsequence =====\n\n";

        std::string s7 = "BBABCBCAB";

        std::cout << "String: " << s7 << "\n\n";

        {
            Timer timer("Longest Palindromic Subsequence");
            int length = LongestCommonSubsequence::longestPalindromicSubsequence(s7);
            std::cout << "LPS length: " << length << "\n";
        }

        std::string lps = LongestCommonSubsequence::printLongestPalindromicSubsequence(s7);
        std::cout << "Longest Palindromic Subsequence: " << lps << "\n\n";
    }

    /**
     * @brief Run examples for Edit Distance
     */
    static void editDistanceExamples()
    {
        std::cout << "===== Edit Distance =====\n\n";

        std::string word1 = "horse";
        std::string word2 = "ros";

        std::cout << "Word 1: " << word1 << "\n";
        std::cout << "Word 2: " << word2 << "\n\n";

        // Print DP table for educational purposes
        EditDistance::printDPTable(word1, word2);

        // Standard solution
        {
            Timer timer("Edit Distance");
            int distance = EditDistance::minDistance(word1, word2);
            std::cout << "Edit Distance: " << distance << "\n";
        }

        // Space-optimized solution
        {
            Timer timer("Space-optimized Edit Distance");
            int distance = EditDistance::minDistanceOptimized(word1, word2);
            std::cout << "Edit Distance (optimized): " << distance << "\n";
        }

        // Get the edit operations
        std::vector<std::string> operations = EditDistance::getEditOperations(word1, word2);
        std::cout << "\nEdit Operations:\n";
        for (const std::string &op : operations)
        {
            std::cout << "- " << op << "\n";
        }

        // Different types of edit distance
        std::cout << "\n===== Different Types of Edit Distance =====\n\n";

        std::string word3 = "saturday";
        std::string word4 = "sunday";

        std::cout << "Word 1: " << word3 << "\n";
        std::cout << "Word 2: " << word4 << "\n\n";

        {
            Timer timer("Levenshtein Distance");
            int distance = EditDistance::levenshteinDistance(word3, word4);
            std::cout << "Levenshtein Distance: " << distance << "\n";
        }

        {
            Timer timer("Damerau-Levenshtein Distance");
            int distance = EditDistance::damerauLevenshteinDistance(word3, word4);
            std::cout << "Damerau-Levenshtein Distance: " << distance << "\n";
        }

        // One Edit Distance check
        std::string word5 = "cat";
        std::string word6 = "cut";

        std::cout << "\nChecking if \"" << word5 << "\" and \"" << word6 << "\" are one edit distance apart: ";
        std::cout << (EditDistance::isOneEditDistance(word5, word6) ? "Yes" : "No") << "\n\n";
    }

    /**
     * @brief Run examples for Matrix Chain Multiplication
     */
    static void matrixChainExamples()
    {
        std::cout << "===== Matrix Chain Multiplication =====\n\n";

        std::vector<int> dims = {40, 20, 30, 10, 30};

        std::cout << "Matrix dimensions: ";
        for (int i = 0; i < dims.size() - 1; i++)
        {
            std::cout << dims[i] << "x" << dims[i + 1];
            if (i < dims.size() - 2)
                std::cout << ", ";
        }
        std::cout << "\n\n";

        // Print DP table for educational purposes
        MatrixChainMultiplication::printDPTable(dims);

        // Bottom-up solution
        {
            Timer timer("Matrix Chain Multiplication");
            int min_ops = MatrixChainMultiplication::matrixChainOrder(dims);
            std::cout << "Minimum number of scalar multiplications: " << min_ops << "\n";
        }

        // Memoized solution
        {
            Timer timer("Memoized Matrix Chain");
            int min_ops = MatrixChainMultiplication::matrixChainMemoized(dims);
            std::cout << "Minimum number of scalar multiplications (memoized): " << min_ops << "\n";
        }

        // Get the optimal parenthesization
        std::string parenthesization = MatrixChainMultiplication::getOptimalParenthesization(dims);
        std::cout << "\nOptimal Parenthesization: " << parenthesization << "\n\n";
    }

    /**
     * @brief Run examples for Optimal BST
     */
    static void optimalBSTExamples()
    {
        std::cout << "===== Optimal Binary Search Tree =====\n\n";

        std::vector<int> keys = {10, 12, 20};
        std::vector<int> freq = {34, 8, 50};

        std::cout << "Keys: ";
        for (int key : keys)
            std::cout << key << " ";
        std::cout << "\nFrequencies: ";
        for (int f : freq)
            std::cout << f << " ";
        std::cout << "\n\n";

        // Standard solution
        {
            Timer timer("Optimal BST");
            int cost = OptimalBST::optimalBST(keys, freq);
            std::cout << "Minimum search cost: " << cost << "\n";
        }

        // Optimized solution
        {
            Timer timer("Optimized Optimal BST");
            int cost = OptimalBST::optimalBSTOptimized(keys, freq);
            std::cout << "Minimum search cost (optimized): " << cost << "\n";
        }

        // Print the optimal BST
        std::cout << "\n";
        OptimalBST::printOptimalBST(keys, freq);
        std::cout << "\n";
    }

    /**
     * @brief Run examples for Palindrome DP problems
     */
    static void palindromeExamples()
    {
        std::cout << "===== Palindrome DP Problems =====\n\n";

        std::string s = "babad";

        std::cout << "String: " << s << "\n\n";

        // Longest Palindromic Substring
        {
            Timer timer("Longest Palindromic Substring");
            std::string lps = PalindromeDP::longestPalindromicSubstring(s);
            std::cout << "Longest Palindromic Substring: " << lps << "\n";
        }

        // Count Palindromic Substrings
        {
            Timer timer("Count Palindromic Substrings");
            int count = PalindromeDP::countPalindromicSubstrings(s);
            std::cout << "Count of Palindromic Substrings: " << count << "\n";
        }

        // Palindrome Partitioning
        {
            Timer timer("Palindrome Partitioning");
            int cuts = PalindromeDP::palindromePartitioning(s);
            std::cout << "Minimum Cuts for Palindrome Partitioning: " << cuts << "\n";
        }

        // Get all Palindrome Partitioning
        {
            Timer timer("All Palindrome Partitioning");
            std::vector<std::vector<std::string>> partitions = PalindromeDP::palindromePartitioningAll(s);
            std::cout << "All Palindrome Partitioning:\n";
            for (const auto &partition : partitions)
            {
                for (const std::string &part : partition)
                {
                    std::cout << part;
                    if (&part != &partition.back())
                        std::cout << " | ";
                }
                std::cout << "\n";
            }
        }

        std::cout << "\n";
    }
};

/**
 * @brief Main function demonstrating DP algorithms
 */
int main()
{
    std::cout << "==================================\n";
    std::cout << "Dynamic Programming - Part 2\n";
    std::cout << "==================================\n\n";

    // Ask user which examples to run
    std::cout << "Which examples would you like to run?\n";
    std::cout << "1. Knapsack Problem\n";
    std::cout << "2. Longest Common Subsequence\n";
    std::cout << "3. Edit Distance\n";
    std::cout << "4. Matrix Chain Multiplication\n";
    std::cout << "5. Optimal Binary Search Tree\n";
    std::cout << "6. Palindrome Problems\n";
    std::cout << "7. All Examples\n";
    std::cout << "Enter choice (1-7): ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        DPExamples::knapsackExamples();
        break;
    case 2:
        DPExamples::lcsExamples();
        break;
    case 3:
        DPExamples::editDistanceExamples();
        break;
    case 4:
        DPExamples::matrixChainExamples();
        break;
    case 5:
        DPExamples::optimalBSTExamples();
        break;
    case 6:
        DPExamples::palindromeExamples();
        break;
    case 7:
        DPExamples::knapsackExamples();
        DPExamples::lcsExamples();
        DPExamples::editDistanceExamples();
        DPExamples::matrixChainExamples();
        DPExamples::optimalBSTExamples();
        DPExamples::palindromeExamples();
        break;
    default:
        std::cout << "Invalid choice. Exiting.\n";
    }

    std::cout << "\nThank you for exploring Advanced Dynamic Programming!\n";
    return 0;
}