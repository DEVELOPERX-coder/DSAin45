// Day 12: Hash Tables - Part 2 #DSAin45
// Advanced hash table implementations and applications

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <random>
#include <functional>
#include <chrono>
#include <cmath>
#include <memory>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <bitset>
#include <atomic>
#include <mutex>
#include <thread>
#include <iomanip>

// ===== ADVANCED HASH TABLE IMPLEMENTATIONS =====

// 1. Cuckoo Hash Table
template <typename K, typename V>
class CuckooHashTable
{
private:
    static const size_t NUM_TABLES = 2;
    static const size_t MAX_LOOP = 100; // Prevent infinite loops

    struct Entry
    {
        K key;
        V value;
        bool occupied;

        Entry() : occupied(false) {}
    };

    std::vector<Entry> tables[NUM_TABLES];
    std::hash<K> hashers[NUM_TABLES];
    size_t size_;

    // Hash function with seed for different tables
    size_t hash(size_t tableIdx, const K &key) const
    {
        return (hashers[tableIdx](key) ^ (tableIdx * 0x5bd1e995)) % tables[tableIdx].size();
    }

    // Insert with potential evictions
    bool insertWithEviction(const K &key, const V &value, size_t loopCount = 0)
    {
        if (loopCount >= MAX_LOOP)
        {
            // Too many evictions, resize
            rehash();
            return insert(key, value);
        }

        K currentKey = key;
        V currentValue = value;
        size_t tableIdx = 0;

        // Try inserting/evicting up to MAX_LOOP times
        for (size_t count = 0; count < MAX_LOOP; count++)
        {
            // Try each table
            for (size_t t = 0; t < NUM_TABLES; t++)
            {
                tableIdx = (tableIdx + t) % NUM_TABLES;
                size_t pos = hash(tableIdx, currentKey);

                // If empty slot, insert here
                if (!tables[tableIdx][pos].occupied)
                {
                    tables[tableIdx][pos].key = currentKey;
                    tables[tableIdx][pos].value = currentValue;
                    tables[tableIdx][pos].occupied = true;
                    return true;
                }

                // If this key already exists, update value
                if (tables[tableIdx][pos].key == currentKey)
                {
                    tables[tableIdx][pos].value = currentValue;
                    return true;
                }
            }

            // No empty slot, evict from a random table
            tableIdx = rand() % NUM_TABLES;
            size_t pos = hash(tableIdx, currentKey);

            // Evict existing entry
            std::swap(currentKey, tables[tableIdx][pos].key);
            std::swap(currentValue, tables[tableIdx][pos].value);
            // No need to swap occupied as it's already true
        }

        // If we get here, we've hit the eviction limit
        rehash();
        return insert(key, value);
    }

    // Resize and rehash
    void rehash()
    {
        std::vector<std::pair<K, V>> allEntries;

        // Save all entries
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            for (const auto &entry : tables[t])
            {
                if (entry.occupied)
                {
                    allEntries.push_back({entry.key, entry.value});
                }
            }
        }

        // Resize tables
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            tables[t].clear();
            tables[t].resize(allEntries.size() * 4); // Increase size
        }

        size_ = 0;

        // Reinsert all entries
        for (const auto &[k, v] : allEntries)
        {
            insert(k, v);
        }
    }

public:
    CuckooHashTable(size_t initialSize = 16) : size_(0)
    {
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            tables[t].resize(initialSize);
        }

        // Initialize hash functions
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            hashers[t] = std::hash<K>{};
        }
    }

    // Insert a key-value pair
    bool insert(const K &key, const V &value)
    {
        // Check if key already exists
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            size_t pos = hash(t, key);
            if (tables[t][pos].occupied && tables[t][pos].key == key)
            {
                tables[t][pos].value = value;
                return true;
            }
        }

        // If load factor is too high, rehash
        if (size_ >= tables[0].size() * 0.45)
        {
            rehash();
        }

        // Insert with potential evictions
        if (insertWithEviction(key, value))
        {
            size_++;
            return true;
        }

        return false;
    }

    // Lookup a key
    std::optional<V> lookup(const K &key) const
    {
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            size_t pos = hash(t, key);
            if (tables[t][pos].occupied && tables[t][pos].key == key)
            {
                return tables[t][pos].value;
            }
        }
        return std::nullopt;
    }

    // Remove a key
    bool remove(const K &key)
    {
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            size_t pos = hash(t, key);
            if (tables[t][pos].occupied && tables[t][pos].key == key)
            {
                tables[t][pos].occupied = false;
                size_--;
                return true;
            }
        }
        return false;
    }

    // Get current size
    size_t size() const
    {
        return size_;
    }

    // Get total capacity
    size_t capacity() const
    {
        return tables[0].size() * NUM_TABLES;
    }

    // Get load factor
    double loadFactor() const
    {
        return static_cast<double>(size_) / capacity();
    }

    // Print statistics
    void printStats() const
    {
        std::cout << "Cuckoo Hash Table Stats:" << std::endl;
        std::cout << "  Size: " << size() << std::endl;
        std::cout << "  Capacity: " << capacity() << std::endl;
        std::cout << "  Load factor: " << loadFactor() << std::endl;

        // Count empty slots
        size_t emptySlotsTotal = 0;
        for (size_t t = 0; t < NUM_TABLES; t++)
        {
            size_t emptySlots = 0;
            for (const auto &entry : tables[t])
            {
                if (!entry.occupied)
                    emptySlots++;
            }
            std::cout << "  Table " << t << " empty slots: " << emptySlots
                      << " (" << (100.0 * emptySlots / tables[t].size()) << "%)" << std::endl;
            emptySlotsTotal += emptySlots;
        }
        std::cout << "  Total empty slots: " << emptySlotsTotal
                  << " (" << (100.0 * emptySlotsTotal / capacity()) << "%)" << std::endl;
    }
};

// 2. Robin Hood Hash Table
template <typename K, typename V>
class RobinHoodHashTable
{
private:
    struct Entry
    {
        K key;
        V value;
        bool occupied;
        int probeDistance; // Distance from ideal position

        Entry() : occupied(false), probeDistance(-1) {}
    };

