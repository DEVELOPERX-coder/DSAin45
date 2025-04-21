#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <sstream>
#include <climits>
#include <unordered_map>
#include <functional>
#include <iomanip>

/**
 * @file BinaryTrees.cpp
 * @brief Comprehensive implementation of Binary Trees in C++
 *
 * This file contains implementations of:
 * 1. Basic Binary Tree structure and operations
 * 2. Various traversal methods (in-order, pre-order, post-order, level-order)
 * 3. Common binary tree operations (height, diameter, etc.)
 * 4. Various binary tree construction techniques
 * 5. Solutions to common binary tree problems
 *
 * @author #DSAin45 Series
 * @date Day 22
 */

// ============================================================================
// Basic Binary Tree Node Structure
// ============================================================================

/**
 * Definition for a binary tree node
 */
struct TreeNode
{
    int val;         // The value stored in the node
    TreeNode *left;  // Pointer to the left child
    TreeNode *right; // Pointer to the right child

    // Constructor with default values
    TreeNode(int x = 0) : val(x), left(nullptr), right(nullptr) {}

    // Constructor with left and right child pointers
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// ============================================================================
// Binary Tree Helper Functions
// ============================================================================

/**
 * Print a level-order representation of a binary tree
 *
 * @param root The root of the binary tree
 */
void printBinaryTree(TreeNode *root)
{
    if (root == nullptr)
    {
        std::cout << "Empty tree" << std::endl;
        return;
    }

    std::queue<TreeNode *> q;
    q.push(root);

    std::cout << "Level-order traversal: ";

    while (!q.empty())
    {
        TreeNode *current = q.front();
        q.pop();

        if (current == nullptr)
        {
            std::cout << "null ";
        }
        else
        {
            std::cout << current->val << " ";

            // Add both children to the queue, even if they're null
            // This helps visualize the structure better
            q.push(current->left);
            q.push(current->right);
        }
    }

    std::cout << std::endl;
}

/**
 * Print a visual representation of a binary tree (more human-readable)
 *
 * @param root The root of the binary tree
 * @param prefix The prefix to use for the current line
 * @param isLeft Whether the current node is a left child
 */
void printTreeVisual(TreeNode *root, const std::string &prefix = "", bool isLeft = true)
{
    if (root == nullptr)
    {
        std::cout << prefix << (isLeft ? "└── " : "┌── ") << "null" << std::endl;
        return;
    }

    // Print the current node
    std::cout << prefix << (isLeft ? "└── " : "┌── ") << root->val << std::endl;

    // Prepare the prefix for children
    std::string newPrefix = prefix + (isLeft ? "    " : "│   ");

    // Print right child first (will appear above left child)
    if (root->right != nullptr)
    {
        printTreeVisual(root->right, newPrefix, false);
    }
    else if (root->left != nullptr)
    {
        // Only print null for right if left exists (to maintain structure)
        std::cout << newPrefix << "┌── null" << std::endl;
    }

    // Print left child
    if (root->left != nullptr)
    {
        printTreeVisual(root->left, newPrefix, true);
    }
    else if (root->right != nullptr)
    {
        // Only print null for left if right exists (to maintain structure)
        std::cout << newPrefix << "└── null" << std::endl;
    }
}

/**
 * Create a binary tree from a level-order traversal represented as a vector
 *
 * @param values The level-order traversal of the tree (nullptr or a negative value represents null nodes)
 * @return The root of the constructed binary tree
 */
TreeNode *createBinaryTree(const std::vector<int *> &values)
{
    if (values.empty() || values[0] == nullptr)
    {
        return nullptr;
    }

    TreeNode *root = new TreeNode(*values[0]);
    std::queue<TreeNode *> q;
    q.push(root);

    size_t i = 1;
    while (!q.empty() && i < values.size())
    {
        TreeNode *current = q.front();
        q.pop();

        // Left child
        if (i < values.size() && values[i] != nullptr)
        {
            current->left = new TreeNode(*values[i]);
            q.push(current->left);
        }
        i++;

        // Right child
        if (i < values.size() && values[i] != nullptr)
        {
            current->right = new TreeNode(*values[i]);
            q.push(current->right);
        }
        i++;
    }

    return root;
}

/**
 * Helper function to create a tree from a vector of integers
 *
 * @param values The level-order traversal of the tree (negative values represent null nodes)
 * @return The root of the constructed binary tree
 */
TreeNode *createBinaryTree(const std::vector<int> &values)
{
    if (values.empty())
    {
        return nullptr;
    }

    TreeNode *root = new TreeNode(values[0]);
    std::queue<TreeNode *> q;
    q.push(root);

    size_t i = 1;
    while (!q.empty() && i < values.size())
    {
        TreeNode *current = q.front();
        q.pop();

        // Left child
        if (i < values.size() && values[i] != INT_MIN)
        {
            current->left = new TreeNode(values[i]);
            q.push(current->left);
        }
        i++;

        // Right child
        if (i < values.size() && values[i] != INT_MIN)
        {
            current->right = new TreeNode(values[i]);
            q.push(current->right);
        }
        i++;
    }

    return root;
}

/**
 * Create a binary tree manually with specific structure
 *
 * @return The root node of the sample tree
 */
TreeNode *createSampleTree()
{
    /*
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

/**
 * Delete a binary tree to free memory
 *
 * @param root The root of the binary tree to delete
 */
void deleteTree(TreeNode *root)
{
    if (root == nullptr)
    {
        return;
    }

    // Post-order deletion
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// ============================================================================
// Tree Traversal Implementations
// ============================================================================

namespace TreeTraversal
{

    /**
     * In-order traversal (Left-Root-Right)
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in in-order
     */
    std::vector<int> inorderTraversal(TreeNode *root)
    {
        std::vector<int> result;
        inorderHelper(root, result);
        return result;
    }

    /**
     * Helper function for recursive in-order traversal
     */
    void inorderHelper(TreeNode *root, std::vector<int> &result)
    {
        if (root == nullptr)
        {
            return;
        }

        inorderHelper(root->left, result);
        result.push_back(root->val);
        inorderHelper(root->right, result);
    }

    /**
     * Iterative in-order traversal implementation
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in in-order
     */
    std::vector<int> inorderTraversalIterative(TreeNode *root)
    {
        std::vector<int> result;
        if (root == nullptr)
        {
            return result;
        }

        std::stack<TreeNode *> s;
        TreeNode *current = root;

        while (current != nullptr || !s.empty())
        {
            // Reach the leftmost node of the current subtree
            while (current != nullptr)
            {
                s.push(current);
                current = current->left;
            }

            // Current is now nullptr, pop from stack
            current = s.top();
            s.pop();

            // Visit the node
            result.push_back(current->val);

            // Move to the right subtree
            current = current->right;
        }

        return result;
    }

    /**
     * Pre-order traversal (Root-Left-Right)
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in pre-order
     */
    std::vector<int> preorderTraversal(TreeNode *root)
    {
        std::vector<int> result;
        preorderHelper(root, result);
        return result;
    }

    /**
     * Helper function for recursive pre-order traversal
     */
    void preorderHelper(TreeNode *root, std::vector<int> &result)
    {
        if (root == nullptr)
        {
            return;
        }

        result.push_back(root->val);
        preorderHelper(root->left, result);
        preorderHelper(root->right, result);
    }

    /**
     * Iterative pre-order traversal implementation
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in pre-order
     */
    std::vector<int> preorderTraversalIterative(TreeNode *root)
    {
        std::vector<int> result;
        if (root == nullptr)
        {
            return result;
        }

        std::stack<TreeNode *> s;
        s.push(root);

        while (!s.empty())
        {
            TreeNode *current = s.top();
            s.pop();

            // Visit the current node
            result.push_back(current->val);

            // Push right child first so that left child is processed first (LIFO)
            if (current->right)
            {
                s.push(current->right);
            }
            if (current->left)
            {
                s.push(current->left);
            }
        }

        return result;
    }

    /**
     * Post-order traversal (Left-Right-Root)
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in post-order
     */
    std::vector<int> postorderTraversal(TreeNode *root)
    {
        std::vector<int> result;
        postorderHelper(root, result);
        return result;
    }

    /**
     * Helper function for recursive post-order traversal
     */
    void postorderHelper(TreeNode *root, std::vector<int> &result)
    {
        if (root == nullptr)
        {
            return;
        }

        postorderHelper(root->left, result);
        postorderHelper(root->right, result);
        result.push_back(root->val);
    }

    /**
     * Iterative post-order traversal implementation using two stacks
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in post-order
     */
    std::vector<int> postorderTraversalIterative(TreeNode *root)
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
     * Level-order traversal (Breadth-First Search)
     *
     * @param root The root of the binary tree
     * @return Vector containing node values in level-order
     */
    std::vector<int> levelOrderTraversal(TreeNode *root)
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
     * Level-order traversal that returns each level as a separate vector
     *
     * @param root The root of the binary tree
     * @return Vector of vectors containing node values by level
     */
    std::vector<std::vector<int>> levelOrderTraversalByLevel(TreeNode *root)
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

    /**
     * Zigzag level-order traversal (levels alternate left-to-right and right-to-left)
     *
     * @param root The root of the binary tree
     * @return Vector of vectors containing node values in zigzag level-order
     */
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode *root)
    {
        std::vector<std::vector<int>> result;
        if (root == nullptr)
        {
            return result;
        }

        std::queue<TreeNode *> q;
        q.push(root);
        bool leftToRight = true;

        while (!q.empty())
        {
            int levelSize = q.size();
            std::vector<int> currentLevel(levelSize);

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *current = q.front();
                q.pop();

                // If leftToRight is true, fill the level from left to right
                // Otherwise, fill it from right to left
                int index = leftToRight ? i : levelSize - 1 - i;
                currentLevel[index] = current->val;

                if (current->left)
                {
                    q.push(current->left);
                }
                if (current->right)
                {
                    q.push(current->right);
                }
            }

            // Toggle the direction for the next level
            leftToRight = !leftToRight;
            result.push_back(currentLevel);
        }

        return result;
    }

} // namespace TreeTraversal

// ============================================================================
// Basic Binary Tree Operations
// ============================================================================

namespace TreeOperations
{

