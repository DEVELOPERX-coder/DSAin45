# 🔍 DAY 12: Hash Tables - Part 2 - Advanced Techniques & Applications #DSAin45

Welcome to Day 12! Yesterday we laid the foundation for hash tables—understanding their basic structure, operations, and collision resolution strategies. Today, we'll dive deeper into advanced techniques, specialized variants, and sophisticated real-world applications that showcase the true power of hash tables.

_If hash tables were superheroes, yesterday we saw them saving cats from trees. Today we'll watch them stop meteors from destroying Earth while simultaneously brewing the perfect cup of coffee._

## 🏆 Advanced Hash Table Techniques

### 1. Perfect Hashing

Perfect hashing guarantees zero collisions, achieving true O(1) worst-case lookups. There are two main approaches:

#### A. Static Perfect Hashing

When the key set is fixed and known in advance (like compiler keywords), we can construct a perfect hash function:

```cpp
class PerfectHashTable {
private:
    std::vector<std::string> table;
    std::function<size_t(const std::string&)> hashFunc;

    // Compute a perfect hash function for a given set of keys
    std::function<size_t(const std::string&)> computePerfectHashFunction(
        const std::vector<std::string>& keys
    ) {
        // This is a simplified placeholder - in reality, this is complex

        // A real implementation would use algorithms like:
        // - FKS (Fredman-Komlós-Szemerédi) method
        // - Cichelli's method
        // - Czech et al.'s algorithm

        // For now, we'll just return a simple hash function
        return [](const std::string& key) -> size_t {
            // This is NOT actually perfect, just a placeholder
            return std::hash<std::string>{}(key);
        };
    }

public:
    PerfectHashTable(const std::vector<std::string>& keys) {
        // Compute table size (typically slightly larger than key count)
        size_t m = keys.size() * 1.3;
        table.resize(m);

        // Compute a perfect hash function for these keys
        hashFunc = computePerfectHashFunction(keys);

        // Insert all keys
        for (const auto& key : keys) {
            size_t index = hashFunc(key) % table.size();
            table[index] = key;
        }
    }

    bool contains(const std::string& key) const {
        size_t index = hashFunc(key) % table.size();
        return table[index] == key;
    }
};
```

_Perfect hashing is like having a custom-sized drawer for each of your items, pre-built exactly for your collection—a luxury you can only afford when you know your inventory in advance._

#### B. FKS Two-Level Hashing

For static sets, the FKS scheme uses two levels of hashing:

1. The first level is a regular hash table with potential collisions
2. For each bucket with collisions, a secondary perfect hash function is created

```cpp
class FKSTwoLevelHash {
private:
    struct Bucket {
        std::vector<std::string> keys;
        std::function<size_t(const std::string&)> hashFunc;

        bool contains(const std::string& key) const {
            size_t index = hashFunc(key) % keys.size();
            return keys[index] == key;
        }
    };

    std::vector<Bucket> buckets;
    std::function<size_t(const std::string&)> primaryHashFunc;

public:
    FKSTwoLevelHash(const std::vector<std::string>& keys) {
        // Initialize primary hash table
        buckets.resize(keys.size());
        primaryHashFunc = std::hash<std::string>{};

        // Group keys by primary hash
        std::vector<std::vector<std::string>> groupedKeys(buckets.size());
        for (const auto& key : keys) {
            size_t index = primaryHashFunc(key) % buckets.size();
            groupedKeys[index].push_back(key);
        }

        // Create perfect hash function for each bucket
        for (size_t i = 0; i < buckets.size(); i++) {
            if (!groupedKeys[i].empty()) {
                // Compute perfect hash for this bucket
                buckets[i].keys.resize(groupedKeys[i].size() * groupedKeys[i].size());

                // In a real implementation, we'd compute a perfect hash function
                // For now, use a simple hash just to illustrate the concept
                buckets[i].hashFunc = primaryHashFunc;

                // Insert keys into the bucket
                for (const auto& key : groupedKeys[i]) {
                    size_t index = buckets[i].hashFunc(key) % buckets[i].keys.size();
                    buckets[i].keys[index] = key;
                }
            }
        }
    }

    bool contains(const std::string& key) const {
        size_t bucketIndex = primaryHashFunc(key) % buckets.size();
        return buckets[bucketIndex].contains(key);
    }
};
```

_The FKS scheme is like a well-organized library with a main catalog that directs you to specific sections, each with its own perfectly arranged shelf system._

### 2. Cuckoo Hashing

Cuckoo hashing uses multiple hash functions and allows displacing existing items, like a cuckoo bird pushing other eggs out of a nest:

```cpp
template <typename K, typename V>
class CuckooHashTable {
private:
    static const size_t NUM_TABLES = 2;
    static const size_t MAX_LOOP = 100; // Prevent infinite loops

    struct Entry {
        K key;
        V value;
        bool occupied;

        Entry() : occupied(false) {}
    };

    std::vector<Entry> tables[NUM_TABLES];
    std::hash<K> hashFuncs[NUM_TABLES];
    size_t size_;

    size_t hash(size_t tableIndex, const K& key) const {
        return hashFuncs[tableIndex](key) % tables[tableIndex].size();
    }

    bool insertWithEviction(const K& key, const V& value, size_t loopCount = 0) {
        if (loopCount >= MAX_LOOP) {
            rehash(); // Too many evictions, resize and rehash
            return insert(key, value);
        }

        K currentKey = key;
        V currentValue = value;

        for (size_t i = 0; i < NUM_TABLES; i++) {
            size_t idx = hash(i, currentKey);

            // Place the current key-value pair here and evict any previous occupant
            std::swap(currentKey, tables[i][idx].key);
            std::swap(currentValue, tables[i][idx].value);
            std::swap(tables[i][idx].occupied, *reinterpret_cast<bool*>(&tables[i][idx].occupied) = true);

            // If we evicted nothing, we're done
            if (!tables[i][idx].occupied || currentKey == tables[i][idx].key) {
                return true;
            }

            // Otherwise, try to re-insert the evicted item
            if (i == NUM_TABLES - 1) {
                return insertWithEviction(currentKey, currentValue, loopCount + 1);
            }
        }

        return true; // Should not reach here
    }

    void rehash() {
        // Save old tables
        std::vector<Entry> oldTables[NUM_TABLES];
        for (size_t i = 0; i < NUM_TABLES; i++) {
            oldTables[i] = std::move(tables[i]);
            tables[i].resize(oldTables[i].size() * 2);
        }

        size_ = 0;

        // Reinsert all elements
        for (size_t tableIdx = 0; tableIdx < NUM_TABLES; tableIdx++) {
            for (const Entry& entry : oldTables[tableIdx]) {
                if (entry.occupied) {
                    insert(entry.key, entry.value);
                }
            }
        }
    }

public:
    CuckooHashTable(size_t initialSize = 16) : size_(0) {
        for (size_t i = 0; i < NUM_TABLES; i++) {
            tables[i].resize(initialSize);
            // In a real implementation, we'd use different hash functions
            // Here we're using the same with different seeds
            hashFuncs[i] = std::hash<K>{};
        }
    }

    bool insert(const K& key, const V& value) {
        // Check if key already exists
        for (size_t i = 0; i < NUM_TABLES; i++) {
            size_t idx = hash(i, key);
            if (tables[i][idx].occupied && tables[i][idx].key == key) {
                tables[i][idx].value = value;
                return true;
            }
        }

        // Try to insert with possible evictions
        if (insertWithEviction(key, value)) {
            size_++;
            return true;
        }

        return false;
    }

    bool lookup(const K& key, V& value) const {
        for (size_t i = 0; i < NUM_TABLES; i++) {
            size_t idx = hash(i, key);
            if (tables[i][idx].occupied && tables[i][idx].key == key) {
                value = tables[i][idx].value;
                return true;
            }
        }
        return false;
    }

    bool remove(const K& key) {
        for (size_t i = 0; i < NUM_TABLES; i++) {
            size_t idx = hash(i, key);
            if (tables[i][idx].occupied && tables[i][idx].key == key) {
                tables[i][idx].occupied = false;
                size_--;
                return true;
            }
        }
        return false;
    }

    size_t size() const {
        return size_;
    }
};
```

