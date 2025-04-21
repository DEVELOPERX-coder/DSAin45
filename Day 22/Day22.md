# 🌳 DAY 22: Binary Trees Basics - Branching into Hierarchical Data Structures #DSAin45

Welcome to Day 22 and the start of Week 4 in our 45-day DSA journey! We're transitioning from linear data structures and algorithms to hierarchical ones, beginning with one of the most fundamental non-linear data structures: Binary Trees.

_Think of a binary tree as a family tree that follows a strict two-child policy. Each parent can have at most two children, creating a hierarchy that branches out elegantly with each generation—making relationships clear and searchable in ways that linear structures simply cannot achieve._

## 📌 Understanding Binary Trees

A binary tree is a hierarchical data structure in which each node has at most two children, referred to as the left child and the right child. This simple constraint creates a powerful structure that forms the foundation for many advanced data structures and algorithms.

### Tree Terminology

Before diving deeper, let's familiarize ourselves with the essential terminology:

- **Node**: Each element in a tree containing data and pointers to its children
- **Root**: The topmost node of the tree
- **Child**: A node directly connected to another node when moving away from the root
- **Parent**: The converse of a child; a node that has other nodes connected below it
- **Leaf Node**: A node with no children
- **Internal Node**: A node with at least one child
- **Siblings**: Nodes that share the same parent
- **Depth**: The number of edges from the root to a specific node
- **Height**: The number of edges on the longest path from a node to a leaf
- **Height of Tree**: The height of the root node (or the maximum depth)
- **Subtree**: A tree consisting of a node and all its descendants

### Visual Representation

```
        1      ← Root
       / \
      2   3    ← Internal Nodes
     / \   \
    4   5   6  ← Leaf Nodes (4, 5, 6)
```

In this tree:

- 1 is the root
- 2 and 3 are children of 1
- 4 and 5 are children of 2
- 6 is a child of 3
- 4, 5, and 6 are leaf nodes
- The height of the tree is 2

## 💻 Binary Tree Implementation

Let's start by implementing the basic structure of a binary tree in C++:

```cpp
// Definition for a binary tree node
struct TreeNode {
    int val;           // The value/data stored in the node
    TreeNode* left;    // Pointer to the left child
    TreeNode* right;   // Pointer to the right child

    // Constructor
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

This simple structure forms the building block for our binary tree. Each node contains:

- A value (in this case, an integer)
- A pointer to the left child
- A pointer to the right child

### Creating a Simple Binary Tree

Let's create a basic binary tree manually:

```cpp
TreeNode* createSampleTree() {
    /*
        1
       / \
      2   3
     / \   \
    4   5   6
    */

    // Create all nodes
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);

    return root;
}
```

This function creates the tree we visualized earlier. In a real-world scenario, trees would typically be constructed dynamically based on input data, but this gives us a starting point for our examples.

### Memory Management

When working with trees, proper memory management is crucial to prevent memory leaks:

```cpp
void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // Post-order deletion
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
```

This function recursively deletes all nodes in the tree using a post-order traversal approach (which we'll discuss in detail later). The key takeaway is that we must delete child nodes before deleting the parent to avoid losing references to memory that needs to be freed.

## 🔄 Tree Traversal Techniques

One of the most important operations on trees is traversal - the process of visiting each node in the tree exactly once. Unlike linear data structures that have a single way to traverse (from start to end), trees offer multiple traversal patterns, each with its specific use cases.

### Depth-First Traversals

Depth-first traversals explore as far down a branch as possible before backtracking. There are three main types of depth-first traversals:

#### 1. Pre-order Traversal (Root-Left-Right)

In a pre-order traversal, we:

1. Visit the current node
2. Recursively traverse the left subtree
3. Recursively traverse the right subtree

```cpp
void preorderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // Visit the current node
    std::cout << root->val << " ";

    // Traverse left subtree
    preorderTraversal(root->left);

    // Traverse right subtree
    preorderTraversal(root->right);
}
```

For our sample tree, the pre-order traversal would visit nodes in the sequence: 1, 2, 4, 5, 3, 6

Applications of pre-order traversal:

- Creating a copy of the tree
- Prefix expression evaluation (Polish notation)
- Generating a file directory listing

#### 2. In-order Traversal (Left-Root-Right)

In an in-order traversal, we:

1. Recursively traverse the left subtree
2. Visit the current node
3. Recursively traverse the right subtree

```cpp
void inorderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // Traverse left subtree
    inorderTraversal(root->left);

    // Visit the current node
    std::cout << root->val << " ";

    // Traverse right subtree
    inorderTraversal(root->right);
}
```

For our sample tree, the in-order traversal would visit nodes in the sequence: 4, 2, 5, 1, 3, 6

Applications of in-order traversal:

- Getting values in sorted order from a Binary Search Tree (BST)
- Converting a tree to a linear data structure while preserving order
- Infix expression evaluation

#### 3. Post-order Traversal (Left-Right-Root)

In a post-order traversal, we:

1. Recursively traverse the left subtree
2. Recursively traverse the right subtree
3. Visit the current node

```cpp
void postorderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // Traverse left subtree
    postorderTraversal(root->left);

    // Traverse right subtree
    postorderTraversal(root->right);

    // Visit the current node
    std::cout << root->val << " ";
}
```

For our sample tree, the post-order traversal would visit nodes in the sequence: 4, 5, 2, 6, 3, 1

Applications of post-order traversal:

- Deleting a tree (as we saw in our `deleteTree` function)
- Evaluating postfix expressions
- Computing the size or height of a tree
- Finding the longest path from root to leaf

### Breadth-First Traversal (Level-Order)

In contrast to depth-first approaches, breadth-first traversal visits all nodes at the current depth before moving to nodes at the next depth level. This is also known as level-order traversal.

```cpp
void levelOrderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // Use a queue for level-order traversal
    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        // Get the front node from the queue
        TreeNode* current = q.front();
        q.pop();

        // Visit the current node
        std::cout << current->val << " ";

        // Enqueue the left child if it exists
        if (current->left) {
            q.push(current->left);
        }

        // Enqueue the right child if it exists
        if (current->right) {
            q.push(current->right);
        }
    }
}
```

For our sample tree, the level-order traversal would visit nodes in the sequence: 1, 2, 3, 4, 5, 6

Applications of level-order traversal:

- Finding the minimum depth of a binary tree
- Connecting nodes at the same level
- Creating a level-wise representation of a tree
- Solving problems that require processing level by level (e.g., zigzag traversal)

### Iterative Implementations of DFS

While recursive implementations are elegant, they can lead to stack overflow for deep trees. Iterative versions using explicit stacks can be more efficient:

#### Iterative Pre-order Traversal

```cpp
std::vector<int> preorderTraversalIterative(TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<TreeNode*> s;
    s.push(root);

    while (!s.empty()) {
        TreeNode* current = s.top();
        s.pop();

        // Visit the current node
        result.push_back(current->val);

        // Push right child first so that left child is processed first (LIFO)
        if (current->right) {
            s.push(current->right);
        }
        if (current->left) {
            s.push(current->left);
        }
    }

    return result;
}
```

#### Iterative In-order Traversal

```cpp
std::vector<int> inorderTraversalIterative(TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<TreeNode*> s;
    TreeNode* current = root;

    while (current != nullptr || !s.empty()) {
        // Reach the leftmost node of the current subtree
        while (current != nullptr) {
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
```

#### Iterative Post-order Traversal

Post-order traversal is trickier to implement iteratively. One approach uses two stacks:

```cpp
std::vector<int> postorderTraversalIterative(TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<TreeNode*> s1, s2;
    s1.push(root);

    // First stack helps us to populate the second stack in reverse post-order
    while (!s1.empty()) {
        TreeNode* current = s1.top();
        s1.pop();
        s2.push(current);

        if (current->left) {
            s1.push(current->left);
        }
        if (current->right) {
            s1.push(current->right);
        }
    }

    // Second stack now has nodes in post-order, but reversed
    while (!s2.empty()) {
        result.push_back(s2.top()->val);
        s2.pop();
    }

    return result;
}
```

## 📏 Properties and Types of Binary Trees

Binary trees come in various forms, each with specific properties:

### 1. Full Binary Tree

A full binary tree is a binary tree in which every node has either 0 or 2 children.

```
    1
   / \
  2   3
 / \
4   5
```

### 2. Complete Binary Tree

A complete binary tree is a binary tree in which every level, except possibly the last, is completely filled, and all nodes are as far left as possible.

```
    1
   / \
  2   3
 / \  /
4   5 6
```

### 3. Perfect Binary Tree

A perfect binary tree is a binary tree in which all internal nodes have exactly two children and all leaf nodes are at the same level.

```
    1
   / \
  2   3
 / \ / \
4  5 6  7
```

### 4. Balanced Binary Tree

A balanced binary tree is a binary tree in which the height of the left and right subtree of any node differs by at most one.

```
    1
   / \
  2   3
 /   / \
4   5   6
```

### 5. Degenerate (or Pathological) Tree

A degenerate tree is a tree where each parent node has only one child node, essentially becoming a linked list.

```
1
 \
  2
   \
    3
     \
      4
```

## 🧮 Binary Tree Operations

Let's look at some common operations on binary trees:

### 1. Finding the Height of a Binary Tree

```cpp
int height(TreeNode* root) {
    if (root == nullptr) {
        return -1; // Height of an empty tree is -1
    }

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    return std::max(leftHeight, rightHeight) + 1;
}
```

### 2. Counting the Number of Nodes

```cpp
int countNodes(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    return 1 + countNodes(root->left) + countNodes(root->right);
}
```

### 3. Counting Leaf Nodes

```cpp
int countLeafNodes(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    // If this is a leaf node
    if (root->left == nullptr && root->right == nullptr) {
        return 1;
    }

    return countLeafNodes(root->left) + countLeafNodes(root->right);
}
```

### 4. Checking if Two Trees are Identical

```cpp
bool areIdentical(TreeNode* root1, TreeNode* root2) {
    // If both trees are empty, they're identical
    if (root1 == nullptr && root2 == nullptr) {
        return true;
    }

    // If one is empty but the other isn't, they're not identical
    if (root1 == nullptr || root2 == nullptr) {
        return false;
    }

    // Check if current nodes and both subtrees are identical
    return (root1->val == root2->val &&
            areIdentical(root1->left, root2->left) &&
            areIdentical(root1->right, root2->right));
}
```

### 5. Finding the Diameter of a Binary Tree

The diameter (or width) of a binary tree is the number of nodes on the longest path between any two leaf nodes.

```cpp
int diameterOfBinaryTree(TreeNode* root) {
    int diameter = 0;
    calculateHeight(root, diameter);
    return diameter;
}

int calculateHeight(TreeNode* root, int& diameter) {
    if (root == nullptr) {
        return 0;
    }

    int leftHeight = calculateHeight(root->left, diameter);
    int rightHeight = calculateHeight(root->right, diameter);

    // Update diameter if needed
    diameter = std::max(diameter, leftHeight + rightHeight);

    // Return height of current subtree
    return std::max(leftHeight, rightHeight) + 1;
}
```

### 6. Checking if a Binary Tree is Balanced

```cpp
bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
}

int checkHeight(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    int leftHeight = checkHeight(root->left);
    if (leftHeight == -1) {
        return -1; // Left subtree is unbalanced
    }

    int rightHeight = checkHeight(root->right);
    if (rightHeight == -1) {
        return -1; // Right subtree is unbalanced
    }

    // Check if current node is balanced
    if (abs(leftHeight - rightHeight) > 1) {
        return -1; // Unbalanced
    }

    // Return height of current subtree
    return std::max(leftHeight, rightHeight) + 1;
}
```

## 🌎 Real-World Applications of Binary Trees

Binary trees form the foundation for numerous data structures and algorithms used across various domains:

### 1. File Systems

Operating systems use tree structures to organize directories and files. Each directory can contain files and/or other directories, creating a hierarchical structure.

### 2. Database Indexing

B-trees and their variants (B+ trees, B\* trees) are used for efficient index implementation in databases. These structures allow quick insertions, deletions, and searches.

### 3. Network Routing

Routing algorithms use tree-like structures to find optimal paths in networks. The shortest path tree, for example, helps determine the best route from a source to all other nodes.

### 4. Syntax Analysis

Compilers use parse trees and abstract syntax trees to analyze and represent the grammatical structure of program code.

### 5. Decision Trees in Machine Learning

Decision trees partition the data space into regions, with each internal node representing a feature-based decision rule and each leaf node corresponding to a predicted outcome.

### 6. Huffman Coding for Compression

Huffman trees are used in data compression algorithms to assign variable-length codes to characters based on their frequencies.

### 7. Game AI

In game development, techniques like Minimax with Alpha-Beta pruning use tree structures to represent possible game states and determine optimal moves.

### 8. Expression Evaluation

Binary expression trees represent mathematical expressions, with operators as internal nodes and operands as leaf nodes, facilitating evaluation and transformation.

## 🧠 Different Ways to Represent Binary Trees

While we've focused on linked representation using pointers, there are alternative ways to represent binary trees:

### 1. Array Representation

For a complete binary tree, we can use an array where:

- The root is at index 0
- For a node at index i:
  - Its left child is at index 2i + 1
  - Its right child is at index 2i + 2
  - Its parent is at index (i-1)/2 (integer division)

```cpp
class ArrayBinaryTree {
private:
    std::vector<int> tree;

public:
    ArrayBinaryTree(int root) {
        tree.push_back(root);
    }

    void setLeft(int parentIndex, int value) {
        int leftIndex = 2 * parentIndex + 1;

        // Ensure the array is big enough
        if (leftIndex >= tree.size()) {
            tree.resize(leftIndex + 1, -1); // Use -1 to represent empty spots
        }

        tree[leftIndex] = value;
    }

    void setRight(int parentIndex, int value) {
        int rightIndex = 2 * parentIndex + 2;

        // Ensure the array is big enough
        if (rightIndex >= tree.size()) {
            tree.resize(rightIndex + 1, -1);
        }

        tree[rightIndex] = value;
    }

    void print() {
        for (int i = 0; i < tree.size(); i++) {
            if (tree[i] != -1) {
                std::cout << "Index " << i << ": " << tree[i] << std::endl;
            }
        }
    }
};
```

The array representation is particularly useful for heaps and complete binary trees, as it avoids the overhead of pointer management and provides excellent cache locality.

### 2. Left-Child Right-Sibling Representation

This representation can be used to represent trees with any number of children per node (not just binary trees). Each node has two pointers:

- A pointer to the leftmost child
- A pointer to the next sibling

```cpp
struct MultiWayTreeNode {
    int val;
    MultiWayTreeNode* firstChild;    // Points to the first (leftmost) child
    MultiWayTreeNode* nextSibling;   // Points to the next sibling

    MultiWayTreeNode(int x) : val(x), firstChild(nullptr), nextSibling(nullptr) {}
};
```

This representation simplifies certain tree operations, particularly when dealing with trees with variable numbers of children per node.

## 😄 Binary Tree Jokes and Analogies

1. **The Family Tree Analogy**:
   "Binary trees are like strict traditional families where each parent can have at most two children. Unlike real families, though, deleting the root doesn't cause inheritance disputes—it just turns the whole structure into a forest!"

2. **The Recursive Joke**:
   "How does a recursive function eat a binary tree? It starts at the root, takes a byte out of it, and then recursively eats the left and right subtrees."

3. **The Tree Traversal Joke**:
   "A pre-order, in-order, and post-order traversal walk into a bar. The bartender asks, 'What'll it be?' The pre-order says, 'I'll order first, then my left friend, then my right friend.' The in-order says, 'My left friend will order, then me, then my right friend.' The post-order says, 'My left friend will order, then my right friend, then I'll pay the bill!'"

4. **The Balance Analogy**:
   "Balanced binary trees are like perfectly balanced scales. Add too many nodes to one side, and your operations start tipping from O(log n) to O(n), making your algorithm as efficient as a linear search through a linked list!"

5. **The Tree Rotation Joke**:
   "What do you call a binary tree after a workout? A 'rotated' tree!"

6. **The Leaf Node Pickup Line**:
   "Are you a leaf node? Because I don't see any children in your future, just a promising connection with me!"

7. **The Computer Science Family Analogy**:
   "In a binary tree family, the root node constantly worries about maintaining balance between its left and right children. If it succeeds, searches through the family directory take logarithmic time. If it fails, everyone waits in linear time while Grandma looks through her contact list one by one."

8. **The Binary Tree Book Analogy**:
   "A binary tree is like a choose-your-own-adventure book. At each node, you make one of two choices, leading to a different path through the story. Except in this book, efficient searching means you can skip boring chapters entirely!"

## 🚀 LeetCode Practice Problems

### Easy:

1. **Maximum Depth of Binary Tree** (LeetCode #104)

   - Find the maximum depth (height) of a binary tree.

   ```cpp
   int maxDepth(TreeNode* root) {
       if (root == nullptr) {
           return 0;
       }

       int leftDepth = maxDepth(root->left);
       int rightDepth = maxDepth(root->right);

       return std::max(leftDepth, rightDepth) + 1;
   }
   ```

2. **Invert Binary Tree** (LeetCode #226)

   - Invert a binary tree (mirror it).

   ```cpp
   TreeNode* invertTree(TreeNode* root) {
       if (root == nullptr) {
           return nullptr;
       }

       // Swap the left and right subtrees
       TreeNode* temp = root->left;
       root->left = root->right;
       root->right = temp;

       // Recursively invert the subtrees
       invertTree(root->left);
       invertTree(root->right);

       return root;
   }
   ```

3. **Same Tree** (LeetCode #100)

   - Check if two binary trees are identical.

   ```cpp
   bool isSameTree(TreeNode* p, TreeNode* q) {
       // If both trees are empty, they're identical
       if (p == nullptr && q == nullptr) {
           return true;
       }

       // If one is empty but the other isn't, they're not identical
       if (p == nullptr || q == nullptr) {
           return false;
       }

       // Check if current nodes and both subtrees are identical
       return (p->val == q->val &&
               isSameTree(p->left, q->left) &&
               isSameTree(p->right, q->right));
   }
   ```

### Medium:

1. **Binary Tree Level Order Traversal** (LeetCode #102)

   - Return the level order traversal of a binary tree's values as a 2D array.

   ```cpp
   std::vector<std::vector<int>> levelOrder(TreeNode* root) {
       std::vector<std::vector<int>> result;

       if (root == nullptr) {
           return result;
       }

       std::queue<TreeNode*> q;
       q.push(root);

       while (!q.empty()) {
           int levelSize = q.size();
           std::vector<int> currentLevel;

           for (int i = 0; i < levelSize; i++) {
               TreeNode* node = q.front();
               q.pop();

               currentLevel.push_back(node->val);

               if (node->left) {
                   q.push(node->left);
               }
               if (node->right) {
                   q.push(node->right);
               }
           }

           result.push_back(currentLevel);
       }

       return result;
   }
   ```

2. **Construct Binary Tree from Preorder and Inorder Traversal** (LeetCode #105)

   - Build a binary tree from preorder and inorder traversal arrays.

   ```cpp
   TreeNode* buildTree(std::vector<int>& preorder, std::vector<int>& inorder) {
       return buildTreeHelper(preorder, 0, preorder.size() - 1,
                             inorder, 0, inorder.size() - 1);
   }

   TreeNode* buildTreeHelper(std::vector<int>& preorder, int preStart, int preEnd,
                            std::vector<int>& inorder, int inStart, int inEnd) {
       if (preStart > preEnd || inStart > inEnd) {
           return nullptr;
       }

       // The first element in preorder is the root
       int rootValue = preorder[preStart];
       TreeNode* root = new TreeNode(rootValue);

       // Find the position of the root in inorder
       int rootIndex = inStart;
       for (int i = inStart; i <= inEnd; i++) {
           if (inorder[i] == rootValue) {
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
   ```

3. **Path Sum II** (LeetCode #113)

   - Find all root-to-leaf paths where the sum of node values equals a given target.

   ```cpp
   std::vector<std::vector<int>> pathSum(TreeNode* root, int targetSum) {
       std::vector<std::vector<int>> paths;
       std::vector<int> currentPath;

       dfs(root, targetSum, currentPath, paths);

       return paths;
   }

   void dfs(TreeNode* node, int remainingSum, std::vector<int>& currentPath,
            std::vector<std::vector<int>>& paths) {
       if (node == nullptr) {
           return;
       }

       // Add current node to the path
       currentPath.push_back(node->val);

       // Check if it's a leaf node and the path sum equals the target
       if (node->left == nullptr && node->right == nullptr && remainingSum == node->val) {
           paths.push_back(currentPath);
       } else {
           // Continue DFS on both subtrees
           dfs(node->left, remainingSum - node->val, currentPath, paths);
           dfs(node->right, remainingSum - node->val, currentPath, paths);
       }

       // Backtrack by removing the current node from the path
       currentPath.pop_back();
   }
   ```

### Hard:

1. **Binary Tree Maximum Path Sum** (LeetCode #124)

   - Find the maximum path sum in a binary tree (path can start and end at any node).

   ```cpp
   int maxPathSum(TreeNode* root) {
       int maxSum = INT_MIN;
       maxPathSumHelper(root, maxSum);
       return maxSum;
   }

   int maxPathSumHelper(TreeNode* node, int& maxSum) {
       if (node == nullptr) {
           return 0;
       }

       // Calculate the maximum path sum from left and right subtrees
       // If negative, we can choose not to include that path by taking 0
       int leftMax = std::max(0, maxPathSumHelper(node->left, maxSum));
       int rightMax = std::max(0, maxPathSumHelper(node->right, maxSum));

       // The maximum path sum that passes through the current node
       int currentPathSum = node->val + leftMax + rightMax;

       // Update the global maximum
       maxSum = std::max(maxSum, currentPathSum);

       // Return the maximum sum of a path that can be extended to parent
       // (can only include one subtree path)
       return node->val + std::max(leftMax, rightMax);
   }
   ```

2. **Serialize and Deserialize Binary Tree** (LeetCode #297)

   - Design an algorithm to serialize and deserialize a binary tree.

   ```cpp
   // Encodes a tree to a single string.
   std::string serialize(TreeNode* root) {
       if (root == nullptr) {
           return "null,";
       }

       std::string result = std::to_string(root->val) + ",";
       result += serialize(root->left);
       result += serialize(root->right);

       return result;
   }

   // Helper function for deserialization
   TreeNode* deserializeHelper(std::queue<std::string>& nodes) {
       std::string val = nodes.front();
       nodes.pop();

       if (val == "null") {
           return nullptr;
       }

       TreeNode* root = new TreeNode(std::stoi(val));
       root->left = deserializeHelper(nodes);
       root->right = deserializeHelper(nodes);

       return root;
   }

   // Decodes your encoded data to tree.
   TreeNode* deserialize(std::string data) {
       std::queue<std::string> nodes;
       std::string val;

       // Parse the string into a queue of values
       for (char c : data) {
           if (c == ',') {
               nodes.push(val);
               val = "";
           } else {
               val += c;
           }
       }

       return deserializeHelper(nodes);
   }
   ```

3. **Binary Tree Cameras** (LeetCode #968)

   - Place cameras on nodes to monitor all nodes with the minimum number of cameras.

   ```cpp
   enum State { NOT_MONITORED, MONITORED, HAS_CAMERA };

   int minCameraCover(TreeNode* root) {
       int cameras = 0;

       // Handle the edge case where the root is a leaf
       if (dfs(root, cameras) == NOT_MONITORED) {
           cameras++;
       }

       return cameras;
   }

   State dfs(TreeNode* node, int& cameras) {
       if (node == nullptr) {
           return MONITORED; // Null nodes are considered monitored
       }

       State leftState = dfs(node->left, cameras);
       State rightState = dfs(node->right, cameras);

       // If either child is not monitored, we need a camera at this node
       if (leftState == NOT_MONITORED || rightState == NOT_MONITORED) {
           cameras++;
           return HAS_CAMERA;
       }

       // If either child has a camera, this node is already monitored
       if (leftState == HAS_CAMERA || rightState == HAS_CAMERA) {
           return MONITORED;
       }

       // Both children are monitored, but no child has a camera
       return NOT_MONITORED;
   }
   ```

## 🎭 Common Variations and Extensions of Binary Trees

Beyond the basic binary tree, there are several important variations:

### 1. Binary Search Tree (BST)

A binary search tree is a binary tree where:

- All nodes in the left subtree have values less than the node's value
- All nodes in the right subtree have values greater than the node's value
- Both the left and right subtrees are also BSTs

We'll cover BSTs in detail tomorrow.

### 2. AVL Tree

An AVL tree is a self-balancing BST where the height difference between left and right subtrees cannot exceed 1 for any node. We'll cover AVL trees in Day 24.

### 3. Red-Black Tree

A red-black tree is another self-balancing BST with additional properties to ensure balance, using "red" and "black" labels for nodes.

### 4. B-Tree and B+ Tree

These are generalizations of binary search trees that allow nodes to have more than two children, making them particularly suitable for storage systems.

### 5. Segment Tree

A segment tree is a binary tree used for storing intervals or segments, allowing efficient range queries.

### 6. Trie (Prefix Tree)

A trie is a tree-like data structure used to store a dynamic set of strings, where the keys are usually strings. We'll cover tries in Day 27.

### 7. Heap

A heap is a complete binary tree where every node satisfies the heap property (either min-heap or max-heap). We'll cover heaps in Day 25.

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive deeper into Binary Search Trees (BSTs), a special type of binary tree that maintains an ordered structure. We'll learn:

- The BST property and how it enables efficient search, insertion, and deletion
- Implementation of basic BST operations
- Self-balancing techniques to maintain performance guarantees
- Common problems and optimizations for BSTs
- Real-world applications of BSTs in database indexing, priority queues, and more

_If a binary tree is like a family tree, a binary search tree is like a well-organized family where everyone knows their place in the hierarchy—making it incredibly efficient to find any individual family member!_

Stay rooted for tomorrow's branching adventure!

#DSAin45 #Day22 #BinaryTrees #TreeTraversal #DataStructures #CPlusPlus #Algorithms #RecursiveTraversal #CodingInterview #ComputerScience