    /**
     * Get the height/depth of a binary tree
     *
     * @param root The root of the binary tree
     * @return The height of the tree (-1 for empty tree)
     */
    int height(TreeNode *root)
    {
        if (root == nullptr)
        {
            return -1; // Height of an empty tree is -1
        }

        int leftHeight = height(root->left);
        int rightHeight = height(root->right);

        return std::max(leftHeight, rightHeight) + 1;
    }

    /**
     * Count the total number of nodes in a binary tree
     *
     * @param root The root of the binary tree
     * @return The number of nodes in the tree
     */
    int countNodes(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    /**
     * Count the number of leaf nodes in a binary tree
     *
     * @param root The root of the binary tree
     * @return The number of leaf nodes in the tree
     */
    int countLeafNodes(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        // If this is a leaf node
        if (root->left == nullptr && root->right == nullptr)
        {
            return 1;
        }

        return countLeafNodes(root->left) + countLeafNodes(root->right);
    }

    /**
     * Check if two binary trees are identical
     *
     * @param p First binary tree
     * @param q Second binary tree
     * @return True if the trees are identical, false otherwise
     */
    bool areIdentical(TreeNode *p, TreeNode *q)
    {
        // If both trees are empty, they're identical
        if (p == nullptr && q == nullptr)
        {
            return true;
        }

        // If one is empty but the other isn't, they're not identical
        if (p == nullptr || q == nullptr)
        {
            return false;
        }

        // Check if current nodes and both subtrees are identical
        return (p->val == q->val &&
                areIdentical(p->left, q->left) &&
                areIdentical(p->right, q->right));
    }

    /**
     * Find the diameter of a binary tree (longest path between any two nodes)
     *
     * @param root The root of the binary tree
     * @return The diameter of the tree
     */
    int diameterOfBinaryTree(TreeNode *root)
    {
        int diameter = 0;
        calculateHeight(root, diameter);
        return diameter;
    }

    /**
     * Helper function to calculate the height and update the diameter
     *
     * @param root The current node
     * @param diameter Reference to the current maximum diameter
     * @return The height of the subtree rooted at the current node
     */
    int calculateHeight(TreeNode *root, int &diameter)
    {
        if (root == nullptr)
        {
            return 0;
        }

        int leftHeight = calculateHeight(root->left, diameter);
        int rightHeight = calculateHeight(root->right, diameter);

        // Update diameter if needed
        diameter = std::max(diameter, leftHeight + rightHeight);

        // Return height of current subtree
        return std::max(leftHeight, rightHeight) + 1;
    }

    /**
     * Check if a binary tree is balanced (height difference between left and right subtrees <= 1)
     *
     * @param root The root of the binary tree
     * @return True if the tree is balanced, false otherwise
     */
    bool isBalanced(TreeNode *root)
    {
        return checkHeight(root) != -1;
    }

    /**
     * Helper function to check if a tree is balanced while calculating its height
     *
     * @param root The current node
     * @return The height of the subtree if balanced, -1 if unbalanced
     */
    int checkHeight(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        int leftHeight = checkHeight(root->left);
        if (leftHeight == -1)
        {
            return -1; // Left subtree is unbalanced
        }

        int rightHeight = checkHeight(root->right);
        if (rightHeight == -1)
        {
            return -1; // Right subtree is unbalanced
        }

        // Check if current node is balanced
        if (std::abs(leftHeight - rightHeight) > 1)
        {
            return -1; // Unbalanced
        }

        // Return height of current subtree
        return std::max(leftHeight, rightHeight) + 1;
    }

    /**
     * Check if a binary tree is symmetric around its center
     *
     * @param root The root of the binary tree
     * @return True if the tree is symmetric, false otherwise
     */
    bool isSymmetric(TreeNode *root)
    {
        if (root == nullptr)
        {
            return true;
        }

        return isMirror(root->left, root->right);
    }

    /**
     * Helper function to check if two subtrees are mirrors of each other
     *
     * @param left First subtree
     * @param right Second subtree
     * @return True if the subtrees are mirrors, false otherwise
     */
    bool isMirror(TreeNode *left, TreeNode *right)
    {
        // If both are null, they're symmetric
        if (left == nullptr && right == nullptr)
        {
            return true;
        }

        // If only one is null, they're not symmetric
        if (left == nullptr || right == nullptr)
        {
            return false;
        }

        // Check if values match and subtrees are mirrors
        return (left->val == right->val) &&
               isMirror(left->left, right->right) &&
               isMirror(left->right, right->left);
    }

    /**
     * Check if a binary tree is a valid binary search tree (BST)
     *
     * @param root The root of the binary tree
     * @return True if the tree is a valid BST, false otherwise
     */
    bool isValidBST(TreeNode *root)
    {
        return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
    }

    /**
     * Helper function to check if a tree is a valid BST within a range
     *
     * @param root The current node
     * @param min The minimum allowed value
     * @param max The maximum allowed value
     * @return True if the subtree is a valid BST, false otherwise
     */
    bool isValidBSTHelper(TreeNode *root, long min, long max)
    {
        if (root == nullptr)
        {
            return true;
        }

        // Check if current node's value is within the allowed range
        if (root->val <= min || root->val >= max)
        {
            return false;
        }

        // Check left and right subtrees with updated ranges
        return isValidBSTHelper(root->left, min, root->val) &&
               isValidBSTHelper(root->right, root->val, max);
    }

    /**
     * Find the lowest common ancestor (LCA) of two nodes in a binary tree
     *
     * @param root The root of the binary tree
     * @param p First node
     * @param q Second node
     * @return The lowest common ancestor node
     */
    TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
    {
        // Base case: if root is null or matches one of the nodes
        if (root == nullptr || root == p || root == q)
        {
            return root;
        }

        // Look for LCA in left and right subtrees
        TreeNode *leftLCA = lowestCommonAncestor(root->left, p, q);
        TreeNode *rightLCA = lowestCommonAncestor(root->right, p, q);

        // If both left and right returned non-null, root is the LCA
        if (leftLCA && rightLCA)
        {
            return root;
        }

        // Otherwise, return the non-null result
        return leftLCA ? leftLCA : rightLCA;
    }

    /**
     * Check if a binary tree is complete (all levels filled except possibly the last)
     *
     * @param root The root of the binary tree
     * @return True if the tree is complete, false otherwise
     */
    bool isCompleteTree(TreeNode *root)
    {
        if (root == nullptr)
        {
            return true;
        }

        std::queue<TreeNode *> q;
        q.push(root);
        bool seenNull = false;

        while (!q.empty())
        {
            TreeNode *current = q.front();
            q.pop();

            // If we see a null node
            if (current == nullptr)
            {
                seenNull = true;
            }
            else
            {
                // If we've already seen a null node and now we see a non-null node
                // The tree is not complete
                if (seenNull)
                {
                    return false;
                }

                // Add both children (including nulls) to the queue
                q.push(current->left);
                q.push(current->right);
            }
        }

        return true;
    }

    /**
     * Check if a binary tree is a full tree (all nodes have 0 or 2 children)
     *
     * @param root The root of the binary tree
     * @return True if the tree is full, false otherwise
     */
    bool isFullTree(TreeNode *root)
    {
        // An empty tree is considered full
        if (root == nullptr)
        {
            return true;
        }

        // If a node has no children, it's a leaf
        if (root->left == nullptr && root->right == nullptr)
        {
            return true;
        }

        // If a node has both children, check if both subtrees are full
        if (root->left != nullptr && root->right != nullptr)
        {
            return isFullTree(root->left) && isFullTree(root->right);
        }

        // If we get here, the node has only one child
        return false;
    }

    /**
     * Check if a binary tree is a perfect tree (all internal nodes have 2 children and all leaves are at the same level)
     *
     * @param root The root of the binary tree
     * @return True if the tree is perfect, false otherwise
     */
    bool isPerfectTree(TreeNode *root)
    {
        int h = height(root);
        return isPerfectTreeHelper(root, h, 0);
    }

    /**
     * Helper function to check if a tree is perfect
     *
     * @param root The current node
     * @param height The height of the tree
     * @param level The current level
     * @return True if the subtree is perfect, false otherwise
     */
    bool isPerfectTreeHelper(TreeNode *root, int height, int level)
    {
        // An empty tree is perfect
        if (root == nullptr)
        {
            return true;
        }

        // If it's a leaf node, it must be at the maximum level
        if (root->left == nullptr && root->right == nullptr)
        {
            return level == height;
        }

        // If an internal node doesn't have both children, it's not perfect
        if (root->left == nullptr || root->right == nullptr)
        {
            return false;
        }

        // Recursively check both subtrees
        return isPerfectTreeHelper(root->left, height, level + 1) &&
               isPerfectTreeHelper(root->right, height, level + 1);
    }

} // namespace TreeOperations

// ============================================================================
// Tree Construction and Modification
// ============================================================================

namespace TreeConstruction
{

