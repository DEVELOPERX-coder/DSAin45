# 🔍 DAY 24: AVL Trees - Self-Balancing Guardians of Efficiency #DSAin45

Welcome to Day 24 of our 45-day DSA journey! Yesterday, we explored Binary Search Trees and their efficient operations. Today, we'll tackle one of their most important evolutions: AVL Trees, the first self-balancing binary search tree ever invented.

_Imagine if BSTs were like bookshelves that could detect when they're becoming lopsided and automatically reorganize themselves to maintain optimal access times. That's precisely what AVL trees do—they're the self-correcting, efficiency-preserving guardians of the tree world, ensuring that no matter what sequence of operations you perform, the tree remains balanced and operations stay logarithmic._

## 📌 The Problem with Regular BSTs

As we learned yesterday, a standard Binary Search Tree offers O(log n) time complexity for search, insert, and delete operations—but only when the tree is balanced. When a BST becomes unbalanced or skewed (like when inserting already-sorted data), its performance degrades to O(n), effectively becoming a linked list.

This visualization illustrates the problem:

```
Balanced BST (Height: 3)     Skewed BST (Height: 5)
        4                            1
       / \                            \
      2   6                            2
     / \ / \                            \
    1  3 5  7                            3
                                          \
                                           4
                                            \
                                             5
```

The balanced tree requires at most 3 comparisons to find any element, while the skewed tree might need up to 5 comparisons.

## 📏 AVL Trees: The Self-Balancing Solution

AVL trees (named after inventors Adelson-Velsky and Landis, 1962) solve this problem by maintaining balance through automatic rotations whenever an imbalance is detected after insertion or deletion.

### Key Properties of AVL Trees

1. **Balance Factor**: For every node, the heights of its left and right subtrees differ by at most 1.
2. **Height Information**: Each node stores its height or balance factor.
3. **Self-Balancing**: After any modification, the tree automatically rebalances itself.
4. **Guaranteed Efficiency**: All operations maintain O(log n) time complexity even in worst-case scenarios.

### Balance Factor Calculation

The balance factor of a node is defined as:

```
BalanceFactor = Height(LeftSubtree) - Height(RightSubtree)
```

In an AVL tree, this value must always be -1, 0, or 1 for every node.

## 💻 AVL Tree Node Implementation

Let's define our AVL Tree node structure:

```cpp
template <typename T>
class AVLNode {
public:
    T value;
    AVLNode* left;
    AVLNode* right;
    int height; // Height of the subtree rooted at this node

    AVLNode(T val) : value(val), left(nullptr), right(nullptr), height(1) {}
};
```

Notice that each node stores its height, which is essential for balancing operations.

## 🌳 AVL Tree Class Implementation

Now, let's build the AVL Tree class with core functionality:

```cpp
template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    // Helper functions
    int height(AVLNode<T>* node);
    int getBalanceFactor(AVLNode<T>* node);
    AVLNode<T>* rightRotate(AVLNode<T>* y);
    AVLNode<T>* leftRotate(AVLNode<T>* x);
    AVLNode<T>* insertNode(AVLNode<T>* node, T value);
    AVLNode<T>* deleteNode(AVLNode<T>* node, T value);
    AVLNode<T>* minValueNode(AVLNode<T>* node);
    void inorderTraversal(AVLNode<T>* node);
    void destroyTree(AVLNode<T>* node);

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree();

    void insert(T value);
    void remove(T value);
    bool search(T value);
    void printInorder();
    int getHeight();
};
```

## 🔄 Core Balancing Operations: Rotations

Rotations are the key mechanism that AVL trees use to maintain balance. There are four types of rotations:

1. **Left Rotation** - Used when a right subtree becomes too heavy
2. **Right Rotation** - Used when a left subtree becomes too heavy
3. **Left-Right Rotation** (LR) - A double rotation for left-right imbalance
4. **Right-Left Rotation** (RL) - A double rotation for right-left imbalance

### Height and Balance Factor Calculation

```cpp
template <typename T>
int AVLTree<T>::height(AVLNode<T>* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

template <typename T>
int AVLTree<T>::getBalanceFactor(AVLNode<T>* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}
```

### Right Rotation

```cpp
template <typename T>
AVLNode<T>* AVLTree<T>::rightRotate(AVLNode<T>* y) {
    AVLNode<T>* x = y->left;
    AVLNode<T>* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}
```

Visualization of a right rotation:

```
    y                               x
   / \                            /   \
  x   T3      Right Rotation     T1    y
 / \          ------------->          / \
T1  T2                              T2   T3
```

### Left Rotation

```cpp
template <typename T>
AVLNode<T>* AVLTree<T>::leftRotate(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    AVLNode<T>* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}
```

Visualization of a left rotation:

```
  x                               y
 / \                            /   \
T1  y      Left Rotation       x     T3
   / \     ------------->     / \
  T2 T3                      T1  T2
```

### Left-Right (LR) and Right-Left (RL) Rotations

These are combinations of the above rotations. We'll implement them within our insert and delete methods.

## 🔍 Insertion with Balancing

Insertion in an AVL tree follows the same process as a regular BST, with an additional step to maintain balance:

```cpp
template <typename T>
void AVLTree<T>::insert(T value) {
    root = insertNode(root, value);
}

template <typename T>
AVLNode<T>* AVLTree<T>::insertNode(AVLNode<T>* node, T value) {
    // 1. Perform standard BST insertion
    if (node == nullptr) {
        return new AVLNode<T>(value);
    }

    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    } else {
        // Duplicate values not allowed
        return node;
    }

    // 2. Update height of this ancestor node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // 3. Get the balance factor to check if node became unbalanced
    int balance = getBalanceFactor(node);

    // 4. If unbalanced, there are four cases to consider:

    // Left Left Case
    if (balance > 1 && value < node->left->value) {
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && value > node->right->value) {
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && value > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && value < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the unchanged node pointer
    return node;
}
```

### Understanding the Four Imbalance Cases

1. **Left-Left Case (LL)**: An imbalance occurs at a node, and the left subtree of the left child is heavier.

   - Solution: Right rotation at the imbalanced node.

2. **Right-Right Case (RR)**: An imbalance occurs at a node, and the right subtree of the right child is heavier.

   - Solution: Left rotation at the imbalanced node.

3. **Left-Right Case (LR)**: An imbalance occurs at a node, and the right subtree of the left child is heavier.

   - Solution: Left rotation at the left child, followed by right rotation at the imbalanced node.

4. **Right-Left Case (RL)**: An imbalance occurs at a node, and the left subtree of the right child is heavier.
   - Solution: Right rotation at the right child, followed by left rotation at the imbalanced node.

Let's visualize each case:

#### Left-Left Case (LL)

```
     z                                      y
    / \                                   /   \
   y   T4      Right Rotate (z)          x     z
  / \          - - - - - - - - ->      /  \   /  \
 x   T3                               T1  T2 T3  T4
/ \
T1 T2
```

#### Right-Right Case (RR)

```
  z                                y
 /  \                            /   \
T1   y     Left Rotate(z)       z      x
    / \   - - - - - - - ->     / \    / \
   T2  x                      T1  T2 T3  T4
      / \
     T3 T4
```

#### Left-Right Case (LR)

```
     z                               z                           x
    / \                            /   \                        /  \
   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2  T3  T4
    / \                        / \
  T2   T3                    T1   T2
```

#### Right-Left Case (RL)

```
   z                            z                            x
  / \                          / \                          /  \
T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
    / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
   x   T4                      T2   y                  T1  T2  T3  T4
  / \                              /  \
T2   T3                           T3   T4
```

## 🗑️ Deletion with Balancing

Deletion in an AVL tree is similar to a BST but requires additional balancing:

```cpp
template <typename T>
void AVLTree<T>::remove(T value) {
    root = deleteNode(root, value);
}

template <typename T>
AVLNode<T>* AVLTree<T>::deleteNode(AVLNode<T>* node, T value) {
    // 1. Perform standard BST delete
    if (node == nullptr) {
        return nullptr;
    }

    // Search for the node to be deleted
    if (value < node->value) {
        node->left = deleteNode(node->left, value);
    } else if (value > node->value) {
        node->right = deleteNode(node->right, value);
    } else {
        // Node found - implement deletion based on node's children

        // Case 1: Node with no child or only one child
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode<T>* temp = node->left ? node->left : node->right;

            // No child case
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                // One child case: copy the contents of the non-empty child
                *node = *temp; // Copy content
            }
            delete temp;
        } else {
            // Case 2: Node with two children
            // Get the inorder successor (smallest in the right subtree)
            AVLNode<T>* temp = minValueNode(node->right);

            // Copy the inorder successor's data to this node
            node->value = temp->value;

            // Delete the inorder successor
            node->right = deleteNode(node->right, temp->value);
        }
    }

    // If the tree had only one node then return
    if (node == nullptr) {
        return nullptr;
    }

    // 2. Update height of the current node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // 3. Get the balance factor of this node
    int balance = getBalanceFactor(node);

    // 4. If unbalanced, there are four cases (same as insertion)

    // Left Left Case
    if (balance > 1 && getBalanceFactor(node->left) >= 0) {
        return rightRotate(node);
    }

    // Left Right Case
    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(node->right) <= 0) {
        return leftRotate(node);
    }

    // Right Left Case
    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

template <typename T>
AVLNode<T>* AVLTree<T>::minValueNode(AVLNode<T>* node) {
    AVLNode<T>* current = node;

    // Loop down to find the leftmost leaf
    while (current->left != nullptr) {
        current = current->left;
    }

    return current;
}
```

