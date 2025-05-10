# Day 43: System Design Basics #DSAin45

## Why System Design Matters

System design is where your DSA knowledge meets real-world scalability challenges. Understanding how to architect systems that handle millions of users, petabytes of data, and thousands of requests per second requires:

1. **Algorithm Selection**: Choosing the right algorithms for different components
2. **Data Structure Design**: Optimizing for distributed environments
3. **Scalability Patterns**: Building systems that grow efficiently
4. **Trade-off Analysis**: Balancing consistency, availability, and partition tolerance

## System Design Principles

### 1. Scalability

**Horizontal Scaling (Scale-Out)**

- Add more machines to handle increased load
- Requires distributed algorithms and data structures
- Better fault tolerance, but increases complexity

**Vertical Scaling (Scale-Up)**

- Add more resources to existing machines
- Simpler to implement initially
- Limited by hardware constraints

### 2. Reliability

**Redundancy**

- Multiple copies of data and services
- Algorithm: Consensus protocols (Raft, Paxos)
- Data Structure: Replicated state machines

**Fault Tolerance**

- System continues working despite failures
- Algorithm: Circuit breakers, retry with exponential backoff
- Data Structure: Gossip protocols for failure detection

### 3. Availability

**Load Balancing**

- Distribute traffic across multiple servers
- Algorithm: Round-robin, least-connections, consistent hashing
- Data Structure: Weighted heap for server selection

**Caching**

- Reduce latency by storing frequently accessed data
- Algorithm: LRU, LFU, FIFO eviction policies
- Data Structure: Hash tables with ordered lists

### 4. Consistency vs. Availability (CAP Theorem)

You can only guarantee two of three:

- **Consistency**: All nodes see the same data at the same time
- **Availability**: Every request receives a response
- **Partition tolerance**: System continues operating despite network failures

## DSA Applications in System Design

### 1. Consistent Hashing for Distributed Systems

```cpp
// Consistent hashing implementation
class ConsistentHash {
private:
    std::map<uint64_t, std::string> ring;
    int numReplicas;

    uint64_t hash(const std::string& key) {
        // Using 64-bit hash for better distribution
        return hashFunction(key);
    }

public:
    ConsistentHash(int replicas = 100) : numReplicas(replicas) {}

    void addNode(const std::string& nodeId) {
        for (int i = 0; i < numReplicas; i++) {
            std::string replicaKey = nodeId + ":" + std::to_string(i);
            ring[hash(replicaKey)] = nodeId;
        }
    }

    void removeNode(const std::string& nodeId) {
        for (int i = 0; i < numReplicas; i++) {
            std::string replicaKey = nodeId + ":" + std::to_string(i);
            ring.erase(hash(replicaKey));
        }
    }

    std::string getNode(const std::string& key) {
        if (ring.empty()) return "";

        uint64_t keyHash = hash(key);

        // Find first node with hash >= keyHash
        auto it = ring.lower_bound(keyHash);

        if (it == ring.end()) {
            // Wrap around to the beginning
            return ring.begin()->second;
        }

        return it->second;
    }
};
```

**Why it works**:

- Time Complexity: O(log n) for lookups
- Space Complexity: O(n × replicas)
- Minimizes data movement when nodes are added/removed

### 2. Bloom Filters for Distributed Cache

```cpp
// Bloom filter for cache miss prevention
class BloomFilter {
private:
    std::vector<bool> bits;
    int numHashes;
    std::vector<uint64_t> hashSeeds;

public:
    BloomFilter(int size, int hashes) : bits(size, false), numHashes(hashes) {
        // Initialize hash seeds
        std::random_device rd;
        for (int i = 0; i < hashes; i++) {
            hashSeeds.push_back(rd());
        }
    }

    void add(const std::string& item) {
        for (int i = 0; i < numHashes; i++) {
            size_t index = hashWithSeed(item, hashSeeds[i]) % bits.size();
            bits[index] = true;
        }
    }

    bool mightContain(const std::string& item) {
        for (int i = 0; i < numHashes; i++) {
            size_t index = hashWithSeed(item, hashSeeds[i]) % bits.size();
            if (!bits[index]) {
                return false;  // Definitely not in the set
            }
        }
        return true;  // Might be in the set
    }
};
```

**Applications**:

