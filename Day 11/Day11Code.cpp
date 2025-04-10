// Day 11: Hash Tables - Part 1 #DSAin45
// A comprehensive implementation with various hash table techniques

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility> // for std::pair
#include <algorithm>
#include <functional> // for std::hash
#include <stdexcept>
#include <cmath>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <iomanip> // for std::setw
#include <chrono>
#include <random>

// ===== HASH TABLE IMPLEMENTATIONS =====

// 1. Separate Chaining Hash Table
template <typename K, typename V>
class SeparateChainingHashTable {
private:
    struct KeyValuePair {
        K key;
        V value;
        
        KeyValuePair(const K& k, const V& v) : key(k), value(v) {}
    };
    
    std::vector<std::list<KeyValuePair>> buckets;
    size_t count;
    double max_load_factor;
    
    // Hash function wrapper
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }
    
    // Find a key in a bucket
    typename std::list<KeyValuePair>::iterator findKey(const K& key, std::list<KeyValuePair>& bucket) {
        return std::find_if(bucket.begin(), bucket.end(),
                           [&key](const KeyValuePair& pair) { return pair.key == key; });
    }
    
public:
    // Constructor
    SeparateChainingHashTable(size_t bucket_count = 16, double max_lf = 0.75)
        : buckets(bucket_count), count(0), max_load_factor(max_lf) {}
    
    // Insert a key-value pair
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        auto it = findKey(key, buckets[index]);
        
        if (it != buckets[index].end()) {
            // Key exists, update value
            it->value = value;
        } else {
            // Insert new key-value pair
            buckets[index].emplace_back(key, value);
            count++;
            
            // Check if rehash is needed
            if (load_factor() > max_load_factor) {
                rehash(buckets.size() * 2);
            }
        }
    }
    
    // Get value for a key
    std::optional<V> get(const K& key) const {
        size_t index = hash(key);
        
        for (const auto& pair : buckets[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        
        return std::nullopt; // Key not found
    }
    
    // Remove a key-value pair
    bool remove(const K& key) {
        size_t index = hash(key);
        auto it = findKey(key, buckets[index]);
        
        if (it != buckets[index].end()) {
            buckets[index].erase(it);
            count--;
            return true;
        }
        
        return false; // Key not found
    }
    
    // Check if key exists
    bool contains(const K& key) const {
        size_t index = hash(key);
        
        for (const auto& pair : buckets[index]) {
            if (pair.key == key) {
                return true;
            }
        }
        
        return false;
    }
    
    // Current load factor
    double load_factor() const {
        return static_cast<double>(count) / buckets.size();
    }
    
    // Rehash the table with a new size
    void rehash(size_t new_bucket_count) {
        std::vector<std::list<KeyValuePair>> old_buckets = std::move(buckets);
        buckets.resize(new_bucket_count);
        buckets.clear();
        buckets.resize(new_bucket_count);
        count = 0;
        
        // Re-insert all elements
        for (const auto& bucket : old_buckets) {
            for (const auto& pair : bucket) {
                insert(pair.key, pair.value);
            }
        }
    }
    
    // Statistics for debugging
    void printStats() const {
        std::cout << "Hash Table Stats:" << std::endl;
        std::cout << "  Bucket count: " << buckets.size() << std::endl;
        std::cout << "  Element count: " << count << std::endl;
        std::cout << "  Load factor: " << load_factor() << std::endl;
        
        // Count collisions and empty buckets
        size_t empty_buckets = 0;
        size_t max_chain_length = 0;
        
        for (const auto& bucket : buckets) {
            if (bucket.empty()) {
                empty_buckets++;
            } else {
                max_chain_length = std::max(max_chain_length, bucket.size());
            }
        }
        
        std::cout << "  Empty buckets: " << empty_buckets << " (" 
                  << (100.0 * empty_buckets / buckets.size()) << "%)" << std::endl;
        std::cout << "  Max chain length: " << max_chain_length << std::endl;
    }
    
    // Visualize the hash table distribution
    void visualize() const {
        std::cout << "Hash Table Visualization:" << std::endl;
        std::cout << "  Each '#' represents an element in the bucket" << std::endl;
        
        for (size_t i = 0; i < buckets.size(); i++) {
            std::cout << "  " << std::setw(3) << i << " | ";
            for (size_t j = 0; j < buckets[i].size(); j++) {
                std::cout << "#";
            }
            std::cout << std::endl;
        }
    }
};

// 2. Linear Probing Hash Table
template <typename K, typename V>
class LinearProbingHashTable {
private:
    enum class SlotStatus { EMPTY, OCCUPIED, DELETED };
    
    struct Slot {
        K key;
        V value;
        SlotStatus status;
        
        Slot() : status(SlotStatus::EMPTY) {}
    };
    
    std::vector<Slot> slots;
    size_t count;
    double max_load_factor;
    
    // Hash function wrapper
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % slots.size();
    }
    
    // Find the slot index for a key
    // Returns index where key is found, or first empty/deleted slot if not found
    size_t findSlot(const K& key) const {
        size_t index = hash(key);
        size_t start_index = index;
        
        // Linear probing
        do {
            if (slots[index].status == SlotStatus::EMPTY) {
                // Found empty slot - key not in table
                return index;
            }
            
            if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
                // Found the key
                return index;
            }
            
            // Move to next slot (linear probe)
            index = (index + 1) % slots.size();
        } while (index != start_index);
        
        // Table is full (should not reach here if max_load_factor < 1.0)
        return start_index;
    }
    
