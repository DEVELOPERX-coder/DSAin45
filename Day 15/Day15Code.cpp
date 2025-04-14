// Day 15: Recursion Fundamentals #DSAin45
// Comprehensive implementations of various recursive algorithms

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <chrono>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <memory>
#include <stack>

// ===== UTILITY FUNCTIONS =====

// Helper function to measure execution time
template <typename Func, typename... Args>
auto measureExecutionTime(Func func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return std::make_pair(result, duration.count());
}

// Overload for void functions
template <typename Func, typename... Args>
auto measureExecutionTimeVoid(Func func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return duration.count();
}

// Print a vector of any type
template <typename T>
void printVector(const std::vector<T> &vec, std::string name = "Vector")
{
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i];
        if (i < vec.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Print a 2D vector of any type
template <typename T>
void print2DVector(const std::vector<std::vector<T>> &vec, std::string name = "2D Vector")
{
    std::cout << name << ":" << std::endl;
    for (const auto &row : vec)
    {
        std::cout << "  [";
        for (size_t i = 0; i < row.size(); ++i)
        {
            std::cout << row[i];
            if (i < row.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

// ===== BASIC RECURSION EXAMPLES =====

// 1. Factorial calculation - recursive version
int factorial(int n)
{
    // Base case
    if (n <= 1)
    {
        return 1;
    }

    // Recursive case
    return n * factorial(n - 1);
}

// 1b. Factorial calculation - iterative version for comparison
int factorialIterative(int n)
{
    int result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// 1c. Factorial with tail recursion
int factorialTail(int n, int acc = 1)
{
    // Base case
    if (n <= 1)
    {
        return acc;
    }

    // Recursive case (tail call)
    return factorialTail(n - 1, n * acc);
}

// 2. Fibonacci calculation - recursive version
int fibonacci(int n)
{
    // Base cases
    if (n <= 0)
        return 0;
    if (n == 1)
        return 1;

    // Recursive case
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 2b. Fibonacci calculation - iterative version for comparison
int fibonacciIterative(int n)
{
    if (n <= 0)
        return 0;
    if (n == 1)
        return 1;

    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++)
    {
        int temp = curr;
        curr = prev + curr;
        prev = temp;
    }
    return curr;
}

// 2c. Fibonacci with memoization
int fibonacciMemoized(int n, std::unordered_map<int, int> &memo)
{
    // Check if already computed
    if (memo.find(n) != memo.end())
    {
        return memo[n];
    }

    // Base cases
    if (n <= 0)
        return 0;
    if (n == 1)
        return 1;

    // Compute and store result
    memo[n] = fibonacciMemoized(n - 1, memo) + fibonacciMemoized(n - 2, memo);
    return memo[n];
}

// Wrapper function for memoized Fibonacci
int fibMemo(int n)
{
    std::unordered_map<int, int> memo;
    return fibonacciMemoized(n, memo);
}

// 3. Sum of array elements - recursive
int sumArray(const std::vector<int> &arr, int index = 0)
{
    // Base case: reached end of array
    if (index >= arr.size())
    {
        return 0;
    }

    // Recursive case: current element + sum of rest
    return arr[index] + sumArray(arr, index + 1);
}

// 3b. Sum of array elements - tail recursive
int sumArrayTail(const std::vector<int> &arr, int index = 0, int acc = 0)
{
    // Base case: reached end of array
    if (index >= arr.size())
    {
        return acc;
    }

    // Recursive case: add current element to accumulator
    return sumArrayTail(arr, index + 1, acc + arr[index]);
}

// 4. Palindrome check - recursive
bool isPalindrome(const std::string &str, int left = 0, int right = -1)
{
    if (right == -1)
        right = str.length() - 1; // Initialize right pointer

    // Base case: empty string or one character
    if (left >= right)
    {
        return true;
    }

    // Check if characters at both ends match
    if (str[left] != str[right])
    {
        return false;
    }

    // Recursive case: check substring
    return isPalindrome(str, left + 1, right - 1);
}

// 5. Greatest Common Divisor (Euclidean algorithm) - recursive
int gcd(int a, int b)
{
    // Base case: when b is 0, a is the GCD
    if (b == 0)
    {
        return a;
    }

    // Recursive case: gcd(a, b) = gcd(b, a % b)
    return gcd(b, a % b);
}

// 6. Power function - recursive
double power(double base, int exponent)
{
    // Handle negative exponent
    if (exponent < 0)
    {
        return 1.0 / power(base, -exponent);
    }

    // Base cases
    if (exponent == 0)
        return 1;
    if (exponent == 1)
        return base;

    // Recursive case: use divide and conquer for efficiency
    if (exponent % 2 == 0)
    {
        // For even exponents: x^n = (x^(n/2))^2
        double half = power(base, exponent / 2);
        return half * half;
    }
    else
    {
        // For odd exponents: x^n = x * x^(n-1)
        return base * power(base, exponent - 1);
    }
}

// ===== RECURSIVE DATA STRUCTURE TRAVERSAL =====

// Binary Tree Node
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Create a sample binary tree
TreeNode *createSampleTree()
{
    /*
        Create a tree:
              1
             / \
            2   3
           / \   \
          4   5   6
    */
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    return root;
}

// Free tree memory
void deleteTree(TreeNode *root)
{
    if (!root)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// 1. In-order traversal (left, root, right)
void inorderTraversal(TreeNode *root, std::vector<int> &result)
{
    if (!root)
        return;

    inorderTraversal(root->left, result);
    result.push_back(root->val);
    inorderTraversal(root->right, result);
}

// 2. Pre-order traversal (root, left, right)
void preorderTraversal(TreeNode *root, std::vector<int> &result)
{
    if (!root)
        return;

    result.push_back(root->val);
    preorderTraversal(root->left, result);
    preorderTraversal(root->right, result);
}

// 3. Post-order traversal (left, right, root)
void postorderTraversal(TreeNode *root, std::vector<int> &result)
{
    if (!root)
        return;

    postorderTraversal(root->left, result);
    postorderTraversal(root->right, result);
    result.push_back(root->val);
}

// Iterative versions for comparison
// 1b. Iterative in-order traversal
std::vector<int> inorderTraversalIterative(TreeNode *root)
{
    std::vector<int> result;
    std::stack<TreeNode *> stack;
    TreeNode *current = root;

    while (current || !stack.empty())
    {
        // Go to leftmost node
        while (current)
        {
            stack.push(current);
            current = current->left;
        }

        // Visit node
        current = stack.top();
        stack.pop();
        result.push_back(current->val);

        // Move to right child
        current = current->right;
    }

    return result;
}

// 2b. Iterative pre-order traversal
std::vector<int> preorderTraversalIterative(TreeNode *root)
{
    if (!root)
        return {};

    std::vector<int> result;
    std::stack<TreeNode *> stack;
    stack.push(root);

    while (!stack.empty())
    {
        TreeNode *current = stack.top();
        stack.pop();

        result.push_back(current->val);

        // Push right first (so left is processed first - LIFO)
        if (current->right)
            stack.push(current->right);
        if (current->left)
            stack.push(current->left);
    }

    return result;
}

// ===== DIVIDE AND CONQUER RECURSION =====

// 1. Merge Sort
void merge(std::vector<int> &arr, int left, int mid, int right)
{
    // Create temporary arrays
    std::vector<int> leftArray(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<int> rightArray(arr.begin() + mid + 1, arr.begin() + right + 1);

    // Initial indexes for left, right, and merged arrays
    int leftIdx = 0, rightIdx = 0, mergedIdx = left;

    // Merge the two arrays back into arr
    while (leftIdx < leftArray.size() && rightIdx < rightArray.size())
    {
        if (leftArray[leftIdx] <= rightArray[rightIdx])
        {
            arr[mergedIdx++] = leftArray[leftIdx++];
        }
        else
        {
            arr[mergedIdx++] = rightArray[rightIdx++];
        }
    }

    // Copy remaining elements
    while (leftIdx < leftArray.size())
    {
        arr[mergedIdx++] = leftArray[leftIdx++];
    }

    while (rightIdx < rightArray.size())
    {
        arr[mergedIdx++] = rightArray[rightIdx++];
    }
}

void mergeSort(std::vector<int> &arr, int left, int right)
{
    // Base case: array of size 0 or 1
    if (left >= right)
        return;

    // Find the middle point
    int mid = left + (right - left) / 2;

    // Recursively sort first and second halves
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge the sorted halves
    merge(arr, left, mid, right);
}

// Wrapper for mergeSort
void mergeSort(std::vector<int> &arr)
{
    if (arr.size() <= 1)
        return;
    mergeSort(arr, 0, arr.size() - 1);
}

// 2. Quick Sort
int partition(std::vector<int> &arr, int low, int high)
{
    // Choose the rightmost element as pivot
    int pivot = arr[high];

    // Index of smaller element
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    // Place pivot in its correct position
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int> &arr, int low, int high)
{
    // Base case
    if (low >= high)
        return;

    // Partition the array and get pivot position
    int pivotPos = partition(arr, low, high);

    // Recursively sort elements before and after partition
    quickSort(arr, low, pivotPos - 1);
    quickSort(arr, pivotPos + 1, high);
}

// Wrapper for quickSort
void quickSort(std::vector<int> &arr)
{
    if (arr.size() <= 1)
        return;
    quickSort(arr, 0, arr.size() - 1);
}

// 3. Binary Search - recursive
int binarySearch(const std::vector<int> &arr, int target, int left, int right)
{
    // Base case: element not found
    if (left > right)
        return -1;

    // Find the middle index
    int mid = left + (right - left) / 2;

    // Check if target is at the middle
    if (arr[mid] == target)
        return mid;

    // If target is smaller, search in left subarray
    if (arr[mid] > target)
    {
        return binarySearch(arr, target, left, mid - 1);
    }

    // If target is larger, search in right subarray
    return binarySearch(arr, target, mid + 1, right);
}

// Wrapper for binarySearch
int binarySearch(const std::vector<int> &arr, int target)
{
    return binarySearch(arr, target, 0, arr.size() - 1);
}

// Iterative version for comparison
int binarySearchIterative(const std::vector<int> &arr, int target)
{
    int left = 0, right = arr.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
            return mid;

        if (arr[mid] > target)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    return -1; // Element not found
}

// ===== BACKTRACKING RECURSION =====

// 1. N-Queens Problem
bool isSafe(std::vector<std::vector<char>> &board, int row, int col, int n)
{
    // Check row to the left
    for (int i = 0; i < col; i++)
    {
        if (board[row][i] == 'Q')
            return false;
    }

    // Check upper diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == 'Q')
            return false;
    }

    // Check lower diagonal
    for (int i = row, j = col; i < n && j >= 0; i++, j--)
    {
        if (board[i][j] == 'Q')
            return false;
    }

    return true;
}

bool solveNQueens(std::vector<std::vector<char>> &board, int col, int n)
{
    // Base case: all queens placed
    if (col >= n)
        return true;

    // Try placing a queen in each row of the current column
    for (int row = 0; row < n; row++)
    {
        if (isSafe(board, row, col, n))
        {
            // Place queen
            board[row][col] = 'Q';

            // Recursively try to place queens in remaining columns
            if (solveNQueens(board, col + 1, n))
                return true;

            // If placing queen here didn't work, remove it (backtrack)
            board[row][col] = '.';
        }
    }

    // Couldn't place a queen in any row of this column
    return false;
}

// Wrapper for N-Queens
std::vector<std::vector<char>> solveNQueens(int n)
{
    std::vector<std::vector<char>> board(n, std::vector<char>(n, '.'));

    if (solveNQueens(board, 0, n))
    {
        return board;
    }

    // No solution found
    return {};
}

// Print a chess board
void printBoard(const std::vector<std::vector<char>> &board)
{
    std::cout << "Board:" << std::endl;
    for (const auto &row : board)
    {
        for (char cell : row)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

// 2. Generating all permutations of an array
void permute(std::vector<int> &nums, int start, std::vector<std::vector<int>> &result)
{
    // Base case: reached the end of array
    if (start == nums.size() - 1)
    {
        result.push_back(nums);
        return;
    }

    // Try each element at the current position
    for (int i = start; i < nums.size(); i++)
    {
        // Swap to put nums[i] at position start
        std::swap(nums[start], nums[i]);

        // Recursively generate permutations for the rest of the array
        permute(nums, start + 1, result);

        // Backtrack - restore array to original state
        std::swap(nums[start], nums[i]);
    }
}

// Wrapper for permutations
std::vector<std::vector<int>> permutations(std::vector<int> nums)
{
    std::vector<std::vector<int>> result;
    permute(nums, 0, result);
    return result;
}

// 3. Generate all subsets of an array
void generateSubsets(std::vector<int> &nums, int index, std::vector<int> &current, std::vector<std::vector<int>> &result)
{
    // Base case: reached the end of nums
    if (index == nums.size())
    {
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

// Wrapper for subsets
std::vector<std::vector<int>> subsets(std::vector<int> nums)
{
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    generateSubsets(nums, 0, current, result);
    return result;
}

// ===== CLASSIC RECURSION PROBLEMS =====

// 1. Tower of Hanoi
void towerOfHanoi(int n, char source, char auxiliary, char target, std::vector<std::string> &moves)
{
    // Base case: no disk to move
    if (n == 0)
        return;

    // Move n-1 disks from source to auxiliary (using target as auxiliary)
    towerOfHanoi(n - 1, source, target, auxiliary, moves);

    // Move the largest disk from source to target
    moves.push_back("Move disk " + std::to_string(n) + " from " + source + " to " + target);

    // Move n-1 disks from auxiliary to target (using source as auxiliary)
    towerOfHanoi(n - 1, auxiliary, source, target, moves);
}

// Wrapper for Tower of Hanoi
std::vector<std::string> towerOfHanoi(int n)
{
    std::vector<std::string> moves;
    towerOfHanoi(n, 'A', 'B', 'C', moves);
    return moves;
}

// 2. Flood Fill Algorithm
void floodFill(std::vector<std::vector<int>> &image, int sr, int sc, int newColor, int oldColor)
{
    // Base cases: out of bounds or different color
    if (sr < 0 || sr >= image.size() || sc < 0 || sc >= image[0].size() || image[sr][sc] != oldColor)
    {
        return;
    }

    // Fill current pixel
    image[sr][sc] = newColor;

    // Recursively fill adjacent pixels
    floodFill(image, sr + 1, sc, newColor, oldColor); // Down
    floodFill(image, sr - 1, sc, newColor, oldColor); // Up
    floodFill(image, sr, sc + 1, newColor, oldColor); // Right
    floodFill(image, sr, sc - 1, newColor, oldColor); // Left
}

// Wrapper for Flood Fill
std::vector<std::vector<int>> floodFill(std::vector<std::vector<int>> image, int sr, int sc, int newColor)
{
    if (image[sr][sc] != newColor)
    {
        floodFill(image, sr, sc, newColor, image[sr][sc]);
    }
    return image;
}

// 3. Recursive String Reversal
std::string reverseString(const std::string &str)
{
    // Base case: empty string or single character
    if (str.length() <= 1)
    {
        return str;
    }

    // Recursive case: reverse substring and append first character
    return reverseString(str.substr(1)) + str[0];
}

// More efficient string reversal with pointers
void reverseStringInPlace(std::string &str, int left = 0, int right = -1)
{
    if (right == -1)
        right = str.length() - 1;

    // Base case: pointers meet or cross
    if (left >= right)
        return;

    // Swap characters and recurse
    std::swap(str[left], str[right]);
    reverseStringInPlace(str, left + 1, right - 1);
}

// ===== COMPLEX RECURSION APPLICATIONS =====

// 1. Regular Expression Matching (simplified)
bool isMatch(const std::string &s, const std::string &p, int i = 0, int j = 0)
{
    // Base case: end of pattern
    if (j == p.length())
        return i == s.length();

    // Check if current characters match
    bool currentMatch = i < s.length() && (p[j] == '.' || p[j] == s[i]);

    // If next character is '*'
    if (j + 1 < p.length() && p[j + 1] == '*')
    {
        // Either ignore current pattern char and '*' (zero matches)
        // Or use the current pattern char for current string char and try again (if they match)
        return isMatch(s, p, i, j + 2) || (currentMatch && isMatch(s, p, i + 1, j));
    }

    // No '*', just check if current chars match and proceed
    return currentMatch && isMatch(s, p, i + 1, j + 1);
}

// Add memoization to regex matching for efficiency
bool isMatchMemoized(const std::string &s, const std::string &p,
                     int i = 0, int j = 0,
                     std::unordered_map<std::string, bool> &memo)
{
    // Generate a key for memoization
    std::string key = std::to_string(i) + "," + std::to_string(j);

    // Check if already computed
    if (memo.find(key) != memo.end())
    {
        return memo[key];
    }

    bool result;

    // Base case: end of pattern
    if (j == p.length())
    {
        result = (i == s.length());
    }
    else
    {
        // Check if current characters match
        bool currentMatch = i < s.length() && (p[j] == '.' || p[j] == s[i]);

        // If next character is '*'
        if (j + 1 < p.length() && p[j + 1] == '*')
        {
            result = isMatchMemoized(s, p, i, j + 2, memo) ||
                     (currentMatch && isMatchMemoized(s, p, i + 1, j, memo));
        }
        else
        {
            // No '*', just check if current chars match and proceed
            result = currentMatch && isMatchMemoized(s, p, i + 1, j + 1, memo);
        }
    }

    // Memoize result
    memo[key] = result;
    return result;
}

// Wrapper for memoized regex matching
bool isMatchMemoized(const std::string &s, const std::string &p)
{
    std::unordered_map<std::string, bool> memo;
    return isMatchMemoized(s, p, 0, 0, memo);
}

// 2. Solve Sudoku
bool isValidSudoku(std::vector<std::vector<int>> &board, int row, int col, int num)
{
    // Check row
    for (int j = 0; j < 9; j++)
    {
        if (board[row][j] == num)
            return false;
    }

    // Check column
    for (int i = 0; i < 9; i++)
    {
        if (board[i][col] == num)
            return false;
    }

    // Check 3x3 box
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int i = boxRow; i < boxRow + 3; i++)
    {
        for (int j = boxCol; j < boxCol + 3; j++)
        {
            if (board[i][j] == num)
                return false;
        }
    }

    return true;
}

bool solveSudoku(std::vector<std::vector<int>> &board)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            // Find an empty cell
            if (board[row][col] == 0)
            {
                // Try placing digits 1-9
                for (int num = 1; num <= 9; num++)
                {
                    if (isValidSudoku(board, row, col, num))
                    {
                        // Place the digit
                        board[row][col] = num;

                        // Recursively solve the rest of the puzzle
                        if (solveSudoku(board))
                        {
                            return true;
                        }

                        // If placing this digit doesn't lead to a solution, backtrack
                        board[row][col] = 0;
                    }
                }

                // No valid digit found for this cell
                return false;
            }
        }
    }

    // All cells filled
    return true;
}

// Print a Sudoku board
void printSudoku(const std::vector<std::vector<int>> &board)
{
    std::cout << "Sudoku Solution:" << std::endl;
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0 && i != 0)
        {
            std::cout << "- - - + - - - + - - -" << std::endl;
        }

        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0 && j != 0)
            {
                std::cout << "| ";
            }

            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// ===== TESTING AND PERFORMANCE COMPARISON =====

// Test basic recursive functions against their iterative counterparts
void testBasicRecursion()
{
    std::cout << "\n===== BASIC RECURSION TESTS =====" << std::endl;

    // 1. Factorial
    std::cout << "\nFactorial:" << std::endl;
    int n = 10;

    auto [resultRec, timeRec] = measureExecutionTime(factorial, n);
    auto [resultIter, timeIter] = measureExecutionTime(factorialIterative, n);
    auto [resultTail, timeTail] = measureExecutionTime(factorialTail, n);

    std::cout << "n = " << n << std::endl;
    std::cout << "  Recursive:    " << resultRec << " (in " << timeRec << " μs)" << std::endl;
    std::cout << "  Tail recursi: " << resultTail << " (in " << timeTail << " μs)" << std::endl;
    std::cout << "  Iterative:    " << resultIter << " (in " << timeIter << " μs)" << std::endl;

    // 2. Fibonacci
    std::cout << "\nFibonacci:" << std::endl;
    n = 30;

    auto [resultFibIter, timeFibIter] = measureExecutionTime(fibonacciIterative, n);
    auto [resultFibMemo, timeFibMemo] = measureExecutionTime(fibMemo, n);

    std::cout << "n = " << n << std::endl;
    std::cout << "  Memoized:     " << resultFibMemo << " (in " << timeFibMemo << " μs)" << std::endl;
    std::cout << "  Iterative:    " << resultFibIter << " (in " << timeFibIter << " μs)" << std::endl;

    // Don't run naive recursion for large n as it's exponentially slow
    n = 15;
    auto [resultFibRec, timeFibRec] = measureExecutionTime(fibonacci, n);
    std::cout << "n = " << n << " (smaller due to exponential complexity)" << std::endl;
    std::cout << "  Naive recurs: " << resultFibRec << " (in " << timeFibRec << " μs)" << std::endl;

    // 3. Sum of Array
    std::cout << "\nSum of Array:" << std::endl;
    std::vector<int> arr(1000, 1); // Array of 1000 ones

    auto [resultSumRec, timeSumRec] = measureExecutionTime(sumArray, arr);
    auto [resultSumTail, timeSumTail] = measureExecutionTime(sumArrayTail, arr);

    // Iterative version for comparison
    auto iterSum = [](const std::vector<int> &arr)
    {
        int sum = 0;
        for (int num : arr)
            sum += num;
        return sum;
    };

    auto [resultSumIter, timeSumIter] = measureExecutionTime(iterSum, arr);

    std::cout << "Array size: " << arr.size() << std::endl;
    std::cout << "  Recursive:    " << resultSumRec << " (in " << timeSumRec << " μs)" << std::endl;
    std::cout << "  Tail recursi: " << resultSumTail << " (in " << timeSumTail << " μs)" << std::endl;
    std::cout << "  Iterative:    " << resultSumIter << " (in " << timeSumIter << " μs)" << std::endl;

    // 4. Palindrome
    std::cout << "\nPalindrome Check:" << std::endl;

    std::vector<std::string> testStrings = {
        "racecar",
        "hello",
        "A man a plan a canal Panama",
        std::string(1000, 'a') // Long palindrome
    };

    for (const auto &str : testStrings)
    {
        std::string normalized = str;
        // Remove spaces and make lowercase
        normalized.erase(std::remove_if(normalized.begin(), normalized.end(),
                                        [](char c)
                                        { return std::isspace(c); }),
                         normalized.end());
        std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                       [](char c)
                       { return std::tolower(c); });

        auto [resultPal, timePal] = measureExecutionTime(isPalindrome, normalized);

        std::cout << "String: \"" << (str.length() > 20 ? str.substr(0, 17) + "..." : str) << "\" ("
                  << str.length() << " chars)" << std::endl;
        std::cout << "  Is palindrome: " << (resultPal ? "Yes" : "No")
                  << " (in " << timePal << " μs)" << std::endl;
    }

    // 5. Power Function
    std::cout << "\nPower Function:" << std::endl;

    auto [resultPow1, timePow1] = measureExecutionTime(power, 2.0, 10);
    auto [resultPow2, timePow2] = measureExecutionTime(power, 1.5, 20);
    auto [resultPow3, timePow3] = measureExecutionTime(power, 0.5, -5);

    std::cout << "  2^10 = " << resultPow1 << " (in " << timePow1 << " μs)" << std::endl;
    std::cout << "  1.5^20 = " << resultPow2 << " (in " << timePow2 << " μs)" << std::endl;
    std::cout << "  0.5^(-5) = " << resultPow3 << " (in " << timePow3 << " μs)" << std::endl;
}

// Test recursive tree traversal
void testTreeTraversal()
{
    std::cout << "\n===== TREE TRAVERSAL TESTS =====" << std::endl;

    // Create a sample tree
    TreeNode *root = createSampleTree();

    // 1. In-order traversal
    std::vector<int> inorderResult;
    long long inorderTime = measureExecutionTimeVoid([&]()
                                                     { inorderTraversal(root, inorderResult); });

    std::cout << "In-order traversal:" << std::endl;
    printVector(inorderResult, "  Result");
    std::cout << "  Time: " << inorderTime << " μs" << std::endl;

    // 2. Pre-order traversal
    std::vector<int> preorderResult;
    long long preorderTime = measureExecutionTimeVoid([&]()
                                                      { preorderTraversal(root, preorderResult); });

    std::cout << "Pre-order traversal:" << std::endl;
    printVector(preorderResult, "  Result");
    std::cout << "  Time: " << preorderTime << " μs" << std::endl;

    // 3. Post-order traversal
    std::vector<int> postorderResult;
    long long postorderTime = measureExecutionTimeVoid([&]()
                                                       { postorderTraversal(root, postorderResult); });

    std::cout << "Post-order traversal:" << std::endl;
    printVector(postorderResult, "  Result");
    std::cout << "  Time: " << postorderTime << " μs" << std::endl;

    // 4. Iterative vs Recursive traversal
    std::cout << "\nRecursive vs Iterative In-order Traversal:" << std::endl;

    inorderResult.clear(); // Clear previous results

    // Recursive
    auto [_, recursiveTime] = measureExecutionTime([&]()
                                                   {
        std::vector<int> result;
        inorderTraversal(root, result);
        return result; });

    // Iterative
    auto [iterResult, iterativeTime] = measureExecutionTime(inorderTraversalIterative, root);

    std::cout << "  Recursive: " << recursiveTime << " μs" << std::endl;
    std::cout << "  Iterative: " << iterativeTime << " μs" << std::endl;

    // Clean up tree
    deleteTree(root);
}

// Test divide and conquer algorithms
void testDivideAndConquer()
{
    std::cout << "\n===== DIVIDE AND CONQUER TESTS =====" << std::endl;

    // Create a random array
    std::vector<int> arr(1000);
    for (int i = 0; i < arr.size(); i++)
    {
        arr[i] = rand() % 10000;
    }

    // 1. Merge Sort
    std::cout << "\nMerge Sort:" << std::endl;

    std::vector<int> arrCopy = arr;
    long long mergeSortTime = measureExecutionTimeVoid([&]()
                                                       { mergeSort(arrCopy); });

    std::cout << "  Time to sort " << arr.size() << " elements: " << mergeSortTime << " μs" << std::endl;
    std::cout << "  Sorted correctly: " << (std::is_sorted(arrCopy.begin(), arrCopy.end()) ? "Yes" : "No") << std::endl;

    // 2. Quick Sort
    std::cout << "\nQuick Sort:" << std::endl;

    arrCopy = arr;
    long long quickSortTime = measureExecutionTimeVoid([&]()
                                                       { quickSort(arrCopy); });

    std::cout << "  Time to sort " << arr.size() << " elements: " << quickSortTime << " μs" << std::endl;
    std::cout << "  Sorted correctly: " << (std::is_sorted(arrCopy.begin(), arrCopy.end()) ? "Yes" : "No") << std::endl;

    // 3. Binary Search
    std::cout << "\nBinary Search:" << std::endl;

    // Use the sorted array from merge sort
    std::sort(arrCopy.begin(), arrCopy.end());

    // Select a few elements to search for
    std::vector<int> searchElements = {
        arrCopy[arrCopy.size() / 4],     // Element that exists (25%)
        arrCopy[arrCopy.size() / 2],     // Element that exists (50%)
        arrCopy[3 * arrCopy.size() / 4], // Element that exists (75%)
        -1,                              // Element that doesn't exist
        10001                            // Element that doesn't exist
    };

    for (int target : searchElements)
    {
        // Recursive binary search
        auto [resultRec, timeRec] = measureExecutionTime(binarySearch, arrCopy, target);

        // Iterative binary search
        auto [resultIter, timeIter] = measureExecutionTime(binarySearchIterative, arrCopy, target);

        std::cout << "  Searching for " << target << ":" << std::endl;
        std::cout << "    Recursive: " << (resultRec != -1 ? "Found at index " + std::to_string(resultRec) : "Not found")
                  << " (in " << timeRec << " μs)" << std::endl;
        std::cout << "    Iterative: " << (resultIter != -1 ? "Found at index " + std::to_string(resultIter) : "Not found")
                  << " (in " << timeIter << " μs)" << std::endl;
    }
}

// Test backtracking algorithms
void testBacktracking()
{
    std::cout << "\n===== BACKTRACKING TESTS =====" << std::endl;

    // 1. N-Queens
    std::cout << "\nN-Queens Problem:" << std::endl;

    for (int n = 4; n <= 8; n++)
    {
        auto [solution, time] = measureExecutionTime(solveNQueens, n);

        std::cout << "  " << n << "-Queens: " << (solution.empty() ? "No solution" : "Solution found")
                  << " (in " << time << " μs)" << std::endl;

        if (!solution.empty() && n <= 5)
        { // Only print small boards
            printBoard(solution);
        }
    }

    // 2. Permutations
    std::cout << "\nPermutations:" << std::endl;

    std::vector<int> nums = {1, 2, 3, 4};
    auto [perms, permsTime] = measureExecutionTime(permutations, nums);

    std::cout << "  Permutations of {1, 2, 3, 4}: " << perms.size() << " permutations found (in "
              << permsTime << " μs)" << std::endl;

    if (perms.size() <= 24)
    { // Only print if not too many
        print2DVector(perms, "  All permutations");
    }

    // 3. Subsets
    std::cout << "\nSubsets:" << std::endl;

    auto [subsetResult, subsetTime] = measureExecutionTime(subsets, nums);

    std::cout << "  Subsets of {1, 2, 3, 4}: " << subsetResult.size() << " subsets found (in "
              << subsetTime << " μs)" << std::endl;

    if (subsetResult.size() <= 16)
    { // Only print if not too many
        print2DVector(subsetResult, "  All subsets");
    }
}

// Test classic recursion problems
void testClassicProblems()
{
    std::cout << "\n===== CLASSIC RECURSION PROBLEMS =====" << std::endl;

    // 1. Tower of Hanoi
    std::cout << "\nTower of Hanoi:" << std::endl;

    for (int n = 1; n <= 5; n++)
    {
        auto [moves, time] = measureExecutionTime(towerOfHanoi, n);

        std::cout << "  " << n << " disks: " << moves.size() << " moves (in " << time << " μs)" << std::endl;

        if (n <= 3)
        { // Only print moves for small n
            for (const auto &move : moves)
            {
                std::cout << "    " << move << std::endl;
            }
        }
    }

    // 2. Flood Fill
    std::cout << "\nFlood Fill:" << std::endl;

    std::vector<std::vector<int>> image = {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}};

    std::cout << "  Original image:" << std::endl;
    for (const auto &row : image)
    {
        std::cout << "    ";
        for (int pixel : row)
        {
            std::cout << pixel << " ";
        }
        std::cout << std::endl;
    }

    auto [filledImage, fillTime] = measureExecutionTime(floodFill, image, 2, 2, 2);

    std::cout << "  After flood fill (in " << fillTime << " μs):" << std::endl;
    for (const auto &row : filledImage)
    {
        std::cout << "    ";
        for (int pixel : row)
        {
            std::cout << pixel << " ";
        }
        std::cout << std::endl;
    }

    // 3. String Reversal
    std::cout << "\nString Reversal:" << std::endl;

    std::vector<std::string> testStrings = {
        "hello",
        "recursion",
        "abcdefghijklmnopqrstuvwxyz"};

    for (auto str : testStrings)
    {
        auto [reversed1, time1] = measureExecutionTime(reverseString, str);

        std::string strCopy = str;
        long long time2 = measureExecutionTimeVoid([&]()
                                                   { reverseStringInPlace(strCopy); });

        std::cout << "  Original: \"" << str << "\"" << std::endl;
        std::cout << "    Reversed (concat):  \"" << reversed1 << "\" (in " << time1 << " μs)" << std::endl;
        std::cout << "    Reversed (in-place): \"" << strCopy << "\" (in " << time2 << " μs)" << std::endl;
    }
}

// Test complex recursion applications
void testComplexApplications()
{
    std::cout << "\n===== COMPLEX RECURSION APPLICATIONS =====" << std::endl;

    // 1. Regular Expression Matching
    std::cout << "\nRegular Expression Matching:" << std::endl;

    std::vector<std::pair<std::string, std::string>> regexTests = {
        {"aa", "a"},                  // false
        {"aa", "a*"},                 // true
        {"ab", ".*"},                 // true
        {"aab", "c*a*b"},             // true
        {"mississippi", "mis*is*p*."} // false
    };

    for (const auto &[s, p] : regexTests)
    {
        auto [result1, time1] = measureExecutionTime(isMatch, s, p);
        auto [result2, time2] = measureExecutionTime(isMatchMemoized, s, p);

        std::cout << "  String: \"" << s << "\", Pattern: \"" << p << "\"" << std::endl;
        std::cout << "    Naive:    " << (result1 ? "Match" : "No match") << " (in " << time1 << " μs)" << std::endl;
        std::cout << "    Memoized: " << (result2 ? "Match" : "No match") << " (in " << time2 << " μs)" << std::endl;
    }

    // 2. Sudoku Solver
    std::cout << "\nSudoku Solver:" << std::endl;

    std::vector<std::vector<int>> sudokuBoard = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}};

    std::cout << "  Original board:" << std::endl;
    printSudoku(sudokuBoard);

    auto sudokuCopy = sudokuBoard;
    long long solveTime = measureExecutionTimeVoid([&]()
                                                   { solveSudoku(sudokuCopy); });

    std::cout << "  Solved in " << solveTime << " μs:" << std::endl;
    printSudoku(sudokuCopy);
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "====================================================" << std::endl;
    std::cout << "DAY 15: RECURSION FUNDAMENTALS #DSAin45" << std::endl;
    std::cout << "====================================================" << std::endl;

    // Test basic recursive functions
    testBasicRecursion();

    // Test tree traversal
    testTreeTraversal();

    // Test divide and conquer algorithms
    testDivideAndConquer();

    // Test backtracking algorithms
    testBacktracking();

    // Test classic recursion problems
    testClassicProblems();

    // Test complex recursion applications
    testComplexApplications();

    return 0;
}