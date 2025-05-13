/**
 * @file dsa_toolkit.cpp
 * @brief Comprehensive toolkit of DSA concepts covered in the #DSAin45 series
 *
 * This file contains implementations of key algorithms and data structures
 * covered throughout the 45-day journey, serving as both a reference and
 * a practice tool for continued learning. Each implementation includes
 * detailed comments, complexity analysis, and usage examples.
 *
 * Part of the #DSAin45 series - Day 45: Journey Completion & Next Steps
 *
 * @author #DSAin45
 * @date May 2025
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <functional>
#include <limits>
#include <numeric>
#include <cmath>
#include <chrono>
#include <random>
#include <utility>
#include <cassert>

using namespace std;

/**
 * @brief Namespace containing core array and string algorithms
 */
namespace ArrayStringAlgorithms
{
    /**
     * @brief Two Sum - Find indices of two numbers that add up to target
     * @param nums Array of integers
     * @param target Target sum
     * @return Vector of two indices
     *
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    vector<int> twoSum(const vector<int> &nums, int target)
    {
        unordered_map<int, int> numToIndex;

        for (int i = 0; i < nums.size(); i++)
        {
            int complement = target - nums[i];

            if (numToIndex.count(complement))
            {
                return {numToIndex[complement], i};
            }

            numToIndex[nums[i]] = i;
        }

        return {}; // No solution found
    }

    /**
     * @brief Kadane's algorithm for maximum subarray sum
     * @param nums Input array
     * @return Maximum subarray sum
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    int maxSubarraySum(const vector<int> &nums)
    {
        int maxEndingHere = nums[0];
        int maxSoFar = nums[0];

        for (int i = 1; i < nums.size(); i++)
        {
            maxEndingHere = max(nums[i], maxEndingHere + nums[i]);
            maxSoFar = max(maxSoFar, maxEndingHere);
        }

        return maxSoFar;
    }

    /**
     * @brief Sliding window for longest substring without repeating characters
     * @param s Input string
     * @return Length of longest substring
     *
     * Time Complexity: O(n)
     * Space Complexity: O(min(m, n)) where m is the size of character set
     */
    int lengthOfLongestSubstring(const string &s)
    {
        unordered_set<char> charSet;
        int maxLength = 0;
        int left = 0;

        for (int right = 0; right < s.length(); right++)
        {
            while (charSet.count(s[right]))
            {
                charSet.erase(s[left]);
                left++;
            }

            charSet.insert(s[right]);
            maxLength = max(maxLength, right - left + 1);
        }

        return maxLength;
    }

    /**
     * @brief Dutch national flag algorithm for sorting 0s, 1s, and 2s
     * @param nums Input array containing only 0s, 1s, and 2s
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    void sortColors(vector<int> &nums)
    {
        int low = 0;                // Boundary for 0s
        int mid = 0;                // Current element
        int high = nums.size() - 1; // Boundary for 2s

        while (mid <= high)
        {
            if (nums[mid] == 0)
            {
                swap(nums[low], nums[mid]);
                low++;
                mid++;
            }
            else if (nums[mid] == 1)
            {
                mid++;
            }
            else
            { // nums[mid] == 2
                swap(nums[mid], nums[high]);
                high--;
                // Don't increment mid here because the swapped element needs to be processed
            }
        }
    }
}

/**
 * @brief Implementation of key linked list algorithms
 */
namespace LinkedListAlgorithms
{
    /**
     * @brief Definition for singly-linked list node
     */
    struct ListNode
    {
        int val;
        ListNode *next;
        ListNode(int x) : val(x), next(nullptr) {}
    };

