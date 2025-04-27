#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <string>
#include <sstream>
#include <functional>
#include <chrono>
#include <iomanip>

/**
 * @brief Day 28: Weekly Challenge & Tree Problems
 *
 * This file implements solutions to common tree problems:
 *  - Lowest Common Ancestor (LCA)
 *  - Height Balancing
 *  - Path Sum Problems
 *
 * Part of the #DSAin45 challenge
 *
 * @author #DSAin45 Course
 * @date Day 28 of 45-Day DSA Challenge
 */

/****************************************************************************************
 * BINARY TREE STRUCTURE DEFINITIONS
 ****************************************************************************************/

/**
 * @brief Basic TreeNode structure for binary trees
 */
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;

    // Standard constructor
    TreeNode(int x = 0, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : val(x), left(left), right(right) {}
};

/**
 * @brief AVL Tree Node structure with height balance information
 */
struct AVLNode
{
    int val;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(int x) : val(x), left(nullptr), right(nullptr), height(1) {}
};

/****************************************************************************************
 * HELPER FUNCTIONS - TREE CREATION, TRAVERSAL AND DISPLAY
 ****************************************************************************************/

/**
 * @brief Create a binary tree from level order traversal (array representation)
 *
 * @param values Level order values with nullptr represented as -1
 * @return TreeNode* Root of the created tree
 */
TreeNode *createTreeFromLevelOrder(const std::vector<int> &values)
{
    if (values.empty() || values[0] == -1)
    {
        return nullptr;
    }

    TreeNode *root = new TreeNode(values[0]);
    std::queue<TreeNode *> queue;
    queue.push(root);

    size_t i = 1;
    while (!queue.empty() && i < values.size())
    {
        TreeNode *current = queue.front();
        queue.pop();

        // Create left child
        if (i < values.size() && values[i] != -1)
        {
            current->left = new TreeNode(values[i]);
            queue.push(current->left);
        }
        i++;

        // Create right child
        if (i < values.size() && values[i] != -1)
        {
            current->right = new TreeNode(values[i]);
            queue.push(current->right);
        }
        i++;
    }

    return root;
}

/**
 * @brief Delete a binary tree to prevent memory leaks
 *
 * @param root Root of the tree to delete
 */
void deleteTree(TreeNode *root)
{
    if (root == nullptr)
    {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

/**
 * @brief Delete an AVL tree to prevent memory leaks
 *
 * @param root Root of the AVL tree to delete
 */
void deleteAVLTree(AVLNode *root)
{
    if (root == nullptr)
    {
        return;
    }

    deleteAVLTree(root->left);
    deleteAVLTree(root->right);
    delete root;
}

/**
 * @brief Print a binary tree in a visual format
 *
 * @param root Root of the tree to print
 * @param prefix Current prefix for the line
 * @param isLeft Flag indicating if current node is a left child
 */
void printTree(TreeNode *root, const std::string &prefix = "", bool isLeft = true)
{
    if (root == nullptr)
    {
        std::cout << prefix << (isLeft ? "└── " : "┌── ") << "NULL" << std::endl;
        return;
    }

    // Print current node
    std::cout << prefix << (isLeft ? "└── " : "┌── ") << root->val << std::endl;

    // Enter the next tree level - left and right branch
    std::string newPrefix = prefix + (isLeft ? "    " : "│   ");

    if (root->right)
    {
        printTree(root->right, newPrefix, false);
    }
    else if (root->left)
    {
        std::cout << newPrefix << "┌── NULL" << std::endl;
    }

    if (root->left)
    {
        printTree(root->left, newPrefix, true);
    }
    else if (root->right)
    {
        std::cout << newPrefix << "└── NULL" << std::endl;
    }
}

/**
 * @brief Print an AVL tree in a visual format with height information
 *
 * @param root Root of the AVL tree to print
 * @param prefix Current prefix for the line
 * @param isLeft Flag indicating if current node is a left child
 */
void printAVLTree(AVLNode *root, const std::string &prefix = "", bool isLeft = true)
{
    if (root == nullptr)
    {
        std::cout << prefix << (isLeft ? "└── " : "┌── ") << "NULL" << std::endl;
        return;
    }

    // Print current node with height
    std::cout << prefix << (isLeft ? "└── " : "┌── ") << root->val << " (h=" << root->height << ")" << std::endl;

    // Enter the next tree level - left and right branch
    std::string newPrefix = prefix + (isLeft ? "    " : "│   ");

    if (root->right)
    {
        printAVLTree(root->right, newPrefix, false);
    }
    else if (root->left)
    {
        std::cout << newPrefix << "┌── NULL" << std::endl;
    }

    if (root->left)
    {
        printAVLTree(root->left, newPrefix, true);
    }
    else if (root->right)
    {
        std::cout << newPrefix << "└── NULL" << std::endl;
    }
}

/**
 * @brief Serialize a binary tree to string (for easy comparison)
 *
 * @param root Root of the tree to serialize
 * @return std::string Serialized representation of the tree
 */
std::string serializeTree(TreeNode *root)
{
    if (root == nullptr)
    {
        return "[]";
    }

    std::vector<std::string> result;
    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty())
    {
        TreeNode *node = q.front();
        q.pop();

        if (node == nullptr)
        {
            result.push_back("null");
        }
        else
        {
            result.push_back(std::to_string(node->val));
            q.push(node->left);
            q.push(node->right);
        }
    }

    // Remove trailing nulls
    while (!result.empty() && result.back() == "null")
    {
        result.pop_back();
    }

    // Format as string
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < result.size(); ++i)
    {
        ss << result[i];
        if (i < result.size() - 1)
        {
            ss << ",";
        }
    }
    ss << "]";

    return ss.str();
}

