# 🏆 DAY 14: Weekly Challenge & Recap - Mastering the Fundamentals #DSAin45

Congratulations on reaching Day 14! We've completed three full weeks of our 45-day DSA journey, covering the essential building blocks that will serve as the foundation for everything that follows. Today, we'll consolidate your knowledge by tackling challenging problems that combine multiple data structures, review key concepts, and prepare for the exciting topics ahead.

_It's like we've been collecting powerful tools for our toolbox over the past three weeks. Today, we put them all together to build something impressive!_

## 🔍 Recap of Our Journey So Far

### Week 1: C++ Fundamentals & Basic Data Structures

- **Day 1:** Introduction to DSA and setting up our C++ environment
- **Day 2:** Time & space complexity analysis (Big O notation)
- **Day 3:** Arrays (static vs dynamic, operations, multidimensional)
- **Day 4:** Strings (implementation, operations, manipulation)
- **Day 5:** Vectors & STL containers (basics and usage patterns)
- **Day 6-7:** Linked lists (singly, doubly, circular implementations)

### Week 2: Basic Data Structures

- **Day 8:** Stacks (implementations, applications)
- **Day 9:** Queues (standard, circular implementations)
- **Day 10:** Deques & priority queues (operations, applications)
- **Day 11-12:** Hash tables (fundamentals, implementation, collision resolution)
- **Day 13:** Sets & Maps in C++ STL (ordered vs unordered variants)

Let's revisit some key principles before diving into our challenges:

## 🧠 Core Principles to Remember

### 1. Choosing the Right Data Structure

| If You Need...                     | Consider Using...                   | Time Complexity Benefits                         |
| ---------------------------------- | ----------------------------------- | ------------------------------------------------ |
| Fast lookups by key                | Hash tables (`unordered_map`)       | O(1) average case lookups                        |
| Maintaining sorted order           | BST-based containers (`map`, `set`) | O(log n) operations with ordering                |
| LIFO processing                    | Stack                               | O(1) for push/pop operations                     |
| FIFO processing                    | Queue                               | O(1) for enqueue/dequeue operations              |
| Both FIFO and LIFO                 | Deque                               | O(1) for operations at both ends                 |
| Highest/lowest priority first      | Priority queue                      | O(1) find max/min, O(log n) insert/remove        |
| Dynamic size with random access    | Vector                              | O(1) access with amortized O(1) insertion at end |
| Fast insertions/deletions anywhere | Linked list                         | O(1) insertions/deletions once position is found |

### 2. Common Time & Space Complexity Pitfalls

- **Nested loops without optimization** can lead to O(n²) time complexity
- **Recursive solutions without memoization** may recalculate the same values repeatedly
- **Unnecessary copying of large data structures** instead of using references
- **Using the wrong container** (e.g., `vector` when many middle insertions are needed)
- **String concatenation in loops** instead of using string builders or reserve

### 3. C++ STL Best Practices

- **Container choice matters greatly** for performance
- **Use reserve for vectors** when size is known to avoid reallocations
- **Prefer emplace over insert** when constructing objects directly in containers
- **Use references for large objects** to avoid copies
- **Understand iterator invalidation rules** for each container

## 🎯 Weekly Challenge Problems

Now let's put your knowledge to the test with three comprehensive problems that will require multiple data structures and algorithms from the past three weeks.

### Challenge #1: Document Word Frequency Analyzer

**Problem:** Design a system to analyze a text document and provide the following information:

1. Top N most frequent words
2. Words that appear exactly K times
3. For a given pair of words, find the minimum distance between them in the document
4. Build a simple "search engine" that returns all sentences containing a specific word

**Input:**

- A text document (string)
- Parameters N and K for the queries
- Word pairs for distance queries
- Search terms for the search engine functionality

**Example:**