    std::vector<Entry> table;
    size_t size_;
    double maxLoadFactor;
    std::hash<K> hasher;

    // Hash function
    size_t hash(const K &key) const
    {
        return hasher(key) % table.size();
    }

    // Resize and rehash
    void resize(size_t newSize)
    {
        std::vector<Entry> oldTable = std::move(table);
        table.resize(newSize);
        size_ = 0;

        // Re-insert all occupied entries
        for (const auto &entry : oldTable)
        {
            if (entry.occupied)
            {
                insert(entry.key, entry.value);
            }
        }
    }

public:
    RobinHoodHashTable(size_t initialSize = 16, double loadFactor = 0.7)
        : table(initialSize), size_(0), maxLoadFactor(loadFactor) {}

    // Insert a key-value pair
    bool insert(const K &key, const V &value)
    {
        // Check if resize is needed
        if (static_cast<double>(size_) / table.size() > maxLoadFactor)
        {
            resize(table.size() * 2);
        }

        size_t idealPos = hash(key);
        int probeDistance = 0;

        // Current element to insert
        K currentKey = key;
        V currentValue = value;
        int currentProbeDistance = probeDistance;

        // Loop until we find a place for the element
        while (true)
        {
            size_t pos = (idealPos + probeDistance) % table.size();

            // Case 1: Empty slot - insert here
            if (!table[pos].occupied)
            {
                table[pos].key = currentKey;
                table[pos].value = currentValue;
                table[pos].occupied = true;
                table[pos].probeDistance = currentProbeDistance;
                size_++;
                return true;
            }

            // Case 2: Key already exists - update value
            if (table[pos].key == currentKey)
            {
                table[pos].value = currentValue;
                return true;
            }

            // Case 3: Robin Hood - steal from rich, give to poor
            if (currentProbeDistance > table[pos].probeDistance)
            {
                // Swap entries
                std::swap(currentKey, table[pos].key);
                std::swap(currentValue, table[pos].value);
                std::swap(currentProbeDistance, table[pos].probeDistance);
            }

            // Move to next probe position
            probeDistance++;
            currentProbeDistance++;

            // Prevent infinite loops
            if (probeDistance >= static_cast<int>(table.size()))
            {
                resize(table.size() * 2);
                return insert(currentKey, currentValue);
            }
        }
    }

    // Lookup a key
    std::optional<V> lookup(const K &key) const
    {
        size_t idealPos = hash(key);
        int probeDistance = 0;

        // Search for key
        while (true)
        {
            size_t pos = (idealPos + probeDistance) % table.size();

            // Empty slot or probe distance too large - key not found
            if (!table[pos].occupied || probeDistance > table[pos].probeDistance)
            {
                return std::nullopt;
            }

            // Found the key
            if (table[pos].key == key)
            {
                return table[pos].value;
            }

            // Move to next position
            probeDistance++;

            // Prevent infinite loops
            if (probeDistance >= static_cast<int>(table.size()))
            {
                return std::nullopt;
            }
        }
    }

    // Remove a key with backward shift deletion
    bool remove(const K &key)
    {
        size_t idealPos = hash(key);
        int probeDistance = 0;

        // Find the key
        while (true)
        {
            size_t pos = (idealPos + probeDistance) % table.size();

            // Empty slot or probe distance too large - key not found
            if (!table[pos].occupied || probeDistance > table[pos].probeDistance)
            {
                return false;
            }

            // Found the key
            if (table[pos].key == key)
            {
                // Mark as deleted
                table[pos].occupied = false;

                // Backward shift deletion
                size_t current = pos;
                size_t next = (current + 1) % table.size();

                // Shift entries backward to maintain robin hood property
                while (table[next].occupied && table[next].probeDistance > 0)
                {
                    table[current] = table[next];
                    table[current].probeDistance--; // One step closer to ideal position

                    current = next;
                    next = (current + 1) % table.size();
                }

                // Mark the last shifted position as unoccupied
                table[current].occupied = false;

                size_--;
                return true;
            }

            // Move to next position
            probeDistance++;

            // Prevent infinite loops
            if (probeDistance >= static_cast<int>(table.size()))
            {
                return false;
            }
        }
    }

    // Get current size
    size_t size() const
    {
        return size_;
    }

    // Get capacity
    size_t capacity() const
    {
        return table.size();
    }

    // Get load factor
    double loadFactor() const
    {
        return static_cast<double>(size_) / table.size();
    }

    // Print statistics
    void printStats() const
    {
        std::cout << "Robin Hood Hash Table Stats:" << std::endl;
        std::cout << "  Size: " << size() << std::endl;
        std::cout << "  Capacity: " << capacity() << std::endl;
        std::cout << "  Load factor: " << loadFactor() << std::endl;

        // Calculate probe distance statistics
        int maxProbeDistance = -1;
        double totalProbeDistance = 0;
        size_t emptySlots = 0;

        for (const auto &entry : table)
        {
            if (!entry.occupied)
            {
                emptySlots++;
            }
            else
            {
                totalProbeDistance += entry.probeDistance;
                maxProbeDistance = std::max(maxProbeDistance, entry.probeDistance);
            }
        }

        std::cout << "  Empty slots: " << emptySlots
                  << " (" << (100.0 * emptySlots / table.size()) << "%)" << std::endl;
        std::cout << "  Average probe distance: "
                  << (size_ > 0 ? totalProbeDistance / size_ : 0) << std::endl;
        std::cout << "  Maximum probe distance: " << maxProbeDistance << std::endl;
    }

    // Visualize the hash table
    void visualize(size_t maxRows = 20) const
    {
        std::cout << "Robin Hood Hash Table Visualization:" << std::endl;
        std::cout << "  'O' = Occupied, '.' = Empty, Number = Probe Distance" << std::endl;

        size_t rows = std::min(maxRows, table.size());
        for (size_t i = 0; i < rows; i++)
        {
            std::cout << std::setw(4) << i << " | ";

            if (table[i].occupied)
            {
                std::cout << "O:" << table[i].probeDistance;
            }
            else
            {
                std::cout << ".";
            }

            std::cout << std::endl;
        }

        if (rows < table.size())
        {
            std::cout << "  ... " << (table.size() - rows) << " more rows ..." << std::endl;
        }
    }
};

