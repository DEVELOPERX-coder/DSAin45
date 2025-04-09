#include <iostream>
#include <vector>
#include <map>
using namespace std;
// ADVANCED IMPLEMENTATION EXAMPLES FOR DAY 10

// ===== INDEXED PRIORITY QUEUE IMPLEMENTATION =====
// This allows updating priorities of elements already in the queue - useful for Dijkstra's algorithm

template <typename Key, typename Value>
class IndexedPriorityQueue
{
private:
    std::vector<std::pair<Key, Value>> heap;      // The binary heap
    std::unordered_map<Key, size_t> key_to_index; // Maps keys to their positions in the heap

    // Helper functions for maintaining heap property
    size_t parent(size_t i) { return (i - 1) / 2; }
    size_t leftChild(size_t i) { return 2 * i + 1; }
    size_t rightChild(size_t i) { return 2 * i + 2; }

    void heapifyUp(size_t i)
    {
        while (i > 0 && heap[parent(i)].second > heap[i].second)
        {
            std::swap(heap[i], heap[parent(i)]);
            key_to_index[heap[i].first] = i;
            key_to_index[heap[parent(i)].first] = parent(i);
            i = parent(i);
        }
    }

    void heapifyDown(size_t i)
    {
        size_t minIndex = i;
        size_t left = leftChild(i);

        if (left < heap.size() && heap[left].second < heap[minIndex].second)
        {
            minIndex = left;
        }

        size_t right = rightChild(i);
        if (right < heap.size() && heap[right].second < heap[minIndex].second)
        {
            minIndex = right;
        }

        if (i != minIndex)
        {
            std::swap(heap[i], heap[minIndex]);
            key_to_index[heap[i].first] = i;
            key_to_index[heap[minIndex].first] = minIndex;
            heapifyDown(minIndex);
        }
    }

public:
    // Check if key exists in the queue
    bool contains(const Key &key) const
    {
        return key_to_index.find(key) != key_to_index.end();
    }

    // Insert or update key-value pair
    void push(const Key &key, const Value &value)
    {
        if (contains(key))
        {
            // Update existing key
            size_t i = key_to_index[key];
            Value old_value = heap[i].second;
            heap[i].second = value;

            // Restore heap property
            if (value < old_value)
            {
                heapifyUp(i);
            }
            else if (value > old_value)
            {
                heapifyDown(i);
            }
        }
        else
        {
            // Insert new key
            heap.push_back({key, value});
            key_to_index[key] = heap.size() - 1;
            heapifyUp(heap.size() - 1);
        }
    }

    // Remove and return the minimum element
    std::pair<Key, Value> pop()
    {
        if (isEmpty())
        {
            throw std::underflow_error("Priority queue underflow");
        }

        std::pair<Key, Value> min = heap[0];
        key_to_index.erase(min.first);

        if (heap.size() > 1)
        {
            heap[0] = heap.back();
            key_to_index[heap[0].first] = 0;
            heap.pop_back();
            heapifyDown(0);
        }
        else
        {
            heap.pop_back();
        }

        return min;
    }

    // Get value associated with key
    Value getValue(const Key &key) const
    {
        if (!contains(key))
        {
            throw std::out_of_range("Key not found in priority queue");
        }
        return heap[key_to_index.at(key)].second;
    }

    // Remove a specific key
    void remove(const Key &key)
    {
        if (!contains(key))
        {
            throw std::out_of_range("Key not found in priority queue");
        }

        size_t i = key_to_index[key];
        key_to_index.erase(key);

        if (i == heap.size() - 1)
        {
            heap.pop_back();
        }
        else
        {
            heap[i] = heap.back();
            key_to_index[heap[i].first] = i;
            heap.pop_back();

            Value old_value = heap[i].second;
            heapifyDown(i);

            // If heapifyDown didn't move the element, try heapifyUp
            if (i < heap.size() && heap[i].second == old_value)
            {
                heapifyUp(i);
            }
        }
    }

    // Get the minimum element without removing it
    std::pair<Key, Value> top() const
    {
        if (isEmpty())
        {
            throw std::underflow_error("Priority queue is empty");
        }
        return heap[0];
    }

    // Check if priority queue is empty
    bool isEmpty() const
    {
        return heap.empty();
    }

    // Get the number of elements
    size_t size() const
    {
        return heap.size();
    }
};

// ===== DOUBLE-ENDED PRIORITY QUEUE IMPLEMENTATION =====
// Efficient access to both minimum and maximum elements

template <typename T>
class DoubleEndedPriorityQueue
{
private:
    std::priority_queue<T> maxHeap;                                  // For finding maximum
    std::priority_queue<T, std::vector<T>, std::greater<T>> minHeap; // For finding minimum
    std::unordered_map<T, int> frequency;                            // Track count of each element