_Cuckoo hashing is like a game of musical chairs where everyone eventually finds a seat—some players might need to displace others, but in the end, lookups become deterministic and fast._

### 3. Hopscotch Hashing

Hopscotch hashing combines the advantages of linear probing (good cache locality) and cuckoo hashing (bounded search time):

```cpp
template <typename K, typename V>
class HopscotchHashTable {
private:
    struct Entry {
        K key;
        V value;
        bool occupied;

        Entry() : occupied(false) {}
    };

    std::vector<Entry> table;
    std::vector<uint32_t> hopInfo; // Bitmap of positions
    size_t size_;
    std::hash<K> hashFunc;
    static const size_t HOP_RANGE = 32; // Size of neighborhood

    size_t hash(const K& key) const {
        return hashFunc(key) % table.size();
    }

    void resize() {
        // Save old table
        std::vector<Entry> oldTable = std::move(table);
        std::vector<uint32_t> oldHopInfo = std::move(hopInfo);

        // Create new table of double size
        table.resize(oldTable.size() * 2);
        hopInfo.resize(table.size(), 0);
        size_ = 0;

        // Reinsert all elements
        for (size_t i = 0; i < oldTable.size(); i++) {
            if (oldTable[i].occupied) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

    bool findEmptySlot(size_t startIdx, size_t& emptyIdx) {
        // Look for an empty slot within the hop range
        for (size_t i = 0; i < table.size(); i++) {
            size_t idx = (startIdx + i) % table.size();
            if (!table[idx].occupied) {
                emptyIdx = idx;
                return true;
            }
        }
        return false;
    }

    bool moveCloser(size_t emptyIdx, size_t& startIdx) {
        // Try to move empty slot closer to the start index
        for (int i = HOP_RANGE - 1; i > 0; i--) {
            size_t fromIdx = (emptyIdx - i + table.size()) % table.size();
            uint32_t hopBit = 1 << i;

            // Find an item that can be moved
            for (int j = 0; j < i; j++) {
                size_t checkIdx = (fromIdx + j) % table.size();
                uint32_t checkBit = 1 << j;

                if ((hopInfo[checkIdx] & hopBit) != 0) {
                    // Found an item that can be moved
                    size_t moveFromIdx = (checkIdx + i) % table.size();

                    // Move the item
                    table[emptyIdx] = table[moveFromIdx];
                    table[moveFromIdx].occupied = false;

                    // Update hop info
                    hopInfo[checkIdx] &= ~hopBit;
                    hopInfo[checkIdx] |= (1 << (emptyIdx - checkIdx) % table.size());

                    // Update empty slot
                    emptyIdx = moveFromIdx;

                    // If empty slot is now within range of start, we're done
                    if ((emptyIdx - startIdx + table.size()) % table.size() < HOP_RANGE) {
                        return true;
                    }

                    // Otherwise, try again with the new empty slot
                    return moveCloser(emptyIdx, startIdx);
                }
            }
        }

        return false;
    }

public:
    HopscotchHashTable(size_t initialSize = 64)
        : table(initialSize), hopInfo(initialSize, 0), size_(0) {}

    bool insert(const K& key, const V& value) {
        // Check load factor and resize if necessary
        if (size_ > table.size() * 0.7) {
            resize();
        }

        // Get initial hash position
        size_t startIdx = hash(key);

        // Check if key already exists in its neighborhood
        for (size_t i = 0; i < HOP_RANGE; i++) {
            size_t idx = (startIdx + i) % table.size();
            if (table[idx].occupied && table[idx].key == key) {
                table[idx].value = value;
                return true;
            }
        }

        // Find an empty slot
        size_t emptyIdx;
        if (!findEmptySlot(startIdx, emptyIdx)) {
            // No empty slots in the table
            resize();
            return insert(key, value);
        }

        // If empty slot is not within HOP_RANGE of start, move it closer
        if ((emptyIdx - startIdx + table.size()) % table.size() >= HOP_RANGE) {
            if (!moveCloser(emptyIdx, startIdx)) {
                // Couldn't move an empty slot closer
                resize();
                return insert(key, value);
            }
        }

        // Insert the new key-value pair
        table[emptyIdx].key = key;
        table[emptyIdx].value = value;
        table[emptyIdx].occupied = true;

        // Update hop info
        hopInfo[startIdx] |= (1 << ((emptyIdx - startIdx + table.size()) % table.size()));

        size_++;
        return true;
    }

    bool lookup(const K& key, V& value) const {
        size_t startIdx = hash(key);

        // Check each position in the neighborhood
        uint32_t hopInfo_tmp = hopInfo[startIdx];
        for (size_t i = 0; hopInfo_tmp != 0 && i < HOP_RANGE; i++) {
            if ((hopInfo_tmp & (1 << i)) != 0) {
                size_t idx = (startIdx + i) % table.size();
                if (table[idx].key == key) {
                    value = table[idx].value;
                    return true;
                }
            }
        }

        return false;
    }

    bool remove(const K& key) {
        size_t startIdx = hash(key);

        // Check each position in the neighborhood
        for (size_t i = 0; i < HOP_RANGE; i++) {
            if ((hopInfo[startIdx] & (1 << i)) != 0) {
                size_t idx = (startIdx + i) % table.size();
                if (table[idx].key == key) {
                    table[idx].occupied = false;
                    hopInfo[startIdx] &= ~(1 << i);
                    size_--;
                    return true;
                }
            }
        }

        return false;
    }

    size_t size() const {
        return size_;
    }
};
```

_Hopscotch hashing is like playing leapfrog to keep important items within arm's reach—you might need to rearrange things a bit, but you'll never have to look far for what you need._

### 4. Robin Hood Hashing with Backward Shift Deletion