/****************************************************************************************
 * LOWEST COMMON ANCESTOR (LCA) IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Find the lowest common ancestor of two nodes in a binary tree - Recursive approach
 *
 * @param root Root of the binary tree
 * @param p First node value to find LCA for
 * @param q Second node value to find LCA for
 * @return TreeNode* LCA node, or nullptr if not found
 *
 * Time Complexity: O(n) where n is the number of nodes in the tree
 * Space Complexity: O(h) where h is the height of the tree (recursion stack)
 */
TreeNode *findLCARecursive(TreeNode *root, int p, int q)
{
    // Base case: if root is null or one of the nodes, return root
    if (root == nullptr || root->val == p || root->val == q)
    {
        return root;
    }

    // Look for LCA in left and right subtrees
    TreeNode *leftLCA = findLCARecursive(root->left, p, q);
    TreeNode *rightLCA = findLCARecursive(root->right, p, q);

    // If both left and right return non-null, root is the LCA
    if (leftLCA && rightLCA)
    {
        return root;
    }

    // Otherwise, return the non-null result
    return leftLCA ? leftLCA : rightLCA;
}

/**
 * @brief Find the lowest common ancestor of two nodes in a binary tree - Iterative approach
 *
 * @param root Root of the binary tree
 * @param p First node value to find LCA for
 * @param q Second node value to find LCA for
 * @return TreeNode* LCA node, or nullptr if not found
 *
 * This approach uses parent pointers to trace back from p and q.
 *
 * Time Complexity: O(n) where n is the number of nodes in the tree
 * Space Complexity: O(n) for the parent pointers and visited set
 */
TreeNode *findLCAIterative(TreeNode *root, int p, int q)
{
    if (!root)
        return nullptr;

    // Map to store parent pointers
    std::unordered_map<int, TreeNode *> parent;
    parent[root->val] = nullptr;

    // DFS to build parent pointers
    std::stack<TreeNode *> stack;
    stack.push(root);

    // Continue DFS until we find both p and q
    while (parent.find(p) == parent.end() || parent.find(q) == parent.end())
    {
        if (stack.empty())
        {
            // One or both nodes not found
            return nullptr;
        }

        TreeNode *node = stack.top();
        stack.pop();

        // Process children and update parent pointers
        if (node->left)
        {
            parent[node->left->val] = node;
            stack.push(node->left);
        }
        if (node->right)
        {
            parent[node->right->val] = node;
            stack.push(node->right);
        }
    }

    // Create a set of ancestors for p
    std::unordered_map<int, bool> ancestors;
    TreeNode *pNode = nullptr;

    // Find the actual node p
    for (auto pair : parent)
    {
        if (pair.first == p)
        {
            for (auto nodePair : parent)
            {
                if (nodePair.first == p && !pNode)
                {
                    // Find the actual node with value p
                    if (!nodePair.second || nodePair.second->left && nodePair.second->left->val == p)
                    {
                        pNode = nodePair.second->left;
                    }
                    else
                    {
                        pNode = nodePair.second->right;
                    }
                    break;
                }
            }
            break;
        }
    }

    // If we couldn't find the node with value p, search directly
    if (!pNode)
    {
        std::stack<TreeNode *> searchStack;
        searchStack.push(root);
        while (!searchStack.empty())
        {
            TreeNode *node = searchStack.top();
            searchStack.pop();
            if (node->val == p)
            {
                pNode = node;
                break;
            }
            if (node->right)
                searchStack.push(node->right);
            if (node->left)
                searchStack.push(node->left);
        }
    }

    // Add all ancestors of p to the set
    while (pNode)
    {
        ancestors[pNode->val] = true;
        if (parent.find(pNode->val) != parent.end())
        {
            pNode = parent[pNode->val];
        }
        else
        {
            pNode = nullptr;
        }
    }

    // Find the first ancestor of q that is also an ancestor of p
    TreeNode *qNode = nullptr;

    // Find the actual node q using similar approach
    for (auto pair : parent)
    {
        if (pair.first == q)
        {
            for (auto nodePair : parent)
            {
                if (nodePair.first == q && !qNode)
                {
                    if (!nodePair.second || nodePair.second->left && nodePair.second->left->val == q)
                    {
                        qNode = nodePair.second->left;
                    }
                    else
                    {
                        qNode = nodePair.second->right;
                    }
                    break;
                }
            }
            break;
        }
    }

    // If we couldn't find the node with value q, search directly
    if (!qNode)
    {
        std::stack<TreeNode *> searchStack;
        searchStack.push(root);
        while (!searchStack.empty())
        {
            TreeNode *node = searchStack.top();
            searchStack.pop();
            if (node->val == q)
            {
                qNode = node;
                break;
            }
            if (node->right)
                searchStack.push(node->right);
            if (node->left)
                searchStack.push(node->left);
        }
    }

    // Trace back from q until we find a common ancestor
    while (qNode)
    {
        if (ancestors.find(qNode->val) != ancestors.end())
        {
            return qNode;
        }
        if (parent.find(qNode->val) != parent.end())
        {
            qNode = parent[qNode->val];
        }
        else
        {
            qNode = nullptr;
        }
    }

    return nullptr;
}

