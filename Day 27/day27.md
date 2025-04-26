# 🔍 DAY 27: Trie Data Structure - Efficient String Storage and Retrieval #DSAin45

Welcome to Day 27 of our 45-day DSA journey! Yesterday, we explored Heap Sort and its applications. Today, we're diving into Tries, a specialized tree-based data structure that excels at string operations.

_Imagine having a dictionary that lets you look up not just whole words, but also all words sharing a common prefix—and doing it with lightning speed. That's the power of tries: they organize characters in a way that makes string operations like autocomplete, spell checking, and prefix matching incredibly efficient._

## 📌 What is a Trie?

A Trie (pronounced "try" or "tree") is a tree-like data structure that stores a dynamic set of strings, where the keys are usually strings. Unlike a binary search tree, nodes in a trie aren't associated with actual values. Instead, the position of a node in the tree defines the key it's associated with.

The name "trie" comes from the word "retrieval," highlighting its primary purpose: fast information retrieval. Tries are also known as prefix trees, as they excel at finding strings with common prefixes.

### Key Features of Tries:

1. **Prefix-Based Organization**: All descendants of a node share a common prefix
2. **Path-Defined Keys**: The path from the root to a node forms a key
3. **Character-Based Branching**: Each node can have up to |Σ| children (where Σ is the alphabet)
4. **End-of-Word Markers**: Special markers indicate complete words
5. **Efficient Prefix Operations**: O(m) lookup time where m is the key length

## 🌲 Trie Structure and Representation

A trie node typically contains:

1. An array or map of child pointers (one for each possible character)
2. A boolean flag indicating if the node represents the end of a word

Here's a simple representation of a trie node in C++:

```cpp
class TrieNode {
public:
    bool isEndOfWord;
    std::unordered_map<char, TrieNode*> children;

    TrieNode() : isEndOfWord(false) {}
};
```

For a visual example, let's see how a trie would store the words "tree", "trie", "try", "tea", and "to":

```
                     root
                    /    \
                   t      ...
                  /
                 r
                / \
               e   y*
              / \
             e*  i
                 |
                 e*
```

The asterisks (\*) indicate nodes where a complete word ends.

## 🔄 Basic Trie Operations

### 1. Insertion

To insert a word into a trie, we start from the root and follow these steps for each character:

1. If a link exists for the current character, follow it
2. If no link exists, create a new node and link to it
3. After processing all characters, mark the last node as the end of a word

```cpp
void insert(const std::string& word) {
    TrieNode* current = root;

    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }

    current->isEndOfWord = true;
}
```

**Time Complexity**: O(m) where m is the length of the word
**Space Complexity**: O(m)

### 2. Search

Searching for a word follows a similar process:

1. Start from the root
2. For each character, follow the corresponding link if it exists
3. If we reach the end of the word, check if the current node is marked as end-of-word
4. Return true if both conditions are met, false otherwise

```cpp
bool search(const std::string& word) {
    TrieNode* current = root;

    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children[c];
    }

    return current->isEndOfWord;
}
```

**Time Complexity**: O(m) where m is the length of the word
**Space Complexity**: O(1)

### 3. Prefix Search

One of the most powerful operations in a trie is prefix searching—finding all words that start with a given prefix:

1. Navigate to the node representing the prefix
2. Perform a depth-first search from that node to collect all words

```cpp
bool startsWith(const std::string& prefix) {
    TrieNode* current = root;

    for (char c : prefix) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children[c];
    }

    return true;
}

std::vector<std::string> findWordsWithPrefix(const std::string& prefix) {
    std::vector<std::string> result;
    TrieNode* current = root;

    // Navigate to the node representing the prefix
    for (char c : prefix) {
        if (current->children.find(c) == current->children.end()) {
            return result; // Empty result if prefix not found
        }
        current = current->children[c];
    }

    // Collect all words starting from the prefix node
    collectWords(current, prefix, result);

    return result;
}

void collectWords(TrieNode* node, const std::string& prefix, std::vector<std::string>& result) {
    if (node->isEndOfWord) {
        result.push_back(prefix);
    }

    for (const auto& pair : node->children) {
        char c = pair.first;
        TrieNode* child = pair.second;
        collectWords(child, prefix + c, result);
    }
}
```