public:
    // Constructor
    LinearProbingHashTable(size_t slot_count = 16, double max_lf = 0.7)
        : slots(slot_count), count(0), max_load_factor(max_lf) {}
    
    // Insert a key-value pair
    void insert(const K& key, const V& value) {
        // Check if rehash is needed before insertion
        if (load_factor() > max_load_factor) {
            rehash(slots.size() * 2);
        }
        
        size_t index = findSlot(key);
        
        // If key already exists, update its value
        if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
            slots[index].value = value;
            return;
        }
        
        // Insert new key-value pair
        slots[index].key = key;
        slots[index].value = value;
        slots[index].status = SlotStatus::OCCUPIED;
        count++;
    }
    
    // Get value for a key
    std::optional<V> get(const K& key) const {
        size_t index = findSlot(key);
        
        if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
            return slots[index].value;
        }
        
        return std::nullopt; // Key not found
    }
    
    // Remove a key-value pair
    bool remove(const K& key) {
        size_t index = findSlot(key);
        
        if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
            slots[index].status = SlotStatus::DELETED;
            count--;
            return true;
        }
        
        return false; // Key not found
    }
    
    // Check if key exists
    bool contains(const K& key) const {
        size_t index = findSlot(key);
        return (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key);
    }
    
    // Current load factor
    double load_factor() const {
        return static_cast<double>(count) / slots.size();
    }
    
    // Rehash the table with a new size
    void rehash(size_t new_slot_count) {
        std::vector<Slot> old_slots = std::move(slots);
        slots.clear();
        slots.resize(new_slot_count);
        count = 0;
        
        // Re-insert all occupied elements
        for (const auto& slot : old_slots) {
            if (slot.status == SlotStatus::OCCUPIED) {
                insert(slot.key, slot.value);
            }
        }
    }
    
    // Statistics for debugging
    void printStats() const {
        std::cout << "Hash Table Stats:" << std::endl;
        std::cout << "  Slot count: " << slots.size() << std::endl;
        std::cout << "  Element count: " << count << std::endl;
        std::cout << "  Load factor: " << load_factor() << std::endl;
        
        // Count empty and deleted slots
        size_t empty_slots = 0;
        size_t deleted_slots = 0;
        
        for (const auto& slot : slots) {
            if (slot.status == SlotStatus::EMPTY) {
                empty_slots++;
            } else if (slot.status == SlotStatus::DELETED) {
                deleted_slots++;
            }
        }
        
        std::cout << "  Empty slots: " << empty_slots << " (" 
                  << (100.0 * empty_slots / slots.size()) << "%)" << std::endl;
        std::cout << "  Deleted slots: " << deleted_slots << " (" 
                  << (100.0 * deleted_slots / slots.size()) << "%)" << std::endl;
    }
    
    // Visualize the hash table
    void visualize() const {
        std::cout << "Hash Table Visualization:" << std::endl;
        std::cout << "  'O' = Occupied, 'D' = Deleted, '.' = Empty" << std::endl;
        
        size_t slots_per_row = 32;
        for (size_t i = 0; i < slots.size(); i++) {
            if (i % slots_per_row == 0) {
                std::cout << "  " << std::setw(3) << i << " | ";
            }
            
            char status_char = '.';
            if (slots[i].status == SlotStatus::OCCUPIED) {
                status_char = 'O';
            } else if (slots[i].status == SlotStatus::DELETED) {
                status_char = 'D';
            }
            
            std::cout << status_char;
            
            if ((i + 1) % slots_per_row == 0 || i == slots.size() - 1) {
                std::cout << std::endl;
            }
        }
    }
};

// 3. Double Hashing Hash Table
template <typename K, typename V>
class DoubleHashingHashTable {
private:
    enum class SlotStatus { EMPTY, OCCUPIED, DELETED };
    
    struct Slot {
        K key;
        V value;
        SlotStatus status;
        
        Slot() : status(SlotStatus::EMPTY) {}
    };
    
    std::vector<Slot> slots;
    size_t count;
    double max_load_factor;
    
    // Primary hash function
    size_t hash1(const K& key) const {
        return std::hash<K>{}(key) % slots.size();
    }
    