    /**
     * Create a binary tree from inorder and preorder traversals
     *
     * @param preorder The preorder traversal of the tree
     * @param inorder The inorder traversal of the tree
     * @return The root of the constructed binary tree
     */
    TreeNode *buildTreeFromPreorderAndInorder(std::vector<int> &preorder, std::vector<int> &inorder)
    {
        return buildTreeHelper(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }

    /**
     * Helper function to build a tree from preorder and inorder traversals
     */
    TreeNode *buildTreeHelper(std::vector<int> &preorder, int preStart, int preEnd,
                              std::vector<int> &inorder, int inStart, int inEnd)
    {
        if (preStart > preEnd || inStart > inEnd)
        {
            return nullptr;
        }

        // The first element in preorder is the root
        int rootValue = preorder[preStart];
        TreeNode *root = new TreeNode(rootValue);

        // Find the position of the root in inorder
        int rootIndex = inStart;
        for (int i = inStart; i <= inEnd; i++)
        {
            if (inorder[i] == rootValue)
            {
                rootIndex = i;
                break;
            }
        }

        // Calculate the size of the left subtree
        int leftSize = rootIndex - inStart;

        // Recursively build the left and right subtrees
        root->left = buildTreeHelper(preorder, preStart + 1, preStart + leftSize,
                                     inorder, inStart, rootIndex - 1);
        root->right = buildTreeHelper(preorder, preStart + leftSize + 1, preEnd,
                                      inorder, rootIndex + 1, inEnd);

        return root;
    }

    /**
     * Create a binary tree from inorder and postorder traversals
     *
     * @param inorder The inorder traversal of the tree
     * @param postorder The postorder traversal of the tree
     * @return The root of the constructed binary tree
     */
    TreeNode *buildTreeFromInorderAndPostorder(std::vector<int> &inorder, std::vector<int> &postorder)
    {
        return buildTreePostorderHelper(inorder, 0, inorder.size() - 1,
                                        postorder, 0, postorder.size() - 1);
    }

    /**
     * Helper function to build a tree from inorder and postorder traversals
     */
    TreeNode *buildTreePostorderHelper(std::vector<int> &inorder, int inStart, int inEnd,
                                       std::vector<int> &postorder, int postStart, int postEnd)
    {
        if (inStart > inEnd || postStart > postEnd)
        {
            return nullptr;
        }

        // The last element in postorder is the root
        int rootValue = postorder[postEnd];
        TreeNode *root = new TreeNode(rootValue);

        // Find the position of the root in inorder
        int rootIndex = inStart;
        for (int i = inStart; i <= inEnd; i++)
        {
            if (inorder[i] == rootValue)
            {
                rootIndex = i;
                break;
            }
        }

        // Calculate the size of the left subtree
        int leftSize = rootIndex - inStart;

        // Recursively build the left and right subtrees
        root->left = buildTreePostorderHelper(inorder, inStart, rootIndex - 1,
                                              postorder, postStart, postStart + leftSize - 1);
        root->right = buildTreePostorderHelper(inorder, rootIndex + 1, inEnd,
                                               postorder, postStart + leftSize, postEnd - 1);

        return root;
    }

    /**
     * Create a complete binary tree from an array
     *
     * @param nums The array of node values
     * @return The root of the constructed complete binary tree
     */
    TreeNode *createCompleteBinaryTree(const std::vector<int> &nums)
    {
        if (nums.empty())
        {
            return nullptr;
        }

        return createCompleteBinaryTreeHelper(nums, 0);
    }

    /**
     * Helper function to create a complete binary tree from an array
     */
    TreeNode *createCompleteBinaryTreeHelper(const std::vector<int> &nums, int index)
    {
        if (index >= nums.size())
        {
            return nullptr;
        }

        TreeNode *root = new TreeNode(nums[index]);

        // Calculate indices for left and right children
        int leftIndex = 2 * index + 1;
        int rightIndex = 2 * index + 2;

        // Recursively create left and right subtrees
        root->left = createCompleteBinaryTreeHelper(nums, leftIndex);
        root->right = createCompleteBinaryTreeHelper(nums, rightIndex);

        return root;
    }

    /**
     * Create a balanced binary search tree from a sorted array
     *
     * @param nums The sorted array of node values
     * @return The root of the constructed balanced BST
     */
    TreeNode *sortedArrayToBST(std::vector<int> &nums)
    {
        return sortedArrayToBSTHelper(nums, 0, nums.size() - 1);
    }

    /**
     * Helper function to create a balanced BST from a sorted array
     */
    TreeNode *sortedArrayToBSTHelper(std::vector<int> &nums, int start, int end)
    {
        if (start > end)
        {
            return nullptr;
        }

        // Always choose the middle element as the root
        int mid = start + (end - start) / 2;
        TreeNode *root = new TreeNode(nums[mid]);

        // Recursively create left and right subtrees
        root->left = sortedArrayToBSTHelper(nums, start, mid - 1);
        root->right = sortedArrayToBSTHelper(nums, mid + 1, end);

        return root;
    }

    /**
     * Serialize a binary tree to a string
     *
     * @param root The root of the binary tree
     * @return The serialized string representation of the tree
     */
    std::string serialize(TreeNode *root)
    {
        if (root == nullptr)
        {
            return "null,";
        }

        std::string result = std::to_string(root->val) + ",";
        result += serialize(root->left);
        result += serialize(root->right);

        return result;
    }

    /**
     * Deserialize a string to a binary tree
     *
     * @param data The serialized string representation of the tree
     * @return The root of the deserialized binary tree
     */
    TreeNode *deserialize(std::string data)
    {
        // Parse the string into a queue of values
        std::queue<std::string> nodes;
        std::string val;

        for (char c : data)
        {
            if (c == ',')
            {
                nodes.push(val);
                val = "";
            }
            else
            {
                val += c;
            }
        }

        // If there's a remaining value, add it to the queue
        if (!val.empty())
        {
            nodes.push(val);
        }

        return deserializeHelper(nodes);
    }

    /**
     * Helper function for deserialization
     */
    TreeNode *deserializeHelper(std::queue<std::string> &nodes)
    {
        if (nodes.empty())
        {
            return nullptr;
        }

        std::string val = nodes.front();
        nodes.pop();

        if (val == "null")
        {
            return nullptr;
        }

        TreeNode *root = new TreeNode(std::stoi(val));
        root->left = deserializeHelper(nodes);
        root->right = deserializeHelper(nodes);

        return root;
    }

    /**
     * Clone a binary tree
     *
     * @param root The root of the binary tree to clone
     * @return The root of the cloned binary tree
     */
    TreeNode *cloneTree(TreeNode *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        TreeNode *newRoot = new TreeNode(root->val);
        newRoot->left = cloneTree(root->left);
        newRoot->right = cloneTree(root->right);

        return newRoot;
    }

    /**
     * Invert a binary tree (mirror it)
     *
     * @param root The root of the binary tree to invert
     * @return The root of the inverted binary tree
     */
    TreeNode *invertTree(TreeNode *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        // Swap the left and right children
        TreeNode *temp = root->left;
        root->left = root->right;
        root->right = temp;

        // Recursively invert the subtrees
        invertTree(root->left);
        invertTree(root->right);

        return root;
    }

    /**
     * Flatten a binary tree to a linked list (in-place)
     * The "linked list" should use the same TreeNode structure where the right child pointer
     * points to the next node in the list and the left child pointer is always null.
     *
     * @param root The root of the binary tree to flatten
     */
    void flatten(TreeNode *root)
    {
        if (root == nullptr)
        {
            return;
        }

        // Store the original left and right subtrees
        TreeNode *leftSubtree = root->left;
        TreeNode *rightSubtree = root->right;

        // Flatten left and right subtrees
        flatten(leftSubtree);
        flatten(rightSubtree);

        // Make left subtree null
        root->left = nullptr;

        // Make left subtree the new right subtree
        root->right = leftSubtree;

        // Find the end of the flattened left subtree
        TreeNode *current = root;
        while (current->right != nullptr)
        {
            current = current->right;
        }

        // Attach the flattened right subtree
        current->right = rightSubtree;
    }

} // namespace TreeConstruction

// ============================================================================
// Common Binary Tree Problems and Solutions
// ============================================================================

namespace TreeProblems
{

