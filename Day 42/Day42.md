# Day 42: Weekly Challenge & Advanced Problems #DSAin45

## Integrated Algorithm Challenges

After covering a wide range of data structures and algorithms over the past six weeks, it's time to apply this knowledge to more complex, multi-algorithm problems. Real-world software engineering often requires combining multiple algorithmic approaches to craft efficient solutions.

## Challenge 1: Efficient File Indexing System

**Problem Statement:** Design a system that can:

1. Index a large collection of text files
2. Support fast substring searches across all files
3. Return results ranked by relevance
4. Handle incremental updates as files change

### Breaking Down the Problem

This problem requires combining several data structures and algorithms:

1. **String Matching**: For searching text within files
2. **Indexing Structure**: For quickly locating files containing specific terms
3. **Ranking Algorithm**: For sorting results by relevance
4. **Update Mechanism**: For efficiently handling changes

### Solution Approach

```cpp
// Core data structures
struct FileIndex {
    // Inverted index: term -> {file_id, positions[]}
    unordered_map<string, unordered_map<int, vector<int>>> index;

    // Document metadata
    unordered_map<int, string> fileNames;
    unordered_map<int, size_t> fileSizes;

    // Term frequency across all documents
    unordered_map<string, int> termFrequency;
};

// Index a single file
void indexFile(FileIndex& idx, int fileId, const string& content) {
    // Tokenize content into terms
    vector<string> terms = tokenize(content);

    // Build inverted index
    for (int pos = 0; pos < terms.size(); pos++) {
        string term = terms[pos];

        // Update term position for this file
        idx.index[term][fileId].push_back(pos);

        // Update term frequency
        idx.termFrequency[term]++;
    }

    // Store file metadata
    idx.fileSizes[fileId] = content.size();
}

// Search for a query and rank results
vector<pair<int, double>> search(FileIndex& idx, const string& query) {
    vector<string> queryTerms = tokenize(query);

    // Documents matching at least one term
    unordered_map<int, double> scores;

    // Calculate TF-IDF scores
    for (const string& term : queryTerms) {
        if (idx.index.find(term) != idx.index.end()) {
            // Calculate IDF (Inverse Document Frequency)
            int docFreq = idx.index[term].size();
            double idf = log10((double)idx.fileNames.size() / docFreq);

            // Calculate score contribution for each file
            for (const auto& [fileId, positions] : idx.index[term]) {
                // Term frequency in this document
                double tf = (double)positions.size() / idx.fileSizes[fileId];

                // Accumulate TF-IDF score
                scores[fileId] += tf * idf;
            }
        }
    }

    // Convert to sortable vector
    vector<pair<int, double>> results;
    for (const auto& [fileId, score] : scores) {
        results.push_back({fileId, score});
    }

    // Sort by descending score
    sort(results.begin(), results.end(),
         [](const pair<int, double>& a, const pair<int, double>& b) {
             return a.second > b.second;
         });

    return results;
}
```

### Time & Space Complexity Analysis

- **Indexing**: O(n) time where n is total content size
- **Search**: O(q + r log r) where q is query size and r is number of results
- **Space**: O(n) for the inverted index

### Optimization Techniques

1. **Use suffix trees or arrays** for more efficient substring search
2. **Implement positional information** to support phrase queries
3. **Use a bloom filter** as a pre-filter to quickly eliminate non-matching documents
4. **Adopt a trie data structure** for prefix-based searches
5. **Implement incremental updates** by tracking document versions

## Challenge 2: Route Optimization with Constraints

**Problem Statement:** Design an algorithm to find the optimal route for a delivery vehicle that:

1. Must visit multiple locations
2. Has time window constraints at each location
3. Has a limited capacity for packages
4. Needs to minimize total distance traveled

### Breaking Down the Problem

This combines several challenging problems:

1. **Traveling Salesman Problem (TSP)**: Finding the shortest route visiting all points
2. **Constraint Satisfaction**: Meeting time windows
3. **Knapsack Problem**: Optimal loading under capacity constraints

### Solution Approach