Robin Hood hashing steals slots from "rich" elements (close to their ideal position) to give to "poor" elements (far from their ideal position). Backward shift deletion enhances this by efficiently handling deletions:

```cpp
template <typename K, typename V>
class RobinHoodHashTable {
private:
    struct Entry {
        K key;
        V value;
        bool occupied;
        int probeDistance; // Distance from ideal position

        Entry() : occupied(false), probeDistance(-1) {}
    };

    std::vector<Entry> table;
    size_t size_;
    double maxLoadFactor;
    std::hash<K> hashFunc;

    size_t hash(const K& key) const {
        return hashFunc(key) % table.size();
    }

    void resize() {
        std::vector<Entry> oldTable = std::move(table);
        table.resize(oldTable.size() * 2);
        size_ = 0;

        for (const Entry& entry : oldTable) {
            if (entry.occupied) {
                insert(entry.key, entry.value);
            }
        }
    }

public:
    RobinHoodHashTable(size_t initialSize = 16, double loadFactor = 0.7)
        : table(initialSize), size_(0), maxLoadFactor(loadFactor) {}

    bool insert(const K& key, const V& value) {
        if (static_cast<double>(size_) / table.size() > maxLoadFactor) {
            resize();
        }

        size_t idealPos = hash(key);
        int probeDistance = 0;

        K currentKey = key;
        V currentValue = value;
        int currentProbeDistance = probeDistance;

        while (true) {
            size_t pos = (idealPos + probeDistance) % table.size();

            // If empty slot, insert here
            if (!table[pos].occupied) {
                table[pos].key = currentKey;
                table[pos].value = currentValue;
                table[pos].occupied = true;
                table[pos].probeDistance = currentProbeDistance;
                size_++;
                return true;
            }

            // If key already exists, update value
            if (table[pos].key == currentKey) {
                table[pos].value = currentValue;
                return true;
            }

            // Robin Hood: Steal from the rich, give to the poor
            if (currentProbeDistance > table[pos].probeDistance) {
                // Swap entries
                std::swap(currentKey, table[pos].key);
                std::swap(currentValue, table[pos].value);
                std::swap(currentProbeDistance, table[pos].probeDistance);
            }

            // Move to next position
            probeDistance++;
            currentProbeDistance++;

            // Prevent infinite loop
            if (probeDistance >= static_cast<int>(table.size())) {
                resize();
                return insert(currentKey, currentValue);
            }
        }
    }

    bool lookup(const K& key, V& value) const {
        size_t idealPos = hash(key);
        int probeDistance = 0;

        while (true) {
            size_t pos = (idealPos + probeDistance) % table.size();

            // If empty slot or exceeded probe distance, key is not in table
            if (!table[pos].occupied || probeDistance > table[pos].probeDistance) {
                return false;
            }

            // Found the key
            if (table[pos].key == key) {
                value = table[pos].value;
                return true;
            }

            // Move to next position
            probeDistance++;

            // Prevent infinite loop
            if (probeDistance >= static_cast<int>(table.size())) {
                return false;
            }
        }
    }

    bool remove(const K& key) {
        size_t idealPos = hash(key);
        int probeDistance = 0;

        while (true) {
            size_t pos = (idealPos + probeDistance) % table.size();

            // If empty slot or exceeded probe distance, key is not in table
            if (!table[pos].occupied || probeDistance > table[pos].probeDistance) {
                return false;
            }

            // Found the key
            if (table[pos].key == key) {
                // Backward shift deletion
                size_t current = pos;
                size_t next = (current + 1) % table.size();

                while (table[next].occupied && table[next].probeDistance > 0) {
                    table[current] = table[next];
                    table[current].probeDistance--;

                    current = next;
                    next = (current + 1) % table.size();
                }

                // Mark the last shifted position as unoccupied
                table[current].occupied = false;
                table[current].probeDistance = -1;

                size_--;
                return true;
            }

            // Move to next position
            probeDistance++;

            // Prevent infinite loop
            if (probeDistance >= static_cast<int>(table.size())) {
                return false;
            }
        }
    }

    size_t size() const {
        return size_;
    }
};
```

_Robin Hood with backward shift deletion is like a well-managed homeless shelter: not only does it ensure everyone has a place that minimizes their distance from home, but when someone leaves, others shift up to fill the gap, maintaining optimal distances for all._

## 🌟 Specialized Hash Table Variants

### 1. Advanced Bloom Filters

Bloom filters are space-efficient probabilistic data structures for testing set membership with a possibility of false positives:

```cpp
class CountingBloomFilter {
private:
    std::vector<uint8_t> counters; // Use counters instead of bits
    size_t numHashFunctions;
    std::vector<std::function<size_t(const std::string&)>> hashFuncs;

    void initHashFunctions() {
        // Create different hash functions (simplified here)
        for (size_t i = 0; i < numHashFunctions; i++) {
            // In a real implementation, these would be truly independent hash functions
            hashFuncs.push_back([i](const std::string& key) {
                std::hash<std::string> hasher;
                return hasher(key + std::to_string(i));
            });
        }
    }

public:
    CountingBloomFilter(size_t size, size_t numHashes)
        : counters(size, 0), numHashFunctions(numHashes) {
        initHashFunctions();
    }

    void insert(const std::string& key) {
        for (const auto& hashFunc : hashFuncs) {
            size_t index = hashFunc(key) % counters.size();

            // Increment counter, saturate at 255
            if (counters[index] < 255) {
                counters[index]++;
            }
        }
    }

    void remove(const std::string& key) {
        // Check if the key might be in the filter first
        if (!mightContain(key)) {
            return;
        }

        for (const auto& hashFunc : hashFuncs) {
            size_t index = hashFunc(key) % counters.size();

            // Decrement counter if not zero
            if (counters[index] > 0) {
                counters[index]--;
            }
        }
    }

    bool mightContain(const std::string& key) const {
        for (const auto& hashFunc : hashFuncs) {
            size_t index = hashFunc(key) % counters.size();
            if (counters[index] == 0) {
                return false; // Definitely not in the set
            }
        }
        return true; // Might be in the set
    }

    double estimateItemCount() const {
        // Use the minimum counter value approach
        size_t minCounter = std::numeric_limits<size_t>::max();

        for (uint8_t count : counters) {
            if (count > 0 && count < minCounter) {
                minCounter = count;
            }
        }

        // Rough estimate based on minimum counter value
        if (minCounter == std::numeric_limits<size_t>::max()) {
            return 0; // Empty filter
        }

        return -static_cast<double>(counters.size()) *
               log(1.0 - static_cast<double>(minCounter) / 255) /
               numHashFunctions;
    }
};
```

_A counting Bloom filter is like a sophisticated nightclub bouncer who not only remembers who's inside but can also count how many people with the same name are in the club, allowing some to leave without affecting others._

### 2. Cuckoo Filters

Cuckoo filters improve on Bloom filters by supporting deletion and having better space efficiency:

```cpp
class CuckooFilter {
private:
    static const size_t BUCKET_SIZE = 4; // Entries per bucket
    static const size_t MAX_RELOCATION = 500; // Max number of evictions

    struct Bucket {
        std::vector<uint32_t> fingerprints;

        Bucket() : fingerprints(BUCKET_SIZE, 0) {}

        bool insert(uint32_t fingerprint) {
            for (size_t i = 0; i < BUCKET_SIZE; i++) {
                if (fingerprints[i] == 0) {
                    fingerprints[i] = fingerprint;
                    return true;
                }
            }
            return false; // Bucket is full
        }

        bool remove(uint32_t fingerprint) {
            for (size_t i = 0; i < BUCKET_SIZE; i++) {
                if (fingerprints[i] == fingerprint) {
                    fingerprints[i] = 0;
                    return true;
                }
            }
            return false; // Fingerprint not found
        }

        bool contains(uint32_t fingerprint) const {
            for (size_t i = 0; i < BUCKET_SIZE; i++) {
                if (fingerprints[i] == fingerprint) {
                    return true;
                }
            }
            return false;
        }

        uint32_t getRandomFingerprint() const {
            std::vector<uint32_t> nonZero;
            for (uint32_t fp : fingerprints) {
                if (fp != 0) {
                    nonZero.push_back(fp);
                }
            }

            if (nonZero.empty()) {
                return 0;
            }

            return nonZero[rand() % nonZero.size()];
        }
    };

    std::vector<Bucket> buckets;
    size_t numItems;

    // Hash functions
    uint32_t hashToFingerprint(const std::string& item) const {
        std::hash<std::string> hasher;
        return (hasher(item) & 0xFFFFFF) | 0x1; // Ensure non-zero fingerprint
    }

    size_t hashToIndex(uint32_t fingerprint, size_t seed) const {
        std::hash<size_t> hasher;
        return (hasher(fingerprint ^ seed)) % buckets.size();
    }

    size_t alternateIndex(size_t index, uint32_t fingerprint) const {
        return (index ^ hashToIndex(fingerprint, 0x5bd1e995)) % buckets.size();
    }

public:
    CuckooFilter(size_t capacity) : buckets(nextPowerOf2(capacity) / BUCKET_SIZE), numItems(0) {}

    bool insert(const std::string& item) {
        uint32_t fingerprint = hashToFingerprint(item);
        size_t i1 = hashToIndex(fingerprint, 0);

        // Try to insert in first bucket
        if (buckets[i1].insert(fingerprint)) {
            numItems++;
            return true;
        }

        // Try to insert in second bucket
        size_t i2 = alternateIndex(i1, fingerprint);
        if (buckets[i2].insert(fingerprint)) {
            numItems++;
            return true;
        }

        // Both buckets are full, start cuckoo relocation
        size_t currentIndex = (rand() % 2) ? i1 : i2;

        for (size_t count = 0; count < MAX_RELOCATION; count++) {
            // Get a random fingerprint from the bucket
            uint32_t evictedFP = buckets[currentIndex].getRandomFingerprint();

            // Remove the fingerprint and insert the current one
            for (size_t i = 0; i < BUCKET_SIZE; i++) {
                if (buckets[currentIndex].fingerprints[i] == evictedFP) {
                    buckets[currentIndex].fingerprints[i] = fingerprint;

                    // Try to find a new place for the evicted fingerprint
                    fingerprint = evictedFP;
                    currentIndex = alternateIndex(currentIndex, fingerprint);

                    // If the new bucket has space, we're done
                    if (buckets[currentIndex].insert(fingerprint)) {
                        numItems++;
                        return true;
                    }

                    // Otherwise, continue evicting
                    break;
                }
            }
        }

        // Too many relocations, resize the filter
        // (simplified - would actually double the size and rehash)
        return false;
    }

    bool remove(const std::string& item) {
        uint32_t fingerprint = hashToFingerprint(item);
        size_t i1 = hashToIndex(fingerprint, 0);

        if (buckets[i1].remove(fingerprint)) {
            numItems--;
            return true;
        }

        size_t i2 = alternateIndex(i1, fingerprint);
        if (buckets[i2].remove(fingerprint)) {
            numItems--;
            return true;
        }

        return false; // Item not found
    }

    bool contains(const std::string& item) const {
        uint32_t fingerprint = hashToFingerprint(item);
        size_t i1 = hashToIndex(fingerprint, 0);
        size_t i2 = alternateIndex(i1, fingerprint);

        return buckets[i1].contains(fingerprint) || buckets[i2].contains(fingerprint);
    }

    size_t size() const {
        return numItems;
    }

    double loadFactor() const {
        return static_cast<double>(numItems) / (buckets.size() * BUCKET_SIZE);
    }

private:
    static size_t nextPowerOf2(size_t n) {
        size_t power = 1;
        while (power < n) {
            power *= 2;
        }
        return power;
    }
};
```

_A cuckoo filter is like a cleverly designed apartment building where each resident knows exactly two possible units they can live in—if one is occupied, they can kick out the current tenant who then finds their alternate spot, creating a chain reaction that almost always leads to everyone having a home._

### 3. Count-Min Sketch

The Count-Min Sketch efficiently estimates frequencies of elements in a stream:

```cpp
class CountMinSketch {
private:
    std::vector<std::vector<int>> counters;
    std::vector<std::function<size_t(const std::string&)>> hashFuncs;

    void initHashFunctions(size_t d) {
        for (size_t i = 0; i < d; i++) {
            // In a real implementation, these would be truly independent hash functions
            hashFuncs.push_back([i](const std::string& key) {
                std::hash<std::string> hasher;
                return hasher(key + std::to_string(i));
            });
        }
    }

public:
    CountMinSketch(double epsilon, double delta) {
        // Calculate dimensions based on error bounds
        size_t w = ceil(2 / epsilon); // Width (columns)
        size_t d = ceil(log(1 / delta)); // Depth (rows)

        counters.resize(d, std::vector<int>(w, 0));
        initHashFunctions(d);
    }

    void add(const std::string& item, int count = 1) {
        for (size_t i = 0; i < hashFuncs.size(); i++) {
            size_t index = hashFuncs[i](item) % counters[i].size();
            counters[i][index] += count;
        }
    }

    int estimate(const std::string& item) const {
        int estimate = std::numeric_limits<int>::max();

        for (size_t i = 0; i < hashFuncs.size(); i++) {
            size_t index = hashFuncs[i](item) % counters[i].size();
            estimate = std::min(estimate, counters[i][index]);
        }

        return estimate;
    }

    // Merge with another Count-Min Sketch
    void merge(const CountMinSketch& other) {
        if (counters.size() != other.counters.size() ||
            counters[0].size() != other.counters[0].size()) {
            throw std::invalid_argument("Sketches must have the same dimensions");
        }

        for (size_t i = 0; i < counters.size(); i++) {
            for (size_t j = 0; j < counters[i].size(); j++) {
                counters[i][j] += other.counters[i][j];
            }
        }
    }
};
```