    /**
     * Maximum depth of a binary tree
     *
     * @param root The root of the binary tree
     * @return The maximum depth of the tree
     */
    int maxDepth(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);

        return std::max(leftDepth, rightDepth) + 1;
    }

    /**
     * Minimum depth of a binary tree
     *
     * @param root The root of the binary tree
     * @return The minimum depth of the tree
     */
    int minDepth(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        // If one of the subtrees is empty, return the depth of the non-empty subtree
        if (root->left == nullptr)
        {
            return 1 + minDepth(root->right);
        }
        if (root->right == nullptr)
        {
            return 1 + minDepth(root->left);
        }

        // Both subtrees are non-empty, return the minimum depth
        return 1 + std::min(minDepth(root->left), minDepth(root->right));
    }

    /**
     * Find the sum of all node values in a binary tree
     *
     * @param root The root of the binary tree
     * @return The sum of all node values
     */
    int sumOfNodes(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        return root->val + sumOfNodes(root->left) + sumOfNodes(root->right);
    }

    /**
     * Check if a given sum exists as a path from root to leaf
     *
     * @param root The root of the binary tree
     * @param sum The target sum
     * @return True if there is a root-to-leaf path with the given sum, false otherwise
     */
    bool hasPathSum(TreeNode *root, int sum)
    {
        if (root == nullptr)
        {
            return false;
        }

        // If it's a leaf node, check if the remaining sum equals the node's value
        if (root->left == nullptr && root->right == nullptr)
        {
            return sum == root->val;
        }

        // Check if either left or right subtree has a path with the remaining sum
        return hasPathSum(root->left, sum - root->val) ||
               hasPathSum(root->right, sum - root->val);
    }

