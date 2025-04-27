# 🔍 DAY 28: Tree Problems Weekly Challenge - Mastering Tree Algorithms #DSAin45

Welcome to Day 28 of our 45-day DSA journey! Today marks the end of our fourth week, where we've explored various tree data structures. Let's consolidate our knowledge by tackling common tree problem patterns and solutions that appear frequently in technical interviews.

_Trees might seem simple at first glance, but they're the source of some of the most elegant and powerful algorithms in computer science. Today, we'll explore how to approach these problems systematically, building problem-solving patterns that will make even the most intimidating tree problems manageable and intuitive._

## 📌 Recap of Tree Data Structures

Before diving into problem-solving, let's briefly review the tree data structures we've covered this week:

1. **Binary Trees** (Day 22): A hierarchical structure where each node has at most two children.
2. **Binary Search Trees** (Day 23): Binary trees with the property that all left subtree values < node value < all right subtree values.
3. **AVL Trees** (Day 24): Self-balancing BSTs that maintain height balance using rotations.
4. **Heap** (Day 25): Complete binary trees that satisfy the heap property (max-heap or min-heap).
5. **Heap Sort & Applications** (Day 26): Using heaps for efficient sorting and other applications.
6. **Trie** (Day 27): Tree-like structures optimized for string operations and prefix searching.

## 🧠 Tree Problem-Solving Framework

Let's develop a systematic approach to solving tree problems:

### 1. Identify the Problem Type

Tree problems typically fall into these categories:

| Problem Type                   | Description                                     | Example Problems                                       |
| ------------------------------ | ----------------------------------------------- | ------------------------------------------------------ |
| **Traversal Problems**         | Visiting nodes in a specific order              | Inorder, preorder, postorder, level-order traversal    |
| **Path Problems**              | Finding specific paths in the tree              | Path sum, all paths, binary tree paths                 |
| **Property Verification**      | Checking if a tree satisfies certain properties | Is balanced, is symmetric, is same tree                |
| **Construction Problems**      | Building trees from given data                  | Construct from preorder/inorder, serialize/deserialize |
| **Node Relationship Problems** | Finding relationships between nodes             | Lowest common ancestor, distance between nodes         |
| **Modification Problems**      | Modifying the tree structure                    | Flatten binary tree, invert binary tree                |

### 2. Choose the Right Traversal Strategy

Based on the problem type, select the appropriate traversal method:

- **Depth-First Search (DFS)**:

  - **Preorder (Root → Left → Right)**: Good for creating copies, exploring paths
  - **Inorder (Left → Root → Right)**: Gives sorted output for BSTs
  - **Postorder (Left → Right → Root)**: Good for deletion, bottom-up calculations

- **Breadth-First Search (BFS)**:
  - **Level-order**: Good for level-by-level processing, nearest neighbor problems

### 3. Define the Recursive Structure or Iterative Approach

Most tree problems can be elegantly solved using recursion, but iterative approaches are sometimes necessary for space optimization.

### 4. Identify Base Cases and Edge Cases

Common base cases:

- Empty tree (null node)
- Leaf node (no children)
- Single node tree

Common edge cases:

- Skewed trees (all left or all right children)
- Trees with duplicate values
- Trees with negative values (for sum problems)

### 5. Implement the Solution with Clear, Concise Code

Write clean code with descriptive variable names and comments for complex logic.

## 🔢 Common Tree Problem Patterns

Let's explore key patterns for solving different types of tree problems:

### 1. The "Bottom-Up" Pattern

**When to use**: Problems where information from child nodes needs to be combined.

**Examples**: Maximum depth, diameter, is balanced.

**Implementation pattern**:

```cpp
Result function(TreeNode* root) {
    // Base case
    if (!root) return baseValue;

    // Get results from children
    Result leftResult = function(root->left);
    Result rightResult = function(root->right);

    // Combine results and return
    return combineResults(leftResult, rightResult, root);
}
```

**Example Problem: Maximum Depth of Binary Tree**

```cpp
int maxDepth(TreeNode* root) {
    if (!root) return 0;

    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);

    return 1 + max(leftDepth, rightDepth);
}
```

### 2. The "Top-Down" Pattern

**When to use**: Problems where information flows from parent to children.

**Examples**: Path sum, build paths, preorder traversal.

**Implementation pattern**:

```cpp
void function(TreeNode* root, State state) {
    // Base case
    if (!root) return;

    // Update state with current node
    updateState(state, root);

    // Check if we've reached a goal
    if (isGoal(state, root)) {
        processResult(state);
        return;
    }

    // Recurse on children with updated state
    function(root->left, state);
    function(root->right, state);
}
```

