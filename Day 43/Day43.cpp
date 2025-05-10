/*
 * SystemDesign.cpp
 * Day 43 of #DSAin45 - Comprehensive System Design Implementation
 *
 * This file demonstrates practical system design concepts using DSA principles,
 * including distributed algorithms, scalability patterns, and real-world systems.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <memory>
#include <future>
#include <condition_variable>

// Utility function to generate hash
uint64_t hashFunction(const std::string &key, uint64_t seed = 13)
{
    uint64_t hash = seed;
    for (char c : key)
    {
        hash = hash * 31 + c;
    }
    return hash;
}

// Utility for timing
template <typename TimePoint>
std::chrono::microseconds getElapsedMicroseconds(TimePoint start, TimePoint end)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

//==============================================================================
// Consistent Hashing for Distributed Systems
//==============================================================================

namespace ConsistentHashingSystem
{
    // Virtual node representation
    struct VirtualNode
    {
        uint64_t position;
        std::string realNode;

        VirtualNode(uint64_t pos, std::string node) : position(pos), realNode(node) {}

        bool operator<(const VirtualNode &other) const
        {
            return position < other.position;
        }
    };

    // Consistent Hash Ring implementation
    class ConsistentHash
    {
    private:
        std::vector<VirtualNode> ring;
        std::unordered_map<std::string, std::vector<size_t>> nodeToPositions;
        int numReplicas;
        mutable std::shared_mutex mutex;

        uint64_t hash(const std::string &key)
        {
            return hashFunction(key);
        }

    public:
        ConsistentHash(int replicas = 100) : numReplicas(replicas) {}

        // Add a physical node with multiple virtual replicas
        void addNode(const std::string &node)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            std::vector<size_t> positions;

            for (int i = 0; i < numReplicas; i++)
            {
                std::string virtualKey = node + "#" + std::to_string(i);
                uint64_t position = hash(virtualKey);

                // Insert into ring maintaining sorted order
                auto it = std::lower_bound(ring.begin(), ring.end(),
                                           VirtualNode(position, ""),
                                           [](const VirtualNode &a, const VirtualNode &b)
                                           {
                                               return a.position < b.position;
                                           });

                size_t index = it - ring.begin();
                ring.insert(it, VirtualNode(position, node));
                positions.push_back(index);
            }

            nodeToPositions[node] = std::move(positions);
        }

        // Remove a physical node
        void removeNode(const std::string &node)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            auto it = nodeToPositions.find(node);
            if (it == nodeToPositions.end())
            {
                return;
            }

            // Remove all virtual nodes for this physical node
            std::vector<size_t> positions = it->second;

            // Sort positions in descending order to avoid index shifting
            std::sort(positions.begin(), positions.end(), std::greater<size_t>());

            for (size_t pos : positions)
            {
                ring.erase(ring.begin() + pos);
            }

            nodeToPositions.erase(it);

            // Update position mappings for remaining nodes
            rebuildPositionMapping();
        }

        // Get the node responsible for a key
        std::string getNode(const std::string &key)
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            if (ring.empty())
            {
                return "";
            }

            uint64_t keyHash = hash(key);

            // Find first node with position >= keyHash
            auto it = std::lower_bound(ring.begin(), ring.end(),
                                       VirtualNode(keyHash, ""),
                                       [](const VirtualNode &a, const VirtualNode &b)
                                       {
                                           return a.position < b.position;
                                       });

            if (it == ring.end())
            {
                // Wrap around to the beginning
                return ring[0].realNode;
            }

            return it->realNode;
        }

        // Get k nearest nodes for replication
        std::vector<std::string> getKNearestNodes(const std::string &key, int k)
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            if (ring.empty())
            {
                return {};
            }

            uint64_t keyHash = hash(key);
            std::vector<std::string> result;
            std::unordered_set<std::string> seen;

            // Find starting position
            auto startIt = std::lower_bound(ring.begin(), ring.end(),
                                            VirtualNode(keyHash, ""),
                                            [](const VirtualNode &a, const VirtualNode &b)
                                            {
                                                return a.position < b.position;
                                            });

            size_t startIndex = (startIt == ring.end()) ? 0 : startIt - ring.begin();

            // Collect k unique physical nodes
            for (size_t i = 0; i < ring.size() && result.size() < k; i++)
            {
                size_t index = (startIndex + i) % ring.size();
                std::string node = ring[index].realNode;

                if (seen.find(node) == seen.end())
                {
                    result.push_back(node);
                    seen.insert(node);
                }
            }

            return result;
        }

        // Get distribution statistics
        void getDistributionStats()
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::unordered_map<std::string, int> nodeCount;

            // Count ranges for each node
            for (size_t i = 0; i < ring.size(); i++)
            {
                size_t nextIndex = (i + 1) % ring.size();

                uint64_t startPos = ring[i].position;
                uint64_t endPos = ring[nextIndex].position;

                if (nextIndex == 0)
                {
                    // Handle wrap-around
                    endPos += UINT64_MAX;
                }

                uint64_t rangeSize = endPos - startPos;
                nodeCount[ring[i].realNode] += rangeSize;
            }

            // Print statistics
            std::cout << "Consistent Hash Distribution:\n";
            for (const auto &[node, count] : nodeCount)
            {
                double percentage = (double)count / UINT64_MAX * 100;
                std::cout << "  Node " << node << ": " << std::fixed << std::setprecision(2)
                          << percentage << "%\n";
            }
        }

    private:
        void rebuildPositionMapping()
        {
            nodeToPositions.clear();

            for (size_t i = 0; i < ring.size(); i++)
            {
                const std::string &node = ring[i].realNode;
                nodeToPositions[node].push_back(i);
            }
        }
    };

    // Demonstration of consistent hashing
    void runConsistentHashingDemo()
    {
        std::cout << "\n=== CONSISTENT HASHING DEMO ===\n";

        ConsistentHash hash(50); // 50 virtual nodes per physical node

        // Add initial nodes
        std::vector<std::string> nodes = {"Node1", "Node2", "Node3", "Node4"};

        for (const auto &node : nodes)
        {
            hash.addNode(node);
        }

        // Test key distribution
        std::cout << "Initial distribution:\n";
        hash.getDistributionStats();

        // Test key assignments
        std::vector<std::string> testKeys = {"key1", "key2", "key3", "key4", "key5",
                                             "user123", "doc456", "image789"};

        std::cout << "\nKey assignments:\n";
        for (const auto &key : testKeys)
        {
            std::cout << "  " << key << " -> " << hash.getNode(key) << "\n";
        }

        // Test adding a new node
        std::cout << "\nAdding new node 'Node5'...\n";
        hash.addNode("Node5");

        std::cout << "Distribution after adding node:\n";
        hash.getDistributionStats();

        std::cout << "\nKey assignments after adding node:\n";
        for (const auto &key : testKeys)
        {
            std::cout << "  " << key << " -> " << hash.getNode(key) << "\n";
        }

        // Test removing a node
        std::cout << "\nRemoving node 'Node3'...\n";
        hash.removeNode("Node3");

        std::cout << "Distribution after removing node:\n";
        hash.getDistributionStats();

        // Test replication
        std::cout << "\nReplication example (3 replicas for 'critical_data'):\n";
        auto replicas = hash.getKNearestNodes("critical_data", 3);
        for (size_t i = 0; i < replicas.size(); i++)
        {
            std::cout << "  Replica " << i + 1 << ": " << replicas[i] << "\n";
        }
    }
}

//==============================================================================
// Bloom Filter for Distributed Cache
//==============================================================================

namespace BloomFilterSystem
{
    // Optimized Bloom Filter implementation
    class BloomFilter
    {
    private:
        std::vector<std::atomic<bool>> bits;
        int numHashes;
        std::vector<uint64_t> hashSeeds;

        size_t hash(const std::string &item, uint64_t seed) const
        {
            return hashFunction(item, seed) % bits.size();
        }

    public:
        BloomFilter(size_t size, int numHashes) : numHashes(numHashes)
        {
            bits.resize(size);
            for (size_t i = 0; i < size; i++)
            {
                bits[i].store(false);
            }

            // Initialize hash seeds
            std::random_device rd;
            std::mt19937_64 gen(rd());
            for (int i = 0; i < numHashes; i++)
            {
                hashSeeds.push_back(gen());
            }
        }

        // Add item to filter
        void add(const std::string &item)
        {
            for (int i = 0; i < numHashes; i++)
            {
                size_t index = hash(item, hashSeeds[i]);
                bits[index].store(true);
            }
        }

        // Check if item might be in the set
        bool mightContain(const std::string &item) const
        {
            for (int i = 0; i < numHashes; i++)
            {
                size_t index = hash(item, hashSeeds[i]);
                if (!bits[index].load())
                {
                    return false; // Definitely not in the set
                }
            }
            return true; // Might be in the set
        }

        // Clear the filter
        void clear()
        {
            for (auto &bit : bits)
            {
                bit.store(false);
            }
        }

        // Calculate false positive probability
        double getFalsePositiveProbability(size_t numElements) const
        {
            double m = bits.size();
            double n = numElements;
            double k = numHashes;

            // Formula: (1 - e^(-k*n/m))^k
            return std::pow(1 - std::exp(-k * n / m), k);
        }

        // Get optimal number of hash functions
        static int getOptimalHashFunctions(size_t numElements, size_t filterSize)
        {
            // Optimal k = (m/n) * ln(2)
            double m = filterSize;
            double n = numElements;
            return std::max(1, static_cast<int>(std::round((m / n) * std::log(2))));
        }
    };

    // Cache with Bloom Filter optimization
    class OptimizedCache
    {
    private:
        // LRU cache implementation
        struct CacheEntry
        {
            std::string value;
            std::chrono::steady_clock::time_point lastAccess;

            CacheEntry(std::string val) : value(std::move(val)),
                                          lastAccess(std::chrono::steady_clock::now()) {}
        };

        std::unordered_map<std::string, CacheEntry> cache;
        BloomFilter bloomFilter;
        size_t maxSize;
        mutable std::shared_mutex mutex;

    public:
        OptimizedCache(size_t capacity, size_t bloomSize, int numHashes)
            : maxSize(capacity), bloomFilter(bloomSize, numHashes) {}

        // Get value with Bloom filter optimization
        std::optional<std::string> get(const std::string &key)
        {
            // Quick check with Bloom filter
            if (!bloomFilter.mightContain(key))
            {
                return std::nullopt; // Definitely not in cache
            }

            std::shared_lock<std::shared_mutex> lock(mutex);

            auto it = cache.find(key);
            if (it == cache.end())
            {
                return std::nullopt; // False positive
            }

            // Update access time
            it->second.lastAccess = std::chrono::steady_clock::now();
            return it->second.value;
        }

        // Put value in cache
        void put(const std::string &key, const std::string &value)
        {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);

                // If cache is full, evict LRU item
                if (cache.size() >= maxSize)
                {
                    evictLRU();
                }

                // Add to cache
                cache[key] = CacheEntry(value);
            }

            // Add to Bloom filter
            bloomFilter.add(key);
        }

        // Remove from cache
        void remove(const std::string &key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            cache.erase(key);
            // Note: Can't remove from Bloom filter
        }

        // Get statistics
        void getStatistics() const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::cout << "Cache Statistics:\n";
            std::cout << "  Current Size: " << cache.size() << "/" << maxSize << "\n";

            double fpp = bloomFilter.getFalsePositiveProbability(cache.size());
            std::cout << "  Bloom Filter False Positive Rate: "
                      << std::fixed << std::setprecision(4) << fpp * 100 << "%\n";
        }

    private:
        void evictLRU()
        {
            auto oldestIt = cache.begin();
            auto oldestTime = oldestIt->second.lastAccess;

            for (auto it = cache.begin(); it != cache.end(); ++it)
            {
                if (it->second.lastAccess < oldestTime)
                {
                    oldestTime = it->second.lastAccess;
                    oldestIt = it;
                }
            }

            cache.erase(oldestIt);
        }
    };

    // Demonstration of Bloom Filter with cache
    void runBloomFilterDemo()
    {
        std::cout << "\n=== BLOOM FILTER CACHE DEMO ===\n";

        // Create cache with Bloom filter
        size_t cacheCapacity = 1000;
        size_t bloomSize = 10000;
        int numHashes = BloomFilter::getOptimalHashFunctions(cacheCapacity, bloomSize);

        std::cout << "Using " << numHashes << " hash functions for optimal performance\n";

        OptimizedCache cache(cacheCapacity, bloomSize, numHashes);

        // Add test data
        std::cout << "\nAdding 500 items to cache...\n";
        for (int i = 0; i < 500; i++)
        {
            std::string key = "key_" + std::to_string(i);
            std::string value = "value_" + std::to_string(i);
            cache.put(key, value);
        }

        cache.getStatistics();

        // Test cache hits and misses
        std::cout << "\nTesting cache lookups:\n";

        // Test hits
        int hits = 0;
        for (int i = 0; i < 100; i++)
        {
            std::string key = "key_" + std::to_string(i);
            if (cache.get(key))
            {
                hits++;
            }
        }
        std::cout << "Cache hits: " << hits << "/100\n";

        // Test misses
        int bloomMisses = 0;
        int actualMisses = 0;

        for (int i = 1000; i < 1100; i++)
        {
            std::string key = "key_" + std::to_string(i);

            // These keys shouldn't exist in cache
            auto result = cache.get(key);
            if (!result)
            {
                actualMisses++;
            }
        }

        std::cout << "Cache misses: " << actualMisses << "/100\n";

        // Fill cache to capacity
        std::cout << "\nFilling cache to capacity...\n";
        for (int i = 500; i < 1500; i++)
        {
            std::string key = "key_" + std::to_string(i);
            std::string value = "value_" + std::to_string(i);
            cache.put(key, value);
        }

        cache.getStatistics();
    }
}

//==============================================================================
// Rate Limiting Algorithms
//==============================================================================

namespace RateLimitingSystem
{
    // Token Bucket rate limiter
    class TokenBucket
    {
    private:
        long capacity;
        std::atomic<long> tokens;
        long refillRate; // tokens per second
        std::chrono::steady_clock::time_point lastRefill;
        mutable std::mutex mutex;

    public:
        TokenBucket(long cap, long rate)
            : capacity(cap), tokens(cap), refillRate(rate),
              lastRefill(std::chrono::steady_clock::now()) {}

        bool tryConsume(long numTokens = 1)
        {
            std::lock_guard<std::mutex> lock(mutex);

            refillTokens();

            long currentTokens = tokens.load();
            if (currentTokens >= numTokens)
            {
                tokens.store(currentTokens - numTokens);
                return true;
            }

            return false;
        }

        long getAvailableTokens()
        {
            std::lock_guard<std::mutex> lock(mutex);
            refillTokens();
            return tokens.load();
        }

    private:
        void refillTokens()
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRefill).count();

            if (elapsed > 0)
            {
                long newTokens = (elapsed * refillRate) / 1000;
                long currentTokens = tokens.load();
                long updatedTokens = std::min(capacity, currentTokens + newTokens);

                tokens.store(updatedTokens);
                lastRefill = now;
            }
        }
    };

    // Leaky Bucket rate limiter
    class LeakyBucket
    {
    private:
        std::queue<std::chrono::steady_clock::time_point> requests;
        long capacity;
        long leakRate; // requests per second
        mutable std::mutex mutex;

    public:
        LeakyBucket(long cap, long rate) : capacity(cap), leakRate(rate) {}

        bool tryEnqueue()
        {
            std::lock_guard<std::mutex> lock(mutex);

            auto now = std::chrono::steady_clock::now();

            // Leak old requests
            while (!requests.empty())
            {
                auto oldest = requests.front();
                auto age = std::chrono::duration_cast<std::chrono::seconds>(now - oldest).count();

                if (age >= (long)requests.size() / leakRate)
                {
                    requests.pop();
                }
                else
                {
                    break;
                }
            }

            // Check if there's space
            if ((long)requests.size() < capacity)
            {
                requests.push(now);
                return true;
            }

            return false;
        }

        long getQueueSize()
        {
            std::lock_guard<std::mutex> lock(mutex);
            return requests.size();
        }
    };

    // Sliding Window Log rate limiter
    class SlidingWindowLog
    {
    private:
        std::multiset<std::chrono::steady_clock::time_point> requestLog;
        long windowSize; // in milliseconds
        long maxRequests;
        mutable std::mutex mutex;

    public:
        SlidingWindowLog(long windowMs, long maxReq)
            : windowSize(windowMs), maxRequests(maxReq) {}

        bool tryRequest()
        {
            std::lock_guard<std::mutex> lock(mutex);

            auto now = std::chrono::steady_clock::now();
            auto windowStart = now - std::chrono::milliseconds(windowSize);

            // Remove old entries
            auto cutoff = requestLog.lower_bound(windowStart);
            requestLog.erase(requestLog.begin(), cutoff);

            // Check if we can add new request
            if ((long)requestLog.size() < maxRequests)
            {
                requestLog.insert(now);
                return true;
            }

            return false;
        }

        long getCurrentRequestCount()
        {
            std::lock_guard<std::mutex> lock(mutex);

            auto now = std::chrono::steady_clock::now();
            auto windowStart = now - std::chrono::milliseconds(windowSize);

            // Count requests in current window
            auto start = requestLog.lower_bound(windowStart);
            return std::distance(start, requestLog.end());
        }
    };

    // Sliding Window Counter rate limiter (more memory efficient)
    class SlidingWindowCounter
    {
    private:
        std::map<long, long> counters; // timestamp window -> count
        long windowSize;               // in milliseconds
        long maxRequests;
        long granularity; // Window subdivision
        mutable std::mutex mutex;

    public:
        SlidingWindowCounter(long windowMs, long maxReq, long gran = 100)
            : windowSize(windowMs), maxRequests(maxReq), granularity(gran) {}

        bool tryRequest()
        {
            std::lock_guard<std::mutex> lock(mutex);

            auto now = std::chrono::steady_clock::now();
            long currentWindow = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() / granularity;

            // Clean old windows
            auto cutoff = currentWindow - (windowSize / granularity);
            auto it = counters.begin();
            while (it != counters.end() && it->first < cutoff)
            {
                it = counters.erase(it);
            }

            // Calculate current count
            long currentCount = 0;
            for (const auto &[window, count] : counters)
            {
                currentCount += count;
            }

            // Check if we can add new request
            if (currentCount < maxRequests)
            {
                counters[currentWindow]++;
                return true;
            }

            return false;
        }
    };

    // Distributed rate limiter using Redis-like approach
    class DistributedRateLimiter
    {
    private:
        // Simulated Redis-like storage
        struct RedisLikeStore
        {
            std::unordered_map<std::string, long> counters;
            std::unordered_map<std::string, std::chrono::steady_clock::time_point> expiries;
            mutable std::mutex mutex;

            bool increment(const std::string &key, long ttlMs = 60000)
            {
                std::lock_guard<std::mutex> lock(mutex);

                auto now = std::chrono::steady_clock::now();

                // Check and remove expired keys
                auto it = expiries.find(key);
                if (it != expiries.end())
                {
                    if (now > it->second)
                    {
                        counters.erase(key);
                        expiries.erase(it);
                    }
                }

                // Increment counter
                counters[key]++;
                expiries[key] = now + std::chrono::milliseconds(ttlMs);

                return true;
            }

            long get(const std::string &key)
            {
                std::lock_guard<std::mutex> lock(mutex);

                auto now = std::chrono::steady_clock::now();

                // Check expiry
                auto expIt = expiries.find(key);
                if (expIt != expiries.end() && now > expIt->second)
                {
                    counters.erase(key);
                    expiries.erase(expIt);
                    return 0;
                }

                auto it = counters.find(key);
                return (it != counters.end()) ? it->second : 0;
            }
        };

        RedisLikeStore store;
        long windowSize;
        long maxRequests;

    public:
        DistributedRateLimiter(long windowMs, long maxReq)
            : windowSize(windowMs), maxRequests(maxReq) {}

        bool tryRequest(const std::string &clientId)
        {
            // Create window-based key
            auto now = std::chrono::steady_clock::now();
            long window = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() / windowSize;

            std::string key = clientId + ":" + std::to_string(window);

            // Get current count
            long currentCount = store.get(key);

            if (currentCount >= maxRequests)
            {
                return false;
            }

            // Increment counter
            store.increment(key, windowSize);
            return true;
        }
    };

    // Rate limiter benchmark
    void runRateLimiterBenchmark()
    {
        std::cout << "\n=== RATE LIMITER BENCHMARK ===\n";

        // Test parameters
        const long capacity = 100;
        const long refillRate = 10; // 10 requests per second
        const int numThreads = 4;
        const int requestsPerThread = 1000;

        // Test Token Bucket
        {
            std::cout << "\nTesting Token Bucket...\n";
            TokenBucket limiter(capacity, refillRate);

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<std::future<int>> futures;
            for (int t = 0; t < numThreads; t++)
            {
                futures.push_back(std::async(std::launch::async, [&limiter, requestsPerThread]
                                             {
                    int accepted = 0;
                    for (int i = 0; i < requestsPerThread; i++) {
                        if (limiter.tryConsume()) {
                            accepted++;
                        }
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }
                    return accepted; }));
            }

            int totalAccepted = 0;
            for (auto &future : futures)
            {
                totalAccepted += future.get();
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << "  Accepted: " << totalAccepted << "/"
                      << numThreads * requestsPerThread << " requests\n";
            std::cout << "  Time: " << elapsed.count() / 1000.0 << " ms\n";
        }

        // Test Sliding Window Log
        {
            std::cout << "\nTesting Sliding Window Log...\n";
            SlidingWindowLog limiter(1000, capacity); // 1-second window

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<std::future<int>> futures;
            for (int t = 0; t < numThreads; t++)
            {
                futures.push_back(std::async(std::launch::async, [&limiter, requestsPerThread]
                                             {
                    int accepted = 0;
                    for (int i = 0; i < requestsPerThread; i++) {
                        if (limiter.tryRequest()) {
                            accepted++;
                        }
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }
                    return accepted; }));
            }

            int totalAccepted = 0;
            for (auto &future : futures)
            {
                totalAccepted += future.get();
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << "  Accepted: " << totalAccepted << "/"
                      << numThreads * requestsPerThread << " requests\n";
            std::cout << "  Time: " << elapsed.count() / 1000.0 << " ms\n";
        }

        // Test Sliding Window Counter
        {
            std::cout << "\nTesting Sliding Window Counter...\n";
            SlidingWindowCounter limiter(1000, capacity); // 1-second window

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<std::future<int>> futures;
            for (int t = 0; t < numThreads; t++)
            {
                futures.push_back(std::async(std::launch::async, [&limiter, requestsPerThread]
                                             {
                    int accepted = 0;
                    for (int i = 0; i < requestsPerThread; i++) {
                        if (limiter.tryRequest()) {
                            accepted++;
                        }
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }
                    return accepted; }));
            }

            int totalAccepted = 0;
            for (auto &future : futures)
            {
                totalAccepted += future.get();
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << "  Accepted: " << totalAccepted << "/"
                      << numThreads * requestsPerThread << " requests\n";
            std::cout << "  Time: " << elapsed.count() / 1000.0 << " ms\n";
        }
    }
}

//==============================================================================
// System Design Patterns: URL Shortener
//==============================================================================

namespace URLShortenerSystem
{
    // Snowflake-style ID generator
    class SnowflakeIDGenerator
    {
    private:
        const long long epoch = 1609459200000LL; // January 1, 2021
        const int workerIdBits = 5;
        const int datacenterIdBits = 5;
        const int sequenceBits = 12;

        const long long maxWorkerId = (1LL << workerIdBits) - 1;
        const long long maxDatacenterId = (1LL << datacenterIdBits) - 1;
        const long long maxSequence = (1LL << sequenceBits) - 1;

        long long workerId;
        long long datacenterId;
        long long sequence = 0;
        long long lastTimestamp = -1;

        mutable std::mutex mutex;

        long long currentTimestamp()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        long long waitForNextTimestamp(long long lastTimestamp)
        {
            long long timestamp;
            do
            {
                timestamp = currentTimestamp();
            } while (timestamp <= lastTimestamp);
            return timestamp;
        }

    public:
        SnowflakeIDGenerator(long long worker, long long datacenter)
            : workerId(worker), datacenterId(datacenter)
        {

            if (workerId > maxWorkerId || workerId < 0)
            {
                throw std::invalid_argument("Worker ID out of range");
            }

            if (datacenterId > maxDatacenterId || datacenterId < 0)
            {
                throw std::invalid_argument("Datacenter ID out of range");
            }
        }

        long long nextId()
        {
            std::lock_guard<std::mutex> lock(mutex);

            long long timestamp = currentTimestamp();

            if (timestamp < lastTimestamp)
            {
                throw std::runtime_error("Clock moved backwards");
            }

            if (timestamp == lastTimestamp)
            {
                sequence = (sequence + 1) & maxSequence;
                if (sequence == 0)
                {
                    timestamp = waitForNextTimestamp(lastTimestamp);
                }
            }
            else
            {
                sequence = 0;
            }

            lastTimestamp = timestamp;

            return ((timestamp - epoch) << (workerIdBits + datacenterIdBits + sequenceBits)) | (datacenterId << (workerIdBits + sequenceBits)) | (workerId << sequenceBits) | sequence;
        }
    };

    // Base62 encoder for URL shortening
    class Base62Encoder
    {
    private:
        static const std::string ALPHABET;

    public:
        static std::string encode(long long num)
        {
            if (num == 0)
                return "0";

            std::string result;
            while (num > 0)
            {
                result = ALPHABET[num % 62] + result;
                num /= 62;
            }

            return result;
        }

        static long long decode(const std::string &str)
        {
            long long result = 0;
            for (char c : str)
            {
                result = result * 62 + ALPHABET.find(c);
            }
            return result;
        }
    };

    const std::string Base62Encoder::ALPHABET =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    // URL mapping storage
    struct URLMapping
    {
        std::string longURL;
        std::chrono::steady_clock::time_point createdAt;
        long long hitCount;

        URLMapping() : hitCount(0) {}
        URLMapping(std::string url)
            : longURL(std::move(url)),
              createdAt(std::chrono::steady_clock::now()),
              hitCount(0) {}
    };

    // URL Shortener service
    class URLShortener
    {
    private:
        SnowflakeIDGenerator idGenerator;
        ConsistentHashingSystem::ConsistentHash nodeHash;
        std::unordered_map<std::string, URLMapping> urlMap;      // shortCode -> URLMapping
        std::unordered_map<std::string, std::string> reverseMap; // longURL -> shortCode
        BloomFilterSystem::BloomFilter bloomFilter;
        RateLimitingSystem::TokenBucket rateLimiter;

        mutable std::shared_mutex mutex;

        std::string getNodeForURL(const std::string &shortCode)
        {
            return nodeHash.getNode(shortCode);
        }

    public:
        URLShortener(long long workerId, long long datacenterId,
                     size_t bloomSize = 1000000)
            : idGenerator(workerId, datacenterId),
              bloomFilter(bloomSize, 5),
              rateLimiter(1000, 100) // 100 requests per second
        {
            // Initialize node hash with some nodes
            nodeHash.addNode("node1");
            nodeHash.addNode("node2");
            nodeHash.addNode("node3");
        }

        // Create short URL
        std::string createShortURL(const std::string &longURL)
        {
            // Check rate limit
            if (!rateLimiter.tryConsume())
            {
                throw std::runtime_error("Rate limit exceeded");
            }

            // Check if URL already exists
            {
                std::shared_lock<std::shared_mutex> lock(mutex);
                auto it = reverseMap.find(longURL);
                if (it != reverseMap.end())
                {
                    return it->second;
                }
            }

            // Generate new short code
            long long id = idGenerator.nextId();
            std::string shortCode = Base62Encoder::encode(id);

            // Store mapping
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                urlMap[shortCode] = URLMapping(longURL);
                reverseMap[longURL] = shortCode;
            }

            // Add to bloom filter
            bloomFilter.add(shortCode);

            return shortCode;
        }

        // Resolve short URL
        std::optional<std::string> resolveShortURL(const std::string &shortCode)
        {
            // Quick check with bloom filter
            if (!bloomFilter.mightContain(shortCode))
            {
                return std::nullopt;
            }

            // Check rate limit
            if (!rateLimiter.tryConsume())
            {
                throw std::runtime_error("Rate limit exceeded");
            }

            // Look up mapping
            {
                std::shared_lock<std::shared_mutex> lock(mutex);
                auto it = urlMap.find(shortCode);
                if (it == urlMap.end())
                {
                    return std::nullopt; // Bloom filter false positive
                }

                // Update hit count
                it->second.hitCount++;
                return it->second.longURL;
            }
        }

        // Get analytics for a short URL
        std::optional<URLMapping> getAnalytics(const std::string &shortCode)
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            auto it = urlMap.find(shortCode);
            if (it == urlMap.end())
            {
                return std::nullopt;
            }

            return it->second;
        }

        // Delete a short URL
        bool deleteShortURL(const std::string &shortCode)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            auto it = urlMap.find(shortCode);
            if (it == urlMap.end())
            {
                return false;
            }

            std::string longURL = it->second.longURL;
            urlMap.erase(it);
            reverseMap.erase(longURL);

            return true;
        }

        // Get statistics
        void getStatistics()
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::cout << "URL Shortener Statistics:\n";
            std::cout << "  Total URLs: " << urlMap.size() << "\n";

            // Calculate total hits
            long long totalHits = 0;
            for (const auto &[code, mapping] : urlMap)
            {
                totalHits += mapping.hitCount;
            }
            std::cout << "  Total Hits: " << totalHits << "\n";

            // Available tokens
            std::cout << "  Available Rate Limit Tokens: "
                      << rateLimiter.getAvailableTokens() << "\n";
        }
    };

    // Demonstration of URL shortener system
    void runURLShortenerDemo()
    {
        std::cout << "\n=== URL SHORTENER DEMO ===\n";

        URLShortener shortener(1, 1); // Worker ID 1, Datacenter ID 1

        // Test URL creation
        std::vector<std::string> testURLs = {
            "https://www.example.com/very/long/url/with/many/parameters?foo=bar&baz=qux",
            "https://www.google.com",
            "https://www.github.com/awesome/repo",
            "https://www.stackoverflow.com/questions/12345678/how-to-implement-url-shortener",
            "https://www.example.com/very/long/url/with/many/parameters?foo=bar&baz=qux" // Duplicate
        };

        std::cout << "Creating short URLs:\n";
        std::unordered_map<std::string, std::string> shortCodes;

        for (const auto &url : testURLs)
        {
            try
            {
                std::string shortCode = shortener.createShortURL(url);
                shortCodes[url] = shortCode;
                std::cout << "  " << url.substr(0, 50) << "... -> " << shortCode << "\n";
            }
            catch (const std::exception &e)
            {
                std::cout << "  Error: " << e.what() << "\n";
            }
        }

        // Test URL resolution
        std::cout << "\nResolving short URLs:\n";
        for (const auto &[url, code] : shortCodes)
        {
            auto resolved = shortener.resolveShortURL(code);
            if (resolved)
            {
                std::cout << "  " << code << " -> " << resolved->substr(0, 50) << "...\n";
            }
            else
            {
                std::cout << "  " << code << " -> NOT FOUND\n";
            }
        }

        // Test analytics
        std::cout << "\nAnalytics:\n";
        for (const auto &[url, code] : shortCodes)
        {
            auto analytics = shortener.getAnalytics(code);
            if (analytics)
            {
                std::cout << "  " << code << " - Hits: " << analytics->hitCount << "\n";
            }
        }

        // Test non-existent URL
        std::cout << "\nTesting non-existent short code:\n";
        auto result = shortener.resolveShortURL("nonexistent");
        std::cout << "  'nonexistent' -> " << (result ? *result : "NOT FOUND") << "\n";

        // Get statistics
        std::cout << "\n";
        shortener.getStatistics();

        // Stress test rate limiting
        std::cout << "\nStress testing rate limit:\n";
        int accepted = 0;
        int rejected = 0;

        for (int i = 0; i < 200; i++)
        {
            try
            {
                shortener.createShortURL("https://test.com/" + std::to_string(i));
                accepted++;
            }
            catch (const std::exception &e)
            {
                rejected++;
            }
        }

        std::cout << "  Accepted: " << accepted << ", Rejected: " << rejected << "\n";
    }
}

//==============================================================================
// Main Function and System Demonstrations
//==============================================================================

int main()
{
    std::cout << "===================================\n";
    std::cout << "Day 43: System Design Fundamentals\n";
    std::cout << "===================================\n";

    // Run all demonstrations
    ConsistentHashingSystem::runConsistentHashingDemo();
    BloomFilterSystem::runBloomFilterDemo();
    RateLimitingSystem::runRateLimiterBenchmark();
    URLShortenerSystem::runURLShortenerDemo();

    return 0;
}