    // Move elements between heaps to maintain balance
    void rebalance()
    {
        if (maxHeap.empty() || minHeap.empty())
        {
            return;
        }

        T max_top = maxHeap.top();
        T min_top = minHeap.top();

        // If min-heap's min > max-heap's max, swap them
        if (min_top > max_top)
        {
            maxHeap.pop();
            minHeap.pop();

            maxHeap.push(min_top);
            minHeap.push(max_top);
        }
    }

public:
    // Insert element into both heaps
    void insert(const T &value)
    {
        maxHeap.push(value);
        minHeap.push(value);
        frequency[value]++;
    }

    // Find and remove the minimum element
    T findMin()
    {
        if (minHeap.empty())
        {
            throw std::underflow_error("Double-ended priority queue is empty");
        }

        T min_value = minHeap.top();
        removeElement(min_value);
        return min_value;
    }

    // Find and remove the maximum element
    T findMax()
    {
        if (maxHeap.empty())
        {
            throw std::underflow_error("Double-ended priority queue is empty");
        }

        T max_value = maxHeap.top();
        removeElement(max_value);
        return max_value;
    }

    // Get the minimum element without removing
    T getMin() const
    {
        if (minHeap.empty())
        {
            throw std::underflow_error("Double-ended priority queue is empty");
        }
        return minHeap.top();
    }

    // Get the maximum element without removing
    T getMax() const
    {
        if (maxHeap.empty())
        {
            throw std::underflow_error("Double-ended priority queue is empty");
        }
        return maxHeap.top();
    }

    // Remove a specific element (one occurrence)
    void removeElement(const T &value)
    {
        if (frequency.find(value) == frequency.end() || frequency[value] == 0)
        {
            throw std::out_of_range("Element not found in double-ended priority queue");
        }

        frequency[value]--;

        // Note: We don't physically remove from heaps here, just mark as removed via frequency
        // Actual removal happens during find operations
        // This is a lazy approach - in a real implementation we might want to periodically rebuild heaps
    }

    // Check if empty
    bool isEmpty() const
    {
        return maxHeap.empty() || minHeap.empty();
    }

    // Number of unique elements (not counting duplicates)
    size_t uniqueSize() const
    {
        return frequency.size();
    }

    // Total number of elements (including duplicates)
    size_t totalSize() const
    {
        size_t total = 0;
        for (const auto &pair : frequency)
        {
            total += pair.second;
        }
        return total;
    }
};

// ===== REAL-WORLD APPLICATION: HUFFMAN CODING =====
// Data compression algorithm using a priority queue

struct HuffmanNode
{
    char character;
    int frequency;
    HuffmanNode *left, *right;

    HuffmanNode(char ch, int freq, HuffmanNode *l = nullptr, HuffmanNode *r = nullptr)
        : character(ch), frequency(freq), left(l), right(r) {}

    // For priority queue comparison
    bool operator>(const HuffmanNode &other) const
    {
        return frequency > other.frequency;
    }
};

// Create Huffman coding tree
HuffmanNode *buildHuffmanTree(const std::string &text)
{
    // Count frequency of each character
    std::unordered_map<char, int> freq;
    for (char c : text)
    {
        freq[c]++;
    }

    // Create a min heap (priority queue)
    std::priority_queue<HuffmanNode, std::vector<HuffmanNode>, std::greater<HuffmanNode>> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto &pair : freq)
    {
        pq.push(HuffmanNode(pair.first, pair.second));
    }

    // Build Huffman Tree: iterate until there is only one node left
    while (pq.size() > 1)
    {
        // Extract the two nodes of the highest priority (lowest frequency)
        HuffmanNode *left = new HuffmanNode(pq.top());
        pq.pop();

        HuffmanNode *right = new HuffmanNode(pq.top());
        pq.pop();

        // Create a new internal node with these two nodes as children
        // and with a frequency equal to the sum of both nodes' frequencies
        int sum_freq = left->frequency + right->frequency;
        HuffmanNode *newNode = new HuffmanNode('\0', sum_freq, left, right);

        // Add this node to the priority queue
        pq.push(*newNode);
    }

    // The final node is the root of the Huffman tree
    return new HuffmanNode(pq.top());
}

