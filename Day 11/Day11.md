# 🔍 DAY 11: Hash Tables - Part 1 - Key-Value Superpowers #DSAin45

Welcome to Day 11! Yesterday we explored deques and priority queues—flexible data structures for ordered data. Today, we're diving into hash tables—the lightning-fast lookup champions that power everything from databases to language dictionaries.

_In a world where time is money, hash tables are the billionaires of data structures—they can find your data in (almost) constant time, no matter how much stuff you've got stored._

## 🤔 What Is a Hash Table?

A hash table (also called a hash map) is a data structure that implements an associative array—a structure that maps keys to values. It uses a hash function to compute an index into an array of buckets, from which the desired value can be found.

```
  Keys          Hash Function       Array Indices     Values
 ┌─────┐
 │"cat"│             ┌─┐               ┌─┐        ┌─────────┐
 └─────┘ ─────────▶  │h│ ─────────▶    │2│ ─────▶ │"meow"   │
                     └─┘               └─┘        └─────────┘
 ┌─────┐
 │"dog"│             ┌─┐               ┌─┐        ┌─────────┐
 └─────┘ ─────────▶  │h│ ─────────▶    │5│ ─────▶ │"woof"   │
                     └─┘               └─┘        └─────────┘
 ┌─────┐
 │"fox"│             ┌─┐               ┌─┐        ┌─────────┐
 └─────┘ ─────────▶  │h│ ─────────▶    │0│ ─────▶ │"????"   │
                     └─┘               └─┘        └─────────┘
```

> **🧠 Fun Fact**: The concept of hashing dates back to the 1950s, but the term "hash table" was first coined by Hans Peter Luhn of IBM in the 1960s. Interestingly, early hash functions were designed by hand using pencil and paper!

## 📋 Hash Table Operations

A hash table supports these primary operations:

- **Insert**: Add a key-value pair to the hash table
- **Search/Get**: Find the value associated with a specific key
- **Delete**: Remove a key-value pair from the hash table

What makes hash tables special is that these operations typically run in O(1) (constant) time on average. This means that no matter how many elements you have stored, lookups remain lightning-fast!

_Hash tables are like teleportation devices for your data—instead of searching through every element, they can zap directly to the desired location with the push of a button (or hash of a key)._

## 🔢 How Hash Tables Work

The magic of hash tables involves three main components:

1. **Hash Function**: Converts the key into an array index
2. **Array**: Stores the data (or pointers to the data)
3. **Collision Resolution Strategy**: Handles cases when different keys hash to the same index

### 1. Hash Function

A hash function takes a key and returns an integer that serves as the index in the underlying array. A good hash function should:

- Be deterministic (same key always produces same hash)
- Distribute keys uniformly across the array
- Be efficient to compute
- Minimize collisions (different keys producing the same hash)

_A hash function is like a fingerprint scanner for data—it quickly converts complex keys into simple numbers while trying to ensure each key gets its own unique "fingerprint"._

#### Common Hash Function Techniques

##### For Integer Keys

```cpp
int hash(int key, int tableSize) {
    return key % tableSize;
}
```

While simple, this can be problematic if `tableSize` isn't a prime number or if keys have patterns (like all even numbers).

##### For String Keys

A common approach is to use a polynomial rolling hash:

```cpp
int hash(const std::string& key, int tableSize) {
    int hash = 0;
    int p = 31; // Prime number (good for alphabet characters)
    int p_pow = 1;
    
    for (char c : key) {
        hash = (hash + (c - 'a' + 1) * p_pow) % tableSize;
        p_pow = (p_pow * p) % tableSize;
    }
    
    return hash;
}
```

For built-in C++ types, you can also use the standard library hash functions:

```cpp
std::hash<std::string> hasher;
size_t hashValue = hasher("hello") % tableSize;
```

_String hashing is like creating a mathematical fingerprint of text. By combining character positions and values with prime numbers, we get a good distribution while keeping the computation simple._

