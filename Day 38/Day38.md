# Day 38: Greedy Algorithms #DSAin45

## Introduction

Welcome to Day 38 of our #DSAin45 journey! Yesterday, we concluded our exploration of Dynamic Programming with advanced techniques for solving complex optimization problems. Today, we're switching gears to study **Greedy Algorithms** - a powerful algorithmic paradigm that makes locally optimal choices at each step with the hope of finding a global optimum. Unlike DP, which explores all possible solutions, greedy algorithms commit to decisions immediately without reconsidering them.

## Core Concepts of Greedy Algorithms

### What Makes an Algorithm "Greedy"?

A greedy algorithm builds a solution piece by piece, always choosing the next piece that offers the most immediate benefit. The algorithm never reconsiders its choices - once a decision is made, it's final. This "greedy" approach can be summarized as:

1. **Make the locally optimal choice at each step**
2. **Hope that these choices lead to a globally optimal solution**
3. **Never backtrack or reconsider previous decisions**

This might remind you of hiking up a mountain by always walking in the steepest upward direction. Sometimes this leads you to the summit (optimal solution), but sometimes you might get stuck at a lower peak (local optimum).

### When Do Greedy Algorithms Work?

Greedy algorithms work correctly when the problem has these two key properties:

1. **Greedy Choice Property**: A global optimum can be reached by making locally optimal choices. In other words, making the best choice at each step leads to the overall best solution.

2. **Optimal Substructure**: An optimal solution to the problem contains optimal solutions to its subproblems.

If a problem doesn't satisfy these properties, a greedy approach might give suboptimal results.

### Greedy vs. Dynamic Programming

| Aspect            | Greedy                                              | Dynamic Programming                               |
| ----------------- | --------------------------------------------------- | ------------------------------------------------- |
| Decision making   | Makes immediate decisions without reconsideration   | Explores all possible decisions before committing |
| Memory usage      | Typically lower (O(1) or O(n))                      | Often higher (O(n) to O(n²))                      |
| Speed             | Usually faster                                      | Generally slower                                  |
| Correctness       | Works only for problems with greedy choice property | Works for problems with optimal substructure      |
| Optimal guarantee | Only for certain problems                           | For all problems it applies to                    |

As one professor likes to joke: "Greedy is like getting married after the first date, while DP is like dating everyone in town before deciding!" 😄

## Classic Greedy Algorithms and Problems

### 1. Activity Selection Problem

**Problem**: Given a set of activities with start and finish times, select the maximum number of non-overlapping activities.

**Greedy Approach**: Sort activities by finish time and select activities that don't conflict.

```cpp
vector<int> activitySelection(vector<int>& start, vector<int>& finish) {
    // Sort activities by finish time
    vector<pair<int, int>> activities;
    for (int i = 0; i < start.size(); i++) {
        activities.push_back({finish[i], i});
    }
    sort(activities.begin(), activities.end());

    vector<int> result;
    result.push_back(activities[0].second); // Select first activity
    int lastFinishTime = activities[0].first;

    for (int i = 1; i < activities.size(); i++) {
        // If this activity starts after the last selected activity finishes
        if (start[activities[i].second] >= lastFinishTime) {
            result.push_back(activities[i].second);
            lastFinishTime = activities[i].first;
        }
    }

    return result;
}
```

**Proof of Correctness**: This works because by selecting activities that finish earliest, we maximize the room for scheduling more activities later.

### 2. Fractional Knapsack

**Problem**: Given weights and values of n items, put these items in a knapsack of capacity W to get the maximum total value. Unlike 0/1 Knapsack, we can break items into fractions.

**Greedy Approach**: Sort items by value-to-weight ratio and take items with highest ratio first.