**Time Complexity**: O(m + k) where m is the length of the prefix and k is the number of words matching the prefix
**Space Complexity**: O(k)

### 4. Deletion

Removing a word from a trie is more complex than the other operations because we need to ensure we don't disrupt other words:

1. Find the node corresponding to the last character of the word
2. Unmark it as an end-of-word if it is
3. Remove nodes that are no longer needed (have no children and are not end-of-word)

```cpp
bool remove(const std::string& word) {
    return removeHelper(root, word, 0);
}

bool removeHelper(TrieNode* node, const std::string& word, int depth) {
    // If we've reached the end of the word
    if (depth == word.length()) {
        // Word doesn't exist in trie
        if (!node->isEndOfWord) {
            return false;
        }

        // Unmark as end of word
        node->isEndOfWord = false;

        // Return true if this node has no children (can be deleted)
        return node->children.empty();
    }

    char c = word[depth];

    // If character doesn't exist in trie
    if (node->children.find(c) == node->children.end()) {
        return false;
    }

    // Recursively check if we should delete the child
    bool shouldDeleteChild = removeHelper(node->children[c], word, depth + 1);

    // If we should delete the child
    if (shouldDeleteChild) {
        delete node->children[c];
        node->children.erase(c);

        // Return true if this node can be deleted
        return node->children.empty() && !node->isEndOfWord;
    }

    return false;
}
```

**Time Complexity**: O(m) where m is the length of the word
**Space Complexity**: O(m) due to recursion stack

## 📝 Complete Trie Implementation in C++

Here's a complete implementation of a Trie in C++:

```cpp
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class Trie {
private:
    class TrieNode {
    public:
        bool isEndOfWord;
        std::unordered_map<char, TrieNode*> children;

        TrieNode() : isEndOfWord(false) {}

        ~TrieNode() {
            for (auto& pair : children) {
                delete pair.second;
            }
        }
    };

    TrieNode* root;

    // Helper function to collect words with a given prefix
    void collectWords(TrieNode* node, const std::string& prefix, std::vector<std::string>& result) {
        if (node->isEndOfWord) {
            result.push_back(prefix);
        }

        for (const auto& pair : node->children) {
            char c = pair.first;
            TrieNode* child = pair.second;
            collectWords(child, prefix + c, result);
        }
    }

    // Helper function for removing words
    bool removeHelper(TrieNode* node, const std::string& word, int depth) {
        if (depth == word.length()) {
            if (!node->isEndOfWord) {
                return false;
            }

            node->isEndOfWord = false;
            return node->children.empty();
        }

        char c = word[depth];
        if (node->children.find(c) == node->children.end()) {
            return false;
        }

        bool shouldDeleteChild = removeHelper(node->children[c], word, depth + 1);

        if (shouldDeleteChild) {
            delete node->children[c];
            node->children.erase(c);
            return node->children.empty() && !node->isEndOfWord;
        }

        return false;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        delete root;
    }

    // Insert a word into the trie
    void insert(const std::string& word) {
        TrieNode* current = root;

        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }

        current->isEndOfWord = true;
    }

    // Search for a word in the trie
    bool search(const std::string& word) {
        TrieNode* current = root;

        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }

        return current->isEndOfWord;
    }

    // Check if there is any word in the trie that starts with the given prefix
    bool startsWith(const std::string& prefix) {
        TrieNode* current = root;

        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }

        return true;
    }

    // Find all words with the given prefix
    std::vector<std::string> findWordsWithPrefix(const std::string& prefix) {
        std::vector<std::string> result;
        TrieNode* current = root;

        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return result;
            }
            current = current->children[c];
        }

        collectWords(current, prefix, result);
        return result;
    }

    // Remove a word from the trie
    bool remove(const std::string& word) {
        return removeHelper(root, word, 0);
    }
};
```

## 🔧 Trie Optimizations and Variations

### 1. Array-Based Implementation

For alphabets with a fixed size (e.g., lowercase letters), we can use arrays instead of maps for faster access:

```cpp
class TrieNode {
public:
    bool isEndOfWord;
    TrieNode* children[26]; // For lowercase English letters

    TrieNode() : isEndOfWord(false) {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }

    ~TrieNode() {
        for (int i = 0; i < 26; i++) {
            delete children[i];
        }
    }
};
```

