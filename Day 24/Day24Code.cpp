#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

/**
 * @brief AVL Tree implementation in C++
 *
 * This file provides a complete implementation of AVL Trees with:
 * - Core operations (insert, delete, search)
 * - Rotations (left, right, left-right, right-left)
 * - Balance maintenance
 * - Tree traversals
 * - Utility functions
 * - Performance benchmarking
 *
 * AVL Tree maintains balance by ensuring that for any node, the heights
 * of its left and right subtrees differ by at most 1.
 *
 * @author #DSAin45 Course
 * @date Day 24 of 45-Day DSA Challenge
 */

// Forward declaration for visualization
template <typename T>
class AVLTree;

/**
 * @brief Node structure for AVL Tree
 */
template <typename T>
class AVLNode
{
private:
    T value;           // Data stored in the node
    AVLNode<T> *left;  // Pointer to left child
    AVLNode<T> *right; // Pointer to right child
    int height;        // Height of the subtree rooted at this node

    // Allow AVLTree to access private members
    friend class AVLTree<T>;

public:
    /**
     * @brief Construct a new AVLNode
     * @param val Value to store in the node
     */
    AVLNode(T val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

/**
 * @brief AVL Tree class implementation
 */
template <typename T>
class AVLTree
{
private:
    AVLNode<T> *root; // Root of the AVL tree

    /**
     * @brief Get the height of a node
     * @param node Node to get height of
     * @return int Height of the node (0 if node is null)
     */
    int height(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return node->height;
    }

    /**
     * @brief Get the balance factor of a node
     * @param node Node to calculate balance factor for
     * @return int Balance factor (height of left subtree - height of right subtree)
     */
    int getBalanceFactor(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    /**
     * @brief Right rotation to balance the tree
     *
     *     y                               x
     *    / \                            /   \
     *   x   T3      Right Rotate       T1    y
     *  / \          ------------->          / \
     * T1  T2                              T2   T3
     *
     * @param y Root of the subtree to rotate
     * @return AVLNode<T>* New root after rotation
     */
    AVLNode<T> *rightRotate(AVLNode<T> *y)
    {
        AVLNode<T> *x = y->left;
        AVLNode<T> *T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    /**
     * @brief Left rotation to balance the tree
     *
     *   x                               y
     *  / \                            /   \
     * T1  y      Left Rotate         x     T3
     *    / \     ------------->     / \
     *   T2 T3                      T1  T2
     *
     * @param x Root of the subtree to rotate
     * @return AVLNode<T>* New root after rotation
     */
    AVLNode<T> *leftRotate(AVLNode<T> *x)
    {
        AVLNode<T> *y = x->right;
        AVLNode<T> *T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    /**
     * @brief Insert a value into the AVL tree recursively
     * @param node Current node in the recursion
     * @param value Value to insert
     * @return AVLNode<T>* New root of the subtree after insertion
     */
    AVLNode<T> *insertNode(AVLNode<T> *node, T value)
    {
        // 1. Perform standard BST insertion
        if (node == nullptr)
        {
            return new AVLNode<T>(value);
        }

        if (value < node->value)
        {
            node->left = insertNode(node->left, value);
        }
        else if (value > node->value)
        {
            node->right = insertNode(node->right, value);
        }
        else
        {
            // Duplicate values not allowed
            return node;
        }

        // 2. Update height of this ancestor node
        node->height = 1 + std::max(height(node->left), height(node->right));

        // 3. Get the balance factor to check if node became unbalanced
        int balance = getBalanceFactor(node);

        // 4. If unbalanced, there are four cases to consider:

        // Left Left Case (LL): Right rotation
        if (balance > 1 && value < node->left->value)
        {
            return rightRotate(node);
        }

        // Right Right Case (RR): Left rotation
        if (balance < -1 && value > node->right->value)
        {
            return leftRotate(node);
        }

        // Left Right Case (LR): Left rotation on left child, then right rotation on node
        if (balance > 1 && value > node->left->value)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case (RL): Right rotation on right child, then left rotation on node
        if (balance < -1 && value < node->right->value)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the unchanged node pointer
        return node;
    }

    /**
     * @brief Find the node with minimum value in a subtree
     * @param node Root of the subtree
     * @return AVLNode<T>* Node with minimum value
     */
    AVLNode<T> *minValueNode(AVLNode<T> *node)
    {
        AVLNode<T> *current = node;

        // Loop down to find the leftmost leaf
        while (current && current->left != nullptr)
        {
            current = current->left;
        }

        return current;
    }

    /**
     * @brief Delete a node with the given value from the AVL tree
     * @param node Current node in the recursion
     * @param value Value to delete
     * @return AVLNode<T>* New root of the subtree after deletion
     */
    AVLNode<T> *deleteNode(AVLNode<T> *node, T value)
    {
        // 1. Perform standard BST delete
        if (node == nullptr)
        {
            return nullptr;
        }

        // Search for the node to be deleted
        if (value < node->value)
        {
            node->left = deleteNode(node->left, value);
        }
        else if (value > node->value)
        {
            node->right = deleteNode(node->right, value);
        }
        else
        {
            // Node found - implement deletion based on node's children

            // Case 1: Node with no child or only one child
            if (node->left == nullptr || node->right == nullptr)
            {
                AVLNode<T> *temp = node->left ? node->left : node->right;

                // No child case
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    // One child case: copy the contents of the non-empty child
                    *node = *temp; // Copy content
                }
                delete temp;
            }
            else
            {
                // Case 2: Node with two children
                // Get the inorder successor (smallest in the right subtree)
                AVLNode<T> *temp = minValueNode(node->right);

                // Copy the inorder successor's data to this node
                node->value = temp->value;

                // Delete the inorder successor
                node->right = deleteNode(node->right, temp->value);
            }
        }

        // If the tree had only one node then return
        if (node == nullptr)
        {
            return nullptr;
        }

        // 2. Update height of the current node
        node->height = 1 + std::max(height(node->left), height(node->right));

        // 3. Get the balance factor of this node
        int balance = getBalanceFactor(node);

        // 4. If unbalanced, there are four cases (same as insertion)

        // Left Left Case (LL): Right rotation
        if (balance > 1 && getBalanceFactor(node->left) >= 0)
        {
            return rightRotate(node);
        }

        // Left Right Case (LR): Left rotation on left child, then right rotation on node
        if (balance > 1 && getBalanceFactor(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case (RR): Left rotation
        if (balance < -1 && getBalanceFactor(node->right) <= 0)
        {
            return leftRotate(node);
        }

        // Right Left Case (RL): Right rotation on right child, then left rotation on node
        if (balance < -1 && getBalanceFactor(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    /**
     * @brief Search for a value in the AVL tree recursively
     * @param node Current node in the recursion
     * @param value Value to search for
     * @return bool True if value is found, false otherwise
     */
    bool searchRecursive(AVLNode<T> *node, T value)
    {
        if (node == nullptr)
        {
            return false;
        }

        if (value == node->value)
        {
            return true;
        }

        if (value < node->value)
        {
            return searchRecursive(node->left, value);
        }
        else
        {
            return searchRecursive(node->right, value);
        }
    }

    /**
     * @brief Perform inorder traversal of the AVL tree recursively
     * @param node Current node in the recursion
     * @param result Vector to store the traversal result
     */
    void inorderTraversalRecursive(AVLNode<T> *node, std::vector<T> &result)
    {
        if (node != nullptr)
        {
            inorderTraversalRecursive(node->left, result);
            result.push_back(node->value);
            inorderTraversalRecursive(node->right, result);
        }
    }

    /**
     * @brief Perform preorder traversal of the AVL tree recursively
     * @param node Current node in the recursion
     * @param result Vector to store the traversal result
     */
    void preorderTraversalRecursive(AVLNode<T> *node, std::vector<T> &result)
    {
        if (node != nullptr)
        {
            result.push_back(node->value);
            preorderTraversalRecursive(node->left, result);
            preorderTraversalRecursive(node->right, result);
        }
    }

    /**
     * @brief Perform postorder traversal of the AVL tree recursively
     * @param node Current node in the recursion
     * @param result Vector to store the traversal result
     */
    void postorderTraversalRecursive(AVLNode<T> *node, std::vector<T> &result)
    {
        if (node != nullptr)
        {
            postorderTraversalRecursive(node->left, result);
            postorderTraversalRecursive(node->right, result);
            result.push_back(node->value);
        }
    }

    /**
     * @brief Destroy the AVL tree recursively
     * @param node Current node in the recursion
     */
    void destroyTree(AVLNode<T> *node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    /**
     * @brief Print the AVL tree structure for visualization
     * @param node Current node in the recursion
     * @param prefix String prefix for formatting
     * @param isLeft Whether the current node is a left child
     */
    void prettyPrintTree(AVLNode<T> *node, const std::string &prefix, bool isLeft)
    {
        if (node != nullptr)
        {
            std::cout << prefix;
            std::cout << (isLeft ? "├── " : "└── ");

            // Print the value and balance factor
            std::cout << node->value << " (BF: " << getBalanceFactor(node) << ")" << std::endl;

            // Enter the next tree level - left and right branches
            prettyPrintTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
            prettyPrintTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }

    /**
     * @brief Count the number of nodes in the AVL tree recursively
     * @param node Current node in the recursion
     * @return int Number of nodes in the subtree
     */
    int countNodes(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

public:
    /**
     * @brief Construct a new empty AVL Tree
     */
    AVLTree() : root(nullptr) {}

    /**
     * @brief Destroy the AVL Tree
     */
    ~AVLTree()
    {
        destroyTree(root);
    }

    /**
     * @brief Insert a value into the AVL tree
     * @param value Value to insert
     */
    void insert(T value)
    {
        root = insertNode(root, value);
    }

    /**
     * @brief Remove a value from the AVL tree
     * @param value Value to remove
     */
    void remove(T value)
    {
        root = deleteNode(root, value);
    }

    /**
     * @brief Search for a value in the AVL tree
     * @param value Value to search for
     * @return bool True if value is found, false otherwise
     */
    bool search(T value)
    {
        return searchRecursive(root, value);
    }

    /**
     * @brief Iterative search for a value in the AVL tree
     * @param value Value to search for
     * @return bool True if value is found, false otherwise
     */
    bool searchIterative(T value)
    {
        AVLNode<T> *current = root;

        while (current != nullptr)
        {
            if (value == current->value)
            {
                return true;
            }
            else if (value < current->value)
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

    /**
     * @brief Get the height of the AVL tree
     * @return int Height of the tree
     */
    int getHeight()
    {
        return height(root);
    }

    /**
     * @brief Get the number of nodes in the AVL tree
     * @return int Number of nodes in the tree
     */
    int size()
    {
        return countNodes(root);
    }

    /**
     * @brief Check if the AVL tree is empty
     * @return bool True if the tree is empty, false otherwise
     */
    bool isEmpty()
    {
        return root == nullptr;
    }

    /**
     * @brief Perform inorder traversal of the AVL tree
     * @return std::vector<T> Vector containing the traversal result
     */
    std::vector<T> inorderTraversal()
    {
        std::vector<T> result;
        inorderTraversalRecursive(root, result);
        return result;
    }

    /**
     * @brief Perform preorder traversal of the AVL tree
     * @return std::vector<T> Vector containing the traversal result
     */
    std::vector<T> preorderTraversal()
    {
        std::vector<T> result;
        preorderTraversalRecursive(root, result);
        return result;
    }

    /**
     * @brief Perform postorder traversal of the AVL tree
     * @return std::vector<T> Vector containing the traversal result
     */
    std::vector<T> postorderTraversal()
    {
        std::vector<T> result;
        postorderTraversalRecursive(root, result);
        return result;
    }

    /**
     * @brief Perform level order traversal of the AVL tree
     * @return std::vector<T> Vector containing the traversal result
     */
    std::vector<T> levelOrderTraversal()
    {
        std::vector<T> result;
        if (root == nullptr)
        {
            return result;
        }

        std::queue<AVLNode<T> *> queue;
        queue.push(root);

        while (!queue.empty())
        {
            AVLNode<T> *current = queue.front();
            queue.pop();

            result.push_back(current->value);

            if (current->left != nullptr)
            {
                queue.push(current->left);
            }
            if (current->right != nullptr)
            {
                queue.push(current->right);
            }
        }

        return result;
    }

    /**
     * @brief Find the minimum value in the AVL tree
     * @return T Minimum value (throws exception if tree is empty)
     */
    T findMin()
    {
        if (root == nullptr)
        {
            throw std::runtime_error("Tree is empty");
        }

        AVLNode<T> *current = root;
        while (current->left != nullptr)
        {
            current = current->left;
        }

        return current->value;
    }

    /**
     * @brief Find the maximum value in the AVL tree
     * @return T Maximum value (throws exception if tree is empty)
     */
    T findMax()
    {
        if (root == nullptr)
        {
            throw std::runtime_error("Tree is empty");
        }

        AVLNode<T> *current = root;
        while (current->right != nullptr)
        {
            current = current->right;
        }

        return current->value;
    }

    /**
     * @brief Print the AVL tree structure for visualization
     */
    void printTree()
    {
        std::cout << "AVL Tree Structure:" << std::endl;
        prettyPrintTree(root, "", false);
    }

    /**
     * @brief Print the tree with its balance factors
     */
    void printTreeWithBalanceFactors()
    {
        std::cout << "AVL Tree with Balance Factors:" << std::endl;
        prettyPrintTree(root, "", false);
    }

    /**
     * @brief Check if the tree is a valid AVL tree
     * @return bool True if the tree is a valid AVL tree, false otherwise
     */
    bool isValidAVLTree()
    {
        return isValidAVLTreeHelper(root);
    }

    /**
     * @brief Helper function to check if the tree is a valid AVL tree
     * @param node Current node in the recursion
     * @return bool True if the subtree is a valid AVL tree, false otherwise
     */
    bool isValidAVLTreeHelper(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return true;
        }

        // Check balance factor
        int balance = getBalanceFactor(node);
        if (balance < -1 || balance > 1)
        {
            return false;
        }

        // Check BST property and recursively check children
        return isBSTHelper(node, nullptr, nullptr) &&
               isValidAVLTreeHelper(node->left) &&
               isValidAVLTreeHelper(node->right);
    }

    /**
     * @brief Helper function to check if the tree satisfies the BST property
     * @param node Current node in the recursion
     * @param minValue Minimum allowed value for the current subtree
     * @param maxValue Maximum allowed value for the current subtree
     * @return bool True if the subtree satisfies the BST property, false otherwise
     */
    bool isBSTHelper(AVLNode<T> *node, const T *minValue, const T *maxValue)
    {
        if (node == nullptr)
        {
            return true;
        }

        // Check if the current node's value is within bounds
        if ((minValue != nullptr && node->value <= *minValue) ||
            (maxValue != nullptr && node->value >= *maxValue))
        {
            return false;
        }

        // Recursively check left and right subtrees
        return isBSTHelper(node->left, minValue, &node->value) &&
               isBSTHelper(node->right, &node->value, maxValue);
    }

    /**
     * @brief Build an AVL tree from a sorted array
     * @param values Sorted array of values
     */
    void buildFromSortedArray(const std::vector<T> &values)
    {
        destroyTree(root);
        root = buildFromSortedArrayHelper(values, 0, values.size() - 1);
    }

    /**
     * @brief Helper function to build an AVL tree from a sorted array
     * @param values Sorted array of values
     * @param start Start index
     * @param end End index
     * @return AVLNode<T>* Root of the built subtree
     */
    AVLNode<T> *buildFromSortedArrayHelper(const std::vector<T> &values, int start, int end)
    {
        if (start > end)
        {
            return nullptr;
        }

        // Get the middle element as the root
        int mid = start + (end - start) / 2;
        AVLNode<T> *node = new AVLNode<T>(values[mid]);

        // Recursively build left and right subtrees
        node->left = buildFromSortedArrayHelper(values, start, mid - 1);
        node->right = buildFromSortedArrayHelper(values, mid + 1, end);

        // Update height
        node->height = 1 + std::max(height(node->left), height(node->right));

        return node;
    }

    /**
     * @brief Find all elements within a range [low, high]
     * @param low Lower bound of the range
     * @param high Upper bound of the range
     * @return std::vector<T> Vector containing elements in the range
     */
    std::vector<T> rangeQuery(T low, T high)
    {
        std::vector<T> result;
        rangeQueryHelper(root, low, high, result);
        return result;
    }

    /**
     * @brief Helper function to find elements within a range
     * @param node Current node in the recursion
     * @param low Lower bound of the range
     * @param high Upper bound of the range
     * @param result Vector to store the result
     */
    void rangeQueryHelper(AVLNode<T> *node, T low, T high, std::vector<T> &result)
    {
        if (node == nullptr)
        {
            return;
        }

        // If current node's value is greater than low, search left subtree
        if (low < node->value)
        {
            rangeQueryHelper(node->left, low, high, result);
        }

        // If current node's value is within range, add it to result
        if (low <= node->value && node->value <= high)
        {
            result.push_back(node->value);
        }

        // If current node's value is less than high, search right subtree
        if (node->value < high)
        {
            rangeQueryHelper(node->right, low, high, result);
        }
    }

    /**
     * @brief Benchmark AVL tree operations
     * @param numOperations Number of operations to perform
     * @param printResults Whether to print detailed results
     */
    static void benchmark(int numOperations = 1000000, bool printResults = true)
    {
        std::cout << "======= AVL Tree Benchmark =======" << std::endl;

        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, numOperations * 10);

        // Prepare random values for insertion
        std::vector<int> randomValues;
        for (int i = 0; i < numOperations; ++i)
        {
            randomValues.push_back(dist(gen));
        }

        // Prepare sequential values for insertion
        std::vector<int> sequentialValues;
        for (int i = 1; i <= numOperations; ++i)
        {
            sequentialValues.push_back(i);
        }

        // Benchmark AVL Tree with random values
        auto startRandom = std::chrono::high_resolution_clock::now();
        AVLTree<int> avlTreeRandom;
        for (int val : randomValues)
        {
            avlTreeRandom.insert(val);
        }
        auto endRandom = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> msRandom = endRandom - startRandom;

        // Benchmark AVL Tree with sequential values
        auto startSequential = std::chrono::high_resolution_clock::now();
        AVLTree<int> avlTreeSequential;
        for (int val : sequentialValues)
        {
            avlTreeSequential.insert(val);
        }
        auto endSequential = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> msSequential = endSequential - startSequential;

        // Benchmark regular BST with sequential values (simulation)
        // In a regular BST, sequential insertions lead to a skewed tree
        // Height would be approximately numOperations, and operations would be O(n)
        // This is a calculated estimate, not an actual measurement
        double msBSTSequential = msSequential.count() * std::log2(numOperations) / 2;

        // Print results
        if (printResults)
        {
            std::cout << "Operations performed: " << numOperations << std::endl;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "AVL Tree with random values:" << std::endl;
            std::cout << " - Insertion time: " << msRandom.count() << " ms" << std::endl;
            std::cout << " - Final height: " << avlTreeRandom.getHeight() << std::endl;
            std::cout << " - Height bound: " << 1.44 * std::log2(numOperations + 2) - 0.328 << std::endl;

            std::cout << "AVL Tree with sequential values:" << std::endl;
            std::cout << " - Insertion time: " << msSequential.count() << " ms" << std::endl;
            std::cout << " - Final height: " << avlTreeSequential.getHeight() << std::endl;

            std::cout << "Regular BST with sequential values (estimated):" << std::endl;
            std::cout << " - Insertion time: " << msBSTSequential << " ms" << std::endl;
            std::cout << " - Final height: approximately " << numOperations << std::endl;

            // Comparison
            std::cout << "\nComparison:" << std::endl;
            std::cout << "AVL vs BST height ratio: "
                      << static_cast<double>(avlTreeSequential.getHeight()) / numOperations
                      << std::endl;
            std::cout << "AVL vs BST time ratio with sequential data: "
                      << msSequential.count() / msBSTSequential
                      << std::endl;
        }
    }
};

/**
 * @brief Demonstrates AVL Tree operations
 */
void demonstrateAVLTree()
{
    AVLTree<int> avlTree;

    std::cout << "===== AVL Tree Demonstration =====" << std::endl;

    // Insert some values
    std::cout << "\nInserting values: 10, 20, 30, 40, 50, 25, 15" << std::endl;
    avlTree.insert(10);
    avlTree.insert(20);
    avlTree.insert(30);
    avlTree.insert(40);
    avlTree.insert(50);
    avlTree.insert(25);
    avlTree.insert(15);

    // Print the tree structure
    avlTree.printTree();

    // Print traversals
    std::cout << "\nIn-order traversal: ";
    for (int val : avlTree.inorderTraversal())
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Pre-order traversal: ";
    for (int val : avlTree.preorderTraversal())
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Post-order traversal: ";
    for (int val : avlTree.postorderTraversal())
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Level-order traversal: ";
    for (int val : avlTree.levelOrderTraversal())
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Search for values
    std::cout << "\nSearching:" << std::endl;
    std::cout << "Is 20 in the tree? " << (avlTree.search(20) ? "Yes" : "No") << std::endl;
    std::cout << "Is 100 in the tree? " << (avlTree.search(100) ? "Yes" : "No") << std::endl;

    // Min and max values
    std::cout << "\nMin value: " << avlTree.findMin() << std::endl;
    std::cout << "Max value: " << avlTree.findMax() << std::endl;

    // Range query
    std::cout << "\nValues between 15 and 40: ";
    for (int val : avlTree.rangeQuery(15, 40))
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Delete values
    std::cout << "\nDeleting 30" << std::endl;
    avlTree.remove(30);
    avlTree.printTree();

    std::cout << "\nDeleting 10 (root)" << std::endl;
    avlTree.remove(10);
    avlTree.printTree();

    // Tree properties
    std::cout << "\nTree height: " << avlTree.getHeight() << std::endl;
    std::cout << "Tree size: " << avlTree.size() << std::endl;
    std::cout << "Is valid AVL tree? " << (avlTree.isValidAVLTree() ? "Yes" : "No") << std::endl;

    // Build from sorted array
    std::cout << "\nBuilding tree from sorted array: [1, 2, 3, 4, 5, 6, 7]" << std::endl;
    std::vector<int> sortedArray = {1, 2, 3, 4, 5, 6, 7};
    avlTree.buildFromSortedArray(sortedArray);
    avlTree.printTree();

    std::cout << "Height after building from sorted array: " << avlTree.getHeight() << std::endl;
    std::cout << "Theoretical height bound: " << 1.44 * std::log2(sortedArray.size() + 2) - 0.328 << std::endl;
}

/**
 * @brief Shows difference between AVL Tree and regular BST with sequential insertions
 */
void demonstrateAVLvsRegularBST()
{
    std::cout << "===== AVL Tree vs Regular BST =====" << std::endl;

    // Create AVL Tree
    AVLTree<int> avlTree;

    // Insert sequential values (this would create a skewed tree in a regular BST)
    std::cout << "\nInserting sequential values: 1 through 7" << std::endl;
    for (int i = 1; i <= 7; ++i)
    {
        avlTree.insert(i);
    }

    // Print the AVL tree structure
    std::cout << "\nAVL Tree after sequential insertions:" << std::endl;
    avlTree.printTree();
    std::cout << "AVL Tree height: " << avlTree.getHeight() << std::endl;

    // Simulate what a regular BST would look like
    std::cout << "\nRegular BST after same sequential insertions (simulated):" << std::endl;
    std::cout << "└── 1" << std::endl;
    std::cout << "    └── 2" << std::endl;
    std::cout << "        └── 3" << std::endl;
    std::cout << "            └── 4" << std::endl;
    std::cout << "                └── 5" << std::endl;
    std::cout << "                    └── 6" << std::endl;
    std::cout << "                        └── 7" << std::endl;
    std::cout << "Regular BST height: 7" << std::endl;

    std::cout << "\nConclusion:" << std::endl;
    std::cout << "AVL Trees maintain O(log n) height even with unfavorable insertion patterns," << std::endl;
    std::cout << "while regular BSTs can degenerate to O(n) height, resulting in poor performance." << std::endl;
}

/**
 * @brief Demonstrates the four rotation cases in AVL Trees
 */
void demonstrateRotations()
{
    std::cout << "===== AVL Tree Rotation Demonstration =====" << std::endl;

    // Left-Left case (requires right rotation)
    {
        std::cout << "\n1. Left-Left Case (Right Rotation):" << std::endl;
        AVLTree<int> avlTree;
        avlTree.insert(30);
        avlTree.insert(20);
        avlTree.insert(10); // This will trigger a right rotation
        avlTree.printTreeWithBalanceFactors();
    }

    // Right-Right case (requires left rotation)
    {
        std::cout << "\n2. Right-Right Case (Left Rotation):" << std::endl;
        AVLTree<int> avlTree;
        avlTree.insert(10);
        avlTree.insert(20);
        avlTree.insert(30); // This will trigger a left rotation
        avlTree.printTreeWithBalanceFactors();
    }

    // Left-Right case (requires left-right rotation)
    {
        std::cout << "\n3. Left-Right Case (Left-Right Rotation):" << std::endl;
        AVLTree<int> avlTree;
        avlTree.insert(30);
        avlTree.insert(10);
        avlTree.insert(20); // This will trigger a left-right rotation
        avlTree.printTreeWithBalanceFactors();
    }

    // Right-Left case (requires right-left rotation)
    {
        std::cout << "\n4. Right-Left Case (Right-Left Rotation):" << std::endl;
        AVLTree<int> avlTree;
        avlTree.insert(10);
        avlTree.insert(30);
        avlTree.insert(20); // This will trigger a right-left rotation
        avlTree.printTreeWithBalanceFactors();
    }
}

/**
 * @brief Main function to execute the demonstrations
 */
int main()
{
    // Basic demonstration of AVL Tree operations
    demonstrateAVLTree();

    // Compare AVL Tree with regular BST
    demonstrateAVLvsRegularBST();

    // Demonstrate the four rotation cases
    demonstrateRotations();

    // Benchmark AVL Tree operations (uncomment for benchmarking)
    // AVLTree<int>::benchmark(100000);  // Use smaller value for faster execution

    return 0;
}