```cpp
struct Location {
    int id;
    double x, y;
    int packageWeight;
    int earliestArrival;  // Time window start
    int latestArrival;    // Time window end
};

struct Route {
    vector<int> locationOrder;
    int totalDistance;
    int totalTime;
    bool feasible;
};

// Calculate distance between two locations
int distance(const Location& a, const Location& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Check if a route is feasible given constraints
bool isFeasible(const vector<Location>& locations, const vector<int>& route,
                int vehicleCapacity) {
    int currentTime = 0;
    int currentLoad = 0;
    int prevLocation = 0;  // Start at depot

    for (int locId : route) {
        const Location& loc = locations[locId];

        // Add travel time from previous location
        currentTime += distance(locations[prevLocation], loc);

        // Check if we arrive too late
        if (currentTime > loc.latestArrival) {
            return false;
        }

        // Wait if we arrive too early
        currentTime = max(currentTime, loc.earliestArrival);

        // Add package to vehicle
        currentLoad += loc.packageWeight;

        // Check capacity constraint
        if (currentLoad > vehicleCapacity) {
            return false;
        }

        // Service time at location (constant for simplicity)
        currentTime += 10;
        prevLocation = locId;
    }

    return true;
}

// Simulated Annealing algorithm for route optimization
Route optimizeRoute(const vector<Location>& locations, int vehicleCapacity) {
    int n = locations.size() - 1;  // Excluding depot

    // Initialize with a simple route: 1, 2, 3, ..., n
    vector<int> currentRoute(n);
    for (int i = 0; i < n; i++) {
        currentRoute[i] = i + 1;  // Location IDs start from 1 (0 is depot)
    }

    // Evaluate initial route
    Route best;
    best.locationOrder = currentRoute;
    best.totalDistance = calculateTotalDistance(locations, currentRoute);
    best.feasible = isFeasible(locations, currentRoute, vehicleCapacity);

    // Simulated annealing parameters
    double temperature = 1000.0;
    double coolingRate = 0.995;
    Route current = best;

    while (temperature > 1.0) {
        // Create a neighbor by swapping two locations
        vector<int> newRoute = current.locationOrder;
        int i = rand() % n;
        int j = rand() % n;
        swap(newRoute[i], newRoute[j]);

        // Evaluate new route
        int newDistance = calculateTotalDistance(locations, newRoute);
        bool newFeasible = isFeasible(locations, newRoute, vehicleCapacity);

        // Decide whether to accept new solution
        bool accept = false;

        if (newFeasible && (!current.feasible || newDistance < current.totalDistance)) {
            // Always accept better feasible solutions
            accept = true;
        } else if (newFeasible == current.feasible) {
            // Accept worse solutions with probability based on temperature
            double acceptProbability = exp((current.totalDistance - newDistance) / temperature);
            if ((double)rand() / RAND_MAX < acceptProbability) {
                accept = true;
            }
        }

        if (accept) {
            current.locationOrder = newRoute;
            current.totalDistance = newDistance;
            current.feasible = newFeasible;

            // Update best solution
            if (current.feasible &&
                (!best.feasible || current.totalDistance < best.totalDistance)) {
                best = current;
            }
        }

        // Cool down
        temperature *= coolingRate;
    }

    return best;
}
```

### Time & Space Complexity Analysis

- **Time**: O(I \* n²) where I is the number of iterations and n is the number of locations
- **Space**: O(n) for storing routes and location data

### Optimization Techniques

1. **Use A\* search** for finding routes between points if using a real map
2. **Implement tabu search** to avoid revisiting recent solutions
3. **Apply constraint propagation** to prune infeasible routes early
4. **Use dynamic programming** for small instances
5. **Consider genetic algorithms** for large problem instances

## Challenge 3: Real-Time Network Traffic Analysis

**Problem Statement:** Design a system that:

1. Monitors network traffic in real-time
2. Identifies potential security threats
3. Maintains statistics on traffic patterns
4. Can scale to handle high throughput

### Breaking Down the Problem

This problem combines:

1. **Streaming Algorithms**: Processing continuous data
2. **Pattern Matching**: Identifying suspicious patterns
3. **Probabilistic Data Structures**: Efficiently counting frequent items
4. **Parallel Processing**: Handling high throughput

### Solution Approach