/**
 * @brief Enhanced version of LCA that handles nodes that might not exist in the tree
 *
 * @param root Root of the binary tree
 * @param p First node value
 * @param q Second node value
 * @return TreeNode* LCA node, or nullptr if either node doesn't exist
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)
 */
TreeNode *findLCAWithValidation(TreeNode *root, int p, int q)
{
    // First, check if both nodes exist in the tree
    bool pExists = false, qExists = false;

    std::function<void(TreeNode *)> checkNodes = [&](TreeNode *node)
    {
        if (!node)
            return;
        if (node->val == p)
            pExists = true;
        if (node->val == q)
            qExists = true;
        if (!pExists || !qExists)
        {
            checkNodes(node->left);
        }
        if (!pExists || !qExists)
        {
            checkNodes(node->right);
        }
    };

    checkNodes(root);

    if (!pExists || !qExists)
    {
        return nullptr;
    }

    // If both nodes exist, find their LCA
    return findLCARecursive(root, p, q);
}

/**
 * @brief Find LCA in a Binary Search Tree (optimized for BST properties)
 *
 * @param root Root of the BST
 * @param p First node value
 * @param q Second node value
 * @return TreeNode* LCA node
 *
 * Time Complexity: O(h) where h is the height of the tree
 * Space Complexity: O(1) for iterative, O(h) for recursive due to stack
 */
TreeNode *findLCAinBST(TreeNode *root, int p, int q)
{
    if (!root)
        return nullptr;

    // Ensure p is smaller than q for simplicity
    if (p > q)
        std::swap(p, q);

    // Iterative solution (more efficient)
    while (root)
    {
        if (root->val < p)
        {
            // Both nodes are in right subtree
            root = root->right;
        }
        else if (root->val > q)
        {
            // Both nodes are in left subtree
            root = root->left;
        }
        else
        {
            // Current node is between p and q or equal to one of them
            // This is the LCA
            return root;
        }
    }

    return nullptr;
}

/****************************************************************************************
 * HEIGHT BALANCING AND AVL TREE IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Get the height of a binary tree
 *
 * @param root Root of the tree
 * @return int Height of the tree (0 for empty tree)
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h) due to recursion
 */
int getHeight(TreeNode *root)
{
    if (!root)
        return 0;
    return std::max(getHeight(root->left), getHeight(root->right)) + 1;
}

/**
 * @brief Check if a binary tree is balanced (height difference ≤ 1)
 *
 * @param root Root of the tree
 * @return bool True if the tree is balanced
 *
 * Time Complexity: O(n²) - naive approach recalculating heights
 * Space Complexity: O(h)
 */
bool isBalancedNaive(TreeNode *root)
{
    if (!root)
        return true;

    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    // Check height difference and if subtrees are balanced
    return abs(leftHeight - rightHeight) <= 1 &&
           isBalancedNaive(root->left) &&
           isBalancedNaive(root->right);
}

/**
 * @brief Check if a binary tree is balanced - Optimized approach
 *
 * @param root Root of the tree
 * @return bool True if the tree is balanced
 *
 * Time Complexity: O(n) - calculating height during the balancing check
 * Space Complexity: O(h)
 */
bool isBalancedOptimized(TreeNode *root)
{
    // Helper function that returns height if balanced, -1 if not balanced
    std::function<int(TreeNode *)> checkHeight = [&](TreeNode *node) -> int
    {
        if (!node)
            return 0;

        int leftHeight = checkHeight(node->left);
        if (leftHeight == -1)
            return -1; // Left subtree is not balanced

        int rightHeight = checkHeight(node->right);
        if (rightHeight == -1)
            return -1; // Right subtree is not balanced

        // Check if current node is balanced
        if (abs(leftHeight - rightHeight) > 1)
            return -1;

        // Return current height
        return std::max(leftHeight, rightHeight) + 1;
    };

    return checkHeight(root) != -1;
}

/**
 * @brief Get the height of an AVL tree node
 *
 * @param node AVL tree node
 * @return int Height of the node (0 for null)
 */
int getAVLHeight(AVLNode *node)
{
    if (!node)
        return 0;
    return node->height;
}

/**
 * @brief Update the height of an AVL tree node
 *
 * @param node Node to update
 */
void updateHeight(AVLNode *node)
{
    if (!node)
        return;
    node->height = 1 + std::max(getAVLHeight(node->left), getAVLHeight(node->right));
}