// ===== PROBABILISTIC DATA STRUCTURES =====

// 1. Counting Bloom Filter
class CountingBloomFilter
{
private:
    std::vector<uint8_t> counters; // Use counters instead of bits
    size_t numHashes;
    std::vector<std::function<size_t(const std::string &)>> hashFuncs;

    // Initialize hash functions
    void initHashFunctions()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dist;

        for (size_t i = 0; i < numHashes; i++)
        {
            // Generate random seeds for hash functions
            uint32_t seed = dist(gen);

            hashFuncs.push_back([seed](const std::string &key)
                                {
                std::hash<std::string> hasher;
                return hasher(key + std::to_string(seed)); });
        }
    }

public:
    CountingBloomFilter(size_t size, size_t numHashes)
        : counters(size, 0), numHashes(numHashes)
    {
        initHashFunctions();
    }

    // Add an element to the filter
    void insert(const std::string &key)
    {
        for (const auto &hashFunc : hashFuncs)
        {
            size_t index = hashFunc(key) % counters.size();

            // Increment counter, saturate at 255
            if (counters[index] < 255)
            {
                counters[index]++;
            }
        }
    }

    // Remove an element from the filter
    void remove(const std::string &key)
    {
        // Check if the key might be in the filter first
        if (!mightContain(key))
        {
            return;
        }

        for (const auto &hashFunc : hashFuncs)
        {
            size_t index = hashFunc(key) % counters.size();

            // Decrement counter if not zero
            if (counters[index] > 0)
            {
                counters[index]--;
            }
        }
    }

    // Check if an element might be in the filter
    bool mightContain(const std::string &key) const
    {
        for (const auto &hashFunc : hashFuncs)
        {
            size_t index = hashFunc(key) % counters.size();
            if (counters[index] == 0)
            {
                return false; // Definitely not in the set
            }
        }
        return true; // Might be in the set
    }

    // Estimate the number of elements in the filter
    double estimateCount() const
    {
        // Find average counter value
        double sum = 0;
        size_t nonZeroCounters = 0;

        for (uint8_t count : counters)
        {
            if (count > 0)
            {
                sum += count;
                nonZeroCounters++;
            }
        }

        if (nonZeroCounters == 0)
            return 0;

        double avgCount = sum / nonZeroCounters;

        // Estimate using formula
        return -static_cast<double>(counters.size()) *
               std::log(1.0 - nonZeroCounters / static_cast<double>(counters.size())) /
               numHashes;
    }

    // Get the false positive probability
    double getFalsePositiveRate() const
    {
        // Count non-zero counters
        size_t nonZeroCounters = 0;
        for (uint8_t count : counters)
        {
            if (count > 0)
                nonZeroCounters++;
        }

        // Calculate probability of a random bit being set
        double p = static_cast<double>(nonZeroCounters) / counters.size();

        // False positive probability: (1 - (1 - p)^k)^k
        return std::pow(1.0 - std::pow(1.0 - p, numHashes), numHashes);
    }

    // Print statistics
    void printStats() const
    {
        std::cout << "Counting Bloom Filter Stats:" << std::endl;
        std::cout << "  Size: " << counters.size() << " counters" << std::endl;
        std::cout << "  Number of hash functions: " << numHashes << std::endl;

        // Count non-zero counters and calculate statistics
        size_t nonZeroCounters = 0;
        uint8_t maxCounter = 0;
        double sum = 0;

        for (uint8_t count : counters)
        {
            if (count > 0)
            {
                nonZeroCounters++;
                sum += count;
                maxCounter = std::max(maxCounter, count);
            }
        }

        std::cout << "  Non-zero counters: " << nonZeroCounters
                  << " (" << (100.0 * nonZeroCounters / counters.size()) << "%)" << std::endl;
        std::cout << "  Average counter value: "
                  << (nonZeroCounters > 0 ? sum / nonZeroCounters : 0) << std::endl;
        std::cout << "  Maximum counter value: " << static_cast<int>(maxCounter) << std::endl;
        std::cout << "  Estimated element count: " << estimateCount() << std::endl;
        std::cout << "  Estimated false positive rate: "
                  << (getFalsePositiveRate() * 100) << "%" << std::endl;
    }
};

// 2. Count-Min Sketch
class CountMinSketch
{
private:
    std::vector<std::vector<int>> counters;
    std::vector<std::function<size_t(const std::string &)>> hashFuncs;

    // Initialize hash functions
    void initHashFunctions()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dist;

        for (size_t i = 0; i < counters.size(); i++)
        {
            // Generate random seeds for hash functions
            uint32_t seed = dist(gen);

            hashFuncs.push_back([seed](const std::string &key)
                                {
                std::hash<std::string> hasher;
                return hasher(key + std::to_string(seed)); });
        }
    }

public:
    // Constructor with epsilon and delta parameters
    CountMinSketch(double epsilon, double delta)
    {
        // Calculate dimensions based on error bounds
        size_t width = std::ceil(2.0 / epsilon);         // Width (columns)
        size_t depth = std::ceil(std::log(1.0 / delta)); // Depth (rows)

        counters.resize(depth, std::vector<int>(width, 0));
        initHashFunctions();
    }

    // Constructor with explicit dimensions
    CountMinSketch(size_t depth, size_t width)
    {
        counters.resize(depth, std::vector<int>(width, 0));
        initHashFunctions();
    }

    // Add an item with count
    void add(const std::string &item, int count = 1)
    {
        for (size_t i = 0; i < counters.size(); i++)
        {
            size_t index = hashFuncs[i](item) % counters[i].size();
            counters[i][index] += count;
        }
    }

    // Estimate the count of an item
    int estimate(const std::string &item) const
    {
        int minValue = std::numeric_limits<int>::max();

        for (size_t i = 0; i < counters.size(); i++)
        {
            size_t index = hashFuncs[i](item) % counters[i].size();
            minValue = std::min(minValue, counters[i][index]);
        }

        return minValue;
    }

    // Print statistics
    void printStats() const
    {
        std::cout << "Count-Min Sketch Stats:" << std::endl;
        std::cout << "  Depth: " << counters.size() << " rows" << std::endl;
        std::cout << "  Width: " << (counters.empty() ? 0 : counters[0].size()) << " columns" << std::endl;

        // Calculate statistics
        int maxCount = 0;
        double totalCount = 0;
        size_t nonZeroCounters = 0;

        for (const auto &row : counters)
        {
            for (int count : row)
            {
                if (count > 0)
                {
                    nonZeroCounters++;
                    totalCount += count;
                    maxCount = std::max(maxCount, count);
                }
            }
        }

        size_t totalCounters = counters.size() * (counters.empty() ? 0 : counters[0].size());

        std::cout << "  Non-zero counters: " << nonZeroCounters
                  << " (" << (100.0 * nonZeroCounters / totalCounters) << "%)" << std::endl;
        std::cout << "  Average counter value: "
                  << (nonZeroCounters > 0 ? totalCount / nonZeroCounters : 0) << std::endl;
        std::cout << "  Maximum counter value: " << maxCount << std::endl;
    }
};