    // Secondary hash function
    size_t hash2(const K& key) const {
        // Using a different hash seed, ensure result is odd and non-zero
        size_t h2 = 1 + (std::hash<K>{}(key) * 17) % (slots.size() - 1);
        return h2;
    }
    
    // Find the slot index for a key
    size_t findSlot(const K& key) const {
        size_t index = hash1(key);
        size_t step = hash2(key);
        size_t i = 0;
        size_t slot_index;
        
        // Double hashing probe
        do {
            slot_index = (index + i * step) % slots.size();
            
            if (slots[slot_index].status == SlotStatus::EMPTY) {
                // Found empty slot - key not in table
                return slot_index;
            }
            
            if (slots[slot_index].status == SlotStatus::OCCUPIED && slots[slot_index].key == key) {
                // Found the key
                return slot_index;
            }
            
            i++;
        } while (i < slots.size());
        
        // Table is full (should not reach here if max_load_factor < 1.0)
        return index;
    }
    
public:
    // Constructor
    DoubleHashingHashTable(size_t slot_count = 16, double max_lf = 0.7)
        : slots(slot_count), count(0), max_load_factor(max_lf) {}
    
    // Insert a key-value pair
    void insert(const K& key, const V& value) {
        // Check if rehash is needed before insertion
        if (load_factor() > max_load_factor) {
            rehash(slots.size() * 2);
        }
        
        size_t index = findSlot(key);
        
        // If key already exists, update its value
        if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
            slots[index].value = value;
            return;
        }
        
        // Insert new key-value pair
        slots[index].key = key;
        slots[index].value = value;
        slots[index].status = SlotStatus::OCCUPIED;
        count++;
    }
    
    // Get value for a key
    std::optional<V> get(const K& key) const {
        size_t index = findSlot(key);
        
        if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
            return slots[index].value;
        }
        
        return std::nullopt; // Key not found
    }
    
    // Remove a key-value pair
    bool remove(const K& key) {
        size_t index = findSlot(key);
        
        if (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key) {
            slots[index].status = SlotStatus::DELETED;
            count--;
            return true;
        }
        
        return false; // Key not found
    }
    
    // Check if key exists
    bool contains(const K& key) const {
        size_t index = findSlot(key);
        return (slots[index].status == SlotStatus::OCCUPIED && slots[index].key == key);
    }
    
    // Current load factor
    double load_factor() const {
        return static_cast<double>(count) / slots.size();
    }
    
    // Rehash the table with a new size
    void rehash(size_t new_slot_count) {
        std::vector<Slot> old_slots = std::move(slots);
        slots.clear();
        slots.resize(new_slot_count);
        count = 0;
        
        // Re-insert all occupied elements
        for (const auto& slot : old_slots) {
            if (slot.status == SlotStatus::OCCUPIED) {
                insert(slot.key, slot.value);
            }
        }
    }
    
    // Statistics for debugging
    void printStats() const {
        std::cout << "Hash Table Stats:" << std::endl;
        std::cout << "  Slot count: " << slots.size() << std::endl;
        std::cout << "  Element count: " << count << std::endl;
        std::cout << "  Load factor: " << load_factor() << std::endl;
        
        // Count empty and deleted slots
        size_t empty_slots = 0;
        size_t deleted_slots = 0;
        
        for (const auto& slot : slots) {
            if (slot.status == SlotStatus::EMPTY) {
                empty_slots++;
            } else if (slot.status == SlotStatus::DELETED) {
                deleted_slots++;
            }
        }
        
        std::cout << "  Empty slots: " << empty_slots << " (" 
                  << (100.0 * empty_slots / slots.size()) << "%)" << std::endl;
        std::cout << "  Deleted slots: " << deleted_slots << " (" 
                  << (100.0 * deleted_slots / slots.size()) << "%)" << std::endl;
    }
};

// 4. Robin Hood Hashing
template <typename K, typename V>
class RobinHoodHashTable {
private:
    struct Slot {
        K key;
        V value;
        bool occupied;
        size_t probe_distance; // Distance from ideal position
        
        Slot() : occupied(false), probe_distance(0) {}
    };
    
    std::vector<Slot> slots;
    size_t count;
    double max_load_factor;
    
    // Hash function wrapper
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % slots.size();
    }
    
