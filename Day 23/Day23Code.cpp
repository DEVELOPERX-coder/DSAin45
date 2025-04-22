#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <functional>
#include <iomanip>
#include <random>
#include <chrono>
#include <cmath>
#include <string>
#include <sstream>

/**
 * @file BinarySearchTree.cpp
 * @brief Comprehensive implementation of Binary Search Trees (BST) in C++
 *
 * This file contains:
 * 1. Basic BST structure and core operations (insertion, deletion, search)
 * 2. Multiple traversal methods (in-order, pre-order, post-order)
 * 3. Advanced BST operations (successor, predecessor, range queries)
 * 4. BST utilities (validation, balancing, conversion)
 * 5. BST visualizations and statistics
 * 6. Performance benchmarks and comparisons
 *
 * @author #DSAin45 Series
 * @date Day 23
 */

// ============================================================================
// BST Node Structure
// ============================================================================

/**
 * Definition for a binary search tree node
 */
struct TreeNode
{
    int val;         // The value stored in the node
    TreeNode *left;  // Pointer to the left child
    TreeNode *right; // Pointer to the right child

    // Constructor
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

    // Constructor with left and right child pointers
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// ============================================================================
// Binary Search Tree Class
// ============================================================================

class BinarySearchTree
{
private:
    TreeNode *root;

    // ---- Private Helper Methods ----

    // Core operations helpers
    TreeNode *insertRecursive(TreeNode *root, int val);
    TreeNode *deleteRecursive(TreeNode *root, int val);
    bool searchRecursive(TreeNode *root, int val) const;

    // Traversal helpers
    void inorderTraversalRecursive(TreeNode *root, std::vector<int> &result) const;
    void preorderTraversalRecursive(TreeNode *root, std::vector<int> &result) const;
    void postorderTraversalRecursive(TreeNode *root, std::vector<int> &result) const;

    // Utility helpers
    TreeNode *findMin(TreeNode *root) const;
    TreeNode *findMax(TreeNode *root) const;
    int getHeight(TreeNode *root) const;
    int getSize(TreeNode *root) const;
    void deleteTree(TreeNode *root);
    bool isValidBSTHelper(TreeNode *root, long minVal, long maxVal) const;
    void rangeSearchHelper(TreeNode *root, int low, int high, std::vector<int> &result) const;
    void collectInorder(TreeNode *root, std::vector<int> &elements) const;
    TreeNode *buildBalancedBST(const std::vector<int> &elements, int start, int end);

    // Visualizations
    void printNodeInternal(TreeNode *root, std::string prefix, bool isLeft) const;

public:
    // ---- Constructors & Destructor ----

    /**
     * Default constructor
     */
    BinarySearchTree() : root(nullptr) {}

    /**
     * Constructor from a sorted array
     *
     * @param sortedArray A sorted array to build a balanced BST
     */
    BinarySearchTree(const std::vector<int> &sortedArray);

    /**
     * Destructor to clean up allocated memory
     */
    ~BinarySearchTree();

    // ---- Core BST Operations ----

    /**
     * Insert a value into the BST
     *
     * @param val The value to insert
     */
    void insert(int val);

    /**
     * Insert a value using iterative approach
     *
     * @param val The value to insert
     */
    void insertIterative(int val);

    /**
     * Remove a value from the BST
     *
     * @param val The value to remove
     */
    void remove(int val);

    /**
     * Search for a value in the BST
     *
     * @param val The value to search for
     * @return True if the value is found, false otherwise
     */
    bool search(int val) const;

    /**
     * Search for a value using iterative approach
     *
     * @param val The value to search for
     * @return True if the value is found, false otherwise
     */
    bool searchIterative(int val) const;

    // ---- Tree Traversal Methods ----

    /**
     * Get in-order traversal of the BST
     *
     * @return Vector containing values in in-order (sorted order)
     */
    std::vector<int> inorderTraversal() const;

    /**
     * Get in-order traversal using iterative approach
     *
     * @return Vector containing values in in-order (sorted order)
     */
    std::vector<int> inorderTraversalIterative() const;

    /**
     * Get pre-order traversal of the BST
     *
     * @return Vector containing values in pre-order
     */
    std::vector<int> preorderTraversal() const;

    /**
     * Get pre-order traversal using iterative approach
     *
     * @return Vector containing values in pre-order
     */
    std::vector<int> preorderTraversalIterative() const;

    /**
     * Get post-order traversal of the BST
     *
     * @return Vector containing values in post-order
     */
    std::vector<int> postorderTraversal() const;

    /**
     * Get post-order traversal using iterative approach
     *
     * @return Vector containing values in post-order
     */
    std::vector<int> postorderTraversalIterative() const;