// 3. HyperLogLog for cardinality estimation
class HyperLogLog
{
private:
    std::vector<uint8_t> registers;
    size_t p;       // Number of bits used for bucket index
    double alphaMM; // Bias correction factor * m^2

    // Hash an item and find leading zeros
    std::pair<size_t, int> hashAndCount(const std::string &item) const
    {
        std::hash<std::string> hasher;
        size_t hash = hasher(item);

        // Use p bits for bucket index
        size_t bucket = hash & ((1 << p) - 1);

        // Count leading zeros in the remaining bits
        size_t value = hash >> p;
        int zeros = __builtin_clzll(value) + 1;

        return {bucket, zeros};
    }

public:
    HyperLogLog(size_t precision = 14) : p(precision)
    {
        // Number of registers
        size_t m = 1 << p;
        registers.resize(m, 0);

        // Compute alpha * m * m constant
        double alpha;
        if (m == 16)
            alpha = 0.673;
        else if (m == 32)
            alpha = 0.697;
        else if (m == 64)
            alpha = 0.709;
        else
            alpha = 0.7213 / (1.0 + 1.079 / m);

        alphaMM = alpha * m * m;
    }

    // Add an item
    void add(const std::string &item)
    {
        auto [bucket, zeros] = hashAndCount(item);

        // Update the register if the new value is larger
        registers[bucket] = std::max(registers[bucket], static_cast<uint8_t>(zeros));
    }

    // Estimate cardinality
    double estimate() const
    {
        double sum = 0.0;

        // Sum of 2^(-register)
        for (uint8_t r : registers)
        {
            sum += std::pow(2.0, -r);
        }

        // Calculate the harmonic mean
        double estimate = alphaMM / sum;

        // Apply small and large range corrections
        size_t m = registers.size();
        if (estimate <= 2.5 * m)
        {
            // Small range correction
            // Count number of registers equal to 0
            size_t zeros = 0;
            for (uint8_t r : registers)
            {
                if (r == 0)
                    zeros++;
            }

            if (zeros > 0)
            {
                // Linear counting when in small range
                estimate = m * std::log(static_cast<double>(m) / zeros);
            }
        }
        else if (estimate > std::pow(2, 32) / 30.0)
        {
            // Large range correction
            estimate = -std::pow(2, 32) * std::log(1.0 - estimate / std::pow(2, 32));
        }

        return estimate;
    }

    // Merge with another HyperLogLog
    void merge(const HyperLogLog &other)
    {
        if (p != other.p)
        {
            throw std::invalid_argument("Cannot merge HyperLogLog with different precision");
        }

        for (size_t i = 0; i < registers.size(); i++)
        {
            registers[i] = std::max(registers[i], other.registers[i]);
        }
    }

    // Print statistics
    void printStats() const
    {
        std::cout << "HyperLogLog Stats:" << std::endl;
        std::cout << "  Precision: " << p << " bits" << std::endl;
        std::cout << "  Number of registers: " << registers.size() << std::endl;

        // Calculate register statistics
        size_t zeroRegisters = 0;
        uint8_t maxRegister = 0;
        double sum = 0;

        for (uint8_t r : registers)
        {
            if (r == 0)
                zeroRegisters++;
            maxRegister = std::max(maxRegister, r);
            sum += r;
        }

        std::cout << "  Zero registers: " << zeroRegisters
                  << " (" << (100.0 * zeroRegisters / registers.size()) << "%)" << std::endl;
        std::cout << "  Average register value: "
                  << (sum / registers.size()) << std::endl;
        std::cout << "  Maximum register value: " << static_cast<int>(maxRegister) << std::endl;
        std::cout << "  Estimated cardinality: " << std::fixed << std::setprecision(2)
                  << estimate() << std::endl;
    }
};

// ===== ADVANCED HASH TABLE APPLICATIONS =====

// 1. LRU Cache (Least Recently Used)
template <typename K, typename V>
class LRUCache
{
private:
    // Maximum capacity of the cache
    size_t capacity;

    // List of key-value pairs, most recently used at the front
    std::list<std::pair<K, V>> items;

    // Map from key to iterator in the list
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache;

public:
    LRUCache(size_t capacity) : capacity(capacity) {}

    // Get value associated with key and move to front (most recently used)
    std::optional<V> get(const K &key)
    {
        auto it = cache.find(key);
        if (it == cache.end())
        {
            return std::nullopt; // Key not found
        }

        // Move to front of list (mark as most recently used)
        items.splice(items.begin(), items, it->second);
        return it->second->second; // Return value
    }