public:
    // Constructor
    RobinHoodHashTable(size_t slot_count = 16, double max_lf = 0.7)
        : slots(slot_count), count(0), max_load_factor(max_lf) {}
    
    // Insert a key-value pair
    void insert(const K& key, const V& value) {
        // Check if rehash is needed before insertion
        if (load_factor() > max_load_factor) {
            rehash(slots.size() * 2);
        }
        
        K curr_key = key;
        V curr_value = value;
        size_t ideal_pos = hash(curr_key);
        size_t curr_probe_distance = 0;
        
        // Linear probing with Robin Hood strategy
        for (size_t i = 0; i < slots.size(); i++) {
            size_t pos = (ideal_pos + i) % slots.size();
            
            // If slot is empty, insert here
            if (!slots[pos].occupied) {
                slots[pos].key = curr_key;
                slots[pos].value = curr_value;
                slots[pos].occupied = true;
                slots[pos].probe_distance = curr_probe_distance;
                count++;
                return;
            }
            
            // If key already exists, update its value
            if (slots[pos].occupied && slots[pos].key == curr_key) {
                slots[pos].value = curr_value;
                return;
            }
            
            // Robin Hood: If current element has higher probe distance than existing one, swap them
            if (curr_probe_distance > slots[pos].probe_distance) {
                // Swap
                std::swap(curr_key, slots[pos].key);
                std::swap(curr_value, slots[pos].value);
                std::swap(curr_probe_distance, slots[pos].probe_distance);
            }
            
            curr_probe_distance++;
        }
        
        // Table is full (should not reach here if max_load_factor < 1.0)
        rehash(slots.size() * 2);
        insert(curr_key, curr_value);
    }
    
    // Get value for a key
    std::optional<V> get(const K& key) const {
        size_t ideal_pos = hash(key);
        
        // Search linearly from ideal position
        for (size_t i = 0; i < slots.size(); i++) {
            size_t pos = (ideal_pos + i) % slots.size();
            
            // If we find an empty slot or a slot with lower probe distance, the key is not in the table
            if (!slots[pos].occupied || slots[pos].probe_distance < i) {
                return std::nullopt;
            }
            
            if (slots[pos].occupied && slots[pos].key == key) {
                return slots[pos].value;
            }
        }
        
        return std::nullopt; // Key not found
    }
    
    // Remove a key-value pair (simplified - does not maintain Robin Hood property on removal)
    bool remove(const K& key) {
        size_t ideal_pos = hash(key);
        
        // Search linearly from ideal position
        for (size_t i = 0; i < slots.size(); i++) {
            size_t pos = (ideal_pos + i) % slots.size();
            
            // If we find an empty slot or a slot with lower probe distance, the key is not in the table
            if (!slots[pos].occupied || slots[pos].probe_distance < i) {
                return false;
            }
            
            if (slots[pos].occupied && slots[pos].key == key) {
                // Mark as empty and decrement count
                slots[pos].occupied = false;
                count--;
                
                // In a real implementation, we'd need to shift elements back
                // to maintain the Robin Hood property, but for simplicity we'll rehash instead
                if (count > 0 && count < slots.size() / 4) {
                    rehash(slots.size() / 2);
                }
                
                return true;
            }
        }
        
        return false; // Key not found
    }
    
    // Check if key exists
    bool contains(const K& key) const {
        return get(key).has_value();
    }
    
    // Current load factor
    double load_factor() const {
        return static_cast<double>(count) / slots.size();
    }
    
    // Rehash the table with a new size
    void rehash(size_t new_slot_count) {
        std::vector<Slot> old_slots = std::move(slots);
        slots.clear();
        slots.resize(new_slot_count);
        count = 0;
        
        // Re-insert all occupied elements
        for (const auto& slot : old_slots) {
            if (slot.occupied) {
                insert(slot.key, slot.value);
            }
        }
    }
    
    // Statistics for debugging
    void printStats() const {
        std::cout << "Robin Hood Hash Table Stats:" << std::endl;
        std::cout << "  Slot count: " << slots.size() << std::endl;
        std::cout << "  Element count: " << count << std::endl;
        std::cout << "  Load factor: " << load_factor() << std::endl;
        
        // Calculate probe distance statistics
        size_t empty_slots = 0;
        double total_probe_dist = 0;
        size_t max_probe_dist = 0;
        
        for (const auto& slot : slots) {
            if (!slot.occupied) {
                empty_slots++;
            } else {
                total_probe_dist += slot.probe_distance;
                max_probe_dist = std::max(max_probe_dist, slot.probe_distance);
            }
        }
        
        std::cout << "  Empty slots: " << empty_slots << " (" 
                  << (100.0 * empty_slots / slots.size()) << "%)" << std::endl;
        std::cout << "  Average probe distance: " 
                  << (count > 0 ? total_probe_dist / count : 0) << std::endl;
        std::cout << "  Maximum probe distance: " << max_probe_dist << std::endl;
    }
};

// 5. Simple Bloom Filter
class BloomFilter {
private:
    std::vector<bool> bits;
    size_t num_hashes;
    
    // Generate multiple hash values for a key
    std::vector<size_t> getHashes(const std::string& key) const {
        std::vector<size_t> hashes(num_hashes);
        
        // Use different seed values for each hash function
        for (size_t i = 0; i < num_hashes; i++) {
            std::hash<std::string> hasher;
            hashes[i] = hasher(key + std::to_string(i)) % bits.size();
        }
        
        return hashes;
    }
    
public:
    // Constructor
    BloomFilter(size_t size, size_t hashes) : bits(size, false), num_hashes(hashes) {}
    