    /**
     * Get level-order traversal of the BST
     *
     * @return Vector containing values in level-order
     */
    std::vector<int> levelOrderTraversal() const;

    /**
     * Get level-order traversal with levels separated
     *
     * @return Vector of vectors representing each level of the BST
     */
    std::vector<std::vector<int>> levelOrderTraversalByLevel() const;

    // ---- Advanced BST Operations ----

    /**
     * Find the minimum value in the BST
     *
     * @return The minimum value, or throws if tree is empty
     */
    int findMinimum() const;

    /**
     * Find the maximum value in the BST
     *
     * @return The maximum value, or throws if tree is empty
     */
    int findMaximum() const;

    /**
     * Find the successor of a given value (next largest value)
     *
     * @param val The value to find the successor of
     * @return The successor value, or -1 if not found or no successor
     */
    int successor(int val) const;

    /**
     * Find the predecessor of a given value (next smallest value)
     *
     * @param val The value to find the predecessor of
     * @return The predecessor value, or -1 if not found or no predecessor
     */
    int predecessor(int val) const;

    /**
     * Find the k-th smallest element in the BST (1-indexed)
     *
     * @param k The index of the element to find
     * @return The k-th smallest element
     */
    int kthSmallest(int k) const;

    /**
     * Find all elements within a given range [low, high]
     *
     * @param low The lower bound (inclusive)
     * @param high The upper bound (inclusive)
     * @return Vector of values within the range
     */
    std::vector<int> rangeSearch(int low, int high) const;

    /**
     * Find the closest value to a given target
     *
     * @param target The target value
     * @return The value in the BST closest to the target
     */
    int closestValue(int target) const;

    // ---- BST Utilities ----

    /**
     * Get the height of the BST
     *
     * @return The height (-1 for empty tree)
     */
    int height() const;

    /**
     * Get the number of nodes in the BST
     *
     * @return The number of nodes
     */
    int size() const;

    /**
     * Check if the BST is empty
     *
     * @return True if empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * Check if the tree is a valid BST
     *
     * @return True if the tree is a valid BST, false otherwise
     */
    bool isValidBST() const;

    /**
     * Balance an unbalanced BST
     *
     * @return Reference to this BST (for chaining)
     */
    BinarySearchTree &balance();

    /**
     * Clear the BST (remove all nodes)
     */
    void clear();

    /**
     * Get the root node of the BST
     *
     * @return Pointer to the root node
     */
    TreeNode *getRoot() const;

    // ---- Visualization Methods ----

    /**
     * Print the BST structure
     */
    void printTree() const;

    /**
     * Print the BST statistics
     */
    void printStats() const;
};

// ============================================================================
// Constructor and Destructor Implementations
// ============================================================================

/**
 * Constructor from a sorted array
 * Builds a balanced BST from a sorted array
 */
BinarySearchTree::BinarySearchTree(const std::vector<int> &sortedArray)
{
    if (sortedArray.empty())
    {
        root = nullptr;
        return;
    }

    // Build a balanced BST from the sorted array
    root = buildBalancedBST(sortedArray, 0, sortedArray.size() - 1);
}

/**
 * Destructor
 * Cleans up the entire tree to prevent memory leaks
 */
BinarySearchTree::~BinarySearchTree()
{
    clear();
}

// ============================================================================
// Core BST Operations
// ============================================================================

/**
 * Insert a value into the BST (recursive approach)
 */
void BinarySearchTree::insert(int val)
{
    root = insertRecursive(root, val);
}

/**
 * Helper function for recursive insertion
 */
TreeNode *BinarySearchTree::insertRecursive(TreeNode *node, int val)
{
    // Base case: If tree is empty or we've reached a leaf node, create a new node
    if (node == nullptr)
    {
        return new TreeNode(val);
    }

    // Recursive case: Insert into the appropriate subtree
    if (val < node->val)
    {
        node->left = insertRecursive(node->left, val);
    }
    else if (val > node->val)
    {
        node->right = insertRecursive(node->right, val);
    }
    // If value is already present, do nothing (no duplicates in this implementation)

    return node;
}

/**
 * Insert a value into the BST (iterative approach)
 */
void BinarySearchTree::insertIterative(int val)
{
    TreeNode *newNode = new TreeNode(val);

    // If the tree is empty, make the new node the root
    if (root == nullptr)
    {
        root = newNode;
        return;
    }

    TreeNode *current = root;
    TreeNode *parent = nullptr;

    // Find the appropriate position
    while (current != nullptr)
    {
        parent = current;

        if (val < current->val)
        {
            current = current->left;
        }
        else if (val > current->val)
        {
            current = current->right;
        }
        else
        {
            // Value already exists, don't add duplicate
            delete newNode;
            return;
        }
    }

    // Insert the new node
    if (val < parent->val)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }
}