## 🔍 Searching in AVL Trees

Searching in an AVL tree is identical to searching in a BST:

```cpp
template <typename T>
bool AVLTree<T>::search(T value) {
    AVLNode<T>* current = root;

    while (current != nullptr) {
        if (value == current->value) {
            return true;
        } else if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return false;
}
```

## 🚶 Tree Traversal and Cleanup

```cpp
template <typename T>
void AVLTree<T>::printInorder() {
    std::cout << "Inorder Traversal: ";
    inorderTraversal(root);
    std::cout << std::endl;
}

template <typename T>
void AVLTree<T>::inorderTraversal(AVLNode<T>* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        std::cout << node->value << " ";
        inorderTraversal(node->right);
    }
}

template <typename T>
AVLTree<T>::~AVLTree() {
    destroyTree(root);
}

template <typename T>
void AVLTree<T>::destroyTree(AVLNode<T>* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

template <typename T>
int AVLTree<T>::getHeight() {
    return height(root);
}
```

## 🧮 Time and Space Complexity

AVL trees maintain their balance factor, ensuring operations remain efficient regardless of insertion order.

### Time Complexity

| Operation | Average Case | Worst Case |
| --------- | ------------ | ---------- |
| Search    | O(log n)     | O(log n)   |
| Insert    | O(log n)     | O(log n)   |
| Delete    | O(log n)     | O(log n)   |
| Traversal | O(n)         | O(n)       |

### Space Complexity

- **Storage**: O(n) for n elements
- **Additional per node**: O(1) for the height information
- **Call Stack**: O(log n) for recursive operations

### How AVL Trees Compare to Regular BSTs

1. **Guaranteed Efficiency**: AVL trees guarantee O(log n) operations even in worst-case scenarios, while regular BSTs can degrade to O(n).
2. **Height Bound**: The height of an AVL tree with n nodes is always bounded by 1.44 × log₂(n+2) - 0.328, which is tighter than a potentially skewed BST.
3. **Memory Overhead**: AVL trees require slightly more memory to store height/balance information.
4. **Insertion/Deletion**: These operations are more complex in AVL trees due to the balancing requirement.

## 🏭 Real-World Applications of AVL Trees

1. **Database Indexing**: When frequent lookups are required with less frequent insertions or deletions.
2. **Memory Management Systems**: Efficient storage and retrieval of memory blocks.
3. **File Systems**: For organizing directory structures that require quick lookups.
4. **Network Routing Tables**: For storing IP addresses and routing information.
5. **Auto-completion and Spell Checking**: For quick prefix-based searches.
6. **Game Development**: For storing game state and spatial partitioning.

## 🌵 AVL Trees vs. Other Self-Balancing Trees

| Tree Type       | Balancing Strategy           | Performance Profile                             | When to Use                           |
| --------------- | ---------------------------- | ----------------------------------------------- | ------------------------------------- |
| AVL Trees       | Strictly balanced (BF ≤ 1)   | Faster lookups, more rotations on insert/delete | Read-heavy workloads                  |
| Red-Black Trees | Less strictly balanced       | Fewer rotations, slightly slower lookups        | Write-heavy workloads                 |
| B-Trees         | Multi-way tree nodes         | Better for disk-based storage                   | Databases, file systems               |
| Splay Trees     | Moves accessed nodes to root | Adaptive to access patterns                     | Caching, frequently accessed elements |

## 😄 AVL Tree Analogies and Jokes

1. **The Balancing Act Analogy**:
   "An AVL tree is like a skilled tightrope walker carrying different-sized packages. With every new package, they immediately adjust their position to maintain perfect balance, never letting either side get too heavy. Regular BSTs, on the other hand, are more like someone walking on a beam who doesn't adjust until they're about to fall off!"