    // Add an element to the filter
    void insert(const std::string& key) {
        for (size_t hash : getHashes(key)) {
            bits[hash] = true;
        }
    }
    
    // Check if an element might be in the set
    bool mightContain(const std::string& key) const {
        for (size_t hash : getHashes(key)) {
            if (!bits[hash]) {
                return false; // Definitely not in set
            }
        }
        return true; // Might be in set (false positives possible)
    }
    
    // Get the approximate false positive rate
    double falsePositiveRate() const {
        // Count set bits
        size_t set_bits = 0;
        for (bool bit : bits) {
            if (bit) set_bits++;
        }
        
        // Calculate probability of a bit being set
        double p = static_cast<double>(set_bits) / bits.size();
        
        // Calculate false positive rate: (1 - (1 - p)^k)^k, where k is num_hashes
        return std::pow(1.0 - std::pow(1.0 - p, num_hashes), num_hashes);
    }
    
    // Print filter statistics
    void printStats() const {
        std::cout << "Bloom Filter Stats:" << std::endl;
        std::cout << "  Size: " << bits.size() << " bits" << std::endl;
        std::cout << "  Number of hash functions: " << num_hashes << std::endl;
        
        size_t set_bits = 0;
        for (bool bit : bits) {
            if (bit) set_bits++;
        }
        
        std::cout << "  Set bits: " << set_bits << " (" 
                  << (100.0 * set_bits / bits.size()) << "%)" << std::endl;
        std::cout << "  Approximate false positive rate: " << (falsePositiveRate() * 100.0) << "%" << std::endl;
    }
};

// ===== HASH FUNCTION EXAMPLES =====

// Various hash function implementations
namespace HashFunctions {
    // Simple modulo hash for integers
    size_t divisionHash(int key, size_t tableSize) {
        return key % tableSize;
    }
    
    // Multiplication method
    size_t multiplicationHash(int key, size_t tableSize) {
        const double A = 0.6180339887; // (sqrt(5) - 1) / 2
        double val = key * A;
        val = val - std::floor(val); // Fractional part
        return std::floor(tableSize * val);
    }
    
    // Universal hashing
    class UniversalHashFunction {
    private:
        int a, b, prime;
        size_t tableSize;
        
    public:
        UniversalHashFunction(size_t tableSize, int prime = 2147483647) // prime is 2^31 - 1
            : tableSize(tableSize), prime(prime) {
            // Generate random coefficients
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist_a(1, prime - 1);
            std::uniform_int_distribution<int> dist_b(0, prime - 1);
            
            a = dist_a(gen);
            b = dist_b(gen);
        }
        
        size_t hash(int key) const {
            return ((static_cast<long long>(a) * key + b) % prime) % tableSize;
        }
    };
    
    // DJB2 string hash function
    size_t djb2Hash(const std::string& str, size_t tableSize) {
        unsigned long hash = 5381;
        
        for (char c : str) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        
        return hash % tableSize;
    }
    
    // FNV-1a hash function for strings
    size_t fnv1aHash(const std::string& str, size_t tableSize) {
        const uint64_t FNV_PRIME = 1099511628211ULL;
        const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
        
        uint64_t hash = FNV_OFFSET_BASIS;
        
        for (char c : str) {
            hash = hash ^ static_cast<uint64_t>(c);
            hash = hash * FNV_PRIME;
        }
        
        return hash % tableSize;
    }
}

// ===== APPLICATIONS =====

// 1. Simple Dictionary (Spell Checker)
class SpellChecker {
private:
    SeparateChainingHashTable<std::string, bool> dictionary;
    
public:
    // Load words into the dictionary
    void loadDictionary(const std::vector<std::string>& words) {
        for (const auto& word : words) {
            dictionary.insert(word, true);
        }
    }
    
    // Check if a word is spelled correctly
    bool isCorrectlySpelled(const std::string& word) const {
        return dictionary.contains(word);
    }
    
    // Suggest corrections for a misspelled word (very basic)
    std::vector<std::string> suggestCorrections(const std::string& word, const std::vector<std::string>& wordList) const {
        std::vector<std::string> suggestions;
        
        // For simplicity, just look for similar words by length
        for (const auto& candidate : wordList) {
            if (std::abs(static_cast<int>(candidate.length() - word.length())) <= 1) {
                suggestions.push_back(candidate);
                if (suggestions.size() >= 5) break; // Limit suggestions
            }
        }
        
        return suggestions;
    }
};

// 2. Two Sum Problem
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> numMap; // value -> index
    
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        
        if (numMap.find(complement) != numMap.end()) {
            return {numMap[complement], i};
        }
        
        numMap[nums[i]] = i;
    }
    
    return {}; // No solution
}

// 3. First Non-Repeating Character
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