```cpp
// Core data structures
struct TrafficAnalyzer {
    // Count-Min Sketch for approximate frequency counting
    vector<vector<int>> countMinSketch;
    vector<int> hashParams;

    // Bloom filter for suspicious IP address detection
    vector<bool> bloomFilter;
    vector<int> bloomHashParams;

    // Sliding window for recent events
    deque<NetworkPacket> recentPackets;

    // Anomaly detection thresholds
    int packetRateThreshold;
    int connectionRateThreshold;
};

// Initialize analyzer with appropriate dimensions
TrafficAnalyzer initAnalyzer(int width, int depth, int bloomSize) {
    TrafficAnalyzer analyzer;

    // Initialize Count-Min Sketch
    analyzer.countMinSketch.resize(depth, vector<int>(width, 0));
    analyzer.hashParams.resize(depth);
    for (int i = 0; i < depth; i++) {
        analyzer.hashParams[i] = rand() % 999983; // Large prime
    }

    // Initialize Bloom Filter
    analyzer.bloomFilter.resize(bloomSize, false);
    analyzer.bloomHashParams.resize(5); // Use 5 hash functions
    for (int i = 0; i < 5; i++) {
        analyzer.bloomHashParams[i] = rand() % 999983; // Large prime
    }

    // Set thresholds
    analyzer.packetRateThreshold = 1000;
    analyzer.connectionRateThreshold = 100;

    return analyzer;
}

// Process a new packet
void processPacket(TrafficAnalyzer& analyzer, const NetworkPacket& packet) {
    // Add to sliding window
    analyzer.recentPackets.push_back(packet);

    // Remove old packets (maintain 60-second window)
    while (!analyzer.recentPackets.empty() &&
           analyzer.recentPackets.front().timestamp + 60 < packet.timestamp) {
        analyzer.recentPackets.pop_front();
    }

    // Update Count-Min Sketch for source IP
    string sourceIP = packet.sourceIP;
    for (int i = 0; i < analyzer.countMinSketch.size(); i++) {
        size_t hash = hashFunction(sourceIP, analyzer.hashParams[i]) % analyzer.countMinSketch[i].size();
        analyzer.countMinSketch[i][hash]++;
    }

    // Check for suspicious activity
    if (estimateFrequency(analyzer, sourceIP) > analyzer.packetRateThreshold) {
        // Add to bloom filter of suspicious IPs
        for (int i = 0; i < analyzer.bloomHashParams.size(); i++) {
            size_t hash = hashFunction(sourceIP, analyzer.bloomHashParams[i]) % analyzer.bloomFilter.size();
            analyzer.bloomFilter[hash] = true;
        }

        // Trigger alert
        triggerAlert("High packet rate", sourceIP, packet.timestamp);
    }

    // Record connection (sourceIP:sourcePort -> destIP:destPort)
    string connection = sourceIP + ":" + to_string(packet.sourcePort) + "->" +
                       packet.destIP + ":" + to_string(packet.destPort);

    // Update connection frequency
    for (int i = 0; i < analyzer.countMinSketch.size(); i++) {
        size_t hash = hashFunction(connection, analyzer.hashParams[i]) % analyzer.countMinSketch[i].size();
        analyzer.countMinSketch[i][hash]++;
    }
}

// Estimate frequency of an item using Count-Min Sketch
int estimateFrequency(const TrafficAnalyzer& analyzer, const string& item) {
    int minCount = INT_MAX;

    for (int i = 0; i < analyzer.countMinSketch.size(); i++) {
        size_t hash = hashFunction(item, analyzer.hashParams[i]) % analyzer.countMinSketch[i].size();
        minCount = min(minCount, analyzer.countMinSketch[i][hash]);
    }

    return minCount;
}

// Check if an IP is potentially suspicious
bool isSuspicious(const TrafficAnalyzer& analyzer, const string& ip) {
    for (int i = 0; i < analyzer.bloomHashParams.size(); i++) {
        size_t hash = hashFunction(ip, analyzer.bloomHashParams[i]) % analyzer.bloomFilter.size();
        if (!analyzer.bloomFilter[hash]) {
            return false;
        }
    }

    return true; // Potentially suspicious (might be false positive)
}
```

### Time & Space Complexity Analysis

- **Process Packet**: O(1) per packet
- **Space**: O(w \* d + b + r) where w is sketch width, d is sketch depth, b is bloom filter size, and r is recent packet window size