_A Count-Min Sketch is like a room of counters with bad eyesight—each one tries to count occurrences of items, but sometimes counts other similar-looking items too. By asking all counters and taking the minimum count, we get a pretty good estimate despite their individual inaccuracies._

### 4. HyperLogLog

HyperLogLog estimates the number of distinct elements in a large dataset with minimal memory:

```cpp
class HyperLogLog {
private:
    std::vector<uint8_t> registers;
    size_t p; // Number of bits used for register indexing

    size_t hash(const std::string& item) const {
        std::hash<std::string> hasher;
        return hasher(item);
    }

    // Count leading zeros in a hash value
    static int leadingZeros(size_t value) {
        if (value == 0) return 64;

        int zeros = 0;
        for (int i = 63; i >= 0; i--) {
            if ((value & (1ULL << i)) == 0) {
                zeros++;
            } else {
                break;
            }
        }
        return zeros;
    }

public:
    HyperLogLog(size_t precision) : p(precision) {
        size_t m = 1 << precision; // Number of registers
        registers.resize(m, 0);
    }

    void add(const std::string& item) {
        size_t hashValue = hash(item);

        // Use p bits to determine the register
        size_t registerIdx = hashValue & ((1ULL << p) - 1);

        // Count leading zeros in the remaining bits
        size_t remainingBits = hashValue >> p;
        int zeros = leadingZeros(remainingBits) - p;
        int rank = zeros + 1;

        // Update the register if the new value is larger
        registers[registerIdx] = std::max(registers[registerIdx], static_cast<uint8_t>(rank));
    }

    double estimate() const {
        double sum = 0.0;
        size_t m = registers.size();

        for (uint8_t rank : registers) {
            sum += 1.0 / (1ULL << rank);
        }

        double alpha;
        if (m == 16) alpha = 0.673;
        else if (m == 32) alpha = 0.697;
        else if (m == 64) alpha = 0.709;
        else alpha = 0.7213 / (1 + 1.079 / m);

        double estimate = alpha * m * m / sum;

        // Small range correction
        if (estimate <= 2.5 * m) {
            int zeros = 0;
            for (uint8_t rank : registers) {
                if (rank == 0) zeros++;
            }

            if (zeros > 0) {
                estimate = m * log(static_cast<double>(m) / zeros);
            }
        }

        return estimate;
    }

    // Merge with another HyperLogLog
    void merge(const HyperLogLog& other) {
        if (p != other.p) {
            throw std::invalid_argument("Precision must match");
        }

        for (size_t i = 0; i < registers.size(); i++) {
            registers[i] = std::max(registers[i], other.registers[i]);
        }
    }
};
```

_HyperLogLog is like estimating the number of unique birds in a forest by tracking how rare your sightings are—if you frequently see unusual patterns, there must be many different birds._

## 🛠️ Custom Hash Functions for Complex Types

### 1. Hashing User-Defined Classes

```cpp
struct Point {
    int x, y, z;

    // Define equality operator for hash table comparisons
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Specialize std::hash for Point
namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            // Combine hash values of all fields
            size_t h1 = std::hash<int>{}(p.x);
            size_t h2 = std::hash<int>{}(p.y);
            size_t h3 = std::hash<int>{}(p.z);

            // Combine using a good hash combining function
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

// Now you can use Point as a key in unordered_map or other hash tables
std::unordered_map<Point, std::string> pointLabels;
pointLabels[{1, 2, 3}] = "Corner";
```

_Creating a custom hash function for your class is like giving each of your children a unique ID that combines features of your family name, birth date, and favorite color—ensuring they can be quickly identified in any database._

### 2. Hashing Variable-Length Containers

```cpp
template <typename T>
size_t hashVector(const std::vector<T>& vec) {
    size_t hash = vec.size(); // Start with the size

    // Combine with hash of each element
    for (const T& item : vec) {
        hash ^= std::hash<T>{}(item) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    return hash;
}

// Specialize std::hash for vector<int>
namespace std {
    template <>
    struct hash<vector<int>> {
        size_t operator()(const vector<int>& v) const {
            return hashVector(v);
        }
    };
}

// Now you can use vector<int> as a key
std::unordered_map<std::vector<int>, std::string> pathLabels;
pathLabels[{1, 2, 3}] = "Path A";
```

_Hashing collections is like creating a unique fingerprint for a family based on each member's individual traits, plus their arrangement and total count._

### 3. Writing Effective Hash Combiners

```cpp
// A good hash combiner function (Boost-inspired)
template <typename T>
inline void hash_combine(size_t& seed, const T& val) {
    seed ^= std::hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// Hash for std::pair
template <typename T1, typename T2>
struct pair_hash {
    size_t operator()(const std::pair<T1, T2>& p) const {
        size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);
        return seed;
    }
};

// Usage
std::unordered_map<std::pair<int, int>, int, pair_hash<int, int>> edges;
edges[{1, 2}] = 10; // Edge from 1 to 2 with weight 10
```

_A good hash combiner is like a magical blender that takes multiple ingredients and creates a completely new flavor that still somehow represents all the original components._

## 🔮 Thread-Safe Hash Tables

### 1. Lock-Based Concurrent Hash Table

```cpp
template <typename K, typename V>
class ConcurrentHashTable {
private:
    struct Entry {
        K key;
        V value;
        std::unique_ptr<Entry> next;

        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<std::unique_ptr<Entry>> buckets;
    std::vector<std::mutex> locks; // One lock per bucket
    size_t size_;
    std::hash<K> hashFunc;

    size_t hash(const K& key) const {
        return hashFunc(key) % buckets.size();
    }

public:
    ConcurrentHashTable(size_t numBuckets = 16)
        : buckets(numBuckets), locks(numBuckets), size_(0) {}

    void insert(const K& key, const V& value) {
        size_t bucketIdx = hash(key);
        std::lock_guard<std::mutex> lock(locks[bucketIdx]);

        Entry* current = buckets[bucketIdx].get();

        // If bucket is empty, create first entry
        if (!current) {
            buckets[bucketIdx] = std::make_unique<Entry>(key, value);
            size_++;
            return;
        }

        // Check if key already exists in the bucket
        while (current) {
            if (current->key == key) {
                current->value = value; // Update existing key
                return;
            }

            if (!current->next) {
                // Reached end of chain, add new entry
                current->next = std::make_unique<Entry>(key, value);
                size_++;
                return;
            }

            current = current->next.get();
        }
    }

    bool lookup(const K& key, V& value) const {
        size_t bucketIdx = hash(key);
        std::lock_guard<std::mutex> lock(locks[bucketIdx]);

        Entry* current = buckets[bucketIdx].get();

        while (current) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next.get();
        }

        return false;
    }

    bool remove(const K& key) {
        size_t bucketIdx = hash(key);
        std::lock_guard<std::mutex> lock(locks[bucketIdx]);

        Entry* current = buckets[bucketIdx].get();

        // Special case: first entry matches
        if (current && current->key == key) {
            buckets[bucketIdx] = std::move(current->next);
            size_--;
            return true;
        }

        // Search in the chain
        while (current && current->next) {
            if (current->next->key == key) {
                current->next = std::move(current->next->next);
                size_--;
                return true;
            }
            current = current->next.get();
        }

        return false;
    }

    size_t size() const {
        // This is not thread-safe for exact counts
        return size_;
    }
};
```