### 2. The Array (Bucket Array)

The underlying array (often called a bucket array) stores the actual key-value pairs. Its size affects the hash table's performance:

- **Too small**: More collisions, degrading performance
- **Too large**: Wastes memory

The ratio of elements to array size is called the **load factor**:

```
load factor = number of elements / array size
```

Typically, hash tables resize (rehash all elements into a larger array) when the load factor exceeds a certain threshold (commonly 0.7).

_The bucket array is like a parking lot with numbered spaces. The hash function tells you which space to use, but what happens when that space is already taken? That's where collision resolution comes in._

### 3. Collision Resolution

A collision occurs when two different keys hash to the same array index. There are several strategies to handle this:

#### A. Separate Chaining

Each array index points to a linked list (or another container) of all key-value pairs that hash to that index.

```
Index   Buckets
┌───┐   ┌────────┐     ┌────────┐
│ 0 │──▶│Key1:Val1│────▶│Key5:Val5│
└───┘   └────────┘     └────────┘
┌───┐
│ 1 │──▶ (empty)
└───┘
┌───┐   ┌────────┐
│ 2 │──▶│Key2:Val2│
└───┘   └────────┘
┌───┐   ┌────────┐     ┌────────┐     ┌────────┐
│ 3 │──▶│Key3:Val3│────▶│Key6:Val6│────▶│Key7:Val7│
└───┘   └────────┘     └────────┘     └────────┘
┌───┐   ┌────────┐
│ 4 │──▶│Key4:Val4│
└───┘   └────────┘
```

_Separate chaining is like having overflow parking areas; when one space is full, vehicles form a line, and you need to check each one until you find your car._

#### B. Open Addressing

When a collision occurs, we find another open position in the array using a probing sequence. Common probing techniques include:

##### Linear Probing

If position `hash(key)` is filled, try `(hash(key) + 1) % tableSize`, then `(hash(key) + 2) % tableSize`, and so on.

```cpp
int findSlot(int key, int tableSize) {
    int hash = hash(key, tableSize);
    int i = 0;
    
    while (slots[(hash + i) % tableSize] is filled) {
        i++;
    }
    
    return (hash + i) % tableSize;
}
```

_Linear probing is like finding a parking spot when your preferred space is taken: you just keep driving forward one space at a time until you find an empty spot._

##### Quadratic Probing

Instead of checking positions in a linear order, we use a quadratic function:

```cpp
int findSlot(int key, int tableSize) {
    int hash = hash(key, tableSize);
    int i = 0;
    
    while (slots[(hash + i*i) % tableSize] is filled) {
        i++;
    }
    
    return (hash + i*i) % tableSize;
}
```

_Quadratic probing is like checking spots that are increasingly farther from your original spot—first 1 away, then 4, then 9, etc. This helps reduce "clustering" of filled slots._

##### Double Hashing

Use a second hash function to determine the step size between probes:

```cpp
int findSlot(int key, int tableSize) {
    int hash1 = hash1(key, tableSize);
    int hash2 = hash2(key, tableSize);
    int i = 0;
    
    while (slots[(hash1 + i * hash2) % tableSize] is filled) {
        i++;
    }
    
    return (hash1 + i * hash2) % tableSize;
}
```

_Double hashing is like having a primary parking preference and a backup plan—if your first choice is taken, you move according to a separate pattern based on your license plate._

#### C. Robin Hood Hashing

A variation of open addressing that steals slots from "rich" elements (those that are closer to their ideal position) to give to "poor" elements (those far from their ideal position).

_Robin Hood hashing follows the "steal from the rich, give to the poor" approach—keys that are already close to their ideal position might get bumped to make room for keys that would otherwise have to travel farther._

## 🏗️ Implementing a Hash Table in C++

Let's implement a simple hash table using separate chaining:

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility> // for std::pair

template <typename K, typename V>
class HashTable {
private:
    static const int DEFAULT_SIZE = 10;
    std::vector<std::list<std::pair<K, V>>> buckets;
    size_t size;
    