// 4. Simple LRU Cache (Least Recently Used)
template <typename K, typename V>
class LRUCache {
private:
    size_t capacity;
    std::list<std::pair<K, V>> items; // Doubly-linked list of key-value pairs
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache; // Key to list iterator mapping
    
public:
    LRUCache(size_t capacity) : capacity(capacity) {}
    
    // Get value for key and mark as recently used
    std::optional<V> get(const K& key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return std::nullopt; // Key not found
        }
        
        // Move item to front of list (mark as most recently used)
        items.splice(items.begin(), items, it->second);
        return it->second->second; // Return value
    }
    
    // Insert or update key-value pair
    void put(const K& key, const V& value) {
        // Check if key already exists
        auto it = cache.find(key);
        if (it != cache.end()) {
            // Update existing key
            it->second->second = value;
            // Move to front (most recently used)
            items.splice(items.begin(), items, it->second);
            return;
        }
        
        // Check if cache is full
        if (items.size() == capacity) {
            // Remove least recently used item
            K lru_key = items.back().first;
            items.pop_back();
            cache.erase(lru_key);
        }
        
        // Insert new item at front
        items.push_front({key, value});
        cache[key] = items.begin();
    }
    
    // Get current size
    size_t size() const {
        return items.size();
    }
    
    // Print the cache contents (most recent first)
    void printContents() const {
        std::cout << "LRU Cache Contents (most recent first):" << std::endl;
        for (const auto& item : items) {
            std::cout << "  " << item.first << " -> " << item.second << std::endl;
        }
    }
};

// ===== PERFORMANCE TESTS =====