```
Document: "The quick brown fox jumps over the lazy dog. The dog was not very lazy. The fox was quick."

Query 1: Top 3 frequent words
Result: "the" (4 times), "fox" (2 times), "quick" (2 times)

Query 2: Words that appear exactly 2 times
Result: "fox", "quick"

Query 3: Minimum distance between "fox" and "dog"
Result: 4 words

Query 4: Search for "lazy"
Result: ["The quick brown fox jumps over the lazy dog."]
```

**Solution:**

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <limits>
#include <set>

class DocumentAnalyzer {
private:
    std::vector<std::string> words; // All words in order
    std::vector<std::string> sentences; // All sentences
    std::unordered_map<std::string, int> wordFrequency; // Word frequency count
    std::unordered_map<std::string, std::vector<int>> wordPositions; // Positions of each word
    std::unordered_map<std::string, std::set<int>> wordToSentences; // Which sentences contain each word

    // Helper function to convert to lowercase and remove punctuation
    std::string normalize(const std::string& word) {
        std::string result;
        for (char c : word) {
            if (std::isalpha(c)) {
                result += std::tolower(c);
            }
        }
        return result;
    }

    // Split document into words and sentences
    void processDocument(const std::string& document) {
        // Split into sentences
        size_t start = 0;
        size_t end = document.find_first_of(".!?");

        while (end != std::string::npos) {
            sentences.push_back(document.substr(start, end - start + 1));
            start = end + 1;

            // Skip whitespace
            while (start < document.length() && std::isspace(document[start])) {
                start++;
            }

            end = document.find_first_of(".!?", start);
        }

        // Process remaining text if any
        if (start < document.length()) {
            sentences.push_back(document.substr(start));
        }

        // Process each sentence
        for (int sentenceIdx = 0; sentenceIdx < sentences.size(); sentenceIdx++) {
            std::istringstream iss(sentences[sentenceIdx]);
            std::string word;

            while (iss >> word) {
                std::string normalizedWord = normalize(word);
                if (!normalizedWord.empty()) {
                    words.push_back(normalizedWord);
                    wordFrequency[normalizedWord]++;
                    wordPositions[normalizedWord].push_back(words.size() - 1);
                    wordToSentences[normalizedWord].insert(sentenceIdx);
                }
            }
        }
    }

public:
    DocumentAnalyzer(const std::string& document) {
        processDocument(document);
    }

    // Query 1: Find top N frequent words
    std::vector<std::pair<std::string, int>> topNFrequentWords(int n) {
        // Use min heap to keep track of top N words
        auto compare = [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second; // Min heap based on frequency
        };

        std::priority_queue<std::pair<std::string, int>,
                           std::vector<std::pair<std::string, int>>,
                           decltype(compare)> minHeap(compare);

        for (const auto& [word, freq] : wordFrequency) {
            minHeap.push({word, freq});
            if (minHeap.size() > n) {
                minHeap.pop(); // Remove lowest frequency
            }
        }

        // Convert heap to vector in descending order of frequency
        std::vector<std::pair<std::string, int>> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top());
            minHeap.pop();
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    // Query 2: Find words that appear exactly K times
    std::vector<std::string> wordsWithExactFrequency(int k) {
        std::vector<std::string> result;

        for (const auto& [word, freq] : wordFrequency) {
            if (freq == k) {
                result.push_back(word);
            }
        }

        // Sort result alphabetically for consistent output
        std::sort(result.begin(), result.end());
        return result;
    }

    // Query 3: Find minimum distance between two words
    int minimumDistance(const std::string& word1, const std::string& word2) {
        std::string norm1 = normalize(word1);
        std::string norm2 = normalize(word2);

        // Check if both words exist
        if (wordPositions.find(norm1) == wordPositions.end() ||
            wordPositions.find(norm2) == wordPositions.end()) {
            return -1; // One or both words not found
        }

        const std::vector<int>& positions1 = wordPositions[norm1];
        const std::vector<int>& positions2 = wordPositions[norm2];

        // Find minimum distance using two-pointer technique
        int minDist = std::numeric_limits<int>::max();
        int i = 0, j = 0;

        while (i < positions1.size() && j < positions2.size()) {
            int dist = std::abs(positions1[i] - positions2[j]);
            minDist = std::min(minDist, dist);

            if (positions1[i] < positions2[j]) {
                i++;
            } else {
                j++;
            }
        }

        return minDist;
    }