    // Hash function
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }
    
public:
    // Constructor
    HashTable(int bucketSize = DEFAULT_SIZE) : buckets(bucketSize), size(0) {}
    
    // Insert a key-value pair
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        
        // Check if key already exists
        for (auto& pair : buckets[index]) {
            if (pair.first == key) {
                pair.second = value; // Update value if key exists
                return;
            }
        }
        
        // Key doesn't exist, insert new pair
        buckets[index].push_back({key, value});
        size++;
        
        // Check if rehash is needed
        if (load_factor() > 0.7) {
            rehash();
        }
    }
    
    // Get value for a key
    bool get(const K& key, V& value) const {
        size_t index = hash(key);
        
        for (const auto& pair : buckets[index]) {
            if (pair.first == key) {
                value = pair.second;
                return true;
            }
        }
        
        return false; // Key not found
    }
    
    // Remove a key-value pair
    bool remove(const K& key) {
        size_t index = hash(key);
        
        for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
            if (it->first == key) {
                buckets[index].erase(it);
                size--;
                return true;
            }
        }
        
        return false; // Key not found
    }
    
    // Check if key exists
    bool contains(const K& key) const {
        size_t index = hash(key);
        
        for (const auto& pair : buckets[index]) {
            if (pair.first == key) {
                return true;
            }
        }
        
        return false;
    }
    
    // Get current load factor
    double load_factor() const {
        return static_cast<double>(size) / buckets.size();
    }
    
    // Resize and rehash
    void rehash() {
        std::vector<std::list<std::pair<K, V>>> old_buckets = buckets;
        size_t new_size = buckets.size() * 2;
        buckets.clear();
        buckets.resize(new_size);
        size = 0;
        
        // Re-insert all existing pairs
        for (const auto& bucket : old_buckets) {
            for (const auto& pair : bucket) {
                insert(pair.first, pair.second);
            }
        }
    }
    
    // Get number of elements
    size_t get_size() const {
        return size;
    }
    
    // Get number of buckets
    size_t bucket_count() const {
        return buckets.size();
    }
};
```

_This implementation uses separate chaining with linked lists to handle collisions. It automatically resizes when the load factor exceeds 0.7, keeping operations fast even as the table grows._

### Example Usage:

```cpp
int main() {
    HashTable<std::string, int> age_map;
    
    age_map.insert("Alice", 25);
    age_map.insert("Bob", 30);
    age_map.insert("Charlie", 35);
    
    int age;
    if (age_map.get("Bob", age)) {
        std::cout << "Bob's age: " << age << std::endl; // Output: Bob's age: 30
    }
    
    age_map.remove("Alice");
    std::cout << "Contains Alice? " << (age_map.contains("Alice") ? "Yes" : "No") << std::endl; // Output: Contains Alice? No
    
    return 0;
}
```

## ⚡ Time & Space Complexity

The performance of hash table operations depends on the collision resolution strategy, the quality of the hash function, and the load factor.

For a well-designed hash table:

| Operation | Average Case | Worst Case |
| --------- | ------------ | ---------- |
| Insert    | O(1)         | O(n)       |
| Search    | O(1)         | O(n)       |
| Delete    | O(1)         | O(n)       |
| Space     | O(n)         | O(n)       |

The worst-case occurs when all keys hash to the same index, creating a long chain or probe sequence.

### Comparison of Collision Resolution Strategies

| Strategy          | Pros                                | Cons                               | Best Use Case              |
| ----------------- | ----------------------------------- | ---------------------------------- | -------------------------- |
| Separate Chaining | Simple implementation               | Extra memory for linked lists      | When deletions are frequent |
|                   | Works well with high load factors   | Poor cache locality                | Unknown number of elements |
| Linear Probing    | Better cache performance            | Prone to primary clustering        | Small, fixed-size keys     |
|                   | Less memory overhead                | Deletions are tricky               | When memory is constrained |
| Quadratic Probing | Reduces clustering                  | May not probe all buckets          | Medium-sized tables        |
|                   | Still good cache performance        | Complex deletion                   | Few expected deletions     |
| Double Hashing    | Excellent probe sequence            | Computationally more expensive     | Large hash tables          |
|                   | Minimal clustering                  | Requires good secondary hash       | Security-sensitive apps    |
| Robin Hood        | Reduces variance in probe sequence  | More complex implementation        | High-performance systems   |
|                   | Can achieve very high load factors  | Slightly slower insertions         | When load factor is high   |

_Like choosing between a sports car, SUV, and truck—each collision resolution strategy has its own trade-offs. Choose based on your expected usage patterns and constraints._

## 🔍 Understanding Hash Function Quality

A good hash function is crucial for hash table performance. Here are some properties to look for:

### 1. Uniformity

The hash function should distribute keys evenly across buckets. Poor distribution leads to clustering and more collisions.

### 2. Determinism

The same key should always produce the same hash value.

### 3. Efficiency

The hash function should be fast to compute.

### 4. Avalanche Effect

Small changes in the input should produce large changes in the hash value. This reduces the chance of similar keys colliding.

_A good hash function is like a good DJ—it takes requests (keys) and distributes them evenly across the dance floor (array), ensuring no area gets overcrowded while keeping the energy (performance) high._

## 🧮 Common Hash Functions

### 1. Division Method

```cpp
int hash(int key) {
    return key % table_size;
}
```

Simple but effective if `table_size` is a prime number not close to a power of two.

### 2. Multiplication Method

```cpp
int hash(int key) {
    double A = 0.6180339887; // (sqrt(5) - 1) / 2
    double val = key * A;
    val = val - floor(val);
    return floor(table_size * val);
}
```

Works well for any key, but involves floating-point operations.

### 3. Universal Hashing

```cpp
int hash(int key) {
    int a = 1 + rand() % (PRIME - 1);
    int b = rand() % PRIME;
    return ((a * key + b) % PRIME) % table_size;
}
```

Uses random coefficients to create a family of hash functions, making it harder for an adversary to force collisions.

### 4. String Hashing (djb2)

```cpp
unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash;
}
```

A popular string hash function known for its simplicity and good distribution.

_Each hash function has its strengths like different tools in a toolbox—some are simple and fast, others are more complex but offer better protection against malicious inputs._

## 🌎 Real-World Applications of Hash Tables

### 1. Database Indexing

Databases use hash indexes to quickly locate records based on key values:

```cpp
class DatabaseIndex {
private:
    HashTable<int, DatabaseRecord*> index;
    
public:
    void addRecord(int key, DatabaseRecord* record) {
        index.insert(key, record);
    }
    
