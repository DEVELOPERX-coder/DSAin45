# 🔍 DAY 23: Binary Search Trees - Fast, Ordered, and Efficient #DSAin45

Welcome to Day 23 of our 45-day DSA journey! Yesterday, we explored the foundation of hierarchical data structures with basic binary trees. Today, we're taking a step forward to examine the powerful Binary Search Tree (BST), one of the most important and practical data structures in computer science.

_Think of a binary search tree as a well-organized library where finding any book is remarkably efficient. Each shelf (node) holds a single book, with all lighter books to the left and all heavier books to the right. This elegant arrangement means you can quickly navigate to your desired book by making a simple left or right decision at each shelf, rather than examining every book in the library._

## 📌 Understanding Binary Search Trees

A Binary Search Tree (BST) is a special type of binary tree with an additional property:

- For any node N in the tree, all values in the left subtree of N are less than the value of N
- All values in the right subtree of N are greater than the value of N

This simple ordering rule creates a structure with remarkable search efficiency, typically allowing us to find elements in O(log n) time rather than the O(n) time required for linear structures like arrays or linked lists.

### Visual Representation of a BST

```
        8       ← Root
       / \
      3   10    ← Internal Nodes
     / \    \
    1   6    14 ← Leaf Nodes
       / \
      4   7
```

In this BST:

- Every node in the left subtree of 8 (1, 3, 4, 6, 7) has a value less than 8
- Every node in the right subtree of 8 (10, 14) has a value greater than 8
- The same property holds recursively for each subtree (e.g., 4 and 7 are greater than 3 but less than 8)

## 💻 Basic BST Implementation

Let's implement a Binary Search Tree in C++:

```cpp
// Basic definition for a BST node
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    // Constructor
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BinarySearchTree {
private:
    TreeNode* root;

    // Helper methods (to be implemented)
    TreeNode* insertRecursive(TreeNode* root, int val);
    TreeNode* deleteRecursive(TreeNode* root, int val);
    bool searchRecursive(TreeNode* root, int val);
    void inorderTraversalRecursive(TreeNode* root);

public:
    // Constructor
    BinarySearchTree() : root(nullptr) {}

    // Destructor to clean up memory
    ~BinarySearchTree();

    // BST operations
    void insert(int val);
    void remove(int val);
    bool search(int val);
    void inorderTraversal();
};
```

## 🔄 Core BST Operations

The primary operations that make BSTs valuable are:

1. **Insertion**: Adding a new value in the correct position
2. **Deletion**: Removing a value while maintaining the BST property
3. **Search**: Finding a specific value
4. **Traversal**: Exploring all elements in the tree

Let's implement each of these operations:

### 1. Insertion

To insert a value into a BST, we start at the root and compare the new value with each node's value, traversing left or right based on the comparison until we find an empty spot.

```cpp
// Public insert method
void BinarySearchTree::insert(int val) {
    root = insertRecursive(root, val);
}

// Helper function for recursive insertion
TreeNode* BinarySearchTree::insertRecursive(TreeNode* node, int val) {
    // If the tree is empty or we've reached a leaf node, create a new node
    if (node == nullptr) {
        return new TreeNode(val);
    }

    // Otherwise, recur down the tree
    if (val < node->val) {
        node->left = insertRecursive(node->left, val);
    } else if (val > node->val) {
        node->right = insertRecursive(node->right, val);
    }

    // If value is already present, do nothing (no duplicates in this implementation)

    // Return the unchanged node pointer
    return node;
}
```

**Time Complexity**: O(h) where h is the height of the tree (which is O(log n) for a balanced tree, but can be O(n) in the worst case for a skewed tree)
**Space Complexity**: O(h) for the recursive call stack

We can also implement insertion iteratively, which avoids the overhead of recursion:

```cpp
// Iterative insertion
void BinarySearchTree::insertIterative(int val) {
    TreeNode* newNode = new TreeNode(val);

    // If the tree is empty, make the new node the root
    if (root == nullptr) {
        root = newNode;
        return;
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;

    // Find the appropriate position
    while (current != nullptr) {
        parent = current;

        if (val < current->val) {
            current = current->left;
        } else if (val > current->val) {
            current = current->right;
        } else {
            // Value already exists, don't add duplicate
            delete newNode;
            return;
        }
    }

    // Insert the new node
    if (val < parent->val) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}
```

**Time Complexity**: O(h) same as recursive
**Space Complexity**: O(1) constant space, better than recursive

### 2. Search

Searching in a BST is similar to insertion, but simpler since we only need to find the value without modifying the tree:

```cpp
// Public search method
bool BinarySearchTree::search(int val) {
    return searchRecursive(root, val);
}

// Helper function for recursive search
bool BinarySearchTree::searchRecursive(TreeNode* node, int val) {
    // If tree is empty or value is found
    if (node == nullptr) {
        return false;
    }

    if (val == node->val) {
        return true;
    }

    // Otherwise, recur down the appropriate subtree
    if (val < node->val) {
        return searchRecursive(node->left, val);
    } else {
        return searchRecursive(node->right, val);
    }
}
```

**Time Complexity**: O(h) same as insertion
**Space Complexity**: O(h) for the recursive call stack

Iterative search:

```cpp
// Iterative search
bool BinarySearchTree::searchIterative(int val) {
    TreeNode* current = root;

    while (current != nullptr) {
        if (val == current->val) {
            return true;
        } else if (val < current->val) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return false;
}
```

**Time Complexity**: O(h)
**Space Complexity**: O(1)

### 3. Traversal

For a BST, in-order traversal (left-root-right) automatically visits nodes in sorted order:

```cpp
// Public inorder traversal method
void BinarySearchTree::inorderTraversal() {
    std::cout << "In-order traversal: ";
    inorderTraversalRecursive(root);
    std::cout << std::endl;
}

// Helper function for recursive inorder traversal
void BinarySearchTree::inorderTraversalRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    inorderTraversalRecursive(node->left);
    std::cout << node->val << " ";
    inorderTraversalRecursive(node->right);
}
```

**Time Complexity**: O(n) since we visit every node
**Space Complexity**: O(h) for the recursive call stack

### 4. Deletion

Deleting a node from a BST is the most complex operation because we need to ensure the BST property is maintained. There are three cases to consider:

1. **Node with no children (leaf node)**: Simply remove the node.
2. **Node with one child**: Replace the node with its child.
3. **Node with two children**: Replace the node with its in-order successor (smallest node in its right subtree) or predecessor (largest node in its left subtree).

```cpp
// Public delete method
void BinarySearchTree::remove(int val) {
    root = deleteRecursive(root, val);
}

// Helper function for recursive deletion
TreeNode* BinarySearchTree::deleteRecursive(TreeNode* node, int val) {
    // Base case: If the tree is empty
    if (node == nullptr) {
        return nullptr;
    }

    // Find the node to be deleted
    if (val < node->val) {
        node->left = deleteRecursive(node->left, val);
    } else if (val > node->val) {
        node->right = deleteRecursive(node->right, val);
    } else {
        // Node with the value found

        // Case 1: Leaf node (no children)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2: Node with only one child
        else if (node->left == nullptr) {
            TreeNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            TreeNode* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Node with two children
        // Find the inorder successor (smallest node in the right subtree)
        TreeNode* temp = findMin(node->right);

        // Copy the inorder successor's value to this node
        node->val = temp->val;

        // Delete the inorder successor
        node->right = deleteRecursive(node->right, temp->val);
    }

    return node;
}

// Helper function to find the minimum value node in a BST
TreeNode* BinarySearchTree::findMin(TreeNode* node) {
    TreeNode* current = node;

    // The leftmost node contains the minimum value
    while (current && current->left != nullptr) {
        current = current->left;
    }

    return current;
}
```

**Time Complexity**: O(h) same as other operations
**Space Complexity**: O(h) for the recursive call stack

### 5. Additional Useful Operations