This reduces the overhead of hash map operations, trading space for speed.

### 2. Compressed Trie (Radix Tree)

In a standard trie, each node represents a single character. A compressed trie (or radix tree) merges nodes that have only one child, resulting in nodes that can represent multiple characters:

```
Standard Trie for "test", "team", "tea":

     root
      |
      t
      |
      e
     / \
    s   a
    |   / \
    t   m   #

Compressed Trie:

     root
      |
      te
     / \
    st  a
        / \
       m   #
```

This can significantly reduce space usage, especially for long strings with common prefixes.

### 3. Ternary Search Tree (TST)

A hybrid between a binary search tree and a trie, where each node has three children: left (for smaller characters), middle (for the next character in the string), and right (for larger characters).

```cpp
class TSTNode {
public:
    char character;
    bool isEndOfWord;
    TSTNode* left;
    TSTNode* middle;
    TSTNode* right;

    TSTNode(char c) : character(c), isEndOfWord(false), left(nullptr), middle(nullptr), right(nullptr) {}
};
```

TSTs are more space-efficient than standard tries for sparse datasets.

## 📊 Time and Space Complexity Analysis

Let's analyze the time and space complexity of trie operations and compare them with other data structures:

| Operation     | Trie                     | Hash Table | BST            |
| ------------- | ------------------------ | ---------- | -------------- |
| Insert        | O(m)                     | O(m)       | O(m log n)     |
| Search        | O(m)                     | O(m)       | O(m log n)     |
| Delete        | O(m)                     | O(m)       | O(m log n)     |
| Prefix Search | O(m + k)                 | O(n \* m)  | O(m log n + k) |
| Space         | O(ALPHABET_SIZE _ m _ n) | O(m \* n)  | O(m \* n)      |

Where:

- m is the length of the key (word)
- n is the number of keys (words)
- k is the number of matches for a prefix search

### Memory Footprint Analysis

The space efficiency of a trie depends on several factors:

1. The alphabet size (number of possible characters)
2. The number of shared prefixes
3. The implementation method (map vs. array)

For English words using lowercase letters only, each node could have up to 26 children. With an unordered_map implementation, this can be memory-efficient for sparse alphabets. For a more compact representation, we might use:

1. Compressed tries
2. Bit-level optimizations
3. Array-based implementation with sparse array techniques

## 🚀 Real-World Applications of Tries

### 1. Autocomplete and Search Suggestions

Tries excel at providing autocomplete suggestions as users type. Companies like Google, Amazon, and Netflix use trie-like structures to offer search suggestions:

```cpp
std::vector<std::string> getAutocompleteSuggestions(const std::string& prefix, int limit = 10) {
    std::vector<std::string> suggestions = trie.findWordsWithPrefix(prefix);

    // Sort suggestions by frequency, relevance, etc.
    sortSuggestions(suggestions);

    // Return the top 'limit' suggestions
    return std::vector<std::string>(suggestions.begin(),
                                   suggestions.begin() + std::min(limit, (int)suggestions.size()));
}
```

### 2. Spell Checkers

Tries are used in spell-checking algorithms to quickly verify if a word exists in a dictionary:

```cpp
bool isSpelledCorrectly(const std::string& word) {
    return dictionary.search(word);
}

std::vector<std::string> suggestCorrections(const std::string& misspelled) {
    std::vector<std::string> suggestions;

    // 1. Check for words with one character different
    for (int i = 0; i < misspelled.length(); i++) {
        std::string temp = misspelled;
        for (char c = 'a'; c <= 'z'; c++) {
            temp[i] = c;
            if (dictionary.search(temp)) {
                suggestions.push_back(temp);
            }
        }
    }

    // Add other correction strategies (insertion, deletion, transposition)

    return suggestions;
}
```

### 3. IP Routing (CIDR)

Network routers use trie-like structures for IP address lookups in routing tables:

```cpp
Route* findLongestMatchingPrefix(const std::string& ipAddress) {
    TrieNode* current = root;
    Route* bestMatch = nullptr;
    std::string prefix = "";

    for (char bit : ipAddress) {
        prefix += bit;
        if (current->route) {
            bestMatch = current->route;
        }

        if (!current->children[bit - '0']) {
            break;
        }

        current = current->children[bit - '0'];
    }

    return bestMatch;
}
```