    // Query 4: Search for sentences containing a word
    std::vector<std::string> searchSentences(const std::string& word) {
        std::string normWord = normalize(word);
        std::vector<std::string> result;

        if (wordToSentences.find(normWord) != wordToSentences.end()) {
            for (int idx : wordToSentences[normWord]) {
                result.push_back(sentences[idx]);
            }
        }

        return result;
    }
};

// Example usage:
int main() {
    std::string document = "The quick brown fox jumps over the lazy dog. The dog was not very lazy. The fox was quick.";

    DocumentAnalyzer analyzer(document);

    // Query 1: Top N frequent words
    std::cout << "Top 3 frequent words:" << std::endl;
    auto topWords = analyzer.topNFrequentWords(3);
    for (const auto& [word, freq] : topWords) {
        std::cout << word << ": " << freq << " times" << std::endl;
    }

    // Query 2: Words with exact frequency
    std::cout << "\nWords that appear exactly 2 times:" << std::endl;
    auto exactFreqWords = analyzer.wordsWithExactFrequency(2);
    for (const auto& word : exactFreqWords) {
        std::cout << word << std::endl;
    }

    // Query 3: Minimum distance
    std::cout << "\nMinimum distance between 'fox' and 'dog': "
              << analyzer.minimumDistance("fox", "dog") << " words" << std::endl;

    // Query 4: Search sentences
    std::cout << "\nSentences containing 'lazy':" << std::endl;
    auto sentences = analyzer.searchSentences("lazy");
    for (const auto& sentence : sentences) {
        std::cout << sentence << std::endl;
    }

    return 0;
}
```

**Analysis:**

This solution uses multiple data structures to efficiently implement the required operations:

1. **Hash map (unordered_map)** for word frequency counting and mapping words to their positions and sentences
2. **Priority queue** for finding top N frequent words
3. **Vector** for storing words and sentences sequentially
4. **Set** for efficiently storing which sentences contain each word
5. **Two-pointer technique** for finding minimum distance between words

**Time Complexity:**

- Document processing: O(n) where n is the number of words
- Top N frequent words: O(w log N) where w is the number of unique words
- Words with exact frequency: O(w)
- Minimum distance between words: O(p1 + p2) where p1, p2 are occurrences of each word
- Sentence search: O(s) where s is the number of matching sentences

**Space Complexity:** O(n) for storing all words, frequencies, positions, and sentences

### Challenge #2: Balanced Parentheses Extended

**Problem:** Extend the classic balanced parentheses problem to handle multiple types of brackets and perform advanced operations:

1. Check if a string has balanced parentheses of multiple types: (), [], {}
2. If unbalanced, identify the positions of the problematic brackets
3. Provide a minimum set of changes to make the string balanced
4. Implement a bracket completion feature (like in code editors)

**Input:**

- A string containing various types of parentheses and other characters

**Example:**

```
Input: "({[test]})"
Output: Balanced

Input: "({[test])"
Output: Unbalanced - missing closing } at position 2

Input: "({[test])}"
Output: Unbalanced - mismatched brackets: ) at position 8 should be }

Input: "({[test"
Output: Completion: "({[test]})"
```

**Solution:**

```cpp
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <unordered_map>

class BracketAnalyzer {
private:
    // Maps opening brackets to their corresponding closing brackets
    std::unordered_map<char, char> bracketPairs = {
        {'(', ')'},
        {'[', ']'},
        {'{', '}'}
    };