- Distributed cache: Check if an item might be cached before expensive lookup
- Database: Check if a key might exist before disk read
- Network: Reduce unnecessary network calls

### 3. Sharding Strategies

```cpp
// Shard key generator with multiple strategies
class ShardManager {
private:
    int numShards;
    ShardStrategy strategy;
    std::unordered_map<std::string, int> customShardMap;

public:
    enum ShardStrategy {
        HASH_BASED,      // Simple hash modulo
        RANGE_BASED,     // Based on key ranges
        DIRECTORY_BASED, // Central lookup table
        CONSISTENT_HASH  // Uses consistent hashing
    };

    ShardManager(int shards, ShardStrategy strat)
        : numShards(shards), strategy(strat) {}

    int getShardForKey(const std::string& key) {
        switch (strategy) {
            case HASH_BASED:
                return hashFunction(key) % numShards;

            case RANGE_BASED:
                return getRangeBasedShard(key);

            case DIRECTORY_BASED:
                return getDirectoryBasedShard(key);

            case CONSISTENT_HASH:
                return getConsistentHashShard(key);
        }
    }

private:
    int getRangeBasedShard(const std::string& key) {
        // Simple range-based sharding
        if (key[0] >= 'a' && key[0] <= 'f') return 0;
        if (key[0] >= 'g' && key[0] <= 'l') return 1;
        if (key[0] >= 'm' && key[0] <= 'r') return 2;
        return 3;  // s-z
    }

    int getDirectoryBasedShard(const std::string& key) {
        // Check custom shard mapping
        auto it = customShardMap.find(key);
        if (it != customShardMap.end()) {
            return it->second;
        }

        // Default to hash-based
        return hashFunction(key) % numShards;
    }
};
```

## Common System Design Patterns

### 1. Load Balancing Algorithms

```cpp
// Load balancer with multiple strategies
class LoadBalancer {
private:
    std::vector<Server> servers;
    std::atomic<int> roundRobinCounter;

public:
    enum Strategy {
        ROUND_ROBIN,
        LEAST_CONNECTIONS,
        LEAST_RESPONSE_TIME,
        WEIGHTED_RESPONSE_TIME
    };

    Server* selectServer(Strategy strategy) {
        switch (strategy) {
            case ROUND_ROBIN:
                return selectRoundRobin();

            case LEAST_CONNECTIONS:
                return selectLeastConnections();

            case LEAST_RESPONSE_TIME:
                return selectLeastResponseTime();

            case WEIGHTED_RESPONSE_TIME:
                return selectWeightedResponseTime();
        }
    }

private:
    Server* selectRoundRobin() {
        int index = roundRobinCounter.fetch_add(1) % servers.size();
        return &servers[index];
    }

    Server* selectLeastConnections() {
        // Priority queue for finding server with least connections
        auto comp = [](const Server& a, const Server& b) {
            return a.activeConnections > b.activeConnections;
        };

        return &(*std::min_element(servers.begin(), servers.end(), comp));
    }
};
```

### 2. Rate Limiting

```cpp
// Token bucket rate limiter
class RateLimiter {
private:
    long capacity;         // Maximum tokens
    long tokens;          // Current tokens
    long refillRate;      // Tokens per second
    std::chrono::steady_clock::time_point lastRefill;
    mutable std::mutex mutex;

public:
    RateLimiter(long cap, long rate)
        : capacity(cap), tokens(cap), refillRate(rate),
          lastRefill(std::chrono::steady_clock::now()) {}

    bool tryAcquire(long numTokens = 1) {
        std::lock_guard<std::mutex> lock(mutex);

        refillTokens();

        if (tokens >= numTokens) {
            tokens -= numTokens;
            return true;
        }

        return false;
    }

private:
    void refillTokens() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
                       (now - lastRefill).count();

        long newTokens = (elapsed * refillRate) / 1000;

        if (newTokens > 0) {
            tokens = std::min(capacity, tokens + newTokens);
            lastRefill = now;
        }
    }

    // Leaky bucket implementation for comparison
    class LeakyBucket {
    private:
        std::queue<std::chrono::steady_clock::time_point> requests;
        long capacity;
        long leakRate;  // Requests per second
        mutable std::mutex mutex;

    public:
        LeakyBucket(long cap, long rate) : capacity(cap), leakRate(rate) {}

        bool tryAcquire() {
            std::lock_guard<std::mutex> lock(mutex);

            auto now = std::chrono::steady_clock::now();

            // Leak old requests
            while (!requests.empty()) {
                auto oldest = requests.front();
                auto age = std::chrono::duration_cast<std::chrono::seconds>
                          (now - oldest).count();

                if (age * leakRate >= requests.size()) {
                    break;
                }

                requests.pop();
            }

            // Check if we can add new request
            if (requests.size() < capacity) {
                requests.push(now);
                return true;
            }

            return false;
        }
    };
};
```