_A lock-based concurrent hash table is like a building with many rooms, each with its own door lock—multiple people can access different rooms simultaneously, but only one person at a time can enter a specific room._

### 2. Lock-Free Hash Table (Simplified)

```cpp
template <typename K, typename V>
class LockFreeHashTable {
private:
    struct Entry {
        K key;
        std::atomic<V> value;
        std::atomic<Entry*> next;

        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<std::atomic<Entry*>> buckets;
    std::atomic<size_t> size_;
    std::hash<K> hashFunc;

    size_t hash(const K& key) const {
        return hashFunc(key) % buckets.size();
    }

public:
    LockFreeHashTable(size_t numBuckets = 16)
        : buckets(numBuckets), size_(0) {
        for (auto& bucket : buckets) {
            bucket = nullptr;
        }
    }

    ~LockFreeHashTable() {
        // Cleanup - in a real implementation, this would need to be thread-safe
        for (auto& bucket : buckets) {
            Entry* current = bucket.load();
            while (current) {
                Entry* next = current->next.load();
                delete current;
                current = next;
            }
        }
    }

    void insert(const K& key, const V& value) {
        size_t bucketIdx = hash(key);

        // Create new entry
        Entry* newEntry = new Entry(key, value);

        while (true) {
            Entry* oldHead = buckets[bucketIdx].load();

            // Check if key already exists
            Entry* current = oldHead;
            while (current) {
                if (current->key == key) {
                    // Key exists, update value
                    current->value.store(value);

                    // Delete unused entry
                    delete newEntry;
                    return;
                }
                current = current->next.load();
            }

            // Insert at head of list
            newEntry->next = oldHead;

            // Try to replace head with new entry
            if (buckets[bucketIdx].compare_exchange_strong(oldHead, newEntry)) {
                // Success, increment size
                size_.fetch_add(1);
                return;
            }

            // Failed to insert, retry
        }
    }

    bool lookup(const K& key, V& value) const {
        size_t bucketIdx = hash(key);
        Entry* current = buckets[bucketIdx].load();

        while (current) {
            if (current->key == key) {
                value = current->value.load();
                return true;
            }
            current = current->next.load();
        }

        return false;
    }

    bool remove(const K& key) {
        size_t bucketIdx = hash(key);

        while (true) {
            Entry* head = buckets[bucketIdx].load();
            Entry* prev = nullptr;
            Entry* current = head;

            // Find the entry to remove
            while (current && current->key != key) {
                prev = current;
                current = current->next.load();
            }

            if (!current) {
                return false; // Key not found
            }

            // Key found, try to remove it
            Entry* next = current->next.load();

            if (prev) {
                // Not the head node
                if (prev->next.compare_exchange_strong(current, next)) {
                    // Success, decrement size and delete node
                    size_.fetch_sub(1);
                    delete current;
                    return true;
                }
            } else {
                // Head node
                if (buckets[bucketIdx].compare_exchange_strong(current, next)) {
                    // Success, decrement size and delete node
                    size_.fetch_sub(1);
                    delete current;
                    return true;
                }
            }

            // Failed to remove, retry
        }
    }

    size_t size() const {
        return size_.load();
    }
};
```

_A lock-free hash table is like a building where multiple people can simultaneously work in the same room without colliding, each carefully stepping around others and using special eye signals to coordinate their movements._

## 📊 Performance Optimization Techniques

### 1. Load Factor Management

```cpp
template <typename K, typename V>
class OptimizedHashTable {
private:
    // Implementation details...

    // Dynamically adjust load factor based on operation patterns
    void adjustLoadFactor() {
        // Track patterns of operations
        if (lookupMisses > threshold && loadFactor > 0.5) {
            // Many lookup misses, reduce load factor
            targetLoadFactor = std::max(0.3, targetLoadFactor - 0.1);
            if (currentLoadFactor() > targetLoadFactor) {
                resize(buckets.size() * 2);
            }
        } else if (lookupHits > threshold && loadFactor < 0.7) {
            // Many successful lookups, can increase load factor
            targetLoadFactor = std::min(0.8, targetLoadFactor + 0.1);
        }

        // Reset counters
        lookupHits = lookupMisses = 0;
    }

public:
    // API methods...
};
```

_Dynamically adjusting load factors is like a restaurant that changes how many tables it uses based on customer patterns—fewer tables when people browse longer, more tables when customers eat quickly and leave._

### 2. Cache-Friendly Layout

```cpp
template <typename K, typename V>
class CacheFriendlyHashTable {
private:
    // Store key-value pairs in contiguous memory
    struct Entry {
        K key;
        V value;
        bool occupied;
    };

    std::vector<Entry> entries;

    // Organize entries in cache lines
    static constexpr size_t CACHE_LINE = 64; // Bytes
    static constexpr size_t ENTRIES_PER_LINE =
        CACHE_LINE / sizeof(Entry) ? CACHE_LINE / sizeof(Entry) : 1;

    // Implementation details...

public:
    // API methods...
};
```

_A cache-friendly hash table is like a grocery store organized so that commonly purchased items are placed close together, minimizing the shopper's walking distance._

### 3. Vectorized Hash Computation

```cpp
// Using SIMD for parallel hash computation
std::vector<size_t> computeHashesVectorized(const std::vector<std::string>& keys) {
    std::vector<size_t> hashes(keys.size());

    // This is a conceptual example - actual SIMD implementation
    // would use intrinsics like _mm256_* for AVX2
    #pragma omp simd
    for (size_t i = 0; i < keys.size(); i++) {
        hashes[i] = std::hash<std::string>{}(keys[i]);
    }

    return hashes;
}
```

_Vectorized hash computation is like having multiple checkout lines in a store that can process several customers simultaneously rather than one at a time._

## 🌐 Real-World Applications & Case Studies

### 1. Database Index Implementation

```cpp
class HashIndex {
private:
    std::unordered_map<Key, std::vector<RecordID>> index;

public:
    void addRecord(const Key& key, RecordID rid) {
        index[key].push_back(rid);
    }

    std::vector<RecordID> lookup(const Key& key) {
        auto it = index.find(key);
        if (it != index.end()) {
            return it->second;
        }
        return {};
    }

    void removeRecord(const Key& key, RecordID rid) {
        auto it = index.find(key);
        if (it != index.end()) {
            auto& rids = it->second;
            rids.erase(std::remove(rids.begin(), rids.end(), rid), rids.end());

            if (rids.empty()) {
                index.erase(it);
            }
        }
    }
};
```