    /**
     * Find all root-to-leaf paths with a given sum
     *
     * @param root The root of the binary tree
     * @param sum The target sum
     * @return A vector of vectors containing the paths
     */
    std::vector<std::vector<int>> pathSum(TreeNode *root, int sum)
    {
        std::vector<std::vector<int>> paths;
        std::vector<int> currentPath;

        pathSumHelper(root, sum, currentPath, paths);

        return paths;
    }

    /**
     * Helper function for finding all root-to-leaf paths with a given sum
     */
    void pathSumHelper(TreeNode *root, int remainingSum, std::vector<int> &currentPath,
                       std::vector<std::vector<int>> &paths)
    {
        if (root == nullptr)
        {
            return;
        }

        // Add current node to the path
        currentPath.push_back(root->val);

        // Check if it's a leaf node and the path sum equals the target
        if (root->left == nullptr && root->right == nullptr && remainingSum == root->val)
        {
            paths.push_back(currentPath);
        }
        else
        {
            // Continue DFS on both subtrees
            pathSumHelper(root->left, remainingSum - root->val, currentPath, paths);
            pathSumHelper(root->right, remainingSum - root->val, currentPath, paths);
        }

        // Backtrack by removing the current node from the path
        currentPath.pop_back();
    }

    /**
     * Find the maximum path sum in a binary tree
     *
     * @param root The root of the binary tree
     * @return The maximum path sum
     */
    int maxPathSum(TreeNode *root)
    {
        int maxSum = INT_MIN;
        maxPathSumHelper(root, maxSum);
        return maxSum;
    }

    /**
     * Helper function for finding the maximum path sum
     */
    int maxPathSumHelper(TreeNode *root, int &maxSum)
    {
        if (root == nullptr)
        {
            return 0;
        }

        // Calculate the maximum path sum from left and right subtrees
        // If negative, we can choose not to include that path by taking 0
        int leftMax = std::max(0, maxPathSumHelper(root->left, maxSum));
        int rightMax = std::max(0, maxPathSumHelper(root->right, maxSum));

        // The maximum path sum that passes through the current node
        int currentPathSum = root->val + leftMax + rightMax;

        // Update the global maximum
        maxSum = std::max(maxSum, currentPathSum);

        // Return the maximum sum of a path that can be extended to parent
        // (can only include one subtree path)
        return root->val + std::max(leftMax, rightMax);
    }

    /**
     * Find the right view of a binary tree
     * (nodes visible when looking at the tree from the right side)
     *
     * @param root The root of the binary tree
     * @return A vector of node values visible from the right side
     */
    std::vector<int> rightSideView(TreeNode *root)
    {
        std::vector<int> rightView;
        if (root == nullptr)
        {
            return rightView;
        }

        std::queue<TreeNode *> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *current = q.front();
                q.pop();

                // If it's the last node in the current level, add it to the right view
                if (i == levelSize - 1)
                {
                    rightView.push_back(current->val);
                }

                if (current->left)
                {
                    q.push(current->left);
                }
                if (current->right)
                {
                    q.push(current->right);
                }
            }
        }

        return rightView;
    }