/**
 * Remove a value from the BST
 */
void BinarySearchTree::remove(int val)
{
    root = deleteRecursive(root, val);
}

/**
 * Helper function for recursive deletion
 */
TreeNode *BinarySearchTree::deleteRecursive(TreeNode *node, int val)
{
    // Base case: If the tree is empty
    if (node == nullptr)
    {
        return nullptr;
    }

    // Find the node to be deleted
    if (val < node->val)
    {
        node->left = deleteRecursive(node->left, val);
    }
    else if (val > node->val)
    {
        node->right = deleteRecursive(node->right, val);
    }
    else
    {
        // Node with the value found

        // Case 1: Leaf node (no children)
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }

        // Case 2: Node with only one child
        else if (node->left == nullptr)
        {
            TreeNode *temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr)
        {
            TreeNode *temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Node with two children
        // Find the inorder successor (smallest node in the right subtree)
        TreeNode *temp = findMin(node->right);

        // Copy the inorder successor's value to this node
        node->val = temp->val;

        // Delete the inorder successor
        node->right = deleteRecursive(node->right, temp->val);
    }

    return node;
}

/**
 * Search for a value in the BST (recursive approach)
 */
bool BinarySearchTree::search(int val) const
{
    return searchRecursive(root, val);
}

/**
 * Helper function for recursive search
 */
bool BinarySearchTree::searchRecursive(TreeNode *node, int val) const
{
    // If tree is empty or value is found
    if (node == nullptr)
    {
        return false;
    }

    if (val == node->val)
    {
        return true;
    }

    // Search in the appropriate subtree
    if (val < node->val)
    {
        return searchRecursive(node->left, val);
    }
    else
    {
        return searchRecursive(node->right, val);
    }
}

/**
 * Search for a value in the BST (iterative approach)
 */
bool BinarySearchTree::searchIterative(int val) const
{
    TreeNode *current = root;

    while (current != nullptr)
    {
        if (val == current->val)
        {
            return true;
        }
        else if (val < current->val)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return false;
}

// ============================================================================
// Tree Traversal Methods
// ============================================================================

/**
 * Get in-order traversal of the BST (recursive)
 */
std::vector<int> BinarySearchTree::inorderTraversal() const
{
    std::vector<int> result;
    inorderTraversalRecursive(root, result);
    return result;
}

/**
 * Helper function for recursive in-order traversal
 */
void BinarySearchTree::inorderTraversalRecursive(TreeNode *node, std::vector<int> &result) const
{
    if (node == nullptr)
    {
        return;
    }

    inorderTraversalRecursive(node->left, result);
    result.push_back(node->val);
    inorderTraversalRecursive(node->right, result);
}

/**
 * Get in-order traversal of the BST (iterative)
 */
std::vector<int> BinarySearchTree::inorderTraversalIterative() const
{
    std::vector<int> result;

    if (root == nullptr)
    {
        return result;
    }

    std::stack<TreeNode *> stack;
    TreeNode *current = root;

    while (current != nullptr || !stack.empty())
    {
        // Reach the leftmost node of the current subtree
        while (current != nullptr)
        {
            stack.push(current);
            current = current->left;
        }

        // Current is now nullptr, pop from stack
        current = stack.top();
        stack.pop();

        // Visit the node
        result.push_back(current->val);

        // Move to the right subtree
        current = current->right;
    }

    return result;
}

/**
 * Get pre-order traversal of the BST (recursive)
 */
std::vector<int> BinarySearchTree::preorderTraversal() const
{
    std::vector<int> result;
    preorderTraversalRecursive(root, result);
    return result;
}

/**
 * Helper function for recursive pre-order traversal
 */
void BinarySearchTree::preorderTraversalRecursive(TreeNode *node, std::vector<int> &result) const
{
    if (node == nullptr)
    {
        return;
    }

    result.push_back(node->val);
    preorderTraversalRecursive(node->left, result);
    preorderTraversalRecursive(node->right, result);
}

/**
 * Get pre-order traversal of the BST (iterative)
 */
std::vector<int> BinarySearchTree::preorderTraversalIterative() const
{
    std::vector<int> result;

    if (root == nullptr)
    {
        return result;
    }

    std::stack<TreeNode *> stack;
    stack.push(root);

    while (!stack.empty())
    {
        TreeNode *current = stack.top();
        stack.pop();

        // Visit the node
        result.push_back(current->val);

        // Push right child first so that left child is processed first (LIFO)
        if (current->right)
        {
            stack.push(current->right);
        }
        if (current->left)
        {
            stack.push(current->left);
        }
    }

    return result;
}

/**
 * Get post-order traversal of the BST (recursive)
 */
std::vector<int> BinarySearchTree::postorderTraversal() const
{
    std::vector<int> result;
    postorderTraversalRecursive(root, result);
    return result;
}

/**
 * Helper function for recursive post-order traversal
 */
void BinarySearchTree::postorderTraversalRecursive(TreeNode *node, std::vector<int> &result) const
{
    if (node == nullptr)
    {
        return;
    }

    postorderTraversalRecursive(node->left, result);
    postorderTraversalRecursive(node->right, result);
    result.push_back(node->val);
}

/**
 * Get post-order traversal of the BST (iterative)
 * This uses two stacks approach for simplicity
 */
std::vector<int> BinarySearchTree::postorderTraversalIterative() const
{
    std::vector<int> result;

    if (root == nullptr)
    {
        return result;
    }

    std::stack<TreeNode *> s1, s2;
    s1.push(root);

    // First stack helps us to populate the second stack in reverse post-order
    while (!s1.empty())
    {
        TreeNode *current = s1.top();
        s1.pop();
        s2.push(current);

        if (current->left)
        {
            s1.push(current->left);
        }
        if (current->right)
        {
            s1.push(current->right);
        }
    }

    // Second stack now has nodes in post-order, but reversed
    while (!s2.empty())
    {
        result.push_back(s2.top()->val);
        s2.pop();
    }

    return result;
}

/**
 * Get level-order traversal of the BST
 */
std::vector<int> BinarySearchTree::levelOrderTraversal() const
{
    std::vector<int> result;

    if (root == nullptr)
    {
        return result;
    }

    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty())
    {
        TreeNode *current = q.front();
        q.pop();

        result.push_back(current->val);

        if (current->left)
        {
            q.push(current->left);
        }
        if (current->right)
        {
            q.push(current->right);
        }
    }

    return result;
}

