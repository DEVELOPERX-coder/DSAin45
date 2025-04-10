# Real-World Applications of Hash Tables

## Database Systems

### Database Indexing
Hash indexes allow databases to find records in constant time:

```cpp
// Hash-indexed lookup in a database
Record* findRecord(Database* db, const std::string& key) {
    size_t bucketIndex = hashFunction(key) % db->numBuckets;
    
    // Follow the bucket to find the record
    Bucket* bucket = &db->buckets[bucketIndex];
    for (Record* record : bucket->records) {
        if (record->key == key) {
            return record;
        }
    }
    
    return nullptr; // Record not found
}
```

### Partitioning & Sharding
Distributed databases use hash functions to determine which server holds a specific piece of data:

```cpp
// Determine which database server to query
int getServerIndex(const std::string& key, int numServers) {
    return hashFunction(key) % numServers;
}
```

## Caching Systems

### Memory Caches
Systems like Redis and Memcached use hash tables for lightning-fast data access:

```cpp
class MemoryCache {
private:
    unordered_map<string, pair<string, time_t>> cache; // key -> (value, expiry)
    
public:
    void put(const string& key, const string& value, int ttlSeconds) {
        time_t expiry = time(nullptr) + ttlSeconds;
        cache[key] = {value, expiry};
    }
    
    optional<string> get(const string& key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return nullopt; // Cache miss
        }
        
        // Check if item has expired
        if (time(nullptr) > it->second.second) {
            cache.erase(it);
            return nullopt; // Expired
        }
        
        return it->second.first; // Cache hit
    }
};
```

### Web Page Caching
Browsers use hash tables to store recently visited web pages:

```cpp
// Browser cache lookup
CachedPage* lookupCache(const std::string& url) {
    return urlCache.find(url);
}
```

## Programming Languages

### Symbol Tables
Compilers and interpreters use hash tables to track variables and their attributes:

```cpp
class SymbolTable {
private:
    std::unordered_map<std::string, Symbol> symbols;
    SymbolTable* parent; // For scoping
    
public:
    void define(const std::string& name, const Symbol& symbol) {
        symbols[name] = symbol;
    }
    
    Symbol* resolve(const std::string& name) {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            return &it->second;
        }
        
        // Check parent scope if not found in current scope
        if (parent) {
            return parent->resolve(name);
        }
        
        return nullptr; // Not found in any scope
    }
};
```

### Method Dispatch
Dynamic languages use hash tables for method lookup:

```python
class DynamicObject:
    def __init__(self):
        self.methods = {}
        
    def add_method(self, name, func):
        self.methods[name] = func
        
    def call_method(self, name, *args):
        if name in self.methods:
            return self.methods[name](*args)
        raise AttributeError(f"No method named {name}")
```

## Network Systems

### IP Routing Tables
Routers use hash-based lookups to find the next hop for IP packets:

```cpp
struct RoutingEntry {
    IPAddress destination;
    IPAddress netmask;
    IPAddress nextHop;
};

IPAddress findNextHop(const IPAddress& destination) {
    // Find matching route using hash-based lookup
    for (const auto& route : routingTable) {
        if ((destination & route.netmask) == route.destination) {
            return route.nextHop;
        }
    }
    
    return defaultGateway;
}
```

### DNS Resolution
DNS servers cache domain name lookups using hash tables:

```cpp
// DNS cache lookup
IPAddress resolveDomain(const std::string& domain) {
    // Check cache first
    auto cachedIP = dnsCache.find(domain);
    if (cachedIP != nullptr && !isExpired(cachedIP)) {
        return cachedIP;
    }
    
    // Cache miss - perform actual DNS lookup
    IPAddress resolvedIP = performDNSLookup(domain);
    
    // Update cache
    dnsCache[domain] = {resolvedIP, currentTime() + TTL};
    
    return resolvedIP;
}
```

## Web Development

### Session Storage
Web applications use hash tables to maintain user sessions:

```javascript
class SessionManager {
    constructor() {
        this.sessions = new Map(); // sessionId -> userData
    }
    
    createSession(userId) {
        const sessionId = generateRandomId();
        this.sessions.set(sessionId, {
            userId: userId,
            createdAt: Date.now(),
            data: {}
        });
        return sessionId;
    }
    
    getSession(sessionId) {
        return this.sessions.get(sessionId);
    }
    
    updateSession(sessionId, key, value) {
        const session = this.sessions.get(sessionId);
        if (session) {
            session.data[key] = value;
            return true;
        }
        return false;
    }
}
```

### DOM Element Access
Browsers use hash tables to implement `getElementById()`:

```javascript
// Simplified browser implementation
function getElementById(id) {
    return documentElementsById.get(id);
}
```

## File Systems

### File Indexing
File systems use hash tables to quickly locate files by name:

```cpp
struct DirectoryEntry {
    std::string name;
    uint64_t inodeNumber;
};

uint64_t lookupFile(const std::string& name) {
    // Directory hash table lookup
    auto entry = directoryHashTable.find(name);
    if (entry) {
        return entry->inodeNumber;
    }
    return 0; // File not found
}
```

### Content-Addressable Storage
Systems like Git use hash tables to store and retrieve files by their content hash:

```cpp
class ContentAddressableStorage {
private:
    std::unordered_map<std::string, Blob> objects;
    
public:
    // Store a blob and return its hash
    std::string store(const std::vector<uint8_t>& data) {
        std::string hash = computeSHA1(data);
        objects[hash] = Blob(data);
        return hash;
    }
    
    // Retrieve a blob by its hash
    Blob* retrieve(const std::string& hash) {
        auto it = objects.find(hash);
        if (it != objects.end()) {
            return &it->second;
        }
        return nullptr;
    }
};
```

## Security Applications

### Password Storage
Systems store password hashes instead of plaintext passwords:

```cpp
class PasswordManager {
private:
    std::unordered_map<std::string, std::string> userPasswords; // username -> hashed password
    
public:
    void registerUser(const std::string& username, const std::string& password) {
        std::string salt = generateRandomSalt();
        std::string hashedPassword = hashPassword(password, salt);
        userPasswords[username] = salt + ":" + hashedPassword;
    }
    
    bool verifyPassword(const std::string& username, const std::string& password) {
        auto it = userPasswords.find(username);
        if (it == userPasswords.end()) {
            return false; // User not found
        }
        
        std::string storedData = it->second;
        size_t separatorPos = storedData.find(':');
        std::string salt = storedData.substr(0, separatorPos);
        std::string storedHash = storedData.substr(separatorPos + 1);
        
        std::string computedHash = hashPassword(password, salt);
        return computedHash == storedHash;
    }
};
```

### Bloom Filters
Used for efficient set membership testing with possibility of false positives:

```cpp
class BloomFilter {
private:
    std::vector<bool> bits;
    int numHashFunctions;
    
    std::vector<size_t> getHashPositions(const std::string& item) {
        std::vector<size_t> positions(numHashFunctions);
        for (int i = 0; i < numHashFunctions; i++) {
            positions[i] = hashFunction(item + std::to_string(i)) % bits.size();
        }
        return positions;
    }
    
public:
    BloomFilter(size_t size, int numHashes) : bits(size, false), numHashFunctions(numHashes) {}
    
    void add(const std::string& item) {
        for (size_t pos : getHashPositions(item)) {
            bits[pos] = true;
        }
    }
    
    bool mightContain(const std::string& item) {
        for (size_t pos : getHashPositions(item)) {
            if (!bits[pos]) {
                return false; // Definitely not in set
            }
        }
        return true; // Might be in set
    }
};
```

## Data Processing

### Duplicate Detection
Quickly identifying duplicate items in large datasets:

```cpp
std::vector<T> removeDuplicates(const std::vector<T>& items) {
    std::unordered_set<T> uniqueItems;
    std::vector<T> result;
    
    for (const T& item : items) {
        if (uniqueItems.insert(item).second) {
            // Item was inserted (not a duplicate)
            result.push_back(item);
        }
    }
    
    return result;
}
```

### Counting Frequencies
Efficiently counting occurrences of elements:

```cpp
std::unordered_map<T, int> countFrequencies(const std::vector<T>& items) {
    std::unordered_map<T, int> frequencies;
    
    for (const T& item : items) {
        frequencies[item]++;
    }
    
    return frequencies;
}
```

## Graphics & Gaming

### Spatial Hashing
Games use hash tables for efficient collision detection between objects:

```cpp
class SpatialHashGrid {
private:
    float cellSize;
    std::unordered_map<uint64_t, std::vector<GameObject*>> cells;
    
    uint64_t hashPosition(int x, int y) {
        return (static_cast<uint64_t>(x) << 32) | static_cast<uint64_t>(y);
    }
    
public:
    SpatialHashGrid(float cellSize) : cellSize(cellSize) {}
    
    void insertObject(GameObject* obj) {
        // Calculate grid cell
        int gridX = static_cast<int>(obj->x / cellSize);
        int gridY = static_cast<int>(obj->y / cellSize);
        
        // Add to appropriate cell
        uint64_t cellKey = hashPosition(gridX, gridY);
        cells[cellKey].push_back(obj);
    }
    
    std::vector<GameObject*> getPotentialCollisions(GameObject* obj) {
        // Calculate grid cell
        int gridX = static_cast<int>(obj->x / cellSize);
        int gridY = static_cast<int>(obj->y / cellSize);
        
        // Get objects in same cell
        uint64_t cellKey = hashPosition(gridX, gridY);
        return cells[cellKey];
    }
};
```

### Texture Caching
Graphics engines use hash tables to cache textures:

```cpp
class TextureCache {
private:
    std::unordered_map<std::string, Texture*> textures;
    
public:
    Texture* getTexture(const std::string& filename) {
        auto it = textures.find(filename);
        if (it != textures.end()) {
            return it->second; // Cache hit
        }
        
        // Cache miss - load texture
        Texture* newTexture = loadTextureFromFile(filename);
        textures[filename] = newTexture;
        return newTexture;
    }
};
```

## Machine Learning & AI

### Feature Hashing
Converting high-dimensional sparse features into fixed-size vectors:

```cpp
std::vector<double> hashFeatures(
    const std::unordered_map<std::string, double>& features, 
    int numDimensions
) {
    std::vector<double> result(numDimensions, 0.0);
    
    for (const auto& [feature, value] : features) {
        size_t hashValue = std::hash<std::string>{}(feature) % numDimensions;
        result[hashValue] += value;
    }
    
    return result;
}
```

### Cache for Memoization
Machine learning systems cache intermediate results:

```cpp
class NeuralNetworkCache {
private:
    std::unordered_map<std::string, std::vector<float>> layerOutputs;
    
public:
    void cacheOutput(const std::string& layerName, const std::vector<float>& output) {
        layerOutputs[layerName] = output;
    }
    
    std::vector<float>* getOutput(const std::string& layerName) {
        auto it = layerOutputs.find(layerName);
        if (it != layerOutputs.end()) {
            return &it->second;
        }
        return nullptr;
    }
};
```

## Operating Systems

### Process Table
OS keeps track of running processes in a hash table:

```cpp
class ProcessManager {
private:
    std::unordered_map<int, Process*> processes; // pid -> process
    
public:
    Process* createProcess() {
        Process* process = new Process();
        int pid = generateUniquePID();
        process->pid = pid;
        processes[pid] = process;
        return process;
    }
    
    Process* getProcess(int pid) {
        auto it = processes.find(pid);
        if (it != processes.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    void terminateProcess(int pid) {
        auto it = processes.find(pid);
        if (it != processes.end()) {
            delete it->second;
            processes.erase(it);
        }
    }
};
```

### Memory Management
Virtual memory systems use hash tables for page tables:

```cpp
struct PageTableEntry {
    uint64_t physicalPage;
    bool present;
    bool readOnly;
    // Other flags...
};

PageTableEntry* lookupPage(uint64_t virtualAddress) {
    uint64_t virtualPage = virtualAddress >> PAGE_SHIFT;
    return pageTable.find(virtualPage);
}
```

## Industry-Specific Applications

### E-commerce
Product catalogs and inventory management use hash tables:

```cpp
class ProductCatalog {
private:
    std::unordered_map<std::string, Product> productsBySKU;
    
public:
    void addProduct(const Product& product) {
        productsBySKU[product.sku] = product;
    }
    
    Product* findProduct(const std::string& sku) {
        auto it = productsBySKU.find(sku);
        if (it != productsBySKU.end()) {
            return &it->second;
        }
        return nullptr;
    }
};
```

### Financial Systems
High-frequency trading platforms use hash tables for order books:

```cpp
class OrderBook {
private:
    std::unordered_map<std::string, Order> ordersById;
    
public:
    void addOrder(const Order& order) {
        ordersById[order.id] = order;
    }
    
    void cancelOrder(const std::string& orderId) {
        ordersById.erase(orderId);
    }
    
    Order* findOrder(const std::string& orderId) {
        auto it = ordersById.find(orderId);
        if (it != ordersById.end()) {
            return &it->second;
        }
        return nullptr;
    }
};
```

### Healthcare
Patient record systems rely on hash-based lookups:

```cpp
class PatientDatabase {
private:
    std::unordered_map<std::string, PatientRecord> patientsByID;
    
public:
    void addPatient(const PatientRecord& patient) {
        patientsByID[patient.patientID] = patient;
    }
    
    PatientRecord* findPatient(const std::string& patientID) {
        auto it = patientsByID.find(patientID);
        if (it != patientsByID.end()) {
            return &it->second;
        }
        return nullptr;
    }
};
```

## Common Hash Table Interview Problems

### Two Sum Problem
Find two numbers that add up to a target value:

```cpp
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> numToIndex;
    
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        
        if (numToIndex.find(complement) != numToIndex.end()) {
            return {numToIndex[complement], i};
        }
        
        numToIndex[nums[i]] = i;
    }
    
    return {}; // No solution
}
```

### First Non-Repeating Character
Find the first character in a string that appears only once:

```cpp
char firstNonRepeatingChar(const std::string& s) {
    std::unordered_map<char, int> charCounts;
    
    // Count occurrences of each character
    for (char c : s) {
        charCounts[c]++;
    }
    
    // Find first character with count 1
    for (char c : s) {
        if (charCounts[c] == 1) {
            return c;
        }
    }
    
    return '\0'; // No non-repeating character
}
```

### LRU Cache
Implement a Least Recently Used cache with O(1) operations:

```cpp
class LRUCache {
private:
    int capacity;
    std::list<std::pair<int, int>> items; // key-value pairs, most recent at front
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache; // key -> iterator
    
public:
    LRUCache(int capacity) : capacity(capacity) {}
    
    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return -1; // Key not found
        }
        
        // Move accessed item to front (most recently used)
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
        
        // Add new key-value pair
        if (items.size() == capacity) {
            // Cache is full, remove least recently used item
            int lru_key = items.back().first;
            items.pop_back();
            cache.erase(lru_key);
        }
        
        items.push_front({key, value});
        cache[key] = items.begin();
    }
};
```

### Group Anagrams
Group words by their character counts:

```cpp
std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;
    
    for (const std::string& s : strs) {
        // Create a signature for the string (sorted characters)
        std::string signature = s;
        std::sort(signature.begin(), signature.end());
        
        // Add to appropriate group
        groups[signature].push_back(s);
    }
    
    // Convert map to result vector
    std::vector<std::vector<std::string>> result;
    for (const auto& [signature, group] : groups) {
        result.push_back(group);
    }
    
    return result;
}
```

### Longest Substring Without Repeating Characters
Find the length of the longest substring without repeating characters:

```cpp
int lengthOfLongestSubstring(const std::string& s) {
    std::unordered_map<char, int> charToIndex;
    int maxLength = 0;
    int start = 0;
    
    for (int i = 0; i < s.length(); i++) {
        if (charToIndex.find(s[i]) != charToIndex.end() && charToIndex[s[i]] >= start) {
            // Character repeats within current window
            start = charToIndex[s[i]] + 1;
        }
        
        charToIndex[s[i]] = i;
        maxLength = std::max(maxLength, i - start + 1);
    }
    
    return maxLength;
}
```