    // Put key-value pair, evicting least recently used if at capacity
    void put(const K &key, const V &value)
    {
        // Check if key already exists
        auto it = cache.find(key);
        if (it != cache.end())
        {
            // Update existing key
            it->second->second = value;
            // Move to front (most recently used)
            items.splice(items.begin(), items, it->second);
            return;
        }

        // Check if cache is full
        if (items.size() == capacity)
        {
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
    size_t size() const
    {
        return items.size();
    }

    // Check if cache is empty
    bool empty() const
    {
        return items.empty();
    }

    // Clear the cache
    void clear()
    {
        items.clear();
        cache.clear();
    }

    // Get capacity
    size_t getCapacity() const
    {
        return capacity;
    }

    // Print current contents
    void printContents() const
    {
        std::cout << "LRU Cache Contents (most recent first):" << std::endl;
        for (const auto &[key, value] : items)
        {
            std::cout << "  " << key << " -> " << value << std::endl;
        }
    }
};

// 2. LFU Cache (Least Frequently Used)
template <typename K, typename V>
class LFUCache
{
private:
    // Maximum capacity of the cache
    size_t capacity;

    // Track minimum frequency
    int minFreq;

    // Map: key -> (value, freq, iterator to key list)
    std::unordered_map<K, std::tuple<V, int, typename std::list<K>::iterator>> keyInfo;

    // Map: freq -> list of keys with that frequency
    std::unordered_map<int, std::list<K>> freqKeys;

public:
    LFUCache(size_t capacity) : capacity(capacity), minFreq(0) {}

    // Get value associated with key and update frequency
    std::optional<V> get(const K &key)
    {
        auto it = keyInfo.find(key);
        if (it == keyInfo.end())
        {
            return std::nullopt; // Key not found
        }

        // Extract value, frequency, and iterator
        auto &[value, freq, iter] = it->second;

        // Remove from current frequency list
        freqKeys[freq].erase(iter);

        // Update minimum frequency if needed
        if (freq == minFreq && freqKeys[freq].empty())
        {
            minFreq++;
        }

        // Increase frequency
        freq++;

        // Add to new frequency list
        freqKeys[freq].push_front(key);

        // Update iterator in keyInfo
        it->second = std::make_tuple(value, freq, freqKeys[freq].begin());

        return value;
    }

    // Put key-value pair, evicting least frequently used if at capacity
    void put(const K &key, const V &value)
    {
        if (capacity == 0)
            return;

        // Check if key already exists
        auto it = keyInfo.find(key);
        if (it != keyInfo.end())
        {
            // Update existing key
            auto &[_, freq, iter] = it->second;

            // Remove from current frequency list
            freqKeys[freq].erase(iter);

            // Update minimum frequency if needed
            if (freq == minFreq && freqKeys[freq].empty())
            {
                minFreq++;
            }

            // Increase frequency
            freq++;

            // Add to new frequency list
            freqKeys[freq].push_front(key);

            // Update keyInfo
            it->second = std::make_tuple(value, freq, freqKeys[freq].begin());
            return;
        }

        // If at capacity, remove least frequently used
        if (keyInfo.size() >= capacity)
        {
            // Get the key to remove (last key in minFreq list)
            K lfu_key = freqKeys[minFreq].back();

            // Remove from structures
            freqKeys[minFreq].pop_back();
            keyInfo.erase(lfu_key);
        }

        // Insert new key with frequency 1
        minFreq = 1;
        freqKeys[1].push_front(key);
        keyInfo[key] = std::make_tuple(value, 1, freqKeys[1].begin());
    }

    // Get current size
    size_t size() const
    {
        return keyInfo.size();
    }

    // Check if cache is empty
    bool empty() const
    {
        return keyInfo.empty();
    }

    // Clear the cache
    void clear()
    {
        keyInfo.clear();
        freqKeys.clear();
        minFreq = 0;
    }

    // Get capacity
    size_t getCapacity() const
    {
        return capacity;
    }

    // Print current contents
    void printContents() const
    {
        std::cout << "LFU Cache Contents:" << std::endl;

        // Print all frequencies
        for (const auto &[freq, keys] : freqKeys)
        {
            if (!keys.empty())
            {
                std::cout << "  Frequency " << freq << ": ";
                for (const K &key : keys)
                {
                    auto &[value, _, __] = keyInfo.at(key);
                    std::cout << key << "(" << value << ") ";
                }
                std::cout << std::endl;
            }
        }
    }
};

// 3. Thread-Safe Hash Map
template <typename K, typename V>
class ConcurrentHashMap
{
private:
    static const size_t NUM_SHARDS = 16; // Number of separate hash tables (shards)

    struct Shard
    {
        std::unordered_map<K, V> map;
        mutable std::mutex mutex;
    };

    std::vector<Shard> shards;
    std::hash<K> hasher;

    // Determine shard for a key
    size_t shardIndex(const K &key) const
    {
        return hasher(key) % NUM_SHARDS;
    }

public:
    ConcurrentHashMap() : shards(NUM_SHARDS) {}

    // Insert a key-value pair
    void insert(const K &key, const V &value)
    {
        size_t idx = shardIndex(key);
        std::lock_guard<std::mutex> lock(shards[idx].mutex);
        shards[idx].map[key] = value;
    }

    // Get a value
    std::optional<V> get(const K &key) const
    {
        size_t idx = shardIndex(key);
        std::lock_guard<std::mutex> lock(shards[idx].mutex);

        auto it = shards[idx].map.find(key);
        if (it != shards[idx].map.end())
        {
            return it->second;
        }

        return std::nullopt;
    }

    // Remove a key
    bool erase(const K &key)
    {
        size_t idx = shardIndex(key);
        std::lock_guard<std::mutex> lock(shards[idx].mutex);

        return shards[idx].map.erase(key) > 0;
    }

    // Get all key-value pairs
    std::vector<std::pair<K, V>> getAll() const
    {
        std::vector<std::pair<K, V>> result;

        for (size_t i = 0; i < NUM_SHARDS; i++)
        {
            std::lock_guard<std::mutex> lock(shards[i].mutex);

            for (const auto &[key, value] : shards[i].map)
            {
                result.push_back({key, value});
            }
        }

        return result;
    }

    // Get total size
    size_t size() const
    {
        size_t total = 0;

        for (size_t i = 0; i < NUM_SHARDS; i++)
        {
            std::lock_guard<std::mutex> lock(shards[i].mutex);
            total += shards[i].map.size();
        }

        return total;
    }

    // Print statistics
    void printStats() const
    {
        std::cout << "Concurrent Hash Map Stats:" << std::endl;
        std::cout << "  Number of shards: " << NUM_SHARDS << std::endl;
        std::cout << "  Total size: " << size() << std::endl;

        // Print per-shard statistics
        for (size_t i = 0; i < NUM_SHARDS; i++)
        {
            std::lock_guard<std::mutex> lock(shards[i].mutex);
            std::cout << "  Shard " << i << " size: " << shards[i].map.size() << std::endl;
        }
    }
};

// 4. Consistent Hashing for distributed systems
class ConsistentHash
{
private:
    // Number of virtual nodes per physical node
    int virtualNodes;

    // Sorted hash ring (hash -> node name)
    std::map<size_t, std::string> ring;

    // Hash function
    std::hash<std::string> hasher;

    // Hash a node + virtual node ID
    size_t hashVirtualNode(const std::string &node, int id) const
    {
        return hasher(node + "#" + std::to_string(id));
    }

public:
    ConsistentHash(int vnodes = 100) : virtualNodes(vnodes) {}

    // Add a node to the hash ring
    void addNode(const std::string &node)
    {
        for (int i = 0; i < virtualNodes; i++)
        {
            size_t hash = hashVirtualNode(node, i);
            ring[hash] = node;
        }
    }

    // Remove a node from the hash ring
    void removeNode(const std::string &node)
    {
        for (int i = 0; i < virtualNodes; i++)
        {
            size_t hash = hashVirtualNode(node, i);
            ring.erase(hash);
        }
    }

    // Get the node responsible for a key
    std::string getNode(const std::string &key) const
    {
        if (ring.empty())
        {
            return "";
        }

        size_t hash = hasher(key);

        // Find the first node with hash >= key hash
        auto it = ring.lower_bound(hash);

        // If no such node, wrap around to first node
        if (it == ring.end())
        {
            it = ring.begin();
        }

        return it->second;
    }

    // Get all node names
    std::unordered_set<std::string> getAllNodes() const
    {
        std::unordered_set<std::string> nodes;
        for (const auto &[hash, node] : ring)
        {
            nodes.insert(node);
        }
        return nodes;
    }

    // Print the distribution of keys among nodes
    void printDistribution(const std::vector<std::string> &keys) const
    {
        // Count keys per node
        std::unordered_map<std::string, int> distribution;

        for (const auto &key : keys)
        {
            std::string node = getNode(key);
            distribution[node]++;
        }

        // Print distribution
        std::cout << "Key Distribution:" << std::endl;
        for (const auto &[node, count] : distribution)
        {
            std::cout << "  " << node << ": " << count << " keys ("
                      << (100.0 * count / keys.size()) << "%)" << std::endl;
        }

        // Calculate standard deviation
        if (!distribution.empty())
        {
            double mean = static_cast<double>(keys.size()) / distribution.size();
            double sumSquaredDiff = 0;

            for (const auto &[node, count] : distribution)
            {
                double diff = count - mean;
                sumSquaredDiff += diff * diff;
            }

            double stdDev = std::sqrt(sumSquaredDiff / distribution.size());
            std::cout << "  Standard deviation: " << stdDev << " keys" << std::endl;
        }
    }

    // Print hash ring statistics
    void printStats() const
    {
        std::cout << "Consistent Hash Ring Stats:" << std::endl;
        std::cout << "  Virtual nodes per physical node: " << virtualNodes << std::endl;

        std::unordered_set<std::string> nodes = getAllNodes();
        std::cout << "  Physical nodes: " << nodes.size() << std::endl;
        std::cout << "  Total virtual nodes: " << ring.size() << std::endl;

        // Print node names
        std::cout << "  Nodes: ";
        for (const auto &node : nodes)
        {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }
};

// ===== PERFORMANCE TESTS =====

// Performance test comparing hash table implementations
void performanceTest(int numOperations = 100000)
{
    std::cout << "\n===== HASH TABLE PERFORMANCE TEST =====" << std::endl;
    std::cout << "Testing with " << numOperations << " operations" << std::endl;

    // Generate random keys and values
    std::vector<std::string> keys;
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<> dist(1, 1000000);

    for (int i = 0; i < numOperations; i++)
    {
        keys.push_back("key" + std::to_string(dist(gen)));
    }

    // Lambda to test a hash table implementation
    auto testHashTable = [&](const std::string &name, auto &table)
    {
        // Insertion test
        auto startInsert = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numOperations; i++)
        {
            table.insert(keys[i], i);
        }

        auto endInsert = std::chrono::high_resolution_clock::now();
        auto insertTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                              endInsert - startInsert)
                              .count();

        // Lookup test
        auto startLookup = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numOperations; i++)
        {
            auto result = table.lookup(keys[i]);
        }

        auto endLookup = std::chrono::high_resolution_clock::now();
        auto lookupTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                              endLookup - startLookup)
                              .count();