    DatabaseRecord* findRecord(int key) {
        DatabaseRecord* record = nullptr;
        index.get(key, record);
        return record;
    }
};
```

_Hash indexes turn what would be a full-table scan into a direct lookup, transforming queries from seconds to milliseconds._

### 2. Caching Systems

Web caches, memory caches, and disk caches all use hash tables to store and retrieve data:

```cpp
class Cache {
private:
    HashTable<std::string, std::string> cache;
    
public:
    void store(const std::string& url, const std::string& content) {
        cache.insert(url, content);
    }
    
    bool retrieve(const std::string& url, std::string& content) {
        return cache.get(url, content);
    }
};
```

_Caching systems like Redis and Memcached are essentially sophisticated hash tables, turning slow disk or network operations into lightning-fast memory lookups._

### 3. Symbol Tables in Compilers

Compilers and interpreters use hash tables to track variable names, functions, and their properties:

```cpp
class SymbolTable {
private:
    HashTable<std::string, Symbol> symbols;
    
public:
    void define(const std::string& name, const Symbol& symbol) {
        symbols.insert(name, symbol);
    }
    
    bool lookup(const std::string& name, Symbol& symbol) {
        return symbols.get(name, symbol);
    }
};
```

_Every time a programming language needs to figure out "What is x?" or "What does this function do?", a hash table provides the answer in O(1) time._

### 4. Blockchain and Cryptography

Hash functions are fundamental to blockchain technology and cryptographic systems:

```cpp
class BlockchainNode {
private:
    Block block;
    std::string blockHash;
    
public:
    void setBlock(const Block& b) {
        block = b;
        blockHash = hashBlock(block);
    }
    