```cpp
double fractionalKnapsack(vector<int>& values, vector<int>& weights, int capacity) {
    vector<pair<double, int>> valuePerWeight;
    for (int i = 0; i < values.size(); i++) {
        valuePerWeight.push_back({(double)values[i] / weights[i], i});
    }

    // Sort by value-to-weight ratio in descending order
    sort(valuePerWeight.rbegin(), valuePerWeight.rend());

    double totalValue = 0.0;
    int remainingCapacity = capacity;

    for (auto [ratio, idx] : valuePerWeight) {
        if (weights[idx] <= remainingCapacity) {
            // Take the whole item
            totalValue += values[idx];
            remainingCapacity -= weights[idx];
        } else {
            // Take a fraction of the item
            totalValue += values[idx] * ((double)remainingCapacity / weights[idx]);
            break;
        }
    }

    return totalValue;
}
```

**Note**: Unlike the 0/1 Knapsack we solved with DP, the fractional version has the greedy choice property.

### 3. Huffman Coding

**Problem**: Given a set of characters and their frequencies, construct a variable-length prefix code to minimize the total encoding length.

**Greedy Approach**:

1. Create a leaf node for each character and build a min-heap
2. Extract two nodes with the minimum frequencies
3. Create a new internal node with these two nodes as children, with frequency = sum of two nodes' frequencies
4. Repeat steps 2 and 3 until the heap contains only one node

This algorithm builds an optimal prefix tree (no code is a prefix of another code) by merging the least frequent characters first.

```cpp
// Node structure for Huffman tree
struct Node {
    char data;
    unsigned freq;
    Node *left, *right;

    Node(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Function to print Huffman codes
void printCodes(Node* root, string str, unordered_map<char, string>& huffmanCode) {
    if (!root) return;

    // If this is a leaf node, then it contains a character
    if (!root->left && !root->right) {
        huffmanCode[root->data] = str;
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

// Main function to build Huffman Tree and print codes
unordered_map<char, string> huffmanCodes(string text) {
    // Count frequency of characters
    unordered_map<char, unsigned> freq;
    for (char c : text) {
        freq[c]++;
    }

    // Create a priority queue (min-heap)
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Create a leaf node for each character and add to priority queue
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build Huffman Tree
    while (pq.size() > 1) {
        // Extract two nodes with lowest frequency
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // Create a new internal node with these two nodes as children
        // and with frequency equal to sum of the two nodes' frequencies
        Node* top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        // Add this node to the priority queue
        pq.push(top);
    }

    // The remaining node is the root of Huffman Tree
    Node* root = pq.top();

    // Print Huffman codes
    unordered_map<char, string> huffmanCode;
    printCodes(root, "", huffmanCode);

    return huffmanCode;
}
```

**Time Complexity**: O(n log n) where n is the number of unique characters.

### 4. Dijkstra's Algorithm

**Problem**: Find the shortest path from a source vertex to all other vertices in a weighted graph.

**Greedy Approach**: Always choose the vertex with the minimum distance from the source.

We'll discuss this algorithm in more detail when we revisit graph algorithms, but it's worth noting that Dijkstra's algorithm is a classic example of a greedy approach.

## When Greedy Algorithms Fail

Greedy algorithms don't always produce the optimal solution. Let's look at some examples where they fail:

### 1. Coin Change Problem

For the US coin system (1¢, 5¢, 10¢, 25¢), a greedy approach works for making change with the minimum number of coins. But for arbitrary coin denominations, it may fail.

Consider denominations {1, 3, 4} and amount 6:

- Greedy solution: 4 + 1 + 1 = 3 coins
- Optimal solution: 3 + 3 = 2 coins

### 2. Traveling Salesman Problem

The nearest neighbor algorithm (always visit the closest unvisited city next) is a greedy approach, but it can produce paths far from optimal.

### 3. Knapsack 0/1 Problem