    // Maps closing brackets to their corresponding opening brackets
    std::unordered_map<char, char> reverseBracketPairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    bool isOpeningBracket(char c) {
        return bracketPairs.find(c) != bracketPairs.end();
    }

    bool isClosingBracket(char c) {
        return reverseBracketPairs.find(c) != reverseBracketPairs.end();
    }

public:
    // Check if string has balanced parentheses
    bool isBalanced(const std::string& str) {
        std::stack<std::pair<char, int>> bracketStack; // Stores bracket and its position

        for (int i = 0; i < str.length(); i++) {
            char c = str[i];

            if (isOpeningBracket(c)) {
                bracketStack.push({c, i});
            }
            else if (isClosingBracket(c)) {
                if (bracketStack.empty() ||
                    bracketPairs[bracketStack.top().first] != c) {
                    return false;
                }
                bracketStack.pop();
            }
        }

        return bracketStack.empty();
    }

    // Identify problematic brackets
    std::string findProblem(const std::string& str) {
        std::stack<std::pair<char, int>> bracketStack; // Stores bracket and its position

        for (int i = 0; i < str.length(); i++) {
            char c = str[i];

            if (isOpeningBracket(c)) {
                bracketStack.push({c, i});
            }
            else if (isClosingBracket(c)) {
                if (bracketStack.empty()) {
                    return "Unbalanced - extra closing bracket " + std::string(1, c) +
                           " at position " + std::to_string(i);
                }

                if (bracketPairs[bracketStack.top().first] != c) {
                    return "Unbalanced - mismatched brackets: " + std::string(1, c) +
                           " at position " + std::to_string(i) +
                           " should be " + std::string(1, bracketPairs[bracketStack.top().first]);
                }

                bracketStack.pop();
            }
        }

        if (!bracketStack.empty()) {
            auto [bracket, pos] = bracketStack.top();
            return "Unbalanced - missing closing " + std::string(1, bracketPairs[bracket]) +
                   " at position " + std::to_string(pos);
        }

        return "Balanced";
    }

    // Generate corrections to make the string balanced
    std::string makeBalanced(const std::string& str) {
        std::string result = str;
        std::stack<std::pair<char, int>> bracketStack; // Stores bracket and its position
        std::vector<std::pair<int, char>> corrections; // Position and character to insert/replace

        // First pass: identify mismatches and missing brackets
        for (int i = 0; i < str.length(); i++) {
            char c = str[i];

            if (isOpeningBracket(c)) {
                bracketStack.push({c, i});
            }
            else if (isClosingBracket(c)) {
                if (bracketStack.empty()) {
                    // Extra closing bracket - mark for removal
                    corrections.push_back({i, '\0'});
                }
                else if (bracketPairs[bracketStack.top().first] != c) {
                    // Mismatched bracket - mark for replacement
                    corrections.push_back({i, bracketPairs[bracketStack.top().first]});
                    bracketStack.pop();
                }
                else {
                    // Matching bracket
                    bracketStack.pop();
                }
            }
        }

        // Add missing closing brackets
        while (!bracketStack.empty()) {
            auto [bracket, pos] = bracketStack.top();
            corrections.push_back({str.length(), bracketPairs[bracket]});
            bracketStack.pop();
        }

        // Apply corrections in reverse order to avoid position shifts
        std::sort(corrections.rbegin(), corrections.rend());

        for (const auto& [pos, c] : corrections) {
            if (c == '\0') {
                // Remove character
                result.erase(pos, 1);
            }
            else if (pos >= result.length()) {
                // Append character
                result.push_back(c);
            }
            else {
                // Replace character
                result[pos] = c;
            }
        }

        return result;
    }