### 3. Distributed Cache with Consistency

```cpp
// Distributed cache with eventual consistency
class DistributedCache {
private:
    // Local cache on each node
    LRUCache localCache;

    // Remote node connections
    std::unordered_map<std::string, Node*> remoteNodes;

    // Consistent hashing for key distribution
    ConsistentHash consistentHash;

    // Vector clocks for conflict resolution
    std::unordered_map<std::string, VectorClock> vectorClocks;

public:
    // Get with read-through semantics
    std::optional<std::string> get(const std::string& key) {
        // Check local cache first
        auto localResult = localCache.get(key);
        if (localResult) {
            return localResult;
        }

        // Find responsible node
        std::string nodeId = consistentHash.getNode(key);

        if (nodeId == getCurrentNodeId()) {
            // We are responsible - check persistent storage
            return getFromStorage(key);
        } else {
            // Forward to responsible node
            Node* node = remoteNodes[nodeId];
            return node->get(key);
        }
    }

    // Put with write-through and eventual consistency
    void put(const std::string& key, const std::string& value) {
        // Update local cache
        localCache.put(key, value);

        // Update vector clock
        vectorClocks[key].increment(getCurrentNodeId());

        // Find responsible node
        std::string nodeId = consistentHash.getNode(key);

        // Write to primary
        if (nodeId == getCurrentNodeId()) {
            writeToStorage(key, value, vectorClocks[key]);
        } else {
            Node* node = remoteNodes[nodeId];
            node->put(key, value, vectorClocks[key]);
        }

        // Async replication to replicas
        replicateAsync(key, value, vectorClocks[key]);
    }

private:
    void replicateAsync(const std::string& key,
                       const std::string& value,
                       const VectorClock& clock) {
        // Find replica nodes
        auto replicas = getReplicaNodes(key);

        // Async replication
        for (auto* replica : replicas) {
            std::async(std::launch::async, [=] {
                try {
                    replica->replicate(key, value, clock);
                } catch (const std::exception& e) {
                    logError("Replication failed", key, e.what());
                    // Add to retry queue
                    addToRetryQueue(key, value, clock);
                }
            });
        }
    }
};
```

## Real-World Case Studies

### 1. URL Shortener System

**Requirements**:

- Generate short URLs for long URLs
- Redirect short URLs to original URLs
- Handle 100M+ URLs
- 1000+ requests per second

**Algorithm Selection**:

1. **URL Generation**: Base62 encoding with auto-incrementing ID
2. **Storage**: Hash table with disk persistence
3. **Cache**: LRU cache for frequently accessed URLs
4. **Rate Limiting**: Token bucket algorithm

**System Architecture**:

```cpp
class URLShortener {
private:
    // Base62 encoding for URL generation
    std::string encodeBase62(long long id) {
        const std::string chars =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        std::string result;
        while (id > 0) {
            result = chars[id % 62] + result;
            id /= 62;
        }

        return result.empty() ? "0" : result;
    }

    // Distributed ID generator using Snowflake algorithm
    class SnowflakeIDGenerator {
    private:
        const long long epoch = 1609459200000LL; // 2021-01-01
        const int workerIdBits = 5;
        const int datacenterIdBits = 5;
        const int sequenceBits = 12;

        long long workerId;
        long long datacenterId;
        long long sequence = 0;
        long long lastTimestamp = -1;

    public:
        SnowflakeIDGenerator(long long worker, long long datacenter)
            : workerId(worker), datacenterId(datacenter) {}

        long long nextId() {
            long long timestamp = currentTimestamp();

            if (timestamp < lastTimestamp) {
                throw std::runtime_error("Clock moved backwards");
            }

            if (timestamp == lastTimestamp) {
                sequence = (sequence + 1) & ((1 << sequenceBits) - 1);
                if (sequence == 0) {
                    timestamp = tilNextMillis(lastTimestamp);
                }
            } else {
                sequence = 0;
            }

            lastTimestamp = timestamp;

            return ((timestamp - epoch) << (workerIdBits + datacenterIdBits + sequenceBits))
                   | (datacenterId << (workerIdBits + sequenceBits))
                   | (workerId << sequenceBits)
                   | sequence;
        }
    };

    SnowflakeIDGenerator idGenerator;
    DistributedCache cache;
    DatabaseConnection db;

public:
    // Create short URL
    std::string createShortURL(const std::string& longURL) {
        // Check if URL already exists
        auto existingShortURL = cache.get(longURL + "_reverse");
        if (existingShortURL) {
            return *existingShortURL;
        }

        // Generate new ID and encode
        long long id = idGenerator.nextId();
        std::string shortURL = encodeBase62(id);

        // Store in cache and database
        cache.put(shortURL, longURL);
        cache.put(longURL + "_reverse", shortURL);

        db.insert(shortURL, longURL);

        return shortURL;
    }

    // Resolve short URL
    std::optional<std::string> resolveShortURL(const std::string& shortURL) {
        // Check cache first
        auto longURL = cache.get(shortURL);
        if (longURL) {
            return longURL;
        }

        // Fallback to database
        auto dbResult = db.query(shortURL);
        if (dbResult) {
            // Cache the result
            cache.put(shortURL, *dbResult);
            return dbResult;
        }

        return std::nullopt;
    }
};
```

### 2. Distributed Message Queue

**Requirements**:

- Guarantee message delivery
- Handle millions of messages per second
- Support multiple producers and consumers
- Maintain message ordering within topics

**Algorithm Selection**:

1. **Message Storage**: Write-Ahead Log (WAL)
2. **Partitioning**: Consistent hashing with ordered partitions
3. **Replication**: Leader-follower with Raft consensus
4. **Consumer Groups**: Offset tracking with Kafka-style consumer groups

```cpp
class DistributedMessageQueue {
private:
    // Topic partitions with consistent hashing
    struct TopicPartition {
        std::string topicName;
        int partitionId;
        std::vector<Message> messages;
        std::atomic<long long> offset;
        std::mutex mutex;

        // Write-Ahead Log for durability
        WriteAheadLog wal;
    };

    // Consumer group management
    class ConsumerGroup {
    private:
        std::string groupId;
        std::unordered_map<std::string, ConsumerOffset> offsets;
        std::unordered_map<std::string, Consumer*> consumers;
        std::mutex mutex;

    public:
        void assignPartitions(Consumer* consumer) {
            std::lock_guard<std::mutex> lock(mutex);

            // Implement partition assignment strategy
            // e.g., Range assignment, Round-robin, etc.
            auto partitions = getUnassignedPartitions();

            for (auto& partition : partitions) {
                consumer->assignPartition(partition);
                consumers[partition.id] = consumer;
            }
        }

        void commitOffset(const std::string& partition, long long offset) {
            std::lock_guard<std::mutex> lock(mutex);
            offsets[partition].commit(offset);

            // Persist offset to coordinator
            persistOffsetToCoordinator(partition, offset);
        }
    };

    std::unordered_map<std::string, std::vector<TopicPartition>> topics;
    ConsistentHash partitionHash;

public:
    // Producer sending message
    void send(const std::string& topic, const std::string& key,
              const std::string& message) {
        // Determine partition using key
        int partitionId = getPartition(topic, key);
        TopicPartition& partition = topics[topic][partitionId];

        // Create message with timestamp and offset
        Message msg;
        msg.key = key;
        msg.value = message;
        msg.timestamp = currentTimestamp();
        msg.offset = partition.offset.fetch_add(1);

        // Write to WAL first for durability
        partition.wal.append(msg);

        // Add to partition
        {
            std::lock_guard<std::mutex> lock(partition.mutex);
            partition.messages.push_back(msg);
        }

        // Replicate to follower nodes asynchronously
        replicateMessage(topic, partitionId, msg);
    }

    // Consumer pulling messages
    std::vector<Message> poll(Consumer* consumer) {
        std::vector<Message> messages;

        // Get assigned partitions for this consumer
        auto assignedPartitions = consumer->getAssignedPartitions();

        for (auto& partition : assignedPartitions) {
            TopicPartition& tp = topics[partition.topic][partition.id];

            // Get consumer's current offset
            long long consumerOffset = consumer->getOffset(partition);

            // Fetch messages from offset
            {
                std::lock_guard<std::mutex> lock(tp.mutex);

                // Find starting point
                auto it = std::lower_bound(tp.messages.begin(),
                                         tp.messages.end(),
                                         consumerOffset,
                                         [](const Message& msg, long long offset) {
                                             return msg.offset < offset;
                                         });

                // Collect messages up to batch size
                while (it != tp.messages.end() && messages.size() < MAX_BATCH_SIZE) {
                    messages.push_back(*it);
                    ++it;
                }
            }
        }

        return messages;
    }
};
```