Unlike the fractional knapsack, the 0/1 version (where we can't take fractions of items) doesn't have the greedy choice property. That's why we needed dynamic programming for it.

As programmers joke: "Greedy algorithms are like shopping sprees - they take what looks best right now, but you might regret it later!" 🛒

## Proofs of Correctness

For greedy algorithms, proving correctness typically involves two steps:

1. **Prove the greedy choice property**: Show that a locally optimal choice is part of some globally optimal solution.
2. **Prove optimal substructure**: Show that once the first greedy choice is made, the remaining subproblem has an optimal solution that, combined with the greedy choice, forms an optimal solution to the original problem.

Let's prove the correctness of the Activity Selection problem:

**Theorem**: Sorting activities by finish time and selecting non-conflicting activities greedily results in a maximum size set of non-conflicting activities.

**Proof**:

1. Let A be our greedy solution and O be an optimal solution.
2. Let a₁, a₂, ..., aₙ be the activities in A, sorted by finish time.
3. Let o₁, o₂, ..., oₘ be the activities in O, sorted by finish time.
4. If n ≥ m, then A is optimal.
5. If n < m, consider the first k where aₖ ≠ oₖ.
6. Since the greedy algorithm chooses the activity with earliest finish time, f(aₖ) ≤ f(oₖ).
7. We can replace oₖ with aₖ in O without creating conflicts, creating a new optimal solution that agrees with A on the first k activities.
8. By induction, we can transform O into A without reducing the size, proving A is optimal.

## Real-World Applications

### 1. Network Routing

Routing algorithms in computer networks often use greedy approaches like Dijkstra's algorithm to find the shortest path between nodes.

### 2. Compression Algorithms

Huffman coding is used in many compression tools like GZIP and JPEG to efficiently encode data.

### 3. Scheduling Systems

From operating systems to factory floors, activity selection and job scheduling often use greedy algorithms.

### 4. Resource Allocation

Cloud computing platforms use greedy approaches to allocate resources efficiently.

### 5. Machine Learning

Some decision tree algorithms use greedy approaches for feature selection and splitting criteria.

## Edge Cases and Optimizations

### Handling Edge Cases

1. **Empty input**: Always check if the input is empty before processing.
2. **Single element**: Ensure your algorithm works correctly with a single element.
3. **Negative values**: Some greedy algorithms assume positive values; handle negatives explicitly.
4. **Ties in greedy criteria**: Define a consistent tie-breaking mechanism.

### Optimizations

1. **Precomputation**: Sort or preprocess data once rather than repeatedly.
2. **Data structures**: Use appropriate structures (priority queues, binary heaps) for efficient extraction of the current best choice.
3. **Early termination**: Some greedy algorithms can stop once a certain condition is met.
4. **Space optimization**: Often, we don't need to store all intermediate results.

## Practice Problems

### Easy: Minimum Number of Coins for Change (LeetCode 322)

**Problem**: Given a list of coin denominations and a target amount, find the minimum number of coins needed.
**Hint**: For the general case, this requires DP, but try to identify when greedy works (e.g., for specific coin systems).

### Medium: Task Scheduler (LeetCode 621)

**Problem**: Given a list of tasks and a cooldown period, find the minimum time to complete all tasks.
**Hint**: Process tasks with the highest frequency first, utilizing idle periods strategically.

### Hard: Minimum Cost to Connect All Points (LeetCode 1584)

**Problem**: Connect all points with minimum cost where cost is the Manhattan distance.
**Hint**: This is the Minimum Spanning Tree problem. Use Prim's or Kruskal's algorithm.

## Preview of Tomorrow

Tomorrow, we'll dive into **Backtracking Algorithms** - a powerful technique for solving problems by building candidates to the solution incrementally and abandoning candidates ("backtracking") as soon as we determine they cannot lead to a valid solution. It's like exploring a maze with a pencil, marking your path, and erasing it when you hit dead ends. We'll implement classic problems like N-Queens, Sudoku solving, and more!

## Conclusion

Greedy algorithms offer an elegant and efficient approach to solving optimization problems when the "greedy choice property" holds. They're often simpler and faster than dynamic programming but require careful proof to ensure correctness. The key is recognizing when a problem has the necessary structure for a greedy approach.

Remember the greedy programmer's wisdom: "Take what you need now, and hope you don't regret it later!" Sometimes it works brilliantly, and sometimes you need to backtrack... which, coincidentally, is exactly what we'll learn tomorrow! 🚀

#DSAin45 #GreedyAlgorithms #AlgorithmDesign #CPlusPlus #Optimization #ComputerScience