There are several other useful operations we can perform on a BST:

#### Finding the Minimum and Maximum Values

```cpp
// Find the minimum value
int BinarySearchTree::findMinimum() {
    if (root == nullptr) {
        throw std::runtime_error("Tree is empty");
    }

    TreeNode* current = root;
    while (current->left != nullptr) {
        current = current->left;
    }

    return current->val;
}

// Find the maximum value
int BinarySearchTree::findMaximum() {
    if (root == nullptr) {
        throw std::runtime_error("Tree is empty");
    }

    TreeNode* current = root;
    while (current->right != nullptr) {
        current = current->right;
    }

    return current->val;
}
```

#### Finding Successor and Predecessor

```cpp
// Find inorder successor of a node
TreeNode* BinarySearchTree::findSuccessor(TreeNode* node) {
    // If the right subtree is not null, the successor is the leftmost node in the right subtree
    if (node->right != nullptr) {
        return findMin(node->right);
    }

    // Otherwise, the successor is the lowest ancestor whose left child is also an ancestor
    TreeNode* successor = nullptr;
    TreeNode* ancestor = root;

    while (ancestor != node) {
        if (node->val < ancestor->val) {
            successor = ancestor;
            ancestor = ancestor->left;
        } else {
            ancestor = ancestor->right;
        }
    }

    return successor;
}

// Find inorder predecessor of a node
TreeNode* BinarySearchTree::findPredecessor(TreeNode* node) {
    // If the left subtree is not null, the predecessor is the rightmost node in the left subtree
    if (node->left != nullptr) {
        TreeNode* temp = node->left;
        while (temp->right != nullptr) {
            temp = temp->right;
        }
        return temp;
    }

    // Otherwise, the predecessor is the lowest ancestor whose right child is also an ancestor
    TreeNode* predecessor = nullptr;
    TreeNode* ancestor = root;

    while (ancestor != node) {
        if (node->val > ancestor->val) {
            predecessor = ancestor;
            ancestor = ancestor->right;
        } else {
            ancestor = ancestor->left;
        }
    }

    return predecessor;
}
```

#### Height and Size of the BST

```cpp
// Get the height of the tree
int BinarySearchTree::height() {
    return getHeight(root);
}

int BinarySearchTree::getHeight(TreeNode* node) {
    if (node == nullptr) {
        return -1;  // Height of empty tree is -1
    }

    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

// Get the number of nodes in the tree
int BinarySearchTree::size() {
    return getSize(root);
}

int BinarySearchTree::getSize(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }

    return 1 + getSize(node->left) + getSize(node->right);
}
```

## 📏 BST Properties and Complexity

### Performance Characteristics

A BST's performance heavily depends on its shape:

1. **Balanced BST**:

   - Height: O(log n)
   - Operations (search, insert, delete): O(log n)

2. **Skewed BST** (degenerates to a linked list):
   - Height: O(n)
   - Operations: O(n)