_Database hash indexes allow systems like MongoDB to instantly find documents with specific field values, turning what would be full collection scans into pinpoint retrievals._

### 2. Web Cache Implementation

```cpp
class WebCache {
private:
    struct CacheEntry {
        std::string content;
        std::chrono::steady_clock::time_point expiry;
    };

    std::unordered_map<std::string, CacheEntry> cache;
    std::mutex cacheMutex;

public:
    void store(const std::string& url, const std::string& content,
              std::chrono::seconds ttl) {
        std::lock_guard<std::mutex> lock(cacheMutex);

        auto expiry = std::chrono::steady_clock::now() + ttl;
        cache[url] = {content, expiry};

        // Cleanup expired entries occasionally
        if (rand() % 100 == 0) {
            cleanupExpired();
        }
    }

    std::optional<std::string> retrieve(const std::string& url) {
        std::lock_guard<std::mutex> lock(cacheMutex);

        auto it = cache.find(url);
        if (it != cache.end()) {
            // Check if entry has expired
            if (it->second.expiry > std::chrono::steady_clock::now()) {
                return it->second.content;
            } else {
                // Remove expired entry
                cache.erase(it);
            }
        }

        return std::nullopt;
    }

private:
    void cleanupExpired() {
        auto now = std::chrono::steady_clock::now();

        for (auto it = cache.begin(); it != cache.end(); ) {
            if (it->second.expiry <= now) {
                it = cache.erase(it);
            } else {
                ++it;
            }
        }
    }
};
```

_Web caches like Varnish and Cloudflare use sophisticated hash tables to store and retrieve content based on URLs and other request attributes, serving cached content in microseconds rather than regenerating it._

### 3. Language Virtual Machine (Simplified)

```cpp
class VM {
private:
    struct Object {
        enum Type { INT, STRING, ARRAY, OBJECT };
        Type type;
        union {
            int intValue;
            std::string* stringValue;
            std::vector<Object*>* arrayValue;
            std::unordered_map<std::string, Object*>* objectValue;
        };
    };

    std::unordered_map<std::string, Object*> globals;
    std::vector<std::unordered_map<std::string, Object*>> callStack;

public:
    void setGlobal(const std::string& name, Object* value) {
        globals[name] = value;
    }

    Object* getVariable(const std::string& name) {
        // First check local variables
        if (!callStack.empty()) {
            auto& locals = callStack.back();
            auto it = locals.find(name);
            if (it != locals.end()) {
                return it->second;
            }
        }

        // Then check globals
        auto it = globals.find(name);
        if (it != globals.end()) {
            return it->second;
        }

        return nullptr; // Variable not found
    }

    void pushFrame() {
        callStack.push_back({});
    }

    void popFrame() {
        if (!callStack.empty()) {
            callStack.pop_back();
        }
    }

    void setLocal(const std::string& name, Object* value) {
        if (!callStack.empty()) {
            callStack.back()[name] = value;
        }
    }
};
```

_JavaScript engines like V8 use hash tables to store object properties, making dynamic property access blazingly fast despite JavaScript's flexible, prototype-based object model._

### 4. Network Router Implementation

```cpp
class Router {
private:
    struct Route {
        std::string destination;
        std::string mask;
        std::string nextHop;
        int metric;
    };

    // Fast lookup table for routes
    std::unordered_map<std::string, Route> routes;

    // Use a bloom filter for quick negative lookups
    BloomFilter routeFilter;

public:
    Router() : routeFilter(10000, 5) {}

    void addRoute(const Route& route) {
        routes[route.destination] = route;
        routeFilter.insert(route.destination);
    }

    std::optional<std::string> getNextHop(const std::string& destination) {
        // Quick check if route might exist
        if (!routeFilter.mightContain(destination)) {
            return std::nullopt;
        }

        // Look for exact match
        auto it = routes.find(destination);
        if (it != routes.end()) {
            return it->second.nextHop;
        }

        // Fall back to longest prefix match (simplified)
        // In a real router, this would use a more specialized data structure

        return std::nullopt;
    }
};
```

_Network routers use specialized hash tables to implement route lookup tables, enabling them to forward packets at wire speed by quickly determining the next hop for each destination._

### 5. Machine Learning Feature Hashing

```cpp
class FeatureHasher {
private:
    size_t numDimensions;
    std::hash<std::string> hasher;

public:
    FeatureHasher(size_t dims) : numDimensions(dims) {}

    std::vector<double> hashFeatures(const std::unordered_map<std::string, double>& features) {
        std::vector<double> result(numDimensions, 0.0);

        for (const auto& [feature, value] : features) {
            // Hash the feature name to get the index
            size_t index = hasher(feature) % numDimensions;

            // Determine sign hash (to reduce collisions)
            size_t signHash = hasher(feature + "_sign");
            double sign = (signHash % 2 == 0) ? 1.0 : -1.0;

            // Add the feature value with sign
            result[index] += sign * value;
        }

        return result;
    }

    // Hash multiple examples at once
    std::vector<std::vector<double>> hashBatch(
        const std::vector<std::unordered_map<std::string, double>>& batch) {
        std::vector<std::vector<double>> result;
        result.reserve(batch.size());

        for (const auto& features : batch) {
            result.push_back(hashFeatures(features));
        }

        return result;
    }
};
```

_Feature hashing (or the "hashing trick") allows machine learning systems like Vowpal Wabbit to handle extremely high-dimensional, sparse feature spaces efficiently without requiring an explicit dictionary._

## 🎮 Complex Problem Solutions

### 1. Efficient Autocomplete System

```cpp
class Autocomplete {
private:
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        bool isEndOfWord;
        std::string word;
        int frequency;

        TrieNode() : isEndOfWord(false), frequency(0) {}
    };

    std::unique_ptr<TrieNode> root;

    // Store frequent prefixes and their top completions
    std::unordered_map<std::string, std::vector<std::string>> completionCache;

    // Find all words with given prefix
    std::vector<std::pair<std::string, int>> findAllWithPrefix(const std::string& prefix) {
        std::vector<std::pair<std::string, int>> result;

        // Find the node corresponding to the prefix
        TrieNode* node = root.get();
        for (char c : prefix) {
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                return result; // Prefix not found
            }
            node = it->second.get();
        }

        // Collect all words under this node
        collectWords(node, result);

        return result;
    }

    // DFS to collect all words under a node
    void collectWords(TrieNode* node, std::vector<std::pair<std::string, int>>& result) {
        if (node->isEndOfWord) {
            result.push_back({node->word, node->frequency});
        }

        for (const auto& [c, childNode] : node->children) {
            collectWords(childNode.get(), result);
        }
    }

public:
    Autocomplete() : root(std::make_unique<TrieNode>()) {}

    void addWord(const std::string& word, int frequency = 1) {
        TrieNode* node = root.get();

        for (char c : word) {
            auto& childPtr = node->children[c];
            if (!childPtr) {
                childPtr = std::make_unique<TrieNode>();
            }
            node = childPtr.get();
        }

        node->isEndOfWord = true;
        node->word = word;
        node->frequency += frequency;

        // Clear affected cache entries
        for (size_t i = 1; i <= word.length(); i++) {
            completionCache.erase(word.substr(0, i));
        }
    }

    std::vector<std::string> getCompletions(const std::string& prefix, int limit = 5) {
        // Check cache first
        auto cacheIt = completionCache.find(prefix);
        if (cacheIt != completionCache.end()) {
            return cacheIt->second;
        }

        // Find all matching words
        auto allMatches = findAllWithPrefix(prefix);

        // Sort by frequency
        std::sort(allMatches.begin(), allMatches.end(),
                 [](const auto& a, const auto& b) {
                     return a.second > b.second;
                 });

        // Extract top results
        std::vector<std::string> result;
        for (size_t i = 0; i < allMatches.size() && i < limit; i++) {
            result.push_back(allMatches[i].first);
        }

        // Cache results
        completionCache[prefix] = result;

        return result;
    }
};
```