/**
 * Get level-order traversal with levels separated
 */
std::vector<std::vector<int>> BinarySearchTree::levelOrderTraversalByLevel() const
{
    std::vector<std::vector<int>> result;

    if (root == nullptr)
    {
        return result;
    }

    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty())
    {
        int levelSize = q.size();
        std::vector<int> currentLevel;

        for (int i = 0; i < levelSize; i++)
        {
            TreeNode *current = q.front();
            q.pop();

            currentLevel.push_back(current->val);

            if (current->left)
            {
                q.push(current->left);
            }
            if (current->right)
            {
                q.push(current->right);
            }
        }

        result.push_back(currentLevel);
    }

    return result;
}

// ============================================================================
// Advanced BST Operations
// ============================================================================

/**
 * Find the minimum value in the BST
 */
int BinarySearchTree::findMinimum() const
{
    if (root == nullptr)
    {
        throw std::runtime_error("Tree is empty");
    }

    TreeNode *minNode = findMin(root);
    return minNode->val;
}

/**
 * Helper function to find the minimum value node
 */
TreeNode *BinarySearchTree::findMin(TreeNode *node) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    // The leftmost node contains the minimum value
    while (node->left != nullptr)
    {
        node = node->left;
    }

    return node;
}

/**
 * Find the maximum value in the BST
 */
int BinarySearchTree::findMaximum() const
{
    if (root == nullptr)
    {
        throw std::runtime_error("Tree is empty");
    }

    TreeNode *maxNode = findMax(root);
    return maxNode->val;
}

/**
 * Helper function to find the maximum value node
 */
TreeNode *BinarySearchTree::findMax(TreeNode *node) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    // The rightmost node contains the maximum value
    while (node->right != nullptr)
    {
        node = node->right;
    }

    return node;
}

/**
 * Find the successor of a given value (next largest value)
 */