## Scalability Patterns

### 1. Database Sharding

```cpp
// Database sharding with automatic rebalancing
class ShardedDatabase {
private:
    struct Shard {
        DatabaseConnection connection;
        std::string shardId;
        long long minHash;
        long long maxHash;
        long long documentCount;
    };

    std::vector<Shard> shards;
    ConsistentHash hashRing;

public:
    // Automatic shard splitting when threshold reached
    void checkAndRebalance() {
        for (auto& shard : shards) {
            if (shard.documentCount > SHARD_SPLIT_THRESHOLD) {
                splitShard(shard);
            }
        }

        // Check for hotspots and rebalance if needed
        detectAndRebalanceHotspots();
    }

private:
    void splitShard(Shard& shard) {
        // Create new shard
        Shard newShard;
        newShard.shardId = generateShardId();
        newShard.connection = createNewConnection();

        // Calculate split point
        long long splitHash = (shard.minHash + shard.maxHash) / 2;

        // Migrate data to new shard
        migrateData(shard, newShard, splitHash);

        // Update hash ranges
        newShard.minHash = splitHash + 1;
        newShard.maxHash = shard.maxHash;
        shard.maxHash = splitHash;

        // Add new shard to cluster
        shards.push_back(newShard);
        hashRing.addNode(newShard.shardId);
    }
};
```

### 2. Microservices Architecture

```cpp
// Service discovery with health checks
class ServiceRegistry {
private:
    struct ServiceInstance {
        std::string serviceId;
        std::string address;
        int port;
        std::chrono::steady_clock::time_point lastHeartbeat;
        ServiceHealth health;
    };

    std::unordered_map<std::string, std::vector<ServiceInstance>> services;
    mutable std::shared_mutex mutex;

public:
    // Service registration
    void registerService(const std::string& serviceName,
                        const std::string& instanceId,
                        const std::string& address,
                        int port) {
        std::unique_lock<std::shared_mutex> lock(mutex);

        ServiceInstance instance;
        instance.serviceId = instanceId;
        instance.address = address;
        instance.port = port;
        instance.lastHeartbeat = std::chrono::steady_clock::now();
        instance.health = ServiceHealth::HEALTHY;

        services[serviceName].push_back(instance);
    }

    // Service discovery with load balancing
    std::optional<ServiceInstance> discoverService(const std::string& serviceName,
                                                 LoadBalanceStrategy strategy) {
        std::shared_lock<std::shared_mutex> lock(mutex);

        auto it = services.find(serviceName);
        if (it == services.end() || it->second.empty()) {
            return std::nullopt;
        }

        // Filter healthy instances
        std::vector<ServiceInstance> healthyInstances;
        auto now = std::chrono::steady_clock::now();

        for (const auto& instance : it->second) {
            auto timeSinceHeartbeat = std::chrono::duration_cast<std::chrono::seconds>
                                   (now - instance.lastHeartbeat).count();

            if (timeSinceHeartbeat < HEARTBEAT_TIMEOUT &&
                instance.health == ServiceHealth::HEALTHY) {
                healthyInstances.push_back(instance);
            }
        }

        if (healthyInstances.empty()) {
            return std::nullopt;
        }

        // Apply load balancing strategy
        return selectInstance(healthyInstances, strategy);
    }

    // Circuit breaker pattern for service calls
    class CircuitBreaker {
    private:
        enum State { CLOSED, OPEN, HALF_OPEN };

        State state;
        int failureCount;
        int successCount;
        std::chrono::steady_clock::time_point lastFailureTime;

        const int FAILURE_THRESHOLD = 5;
        const int SUCCESS_THRESHOLD = 3;
        const std::chrono::seconds TIMEOUT{60};

    public:
        CircuitBreaker() : state(CLOSED), failureCount(0), successCount(0) {}

        bool allowRequest() {
            auto now = std::chrono::steady_clock::now();

            switch (state) {
                case CLOSED:
                    return true;

                case OPEN:
                    if (now - lastFailureTime > TIMEOUT) {
                        state = HALF_OPEN;
                        successCount = 0;
                        return true;
                    }
                    return false;

                case HALF_OPEN:
                    return successCount < SUCCESS_THRESHOLD;
            }
        }

        void recordSuccess() {
            if (state == HALF_OPEN) {
                successCount++;
                if (successCount >= SUCCESS_THRESHOLD) {
                    state = CLOSED;
                    failureCount = 0;
                }
            } else if (state == CLOSED) {
                failureCount = 0;
            }
        }

        void recordFailure() {
            failureCount++;
            lastFailureTime = std::chrono::steady_clock::now();

            if (state == CLOSED && failureCount >= FAILURE_THRESHOLD) {
                state = OPEN;
            } else if (state == HALF_OPEN) {
                state = OPEN;
                successCount = 0;
            }
        }
    };
};
```