    // Complete missing brackets
    std::string completeBrackets(const std::string& str) {
        std::string result = str;
        std::stack<char> bracketStack;

        // First pass: check existing brackets
        for (char c : str) {
            if (isOpeningBracket(c)) {
                bracketStack.push(c);
            }
            else if (isClosingBracket(c)) {
                if (!bracketStack.empty() && bracketPairs[bracketStack.top()] == c) {
                    bracketStack.pop();
                }
            }
        }

        // Add missing closing brackets in reverse order
        while (!bracketStack.empty()) {
            result += bracketPairs[bracketStack.top()];
            bracketStack.pop();
        }

        return result;
    }
};

// Example usage:
int main() {
    BracketAnalyzer analyzer;

    std::vector<std::string> testCases = {
        "({[test]})",
        "({[test])",
        "({[test])}",
        "({[test"
    };

    for (const auto& test : testCases) {
        std::cout << "Input: \"" << test << "\"" << std::endl;

        std::cout << "Balance check: " << analyzer.findProblem(test) << std::endl;

        if (!analyzer.isBalanced(test)) {
            std::cout << "Corrected: \"" << analyzer.makeBalanced(test) << "\"" << std::endl;
            std::cout << "Completion: \"" << analyzer.completeBrackets(test) << "\"" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}
```

**Analysis:**

This solution uses stacks to track the opening and closing brackets:

1. **Stack** for tracking open brackets
2. **Hash maps** for mapping between opening and closing brackets
3. **Vector** for storing corrections to make the string balanced

**Time Complexity:**

- Checking if balanced: O(n)
- Finding problems: O(n)
- Making balanced: O(n)
- Completing brackets: O(n)

**Space Complexity:** O(n) in the worst case for the stack and corrections

### Challenge #3: LRU Cache with Time-Based Expiry

**Problem:** Implement an enhanced LRU Cache that not only evicts the least recently used item when capacity is reached but also supports:

1. Time-based expiry of elements (TTL - Time To Live)
2. Batch operations for getting and setting multiple keys
3. Statistics about cache hits and misses
4. Callbacks when items are evicted

**Input:**

- A series of operations (get, set, batch_get, batch_set)
- Parameters including keys, values, TTL

**Example:**

```
set("key1", "value1", 60)  // TTL of 60 seconds
set("key2", "value2", 30)
get("key1")  // Returns "value1"
get("key3")  // Returns null, cache miss
// After 40 seconds
get("key2")  // Returns null, expired
stats()  // Returns {hits: 1, misses: 2, size: 1, evictions: 0}
```

**Solution:**

```cpp
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <chrono>
#include <functional>
#include <vector>
#include <optional>

template <typename K, typename V>
class EnhancedLRUCache {
private:
    // Structure to store cache entry data
    struct CacheEntry {
        K key;
        V value;
        std::chrono::steady_clock::time_point expiry;

        CacheEntry(const K& k, const V& v, int ttlSeconds)
            : key(k), value(v) {
            if (ttlSeconds > 0) {
                expiry = std::chrono::steady_clock::now() +
                         std::chrono::seconds(ttlSeconds);
            } else {
                // No expiry time (effectively infinite TTL)
                expiry = std::chrono::steady_clock::time_point::max();
            }
        }

        bool isExpired() const {
            return std::chrono::steady_clock::now() > expiry;
        }
    };

    // Maximum capacity of the cache
    size_t capacity;

    // List to keep track of LRU order (most recently used at front)
    std::list<CacheEntry> cacheList;

    // Map for O(1) lookups: key -> iterator to list node
    std::unordered_map<K, typename std::list<CacheEntry>::iterator> cacheMap;

    // Statistics
    size_t hits = 0;
    size_t misses = 0;
    size_t evictions = 0;

    // Callback for eviction events
    std::function<void(const K&, const V&)> evictionCallback = nullptr;

    // Clean expired entries (called periodically or when needed)
    void cleanExpired() {
        auto it = cacheList.begin();
        while (it != cacheList.end()) {
            if (it->isExpired()) {
                K key = it->key;
                V value = it->value;

                if (evictionCallback) {
                    evictionCallback(key, value);
                }

                cacheMap.erase(key);
                it = cacheList.erase(it);
                evictions++;
            } else {
                ++it;
            }
        }
    }

public:
    EnhancedLRUCache(size_t cap) : capacity(cap) {}

    // Set eviction callback
    void setEvictionCallback(const std::function<void(const K&, const V&)>& callback) {
        evictionCallback = callback;
    }

    // Get value for a key if it exists and is not expired
    std::optional<V> get(const K& key) {
        cleanExpired(); // Clean expired entries first

        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            misses++;
            return std::nullopt; // Key not found
        }

        auto listIt = it->second;

        // Check if the entry has expired
        if (listIt->isExpired()) {
            // Remove expired entry
            cacheMap.erase(key);
            cacheList.erase(listIt);
            misses++;
            evictions++;
            return std::nullopt;
        }

        // Move entry to front (mark as most recently used)
        cacheList.splice(cacheList.begin(), cacheList, listIt);
        hits++;
        return listIt->value;
    }

    // Set a key-value pair with a TTL (in seconds)
    void set(const K& key, const V& value, int ttlSeconds = 0) {
        cleanExpired(); // Clean expired entries first

        // Check if key already exists
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            // Update existing entry
            cacheList.erase(it->second);
            cacheMap.erase(it);
        }
        else if (cacheList.size() >= capacity) {
            // Cache is full, remove least recently used (back of list)
            const CacheEntry& entry = cacheList.back();

            if (evictionCallback) {
                evictionCallback(entry.key, entry.value);
            }

            cacheMap.erase(entry.key);
            cacheList.pop_back();
            evictions++;
        }

        // Insert new entry at front
        cacheList.emplace_front(key, value, ttlSeconds);
        cacheMap[key] = cacheList.begin();
    }

    // Batch get operation
    std::unordered_map<K, V> batchGet(const std::vector<K>& keys) {
        std::unordered_map<K, V> result;

        for (const K& key : keys) {
            auto value = get(key);
            if (value.has_value()) {
                result[key] = *value;
            }
        }

        return result;
    }

    // Batch set operation
    void batchSet(const std::unordered_map<K, std::pair<V, int>>& entries) {
        for (const auto& [key, valueTtlPair] : entries) {
            const auto& [value, ttl] = valueTtlPair;
            set(key, value, ttl);
        }
    }

    // Get cache statistics
    std::unordered_map<std::string, size_t> getStats() const {
        return {
            {"hits", hits},
            {"misses", misses},
            {"size", cacheList.size()},
            {"evictions", evictions}
        };
    }

    // Clear the cache
    void clear() {
        cacheList.clear();
        cacheMap.clear();
    }

    // Get current size
    size_t size() const {
        return cacheList.size();
    }
};