        // Print results
        std::cout << name << ":" << std::endl;
        std::cout << "  Insert: " << insertTime << " ms" << std::endl;
        std::cout << "  Lookup: " << lookupTime << " ms" << std::endl;
        std::cout << "  Total: " << (insertTime + lookupTime) << " ms" << std::endl;

        // Print stats if available
        if constexpr (requires { table.printStats(); })
        {
            table.printStats();
        }
    };

    // Test different hash table implementations
    std::cout << "\nTesting Cuckoo Hash Table:" << std::endl;
    CuckooHashTable<std::string, int> cuckooTable;
    testHashTable("Cuckoo Hash Table", cuckooTable);

    std::cout << "\nTesting Robin Hood Hash Table:" << std::endl;
    RobinHoodHashTable<std::string, int> robinHoodTable;
    testHashTable("Robin Hood Hash Table", robinHoodTable);

    // Reference: std::unordered_map
    std::cout << "\nTesting std::unordered_map:" << std::endl;
    std::unordered_map<std::string, int> stdMap;

    auto startInsert = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; i++)
    {
        stdMap[keys[i]] = i;
    }
    auto endInsert = std::chrono::high_resolution_clock::now();
    auto insertTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                          endInsert - startInsert)
                          .count();

    auto startLookup = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; i++)
    {
        auto result = stdMap.find(keys[i]);
    }
    auto endLookup = std::chrono::high_resolution_clock::now();
    auto lookupTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                          endLookup - startLookup)
                          .count();

    std::cout << "std::unordered_map:" << std::endl;
    std::cout << "  Insert: " << insertTime << " ms" << std::endl;
    std::cout << "  Lookup: " << lookupTime << " ms" << std::endl;
    std::cout << "  Total: " << (insertTime + lookupTime) << " ms" << std::endl;
    std::cout << "  Load factor: " << stdMap.load_factor() << std::endl;
    std::cout << "  Bucket count: " << stdMap.bucket_count() << std::endl;
}