### 4. Text Prediction and Auto-Correction in Mobile Keyboards

Mobile keyboards use tries to suggest the next word or correct typos:

```cpp
std::vector<std::string> predictNextWord(const std::vector<std::string>& previousWords, const std::string& currentWord) {
    // Simple n-gram model using tries
    std::string context = "";
    for (int i = std::max(0, (int)previousWords.size() - 2); i < previousWords.size(); i++) {
        context += previousWords[i] + " ";
    }

    context += currentWord;

    return nGramTrie.findWordsWithPrefix(context);
}
```

### 5. Word Games and Puzzles

Games like Scrabble, Boggle, and crossword puzzles use tries for efficient word validation:

```cpp
bool isValidWord(const std::string& word) {
    return wordTrie.search(word);
}

// For Boggle
void findWordsInBoggle(std::vector<std::vector<char>>& board) {
    std::vector<std::vector<bool>> visited(board.size(), std::vector<bool>(board[0].size(), false));
    std::unordered_set<std::string> foundWords;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            searchBoggle(board, visited, i, j, "", foundWords);
        }
    }

    // Return or process found words
}

void searchBoggle(std::vector<std::vector<char>>& board, std::vector<std::vector<bool>>& visited,
                 int i, int j, std::string current, std::unordered_set<std::string>& foundWords) {
    // Check bounds and if already visited
    if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || visited[i][j]) {
        return;
    }

    // Add current character
    current += board[i][j];

    // If the current string is not a prefix of any word, stop exploration
    if (!dictionary.startsWith(current)) {
        return;
    }

    // Mark as visited
    visited[i][j] = true;

    // If the current string is a word, add it to the found words
    if (dictionary.search(current)) {
        foundWords.insert(current);
    }

    // Explore all 8 directions
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; k++) {
        searchBoggle(board, visited, i + dx[k], j + dy[k], current, foundWords);
    }

    // Backtrack
    visited[i][j] = false;
}
```

## 😄 Trie Jokes and Analogies

1. **The Library Analogy**:
   "A trie is like a library where each book is split into individual letters and stored in a special sequence of shelves. Want to find all books starting with 'prog'? Just walk down the 'p' aisle, take a right at 'r', another right at 'o', and a left at 'g' - and there's your section with 'program', 'programmer', and 'programming'!"

2. **The Family Tree Joke**:
   "Why did the trie node get promoted? Because it knew how to efficiently manage all its children while maintaining perfect prefixes! Unlike my family tree, where finding a relative with a specific name takes O(n) time and several awkward phone calls."

3. **The Auto-Complete Dating App Joke**:
   "I built a dating app that uses tries for messaging. Start typing 'You look...' and it suggests 'intelligent' and 'thoughtful'. We've eliminated 'like your profile picture' from the database after too many complaints."

4. **The Dictionary Analogy**:
   "A trie is like a dictionary that doesn't just tell you if a word exists, but can also tell you all words starting with 'pre' without flipping through every page. It's like having a magical bookmark that instantly takes you to all related words."

5. **The Navigation System Joke**:
   "My car's GPS uses a trie for street names. Type 'Main' and it finds 'Main Street', 'Main Avenue', and 'Main Boulevard' instantly. Unfortunately, in Boston, it just returns 'Error: Too many matches found, please be more specific than just mentioning a direction.'"

6. **The Spelling Bee Analogy**:
   "A trie is like a spelling bee champion who doesn't just know if a word is correct but can also tell you the next letter before you even ask. And unlike that nervous kid on stage, it never takes O(n) time to think about it!"

## 🧩 LeetCode Practice Problems

### Easy:

#### 1. Implement Trie (Prefix Tree) (LeetCode #208)

**Problem:** Implement a trie with insert, search, and startsWith methods.

**Solution:**