### Optimization Techniques

1. **Use HyperLogLog** for cardinality estimation
2. **Implement Count-Sketch** for more accurate frequency estimation
3. **Apply reservoir sampling** for keeping representative packet samples
4. **Utilize multiple hash functions** to reduce collisions
5. **Shard the data** across multiple analyzers for parallel processing

## Challenge 4: Distributed Cache with Smart Eviction

**Problem Statement:** Design a distributed in-memory cache that:

1. Efficiently stores and retrieves key-value pairs
2. Implements smart eviction policies
3. Distributes data across multiple nodes
4. Handles node failures gracefully

### Breaking Down the Problem

This problem involves:

1. **Hashing Algorithms**: For data distribution
2. **Cache Eviction Policies**: LRU, LFU, etc.
3. **Distributed Systems**: Consistent hashing, replication
4. **Fault Tolerance**: Handling node failures

### Solution Approach

```cpp
// Core data structures
struct CacheEntry {
    string key;
    string value;
    int frequency;
    int lastAccess;

    CacheEntry(string k, string v) :
        key(k), value(v), frequency(1), lastAccess(time(nullptr)) {}
};

struct CacheNode {
    int id;
    unordered_map<string, CacheEntry> entries;
    int capacity;
    string evictionPolicy;

    CacheNode(int nodeId, int cap, string policy) :
        id(nodeId), capacity(cap), evictionPolicy(policy) {}
};

// Consistent hashing ring
struct ConsistentHashRing {
    // Virtual nodes mapped to positions on the ring
    map<size_t, int> ring;
    int numReplicas;

    ConsistentHashRing(int replicas) : numReplicas(replicas) {}

    // Add a node to the ring
    void addNode(int nodeId) {
        for (int i = 0; i < numReplicas; i++) {
            size_t hash = hashFunction(to_string(nodeId) + ":" + to_string(i));
            ring[hash] = nodeId;
        }
    }

    // Remove a node from the ring
    void removeNode(int nodeId) {
        for (int i = 0; i < numReplicas; i++) {
            size_t hash = hashFunction(to_string(nodeId) + ":" + to_string(i));
            ring.erase(hash);
        }
    }

    // Get the node responsible for a key
    int getNode(const string& key) {
        if (ring.empty()) {
            throw runtime_error("Ring is empty");
        }

        size_t hash = hashFunction(key);
        auto it = ring.lower_bound(hash);

        if (it == ring.end()) {
            // Wrap around to the first node
            return ring.begin()->second;
        }

        return it->second;
    }
};

// Distributed cache implementation
struct DistributedCache {
    vector<CacheNode> nodes;
    ConsistentHashRing ring;
    int clockTick;

    DistributedCache(int numNodes, int nodeCapacity, string policy) :
        ring(ConsistentHashRing(100)), clockTick(0) {

        // Initialize nodes
        for (int i = 0; i < numNodes; i++) {
            nodes.push_back(CacheNode(i, nodeCapacity, policy));
            ring.addNode(i);
        }
    }

    // Get value for a key
    optional<string> get(const string& key) {
        int nodeId = ring.getNode(key);
        CacheNode& node = nodes[nodeId];

        auto it = node.entries.find(key);
        if (it == node.entries.end()) {
            return nullopt;
        }

        // Update access statistics
        it->second.frequency++;
        it->second.lastAccess = ++clockTick;

        return it->second.value;
    }

    // Put a key-value pair
    void put(const string& key, const string& value) {
        int nodeId = ring.getNode(key);
        CacheNode& node = nodes[nodeId];

        // Check if we need to evict
        if (node.entries.size() >= node.capacity &&
            node.entries.find(key) == node.entries.end()) {
            evict(node);
        }

        // Add or update entry
        if (node.entries.find(key) != node.entries.end()) {
            node.entries[key].value = value;
            node.entries[key].frequency++;
            node.entries[key].lastAccess = ++clockTick;
        } else {
            node.entries[key] = CacheEntry(key, value);
            node.entries[key].lastAccess = ++clockTick;
        }
    }

    // Evict an entry based on policy
    void evict(CacheNode& node) {
        if (node.evictionPolicy == "LRU") {
            evictLRU(node);
        } else if (node.evictionPolicy == "LFU") {
            evictLFU(node);
        } else {
            // Default to random eviction
            evictRandom(node);
        }
    }

    // LRU eviction
    void evictLRU(CacheNode& node) {
        string lruKey;
        int minAccess = INT_MAX;

        for (const auto& [key, entry] : node.entries) {
            if (entry.lastAccess < minAccess) {
                minAccess = entry.lastAccess;
                lruKey = key;
            }
        }

        node.entries.erase(lruKey);
    }

    // LFU eviction
    void evictLFU(CacheNode& node) {
        string lfuKey;
        int minFreq = INT_MAX;
        int minAccess = INT_MAX;

        for (const auto& [key, entry] : node.entries) {
            if (entry.frequency < minFreq ||
                (entry.frequency == minFreq && entry.lastAccess < minAccess)) {
                minFreq = entry.frequency;
                minAccess = entry.lastAccess;
                lfuKey = key;
            }
        }

        node.entries.erase(lfuKey);
    }

    // Random eviction
    void evictRandom(CacheNode& node) {
        int index = rand() % node.entries.size();
        auto it = node.entries.begin();
        advance(it, index);
        node.entries.erase(it);
    }

    // Handle node failure
    void handleNodeFailure(int nodeId) {
        // Remove node from ring
        ring.removeNode(nodeId);

        // Redistribute data
        auto entries = nodes[nodeId].entries;
        nodes[nodeId].entries.clear();

        for (const auto& [key, entry] : entries) {
            put(key, entry.value);
        }
    }
};
```