**Example Problem: Path Sum**

```cpp
bool hasPathSum(TreeNode* root, int targetSum) {
    if (!root) return false;

    // If leaf node, check if path sum equals target
    if (!root->left && !root->right) {
        return root->val == targetSum;
    }

    // Check if either path to left or right child has the target sum
    return hasPathSum(root->left, targetSum - root->val) ||
           hasPathSum(root->right, targetSum - root->val);
}
```

### 3. The "Meet in the Middle" Pattern

**When to use**: Problems involving finding common ancestors or paths between nodes.

**Examples**: Lowest common ancestor, distance between nodes.

**Implementation pattern**:

```cpp
TreeNode* function(TreeNode* root, TreeNode* p, TreeNode* q) {
    // Base case
    if (!root || root == p || root == q) return root;

    // Look in left and right subtrees
    TreeNode* left = function(root->left, p, q);
    TreeNode* right = function(root->right, p, q);

    // Combine results
    if (left && right) return root;  // Found in both subtrees, this is the meeting point
    return left ? left : right;      // Return non-null result
}
```

**Example Problem: Lowest Common Ancestor**

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;

    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    if (left && right) return root;
    return left ? left : right;
}
```

### 4. The "Level-by-Level" Pattern

**When to use**: Problems requiring processing nodes by level.

**Examples**: Level order traversal, right side view, zigzag traversal.

**Implementation pattern**:

```cpp
vector<vector<int>> function(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel;

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            // Process node
            currentLevel.push_back(node->val);

            // Add children to queue
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        result.push_back(currentLevel);
    }

    return result;
}
```

**Example Problem: Binary Tree Level Order Traversal**

```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel;

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            currentLevel.push_back(node->val);

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        result.push_back(currentLevel);
    }

    return result;
}
```

### 5. The "Tree Construction" Pattern

**When to use**: Problems involving building trees from traversal data.

**Examples**: Construct from preorder/inorder, serialize/deserialize.

**Implementation pattern**:

```cpp
TreeNode* buildTree(vector<Type>& data, int start, int end, AdditionalParams params) {
    // Base case
    if (start > end) return nullptr;

    // Create root node
    int rootValue = findRootValue(data, start, end, params);
    TreeNode* root = new TreeNode(rootValue);

    // Find dividing point for left and right subtrees
    int dividePoint = findDividePoint(data, start, end, rootValue, params);

    // Recursively build left and right subtrees
    root->left = buildTree(data, start, dividePoint - 1, updatedParams);
    root->right = buildTree(data, dividePoint + 1, end, updatedParams);

    return root;
}
```

**Example Problem: Construct Binary Tree from Preorder and Inorder Traversal**

```cpp
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    unordered_map<int, int> inorderMap;
    for (int i = 0; i < inorder.size(); i++) {
        inorderMap[inorder[i]] = i;
    }

    return buildTreeHelper(preorder, 0, preorder.size() - 1,
                           inorder, 0, inorder.size() - 1, inorderMap);
}

TreeNode* buildTreeHelper(vector<int>& preorder, int preStart, int preEnd,
                          vector<int>& inorder, int inStart, int inEnd,
                          unordered_map<int, int>& inorderMap) {
    if (preStart > preEnd) return nullptr;

    // Root is the first element in preorder traversal
    int rootVal = preorder[preStart];
    TreeNode* root = new TreeNode(rootVal);

    // Find position of root in inorder traversal
    int rootInorderIdx = inorderMap[rootVal];

    // Calculate size of left subtree
    int leftSubtreeSize = rootInorderIdx - inStart;

    // Recursively build left and right subtrees
    root->left = buildTreeHelper(preorder, preStart + 1, preStart + leftSubtreeSize,
                                inorder, inStart, rootInorderIdx - 1, inorderMap);
    root->right = buildTreeHelper(preorder, preStart + leftSubtreeSize + 1, preEnd,
                                 inorder, rootInorderIdx + 1, inEnd, inorderMap);

    return root;
}
```

## 🧩 Tree Problem Solutions

Let's solve five classic tree problems step by step, applying our patterns:

### Problem 1: Is Binary Tree Symmetric?

**Problem Description**: Given a binary tree, check whether it is a mirror of itself (i.e., symmetric around its center).

**Example**:

```
    1
   / \
  2   2
 / \ / \