```cpp
class Trie {
private:
    struct TrieNode {
        bool isEndOfWord;
        unordered_map<char, TrieNode*> children;

        TrieNode() : isEndOfWord(false) {}

        ~TrieNode() {
            for (auto& pair : children) {
                delete pair.second;
            }
        }
    };

    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        delete root;
    }

    void insert(string word) {
        TrieNode* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    bool search(string word) {
        TrieNode* node = findNode(word);
        return node != nullptr && node->isEndOfWord;
    }

    bool startsWith(string prefix) {
        return findNode(prefix) != nullptr;
    }

private:
    TrieNode* findNode(const string& prefix) {
        TrieNode* current = root;
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return nullptr;
            }
            current = current->children[c];
        }
        return current;
    }
};
```

#### 2. Longest Common Prefix (LeetCode #14)

**Problem:** Find the longest common prefix string amongst an array of strings.

**Solution:**

```cpp
string longestCommonPrefix(vector<string>& strs) {
    if (strs.empty()) return "";

    // Build a trie with all strings
    Trie trie;
    for (const string& str : strs) {
        if (str.empty()) return ""; // If any string is empty, LCP is ""
        trie.insert(str);
    }

    // Find the longest path from root where each node has only one child
    // and no node is end-of-word (except possibly the last)
    string lcp = "";
    TrieNode* current = trie.root;

    while (true) {
        // If current node is end-of-word, we've reached the end of a string
        if (current->isEndOfWord) break;

        // If current node has more than one child, we've reached a divergence
        if (current->children.size() != 1) break;

        // Add the next character to LCP and continue
        auto it = current->children.begin();
        lcp += it->first;
        current = it->second;
    }

    return lcp;
}
```

### Medium:

#### 1. Replace Words (LeetCode #648)

**Problem:** In English, we have a concept called "root", which can be followed by some other word to form another longer word - let's call this word "successor". For example, when the root "an" is followed by the successor word "other", we can form a new word "another".

Given a dictionary consisting of many roots and a sentence consisting of words separated by spaces, replace all the successors in the sentence with the root forming it. If a successor can be replaced by more than one root, replace it with the root that has the shortest length.

**Solution:**

```cpp
string replaceWords(vector<string>& dictionary, string sentence) {
    // Build the trie with dictionary roots
    Trie trie;
    for (const string& root : dictionary) {
        trie.insert(root);
    }

    stringstream ss(sentence);
    string word, result;

    // Process each word in the sentence
    while (ss >> word) {
        string prefix = trie.findShortestPrefix(word);
        if (!prefix.empty()) {
            result += prefix + " ";
        } else {
            result += word + " ";
        }
    }

    // Remove trailing space and return
    if (!result.empty()) {
        result.pop_back();
    }

    return result;
}

// Add to Trie class:
string findShortestPrefix(const string& word) {
    TrieNode* current = root;
    string prefix = "";

    for (char c : word) {
        if (current->isEndOfWord) {
            return prefix; // Found a root
        }

        if (current->children.find(c) == current->children.end()) {
            return ""; // No root found
        }

        prefix += c;
        current = current->children[c];
    }

    // Check if the entire word is a root
    if (current->isEndOfWord) {
        return prefix;
    }

    return ""; // No root found
}
```

#### 2. Design Add and Search Words Data Structure (LeetCode #211)

**Problem:** Design a data structure that supports adding new words and finding if a string matches any previously added string.

**Solution:**

```cpp
class WordDictionary {
private:
    struct TrieNode {
        bool isEndOfWord;
        unordered_map<char, TrieNode*> children;

        TrieNode() : isEndOfWord(false) {}
    };

    TrieNode* root;

    bool searchHelper(const string& word, int index, TrieNode* node) {
        if (index == word.length()) {
            return node->isEndOfWord;
        }

        char c = word[index];

        if (c == '.') {
            // Wildcard: try all possible children
            for (auto& pair : node->children) {
                if (searchHelper(word, index + 1, pair.second)) {
                    return true;
                }
            }
            return false;
        } else {
            // Regular character
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            return searchHelper(word, index + 1, node->children[c]);
        }
    }

public:
    WordDictionary() {
        root = new TrieNode();
    }

    void addWord(string word) {
        TrieNode* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    bool search(string word) {
        return searchHelper(word, 0, root);
    }
};
```

### Hard:

#### 1. Word Search II (LeetCode #212)