/**
 * @brief Get the balance factor of an AVL tree node
 *
 * @param node Node to check
 * @return int Balance factor (left height - right height)
 */
int getBalanceFactor(AVLNode *node)
{
    if (!node)
        return 0;
    return getAVLHeight(node->left) - getAVLHeight(node->right);
}

/**
 * @brief Perform a right rotation on an AVL tree
 *
 * @param y Root node to rotate
 * @return AVLNode* New root after rotation
 */
AVLNode *rightRotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    updateHeight(y);
    updateHeight(x);

    // Return new root
    return x;
}

/**
 * @brief Perform a left rotation on an AVL tree
 *
 * @param x Root node to rotate
 * @return AVLNode* New root after rotation
 */
AVLNode *leftRotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    updateHeight(x);
    updateHeight(y);

    // Return new root
    return y;
}

/**
 * @brief Insert a value into an AVL tree
 *
 * @param root Root of the AVL tree
 * @param val Value to insert
 * @return AVLNode* New root after insertion
 *
 * Time Complexity: O(log n)
 * Space Complexity: O(log n) due to recursion
 */
AVLNode *insertAVL(AVLNode *root, int val)
{
    // Step 1: Perform standard BST insertion
    if (!root)
    {
        return new AVLNode(val);
    }

    if (val < root->val)
    {
        root->left = insertAVL(root->left, val);
    }
    else if (val > root->val)
    {
        root->right = insertAVL(root->right, val);
    }
    else
    {
        // Duplicate values not allowed
        return root;
    }

    // Step 2: Update height of current node
    updateHeight(root);

    // Step 3: Get the balance factor
    int balance = getBalanceFactor(root);

    // Step 4: Perform rotations if needed to balance the tree

    // Left Left Case
    if (balance > 1 && val < root->left->val)
    {
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && val > root->right->val)
    {
        return leftRotate(root);
    }

    // Left Right Case
    if (balance > 1 && val > root->left->val)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && val < root->right->val)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    // No balancing needed
    return root;
}

/**
 * @brief Delete a value from an AVL tree
 *
 * @param root Root of the AVL tree
 * @param val Value to delete
 * @return AVLNode* New root after deletion
 *
 * Time Complexity: O(log n)
 * Space Complexity: O(log n) due to recursion
 */
AVLNode *deleteAVL(AVLNode *root, int val)
{
    // Step 1: Perform standard BST delete
    if (!root)
    {
        return root;
    }

    if (val < root->val)
    {
        root->left = deleteAVL(root->left, val);
    }
    else if (val > root->val)
    {
        root->right = deleteAVL(root->right, val);
    }
    else
    {
        // Node with the value to be deleted found

        // Case 1: Node with only one child or no child
        if (!root->left || !root->right)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            // No child case
            if (!temp)
            {
                temp = root;
                root = nullptr;
            }
            else
            {
                // One child case
                *root = *temp; // Copy the contents of non-empty child
            }

            delete temp;
        }
        else
        {
            // Case 2: Node with two children

            // Find inorder successor (smallest in right subtree)
            AVLNode *temp = root->right;
            while (temp->left)
            {
                temp = temp->left;
            }

            // Copy successor value to this node
            root->val = temp->val;

            // Delete the successor
            root->right = deleteAVL(root->right, temp->val);
        }
    }

    // If tree had only one node, return
    if (!root)
    {
        return root;
    }

    // Step 2: Update height of current node
    updateHeight(root);

    // Step 3: Get the balance factor
    int balance = getBalanceFactor(root);

    // Step 4: Perform rotations if needed to balance the tree

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
    {
        return rightRotate(root);
    }

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
    {
        return leftRotate(root);
    }

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/**
 * @brief Build a complete AVL tree from a sorted array in O(n) time
 *
 * @param arr Sorted array of values
 * @param start Start index
 * @param end End index
 * @return AVLNode* Root of the balanced AVL tree
 */
AVLNode *sortedArrayToAVL(const std::vector<int> &arr, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }

    // Get the middle element as root
    int mid = (start + end) / 2;
    AVLNode *root = new AVLNode(arr[mid]);

    // Recursively build left and right subtrees
    root->left = sortedArrayToAVL(arr, start, mid - 1);
    root->right = sortedArrayToAVL(arr, mid + 1, end);

    // Update height
    updateHeight(root);

    return root;
}

/**
 * @brief Convert a binary tree to a balanced AVL tree
 *
 * @param root Root of the binary tree
 * @return AVLNode* Root of the balanced AVL tree
 *
 * Time Complexity: O(n log n) for unbalanced input, O(n) for balanced input
 * Space Complexity: O(n)
 */
AVLNode *convertToAVL(TreeNode *root)
{
    if (!root)
    {
        return nullptr;
    }

    // Step 1: In-order traversal to get sorted values
    std::vector<int> values;
    std::function<void(TreeNode *)> inOrder = [&](TreeNode *node)
    {
        if (!node)
            return;
        inOrder(node->left);
        values.push_back(node->val);
        inOrder(node->right);
    };

    inOrder(root);

    // Step 2: Build a balanced AVL tree from sorted array
    return sortedArrayToAVL(values, 0, values.size() - 1);
}