3  4 4  3
```

This tree is symmetric.

**Approach**: Use the "Bottom-Up" pattern with a helper function that checks if two subtrees are mirrors of each other.

**Solution**:

```cpp
bool isSymmetric(TreeNode* root) {
    if (!root) return true;
    return isMirror(root->left, root->right);
}

bool isMirror(TreeNode* left, TreeNode* right) {
    // Both null, they're symmetric
    if (!left && !right) return true;

    // One is null but other isn't, they're not symmetric
    if (!left || !right) return false;

    // Check if values match and subtrees are mirrors
    return (left->val == right->val) &&
           isMirror(left->left, right->right) &&  // outer branches
           isMirror(left->right, right->left);    // inner branches
}
```

**Time Complexity**: O(n) where n is the number of nodes.
**Space Complexity**: O(h) where h is the height of the tree, for the recursion stack.

### Problem 2: Validate Binary Search Tree

**Problem Description**: Given a binary tree, determine if it is a valid binary search tree (BST).

**Example**:

```
    2
   / \
  1   3
```

This is a valid BST.

**Approach**: Use "Top-Down" pattern with range limits that narrow as we go down the tree.

**Solution**:

```cpp
bool isValidBST(TreeNode* root) {
    return isValidBSTHelper(root, nullptr, nullptr);
}

bool isValidBSTHelper(TreeNode* root, TreeNode* minNode, TreeNode* maxNode) {
    if (!root) return true;

    // Check if current node's value is within allowed range
    if ((minNode && root->val <= minNode->val) ||
        (maxNode && root->val >= maxNode->val)) {
        return false;
    }

    // Recursively check left and right subtrees with updated ranges
    return isValidBSTHelper(root->left, minNode, root) &&
           isValidBSTHelper(root->right, root, maxNode);
}
```

**Time Complexity**: O(n) where n is the number of nodes.
**Space Complexity**: O(h) where h is the height of the tree.

### Problem 3: Binary Tree Right Side View

**Problem Description**: Given a binary tree, imagine yourself standing on the right side of it, return the values of the nodes you can see ordered from top to bottom.

**Example**:

```
    1            <---
   / \
  2   3          <---
   \   \
    5   4        <---
```

Right side view: [1, 3, 4]

**Approach**: Use the "Level-by-Level" pattern, keeping track of the rightmost node at each level.

**Solution**:

```cpp
vector<int> rightSideView(TreeNode* root) {
    vector<int> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            // If it's the last node in the level, add to result
            if (i == levelSize - 1) {
                result.push_back(node->val);
            }

            // Add children to queue (left first, right second)
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    return result;
}
```

**Time Complexity**: O(n) where n is the number of nodes.
**Space Complexity**: O(d) where d is the tree's maximum width.

### Problem 4: Path Sum III

**Problem Description**: Given a binary tree and a sum, find the number of paths where the sum of the values along the path equals the given sum. The path doesn't need to start at the root or end at a leaf.

**Example**:

```
      10
     /  \
    5   -3
   / \    \
  3   2   11
 / \   \
3  -2   1
```

sum = 8, result = 3 (paths: 5->3, 5->2->1, -3->11)

**Approach**: Combine "Top-Down" pattern with a prefix sum technique.

**Solution**:

```cpp
int pathSum(TreeNode* root, int targetSum) {
    unordered_map<long, int> prefixSumCount;
    prefixSumCount[0] = 1;  // Empty path has sum 0
    return countPaths(root, 0, targetSum, prefixSumCount);
}