// Time hash table operations and compare implementations
void performanceTest() {
    std::cout << "\n===== HASH TABLE PERFORMANCE TEST =====" << std::endl;
    
    // Test parameters
    const size_t num_operations = 100000;
    const int key_range = 1000000;
    
    // Generate random keys
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, key_range);
    
    std::vector<int> keys(num_operations);
    for (size_t i = 0; i < num_operations; i++) {
        keys[i] = dist(gen);
    }
    
    // Test each hash table implementation
    auto test_hashtable = [&](const std::string& name, auto& table) {
        auto start = std::chrono::high_resolution_clock::now();
        
        // Insert operations
        for (size_t i = 0; i < num_operations; i++) {
            table.insert(keys[i], i);
        }
        
        auto mid = std::chrono::high_resolution_clock::now();
        
        // Lookup operations (all keys)
        for (size_t i = 0; i < num_operations; i++) {
            auto result = table.get(keys[i]);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        // Calculate timings
        auto insert_time = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
        auto lookup_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
        
        std::cout << name << ":" << std::endl;
        std::cout << "  Insert time: " << insert_time << " ms" << std::endl;
        std::cout << "  Lookup time: " << lookup_time << " ms" << std::endl;
        std::cout << "  Total time: " << (insert_time + lookup_time) << " ms" << std::endl;
    };
    
    // Create and test each hash table
    SeparateChainingHashTable<int, size_t> chainedTable;
    LinearProbingHashTable<int, size_t> linearTable;
    DoubleHashingHashTable<int, size_t> doubleTable;
    RobinHoodHashTable<int, size_t> robinHoodTable;
    
    test_hashtable("Separate Chaining", chainedTable);
    test_hashtable("Linear Probing", linearTable);
    test_hashtable("Double Hashing", doubleTable);
    test_hashtable("Robin Hood Hashing", robinHoodTable);
    
    // STL hash table as reference
    std::unordered_map<int, size_t> stlMap;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Insert operations
    for (size_t i = 0; i < num_operations; i++) {
        stlMap[keys[i]] = i;
    }
    
    auto mid = std::chrono::high_resolution_clock::now();
    
    // Lookup operations
    for (size_t i = 0; i < num_operations; i++) {
        auto result = stlMap.find(keys[i]);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto insert_time = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
    auto lookup_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
    
    std::cout << "STL unordered_map:" << std::endl;
    std::cout << "  Insert time: " << insert_time << " ms" << std::endl;
    std::cout << "  Lookup time: " << lookup_time << " ms" << std::endl;
    std::cout << "  Total time: " << (insert_time + lookup_time) << " ms" << std::endl;
}

// ===== MAIN FUNCTION =====

int main() {
    std::cout << "===== DAY 11: HASH TABLES - PART 1 DEMONSTRATION =====" << std::endl;
    
    // ===== SEPARATE CHAINING HASH TABLE DEMO =====
    std::cout << "\n===== SEPARATE CHAINING HASH TABLE DEMO =====" << std::endl;
    
    SeparateChainingHashTable<std::string, std::string> animalSounds;
    
    // Insert key-value pairs
    animalSounds.insert("cat", "meow");
    animalSounds.insert("dog", "woof");
    animalSounds.insert("cow", "moo");
    animalSounds.insert("duck", "quack");
    animalSounds.insert("chicken", "cluck");
    
    // Print statistics
    animalSounds.printStats();
    animalSounds.visualize();
    
    // Lookup values
    std::cout << "Animal sounds:" << std::endl;
    for (const auto& animal : {"cat", "dog", "cow", "duck", "chicken", "fox"}) {
        auto sound = animalSounds.get(animal);
        if (sound) {
            std::cout << "  " << animal << " says " << *sound << std::endl;
        } else {
            std::cout << "  " << animal << " sound is unknown" << std::endl;
        }
    }
    
    // Remove a key-value pair
    animalSounds.remove("duck");
    std::cout << "After removing 'duck':" << std::endl;
    std::cout << "  Contains 'duck'? " << (animalSounds.contains("duck") ? "Yes" : "No") << std::endl;
    
    // Update a value
    animalSounds.insert("cow", "mooooo");
    std::cout << "Updated cow sound: " << *animalSounds.get("cow") << std::endl;
    
    // ===== LINEAR PROBING HASH TABLE DEMO =====
    std::cout << "\n===== LINEAR PROBING HASH TABLE DEMO =====" << std::endl;
    
    LinearProbingHashTable<std::string, int> ages;
    
    // Insert key-value pairs
    ages.insert("Alice", 25);
    ages.insert("Bob", 30);
    ages.insert("Charlie", 35);
    ages.insert("David", 40);
    ages.insert("Eve", 45);
    
    // Print statistics
    ages.printStats();
    ages.visualize();
    
    // Lookup values
    std::cout << "Ages:" << std::endl;
    for (const auto& name : {"Alice", "Bob", "Charlie", "David", "Eve", "Frank"}) {
        auto age = ages.get(name);
        if (age) {
            std::cout << "  " << name << " is " << *age << " years old" << std::endl;
        } else {
            std::cout << "  " << name << "'s age is unknown" << std::endl;
        }
    }
    
    // Remove a key-value pair
    ages.remove("Charlie");
    std::cout << "After removing 'Charlie':" << std::endl;
    std::cout << "  Contains 'Charlie'? " << (ages.contains("Charlie") ? "Yes" : "No") << std::endl;
    
    // Test rehashing
    std::cout << "Adding more names to trigger rehash..." << std::endl;
    ages.insert("Frank", 50);
    ages.insert("Grace", 55);
    ages.insert("Hannah", 60);
    ages.insert("Ian", 65);
    ages.insert("Julia", 70);
    
    ages.printStats();
    
    // ===== DOUBLE HASHING DEMO =====
    std::cout << "\n===== DOUBLE HASHING DEMO =====" << std::endl;
    
    DoubleHashingHashTable<int, std::string> numbers;
    
    // Insert key-value pairs
    numbers.insert(1, "one");
    numbers.insert(2, "two");
    numbers.insert(3, "three");
    numbers.insert(4, "four");
    numbers.insert(5, "five");
    
    // Print statistics
    numbers.printStats();
    
    // Lookup values
    std::cout << "Numbers:" << std::endl;
    for (int i = 1; i <= 6; i++) {
        auto name = numbers.get(i);
        if (name) {
            std::cout << "  " << i << " is called " << *name << std::endl;
        } else {
            std::cout << "  " << i << " is unknown" << std::endl;
        }
    }
    
    // ===== ROBIN HOOD HASHING DEMO =====
    std::cout << "\n===== ROBIN HOOD HASHING DEMO =====" << std::endl;
    
    RobinHoodHashTable<std::string, double> prices;
    
    // Insert key-value pairs
    prices.insert("apple", 1.20);
    prices.insert("banana", 0.50);
    prices.insert("cherry", 3.00);
    prices.insert("date", 2.50);
    prices.insert("elderberry", 4.75);
    
    // Print statistics
    prices.printStats();
    
    // Lookup values
    std::cout << "Fruit prices:" << std::endl;
    for (const auto& fruit : {"apple", "banana", "cherry", "date", "elderberry", "fig"}) {
        auto price = prices.get(fruit);
        if (price) {
            std::cout << "  " << fruit << " costs $" << *price << std::endl;
        } else {
            std::cout << "  " << fruit << " price is unknown" << std::endl;
        }
    }
    
    // ===== BLOOM FILTER DEMO =====
    std::cout << "\n===== BLOOM FILTER DEMO =====" << std::endl;
    
    // Create a bloom filter
    // Size = 100 bits, 3 hash functions
    BloomFilter bloomFilter(100, 3);
    
    // Insert elements
    std::vector<std::string> words = {
        "apple", "banana", "cherry", "date", "elderberry"
    };
    
    std::cout << "Adding words to bloom filter..." << std::endl;
    for (const auto& word : words) {
        bloomFilter.insert(word);
        std::cout << "  Added: " << word << std::endl;
    }
    
    // Print statistics
    bloomFilter.printStats();
    
    // Test membership
    std::cout << "Testing bloom filter membership:" << std::endl;
    std::vector<std::string> testWords = {
        "apple", "banana", "fig", "grape", "honeydew"
    };
    
    for (const auto& word : testWords) {
        bool mightContain = bloomFilter.mightContain(word);
        std::cout << "  " << word << " might be in set? " << (mightContain ? "Yes" : "No") << std::endl;
        
        // Check if it's actually in the set (for demonstration)
        bool actuallyContains = std::find(words.begin(), words.end(), word) != words.end();
        if (mightContain && !actuallyContains) {
            std::cout << "    (This is a false positive)" << std::endl;
        }
    }
    
    // ===== APPLICATIONS DEMO =====
    
    // 1. Two Sum Problem
    std::cout << "\n===== TWO SUM PROBLEM DEMO =====" << std::endl;
    
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    
    std::cout << "Array: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Target: " << target << std::endl;
    
    std::vector<int> result = twoSum(nums, target);
    
    if (!result.empty()) {
        std::cout << "Found pair at indices: " << result[0] << " and " << result[1] << std::endl;
        std::cout << "Values: " << nums[result[0]] << " + " << nums[result[1]] << " = " << target << std::endl;
    } else {
        std::cout << "No solution found" << std::endl;
    }
    
    // 2. First Non-Repeating Character
    std::cout << "\n===== FIRST NON-REPEATING CHARACTER DEMO =====" << std::endl;
    
    std::vector<std::string> strings = {
        "leetcode",
        "loveleetcode",
        "aabb"
    };
    
    for (const auto& s : strings) {
        char firstNonRepeating = firstNonRepeatingChar(s);
        std::cout << "String: " << s << std::endl;
        if (firstNonRepeating != '\0') {
            std::cout << "  First non-repeating character: " << firstNonRepeating << std::endl;
        } else {
            std::cout << "  No non-repeating character found" << std::endl;
        }
    }
    
    // 3. LRU Cache
    std::cout << "\n===== LRU CACHE DEMO =====" << std::endl;
    
    LRUCache<int, std::string> lruCache(3);
    
    std::cout << "Adding items to LRU cache (capacity 3)..." << std::endl;
    lruCache.put(1, "one");
    lruCache.put(2, "two");
    lruCache.put(3, "three");
    
    lruCache.printContents();
    
    std::cout << "Accessing key 1..." << std::endl;
    auto value = lruCache.get(1);
    if (value) {
        std::cout << "  Got value: " << *value << std::endl;
    }
    
    lruCache.printContents();
    
    std::cout << "Adding key 4 (should evict key 2)..." << std::endl;
    lruCache.put(4, "four");
    
    lruCache.printContents();
    
    std::cout << "Trying to access key 2..." << std::endl;
    value = lruCache.get(2);
    if (value) {
        std::cout << "  Got value: " << *value << std::endl;
    } else {
        std::cout << "  Key not found (evicted)" << std::endl;
    }
    
    // ===== HASH FUNCTION EXAMPLES =====
    std::cout << "\n===== HASH FUNCTION EXAMPLES =====" << std::endl;
    
    // Test integer hash functions
    std::vector<int> testInts = {12345, 67890, 13579, 24680, 99999};
    size_t tableSize = 16;
    
    std::cout << "Division Method Hash:" << std::endl;
    for (int key : testInts) {
        std::cout << "  hash(" << key << ") = " 
                  << HashFunctions::divisionHash(key, tableSize) << std::endl;
    }
    
    std::cout << "Multiplication Method Hash:" << std::endl;
    for (int key : testInts) {
        std::cout << "  hash(" << key << ") = " 
                  << HashFunctions::multiplicationHash(key, tableSize) << std::endl;
    }
    
    std::cout << "Universal Hashing:" << std::endl;
    HashFunctions::UniversalHashFunction universalHash(tableSize);
    for (int key : testInts) {
        std::cout << "  hash(" << key << ") = " 
                  << universalHash.hash(key) << std::endl;
    }
    
    // Test string hash functions
    std::vector<std::string> testStrings = {"hello", "world", "hash", "table", "collision"};
    
    std::cout << "DJB2 String Hash:" << std::endl;
    for (const auto& key : testStrings) {
        std::cout << "  hash(\"" << key << "\") = " 
                  << HashFunctions::djb2Hash(key, tableSize) << std::endl;
    }
    
    std::cout << "FNV-1a String Hash:" << std::endl;
    for (const auto& key : testStrings) {
        std::cout << "  hash(\"" << key << "\") = " 
                  << HashFunctions::fnv1aHash(key, tableSize) << std::endl;
    }
    
    // ===== PERFORMANCE TEST =====
    // Comment out if running takes too long
    // performanceTest();
    
    std::cout << "\n===== END OF DEMONSTRATION =====" << std::endl;
    
    return 0;
}