// Example usage with a custom time function for testing
class TimeSimulator {
private:
    static std::chrono::steady_clock::time_point currentTime;

public:
    static void advanceTime(int seconds) {
        currentTime += std::chrono::seconds(seconds);
    }

    static std::chrono::steady_clock::time_point now() {
        return currentTime;
    }

    static void reset() {
        currentTime = std::chrono::steady_clock::now();
    }
};

std::chrono::steady_clock::time_point TimeSimulator::currentTime =
    std::chrono::steady_clock::now();

// Override the clock for testing
namespace std {
namespace chrono {
    steady_clock::time_point steady_clock::now() {
        return TimeSimulator::now();
    }
}}

int main() {
    // Create an LRU cache with capacity 3
    EnhancedLRUCache<std::string, std::string> cache(3);

    // Set eviction callback
    cache.setEvictionCallback([](const std::string& key, const std::string& value) {
        std::cout << "Evicted: " << key << " -> " << value << std::endl;
    });

    // Initial operations
    std::cout << "Initial operations:" << std::endl;
    cache.set("key1", "value1", 60);  // TTL of 60 seconds
    cache.set("key2", "value2", 30);  // TTL of 30 seconds

    // Get operations
    auto value1 = cache.get("key1");
    auto value3 = cache.get("key3");  // Key doesn't exist

    std::cout << "key1: " << (value1 ? *value1 : "not found") << std::endl;
    std::cout << "key3: " << (value3 ? *value3 : "not found") << std::endl;

    // Print initial stats
    auto stats = cache.getStats();
    std::cout << "Stats: hits=" << stats["hits"]
              << ", misses=" << stats["misses"]
              << ", size=" << stats["size"]
              << ", evictions=" << stats["evictions"] << std::endl;

    // Advance time by 40 seconds (key2 should expire)
    std::cout << "\nAfter 40 seconds:" << std::endl;
    TimeSimulator::advanceTime(40);

    auto value2 = cache.get("key2");  // Should be expired
    std::cout << "key2: " << (value2 ? *value2 : "not found (expired)") << std::endl;

    // Batch operations
    std::cout << "\nBatch operations:" << std::endl;
    cache.batchSet({
        {"batch1", {"batch-value1", 100}},
        {"batch2", {"batch-value2", 100}}
    });

    auto batchResults = cache.batchGet({"key1", "batch1", "batch2", "nonexistent"});

    std::cout << "Batch get results:" << std::endl;
    for (const auto& [key, value] : batchResults) {
        std::cout << key << ": " << value << std::endl;
    }

    // Final stats
    stats = cache.getStats();
    std::cout << "\nFinal stats: hits=" << stats["hits"]
              << ", misses=" << stats["misses"]
              << ", size=" << stats["size"]
              << ", evictions=" << stats["evictions"] << std::endl;

    return 0;
}
```

**Analysis:**

This enhanced LRU Cache implementation uses:

1. **Doubly linked list** (std::list) to maintain the order of elements for LRU policy
2. **Hash map** (unordered_map) for O(1) lookups
3. **Chrono library** for handling time-based expiry
4. **Callbacks** for custom actions on eviction
5. **Optional type** for cleaner return values

**Time Complexity:**

- Get: O(1) amortized (occasional cleaning of expired entries)
- Set: O(1) amortized
- Batch operations: O(n) where n is the number of keys in the batch
- Cleaning expired entries: O(n) in the worst case, but amortized over operations

**Space Complexity:** O(capacity) for storing the cache items

## 💡 Common Interview Mistakes and How to Avoid Them

As you prepare for technical interviews, be aware of these common pitfalls:

### 1. Rushing to Code Without Planning

**Mistake:** Immediately starting to code without understanding the problem or having a clear plan.

**Solution:**

- Spend time understanding the problem completely
- Discuss your approach with the interviewer
- Start with a high-level algorithm
- Break down complex problems into smaller steps

### 2. Using the Wrong Data Structure

**Mistake:** Defaulting to familiar data structures without considering performance implications.

**Solution:**

- Consider time/space complexity requirements first
- Think about the operations required (insertions, deletions, lookups)
- Choose data structures based on their strengths for those operations
- Be ready to justify your choice

### 3. Overlooking Edge Cases

**Mistake:** Testing only the happy path and missing critical edge cases.

**Solution:**

- Systematically identify edge cases: empty inputs, single elements, duplicates, etc.
- Test your algorithm with these cases before finalizing
- Think about potential integer overflows, null pointers, etc.

### 4. Inefficient Algorithms

**Mistake:** Implementing the first solution that comes to mind without considering efficiency.

**Solution:**

- Analyze the time and space complexity of your solution
- Consider if there are more efficient approaches
- Look for opportunities to optimize (e.g., using hash maps for lookups)
- Consider amortized cost for operations

### 5. Poor Code Organization

**Mistake:** Writing monolithic, hard-to-understand code.

**Solution:**

- Use helper functions to make your code modular
- Choose meaningful variable names
- Add comments for complex logic
- Structure your code logically

### 6. Ignoring Interviewer Hints

**Mistake:** Not paying attention to subtle hints or feedback from the interviewer.

**Solution:**

- Listen actively to interviewer comments
- Ask clarifying questions
- Be open to changing your approach
- Treat the interview as a collaborative problem-solving session

## 🚀 Performance Optimization Tips

To take your solutions to the next level, consider these optimization strategies:

### 1. Pre-allocate Memory

```cpp
// Inefficient
std::vector<int> values;
for (int i = 0; i < 10000; i++) {
    values.push_back(i);  // May cause multiple reallocations
}