int countPaths(TreeNode* node, long currentSum, int targetSum,
               unordered_map<long, int>& prefixSumCount) {
    if (!node) return 0;

    // Add current node value to running sum
    currentSum += node->val;

    // Check if there are any prefix sums that can be removed to get targetSum
    int numPaths = prefixSumCount[currentSum - targetSum];

    // Update prefix sum count
    prefixSumCount[currentSum]++;

    // Recursively count paths in left and right subtrees
    int result = numPaths +
                countPaths(node->left, currentSum, targetSum, prefixSumCount) +
                countPaths(node->right, currentSum, targetSum, prefixSumCount);

    // Backtrack: remove current node's contribution to the prefix sum
    prefixSumCount[currentSum]--;

    return result;
}
```

**Time Complexity**: O(n) where n is the number of nodes.
**Space Complexity**: O(h) where h is the height of the tree.

### Problem 5: Serialize and Deserialize Binary Tree

**Problem Description**: Design an algorithm to serialize and deserialize a binary tree.

**Approach**: Use preorder traversal for serialization and a queue-based approach for deserialization.

**Solution**:

```cpp
class Codec {
public:
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        ostringstream out;
        serialize(root, out);
        return out.str();
    }

    // Recursive helper function for serialization
    void serialize(TreeNode* root, ostringstream& out) {
        if (!root) {
            out << "# ";  // Use # to denote null nodes
            return;
        }

        out << root->val << " ";
        serialize(root->left, out);
        serialize(root->right, out);
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserialize(in);
    }

    // Recursive helper function for deserialization
    TreeNode* deserialize(istringstream& in) {
        string val;
        in >> val;

        if (val == "#") return nullptr;

        TreeNode* root = new TreeNode(stoi(val));
        root->left = deserialize(in);
        root->right = deserialize(in);

        return root;
    }
};
```

**Time Complexity**: O(n) for both operations where n is the number of nodes.
**Space Complexity**: O(n) for the serialized string and O(h) for the recursion stack during deserialization.

## 😄 Tree Problem Jokes and Analogies

1. **The Tree Traversal GPS Analogy**:
   "Tree algorithms are like different GPS navigation styles. Preorder is the 'announce each turn before you make it' navigator. Inorder is the 'announce when you're at the intersection' navigator. Postorder is the 'wait until you've passed the turn to mention it' navigator. And BFS is the 'let's check every intersection at this street before moving to the next street' navigator."

2. **The Recursion Therapist Joke**:
   "Why are recursive tree algorithms so emotionally stable? Because they always know when to return!"

3. **The Binary Tree Garden Analogy**:
   "Solving tree problems is like gardening. Sometimes you need to prune from the bottom up (postorder), sometimes you need to water from the top down (preorder), and sometimes you need to check one side before moving to the other (inorder). And just like gardening, the key is patience and understanding the natural structure!"

4. **The Family Tree Joke**:
   "I tried to explain the Lowest Common Ancestor problem to my family using our family tree. Now my uncle thinks he's a binary node and keeps asking which of his children has a higher priority value."

5. **The Tree Balancing Act Analogy**:
   "Keeping a binary search tree balanced is like being a parent with twins. Just when you think you've given equal attention to the left child, the right one grows another level, and suddenly you're performing rotations to restore balance in the household!"

## 🔍 Performance: Tree Problem-Solving Time Complexity

| Problem Type      | Typical Time Complexity | Space Complexity | Notes                                |
| ----------------- | ----------------------- | ---------------- | ------------------------------------ |
| Tree Traversal    | O(n)                    | O(h)             | h = height (can be O(n) worst case)  |
| Path Finding      | O(n)                    | O(h)             | Often requires backtracking          |
| Tree Construction | O(n)                    | O(n)             | May need lookup tables               |
| Property Checking | O(n)                    | O(h)             | Early termination sometimes possible |
| LCA Finding       | O(n)                    | O(h)             | More efficient for BSTs: O(h)        |
| Serialization     | O(n)                    | O(n)             | For string representation            |

## 🧩 Weekly Challenge Problems

Try to solve these problems on your own before looking at the solutions:

### 1. Invert Binary Tree (Easy)

**Problem Description**: Invert a binary tree.

**Example**:

```
Input:
     4
   /   \
  2     7
 / \   / \
1   3 6   9

Output:
     4
   /   \
  7     2
 / \   / \
9   6 3   1
```

**Solution**: Use a simple recursive approach with the "Bottom-Up" pattern.

```cpp
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;

    // Invert left and right subtrees
    TreeNode* left = invertTree(root->left);
    TreeNode* right = invertTree(root->right);

    // Swap them
    root->left = right;
    root->right = left;

    return root;
}
```

### 2. Diameter of Binary Tree (Easy)

**Problem Description**: Given a binary tree, compute the length of the diameter (the longest path between any two nodes).

**Example**:

```
     1
    / \
   2   3
  / \
 4   5
```

Diameter is 3 (path: 4->2->1->3 or 5->2->1->3)

**Solution**: Use the "Bottom-Up" pattern, calculating height while finding diameter.

```cpp
int diameterOfBinaryTree(TreeNode* root) {
    int diameter = 0;
    calculateHeight(root, diameter);
    return diameter;
}

int calculateHeight(TreeNode* node, int& diameter) {
    if (!node) return 0;

    // Calculate height of left and right subtrees
    int leftHeight = calculateHeight(node->left, diameter);
    int rightHeight = calculateHeight(node->right, diameter);

    // Update diameter if path through this node is longer
    diameter = max(diameter, leftHeight + rightHeight);

    // Return height of this subtree
    return 1 + max(leftHeight, rightHeight);
}
```

### 3. Construct Binary Tree from Inorder and Postorder Traversal (Medium)

**Problem Description**: Given inorder and postorder traversal of a tree, construct the binary tree.

**Example**:

```
Inorder: [9, 3, 15, 20, 7]
Postorder: [9, 15, 7, 20, 3]