    /**
     * Find the left view of a binary tree
     * (nodes visible when looking at the tree from the left side)
     *
     * @param root The root of the binary tree
     * @return A vector of node values visible from the left side
     */
    std::vector<int> leftSideView(TreeNode *root)
    {
        std::vector<int> leftView;
        if (root == nullptr)
        {
            return leftView;
        }

        std::queue<TreeNode *> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *current = q.front();
                q.pop();

                // If it's the first node in the current level, add it to the left view
                if (i == 0)
                {
                    leftView.push_back(current->val);
                }

                if (current->left)
                {
                    q.push(current->left);
                }
                if (current->right)
                {
                    q.push(current->right);
                }
            }
        }

        return leftView;
    }

    /**
     * Find the vertical order traversal of a binary tree
     *
     * @param root The root of the binary tree
     * @return A vector of vectors containing node values by vertical level
     */
    std::vector<std::vector<int>> verticalOrder(TreeNode *root)
    {
        std::vector<std::vector<int>> result;
        if (root == nullptr)
        {
            return result;
        }

        // Map to store nodes at each vertical level (column)
        std::map<int, std::vector<int>> columnMap;

        // Queue for level order traversal with node and its column
        std::queue<std::pair<TreeNode *, int>> q;
        q.push({root, 0});

        while (!q.empty())
        {
            auto [node, column] = q.front();
            q.pop();

            // Add the current node to its column
            columnMap[column].push_back(node->val);

            // Add left child with column - 1
            if (node->left)
            {
                q.push({node->left, column - 1});
            }

            // Add right child with column + 1
            if (node->right)
            {
                q.push({node->right, column + 1});
            }
        }

        // Convert map to result vector
        for (const auto &[column, nodes] : columnMap)
        {
            result.push_back(nodes);
        }

        return result;
    }

    /**
     * Check if a binary tree is cousins (same level but different parents)
     *
     * @param root The root of the binary tree
     * @param x First node value
     * @param y Second node value
     * @return True if x and y are cousins, false otherwise
     */
    bool isCousins(TreeNode *root, int x, int y)
    {
        if (root == nullptr)
        {
            return false;
        }

        std::queue<TreeNode *> q;
        q.push(root);

        while (!q.empty())
        {
            int levelSize = q.size();
            bool foundX = false;
            bool foundY = false;

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *current = q.front();
                q.pop();

                // Check if current node's children are x and y
                // If they are, they're siblings, not cousins
                if (current->left && current->right)
                {
                    if ((current->left->val == x && current->right->val == y) ||
                        (current->left->val == y && current->right->val == x))
                    {
                        return false;
                    }
                }

                // Check if current node is x or y
                if (current->val == x)
                {
                    foundX = true;
                }
                if (current->val == y)
                {
                    foundY = true;
                }

                // Add children to the queue
                if (current->left)
                {
                    q.push(current->left);
                }
                if (current->right)
                {
                    q.push(current->right);
                }
            }

            // If both x and y were found at this level, they're cousins
            if (foundX && foundY)
            {
                return true;
            }

            // If only one of them was found, they're at different levels
            if (foundX || foundY)
            {
                return false;
            }
        }

        return false;
    }

    /**
     * Find all nodes at distance k from a target node
     *
     * @param root The root of the binary tree
     * @param target The target node
     * @param k The distance
     * @return A vector of node values at distance k from the target
     */
    std::vector<int> distanceK(TreeNode *root, TreeNode *target, int k)
    {
        std::vector<int> result;
        if (root == nullptr || target == nullptr)
        {
            return result;
        }

        // Map to store parent pointers for each node
        std::unordered_map<TreeNode *, TreeNode *> parentMap;
        markParents(root, parentMap);

        // Set to keep track of visited nodes
        std::unordered_set<TreeNode *> visited;

        // Queue for BFS
        std::queue<TreeNode *> q;
        q.push(target);
        visited.insert(target);

        int distance = 0;

        while (!q.empty() && distance <= k)
        {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *current = q.front();
                q.pop();

                // If we've reached distance k, add current node to result
                if (distance == k)
                {
                    result.push_back(current->val);
                }

                // Add unvisited left child
                if (current->left && visited.find(current->left) == visited.end())
                {
                    q.push(current->left);
                    visited.insert(current->left);
                }

                // Add unvisited right child
                if (current->right && visited.find(current->right) == visited.end())
                {
                    q.push(current->right);
                    visited.insert(current->right);
                }

                // Add unvisited parent
                if (parentMap.find(current) != parentMap.end() &&
                    visited.find(parentMap[current]) == visited.end())
                {
                    q.push(parentMap[current]);
                    visited.insert(parentMap[current]);
                }
            }

            distance++;
        }

        return result;
    }

    /**
     * Helper function to mark parent pointers for each node
     */
    void markParents(TreeNode *root, std::unordered_map<TreeNode *, TreeNode *> &parentMap)
    {
        if (root == nullptr)
        {
            return;
        }

        // Process left subtree
        if (root->left)
        {
            parentMap[root->left] = root;
            markParents(root->left, parentMap);
        }

        // Process right subtree
        if (root->right)
        {
            parentMap[root->right] = root;
            markParents(root->right, parentMap);
        }
    }

    /**
     * Count good nodes in a binary tree
     * (a node is good if no node on the path from root to it has a greater value)
     *
     * @param root The root of the binary tree
     * @return The number of good nodes
     */
    int goodNodes(TreeNode *root)
    {
        return goodNodesHelper(root, INT_MIN);
    }

    /**
     * Helper function to count good nodes
     */
    int goodNodesHelper(TreeNode *root, int maxSoFar)
    {
        if (root == nullptr)
        {
            return 0;
        }

        int count = 0;

        // If current node is greater than or equal to the maximum value seen so far,
        // it's a good node
        if (root->val >= maxSoFar)
        {
            count = 1;
            maxSoFar = root->val;
        }

        // Recursively check left and right subtrees
        count += goodNodesHelper(root->left, maxSoFar);
        count += goodNodesHelper(root->right, maxSoFar);

        return count;
    }

    /**
     * Check if a binary tree is a subtree of another binary tree
     *
     * @param root The root of the main tree
     * @param subRoot The root of the potential subtree
     * @return True if subRoot is a subtree of root, false otherwise
     */
    bool isSubtree(TreeNode *root, TreeNode *subRoot)
    {
        if (root == nullptr)
        {
            return subRoot == nullptr;
        }

        if (subRoot == nullptr)
        {
            return true;
        }

        // Check if the current trees are identical
        if (TreeOperations::areIdentical(root, subRoot))
        {
            return true;
        }

        // Recursively check left and right subtrees
        return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
    }

} // namespace TreeProblems

// ============================================================================
// Alternative Tree Representations
// ============================================================================

/**
 * Array-based binary tree implementation
 */
class ArrayBinaryTree
{
private:
    std::vector<int> tree;

public:
    /**
     * Constructor with root value
     */
    ArrayBinaryTree(int root)
    {
        tree.push_back(root);
    }