**Problem:** Given an m x n board of characters and a list of strings words, return all words on the board. Each word must be constructed from letters of sequentially adjacent cells, where adjacent cells are horizontally or vertically neighboring. The same letter cell may not be used more than once in a word.

**Solution:**

```cpp
vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
    // Build a trie with all the words
    Trie trie;
    for (const string& word : words) {
        trie.insert(word);
    }

    int m = board.size();
    int n = board[0].size();
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    unordered_set<string> result;
    string current = "";

    // Search starting from each cell
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            dfs(board, visited, i, j, trie.root, current, result);
        }
    }

    return vector<string>(result.begin(), result.end());
}

void dfs(vector<vector<char>>& board, vector<vector<bool>>& visited, int i, int j,
         TrieNode* node, string& current, unordered_set<string>& result) {
    // Check bounds and if already visited
    if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || visited[i][j]) {
        return;
    }

    char c = board[i][j];

    // If the current character is not in the trie, stop
    if (node->children.find(c) == node->children.end()) {
        return;
    }

    // Add current character to path
    current.push_back(c);
    visited[i][j] = true;

    // Move to the next node in the trie
    node = node->children[c];

    // If we've found a word, add it to the result
    if (node->isEndOfWord) {
        result.insert(current);
    }

    // Explore in all four directions
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {1, 0, -1, 0};

    for (int k = 0; k < 4; k++) {
        dfs(board, visited, i + dx[k], j + dy[k], node, current, result);
    }

    // Backtrack
    current.pop_back();
    visited[i][j] = false;
}
```

#### 2. Stream of Characters (LeetCode #1032)

**Problem:** Implement the StreamChecker class as follows:

- StreamChecker(words): Constructor, init the data structure with the given words.
- query(letter): Returns true if and only if for some k >= 1, the last k characters queried (in order from oldest to newest, including this letter just queried) spell one of the words in the given list.

**Solution:**

```cpp
class StreamChecker {
private:
    struct TrieNode {
        bool isEndOfWord;
        unordered_map<char, TrieNode*> children;

        TrieNode() : isEndOfWord(false) {}
    };

    TrieNode* root;
    string stream;
    int maxLength;

public:
    StreamChecker(vector<string>& words) {
        root = new TrieNode();
        maxLength = 0;

        // Build trie with REVERSED words
        for (const string& word : words) {
            maxLength = max(maxLength, (int)word.length());
            TrieNode* current = root;

            // Insert the word in reverse
            for (int i = word.length() - 1; i >= 0; i--) {
                char c = word[i];
                if (current->children.find(c) == current->children.end()) {
                    current->children[c] = new TrieNode();
                }
                current = current->children[c];
            }

            current->isEndOfWord = true;
        }
    }

    bool query(char letter) {
        // Add the new letter to the stream
        stream.push_back(letter);

        // Keep only the last maxLength characters
        if (stream.length() > maxLength) {
            stream = stream.substr(stream.length() - maxLength);
        }

        // Check if any suffix is a word
        TrieNode* current = root;
        for (int i = stream.length() - 1; i >= 0; i--) {
            char c = stream[i];
            if (current->children.find(c) == current->children.end()) {
                return false;
            }

            current = current->children[c];
            if (current->isEndOfWord) {
                return true;
            }
        }

        return false;
    }
};
```

## 🔮 Tomorrow's Preview

Tomorrow, we'll explore Graph Fundamentals, one of the most versatile and important data structures in computer science. We'll learn:

- How graphs represent relationships between entities
- Different ways to represent graphs (adjacency matrix, adjacency list)
- Basic graph properties and terminology
- Implementing graphs efficiently in C++
- Common operations on graphs
- Real-world applications like social networks, maps, and recommendation systems

_If tries are like word dictionaries with efficient prefix searches, graphs are like interconnected maps that can model virtually any relationship in the real world—from friends in a social network to roads between cities, and even the interactions between atoms in a molecule. Graphs give us the power to model and analyze complex systems with elegance and efficiency, making them an indispensable tool in any programmer's toolkit._

Stay tuned for our journey into the fascinating world of graphs!

#DSAin45 #Day27 #Trie #PrefixTree #DataStructures #Algorithms #CPlusPlus #StringProcessing #CodingInterview