// ===== REAL-WORLD APPLICATION DEMOS =====

// 1. Web Cache Demo
void webCacheDemo()
{
    std::cout << "\n===== WEB CACHE DEMO =====" << std::endl;

    // Create a LRU cache for web pages
    LRUCache<std::string, std::string> webCache(3);

    // Simulate page requests
    std::cout << "Adding pages to cache (capacity 3)..." << std::endl;
    webCache.put("https://example.com", "Example Domain");
    webCache.put("https://example.org", "Example Organization");
    webCache.put("https://example.net", "Example Network");

    webCache.printContents();

    // Request a cached page
    std::cout << "\nRequesting 'https://example.com'..." << std::endl;
    auto content = webCache.get("https://example.com");
    if (content)
    {
        std::cout << "Cache hit! Content: " << *content << std::endl;
    }
    else
    {
        std::cout << "Cache miss!" << std::endl;
    }

    webCache.printContents();

    // Add another page (should evict least recently used)
    std::cout << "\nAdding new page 'https://example.edu'..." << std::endl;
    webCache.put("https://example.edu", "Example Education");

    webCache.printContents();

    // Try to access evicted page
    std::cout << "\nRequesting 'https://example.org'..." << std::endl;
    content = webCache.get("https://example.org");
    if (content)
    {
        std::cout << "Cache hit! Content: " << *content << std::endl;
    }
    else
    {
        std::cout << "Cache miss! (Evicted due to LRU policy)" << std::endl;
    }
}

// 2. Frequency Counter Demo (Count-Min Sketch)
void frequencyCounterDemo()
{
    std::cout << "\n===== FREQUENCY COUNTER DEMO =====" << std::endl;

    // Create a Count-Min Sketch with 4 rows and 100 columns
    CountMinSketch sketch(4, 100);

    // Sample data: counting word frequencies in text
    std::vector<std::string> words = {
        "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog",
        "the", "dog", "barks", "at", "the", "fox", "but", "the", "fox", "ignores",
        "the", "dog", "and", "continues", "to", "jump", "over", "the", "lazy", "dog"};

    std::cout << "Adding words to Count-Min Sketch..." << std::endl;
    for (const auto &word : words)
    {
        sketch.add(word);
    }

    // Compare with actual counts
    std::unordered_map<std::string, int> actualCounts;
    for (const auto &word : words)
    {
        actualCounts[word]++;
    }

    std::cout << "Word frequency comparison:" << std::endl;
    std::cout << std::left << std::setw(12) << "Word"
              << std::setw(15) << "Actual Count"
              << std::setw(15) << "Estimated Count"
              << std::setw(15) << "Error" << std::endl;

    for (const auto &[word, count] : actualCounts)
    {
        int estimated = sketch.estimate(word);
        double error = 100.0 * (estimated - count) / count;

        std::cout << std::left << std::setw(12) << word
                  << std::setw(15) << count
                  << std::setw(15) << estimated
                  << std::setw(15) << std::fixed << std::setprecision(2) << error << "%" << std::endl;
    }

    // Try a word not in the text
    std::string missingWord = "cat";
    std::cout << "\nEstimated count for '" << missingWord << "': "
              << sketch.estimate(missingWord) << " (actual: 0)" << std::endl;

    sketch.printStats();
}

// 3. Distributed Hash Table Demo
void distributedHashTableDemo()
{
    std::cout << "\n===== DISTRIBUTED HASH TABLE DEMO =====" << std::endl;

    // Create a consistent hash ring
    ConsistentHash ring(100);

    // Add nodes to the ring
    std::vector<std::string> nodes = {"node1", "node2", "node3", "node4"};
    for (const auto &node : nodes)
    {
        ring.addNode(node);
        std::cout << "Added node: " << node << std::endl;
    }

    ring.printStats();

    // Generate some keys
    std::vector<std::string> keys;
    for (int i = 0; i < 1000; i++)
    {
        keys.push_back("key" + std::to_string(i));
    }

    // Show initial distribution
    std::cout << "\nInitial key distribution:" << std::endl;
    ring.printDistribution(keys);

    // Remove a node
    std::string nodeToRemove = "node2";
    std::cout << "\nRemoving node: " << nodeToRemove << std::endl;
    ring.removeNode(nodeToRemove);

    // Show distribution after removing a node
    std::cout << "\nKey distribution after removing " << nodeToRemove << ":" << std::endl;
    ring.printDistribution(keys);

    // Add a new node
    std::string newNode = "node5";
    std::cout << "\nAdding new node: " << newNode << std::endl;
    ring.addNode(newNode);

    // Show distribution after adding a node
    std::cout << "\nKey distribution after adding " << newNode << ":" << std::endl;
    ring.printDistribution(keys);
}