    /**
     * Set the left child of a node
     *
     * @param parentIndex The index of the parent node
     * @param value The value of the left child
     */
    void setLeft(int parentIndex, int value)
    {
        int leftIndex = 2 * parentIndex + 1;

        // Ensure the array is big enough
        if (leftIndex >= tree.size())
        {
            tree.resize(leftIndex + 1, -1); // Use -1 to represent empty spots
        }

        tree[leftIndex] = value;
    }

    /**
     * Set the right child of a node
     *
     * @param parentIndex The index of the parent node
     * @param value The value of the right child
     */
    void setRight(int parentIndex, int value)
    {
        int rightIndex = 2 * parentIndex + 2;

        // Ensure the array is big enough
        if (rightIndex >= tree.size())
        {
            tree.resize(rightIndex + 1, -1);
        }

        tree[rightIndex] = value;
    }

    /**
     * Get the value at a specific index
     *
     * @param index The index to get the value from
     * @return The value at the specified index, or -1 if out of bounds or empty
     */
    int getValue(int index)
    {
        if (index < 0 || index >= tree.size())
        {
            return -1;
        }

        return tree[index];
    }

    /**
     * Print the tree
     */
    void print()
    {
        for (int i = 0; i < tree.size(); i++)
        {
            if (tree[i] != -1)
            {
                std::cout << "Index " << i << ": " << tree[i] << std::endl;
            }
        }
    }

    /**
     * Get the left child index of a node
     *
     * @param parentIndex The index of the parent node
     * @return The index of the left child
     */
    int getLeftIndex(int parentIndex)
    {
        return 2 * parentIndex + 1;
    }

    /**
     * Get the right child index of a node
     *
     * @param parentIndex The index of the parent node
     * @return The index of the right child
     */
    int getRightIndex(int parentIndex)
    {
        return 2 * parentIndex + 2;
    }

    /**
     * Get the parent index of a node
     *
     * @param childIndex The index of the child node
     * @return The index of the parent, or -1 if it's the root
     */
    int getParentIndex(int childIndex)
    {
        if (childIndex <= 0)
        {
            return -1;
        }

        return (childIndex - 1) / 2;
    }
};

/**
 * Multi-way tree node structure (left-child right-sibling representation)
 */
struct MultiWayTreeNode
{
    int val;
    MultiWayTreeNode *firstChild;  // Points to the first (leftmost) child
    MultiWayTreeNode *nextSibling; // Points to the next sibling

    // Constructor
    MultiWayTreeNode(int x) : val(x), firstChild(nullptr), nextSibling(nullptr) {}
};

/**
 * Multi-way tree implementation using left-child right-sibling representation
 */
class MultiWayTree
{
private:
    MultiWayTreeNode *root;

    // Helper function to delete the tree
    void deleteTreeHelper(MultiWayTreeNode *node)
    {
        if (node == nullptr)
        {
            return;
        }

        // Delete all siblings
        deleteTreeHelper(node->nextSibling);

        // Delete all children
        deleteTreeHelper(node->firstChild);

        // Delete the node itself
        delete node;
    }

public:
    /**
     * Constructor
     */
    MultiWayTree() : root(nullptr) {}

    /**
     * Constructor with root value
     */
    MultiWayTree(int rootValue)
    {
        root = new MultiWayTreeNode(rootValue);
    }

    /**
     * Destructor
     */
    ~MultiWayTree()
    {
        deleteTreeHelper(root);
    }

    /**
     * Add a child to a parent node
     *
     * @param parentValue The value of the parent node
     * @param childValue The value of the child node to add
     * @return True if the child was added successfully, false otherwise
     */
    bool addChild(int parentValue, int childValue)
    {
        if (root == nullptr)
        {
            return false;
        }

        // Find the parent node
        MultiWayTreeNode *parent = findNode(root, parentValue);
        if (parent == nullptr)
        {
            return false;
        }

        // Create the new child node
        MultiWayTreeNode *child = new MultiWayTreeNode(childValue);

        // If the parent has no children, make this the first child
        if (parent->firstChild == nullptr)
        {
            parent->firstChild = child;
        }
        else
        {
            // Otherwise, add it to the end of the sibling list
            MultiWayTreeNode *sibling = parent->firstChild;
            while (sibling->nextSibling != nullptr)
            {
                sibling = sibling->nextSibling;
            }
            sibling->nextSibling = child;
        }

        return true;
    }

    /**
     * Find a node with a specific value
     *
     * @param node The root of the subtree to search
     * @param value The value to search for
     * @return Pointer to the node if found, nullptr otherwise
     */
    MultiWayTreeNode *findNode(MultiWayTreeNode *node, int value)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        // Check if current node matches
        if (node->val == value)
        {
            return node;
        }

        // Check children
        MultiWayTreeNode *found = findNode(node->firstChild, value);
        if (found != nullptr)
        {
            return found;
        }

        // Check siblings
        return findNode(node->nextSibling, value);
    }

    /**
     * Print the tree
     */
    void print()
    {
        printHelper(root, 0);
    }

    /**
     * Helper function to print the tree
     */
    void printHelper(MultiWayTreeNode *node, int depth)
    {
        if (node == nullptr)
        {
            return;
        }

        // Print indentation
        for (int i = 0; i < depth; i++)
        {
            std::cout << "  ";
        }

        // Print node value
        std::cout << node->val << std::endl;

        // Print children with increased depth
        printHelper(node->firstChild, depth + 1);

        // Print siblings with same depth
        printHelper(node->nextSibling, depth);
    }
};

// ============================================================================
// Main Function with Examples and Demonstrations
// ============================================================================