int BinarySearchTree::successor(int val) const
{
    if (root == nullptr)
    {
        return -1; // Tree is empty
    }

    TreeNode *current = root;
    TreeNode *successor = nullptr;

    // Find the node with val
    while (current != nullptr && current->val != val)
    {
        if (val < current->val)
        {
            successor = current; // Potential successor
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // Node with val not found
    if (current == nullptr)
    {
        return -1;
    }

    // If the node has a right subtree, the successor is the minimum in that subtree
    if (current->right != nullptr)
    {
        return findMin(current->right)->val;
    }

    // Otherwise, it's the lowest ancestor whose left child is also an ancestor
    return (successor != nullptr) ? successor->val : -1;
}

/**
 * Find the predecessor of a given value (next smallest value)
 */
int BinarySearchTree::predecessor(int val) const
{
    if (root == nullptr)
    {
        return -1; // Tree is empty
    }

    TreeNode *current = root;
    TreeNode *predecessor = nullptr;

    // Find the node with val
    while (current != nullptr && current->val != val)
    {
        if (val > current->val)
        {
            predecessor = current; // Potential predecessor
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }

    // Node with val not found
    if (current == nullptr)
    {
        return -1;
    }

    // If the node has a left subtree, the predecessor is the maximum in that subtree
    if (current->left != nullptr)
    {
        return findMax(current->left)->val;
    }

    // Otherwise, it's the lowest ancestor whose right child is also an ancestor
    return (predecessor != nullptr) ? predecessor->val : -1;
}

/**
 * Find the k-th smallest element in the BST (1-indexed)
 */
int BinarySearchTree::kthSmallest(int k) const
{
    if (root == nullptr || k <= 0 || k > size())
    {
        throw std::out_of_range("k is out of range");
    }

    int count = 0;
    int result = -1;

    // Using a lambda to capture by reference
    std::function<void(TreeNode *)> inorderHelper = [&](TreeNode *node)
    {
        if (node == nullptr || count >= k)
        {
            return;
        }

        inorderHelper(node->left);

        count++;
        if (count == k)
        {
            result = node->val;
            return;
        }

        inorderHelper(node->right);
    };

    inorderHelper(root);
    return result;
}

/**
 * Find all elements within a given range [low, high]
 */
std::vector<int> BinarySearchTree::rangeSearch(int low, int high) const
{
    std::vector<int> result;
    rangeSearchHelper(root, low, high, result);
    return result;
}

/**
 * Helper function for range search
 */
void BinarySearchTree::rangeSearchHelper(TreeNode *node, int low, int high, std::vector<int> &result) const
{
    if (node == nullptr)
    {
        return;
    }

    // If current node's value is greater than the lower bound, search left subtree
    if (low < node->val)
    {
        rangeSearchHelper(node->left, low, high, result);
    }

    // If current node's value is within range, add it to result
    if (low <= node->val && node->val <= high)
    {
        result.push_back(node->val);
    }

    // If current node's value is less than the upper bound, search right subtree
    if (node->val < high)
    {
        rangeSearchHelper(node->right, low, high, result);
    }
}

/**
 * Find the closest value to a given target
 */
int BinarySearchTree::closestValue(int target) const
{
    if (root == nullptr)
    {
        throw std::runtime_error("Tree is empty");
    }

    int closest = root->val;
    int minDiff = std::abs(root->val - target);

    TreeNode *current = root;

    while (current != nullptr)
    {
        int currentDiff = std::abs(current->val - target);

        // Update closest if current node is closer to target
        if (currentDiff < minDiff)
        {
            minDiff = currentDiff;
            closest = current->val;
        }

        // If we found an exact match, return immediately
        if (currentDiff == 0)
        {
            return current->val;
        }

        // Move to the appropriate subtree
        if (target < current->val)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return closest;
}

// ============================================================================
// BST Utilities
// ============================================================================

/**
 * Get the height of the BST
 */
int BinarySearchTree::height() const
{
    return getHeight(root);
}

/**
 * Helper function to get the height of a subtree
 */
int BinarySearchTree::getHeight(TreeNode *node) const
{
    if (node == nullptr)
    {
        return -1; // Height of empty tree is -1
    }

    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

/**
 * Get the number of nodes in the BST
 */
int BinarySearchTree::size() const
{
    return getSize(root);
}

/**
 * Helper function to get the size of a subtree
 */
int BinarySearchTree::getSize(TreeNode *node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    return 1 + getSize(node->left) + getSize(node->right);
}

/**
 * Check if the BST is empty
 */
bool BinarySearchTree::isEmpty() const
{
    return root == nullptr;
}

/**
 * Check if the tree is a valid BST
 */
bool BinarySearchTree::isValidBST() const
{
    return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
}

/**
 * Helper function to check if a tree is a valid BST
 */
bool BinarySearchTree::isValidBSTHelper(TreeNode *node, long minVal, long maxVal) const
{
    if (node == nullptr)
    {
        return true;
    }

    // Check if current node's value is within valid range
    if (node->val <= minVal || node->val >= maxVal)
    {
        return false;
    }

    // Recursively check left and right subtrees with updated ranges
    return isValidBSTHelper(node->left, minVal, node->val) &&
           isValidBSTHelper(node->right, node->val, maxVal);
}

/**
 * Balance an unbalanced BST
 */
BinarySearchTree &BinarySearchTree::balance()
{
    // Step 1: Get sorted elements using in-order traversal
    std::vector<int> sortedElements;
    collectInorder(root, sortedElements);

    // Step 2: Rebuild a balanced tree
    deleteTree(root);
    root = buildBalancedBST(sortedElements, 0, sortedElements.size() - 1);

    return *this;
}

/**
 * Helper to collect nodes in sorted order
 */
void BinarySearchTree::collectInorder(TreeNode *node, std::vector<int> &elements) const
{
    if (node == nullptr)
    {
        return;
    }

    collectInorder(node->left, elements);
    elements.push_back(node->val);
    collectInorder(node->right, elements);
}

/**
 * Helper to build a balanced BST from sorted array
 */
TreeNode *BinarySearchTree::buildBalancedBST(const std::vector<int> &elements, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }

    // Use the middle element as the root
    int mid = start + (end - start) / 2;
    TreeNode *node = new TreeNode(elements[mid]);

    // Recursively build left and right subtrees
    node->left = buildBalancedBST(elements, start, mid - 1);
    node->right = buildBalancedBST(elements, mid + 1, end);

    return node;
}

/**
 * Helper to delete the entire tree
 */
void BinarySearchTree::deleteTree(TreeNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

/**
 * Clear the BST (remove all nodes)
 */
void BinarySearchTree::clear()
{
    deleteTree(root);
    root = nullptr;
}

/**
 * Get the root node of the BST
 */
TreeNode *BinarySearchTree::getRoot() const
{
    return root;
}

// ============================================================================
// Visualization Methods
// ============================================================================

/**
 * Print the BST structure
 */
void BinarySearchTree::printTree() const
{
    if (root == nullptr)
    {
        std::cout << "Empty tree" << std::endl;
        return;
    }

    printNodeInternal(root, "", true);
}

/**
 * Helper to print node and its children
 */
void BinarySearchTree::printNodeInternal(TreeNode *node, std::string prefix, bool isLeft) const
{
    if (node == nullptr)
    {
        return;
    }

    std::cout << prefix;
    std::cout << (isLeft ? "└── " : "┌── ");

    // Print the value of the node
    std::cout << node->val << std::endl;

    // Enter the next tree level - left and right branch
    printNodeInternal(node->right, prefix + (isLeft ? "    " : "│   "), false);
    printNodeInternal(node->left, prefix + (isLeft ? "    " : "│   "), true);
}

/**
 * Print the BST statistics
 */
void BinarySearchTree::printStats() const
{
    std::cout << "BST Statistics:" << std::endl;
    std::cout << "Size: " << size() << " nodes" << std::endl;
    std::cout << "Height: " << height() << std::endl;

    // Calculate tree balance metrics
    int balanceFactor = 0;
    if (root)
    {
        int leftHeight = (root->left) ? getHeight(root->left) + 1 : 0;
        int rightHeight = (root->right) ? getHeight(root->right) + 1 : 0;
        balanceFactor = leftHeight - rightHeight;
    }

    std::cout << "Root balance factor: " << balanceFactor << std::endl;
    std::cout << "Is valid BST: " << (isValidBST() ? "Yes" : "No") << std::endl;

    // Calculate ideal height for comparison
    double idealHeight = std::log2(size() + 1) - 1;
    double efficiency = idealHeight / height() * 100;

    std::cout << "Ideal height: " << std::fixed << std::setprecision(2) << idealHeight << std::endl;
    std::cout << "Tree efficiency: " << efficiency << "%" << std::endl;

    // Print the first few elements in sorted order
    std::vector<int> sortedElements = inorderTraversal();
    std::cout << "Sorted elements (first 10): ";
    for (int i = 0; i < std::min(10, (int)sortedElements.size()); i++)
    {
        std::cout << sortedElements[i] << " ";
    }
    if (sortedElements.size() > 10)
    {
        std::cout << "...";
    }
    std::cout << std::endl;
}

// ============================================================================
// BST Testing Utilities
// ============================================================================

/**
 * Generate a random BST with specified number of nodes
 *
 * @param nodeCount Number of nodes to generate
 * @param minVal Minimum value for random nodes
 * @param maxVal Maximum value for random nodes
 * @return A BST with random values
 */
BinarySearchTree generateRandomBST(int nodeCount, int minVal = 1, int maxVal = 1000)
{
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minVal, maxVal);

    // Create a set to avoid duplicates
    std::set<int> values;
    while (values.size() < nodeCount)
    {
        values.insert(distrib(gen));
    }

    // Create the BST
    BinarySearchTree bst;
    for (int val : values)
    {
        bst.insert(val);
    }

    return bst;
}

/**
 * Generate a balanced BST with specified number of nodes
 *
 * @param nodeCount Number of nodes to generate
 * @return A balanced BST
 */
BinarySearchTree generateBalancedBST(int nodeCount)
{
    std::vector<int> values;

    // Generate sequential values for optimal balance
    for (int i = 1; i <= nodeCount; i++)
    {
        values.push_back(i);
    }

    // Create a BST from the sorted array
    return BinarySearchTree(values);
}

/**
 * Generate a skewed BST with specified number of nodes
 *
 * @param nodeCount Number of nodes to generate
 * @param rightSkewed If true, create right-skewed tree, otherwise left-skewed
 * @return A skewed BST
 */
BinarySearchTree generateSkewedBST(int nodeCount, bool rightSkewed = true)
{
    BinarySearchTree bst;

    if (rightSkewed)
    {
        // Insert in ascending order for right-skewed tree
        for (int i = 1; i <= nodeCount; i++)
        {
            bst.insert(i);
        }
    }
    else
    {
        // Insert in descending order for left-skewed tree
        for (int i = nodeCount; i >= 1; i--)
        {
            bst.insert(i);
        }
    }

    return bst;
}

/**
 * Measure execution time of a BST operation
 *
 * @param operation Function to measure
 * @return Execution time in microseconds
 */
template <typename Func>
long long measureExecutionTime(Func operation)
{
    auto start = std::chrono::high_resolution_clock::now();
    operation();
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

/**
 * Compare performance of balanced vs. unbalanced BST
 *
 * @param nodeCount Number of nodes in each tree
 * @param searchCount Number of searches to perform
 */
void compareBSTPerformance(int nodeCount, int searchCount)
{
    // Create a balanced BST
    BinarySearchTree balancedBST = generateBalancedBST(nodeCount);

    // Create a skewed (unbalanced) BST
    BinarySearchTree skewedBST = generateSkewedBST(nodeCount);

    // Generate random values to search for
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, nodeCount);
    std::vector<int> searchValues;
    for (int i = 0; i < searchCount; i++)
    {
        searchValues.push_back(distrib(gen));
    }

    // Measure search time for balanced BST
    auto searchBalanced = [&]()
    {
        for (int val : searchValues)
        {
            balancedBST.search(val);
        }
    };
    long long balancedTime = measureExecutionTime(searchBalanced);

    // Measure search time for skewed BST
    auto searchSkewed = [&]()
    {
        for (int val : searchValues)
        {
            skewedBST.search(val);
        }
    };
    long long skewedTime = measureExecutionTime(searchSkewed);

    // Print results
    std::cout << "=== BST Performance Comparison ===" << std::endl;
    std::cout << "Node count: " << nodeCount << std::endl;
    std::cout << "Search operations: " << searchCount << std::endl;
    std::cout << "Balanced BST height: " << balancedBST.height() << std::endl;
    std::cout << "Skewed BST height: " << skewedBST.height() << std::endl;
    std::cout << "Balanced BST search time: " << balancedTime << " microseconds" << std::endl;
    std::cout << "Skewed BST search time: " << skewedTime << " microseconds" << std::endl;
    std::cout << "Speedup factor: " << (double)skewedTime / balancedTime << "x" << std::endl;
    std::cout << std::endl;
}

// ============================================================================
// Main Function with Examples and Demonstrations
// ============================================================================

int main()
{
    std::cout << "=======================================================" << std::endl;
    std::cout << "            Binary Search Tree Demonstration            " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << std::endl;

    // Create a sample BST
    BinarySearchTree bst;
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7};

    std::cout << "Inserting values: ";
    for (int val : values)
    {
        std::cout << val << " ";
        bst.insert(val);
    }
    std::cout << std::endl
              << std::endl;

    // Print the BST structure
    std::cout << "BST Structure:" << std::endl;
    bst.printTree();
    std::cout << std::endl;

    // Print BST stats
    bst.printStats();
    std::cout << std::endl;

    // Demonstrate traversals
    std::cout << "=== BST Traversals ===" << std::endl;

    // In-order traversal
    std::vector<int> inorder = bst.inorderTraversal();
    std::cout << "In-order (sorted) traversal: ";
    for (int val : inorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Pre-order traversal
    std::vector<int> preorder = bst.preorderTraversal();
    std::cout << "Pre-order traversal: ";
    for (int val : preorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Post-order traversal
    std::vector<int> postorder = bst.postorderTraversal();
    std::cout << "Post-order traversal: ";
    for (int val : postorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Level-order traversal
    std::vector<int> levelorder = bst.levelOrderTraversal();
    std::cout << "Level-order traversal: ";
    for (int val : levelorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Level-order traversal by level
    std::vector<std::vector<int>> levels = bst.levelOrderTraversalByLevel();
    std::cout << "Level-order traversal by level:" << std::endl;
    for (size_t i = 0; i < levels.size(); i++)
    {
        std::cout << "  Level " << i << ": ";
        for (int val : levels[i])
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Demonstrate BST operations
    std::cout << "=== BST Operations ===" << std::endl;

    // Search for values
    int searchValue = 6;
    std::cout << "Search for " << searchValue << ": "
              << (bst.search(searchValue) ? "Found" : "Not found") << std::endl;

    searchValue = 5;
    std::cout << "Search for " << searchValue << ": "
              << (bst.search(searchValue) ? "Found" : "Not found") << std::endl;

    // Find min and max
    std::cout << "Minimum value: " << bst.findMinimum() << std::endl;
    std::cout << "Maximum value: " << bst.findMaximum() << std::endl;

    // Find successor and predecessor
    int testValue = 6;
    std::cout << "Successor of " << testValue << ": " << bst.successor(testValue) << std::endl;
    std::cout << "Predecessor of " << testValue << ": " << bst.predecessor(testValue) << std::endl;

    // Find k-th smallest
    int k = 3;
    std::cout << k << "-th smallest element: " << bst.kthSmallest(k) << std::endl;

    // Range search
    int low = 3, high = 8;
    std::vector<int> rangeResult = bst.rangeSearch(low, high);
    std::cout << "Values in range [" << low << ", " << high << "]: ";
    for (int val : rangeResult)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Find closest value
    int target = 5;
    std::cout << "Closest value to " << target << ": " << bst.closestValue(target) << std::endl;

    std::cout << std::endl;

    // Demonstrate deletion
    std::cout << "=== BST Deletion ===" << std::endl;

    // Delete a leaf node
    int deleteValue = 4;
    std::cout << "Deleting leaf node " << deleteValue << std::endl;
    bst.remove(deleteValue);
    bst.printTree();
    std::cout << std::endl;

    // Delete a node with one child
    deleteValue = 14;
    std::cout << "Deleting node " << deleteValue << " with no children" << std::endl;
    bst.remove(deleteValue);
    bst.printTree();
    std::cout << std::endl;

    // Delete a node with two children
    deleteValue = 3;
    std::cout << "Deleting node " << deleteValue << " with two children" << std::endl;
    bst.remove(deleteValue);
    bst.printTree();
    std::cout << std::endl;

    // Demonstrate balancing
    std::cout << "=== BST Balancing ===" << std::endl;

    // Create an unbalanced (skewed) BST
    BinarySearchTree skewedBST;
    for (int i = 1; i <= 10; i++)
    {
        skewedBST.insert(i); // Insert in ascending order for right-skewed tree
    }

    std::cout << "Unbalanced (right-skewed) BST:" << std::endl;
    skewedBST.printTree();
    std::cout << "Height: " << skewedBST.height() << std::endl;
    std::cout << std::endl;

    // Balance the BST
    std::cout << "After balancing:" << std::endl;
    skewedBST.balance();
    skewedBST.printTree();
    std::cout << "Height: " << skewedBST.height() << std::endl;
    std::cout << std::endl;

    // Performance comparison
    std::cout << "=== Performance Comparisons ===" << std::endl;
    compareBSTPerformance(1000, 10000);
    compareBSTPerformance(10000, 1000);

    // Special cases
    std::cout << "=== Special Cases ===" << std::endl;

    // Empty BST
    BinarySearchTree emptyBST;
    std::cout << "Empty BST:" << std::endl;
    emptyBST.printTree();
    std::cout << "Size: " << emptyBST.size() << std::endl;
    std::cout << "Height: " << emptyBST.height() << std::endl;
    std::cout << "Is empty: " << (emptyBST.isEmpty() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;

    // BST with single node
    BinarySearchTree singleNodeBST;
    singleNodeBST.insert(42);
    std::cout << "Single node BST:" << std::endl;
    singleNodeBST.printTree();
    std::cout << "Size: " << singleNodeBST.size() << std::endl;
    std::cout << "Height: " << singleNodeBST.height() << std::endl;
    std::cout << "Is empty: " << (singleNodeBST.isEmpty() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;

    // Create a BST from a sorted array
    std::vector<int> sortedArray = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinarySearchTree balancedBST(sortedArray);
    std::cout << "BST created from sorted array:" << std::endl;
    balancedBST.printTree();
    std::cout << "Height: " << balancedBST.height() << std::endl;
    std::cout << std::endl;

    return 0;
}