_Autocomplete systems like those in Google Search use sophisticated data structures including hash tables and tries to instantly find the most relevant completions as you type._

### 2. LFU Cache (Least Frequently Used)

```cpp
class LFUCache {
private:
    int capacity;
    int minFreq;

    // Map: key -> {value, frequency, iterator to frequency list}
    std::unordered_map<int, std::tuple<int, int, std::list<int>::iterator>> keyMap;

    // Map: frequency -> list of keys with that frequency
    std::unordered_map<int, std::list<int>> freqMap;

public:
    LFUCache(int capacity) : capacity(capacity), minFreq(0) {}

    int get(int key) {
        auto it = keyMap.find(key);
        if (it == keyMap.end()) {
            return -1; // Key not found
        }

        // Extract current value, frequency, and list iterator
        auto [value, freq, iter] = it->second;

        // Remove from current frequency list
        freqMap[freq].erase(iter);

        // Increase frequency
        freq++;

        // Add to the new frequency list
        freqMap[freq].push_front(key);

        // Update key map with new frequency and iterator
        it->second = {value, freq, freqMap[freq].begin()};

        // Update minFreq if the previous list is now empty
        if (freqMap[minFreq].empty()) {
            minFreq++;
        }

        return value;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;

        // Check if key already exists
        auto it = keyMap.find(key);
        if (it != keyMap.end()) {
            // Update existing key
            auto& [_, freq, iter] = it->second;

            // Remove from current frequency list
            freqMap[freq].erase(iter);

            // Increase frequency
            freq++;

            // Add to the new frequency list
            freqMap[freq].push_front(key);

            // Update key map
            it->second = {value, freq, freqMap[freq].begin()};

            // Update minFreq if needed
            if (freqMap[minFreq].empty()) {
                minFreq++;
            }

            return;
        }

        // Key doesn't exist, check capacity
        if (keyMap.size() >= capacity) {
            // Find the least frequently used key
            int lfu_key = freqMap[minFreq].back();

            // Remove it
            freqMap[minFreq].pop_back();
            keyMap.erase(lfu_key);
        }

        // Insert new key with frequency 1
        minFreq = 1;
        freqMap[1].push_front(key);
        keyMap[key] = {value, 1, freqMap[1].begin()};
    }
};
```

_LFU caches like this one are used in content delivery networks to ensure the most frequently accessed files stay in memory, optimizing for popular content that many users request._

### 3. Distributed Consistent Hashing

```cpp
class ConsistentHash {
private:
    // Number of virtual nodes per physical node
    int replicaCount;

    // Sorted hash ring
    std::map<size_t, std::string> ring;

    // Hash function
    std::hash<std::string> hasher;

public:
    ConsistentHash(int replicas = 100) : replicaCount(replicas) {}

    // Add a node to the hash ring
    void addNode(const std::string& node) {
        for (int i = 0; i < replicaCount; i++) {
            std::string virtualNode = node + "#" + std::to_string(i);
            size_t hash = hasher(virtualNode);
            ring[hash] = node;
        }
    }

    // Remove a node from the hash ring
    void removeNode(const std::string& node) {
        for (int i = 0; i < replicaCount; i++) {
            std::string virtualNode = node + "#" + std::to_string(i);
            size_t hash = hasher(virtualNode);
            ring.erase(hash);
        }
    }

    // Get the node responsible for a key
    std::string getNode(const std::string& key) {
        if (ring.empty()) {
            return "";
        }

        size_t hash = hasher(key);

        // Find the first node with hash >= key hash
        auto it = ring.lower_bound(hash);

        // If no such node, wrap around to the first node
        if (it == ring.end()) {
            it = ring.begin();
        }

        return it->second;
    }

    // Get distribution stats
    std::unordered_map<std::string, int> getDistribution(const std::vector<std::string>& keys) {
        std::unordered_map<std::string, int> distribution;

        for (const auto& key : keys) {
            std::string node = getNode(key);
            distribution[node]++;
        }

        return distribution;
    }
};
```

_Consistent hashing is used by distributed databases like Cassandra and DynamoDB to determine which node stores which key, minimizing data movement when the cluster size changes._

## 😂 Hash Table Jokes

- I told my friend I was implementing a perfect hash function. He said, "No collisions?" I replied, "No, that's a hash you can smoke."

- Two hash tables walk into a bar. The first one says, "I can find anything in constant time." The second one says, "That's nothing, I can do it in O(0.5)." The first one replies, "That's not even a real complexity class, you're definitely not a perfect hash."

- Why did the hash table go to therapy? It had too many collisions and was afraid of commitment to a single bucket.

- Hash tables are like exes—they both claim to offer O(1) lookup, but in the worst case, you still have to search through everyone they've ever been with.

- My hash function is so bad that when I hash "secure" and "password," they both return "admin1234".

- What's a cryptographer's favorite breakfast? A hash brown with no collisions.

## 📚 LeetCode Practice Problems

To reinforce today's advanced hash table concepts, try these LeetCode problems:

### Medium:

- #146: LRU Cache
- #380: Insert Delete GetRandom O(1)
- #981: Time Based Key-Value Store
- #1396: Design Underground System
- #1570: Dot Product of Two Sparse Vectors

### Hard:

- #460: LFU Cache
- #432: All O`one Data Structure
- #895: Maximum Frequency Stack
- #1206: Design Skiplist
- #1622: Fancy Sequence

_These problems will test your understanding of advanced hash table techniques and help you apply them to solve complex programming challenges._

## 🔮 Tomorrow's Preview:

Tomorrow we'll explore Sets & Maps in C++ STL, diving into the standard library implementations of hash tables and tree-based maps. We'll compare their performance characteristics, understand their internal workings, and learn when to use each variant for optimal performance.

_From building our own hash tables, we'll move to leveraging the power of battle-tested implementations in the C++ Standard Library—letting us focus on solving problems rather than reinventing the wheel._

#DSAin45 #Day12 #HashTables #CPlusPlus #DataStructures #CodingInterview