// Generate Huffman codes for each character
void generateCodes(HuffmanNode *root, const std::string &code, std::unordered_map<char, std::string> &huffmanCodes)
{
    if (!root)
        return;

    // Found a leaf node (character)
    if (!root->left && !root->right)
    {
        huffmanCodes[root->character] = code;
    }

    // Traverse left (add 0 to code)
    generateCodes(root->left, code + "0", huffmanCodes);

    // Traverse right (add 1 to code)
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Encode text using Huffman codes
std::string encodeText(const std::string &text, const std::unordered_map<char, std::string> &huffmanCodes)
{
    std::string encodedText = "";
    for (char c : text)
    {
        encodedText += huffmanCodes.at(c);
    }
    return encodedText;
}

// Decode Huffman-encoded text using the Huffman tree
std::string decodeText(const std::string &encodedText, HuffmanNode *root)
{
    std::string decodedText = "";
    HuffmanNode *current = root;

    for (char bit : encodedText)
    {
        // Navigate according to the bit
        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        // If we reach a leaf node, we've found a character
        if (!current->left && !current->right)
        {
            decodedText += current->character;
            current = root; // Reset to the root for the next character
        }
    }

    return decodedText;
}

// ===== REAL-WORLD APPLICATION: DIJKSTRA'S ALGORITHM =====
// Shortest path in a weighted graph using an indexed priority queue

struct Edge
{
    int to;
    int weight;

    Edge(int t, int w) : to(t), weight(w) {}
};

void dijkstra(const std::vector<std::vector<Edge>> &graph, int source, std::vector<int> &dist, std::vector<int> &prev)
{
    int n = graph.size();
    dist.assign(n, INT_MAX);
    prev.assign(n, -1);

    dist[source] = 0;

    // Use indexed priority queue for efficient decrease-key operations
    IndexedPriorityQueue<int, int> pq;
    pq.push(source, 0); // Start with source vertex

    while (!pq.isEmpty())
    {
        auto [u, d] = pq.pop(); // Extract vertex with minimum distance

        if (d > dist[u])
        {
            continue; // Skip if we've found a better path already
        }

        // Process all neighbors of u
        for (const Edge &e : graph[u])
        {
            int v = e.to;
            int weight = e.weight;

            // Relaxation step
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;

                // Update or insert vertex in priority queue
                pq.push(v, dist[v]);
            }
        }
    }
}

// Reconstruct shortest path
std::vector<int> reconstructPath(const std::vector<int> &prev, int source, int target)
{
    std::vector<int> path;

    for (int at = target; at != -1; at = prev[at])
    {
        path.push_back(at);
    }

    std::reverse(path.begin(), path.end());

    if (path[0] == source)
    {
        return path;
    }
    return {}; // No path exists
}

// ===== SLIDING WINDOW MEDIAN =====
// Maintaining the median of a sliding window using two heaps

class SlidingWindowMedian
{
private:
    std::priority_queue<int> maxHeap;                                      // Elements smaller than median
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // Elements larger than median
    std::unordered_map<int, int> deleted;                                  // Track elements to be removed

    // Rebalance heaps to maintain size property
    void balance()
    {
        // Ensure maxHeap has at most one more element than minHeap
        if (maxHeap.size() > minHeap.size() + 1)
        {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
            removeDeletedFromHeap(maxHeap);
        }
        else if (minHeap.size() > maxHeap.size())
        {
            maxHeap.push(minHeap.top());
            minHeap.pop();
            removeDeletedFromHeap(minHeap);
        }
    }

    // Lazily clean the heaps (only when top element is marked for deletion)
    void removeDeletedFromHeap(std::priority_queue<int> &heap)
    {
        while (!heap.empty() && deleted[heap.top()] > 0)
        {
            deleted[heap.top()]--;
            heap.pop();
        }
    }

    void removeDeletedFromHeap(std::priority_queue<int, std::vector<int>, std::greater<int>> &heap)
    {
        while (!heap.empty() && deleted[heap.top()] > 0)
        {
            deleted[heap.top()]--;
            heap.pop();
        }
    }

public:
    std::vector<double> medianSlidingWindow(const std::vector<int> &nums, int k)
    {
        std::vector<double> result;

        // Process the first k elements
        for (int i = 0; i < k; i++)
        {
            addNum(nums[i]);
        }

        // Get the median for the first window
        result.push_back(getMedian());

        // Slide the window
        for (int i = k; i < nums.size(); i++)
        {
            // Remove the leftmost element from the window
            removeNum(nums[i - k]);

            // Add the new element
            addNum(nums[i]);

            // Calculate median for current window
            result.push_back(getMedian());
        }

        return result;
    }

    void addNum(int num)
    {
        if (maxHeap.empty() || num <= maxHeap.top())
        {
            maxHeap.push(num);
        }
        else
        {
            minHeap.push(num);
        }

        balance();
    }

    void removeNum(int num)
    {
        // Mark for deletion (lazy removal)
        deleted[num]++;

        // Clean up heaps if necessary
        if (!maxHeap.empty() && num <= maxHeap.top())
        {
            if (num == maxHeap.top())
            {
                removeDeletedFromHeap(maxHeap);
            }
        }
        else if (!minHeap.empty())
        {
            if (num == minHeap.top())
            {
                removeDeletedFromHeap(minHeap);
            }
        }

        balance();
    }

    double getMedian()
    {
        removeDeletedFromHeap(maxHeap);
        removeDeletedFromHeap(minHeap);

        if (maxHeap.size() > minHeap.size())
        {
            return maxHeap.top();
        }
        else
        {
            return ((double)maxHeap.top() + minHeap.top()) / 2.0;
        }
    }
};