/****************************************************************************************
 * PATH SUM IMPLEMENTATIONS
 ****************************************************************************************/

/**
 * @brief Check if there's a root-to-leaf path with sum equal to targetSum
 *
 * @param root Root of the binary tree
 * @param targetSum Target sum to find
 * @return bool True if such a path exists
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)
 */
bool hasPathSum(TreeNode *root, int targetSum)
{
    // Base case: empty tree
    if (!root)
    {
        return false;
    }

    // If it's a leaf node, check if the value matches the remaining sum
    if (!root->left && !root->right)
    {
        return root->val == targetSum;
    }

    // Recursively check left and right subtrees with reduced sum
    int newSum = targetSum - root->val;
    return hasPathSum(root->left, newSum) || hasPathSum(root->right, newSum);
}

/**
 * @brief Find all root-to-leaf paths with sum equal to targetSum
 *
 * @param root Root of the binary tree
 * @param targetSum Target sum to find
 * @return std::vector<std::vector<int>> All paths with the target sum
 *
 * Time Complexity: O(n²) - In worst case, we might have all paths with target sum
 * Space Complexity: O(h) for recursion + O(n) for paths
 */
std::vector<std::vector<int>> pathSum(TreeNode *root, int targetSum)
{
    std::vector<std::vector<int>> result;
    std::vector<int> currentPath;

    std::function<void(TreeNode *, int)> dfs = [&](TreeNode *node, int remainingSum)
    {
        if (!node)
        {
            return;
        }

        // Add current node to path
        currentPath.push_back(node->val);

        // Check if it's a leaf node and sum matches
        if (!node->left && !node->right && remainingSum == node->val)
        {
            result.push_back(currentPath);
        }

        // Recursively explore left and right subtrees
        int newSum = remainingSum - node->val;
        dfs(node->left, newSum);
        dfs(node->right, newSum);

        // Backtrack: remove current node from path
        currentPath.pop_back();
    };

    dfs(root, targetSum);
    return result;
}

/**
 * @brief Find the number of paths (not necessarily starting at root or ending at leaf)
 *        with sum equal to targetSum
 *
 * @param root Root of the binary tree
 * @param targetSum Target sum to find
 * @return int Number of paths with the target sum
 *
 * Time Complexity: O(n²) in worst case
 * Space Complexity: O(h)
 */
int pathSumIII(TreeNode *root, int targetSum)
{
    int count = 0;

    // Helper function to count paths starting from a given node
    std::function<void(TreeNode *, long)> dfs = [&](TreeNode *node, long currentSum)
    {
        if (!node)
        {
            return;
        }

        // Add current node value to sum
        currentSum += node->val;

        // Check if we found a path with the target sum
        if (currentSum == targetSum)
        {
            count++;
        }

        // Explore left and right subtrees
        dfs(node->left, currentSum);
        dfs(node->right, currentSum);
    };

    // Try starting the path from each node
    std::function<void(TreeNode *)> traverse = [&](TreeNode *node)
    {
        if (!node)
        {
            return;
        }

        // Start a path from this node
        dfs(node, 0);

        // Try starting paths from children nodes
        traverse(node->left);
        traverse(node->right);
    };

    traverse(root);
    return count;
}

/**
 * @brief Find the number of paths with sum equal to targetSum - Optimized with prefix sums
 *
 * @param root Root of the binary tree
 * @param targetSum Target sum to find
 * @return int Number of paths with the target sum
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h) in average case, O(n) in worst case
 */
int pathSumIIIOptimized(TreeNode *root, int targetSum)
{
    // Map to store prefix sums and their frequencies
    std::unordered_map<long, int> prefixSumCount;
    prefixSumCount[0] = 1; // Empty path has sum 0

    int count = 0;

    std::function<void(TreeNode *, long)> dfs = [&](TreeNode *node, long currentSum)
    {
        if (!node)
        {
            return;
        }

        // Add current node to path sum
        currentSum += node->val;

        // Check if there are paths ending at current node with target sum
        // by looking for (currentSum - targetSum) in our prefix sums
        count += prefixSumCount[currentSum - targetSum];

        // Add current sum to prefix sums
        prefixSumCount[currentSum]++;

        // Explore left and right subtrees
        dfs(node->left, currentSum);
        dfs(node->right, currentSum);

        // Backtrack: remove current sum from prefix sums
        prefixSumCount[currentSum]--;
    };

    dfs(root, 0);
    return count;
}

/**
 * @brief Find the maximum sum path in a binary tree (not necessarily starting at root or ending at leaf)
 *
 * @param root Root of the binary tree
 * @return int Maximum path sum
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)
 */
int maxPathSum(TreeNode *root)
{
    int maxSum = INT_MIN;

    // Helper function that computes the max path sum WITH the node as the highest point
    std::function<int(TreeNode *)> maxGain = [&](TreeNode *node) -> int
    {
        if (!node)
        {
            return 0;
        }

        // Calculate max path sum WITH the current node included
        // Ignore negative sums by taking max with 0
        int leftGain = std::max(0, maxGain(node->left));
        int rightGain = std::max(0, maxGain(node->right));

        // Calculate the max path sum INCLUDING the current node
        int pathSum = node->val + leftGain + rightGain;

        // Update the global max sum
        maxSum = std::max(maxSum, pathSum);

        // Return max sum of path ending at current node
        // (can only include one child for a valid path up to parent)
        return node->val + std::max(leftGain, rightGain);
    };

    maxGain(root);
    return maxSum;
}