// 4. Cardinality Estimation Demo
void cardinalityEstimationDemo()
{
    std::cout << "\n===== CARDINALITY ESTIMATION DEMO =====" << std::endl;

    // Create a HyperLogLog with precision 12 (4096 registers)
    HyperLogLog hll(12);

    // Generate random elements
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000000);

    std::unordered_set<int> uniqueElements; // For ground truth

    for (int i = 0; i < 10000; i++)
    {
        int value = dist(gen);
        uniqueElements.insert(value);
        hll.add("element" + std::to_string(value));

        // Print progress at certain points
        if ((i + 1) % 1000 == 0)
        {
            double estimate = hll.estimate();
            double actual = uniqueElements.size();
            double error = 100.0 * std::abs(estimate - actual) / actual;

            std::cout << "After " << (i + 1) << " insertions:" << std::endl;
            std::cout << "  Actual unique count: " << actual << std::endl;
            std::cout << "  Estimated unique count: " << std::fixed << std::setprecision(2)
                      << estimate << std::endl;
            std::cout << "  Error: " << std::fixed << std::setprecision(2) << error << "%" << std::endl;
        }
    }

    hll.printStats();
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "===== DAY 12: HASH TABLES - PART 2 DEMONSTRATION =====" << std::endl;

    // Seed random generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Demo Cuckoo Hash Table
    std::cout << "\n===== CUCKOO HASH TABLE DEMO =====" << std::endl;
    CuckooHashTable<std::string, int> cuckooTable;

    // Insert some key-value pairs
    cuckooTable.insert("apple", 10);
    cuckooTable.insert("banana", 20);
    cuckooTable.insert("cherry", 30);
    cuckooTable.insert("date", 40);
    cuckooTable.insert("elderberry", 50);

    cuckooTable.printStats();

    // Lookup values
    std::cout << "\nLooking up values:" << std::endl;
    auto value = cuckooTable.lookup("apple");
    std::cout << "apple: " << (value ? std::to_string(*value) : "not found") << std::endl;

    value = cuckooTable.lookup("grape");
    std::cout << "grape: " << (value ? std::to_string(*value) : "not found") << std::endl;

    // Remove a key
    std::cout << "\nRemoving 'banana'" << std::endl;
    cuckooTable.remove("banana");

    value = cuckooTable.lookup("banana");
    std::cout << "banana after removal: " << (value ? std::to_string(*value) : "not found") << std::endl;

    cuckooTable.printStats();

    // Demo Robin Hood Hash Table
    std::cout << "\n===== ROBIN HOOD HASH TABLE DEMO =====" << std::endl;
    RobinHoodHashTable<std::string, int> robinHoodTable;

    // Insert some key-value pairs
    robinHoodTable.insert("apple", 10);
    robinHoodTable.insert("banana", 20);
    robinHoodTable.insert("cherry", 30);
    robinHoodTable.insert("date", 40);
    robinHoodTable.insert("elderberry", 50);

    robinHoodTable.printStats();
    robinHoodTable.visualize();

    // Lookup values
    std::cout << "\nLooking up values:" << std::endl;
    value = robinHoodTable.lookup("cherry");
    std::cout << "cherry: " << (value ? std::to_string(*value) : "not found") << std::endl;

    value = robinHoodTable.lookup("grape");
    std::cout << "grape: " << (value ? std::to_string(*value) : "not found") << std::endl;

    // Remove a key with backward shift deletion
    std::cout << "\nRemoving 'date' with backward shift deletion" << std::endl;
    robinHoodTable.remove("date");

    value = robinHoodTable.lookup("date");
    std::cout << "date after removal: " << (value ? std::to_string(*value) : "not found") << std::endl;

    robinHoodTable.printStats();
    robinHoodTable.visualize();

    // Demo Counting Bloom Filter
    std::cout << "\n===== COUNTING BLOOM FILTER DEMO =====" << std::endl;
    CountingBloomFilter cbf(100, 3);

    // Add some elements
    std::vector<std::string> elementsToAdd = {"apple", "banana", "cherry", "date", "elderberry"};
    for (const auto &elem : elementsToAdd)
    {
        cbf.insert(elem);
        std::cout << "Added: " << elem << std::endl;
    }

    // Check membership
    std::vector<std::string> elementsToCheck = {"apple", "banana", "grape", "kiwi", "elderberry"};
    for (const auto &elem : elementsToCheck)
    {
        bool present = cbf.mightContain(elem);
        std::cout << "'" << elem << "' might be in set? " << (present ? "Yes" : "No");

        // Check if it's actually in the set
        bool actuallyPresent = std::find(elementsToAdd.begin(), elementsToAdd.end(), elem) != elementsToAdd.end();
        if (present && !actuallyPresent)
        {
            std::cout << " (false positive)";
        }
        std::cout << std::endl;
    }

    // Remove an element
    std::string elemToRemove = "banana";
    std::cout << "\nRemoving: " << elemToRemove << std::endl;
    cbf.remove(elemToRemove);

    // Check if it's still present
    std::cout << "'" << elemToRemove << "' might be in set after removal? "
              << (cbf.mightContain(elemToRemove) ? "Yes" : "No") << std::endl;

    cbf.printStats();

    // Demo LRU and LFU Caches
    std::cout << "\n===== LRU CACHE DEMO =====" << std::endl;
    LRUCache<int, std::string> lruCache(3);

    lruCache.put(1, "one");
    lruCache.put(2, "two");
    lruCache.put(3, "three");

    lruCache.printContents();

    // Access key 1 (makes it most recently used)
    std::cout << "\nAccessing key 1: " << *lruCache.get(1) << std::endl;
    lruCache.printContents();

    // Add a new key (should evict key 2)
    lruCache.put(4, "four");
    std::cout << "\nAfter adding key 4:" << std::endl;
    lruCache.printContents();

    std::cout << "\n===== LFU CACHE DEMO =====" << std::endl;
    LFUCache<int, std::string> lfuCache(3);

    lfuCache.put(1, "one");
    lfuCache.put(2, "two");
    lfuCache.put(3, "three");

    lfuCache.printContents();

    // Access key 1 twice (increases its frequency)
    std::cout << "\nAccessing key 1 twice" << std::endl;
    lfuCache.get(1);
    lfuCache.get(1);

    // Access key 2 once
    std::cout << "Accessing key 2 once" << std::endl;
    lfuCache.get(2);

    lfuCache.printContents();

    // Add a new key (should evict key 3 as it has lowest frequency)
    lfuCache.put(4, "four");
    std::cout << "\nAfter adding key 4:" << std::endl;
    lfuCache.printContents();

    // Demo real-world applications
    webCacheDemo();
    frequencyCounterDemo();
    distributedHashTableDemo();
    cardinalityEstimationDemo();

    // Run performance test (commented out to save time - uncomment to run)
    // performanceTest(10000);

    std::cout << "\n===== END OF DEMONSTRATION =====" << std::endl;

    return 0;
}