    /**
     * @brief Reverse a linked list
     * @param head Head of the linked list
     * @return New head of the reversed list
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    ListNode *reverseList(ListNode *head)
    {
        ListNode *prev = nullptr;
        ListNode *current = head;

        while (current)
        {
            ListNode *nextTemp = current->next;
            current->next = prev;
            prev = current;
            current = nextTemp;
        }

        return prev;
    }

    /**
     * @brief Detect cycle in a linked list using Floyd's algorithm
     * @param head Head of the linked list
     * @return True if cycle exists, false otherwise
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    bool hasCycle(ListNode *head)
    {
        if (!head || !head->next)
            return false;

        ListNode *slow = head;
        ListNode *fast = head;

        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Merge two sorted linked lists
     * @param l1 First sorted linked list
     * @param l2 Second sorted linked list
     * @return Merged sorted linked list
     *
     * Time Complexity: O(n + m)
     * Space Complexity: O(1)
     */
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2)
    {
        ListNode dummy(0);
        ListNode *tail = &dummy;

        while (l1 && l2)
        {
            if (l1->val <= l2->val)
            {
                tail->next = l1;
                l1 = l1->next;
            }
            else
            {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        // Attach remaining elements
        tail->next = l1 ? l1 : l2;

        return dummy.next;
    }

    /**
     * @brief Find the middle of a linked list
     * @param head Head of the linked list
     * @return Middle node
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    ListNode *middleNode(ListNode *head)
    {
        ListNode *slow = head;
        ListNode *fast = head;

        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }
}

/**
 * @brief Stack-based algorithms and implementations
 */
namespace StackAlgorithms
{
    /**
     * @brief Check if string of brackets is valid
     * @param s String containing only '(', ')', '{', '}', '[', ']'
     * @return true if brackets are valid, false otherwise
     *
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    bool isValid(const string &s)
    {
        stack<char> brackets;
        unordered_map<char, char> bracketPairs = {
            {')', '('},
            {'}', '{'},
            {']', '['}};

        for (char c : s)
        {
            if (bracketPairs.count(c))
            {
                if (brackets.empty() || brackets.top() != bracketPairs[c])
                {
                    return false;
                }
                brackets.pop();
            }
            else
            {
                brackets.push(c);
            }
        }

        return brackets.empty();
    }

    /**
     * @brief Evaluate Reverse Polish Notation (postfix expression)
     * @param tokens Vector of tokens
     * @return Result of expression evaluation
     *
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    int evalRPN(const vector<string> &tokens)
    {
        stack<int> values;

        for (const string &token : tokens)
        {
            if (token == "+" || token == "-" || token == "*" || token == "/")
            {
                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();

                if (token == "+")
                    values.push(a + b);
                else if (token == "-")
                    values.push(a - b);
                else if (token == "*")
                    values.push(a * b);
                else if (token == "/")
                    values.push(a / b);
            }
            else
            {
                values.push(stoi(token));
            }
        }

        return values.top();
    }

    /**
     * @brief Find next greater element for each element in array
     * @param nums Array of integers
     * @return Vector where result[i] is the next greater element for nums[i]
     *
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    vector<int> nextGreaterElement(const vector<int> &nums)
    {
        int n = nums.size();
        vector<int> result(n, -1);
        stack<int> s;

        for (int i = 0; i < n; i++)
        {
            while (!s.empty() && nums[i] > nums[s.top()])
            {
                result[s.top()] = nums[i];
                s.pop();
            }
            s.push(i);
        }

        return result;
    }
}

/**
 * @brief Binary tree algorithms and utilities
 */
namespace TreeAlgorithms
{
    /**
     * @brief Definition for a binary tree node
     */
    struct TreeNode
    {
        int val;
        TreeNode *left;
        TreeNode *right;
        TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Perform inorder traversal of binary tree (recursive)
     * @param root Root of the binary tree
     * @param result Vector to store the traversal
     *
     * Time Complexity: O(n)
     * Space Complexity: O(h) where h is the height of the tree
     */
    void inorderTraversal(TreeNode *root, vector<int> &result)
    {
        if (!root)
            return;

        inorderTraversal(root->left, result);
        result.push_back(root->val);
        inorderTraversal(root->right, result);
    }

    /**
     * @brief Perform inorder traversal (iterative with stack)
     * @param root Root of the binary tree
     * @return Vector containing inorder traversal
     *
     * Time Complexity: O(n)
     * Space Complexity: O(h) where h is the height of the tree
     */
    vector<int> inorderTraversalIterative(TreeNode *root)
    {
        vector<int> result;
        stack<TreeNode *> s;
        TreeNode *curr = root;

        while (curr || !s.empty())
        {
            // Reach the leftmost node
            while (curr)
            {
                s.push(curr);
                curr = curr->left;
            }

            // Current is now null, get the top node
            curr = s.top();
            s.pop();

            // Add the value to result
            result.push_back(curr->val);

            // Move to the right subtree
            curr = curr->right;
        }

        return result;
    }

    /**
     * @brief Level order traversal of binary tree
     * @param root Root of the binary tree
     * @return Vector of vectors containing nodes at each level
     *
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    vector<vector<int>> levelOrder(TreeNode *root)
    {
        vector<vector<int>> result;
        if (!root)
            return result;

        queue<TreeNode *> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size();
            vector<int> currentLevel;

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *node = q.front();
                q.pop();

                currentLevel.push_back(node->val);

                if (node->left)
                    q.push(node->left);
                if (node->right)
                    q.push(node->right);
            }

            result.push_back(currentLevel);
        }

        return result;
    }

    /**
     * @brief Check if a binary tree is a valid BST
     * @param root Root of the binary tree
     * @return True if valid BST, false otherwise
     *
     * Time Complexity: O(n)
     * Space Complexity: O(h) where h is the height of the tree
     */
    bool isValidBST(TreeNode *root)
    {
        return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
    }

    /**
     * @brief Helper function for BST validation
     */
    bool isValidBSTHelper(TreeNode *root, long minVal, long maxVal)
    {
        if (!root)
            return true;

        if (root->val <= minVal || root->val >= maxVal)
        {
            return false;
        }

        return isValidBSTHelper(root->left, minVal, root->val) &&
               isValidBSTHelper(root->right, root->val, maxVal);
    }

    /**
     * @brief Find lowest common ancestor in a binary tree
     * @param root Root of the binary tree
     * @param p First node
     * @param q Second node
     * @return Lowest common ancestor node
     *
     * Time Complexity: O(n)
     * Space Complexity: O(h)
     */
    TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
    {
        // Base case
        if (!root || root == p || root == q)
        {
            return root;
        }

        // Look for p and q in left and right subtrees
        TreeNode *left = lowestCommonAncestor(root->left, p, q);
        TreeNode *right = lowestCommonAncestor(root->right, p, q);

        // If both nodes were found in different subtrees
        if (left && right)
        {
            return root;
        }

        // If both were found in the left subtree
        if (left)
        {
            return left;
        }

        // If both were found in the right subtree
        return right;
    }
}

/**
 * @brief Graph algorithms and implementations
 */
namespace GraphAlgorithms
{
    /**
     * @brief Perform depth-first search on a graph
     * @param graph Adjacency list representation of the graph
     * @param start Starting vertex
     * @return Vector of vertices in DFS order
     *
     * Time Complexity: O(V + E)
     * Space Complexity: O(V)
     */
    vector<int> dfs(const vector<vector<int>> &graph, int start)
    {
        vector<int> result;
        vector<bool> visited(graph.size(), false);
        stack<int> s;

        s.push(start);

        while (!s.empty())
        {
            int vertex = s.top();
            s.pop();

            if (!visited[vertex])
            {
                visited[vertex] = true;
                result.push_back(vertex);

                // Push neighbors in reverse order to visit them in the original order
                for (int i = graph[vertex].size() - 1; i >= 0; i--)
                {
                    if (!visited[graph[vertex][i]])
                    {
                        s.push(graph[vertex][i]);
                    }
                }
            }
        }

        return result;
    }

    /**
     * @brief Perform breadth-first search on a graph
     * @param graph Adjacency list representation of the graph
     * @param start Starting vertex
     * @return Vector of vertices in BFS order
     *
     * Time Complexity: O(V + E)
     * Space Complexity: O(V)
     */
    vector<int> bfs(const vector<vector<int>> &graph, int start)
    {
        vector<int> result;
        vector<bool> visited(graph.size(), false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int vertex = q.front();
            q.pop();
            result.push_back(vertex);

            for (int neighbor : graph[vertex])
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        return result;
    }

    /**
     * @brief Dijkstra's algorithm for single-source shortest paths
     * @param graph Weighted adjacency list (vector of pairs (vertex, weight))
     * @param start Starting vertex
     * @return Vector of shortest distances from start to all vertices
     *
     * Time Complexity: O((V + E) log V)
     * Space Complexity: O(V)
     */
    vector<int> dijkstra(const vector<vector<pair<int, int>>> &graph, int start)
    {
        int n = graph.size();
        vector<int> dist(n, INT_MAX);
        dist[start] = 0;

        // Min heap of pairs (distance, vertex)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        while (!pq.empty())
        {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            // Skip if we've found a better path already
            if (d > dist[u])
                continue;

            // Explore neighbors
            for (auto &edge : graph[u])
            {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }

    /**
     * @brief Kruskal's algorithm for minimum spanning tree
     * @param n Number of vertices
     * @param edges Vector of edges (u, v, weight)
     * @return Total weight of MST
     *
     * Time Complexity: O(E log E)
     * Space Complexity: O(V)
     */
    int kruskalMST(int n, vector<vector<int>> &edges)
    {
        // Union-Find data structure
        vector<int> parent(n);
        vector<int> rank(n, 0);

        for (int i = 0; i < n; i++)
        {
            parent[i] = i; // Each vertex is its own parent initially
        }

        // Find function with path compression
        function<int(int)> find = [&](int x)
        {
            if (parent[x] != x)
            {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        };

        // Union function with rank
        auto unite = [&](int x, int y)
        {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX != rootY)
            {
                if (rank[rootX] < rank[rootY])
                {
                    parent[rootX] = rootY;
                }
                else
                {
                    parent[rootY] = rootX;
                    if (rank[rootX] == rank[rootY])
                    {
                        rank[rootX]++;
                    }
                }
                return true;
            }
            return false;
        };

        // Sort edges by weight
        sort(edges.begin(), edges.end(), [](const vector<int> &a, const vector<int> &b)
             { return a[2] < b[2]; });

        int mstWeight = 0;

        for (const auto &edge : edges)
        {
            int u = edge[0];
            int v = edge[1];
            int weight = edge[2];

            if (unite(u, v))
            {
                mstWeight += weight;
            }
        }

        return mstWeight;
    }
}

/**
 * @brief Dynamic programming algorithms and patterns
 */
namespace DPAlgorithms
{
    /**
     * @brief Solve 0/1 Knapsack problem using dynamic programming
     * @param values Values of n items
     * @param weights Weights of n items
     * @param capacity Knapsack capacity
     * @return Maximum value that can be put in knapsack
     *
     * Time Complexity: O(n * capacity)
     * Space Complexity: O(n * capacity)
     */
    int knapsack(const vector<int> &values, const vector<int> &weights, int capacity)
    {
        int n = values.size();
        vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

        for (int i = 1; i <= n; i++)
        {
            for (int w = 0; w <= capacity; w++)
            {
                // If current item weight is less than capacity, we have two options
                if (weights[i - 1] <= w)
                {
                    // Take the maximum of:
                    // 1. Include current item
                    // 2. Exclude current item
                    dp[i][w] = max(
                        values[i - 1] + dp[i - 1][w - weights[i - 1]],
                        dp[i - 1][w]);
                }
                else
                {
                    // Current item cannot be included
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }

        return dp[n][capacity];
    }

    /**
     * @brief Calculate the length of longest common subsequence
     * @param text1 First string
     * @param text2 Second string
     * @return Length of LCS
     *
     * Time Complexity: O(m * n)
     * Space Complexity: O(m * n)
     */
    int longestCommonSubsequence(const string &text1, const string &text2)
    {
        int m = text1.length();
        int n = text2.length();

        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (text1[i - 1] == text2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        return dp[m][n];
    }

    /**
     * @brief Calculate minimum number of coins needed to make amount
     * @param coins Vector of coin denominations
     * @param amount Target amount
     * @return Minimum number of coins or -1 if impossible
     *
     * Time Complexity: O(amount * coins.size())
     * Space Complexity: O(amount)
     */
    int coinChange(const vector<int> &coins, int amount)
    {
        vector<int> dp(amount + 1, amount + 1); // Initialize with value greater than any possible answer
        dp[0] = 0;                              // Base case: 0 coins needed to make amount 0

        for (int i = 1; i <= amount; i++)
        {
            for (int coin : coins)
            {
                if (coin <= i)
                {
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }

        return dp[amount] > amount ? -1 : dp[amount];
    }

    /**
     * @brief Find length of longest increasing subsequence
     * @param nums Vector of integers
     * @return Length of LIS
     *
     * Time Complexity: O(n log n)
     * Space Complexity: O(n)
     */
    int lengthOfLIS(const vector<int> &nums)
    {
        if (nums.empty())
            return 0;

        vector<int> tails; // tails[i] = smallest value that ends an increasing subsequence of length i+1

        for (int num : nums)
        {
            // Binary search to find the position to insert num
            auto it = lower_bound(tails.begin(), tails.end(), num);

            if (it == tails.end())
            {
                // Num is greater than all elements in tails, extend LIS
                tails.push_back(num);
            }
            else
            {
                // Replace the smallest element >= num
                *it = num;
            }
        }

        return tails.size();
    }
}

/**
 * @brief Backtracking algorithms and patterns
 */
namespace BacktrackingAlgorithms
{
    /**
     * @brief Generate all permutations of a set of distinct integers
     * @param nums Vector of distinct integers
     * @return All possible permutations
     *
     * Time Complexity: O(n!)
     * Space Complexity: O(n) for recursion stack
     */
    vector<vector<int>> permute(const vector<int> &nums)
    {
        vector<vector<int>> result;
        vector<int> current = nums;

        function<void(int)> backtrack = [&](int start)
        {
            if (start == nums.size())
            {
                result.push_back(current);
                return;
            }

            for (int i = start; i < nums.size(); i++)
            {
                // Swap elements to generate permutation
                swap(current[start], current[i]);

                // Recurse with next position
                backtrack(start + 1);

                // Backtrack by undoing the swap
                swap(current[start], current[i]);
            }
        };

        backtrack(0);
        return result;
    }

    /**
     * @brief Generate all subsets of a set of distinct integers
     * @param nums Vector of distinct integers
     * @return All possible subsets
     *
     * Time Complexity: O(2^n)
     * Space Complexity: O(n) for recursion stack
     */
    vector<vector<int>> subsets(const vector<int> &nums)
    {
        vector<vector<int>> result;
        vector<int> current;

        function<void(int)> backtrack = [&](int start)
        {
            result.push_back(current);

            for (int i = start; i < nums.size(); i++)
            {
                // Include nums[i] in the current subset
                current.push_back(nums[i]);

                // Generate all subsets with nums[i] included
                backtrack(i + 1);

                // Backtrack by removing nums[i]
                current.pop_back();
            }
        };

        backtrack(0);
        return result;
    }

    /**
     * @brief Solve N-Queens problem
     * @param n Board size
     * @return All valid N-Queens solutions
     *
     * Time Complexity: O(n!)
     * Space Complexity: O(n) for recursion stack
     */
    vector<vector<string>> solveNQueens(int n)
    {
        vector<vector<string>> result;
        vector<string> board(n, string(n, '.'));

        function<bool(int, int)> isValid = [&](int row, int col)
        {
            // Check column
            for (int i = 0; i < row; i++)
            {
                if (board[i][col] == 'Q')
                {
                    return false;
                }
            }

            // Check upper-left diagonal
            for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
            {
                if (board[i][j] == 'Q')
                {
                    return false;
                }
            }

            // Check upper-right diagonal
            for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++)
            {
                if (board[i][j] == 'Q')
                {
                    return false;
                }
            }

            return true;
        };

        function<void(int)> backtrack = [&](int row)
        {
            if (row == n)
            {
                result.push_back(board);
                return;
            }

            for (int col = 0; col < n; col++)
            {
                if (isValid(row, col))
                {
                    // Place queen
                    board[row][col] = 'Q';

                    // Try next row
                    backtrack(row + 1);

                    // Backtrack
                    board[row][col] = '.';
                }
            }
        };

        backtrack(0);
        return result;
    }
}

/**
 * @brief Greedy algorithms and patterns
 */
namespace GreedyAlgorithms
{
    /**
     * @brief Solve Activity Selection problem
     * @param start Vector of activity start times
     * @param finish Vector of activity finish times
     * @return Maximum number of non-overlapping activities
     *
     * Time Complexity: O(n log n)
     * Space Complexity: O(n)
     */
    int activitySelection(vector<int> &start, vector<int> &finish)
    {
        int n = start.size();

        // Create vector of activity indices
        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);

        // Sort indices by finish time
        sort(indices.begin(), indices.end(), [&](int i, int j)
             { return finish[i] < finish[j]; });

        int count = 0;
        int lastFinish = -1;

        for (int idx : indices)
        {
            if (start[idx] >= lastFinish)
            {
                count++;
                lastFinish = finish[idx];
            }
        }

        return count;
    }

    /**
     * @brief Find minimum number of intervals to remove to make rest non-overlapping
     * @param intervals Vector of intervals [start, end]
     * @return Minimum number of intervals to remove
     *
     * Time Complexity: O(n log n)
     * Space Complexity: O(1) excluding input storage
     */
    int eraseOverlapIntervals(vector<vector<int>> &intervals)
    {
        if (intervals.empty())
            return 0;

        // Sort by end time
        sort(intervals.begin(), intervals.end(), [](const vector<int> &a, const vector<int> &b)
             { return a[1] < b[1]; });

        int count = 0;
        int end = intervals[0][1];

        for (int i = 1; i < intervals.size(); i++)
        {
            if (intervals[i][0] < end)
            {
                // Overlap found, remove the current interval
                count++;
            }
            else
            {
                // No overlap, update end time
                end = intervals[i][1];
            }
        }

        return count;
    }

    /**
     * @brief Huffman Coding (simplified, returns encoded lengths)
     * @param freqs Map of character to frequency
     * @return Map of character to code length
     *
     * Time Complexity: O(n log n)
     * Space Complexity: O(n)
     */
    unordered_map<char, int> huffmanCoding(const unordered_map<char, int> &freqs)
    {
        // Using a priority queue for the min heap
        priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;

        // Insert all characters with their frequencies
        for (const auto &pair : freqs)
        {
            pq.push({pair.second, pair.first});
        }

        // Build Huffman tree - simplified to just track depths
        unordered_map<char, int> depths;

        while (pq.size() > 1)
        {
            auto left = pq.top();
            pq.pop();
            auto right = pq.top();
            pq.pop();

            // A merged node doesn't correspond to a character
            // We just reinsert it with combined frequency
            pq.push({left.first + right.first, '#'});

            // Increase depths of all nodes under these two
            for (auto &pair : depths)
            {
                if (pair.first == left.second || pair.first == right.second)
                {
                    pair.second++;
                }
            }

            // If we haven't seen these characters yet, add them with depth 1
            if (left.second != '#' && depths.find(left.second) == depths.end())
            {
                depths[left.second] = 1;
            }
            if (right.second != '#' && depths.find(right.second) == depths.end())
            {
                depths[right.second] = 1;
            }
        }

        return depths;
    }
}

/**
 * @brief Advanced datastructures and techniques
 */
namespace AdvancedTechniques
{
    /**
     * @brief Trie (Prefix Tree) implementation
     */
    class Trie
    {
    private:
        struct TrieNode
        {
            bool isEndOfWord;
            unordered_map<char, TrieNode *> children;

            TrieNode() : isEndOfWord(false) {}
        };

        TrieNode *root;

    public:
        Trie() : root(new TrieNode()) {}

        ~Trie()
        {
            // Helper function to delete all nodes
            function<void(TrieNode *)> deleteNodes = [&](TrieNode *node)
            {
                for (auto &pair : node->children)
                {
                    deleteNodes(pair.second);
                }
                delete node;
            };

            deleteNodes(root);
        }

        /**
         * @brief Insert a word into the trie
         * @param word Word to insert
         *
         * Time Complexity: O(m) where m is the length of the word
         * Space Complexity: O(m)
         */
        void insert(const string &word)
        {
            TrieNode *node = root;

            for (char c : word)
            {
                if (node->children.find(c) == node->children.end())
                {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }

            node->isEndOfWord = true;
        }

        /**
         * @brief Search for a word in the trie
         * @param word Word to search for
         * @return True if word exists, false otherwise
         *
         * Time Complexity: O(m)
         * Space Complexity: O(1)
         */
        bool search(const string &word)
        {
            TrieNode *node = root;

            for (char c : word)
            {
                if (node->children.find(c) == node->children.end())
                {
                    return false;
                }
                node = node->children[c];
            }

            return node->isEndOfWord;
        }

        /**
         * @brief Check if any word in the trie starts with the given prefix
         * @param prefix Prefix to check
         * @return True if any word starts with prefix, false otherwise
         *
         * Time Complexity: O(m)
         * Space Complexity: O(1)
         */
        bool startsWith(const string &prefix)
        {
            TrieNode *node = root;

            for (char c : prefix)
            {
                if (node->children.find(c) == node->children.end())
                {
                    return false;
                }
                node = node->children[c];
            }

            return true;
        }
    };

    /**
     * @brief Union-Find (Disjoint Set) implementation with path compression and union by rank
     */
    class UnionFind
    {
    private:
        vector<int> parent;
        vector<int> rank;
        int count;

    public:
        /**
         * @brief Initialize Union-Find data structure
         * @param n Number of elements
         *
         * Time Complexity: O(n)
         * Space Complexity: O(n)
         */
        UnionFind(int n) : count(n)
        {
            parent.resize(n);
            rank.resize(n, 0);

            for (int i = 0; i < n; i++)
            {
                parent[i] = i;
            }
        }

        /**
         * @brief Find the root of element x (with path compression)
         * @param x Element to find
         * @return Root of x
         *
         * Time Complexity: O(α(n)) where α is the inverse Ackermann function (nearly constant)
         * Space Complexity: O(1)
         */
        int find(int x)
        {
            if (parent[x] != x)
            {
                parent[x] = find(parent[x]); // Path compression
            }
            return parent[x];
        }

        /**
         * @brief Union two elements (with union by rank)
         * @param x First element
         * @param y Second element
         * @return True if union performed, false if already in same set
         *
         * Time Complexity: O(α(n))
         * Space Complexity: O(1)
         */
        bool unite(int x, int y)
        {
            int rootX = find(x);
            int rootY = find(y);

            if (rootX == rootY)
            {
                return false;
            }

            // Union by rank
            if (rank[rootX] < rank[rootY])
            {
                parent[rootX] = rootY;
            }
            else
            {
                parent[rootY] = rootX;
                if (rank[rootX] == rank[rootY])
                {
                    rank[rootX]++;
                }
            }

            count--;
            return true;
        }

        /**
         * @brief Check if two elements are in the same set
         * @param x First element
         * @param y Second element
         * @return True if in same set, false otherwise
         *
         * Time Complexity: O(α(n))
         * Space Complexity: O(1)
         */
        bool connected(int x, int y)
        {
            return find(x) == find(y);
        }

        /**
         * @brief Get number of disjoint sets
         * @return Number of sets
         *
         * Time Complexity: O(1)
         * Space Complexity: O(1)
         */
        int getCount() const
        {
            return count;
        }
    };

    /**
     * @brief Segment Tree implementation for range sum queries
     */
    class SegmentTree
    {
    private:
        vector<int> tree;
        int n;

        /**
         * @brief Build the segment tree
         * @param arr Input array
         * @param node Current node index
         * @param start Start of range
         * @param end End of range
         *
         * Time Complexity: O(n)
         * Space Complexity: O(log n) for recursion stack
         */
        void build(const vector<int> &arr, int node, int start, int end)
        {
            if (start == end)
            {
                tree[node] = arr[start];
                return;
            }

            int mid = start + (end - start) / 2;
            build(arr, 2 * node + 1, start, mid);
            build(arr, 2 * node + 2, mid + 1, end);

            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }

        /**
         * @brief Update value at index
         * @param node Current node index
         * @param start Start of range
         * @param end End of range
         * @param idx Index to update
         * @param val New value
         *
         * Time Complexity: O(log n)
         * Space Complexity: O(log n) for recursion stack
         */
        void update(int node, int start, int end, int idx, int val)
        {
            if (start == end)
            {
                tree[node] = val;
                return;
            }

            int mid = start + (end - start) / 2;
            if (idx <= mid)
            {
                update(2 * node + 1, start, mid, idx, val);
            }
            else
            {
                update(2 * node + 2, mid + 1, end, idx, val);
            }

            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }

        /**
         * @brief Query sum in range [l, r]
         * @param node Current node index
         * @param start Start of range
         * @param end End of range
         * @param l Left bound of query
         * @param r Right bound of query
         * @return Sum in range [l, r]
         *
         * Time Complexity: O(log n)
         * Space Complexity: O(log n) for recursion stack
         */
        int query(int node, int start, int end, int l, int r)
        {
            // No overlap
            if (start > r || end < l)
            {
                return 0;
            }

            // Complete overlap
            if (l <= start && end <= r)
            {
                return tree[node];
            }

            // Partial overlap
            int mid = start + (end - start) / 2;
            int leftSum = query(2 * node + 1, start, mid, l, r);
            int rightSum = query(2 * node + 2, mid + 1, end, l, r);

            return leftSum + rightSum;
        }

    public:
        /**
         * @brief Initialize segment tree with given array
         * @param arr Input array
         *
         * Time Complexity: O(n)
         * Space Complexity: O(n)
         */
        SegmentTree(const vector<int> &arr)
        {
            n = arr.size();
            // Size of segment tree array is 2*2^ceiling(log2(n)) - 1
            int size = 2 * pow(2, ceil(log2(n))) - 1;
            tree.resize(size, 0);

            if (n > 0)
            {
                build(arr, 0, 0, n - 1);
            }
        }

        /**
         * @brief Update value at index
         * @param idx Index to update
         * @param val New value
         *
         * Time Complexity: O(log n)
         * Space Complexity: O(log n) for recursion stack
         */
        void update(int idx, int val)
        {
            if (idx < 0 || idx >= n)
                return;
            update(0, 0, n - 1, idx, val);
        }

        /**
         * @brief Query sum in range [l, r]
         * @param l Left bound of query
         * @param r Right bound of query
         * @return Sum in range [l, r]
         *
         * Time Complexity: O(log n)
         * Space Complexity: O(log n) for recursion stack
         */
        int query(int l, int r)
        {
            if (l < 0)
                l = 0;
            if (r >= n)
                r = n - 1;
            if (l > r)
                return 0;

            return query(0, 0, n - 1, l, r);
        }
    };
}

/**
 * @brief Utility class for benchmarking and performance measurement
 */
class DSABenchmark
{
public:
    /**
     * @brief Measure execution time of a function
     * @param func Function to benchmark
     * @param name Function name for reporting
     * @param iterations Number of iterations
     */
    template <typename Func>
    static void measure(Func func, const string &name, int iterations = 1)
    {
        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; i++)
        {
            func();
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << "Function: " << name << endl;
        cout << "Total time: " << duration.count() << " microseconds" << endl;
        cout << "Average time per iteration: " << duration.count() / iterations << " microseconds" << endl;
        cout << endl;
    }

    /**
     * @brief Generate random vector of integers
     * @param size Size of vector
     * @param min Minimum value
     * @param max Maximum value
     * @return Random vector
     */
    static vector<int> generateRandomVector(int size, int min = 0, int max = 1000)
    {
        vector<int> result(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(min, max);

        for (int i = 0; i < size; i++)
        {
            result[i] = distrib(gen);
        }

        return result;
    }

    /**
     * @brief Compare performance of different algorithms
     * @param algorithms Vector of pairs (algorithm function, name)
     * @param iterations Number of iterations
     */
    template <typename T, typename... Args>
    static void compareAlgorithms(
        const vector<pair<function<T(Args...)>, string>> &algorithms,
        Args... args,
        int iterations = 1)
    {
        for (const auto &algo : algorithms)
        {
            measure([&]()
                    { algo.first(args...); }, algo.second, iterations);
        }
    }
};

/**
 * @brief Utility class to manage DSA practice and track progress
 */
class DSAPracticeTracker
{
private:
    struct ProblemRecord
    {
        string name;
        string category;
        string difficulty;
        int timeSpent; // in minutes
        bool solved;
        string notes;

        ProblemRecord(
            const string &n,
            const string &cat,
            const string &diff,
            int time,
            bool sol,
            const string &nt) : name(n), category(cat), difficulty(diff), timeSpent(time), solved(sol), notes(nt) {}
    };

    vector<ProblemRecord> records;
    unordered_map<string, int> categoryCount;
    unordered_map<string, int> categorySolved;

public:
    /**
     * @brief Log a problem practice session
     * @param name Problem name
     * @param category Problem category
     * @param difficulty Problem difficulty
     * @param timeSpent Time spent in minutes
     * @param solved Whether problem was solved
     * @param notes Additional notes
     */
    void logProblem(
        const string &name,
        const string &category,
        const string &difficulty,
        int timeSpent,
        bool solved,
        const string &notes = "")
    {
        records.emplace_back(name, category, difficulty, timeSpent, solved, notes);
        categoryCount[category]++;
        if (solved)
        {
            categorySolved[category]++;
        }
    }

    /**
     * @brief Get weakest categories based on solve rate
     * @param limit Number of categories to return
     * @return Vector of pairs (category, solve rate)
     */
    vector<pair<string, double>> getWeakestCategories(int limit = 3)
    {
        vector<pair<string, double>> categoryRates;

        for (const auto &entry : categoryCount)
        {
            const string &category = entry.first;
            int count = entry.second;
            int solved = categorySolved[category];
            double solveRate = count > 0 ? static_cast<double>(solved) / count : 0.0;

            categoryRates.emplace_back(category, solveRate);
        }

        // Sort by solve rate (ascending)
        sort(categoryRates.begin(), categoryRates.end(),
             [](const pair<string, double> &a, const pair<string, double> &b)
             {
                 return a.second < b.second;
             });

        // Return at most 'limit' categories
        if (categoryRates.size() > limit)
        {
            categoryRates.resize(limit);
        }

        return categoryRates;
    }

    /**
     * @brief Get average solve time per difficulty level
     * @return Map of difficulty to average solve time
     */
    unordered_map<string, double> getAverageSolveTime()
    {
        unordered_map<string, int> difficultyCount;
        unordered_map<string, int> difficultyTotalTime;

        for (const auto &record : records)
        {
            if (record.solved)
            {
                difficultyCount[record.difficulty]++;
                difficultyTotalTime[record.difficulty] += record.timeSpent;
            }
        }

        unordered_map<string, double> averageTimes;
        for (const auto &entry : difficultyCount)
        {
            const string &difficulty = entry.first;
            int count = entry.second;
            int totalTime = difficultyTotalTime[difficulty];

            averageTimes[difficulty] = count > 0 ? static_cast<double>(totalTime) / count : 0.0;
        }

        return averageTimes;
    }

    /**
     * @brief Generate a practice recommendation
     * @return Recommendation string
     */
    string generateRecommendation()
    {
        // Get weakest categories
        auto weakCategories = getWeakestCategories();

        if (weakCategories.empty())
        {
            return "No practice data available yet. Start solving problems to get recommendations.";
        }

        string recommendation = "Practice Recommendation:\n";
        recommendation += "Focus on these categories:\n";

        for (const auto &category : weakCategories)
        {
            double solveRate = category.second * 100.0;
            recommendation += "- " + category.first + " (Solve rate: " + to_string(static_cast<int>(solveRate)) + "%)\n";
        }

        // Add difficulty recommendation based on average solve times
        auto avgTimes = getAverageSolveTime();
        if (!avgTimes.empty())
        {
            recommendation += "\nTime management:\n";
            for (const auto &entry : avgTimes)
            {
                recommendation += "- " + entry.first + " problems: avg " + to_string(static_cast<int>(entry.second)) + " minutes\n";
            }
        }

        return recommendation;
    }

    /**
     * @brief Print practice statistics
     */
    void printStats()
    {
        // Count total problems and solved problems
        int total = records.size();
        int solved = 0;
        for (const auto &record : records)
        {
            if (record.solved)
            {
                solved++;
            }
        }

        double solveRate = total > 0 ? static_cast<double>(solved) / total * 100.0 : 0.0;

        cout << "===== DSA Practice Statistics =====" << endl;
        cout << "Total problems attempted: " << total << endl;
        cout << "Problems solved: " << solved << " (" << fixed << setprecision(1) << solveRate << "%)" << endl;
        cout << endl;

        cout << "Category breakdown:" << endl;
        for (const auto &entry : categoryCount)
        {
            const string &category = entry.first;
            int count = entry.second;
            int catSolved = categorySolved[category];
            double catSolveRate = count > 0 ? static_cast<double>(catSolved) / count * 100.0 : 0.0;

            cout << "- " << category << ": " << catSolved << "/" << count
                 << " (" << fixed << setprecision(1) << catSolveRate << "%)" << endl;
        }

        cout << endl
             << generateRecommendation() << endl;
    }
};

/**
 * @brief Main function demonstrating key DSA concepts and tracking system
 */
int main()
{
    cout << "===== DAY 45: JOURNEY COMPLETION & NEXT STEPS =====" << endl;
    cout << "This program demonstrates a comprehensive DSA toolkit and practice management system.\n"
         << endl;

    // Demonstrate algorithm benchmarking
    cout << "===== ALGORITHM BENCHMARKING =====" << endl;
    vector<int> testData = DSABenchmark::generateRandomVector(1000);

    // Compare different sorting algorithms
    vector<pair<function<void(vector<int> &)>, string>> sortingAlgos = {
        {[](vector<int> &v)
         { sort(v.begin(), v.end()); }, "std::sort"},
        {[](vector<int> &v)
         {
             // Bubble sort implementation
             for (int i = 0; i < v.size(); i++)
             {
                 for (int j = 0; j < v.size() - i - 1; j++)
                 {
                     if (v[j] > v[j + 1])
                     {
                         swap(v[j], v[j + 1]);
                     }
                 }
             }
         },
         "Bubble sort"}};

    for (const auto &algo : sortingAlgos)
    {
        vector<int> data = testData; // Make a copy
        DSABenchmark::measure([&]()
                              { algo.first(data); }, algo.second);
    }

    // Demonstrate DSA Practice Tracker
    cout << "===== DSA PRACTICE TRACKER =====" << endl;
    DSAPracticeTracker tracker;

    // Log some sample problems
    tracker.logProblem("Two Sum", "Arrays & Hashing", "Easy", 15, true, "Used hash map for O(n) solution");
    tracker.logProblem("Longest Substring Without Repeating Characters", "Sliding Window", "Medium", 25, true, "Used sliding window technique");
    tracker.logProblem("Merge K Sorted Lists", "Heap", "Hard", 45, false, "Need to review heap operations");
    tracker.logProblem("Valid Parentheses", "Stack", "Easy", 10, true, "Used stack to track opening brackets");
    tracker.logProblem("Number of Islands", "Graphs", "Medium", 30, true, "Used DFS for connected components");
    tracker.logProblem("Word Search II", "Trie", "Hard", 60, false, "Trie + backtracking was complex, need more practice");
    tracker.logProblem("LRU Cache", "Design", "Medium", 40, true, "Used doubly linked list + hash map");
    tracker.logProblem("Maximum Subarray", "Dynamic Programming", "Easy", 15, true, "Used Kadane's algorithm");
    tracker.logProblem("Course Schedule", "Graphs", "Medium", 35, true, "Used topological sort to detect cycles");
    tracker.logProblem("Serialize and Deserialize Binary Tree", "Trees", "Hard", 50, false, "Complex tree serialization logic");

    // Print practice statistics
    tracker.printStats();

    // Output recommendations for sustainable practice
    cout << "\n===== SUSTAINABLE PRACTICE ROUTINE =====" << endl;
    cout << "Recommended practice schedule:\n";
    cout << "- Week 1 post-completion: 5 problems/day (35 problems)\n";
    cout << "- Weeks 2-4: 3 problems/day (63 problems)\n";
    cout << "- Months 2-3: 10 problems/week (80 problems)\n";
    cout << "- Ongoing maintenance: 5 problems/week indefinitely\n";

    cout << "\nFocus distribution:\n";
    cout << "- 70% on weak areas (identified by tracker)\n";
    cout << "- 20% on medium-difficulty problems\n";
    cout << "- 10% on new problem types\n";

    cout << "\n===== JOURNEY COMPLETION =====" << endl;
    cout << "Congratulations on completing the 45-day DSA journey!\n";
    cout << "This toolkit provides reference implementations and a tracking system\n";
    cout << "to help maintain and expand your DSA skills going forward.\n";
    cout << "Remember that consistent practice is key to long-term retention.\n";

    return 0;
}