/****************************************************************************************
 * ADDITIONAL TREE PROBLEMS AND VARIATIONS
 ****************************************************************************************/

/**
 * @brief Find the diameter of a binary tree (length of longest path between any two nodes)
 *
 * @param root Root of the binary tree
 * @return int Diameter of the tree
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)
 */
int diameterOfBinaryTree(TreeNode *root)
{
    int diameter = 0;

    std::function<int(TreeNode *)> height = [&](TreeNode *node) -> int
    {
        if (!node)
        {
            return 0;
        }

        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        // Update diameter if the path going through the current node is longer
        diameter = std::max(diameter, leftHeight + rightHeight);

        // Return height of subtree rooted at node
        return std::max(leftHeight, rightHeight) + 1;
    };

    height(root);
    return diameter;
}

/**
 * @brief Check if a binary tree is symmetric around its center
 *
 * @param root Root of the binary tree
 * @return bool True if the tree is symmetric
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)
 */
bool isSymmetric(TreeNode *root)
{
    if (!root)
    {
        return true;
    }

    std::function<bool(TreeNode *, TreeNode *)> isMirror = [&](TreeNode *left, TreeNode *right) -> bool
    {
        // If both nodes are null, they are mirrors
        if (!left && !right)
        {
            return true;
        }

        // If only one node is null, they are not mirrors
        if (!left || !right)
        {
            return false;
        }

        // Check if the current nodes and their subtrees are mirrors
        return (left->val == right->val) &&
               isMirror(left->left, right->right) &&
               isMirror(left->right, right->left);
    };

    return isMirror(root->left, root->right);
}

/**
 * @brief Serialize a binary tree to a string and deserialize it back
 *
 * This is a more advanced example of serialization that can handle any binary tree.
 */
class Codec
{
public:
    // Encodes a tree to a single string.
    std::string serialize(TreeNode *root)
    {
        std::string result;

        std::function<void(TreeNode *)> preorder = [&](TreeNode *node)
        {
            if (!node)
            {
                result += "null,";
                return;
            }

            result += std::to_string(node->val) + ",";
            preorder(node->left);
            preorder(node->right);
        };

        preorder(root);
        return result;
    }