Output:
    3
   / \
  9  20
    /  \
   15   7
```

**Solution**: Use the "Tree Construction" pattern.

```cpp
TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
    unordered_map<int, int> inorderMap;
    for (int i = 0; i < inorder.size(); i++) {
        inorderMap[inorder[i]] = i;
    }

    return buildTreeHelper(postorder, 0, postorder.size() - 1,
                           inorder, 0, inorder.size() - 1, inorderMap);
}

TreeNode* buildTreeHelper(vector<int>& postorder, int postStart, int postEnd,
                          vector<int>& inorder, int inStart, int inEnd,
                          unordered_map<int, int>& inorderMap) {
    if (postStart > postEnd) return nullptr;

    // Root is the last element in postorder traversal
    int rootVal = postorder[postEnd];
    TreeNode* root = new TreeNode(rootVal);

    // Find position of root in inorder traversal
    int rootInorderIdx = inorderMap[rootVal];

    // Calculate size of left subtree
    int leftSubtreeSize = rootInorderIdx - inStart;

    // Recursively build left and right subtrees
    root->left = buildTreeHelper(postorder, postStart, postStart + leftSubtreeSize - 1,
                                inorder, inStart, rootInorderIdx - 1, inorderMap);
    root->right = buildTreeHelper(postorder, postStart + leftSubtreeSize, postEnd - 1,
                                 inorder, rootInorderIdx + 1, inEnd, inorderMap);

    return root;
}
```

### 4. Binary Tree Zigzag Level Order Traversal (Medium)

**Problem Description**: Given a binary tree, return the zigzag level order traversal of its nodes' values. (i.e., from left to right, then right to left for the next level and alternate between).

**Example**:

```
    3
   / \
  9  20
    /  \
   15   7

Output:
[
  [3],
  [20, 9],
  [15, 7]
]
```

**Solution**: Use the "Level-by-Level" pattern with a direction flag.

```cpp
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel(levelSize);

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            // Position in the level based on direction
            int index = leftToRight ? i : levelSize - 1 - i;
            currentLevel[index] = node->val;

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        result.push_back(currentLevel);
        leftToRight = !leftToRight; // Flip direction for next level
    }

    return result;
}
```

### 5. Recover Binary Search Tree (Hard)

**Problem Description**: You are given the root of a binary search tree (BST), where exactly two nodes were swapped by mistake. Recover the tree without changing its structure.

**Solution**: Use inorder traversal with the "Bottom-Up" pattern to find the swapped nodes.

```cpp
class Solution {
public:
    void recoverTree(TreeNode* root) {
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;
        TreeNode* prev = nullptr;

        // Inorder traversal to find swapped nodes
        inorderTraversal(root, prev, first, second);

        // Swap the values of the two nodes
        if (first && second) {
            swap(first->val, second->val);
        }
    }

private:
    void inorderTraversal(TreeNode* root, TreeNode*& prev, TreeNode*& first, TreeNode*& second) {
        if (!root) return;

        // Traverse left subtree
        inorderTraversal(root->left, prev, first, second);

        // Check current node with previous node
        if (prev && prev->val > root->val) {
            // First violation: mark both nodes
            if (!first) {
                first = prev;
                second = root;
            } else {
                // Second violation: mark only the current node
                second = root;
            }
        }

        // Update prev pointer
        prev = root;

        // Traverse right subtree
        inorderTraversal(root->right, prev, first, second);
    }
};
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll begin an exciting new week focused on Graph algorithms, starting with Graph Fundamentals. We'll learn:

- How graphs represent relationships between entities
- Different ways to represent graphs (adjacency matrix, adjacency list)
- Basic graph properties and terminology
- Implementing graphs efficiently in C++
- Common operations on graphs
- Real-world applications like social networks, maps, and recommendation systems

_If trees are hierarchical parent-child relationships, graphs are the more complex social networks where any entity can connect to any other. Graphs are one of the most versatile data structures, capable of modeling everything from road networks to social media connections, and even the entire internet. Understanding graphs will unlock solutions to a whole new class of problems that go beyond what trees can represent._

Stay tuned for our journey into the fascinating world of graphs!

#DSAin45 #Day28 #TreeProblems #BinaryTree #Algorithms #CPlusPlus #CodingInterviews