This variability in performance is a key drawback of basic BSTs. To address this, self-balancing variants like AVL trees (which we'll cover tomorrow) and Red-Black trees were developed.

### Space Complexity

- **Storage**: O(n) where n is the number of nodes
- **Auxiliary Space for Operations**: O(h) due to recursion, where h is the height of the tree

### Advantages of BSTs

1. **Dynamic Size**: Can grow or shrink as needed
2. **Fast Operations**: O(log n) time complexity for search, insert, and delete operations when balanced
3. **Ordered Structure**: In-order traversal gives elements in sorted order
4. **Flexibility**: Can easily implement range queries, finding predecessors and successors

### Disadvantages of BSTs

1. **No Guarantees**: Standard BSTs can degenerate into linked lists, losing their O(log n) advantage
2. **Memory Overhead**: Each node requires extra space for pointers
3. **Implementation Complexity**: More complex than simple data structures like arrays

## 🔍 BST Traversal and Iteration

We've already seen in-order traversal, which is particularly useful for BSTs as it visits nodes in sorted order. Let's also implement pre-order and post-order traversals:

```cpp
// Pre-order traversal (Root-Left-Right)
void BinarySearchTree::preorderTraversal() {
    std::cout << "Pre-order traversal: ";
    preorderTraversalRecursive(root);
    std::cout << std::endl;
}

void BinarySearchTree::preorderTraversalRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    std::cout << node->val << " ";  // Visit root
    preorderTraversalRecursive(node->left);  // Visit left subtree
    preorderTraversalRecursive(node->right);  // Visit right subtree
}

// Post-order traversal (Left-Right-Root)
void BinarySearchTree::postorderTraversal() {
    std::cout << "Post-order traversal: ";
    postorderTraversalRecursive(root);
    std::cout << std::endl;
}

void BinarySearchTree::postorderTraversalRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    postorderTraversalRecursive(node->left);  // Visit left subtree
    postorderTraversalRecursive(node->right);  // Visit right subtree
    std::cout << node->val << " ";  // Visit root
}
```

### Iterative In-order Traversal

The recursive traversals are elegant but can cause stack overflow for very deep trees. Here's an iterative implementation:

```cpp
void BinarySearchTree::inorderTraversalIterative() {
    std::cout << "Iterative in-order traversal: ";

    if (root == nullptr) {
        std::cout << std::endl;
        return;
    }

    std::stack<TreeNode*> stack;
    TreeNode* current = root;

    while (current != nullptr || !stack.empty()) {
        // Reach the leftmost node of the current subtree
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        // Current is now nullptr, pop from stack
        current = stack.top();
        stack.pop();

        // Visit the node
        std::cout << current->val << " ";

        // Move to the right subtree
        current = current->right;
    }

    std::cout << std::endl;
}
```

## 🔄 BST Range Operations

The ordering property of BSTs makes them particularly efficient for range-based operations:

### Range Search (Finding All Elements Within a Range)

```cpp
// Find all elements within a given range [low, high]
std::vector<int> BinarySearchTree::rangeSearch(int low, int high) {
    std::vector<int> result;
    rangeSearchRecursive(root, low, high, result);
    return result;
}

void BinarySearchTree::rangeSearchRecursive(TreeNode* node, int low, int high, std::vector<int>& result) {
    if (node == nullptr) {
        return;
    }

    // If current node's value is greater than the lower bound, search left subtree
    if (low < node->val) {
        rangeSearchRecursive(node->left, low, high, result);
    }

    // If current node's value is within range, add it to result
    if (low <= node->val && node->val <= high) {
        result.push_back(node->val);
    }

    // If current node's value is less than the upper bound, search right subtree
    if (node->val < high) {
        rangeSearchRecursive(node->right, low, high, result);
    }
}
```

### Finding the k-th Smallest Element

```cpp
// Find the k-th smallest element (1-indexed)
int BinarySearchTree::kthSmallest(int k) {
    int count = 0;
    int result = -1;
    kthSmallestHelper(root, k, count, result);

    if (result == -1) {
        throw std::out_of_range("k is out of range");
    }

    return result;
}

void BinarySearchTree::kthSmallestHelper(TreeNode* node, int k, int& count, int& result) {
    if (node == nullptr || result != -1) {
        return;
    }

    // In-order traversal
    kthSmallestHelper(node->left, k, count, result);

    count++;
    if (count == k) {
        result = node->val;
        return;
    }

    kthSmallestHelper(node->right, k, count, result);
}
```

## 🌎 Real-World Applications of BSTs

### 1. Database Indexing

B-Trees and B+ Trees, which are multi-way search trees, are derived from the BST concept and are widely used for database indexing. They allow for efficient retrieval, insertion, and deletion of records based on key values.

### 2. Symbol Tables in Compilers

Compilers use BSTs to implement symbol tables that keep track of identifiers (variables, functions, etc.) along with their attributes.

### 3. Expression Parsers

BST-based expression trees are used to represent and evaluate arithmetic expressions. Operators are stored at internal nodes while operands are at leaf nodes.

### 4. File System Organization

Some file systems use tree structures similar to BSTs to organize directories and files, enabling efficient lookup and traversal.

### 5. Priority Queues

While heaps are more commonly used, BSTs can also implement priority queues where elements are dequeued based on their priority.

### 6. Routing Tables in Networking

Routers use specialized trees like PATRICIA trees (a variant of tries, which are related to BSTs) to store and look up IP addresses efficiently.

## 🔄 BST Variations and Extensions

### 1. Self-Balancing BSTs

Basic BSTs can become unbalanced, leading to worst-case O(n) performance. Self-balancing variants address this:

- **AVL Trees**: Maintain strict balance criteria using rotations (which we'll cover tomorrow)
- **Red-Black Trees**: Use a relaxed balance condition and node coloring
- **Splay Trees**: Automatically move frequently accessed elements closer to the root

### 2. Augmented BSTs

BSTs can be augmented with additional information to support more operations:

- **Order Statistics Tree**: Keeps track of size of subtrees to quickly find the k-th smallest element
- **Interval Tree**: Stores intervals instead of points, optimized for range queries

### 3. BST with Duplicates

Various strategies exist for handling duplicate values:

1. **Count Field**: Each node has a count field to track duplicates
2. **Equal Value Convention**: Place duplicates always to the left (or always to the right)
3. **Linked List**: Store duplicates in a linked list at each node

```cpp
// BST node with count for duplicates
struct CountTreeNode {
    int val;
    int count;  // Number of occurrences
    CountTreeNode* left;
    CountTreeNode* right;

    CountTreeNode(int x) : val(x), count(1), left(nullptr), right(nullptr) {}
};

// Insertion with duplicate counting
void insertWithCount(CountTreeNode*& root, int val) {
    if (root == nullptr) {
        root = new CountTreeNode(val);
        return;
    }

    if (val == root->val) {
        // Increment count for duplicates
        root->count++;
    } else if (val < root->val) {
        insertWithCount(root->left, val);
    } else {
        insertWithCount(root->right, val);
    }
}
```

## 🤔 Common BST Problems and Solutions

### Problem 1: Validating a BST

A common interview problem is to check if a binary tree is a valid BST:

```cpp
bool isValidBST(TreeNode* root) {
    return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
}

bool isValidBSTHelper(TreeNode* node, long minVal, long maxVal) {
    if (node == nullptr) {
        return true;
    }

    // Check if current node's value is within valid range
    if (node->val <= minVal || node->val >= maxVal) {
        return false;
    }

    // Recursively check left and right subtrees with updated ranges
    return isValidBSTHelper(node->left, minVal, node->val) &&
           isValidBSTHelper(node->right, node->val, maxVal);
}
```

### Problem 2: Converting a Sorted Array to BST

Creating a balanced BST from a sorted array:

```cpp
TreeNode* sortedArrayToBST(std::vector<int>& nums) {
    return sortedArrayToBSTHelper(nums, 0, nums.size() - 1);
}

TreeNode* sortedArrayToBSTHelper(std::vector<int>& nums, int left, int right) {
    if (left > right) {
        return nullptr;
    }

    // Use the middle element as the root
    int mid = left + (right - left) / 2;
    TreeNode* root = new TreeNode(nums[mid]);

    // Recursively build left and right subtrees
    root->left = sortedArrayToBSTHelper(nums, left, mid - 1);
    root->right = sortedArrayToBSTHelper(nums, mid + 1, right);

    return root;
}
```

### Problem 3: Lowest Common Ancestor in a BST

Finding the lowest common ancestor (LCA) of two nodes:

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // Base case
    if (root == nullptr) {
        return nullptr;
    }

    // If both p and q are greater than root, LCA is in the right subtree
    if (p->val > root->val && q->val > root->val) {
        return lowestCommonAncestor(root->right, p, q);
    }

    // If both p and q are less than root, LCA is in the left subtree
    if (p->val < root->val && q->val < root->val) {
        return lowestCommonAncestor(root->left, p, q);
    }

    // If one is less and one is greater, or one equals the root, then root is the LCA
    return root;
}
```

## 🤖 Balancing a BST

While we'll dive deeper into self-balancing trees tomorrow, here's a general approach to balance an unbalanced BST:

1. Collect all elements using in-order traversal (which gives them in sorted order)
2. Reconstruct a balanced BST from the sorted array

```cpp
// Balance an unbalanced BST
TreeNode* BinarySearchTree::balance() {
    // Step 1: Get sorted elements using in-order traversal
    std::vector<int> sortedElements;
    collectInorder(root, sortedElements);

    // Step 2: Rebuild a balanced tree
    deleteTree(root); // Clean up the old tree
    root = buildBalancedBST(sortedElements, 0, sortedElements.size() - 1);

    return root;
}

// Helper to collect nodes in sorted order
void BinarySearchTree::collectInorder(TreeNode* node, std::vector<int>& elements) {
    if (node == nullptr) {
        return;
    }

    collectInorder(node->left, elements);
    elements.push_back(node->val);
    collectInorder(node->right, elements);
}

// Helper to build a balanced BST from sorted array
TreeNode* BinarySearchTree::buildBalancedBST(const std::vector<int>& elements, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    // Use the middle element as the root
    int mid = start + (end - start) / 2;
    TreeNode* node = new TreeNode(elements[mid]);

    // Recursively build left and right subtrees
    node->left = buildBalancedBST(elements, start, mid - 1);
    node->right = buildBalancedBST(elements, mid + 1, end);

    return node;
}

// Helper to delete the entire tree
void BinarySearchTree::deleteTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}
```

## 😄 BST Jokes and Analogies

1. **The Database Query Analogy**:
   "A BST is like a highly efficient customer service system. Instead of asking every representative (linear search), you're guided through a series of directed questions that eliminate half the possibilities each time: 'Is your issue related to billing?' → 'Is your bill more than $100?' And just like that, you've narrowed down your problem logarithmically!"

2. **The Library Joke**:
   "I implemented a BST to organize my library, but I made a mistake in my comparison function. Now all my mystery novels are filed under 'cookbooks' because they both contain 'red herrings'."

3. **The Balanced Diet Analogy**:
   "BSTs are like diets—they perform best when they're balanced. An unbalanced BST is like eating nothing but carbs; technically it works, but you're not getting the performance benefits you were promised."

4. **The Family Tree Joke**:
   "My family tree is actually a binary search tree. Everyone on the left side of the family is less argumentative than the patriarch, and everyone on the right side is more so. Finding the most peaceful relative takes O(log n) time—unless there's a family feud, then it degenerates to a linked list."

5. **The Sorting Hat Analogy**:
   "A BST is like the Sorting Hat from Harry Potter—it makes quick decisions about where elements belong based on their values, placing smaller ones to the left and greater ones to the right. But unlike the Sorting Hat, a BST doesn't have to make all its decisions at once; it can continue sorting as new students... I mean, elements... arrive."

6. **The Car Key Joke**:
   "I organized my key ring using a binary search tree, but I keep losing my car key. I think it's because car theft is an O(1) operation, but finding a specific key in my BST takes O(log n)."

7. **The Dictionary Analogy**:
   "Using a BST is like looking up a word in a dictionary: you open to the middle, check if your word comes before or after, then repeat the process in the appropriate half. You never need to check every page—just like a BST doesn't need to check every node."

8. **The Programmer's Social Life Joke**:
   "My social life is like a poorly balanced BST—it started with a few connections, then I only added friends-of-friends in one direction, and now finding anyone requires a linear search through my entire social network."

## 🚀 LeetCode Practice Problems

### Easy:

1. **Lowest Common Ancestor of a Binary Search Tree** (LeetCode #235)

   - Find the lowest common ancestor of two nodes in a BST.

   ```cpp
   TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
       // BST property allows us to navigate without exploring both subtrees

       // If both values are less than root, search left subtree
       if (p->val < root->val && q->val < root->val) {
           return lowestCommonAncestor(root->left, p, q);
       }

       // If both values are greater than root, search right subtree
       if (p->val > root->val && q->val > root->val) {
           return lowestCommonAncestor(root->right, p, q);
       }

       // If one value is less and one is greater (or one equals the root),
       // then root is the LCA
       return root;
   }
   ```

2. **Search in a Binary Search Tree** (LeetCode #700)

   - Find a node with a specific value in a BST.

   ```cpp
   TreeNode* searchBST(TreeNode* root, int val) {
       // Base case: tree is empty or value is found
       if (root == nullptr || root->val == val) {
           return root;
       }

       // Search in the appropriate subtree
       if (val < root->val) {
           return searchBST(root->left, val);
       } else {
           return searchBST(root->right, val);
       }
   }
   ```

3. **Range Sum of BST** (LeetCode #938)

   - Find the sum of all nodes with values between low and high (inclusive).

   ```cpp
   int rangeSumBST(TreeNode* root, int low, int high) {
       if (root == nullptr) {
           return 0;
       }

       // If current node is within range, include it in sum
       int sum = 0;
       if (low <= root->val && root->val <= high) {
           sum += root->val;
       }

       // If current value is greater than low, search left subtree
       if (low < root->val) {
           sum += rangeSumBST(root->left, low, high);
       }

       // If current value is less than high, search right subtree
       if (root->val < high) {
           sum += rangeSumBST(root->right, low, high);
       }

       return sum;
   }
   ```

### Medium:

1. **Validate Binary Search Tree** (LeetCode #98)

   - Determine if a binary tree is a valid BST.

   ```cpp
   bool isValidBST(TreeNode* root) {
       return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
   }

   bool isValidBSTHelper(TreeNode* node, long minVal, long maxVal) {
       if (node == nullptr) {
           return true;
       }

       // Check if current node's value is within valid range
       if (node->val <= minVal || node->val >= maxVal) {
           return false;
       }

       // Recursively check left and right subtrees with updated ranges
       return isValidBSTHelper(node->left, minVal, node->val) &&
              isValidBSTHelper(node->right, node->val, maxVal);
   }
   ```

2. **Kth Smallest Element in a BST** (LeetCode #230)

   - Find the kth smallest element in a BST.

   ```cpp
   int kthSmallest(TreeNode* root, int k) {
       int count = 0;
       int result = -1;

       inorderTraversal(root, k, count, result);
       return result;
   }

   void inorderTraversal(TreeNode* node, int k, int& count, int& result) {
       if (node == nullptr || count >= k) {
           return;
       }

       // Process left subtree
       inorderTraversal(node->left, k, count, result);

       // Process current node
       count++;
       if (count == k) {
           result = node->val;
           return;
       }

       // Process right subtree
       inorderTraversal(node->right, k, count, result);
   }
   ```

3. **Convert Sorted Array to Binary Search Tree** (LeetCode #108)

   - Convert a sorted array to a height-balanced BST.

   ```cpp
   TreeNode* sortedArrayToBST(std::vector<int>& nums) {
       return sortedArrayToBSTHelper(nums, 0, nums.size() - 1);
   }

   TreeNode* sortedArrayToBSTHelper(std::vector<int>& nums, int left, int right) {
       if (left > right) {
           return nullptr;
       }

       // Use the middle element as the root
       int mid = left + (right - left) / 2;
       TreeNode* root = new TreeNode(nums[mid]);

       // Recursively build left and right subtrees
       root->left = sortedArrayToBSTHelper(nums, left, mid - 1);
       root->right = sortedArrayToBSTHelper(nums, mid + 1, right);

       return root;
   }
   ```

### Hard:

1. **Binary Search Tree Iterator** (LeetCode #173)

   - Implement an iterator over a BST with O(h) memory and amortized O(1) time per operation.

   ```cpp
   class BSTIterator {
   private:
       std::stack<TreeNode*> stack;

       // Helper to push all leftmost nodes to the stack
       void pushLeftmostNodes(TreeNode* node) {
           while (node != nullptr) {
               stack.push(node);
               node = node->left;
           }
       }

   public:
       BSTIterator(TreeNode* root) {
           pushLeftmostNodes(root);
       }

       // Return the next smallest number
       int next() {
           TreeNode* node = stack.top();
           stack.pop();

           // If node has a right child, push its leftmost branch
           pushLeftmostNodes(node->right);

           return node->val;
       }

       // Return whether we have a next smallest number
       bool hasNext() {
           return !stack.empty();
       }
   };
   ```

2. **Recover Binary Search Tree** (LeetCode #99)

   - Restore a BST where two nodes are swapped.

   ```cpp
   void recoverTree(TreeNode* root) {
       TreeNode* first = nullptr;
       TreeNode* second = nullptr;
       TreeNode* prev = nullptr;

       // In-order traversal to find swapped nodes
       std::function<void(TreeNode*)> inorder = [&](TreeNode* node) {
           if (node == nullptr) {
               return;
           }

           inorder(node->left);

           // Check if current node is out of order
           if (prev != nullptr && prev->val > node->val) {
               // First violation
               if (first == nullptr) {
                   first = prev;
               }
               // Second violation or adjacently swapped nodes
               second = node;
           }

           prev = node;
           inorder(node->right);
       };

       inorder(root);

       // Swap the values of the two misplaced nodes
       std::swap(first->val, second->val);
   }
   ```

3. **Serialize and Deserialize BST** (LeetCode #449)

   - Design an algorithm to serialize and deserialize a BST.

   ```cpp
   class Codec {
   public:
       // Encodes a tree to a single string
       string serialize(TreeNode* root) {
           if (root == nullptr) {
               return "";
           }

           std::string result;
           std::queue<TreeNode*> q;
           q.push(root);

           while (!q.empty()) {
               TreeNode* node = q.front();
               q.pop();

               if (node == nullptr) {
                   result += "null,";
               } else {
                   result += std::to_string(node->val) + ",";
                   q.push(node->left);
                   q.push(node->right);
               }
           }

           return result;
       }

       // Decodes your encoded data to tree
       TreeNode* deserialize(string data) {
           if (data.empty()) {
               return nullptr;
           }

           std::vector<std::string> values;
           std::string val;
           for (char c : data) {
               if (c == ',') {
                   values.push_back(val);
                   val.clear();
               } else {
                   val += c;
               }
           }
           if (!val.empty()) {
               values.push_back(val);
           }

           if (values.empty() || values[0] == "null") {
               return nullptr;
           }

           TreeNode* root = new TreeNode(std::stoi(values[0]));
           std::queue<TreeNode*> q;
           q.push(root);

           int i = 1;
           while (!q.empty() && i < values.size()) {
               TreeNode* node = q.front();
               q.pop();

               // Left child
               if (values[i] != "null") {
                   node->left = new TreeNode(std::stoi(values[i]));
                   q.push(node->left);
               }
               i++;

               // Right child
               if (i < values.size() && values[i] != "null") {
                   node->right = new TreeNode(std::stoi(values[i]));
                   q.push(node->right);
               }
               i++;
           }

           return root;
       }
   };
   ```

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive into AVL Trees, which are self-balancing binary search trees. We'll learn:

- Why tree balancing is crucial for maintaining O(log n) performance
- The AVL tree balance criteria and rotation operations
- How to implement insertion and deletion in AVL trees
- The trade-offs between AVL trees and other self-balancing trees
- Real-world applications where tree balancing is essential

_If a BST is like a well-organized library where finding books is efficient, an AVL tree is like a library with a vigilant librarian who constantly reorganizes the shelves to ensure that no section becomes too deep or sparse—guaranteeing that anyone can find any book in logarithmic time, regardless of the pattern of additions or removals._

Stay tuned for our journey into the world of balanced trees!

#DSAin45 #Day23 #BinarySearchTrees #BST #DataStructures #CPlusPlus #TreeTraversal #Algorithms #CodingInterview #SortedData