    bool verifyBlock() {
        return hashBlock(block) == blockHash;
    }
};
```

_In blockchains, hash functions ensure that changing even a single bit in a block would completely change its hash, making tampering easily detectable._

### 5. Spell Checkers and Dictionaries

Hash tables allow for fast word lookups in spell checkers and dictionary applications:

```cpp
class SpellChecker {
private:
    HashTable<std::string, bool> dictionary;
    
public:
    void loadDictionary(const std::vector<std::string>& words) {
        for (const auto& word : words) {
            dictionary.insert(word, true);
        }
    }
    
    bool isCorrectlySpelled(const std::string& word) {
        bool dummy;
        return dictionary.get(word, dummy);
    }
};
```

_When you type a document, your spell checker can instantly verify each word because it's using a hash table under the hood._

## 🎭 Classic Hash Table Problems

### 1. Two Sum

Find two numbers in an array that add up to a target value:

```cpp
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> hash; // value -> index
    
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        
        if (hash.find(complement) != hash.end()) {
            return {hash[complement], i};
        }
        
        hash[nums[i]] = i;
    }
    
    return {}; // No solution
}
```

_Without a hash table, you'd need to check every pair of numbers, taking O(n²) time. With a hash table, it's just O(n)._

### 2. First Non-Repeating Character

Find the first character in a string that doesn't repeat:

```cpp
char firstNonRepeatingChar(const std::string& s) {
    std::unordered_map<char, int> counts;
    
    // Count occurrences of each character
    for (char c : s) {
        counts[c]++;
    }
    
    // Find first character with count 1
    for (char c : s) {
        if (counts[c] == 1) {
            return c;
        }
    }
    
    return '\0'; // No non-repeating character
}
```

_A linear scan through the string might require checking each character against all others. A hash table lets us count occurrences in a single pass._

### 3. LRU Cache

Implement a Least Recently Used (LRU) cache with O(1) operations:

```cpp
class LRUCache {
private:
    int capacity;
    std::list<std::pair<int, int>> items; // key-value pairs in recency order
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache; // key -> iterator in list
    
public:
    LRUCache(int capacity) : capacity(capacity) {}
    
    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return -1; // Key not found
        }
        
        // Move the accessed item to the front (most recently used)
        items.splice(items.begin(), items, it->second);
        return it->second->second; // Return value
    }
    
    void put(int key, int value) {
        auto it = cache.find(key);
        
        if (it != cache.end()) {
            // Update existing key
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }
        
        // Check if cache is full
        if (items.size() == capacity) {
            // Remove least recently used item
            int lru_key = items.back().first;
            items.pop_back();
            cache.erase(lru_key);
        }
        
        // Insert new item at front
        items.push_front({key, value});
        cache[key] = items.begin();
    }
};
```

_An LRU cache combines a hash table for fast lookups with a linked list for maintaining recency order—a perfect example of combining data structures for powerful results._

## 🚀 Advanced Hash Table Techniques

### 1. Perfect Hashing

Perfect hashing guarantees no collisions by using a two-level hash table structure. This is useful for static sets of keys that never change:

```cpp
class PerfectHashTable {
private:
    std::vector<HashTable<K, V>> buckets;
    
    // First-level hash function
    size_t hash1(const K& key) const {
        return firstLevelHash(key) % buckets.size();
    }
    
public:
    PerfectHashTable(const std::vector<std::pair<K, V>>& data) {
        // Construction involves finding appropriate hash functions
        // and bucket sizes to ensure no collisions
    }
    