2. **The Library Joke**:
   "A Binary Search Tree walks into a library and starts placing books on a single shelf, left to right in alphabetical order. Soon, the shelf is sagging dangerously on the right side. An AVL Tree walks in and says, 'You're doing it all wrong! You need to rotate your collection periodically!' The BST responds, 'But that would take log(n) time!' The AVL Tree smirks, 'Exactly.'"

3. **The Yoga Instructor Analogy**:
   "If data structures were yoga instructors, AVL trees would be the strictest ones—constantly checking your balance and making you rotate left and right until your posture is perfect. Some students might prefer the more relaxed Red-Black Tree instructor who lets you be 'mostly balanced.'"

4. **The Political Balance Joke**:
   "An AVL tree and a Binary Search Tree are discussing politics. The BST says, 'I lean heavily to the right.' The AVL tree responds, 'That's your problem—you should rotate left more often.'"

5. **The Restaurant Seating Analogy**:
   "AVL trees are like meticulous restaurant hosts who constantly rearrange the seating to ensure no section becomes too crowded or empty. They'll move entire parties from one side to another if needed, all so that when you ask 'Do you have a table for Adelson?', they can find it in O(log n) time!"

6. **The Perfectionist Joke**:
   "How many AVL trees does it take to change a lightbulb? Just one, but it will rotate the entire house if that's what it takes to maintain balance."

7. **The Parenting Analogy**:
   "BSTs are like permissive parents who let their child nodes grow however they want, sometimes ending up with a completely unbalanced family tree. AVL trees are the parents who make sure every child gets equal attention, even if it means some major family restructuring!"

## 🚀 LeetCode Practice Problems

### Easy:

1. **Convert Sorted Array to AVL Tree** (adaptation of LeetCode #108)

   - Create a height-balanced BST from a sorted array.

   ```cpp
   AVLNode<int>* sortedArrayToAVL(std::vector<int>& nums, int start, int end) {
       if (start > end) {
           return nullptr;
       }

       // Get the middle element and make it root
       int mid = start + (end - start) / 2;
       AVLNode<int>* node = new AVLNode<int>(nums[mid]);

       // Recursively construct left and right subtrees
       node->left = sortedArrayToAVL(nums, start, mid - 1);
       node->right = sortedArrayToAVL(nums, mid + 1, end);

       // Update height
       node->height = 1 + std::max(height(node->left), height(node->right));

       return node;
   }
   ```

2. **Check if a Binary Tree is Balanced** (LeetCode #110)

   - Determine if a binary tree is height-balanced (an AVL tree without the ordering property).

   ```cpp
   bool isBalanced(TreeNode* root) {
       return checkHeight(root) != -1;
   }

   int checkHeight(TreeNode* node) {
       if (node == nullptr) {
           return 0;
       }

       int leftHeight = checkHeight(node->left);
       if (leftHeight == -1) {
           return -1; // Left subtree is unbalanced
       }

       int rightHeight = checkHeight(node->right);
       if (rightHeight == -1) {
           return -1; // Right subtree is unbalanced
       }

       // Check if current node is balanced
       if (std::abs(leftHeight - rightHeight) > 1) {
           return -1; // Current node is unbalanced
       }

       return 1 + std::max(leftHeight, rightHeight);
   }
   ```

### Medium:

1. **Convert BST to AVL Tree**

   - Convert an existing BST to a balanced AVL tree.

   ```cpp
   AVLNode<int>* convertBSTtoAVL(TreeNode* root) {
       // First collect all elements in-order
       std::vector<int> elements;
       inorderCollect(root, elements);

       // Then build a balanced AVL tree
       return sortedArrayToAVL(elements, 0, elements.size() - 1);
   }

   void inorderCollect(TreeNode* node, std::vector<int>& elements) {
       if (node == nullptr) {
           return;
       }
       inorderCollect(node->left, elements);
       elements.push_back(node->val);
       inorderCollect(node->right, elements);
   }
   ```

2. **Range Sum of AVL** (adaptation of LeetCode #938)

   - Find the sum of values in a given range in an AVL tree.

   ```cpp
   int rangeSumAVL(AVLNode<int>* root, int low, int high) {
       if (root == nullptr) {
           return 0;
       }

       // If current node is in range, include it
       int sum = 0;
       if (root->value >= low && root->value <= high) {
           sum += root->value;
       }

       // If current value is greater than low, search left subtree
       if (root->value > low) {
           sum += rangeSumAVL(root->left, low, high);
       }

       // If current value is less than high, search right subtree
       if (root->value < high) {
           sum += rangeSumAVL(root->right, low, high);
       }

       return sum;
   }
   ```

### Hard:

1. **AVL Tree Iterator** (adaptation of LeetCode #173)

   - Implement an iterator over an AVL tree with O(h) memory and amortized O(1) time per operation.

   ```cpp
   class AVLIterator {
   private:
       std::stack<AVLNode<int>*> stack;

       // Helper to push all leftmost nodes to the stack
       void pushLeftmostNodes(AVLNode<int>* node) {
           while (node != nullptr) {
               stack.push(node);
               node = node->left;
           }
       }

   public:
       AVLIterator(AVLNode<int>* root) {
           pushLeftmostNodes(root);
       }

       // Return the next smallest number
       int next() {
           AVLNode<int>* node = stack.top();
           stack.pop();

           // If node has a right child, push its leftmost branch
           pushLeftmostNodes(node->right);

           return node->value;
       }

       // Return whether we have a next smallest number
       bool hasNext() {
           return !stack.empty();
       }
   };
   ```

2. **Implement AVL Set and Map**

   - Create a complete AVL-based implementation of set and map data structures.

   ```cpp
   template<typename K, typename V>
   class AVLMap {
   private:
       // Node structure with key-value pair
       struct MapNode {
           K key;
           V value;
           MapNode* left;
           MapNode* right;
           int height;

           MapNode(K k, V v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
       };

       MapNode* root;

       // AVL tree helper functions (similar to our AVL implementation)
       // ...

   public:
       AVLMap() : root(nullptr) {}
       ~AVLMap() { /* cleanup */ }

       // Map operations
       void insert(K key, V value);
       bool remove(K key);
       bool contains(K key);
       V& operator[](K key);
       // Additional operations...
   };
   ```

## 🌱 Implementing Self-Balancing Features in Your BST

If you have an existing BST implementation from yesterday's lesson, you can enhance it with AVL balancing capabilities:

1. Add height information to each node
2. Implement balance factor calculation
3. Add rotation functions
4. Modify insert and delete to perform balancing

Here's a simplified transformation checklist:

```cpp
// 1. Modify your Node class:
struct Node {
    int value;
    Node* left;
    Node* right;
    int height;  // <-- Add this

    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

// 2. Add these helper methods to your BST class:
int height(Node* node) {
    return node ? node->height : 0;
}

int getBalanceFactor(Node* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

Node* rightRotate(Node* y) {
    // Implementation as shown earlier
}

Node* leftRotate(Node* x) {
    // Implementation as shown earlier
}

// 3. Modify your insert method to include balancing
// 4. Modify your delete method to include balancing
```

## 🏗️ Performance Benchmarks: AVL vs. BST

Here's a comparison of performance for common operations with 1 million elements:

| Operation | Regular BST (random data) | Regular BST (sorted data) | AVL Tree (random data)      | AVL Tree (sorted data)      |
| --------- | ------------------------- | ------------------------- | --------------------------- | --------------------------- |
| Search    | ~20 comparisons           | ~500,000 comparisons      | ~20 comparisons             | ~20 comparisons             |
| Insert    | ~20 comparisons           | ~500,000 comparisons      | ~20 comparisons + rotations | ~20 comparisons + rotations |
| Delete    | ~20 comparisons           | ~500,000 comparisons      | ~20 comparisons + rotations | ~20 comparisons + rotations |

The key insight: AVL trees maintain logarithmic performance regardless of input order, while regular BSTs can deteriorate dramatically with unfavorable data patterns.

## 🔮 Tomorrow's Preview

Tomorrow, we'll dive into the Heap data structure, which is a complete binary tree with a special ordering property. We'll learn:

- The structure and properties of min and max heaps
- How to efficiently implement heap operations
- Using heaps to implement priority queues
- Heap sort algorithm and its analysis
- Real-world applications where heaps shine

_If AVL trees are like perfectly balanced scales that automatically adjust to maintain equilibrium, heaps are like organized pyramids where the most important element always stays at the top. While AVL trees excel at ordered operations like searching in logarithmic time, heaps will show us how to efficiently find the minimum or maximum element regardless of the overall ordering._

Stay tuned for our journey into the world of heaps!

#DSAin45 #Day24 #AVLTrees #SelfBalancingTrees #DataStructures #CPlusPlus #TreeRotations #Algorithms #CodingInterview #BalancedTrees