    // Decodes your encoded data to tree.
    TreeNode *deserialize(std::string data)
    {
        std::vector<std::string> tokens;

        // Split the string by commas
        std::stringstream ss(data);
        std::string token;
        while (std::getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        int index = 0;

        std::function<TreeNode *()> buildTree = [&]() -> TreeNode *
        {
            if (index >= tokens.size() || tokens[index] == "null")
            {
                index++;
                return nullptr;
            }

            TreeNode *node = new TreeNode(std::stoi(tokens[index++]));
            node->left = buildTree();
            node->right = buildTree();

            return node;
        };

        return buildTree();
    }
};

/****************************************************************************************
 * PERFORMANCE BENCHMARKS AND DEMOS
 ****************************************************************************************/

/**
 * @brief Benchmark different LCA implementations
 */
void benchmarkLCA()
{
    std::cout << "===== LCA Implementation Benchmarks =====" << std::endl;

    // Create a balanced tree with 127 nodes (7 levels)
    std::vector<int> values;
    for (int i = 1; i <= 127; i++)
    {
        values.push_back(i);
    }

    TreeNode *root = createTreeFromLevelOrder(values);

    // Choose nodes for LCA
    int p = 30, q = 95; // Nodes in different subtrees, LCA should be node 1 (root)

    const int iterations = 100000;

    // Benchmark recursive LCA
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        TreeNode *lca = findLCARecursive(root, p, q);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> recursiveDuration = end - start;

    // Benchmark iterative LCA
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        TreeNode *lca = findLCAIterative(root, p, q);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> iterativeDuration = end - start;

    // Print results
    std::cout << "Tree size: " << values.size() << " nodes, Iterations: " << iterations << std::endl;
    std::cout << "Recursive LCA: " << std::fixed << std::setprecision(2) << recursiveDuration.count() << " ms" << std::endl;
    std::cout << "Iterative LCA: " << std::fixed << std::setprecision(2) << iterativeDuration.count() << " ms" << std::endl;
    std::cout << std::endl;

    // Clean up
    deleteTree(root);
}

/**
 * @brief Benchmark different height balancing implementations
 */
void benchmarkHeightBalancing()
{
    std::cout << "===== Height Balancing Benchmarks =====" << std::endl;

    // Create trees with different levels of balance
    std::vector<int> balancedValues = {5, 3, 8, 2, 4, 7, 10, 1, -1, -1, -1, 6, -1, 9, 11};
    std::vector<int> unbalancedValues = {1, -1, 2, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5};

    TreeNode *balancedTree = createTreeFromLevelOrder(balancedValues);
    TreeNode *unbalancedTree = createTreeFromLevelOrder(unbalancedValues);

    const int iterations = 10000;

    // Benchmark naive balancing check on balanced tree
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        bool isBalanced = isBalancedNaive(balancedTree);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> naiveBalancedDuration = end - start;

    // Benchmark optimized balancing check on balanced tree
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        bool isBalanced = isBalancedOptimized(balancedTree);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> optimizedBalancedDuration = end - start;

    // Benchmark naive balancing check on unbalanced tree
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        bool isBalanced = isBalancedNaive(unbalancedTree);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> naiveUnbalancedDuration = end - start;

    // Benchmark optimized balancing check on unbalanced tree
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        bool isBalanced = isBalancedOptimized(unbalancedTree);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> optimizedUnbalancedDuration = end - start;

    // Print results
    std::cout << "Iterations: " << iterations << std::endl;
    std::cout << "Balanced Tree (Height: " << getHeight(balancedTree) << "):" << std::endl;
    std::cout << "  Naive Check: " << std::fixed << std::setprecision(2) << naiveBalancedDuration.count() << " ms" << std::endl;
    std::cout << "  Optimized Check: " << std::fixed << std::setprecision(2) << optimizedBalancedDuration.count() << " ms" << std::endl;

    std::cout << "Unbalanced Tree (Height: " << getHeight(unbalancedTree) << "):" << std::endl;
    std::cout << "  Naive Check: " << std::fixed << std::setprecision(2) << naiveUnbalancedDuration.count() << " ms" << std::endl;
    std::cout << "  Optimized Check: " << std::fixed << std::setprecision(2) << optimizedUnbalancedDuration.count() << " ms" << std::endl;
    std::cout << std::endl;

    // Clean up
    deleteTree(balancedTree);
    deleteTree(unbalancedTree);
}

/**
 * @brief Benchmark different path sum implementations
 */
void benchmarkPathSum()
{
    std::cout << "===== Path Sum Benchmarks =====" << std::endl;

    // Create trees for testing
    std::vector<int> values = {5, 4, 8, 11, -1, 13, 4, 7, 2, -1, -1, -1, -1, 5, 1};
    TreeNode *root = createTreeFromLevelOrder(values);

    const int iterations = 100000;
    int targetSum = 22; // Sum of path [5, 4, 11, 2]

    // Benchmark basic path sum
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        bool hasPath = hasPathSum(root, targetSum);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> basicDuration = end - start;

    // Benchmark path sum II (all paths)
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        std::vector<std::vector<int>> paths = pathSum(root, targetSum);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> allPathsDuration = end - start;

    // Benchmark path sum III (any path)
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        int count = pathSumIII(root, targetSum);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> anyPathDuration = end - start;

    // Benchmark optimized path sum III
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++)
    {
        int count = pathSumIIIOptimized(root, targetSum);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> optimizedAnyPathDuration = end - start;

    // Print results
    std::cout << "Tree size: " << values.size() << " nodes, Iterations: " << iterations << std::endl;
    std::cout << "Basic path sum: " << std::fixed << std::setprecision(2) << basicDuration.count() << " ms" << std::endl;
    std::cout << "Find all paths: " << std::fixed << std::setprecision(2) << allPathsDuration.count() << " ms" << std::endl;
    std::cout << "Count any path: " << std::fixed << std::setprecision(2) << anyPathDuration.count() << " ms" << std::endl;
    std::cout << "Optimized any path: " << std::fixed << std::setprecision(2) << optimizedAnyPathDuration.count() << " ms" << std::endl;
    std::cout << std::endl;

    // Clean up
    deleteTree(root);
}

/**
 * @brief Demo for Lowest Common Ancestor
 */
void demoLCA()
{
    std::cout << "===== Lowest Common Ancestor (LCA) Demo =====" << std::endl;

    // Create a tree for demonstration
    //        5
    //       / \
    //      3   8
    //     / \ / \
    //    2  4 7  10
    //   /     /  / \
    //  1     6  9  11
    std::vector<int> values = {5, 3, 8, 2, 4, 7, 10, 1, -1, -1, -1, 6, -1, 9, 11};
    TreeNode *root = createTreeFromLevelOrder(values);

    std::cout << "Tree Structure:" << std::endl;
    printTree(root);
    std::cout << std::endl;

    // Test different LCA scenarios
    struct LCATest
    {
        int p;
        int q;
        std::string description;
    };

    std::vector<LCATest> tests = {
        {2, 4, "Nodes under the same parent (3)"},
        {1, 4, "Nodes at different levels in the same subtree"},
        {2, 8, "Nodes in different subtrees"},
        {7, 11, "Nodes in different subtrees of the right child"},
        {5, 9, "One node is the root, other is deep in the tree"},
        {12, 4, "One node doesn't exist in the tree"}};

    for (const auto &test : tests)
    {
        TreeNode *lca = findLCAWithValidation(root, test.p, test.q);
        std::cout << "LCA of " << test.p << " and " << test.q << " (" << test.description << "): ";
        if (lca)
        {
            std::cout << lca->val << std::endl;
        }
        else
        {
            std::cout << "Not found (one or both nodes don't exist)" << std::endl;
        }
    }

    std::cout << std::endl;

    // Clean up
    deleteTree(root);
}

/**
 * @brief Demo for Height Balancing and AVL Trees
 */
void demoHeightBalancing()
{
    std::cout << "===== Height Balancing and AVL Trees Demo =====" << std::endl;

    // Create an unbalanced binary tree
    std::cout << "Creating an unbalanced binary tree:" << std::endl;
    TreeNode *unbalancedTree = new TreeNode(1);
    unbalancedTree->right = new TreeNode(2);
    unbalancedTree->right->right = new TreeNode(3);
    unbalancedTree->right->right->right = new TreeNode(4);
    unbalancedTree->right->right->right->right = new TreeNode(5);

    printTree(unbalancedTree);

    std::cout << "Height: " << getHeight(unbalancedTree) << std::endl;
    std::cout << "Is balanced? " << (isBalancedOptimized(unbalancedTree) ? "Yes" : "No") << std::endl;
    std::cout << std::endl;

    // Convert to AVL tree
    std::cout << "Converting to AVL tree:" << std::endl;
    AVLNode *avlTree = convertToAVL(unbalancedTree);

    printAVLTree(avlTree);

    std::cout << "Height: " << getAVLHeight(avlTree) << std::endl;
    std::cout << "Is balanced? Always (by definition of AVL tree)" << std::endl;
    std::cout << std::endl;

    // Create an AVL tree from scratch
    std::cout << "Building an AVL tree with insertions:" << std::endl;
    AVLNode *avlTree2 = nullptr;

    // Insert values and show rotations
    std::vector<int> valuesToInsert = {10, 20, 30, 40, 50, 25};

    for (int val : valuesToInsert)
    {
        std::cout << "Inserting " << val << ":" << std::endl;
        avlTree2 = insertAVL(avlTree2, val);
        printAVLTree(avlTree2);
        std::cout << "Height: " << getAVLHeight(avlTree2) << std::endl;
        std::cout << std::endl;
    }

    // Delete a value to demonstrate AVL rebalancing
    int valueToDelete = 30;
    std::cout << "Deleting " << valueToDelete << ":" << std::endl;
    avlTree2 = deleteAVL(avlTree2, valueToDelete);
    printAVLTree(avlTree2);
    std::cout << std::endl;

    // Clean up
    deleteTree(unbalancedTree);
    deleteAVLTree(avlTree);
    deleteAVLTree(avlTree2);
}

/**
 * @brief Demo for Path Sum problems
 */
void demoPathSum()
{
    std::cout << "===== Path Sum Problems Demo =====" << std::endl;

    // Create a tree for demonstration
    //        5
    //       / \
    //      4   8
    //     /   / \
    //    11  13  4
    //   /  \    / \
    //  7    2  5   1
    std::vector<int> values = {5, 4, 8, 11, -1, 13, 4, 7, 2, -1, -1, -1, -1, 5, 1};
    TreeNode *root = createTreeFromLevelOrder(values);

    std::cout << "Tree Structure:" << std::endl;
    printTree(root);
    std::cout << std::endl;

    // Test basic path sum
    int targetSum = 22;
    std::cout << "Path Sum I (root-to-leaf path with sum = " << targetSum << "): ";
    std::cout << (hasPathSum(root, targetSum) ? "Exists" : "Does not exist") << std::endl;

    // Find all paths with the target sum
    std::cout << "\nPath Sum II (all root-to-leaf paths with sum = " << targetSum << "):" << std::endl;
    std::vector<std::vector<int>> paths = pathSum(root, targetSum);

    for (const auto &path : paths)
    {
        std::cout << "  Path: ";
        for (size_t i = 0; i < path.size(); ++i)
        {
            std::cout << path[i];
            if (i < path.size() - 1)
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    // Count any path with the target sum
    targetSum = 9; // Sum of paths [4->5] and [5->4] and [9] and [2->7]
    std::cout << "\nPath Sum III (any path with sum = " << targetSum << "): ";
    std::cout << pathSumIIIOptimized(root, targetSum) << " paths" << std::endl;

    // Find max path sum
    std::cout << "\nMaximum Path Sum: " << maxPathSum(root) << std::endl;
    std::cout << "(The max path could be any path in the tree, not necessarily from root)" << std::endl;

    std::cout << std::endl;

    // Clean up
    deleteTree(root);
}

/**
 * @brief Main function to demonstrate tree problems
 */
int main()
{
    std::cout << "======================================================" << std::endl;
    std::cout << "=== DAY 28: Weekly Challenge & Tree Problems       ===" << std::endl;
    std::cout << "=== Part of #DSAin45 Course                        ===" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;

    // Run demos
    demoLCA();
    demoHeightBalancing();
    demoPathSum();

    // Run benchmarks
    benchmarkLCA();
    benchmarkHeightBalancing();
    benchmarkPathSum();

    std::cout << "======================================================" << std::endl;
    std::cout << "=== End of DAY 28 Demonstrations                   ===" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}