// Optimized
std::vector<int> values;
values.reserve(10000);  // Pre-allocate memory
for (int i = 0; i < 10000; i++) {
    values.push_back(i);  // No reallocations needed
}
```

### 2. Use References to Avoid Copies

```cpp
// Inefficient
void processVector(std::vector<int> vec) {  // Copies the entire vector
    // Process vec
}

// Optimized
void processVector(const std::vector<int>& vec) {  // Uses reference, no copy
    // Process vec
}
```

### 3. Choose the Right Container

```cpp
// Inefficient for many insertions/deletions in the middle
std::vector<int> data;
// Many insertions and deletions in the middle

// Optimized for insertions/deletions at arbitrary positions
std::list<int> data;  // Better for frequent insertions/deletions

// Optimized for fast lookups
std::unordered_map<int, int> data;  // O(1) lookups
```

### 4. Use Move Semantics for Better Performance

```cpp
// Inefficient
std::vector<std::string> result;
std::string temp = createLongString();
result.push_back(temp);  // Copies temp

// Optimized
std::vector<std::string> result;
result.push_back(std::move(createLongString()));  // Moves instead of copying
```

### 5. Minimize String Concatenation

```cpp
// Inefficient
std::string result;
for (int i = 0; i < 1000; i++) {
    result += std::to_string(i);  // Creates new string each time
}