## Time & Space Complexity Analysis

| Component          | Operation       | Time Complexity | Space Complexity |
| ------------------ | --------------- | --------------- | ---------------- |
| Consistent Hashing | Add/Remove Node | O(R log N)      | O(N × R)         |
| Consistent Hashing | Lookup          | O(log N)        | O(N × R)         |
| Bloom Filter       | Add/Check       | O(k)            | O(m)             |
| Rate Limiter       | Token Bucket    | O(1)            | O(1)             |
| Rate Limiter       | Leaky Bucket    | O(1) amortized  | O(n)             |
| Sharding           | Hash-based      | O(1)            | O(1)             |
| Sharding           | Range-based     | O(log n)        | O(n)             |
| Service Discovery  | Registration    | O(1)            | O(n)             |
| Service Discovery  | Lookup          | O(n)            | O(n)             |
| Circuit Breaker    | Check/Update    | O(1)            | O(1)             |

Where:

- N = number of nodes
- R = number of replicas
- k = number of hash functions
- m = bloom filter size
- n = number of services/shards

## Best Practices

### 1. Design for Failure

- Assume everything will fail
- Implement retries with exponential backoff
- Use circuit breakers to prevent cascade failures
- Design idempotent operations

### 2. Optimize for Common Cases

- Cache frequently accessed data
- Use appropriate data structures for access patterns
- Balance read vs. write optimization based on workload

### 3. Monitor Everything

- Track latency percentiles (P50, P90, P99)
- Monitor resource utilization
- Set up alerts for anomalies
- Use distributed tracing

### 4. Plan for Scale

- Design horizontal scaling from the beginning
- Use consistent hashing for data distribution
- Implement backpressure mechanisms
- Plan capacity based on growth projections

## Practice Problems

### Problem 1: Design a Video Streaming Service

Design a system like YouTube that handles:

- Video uploads and processing
- Real-time streaming to millions of users
- Recommendations based on viewing history

**Key Components**: CDN, video transcoding pipeline, recommendation engine, user activity tracking

### Problem 2: Design a Ride-Sharing Service

Design a system like Uber that handles:

- Real-time driver location tracking
- Matching drivers with riders
- Surge pricing calculations
- Trip history and billing

**Key Components**: Geospatial indexing, real-time matching algorithm, pricing engine, payment processing

### Problem 3: Design a Social Media Feed

Design a system like Twitter that handles:

- Real-time feed updates
- Trending topics calculation
- User mentions and notifications
- Media storage and retrieval

**Key Components**: Timeline generation, fanout service, trending detection, notification service

## What's Next

Tomorrow we'll focus on Interview Preparation Strategies, where we'll cover:

- Common system design interview patterns
- How to approach system design questions
- Time management during interviews
- Communication strategies for technical discussions

## Key Takeaways

- System design requires combining multiple algorithms and data structures
- Trade-offs are inevitable - understand the business requirements to make the right choices
- Scalability patterns are built on fundamental DSA concepts
- Monitoring and failure handling are as important as the initial design
- Think in terms of distributed systems, not single machines

#DSAin45 #SystemDesign #Architecture #Scalability #DistributedSystems