int main()
{
    std::cout << "=======================================================" << std::endl;
    std::cout << "           Binary Trees Basics Demonstration            " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << std::endl;

    // Create a sample binary tree
    TreeNode *sampleTree = createSampleTree();

    std::cout << "Sample Binary Tree:" << std::endl;
    printTreeVisual(sampleTree);
    std::cout << std::endl;

    // Demonstrate tree traversals
    std::cout << "=== Tree Traversals ===" << std::endl;

    // In-order traversal
    std::vector<int> inorder = TreeTraversal::inorderTraversal(sampleTree);
    std::cout << "In-order traversal (LNR): ";
    for (int val : inorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Pre-order traversal
    std::vector<int> preorder = TreeTraversal::preorderTraversal(sampleTree);
    std::cout << "Pre-order traversal (NLR): ";
    for (int val : preorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Post-order traversal
    std::vector<int> postorder = TreeTraversal::postorderTraversal(sampleTree);
    std::cout << "Post-order traversal (LRN): ";
    for (int val : postorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Level-order traversal
    std::vector<int> levelorder = TreeTraversal::levelOrderTraversal(sampleTree);
    std::cout << "Level-order traversal (BFS): ";
    for (int val : levelorder)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl
              << std::endl;

    // Demonstrate level-order traversal by level
    std::vector<std::vector<int>> levelsByLevel = TreeTraversal::levelOrderTraversalByLevel(sampleTree);
    std::cout << "Level-order traversal by level:" << std::endl;
    for (int i = 0; i < levelsByLevel.size(); i++)
    {
        std::cout << "Level " << i << ": ";
        for (int val : levelsByLevel[i])
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Demonstrate iterative traversals
    std::cout << "=== Iterative Tree Traversals ===" << std::endl;

    // Iterative in-order traversal
    std::vector<int> inorderIter = TreeTraversal::inorderTraversalIterative(sampleTree);
    std::cout << "Iterative in-order traversal: ";
    for (int val : inorderIter)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Iterative pre-order traversal
    std::vector<int> preorderIter = TreeTraversal::preorderTraversalIterative(sampleTree);
    std::cout << "Iterative pre-order traversal: ";
    for (int val : preorderIter)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Iterative post-order traversal
    std::vector<int> postorderIter = TreeTraversal::postorderTraversalIterative(sampleTree);
    std::cout << "Iterative post-order traversal: ";
    for (int val : postorderIter)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl
              << std::endl;

    // Demonstrate basic tree operations
    std::cout << "=== Basic Tree Operations ===" << std::endl;

    // Height of the tree
    int treeHeight = TreeOperations::height(sampleTree);
    std::cout << "Height of the tree: " << treeHeight << std::endl;

    // Count of nodes
    int nodeCount = TreeOperations::countNodes(sampleTree);
    std::cout << "Number of nodes: " << nodeCount << std::endl;

    // Count of leaf nodes
    int leafCount = TreeOperations::countLeafNodes(sampleTree);
    std::cout << "Number of leaf nodes: " << leafCount << std::endl;

    // Diameter of the tree
    int treeDiameter = TreeOperations::diameterOfBinaryTree(sampleTree);
    std::cout << "Diameter of the tree: " << treeDiameter << std::endl;

    // Check if the tree is balanced
    bool balanced = TreeOperations::isBalanced(sampleTree);
    std::cout << "Is the tree balanced? " << (balanced ? "Yes" : "No") << std::endl;

    // Check if the tree is symmetric
    bool symmetric = TreeOperations::isSymmetric(sampleTree);
    std::cout << "Is the tree symmetric? " << (symmetric ? "Yes" : "No") << std::endl
              << std::endl;

    // Create a symmetric tree for demonstration
    TreeNode *symmetricTree = new TreeNode(1);
    symmetricTree->left = new TreeNode(2);
    symmetricTree->right = new TreeNode(2);
    symmetricTree->left->left = new TreeNode(3);
    symmetricTree->left->right = new TreeNode(4);
    symmetricTree->right->left = new TreeNode(4);
    symmetricTree->right->right = new TreeNode(3);

    std::cout << "Symmetric Tree:" << std::endl;
    printTreeVisual(symmetricTree);
    std::cout << std::endl;

    symmetric = TreeOperations::isSymmetric(symmetricTree);
    std::cout << "Is the symmetric tree symmetric? " << (symmetric ? "Yes" : "No") << std::endl
              << std::endl;

    // Demonstrate tree construction
    std::cout << "=== Tree Construction ===" << std::endl;

    // Build a tree from pre-order and in-order traversals
    std::vector<int> preorderData = {3, 9, 20, 15, 7};
    std::vector<int> inorderData = {9, 3, 15, 20, 7};

    TreeNode *constructedTree = TreeConstruction::buildTreeFromPreorderAndInorder(preorderData, inorderData);

    std::cout << "Tree constructed from pre-order and in-order traversals:" << std::endl;
    printTreeVisual(constructedTree);
    std::cout << std::endl;

    // Demonstrate serialization and deserialization
    std::cout << "=== Serialization and Deserialization ===" << std::endl;

    std::string serialized = TreeConstruction::serialize(sampleTree);
    std::cout << "Serialized tree: " << serialized << std::endl;

    TreeNode *deserialized = TreeConstruction::deserialize(serialized);

    std::cout << "Deserialized tree:" << std::endl;
    printTreeVisual(deserialized);
    std::cout << std::endl;

    // Check if the original and deserialized trees are identical
    bool identical = TreeOperations::areIdentical(sampleTree, deserialized);
    std::cout << "Are the original and deserialized trees identical? " << (identical ? "Yes" : "No") << std::endl
              << std::endl;

    // Demonstrate common tree problems
    std::cout << "=== Common Tree Problems ===" << std::endl;

    // Right side view
    std::vector<int> rightView = TreeProblems::rightSideView(sampleTree);
    std::cout << "Right side view: ";
    for (int val : rightView)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Left side view
    std::vector<int> leftView = TreeProblems::leftSideView(sampleTree);
    std::cout << "Left side view: ";
    for (int val : leftView)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Maximum path sum
    int maxPathSum = TreeProblems::maxPathSum(sampleTree);
    std::cout << "Maximum path sum: " << maxPathSum << std::endl;

    // Count good nodes
    int goodNodeCount = TreeProblems::goodNodes(sampleTree);
    std::cout << "Number of good nodes: " << goodNodeCount << std::endl
              << std::endl;

    // Demonstrate array-based binary tree
    std::cout << "=== Array-Based Binary Tree ===" << std::endl;

    ArrayBinaryTree arrayTree(1);
    arrayTree.setLeft(0, 2);
    arrayTree.setRight(0, 3);
    arrayTree.setLeft(1, 4);
    arrayTree.setRight(1, 5);
    arrayTree.setRight(2, 6);

    std::cout << "Array-based tree representation:" << std::endl;
    arrayTree.print();
    std::cout << std::endl;

    // Demonstrate multi-way tree
    std::cout << "=== Multi-Way Tree (Left-Child Right-Sibling) ===" << std::endl;

    MultiWayTree multiWayTree(1);
    multiWayTree.addChild(1, 2);
    multiWayTree.addChild(1, 3);
    multiWayTree.addChild(1, 4);
    multiWayTree.addChild(2, 5);
    multiWayTree.addChild(2, 6);
    multiWayTree.addChild(3, 7);

    std::cout << "Multi-way tree representation:" << std::endl;
    multiWayTree.print();
    std::cout << std::endl;

    // Free memory
    deleteTree(sampleTree);
    deleteTree(symmetricTree);
    deleteTree(constructedTree);
    deleteTree(deserialized);

    std::cout << "All memory freed." << std::endl;

    return 0;
}