// Optimized
std::string result;
result.reserve(6000);  // Estimate size needed
for (int i = 0; i < 1000; i++) {
    result += std::to_string(i);  // Still not ideal, but better with reserve
}

// Even better
std::ostringstream oss;
for (int i = 0; i < 1000; i++) {
    oss << i;
}
std::string result = oss.str();
```

### 6. Use Compiler Optimizations

```bash
# Basic compilation
g++ -o program program.cpp

# With optimizations
g++ -O3 -o program program.cpp
```

## 🔮 Looking Ahead: Week 4

In Week 4, we'll dive into the world of trees, which are hierarchical data structures that will open up new possibilities for organizing and processing data efficiently. Get ready to explore:

- Binary trees and their traversal algorithms
- Binary search trees for efficient lookups, insertions, and deletions
- Balanced trees like AVL trees to maintain performance
- Heaps and their applications like priority queues
- Tries for efficient string operations

Trees are foundational for many advanced algorithms and applications, from database indexing to compiler design. The concepts we've learned in the first three weeks will be essential as we explore these more complex structures.

_Think of trees as the next evolutionary step after linear data structures—they add a new dimension to how we organize data, creating relationships that can't be represented with simple lists or arrays._

## 🎉 Congratulations!

You've completed the first three weeks of our DSA journey! You've built a solid foundation of programming fundamentals, time and space complexity analysis, and mastered a variety of essential data structures. The problems we've tackled today demonstrate how powerful these tools can be when combined effectively.

Remember, the key to mastering DSA is consistent practice and application. Try implementing variations of the challenge problems, explore different optimization strategies, and don't shy away from tackling difficult problems—they're opportunities to deepen your understanding.

Keep up the great work, and get ready for an exciting exploration of trees in Week 4!

#DSAin45 #Day14 #DataStructures #Algorithms #CodingInterview #CPlusPlus #WeeklyChallenges