### Time & Space Complexity Analysis

- **Get/Put Operations**: O(1) average case
- **Node Failure Handling**: O(n) where n is the number of keys on the failed node
- **Space**: O(m \* k) where m is the number of nodes and k is the number of keys per node

### Optimization Techniques

1. **Implement adaptive replication** based on access patterns
2. **Use a segmented LRU** for better eviction decisions
3. **Apply bloom filters** for quick exists checks
4. **Implement read-through and write-through** for persistence
5. **Use proxy nodes** for better load balancing

## Key Learning Points

### Algorithm Selection

- **No perfect algorithm exists** for all situations
- **Consider multiple constraints**: time, space, scalability, maintainability
- **Hybrid approaches** often outperform pure algorithms for complex problems

### Performance Optimization

- **Measure before optimizing**: use profiling to find bottlenecks
- **Space-time tradeoffs**: sometimes using more memory dramatically improves speed
- **Approximation algorithms**: sometimes an approximate solution is good enough and much faster

### Problem Decomposition

- **Break complex problems** into simpler subproblems
- **Identify which algorithms** fit each subproblem
- **Consider data flow** between components

## Practice Problems

### Problem 1: Implement a Web Crawler

Design a web crawler that efficiently traverses web pages, respects robots.txt, avoids revisiting pages, and handles redirects properly.

**Approach Hint**: Use a combination of BFS for traversal, bloom filters for duplicate detection, and a priority queue for scheduling based on importance.

### Problem 2: Design a Recommendation System

Create a system that recommends items to users based on their past behavior and similarities to other users.

**Approach Hint**: Use collaborative filtering with matrix factorization, implement LSH for finding similar users quickly, and use priority queues for top-K recommendations.

### Problem 3: Implement a Distributed Task Scheduler

Design a system that distributes tasks across multiple workers, handles failures, and ensures tasks are completed exactly once.

**Approach Hint**: Use consistent hashing for task distribution, implement a priority queue for scheduling, and use a two-phase commit protocol for ensuring task completion.

## What's Next

As we wrap up the DSA journey, the next days will focus on system design fundamentals and interview preparation strategies. We'll learn how to apply our algorithm knowledge to design scalable systems and tackle technical interviews with confidence.

## Key Takeaways

- Complex real-world problems require combining multiple algorithms and data structures
- Understanding the tradeoffs between different approaches is crucial
- Approximation algorithms and probabilistic data structures can provide efficient solutions
- Always analyze both time and space complexity to make informed decisions
- Distributed algorithms introduce additional challenges but enable scaling

#DSAin45 #AdvancedAlgorithms #SystemDesign #OptimizationTechniques #ProblemSolving