    // All operations are guaranteed O(1) worst-case
};
```

_Perfect hashing is like having a custom-designed parking garage where every car has its own pre-assigned spot, with no possibility of conflicts._

### 2. Cuckoo Hashing

Cuckoo hashing uses multiple hash functions and allows displacing existing items:

```cpp
class CuckooHashTable {
private:
    std::vector<K> keys1, keys2;
    std::vector<V> values1, values2;
    
    size_t hash1(const K& key) const {
        return firstHash(key) % keys1.size();
    }
    
    size_t hash2(const K& key) const {
        return secondHash(key) % keys2.size();
    }
    
public:
    void insert(const K& key, const V& value) {
        // Try to place in first table
        // If occupied, displace existing item to second table
        // If second table position is occupied, displace that item
        // Continue this process until all items are placed
        // or a cycle is detected (requiring rehash)
    }
};
```

_Cuckoo hashing is like a game of musical chairs where items keep displacing each other until everyone finds a seat—guaranteeing O(1) worst-case lookups at the cost of more complex insertions._

### 3. Bloom Filters

A Bloom filter is a space-efficient probabilistic data structure that tests whether an element is a member of a set:

```cpp
class BloomFilter {
private:
    std::vector<bool> bits;
    int numHashes;
    
    std::vector<size_t> getHashes(const std::string& key) const {
        std::vector<size_t> hashes(numHashes);
        // Calculate multiple hash values for the key
        return hashes;
    }
    
public:
    BloomFilter(size_t size, int hashes) : bits(size, false), numHashes(hashes) {}
    
    void insert(const std::string& key) {
        for (size_t hash : getHashes(key)) {
            bits[hash % bits.size()] = true;
        }
    }
    
    bool mightContain(const std::string& key) const {
        for (size_t hash : getHashes(key)) {
            if (!bits[hash % bits.size()]) {
                return false; // Definitely not in set
            }
        }
        return true; // Might be in set (false positives possible)
    }
};
```

_A Bloom filter is like a doorman who occasionally lets in uninvited guests (false positives) but never turns away invited ones (no false negatives)—perfect for initial filtering before expensive lookups._

## 😂 Hash Table Jokes

- Why did the hash function go to therapy? It was having an identity crisis, mapping everything to the same bucket!

- I tried to explain hash tables to my cat, but all he understood was "collision" and now he keeps knocking things off my desk.

- Hash tables are like finding your soulmate—if you choose the right hash function, searching becomes trivial.

- A good hash function and a good joke have something in common: they both produce unexpected results that everyone appreciates.

- Hash table motto: "Life is too short for linear searches."

## 📚 LeetCode Practice Problems

To reinforce today's hash table concepts, try these LeetCode problems:

### Easy:
- #1: Two Sum
- #217: Contains Duplicate
- #242: Valid Anagram
- #387: First Unique Character in a String
- #706: Design HashMap

### Medium:
- #3: Longest Substring Without Repeating Characters
- #36: Valid Sudoku
- #49: Group Anagrams
- #146: LRU Cache
- #347: Top K Frequent Elements

### Hard:
- #76: Minimum Window Substring
- #128: Longest Consecutive Sequence
- #460: LFU Cache
- #895: Maximum Frequency Stack

_LeetCode hash table problems are like hash functions—they start simple but can quickly lead to complex and surprising results!_

## 🔮 Tomorrow's Preview:

Tomorrow we'll explore Hash Tables - Part 2, diving deeper into hash table implementations, advanced collision resolution techniques, and practical examples of custom hash tables for different use cases. We'll also look at specialized variants like counting bloom filters, cuckoo filters, and how to design custom hash functions for complex types.

_Tomorrow's hash tables are like today's but with super powers—we'll overclock our O(1) operations and learn to wrangle even the most stubborn collisions!_

#DSAin45 #Day11 #HashTables #CPlusPlus #DataStructures #CodingInterview