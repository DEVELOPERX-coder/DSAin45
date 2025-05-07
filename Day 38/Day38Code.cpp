/**
 * @file greedy_algorithms.cpp
 * @brief Comprehensive implementation of common greedy algorithms for DSAin45 Day 38
 * @author DSAin45 Team
 * @date May 2025
 *
 * This file provides production-quality implementations of various greedy algorithms,
 * including Activity Selection, Fractional Knapsack, Huffman Coding, and more.
 * Each algorithm is thoroughly documented with time and space complexity analysis,
 * real-world applications, and benchmarking code to compare different approaches.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <string>
#include <chrono>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <limits>
#include <functional>
#include <utility>

using namespace std;
using namespace std::chrono;

// Forward declarations
namespace ActivitySelection
{
    vector<int> greedy(const vector<int> &start, const vector<int> &finish);
    void runDemo();
    void runBenchmark(int size);
}

namespace FractionalKnapsack
{
    double greedy(const vector<int> &values, const vector<int> &weights, int capacity);
    void runDemo();
    void runBenchmark(int size);
}

namespace HuffmanCoding
{
    struct Node;
    unordered_map<char, string> buildHuffmanCodes(const string &text);
    string encode(const string &text, const unordered_map<char, string> &huffmanCodes);
    string decode(const string &encodedText, Node *root);
    void runDemo();
    void runBenchmark(int size);
}

namespace CoinChange
{
    vector<int> greedyApproach(const vector<int> &denominations, int amount);
    vector<int> dynamicProgrammingApproach(const vector<int> &denominations, int amount);
    void compareApproaches();
    void runDemo();
}

namespace JobScheduling
{
    struct Job;
    vector<int> scheduleJobs(vector<Job> &jobs, int deadline);
    void runDemo();
}

namespace MST
{
    // Prim's and Kruskal's algorithms are greedy approaches for MST
    struct Edge;
    vector<Edge> kruskalMST(vector<Edge> &edges, int V);
    void runDemo();
}

// Utility functions
void printDivider();
void printHeader(const string &title);

/**
 * @brief Main function to demonstrate and benchmark greedy algorithms
 */
int main()
{
    printHeader("GREEDY ALGORITHMS - COMPREHENSIVE IMPLEMENTATIONS");
    cout << "DSAin45 Day 38: Exploring the Power of Greedy Algorithms" << endl
         << endl;

    int choice;

    do
    {
        cout << "\nPlease select an algorithm to demonstrate:\n";
        cout << "1. Activity Selection Problem\n";
        cout << "2. Fractional Knapsack Problem\n";
        cout << "3. Huffman Coding\n";
        cout << "4. Coin Change Problem (Greedy vs DP)\n";
        cout << "5. Job Scheduling with Deadlines\n";
        cout << "6. Minimum Spanning Tree (Kruskal's Algorithm)\n";
        cout << "7. Run All Benchmarks\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            ActivitySelection::runDemo();
            break;
        case 2:
            FractionalKnapsack::runDemo();
            break;
        case 3:
            HuffmanCoding::runDemo();
            break;
        case 4:
            CoinChange::runDemo();
            break;
        case 5:
            JobScheduling::runDemo();
            break;
        case 6:
            MST::runDemo();
            break;
        case 7:
            ActivitySelection::runBenchmark(1000);
            FractionalKnapsack::runBenchmark(1000);
            HuffmanCoding::runBenchmark(10000);
            CoinChange::compareApproaches();
            break;
        case 0:
            cout << "Exiting program. Thank you for exploring greedy algorithms!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}

// Utility function implementations
void printDivider()
{
    cout << "\n"
         << string(80, '-') << "\n"
         << endl;
}

void printHeader(const string &title)
{
    printDivider();
    cout << title << endl;
    printDivider();
}

/**
 * @namespace ActivitySelection
 * @brief Implementation of the Activity Selection Problem
 *
 * Given a set of activities with start and finish times, select the maximum
 * number of non-overlapping activities that can be performed.
 */
namespace ActivitySelection
{
    /**
     * @struct Activity
     * @brief Represents an activity with start and finish times
     */
    struct Activity
    {
        int start;
        int finish;
        int index;

        Activity(int s, int f, int i) : start(s), finish(f), index(i) {}

        // Overload operator for sorting by finish time
        bool operator<(const Activity &other) const
        {
            return finish < other.finish;
        }
    };

    /**
     * @brief Greedy algorithm for activity selection
     * @param start Vector of start times
     * @param finish Vector of finish times
     * @return Vector of indices of selected activities
     * @time_complexity O(n log n) due to sorting
     * @space_complexity O(n) for storing activities and result
     */
    vector<int> greedy(const vector<int> &start, const vector<int> &finish)
    {
        int n = start.size();
        vector<Activity> activities;

        // Create activities with indices
        for (int i = 0; i < n; i++)
        {
            activities.emplace_back(start[i], finish[i], i);
        }

        // Sort activities by finish time
        sort(activities.begin(), activities.end());

        vector<int> result;
        result.push_back(activities[0].index); // Select first activity

        int lastFinishTime = activities[0].finish;

        // Greedily select next activities
        for (int i = 1; i < n; i++)
        {
            // If this activity starts after the last selected activity finishes
            if (activities[i].start >= lastFinishTime)
            {
                result.push_back(activities[i].index);
                lastFinishTime = activities[i].finish;
            }
        }

        return result;
    }

    /**
     * @brief Demonstration of the Activity Selection algorithm
     */
    void runDemo()
    {
        printHeader("ACTIVITY SELECTION PROBLEM");

        cout << "Given a set of activities with start and finish times,\n";
        cout << "select the maximum number of non-overlapping activities.\n\n";

        // Example 1: Standard case
        vector<int> start = {1, 3, 0, 5, 8, 5};
        vector<int> finish = {2, 4, 6, 7, 9, 9};

        cout << "Example 1: Standard case\n";
        cout << "Activities:\n";
        for (int i = 0; i < start.size(); i++)
        {
            cout << "Activity " << i << ": Start = " << start[i] << ", Finish = " << finish[i] << endl;
        }

        vector<int> selected = greedy(start, finish);

        cout << "\nSelected activities: ";
        for (int idx : selected)
        {
            cout << idx << " ";
        }
        cout << "\nTotal activities selected: " << selected.size() << endl;

        // Example 2: Activities with same finish time
        start = {1, 3, 0, 5, 3, 5, 6, 8, 8, 2, 12};
        finish = {4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16};

        cout << "\nExample 2: More complex case\n";
        cout << "Activities:\n";
        for (int i = 0; i < start.size(); i++)
        {
            cout << "Activity " << i << ": Start = " << start[i] << ", Finish = " << finish[i] << endl;
        }

        selected = greedy(start, finish);

        cout << "\nSelected activities: ";
        for (int idx : selected)
        {
            cout << idx << " ";
        }
        cout << "\nTotal activities selected: " << selected.size() << endl;

        // Example 3: Edge case - empty input
        start = {};
        finish = {};
        selected = greedy(start, finish);

        cout << "\nExample 3: Empty input\n";
        cout << "Selected activities: ";
        for (int idx : selected)
        {
            cout << idx << " ";
        }
        cout << "\nTotal activities selected: " << selected.size() << endl;

        // Real-world application
        cout << "\nReal-world Application: Meeting Room Scheduling\n";
        cout << "Imagine you have multiple meeting requests and only one conference room.\n";
        cout << "The activity selection algorithm helps maximize the number of meetings that can be held.\n";

        // Meeting times (hours in 24-hour format)
        vector<int> meetingStart = {9, 10, 11, 12, 13, 14, 15};
        vector<int> meetingEnd = {10, 11, 12, 13, 15, 16, 17};

        cout << "\nMeeting requests:\n";
        for (int i = 0; i < meetingStart.size(); i++)
        {
            cout << "Meeting " << i << ": " << meetingStart[i] << ":00 - " << meetingEnd[i] << ":00" << endl;
        }

        selected = greedy(meetingStart, meetingEnd);

        cout << "\nOptimal meeting schedule: ";
        for (int idx : selected)
        {
            cout << "\nMeeting " << idx << ": " << meetingStart[idx] << ":00 - " << meetingEnd[idx] << ":00";
        }
        cout << "\nTotal meetings scheduled: " << selected.size() << endl;
    }

    /**
     * @brief Benchmark the Activity Selection algorithm
     * @param size Number of activities to generate
     */
    void runBenchmark(int size)
    {
        printHeader("ACTIVITY SELECTION BENCHMARK");

        // Generate random activities
        vector<int> start(size);
        vector<int> finish(size);

        // Seed for reproducibility
        srand(42);

        for (int i = 0; i < size; i++)
        {
            start[i] = rand() % 1000;
            // Ensure finish time is after start time
            finish[i] = start[i] + 1 + rand() % 100;
        }

        cout << "Benchmarking Activity Selection with " << size << " activities..." << endl;

        auto startTime = high_resolution_clock::now();
        vector<int> result = greedy(start, finish);
        auto endTime = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(endTime - startTime);

        cout << "Execution time: " << duration.count() / 1000.0 << " milliseconds" << endl;
        cout << "Selected " << result.size() << " activities out of " << size << endl;
    }
} // namespace ActivitySelection

/**
 * @namespace FractionalKnapsack
 * @brief Implementation of the Fractional Knapsack Problem
 *
 * Given weights and values of n items, put these items in a knapsack of capacity W
 * to get the maximum total value. Items can be broken into fractions.
 */
namespace FractionalKnapsack
{
    /**
     * @struct Item
     * @brief Represents an item with value and weight
     */
    struct Item
    {
        int value;
        int weight;
        double ratio; // value per unit weight
        int index;

        Item(int v, int w, int i) : value(v), weight(w), ratio((double)v / w), index(i) {}

        // Overload operator for sorting by value-to-weight ratio
        bool operator<(const Item &other) const
        {
            return ratio < other.ratio;
        }
    };

    /**
     * @brief Greedy algorithm for fractional knapsack
     * @param values Vector of item values
     * @param weights Vector of item weights
     * @param capacity Knapsack capacity
     * @return Maximum possible value
     * @time_complexity O(n log n) due to sorting
     * @space_complexity O(n) for storing items
     */
    double greedy(const vector<int> &values, const vector<int> &weights, int capacity)
    {
        int n = values.size();
        vector<Item> items;

        // Create items with indices
        for (int i = 0; i < n; i++)
        {
            items.emplace_back(values[i], weights[i], i);
        }

        // Sort items by value-to-weight ratio in descending order
        sort(items.rbegin(), items.rend());

        double totalValue = 0.0;
        int remainingCapacity = capacity;
        vector<pair<int, double>> selectedItems; // (index, fraction)

        for (const auto &item : items)
        {
            if (remainingCapacity >= item.weight)
            {
                // Take the whole item
                totalValue += item.value;
                remainingCapacity -= item.weight;
                selectedItems.push_back({item.index, 1.0});
            }
            else
            {
                // Take a fraction of the item
                double fraction = (double)remainingCapacity / item.weight;
                totalValue += item.value * fraction;
                selectedItems.push_back({item.index, fraction});
                break; // Knapsack is full
            }
        }

        // Print selected items
        cout << "Selected items:\n";
        for (const auto &[index, fraction] : selectedItems)
        {
            cout << "Item " << index << ": ";
            if (fraction == 1.0)
            {
                cout << "100% (complete item)\n";
            }
            else
            {
                cout << fixed << setprecision(2) << fraction * 100 << "% (partial item)\n";
            }
        }

        return totalValue;
    }

    /**
     * @brief Demonstration of the Fractional Knapsack algorithm
     */
    void runDemo()
    {
        printHeader("FRACTIONAL KNAPSACK PROBLEM");

        cout << "Given weights and values of n items, put these items in a knapsack\n";
        cout << "of capacity W to get the maximum total value. Items can be broken into fractions.\n\n";

        // Example 1: Standard case
        vector<int> values = {60, 100, 120};
        vector<int> weights = {10, 20, 30};
        int capacity = 50;

        cout << "Example 1: Standard case\n";
        cout << "Items:\n";
        for (int i = 0; i < values.size(); i++)
        {
            cout << "Item " << i << ": Value = " << values[i] << ", Weight = " << weights[i]
                 << ", Value/Weight = " << fixed << setprecision(2) << (double)values[i] / weights[i] << endl;
        }
        cout << "Knapsack capacity: " << capacity << endl;

        double maxValue = greedy(values, weights, capacity);

        cout << "\nMaximum value: " << fixed << setprecision(2) << maxValue << endl;

        // Example 2: More items
        values = {500, 400, 300, 450, 600, 150};
        weights = {5, 4, 6, 3, 9, 2};
        capacity = 20;

        cout << "\nExample 2: More complex case\n";
        cout << "Items:\n";
        for (int i = 0; i < values.size(); i++)
        {
            cout << "Item " << i << ": Value = " << values[i] << ", Weight = " << weights[i]
                 << ", Value/Weight = " << fixed << setprecision(2) << (double)values[i] / weights[i] << endl;
        }
        cout << "Knapsack capacity: " << capacity << endl;

        maxValue = greedy(values, weights, capacity);

        cout << "\nMaximum value: " << fixed << setprecision(2) << maxValue << endl;

        // Real-world application
        cout << "\nReal-world Application: Cargo Loading\n";
        cout << "A cargo ship has limited weight capacity and must choose which goods to transport.\n";
        cout << "Each type of cargo has a different value per unit weight.\n";

        // Cargo values (in thousands of dollars) and weights (in tons)
        vector<int> cargoValues = {100, 280, 120, 120, 500, 80};
        vector<int> cargoWeights = {10, 40, 20, 24, 100, 20};
        int shipCapacity = 150; // tons

        cout << "\nCargo options:\n";
        for (int i = 0; i < cargoValues.size(); i++)
        {
            cout << "Cargo " << i << ": Value = $" << cargoValues[i] << "K, Weight = " << cargoWeights[i]
                 << " tons, Value/Weight = $" << fixed << setprecision(2) << (double)cargoValues[i] / cargoWeights[i] << "K per ton" << endl;
        }
        cout << "Ship capacity: " << shipCapacity << " tons" << endl;

        maxValue = greedy(cargoValues, cargoWeights, shipCapacity);

        cout << "\nMaximum cargo value: $" << fixed << setprecision(2) << maxValue << "K" << endl;
    }

    /**
     * @brief Benchmark the Fractional Knapsack algorithm
     * @param size Number of items to generate
     */
    void runBenchmark(int size)
    {
        printHeader("FRACTIONAL KNAPSACK BENCHMARK");

        // Generate random items
        vector<int> values(size);
        vector<int> weights(size);

        // Seed for reproducibility
        srand(42);

        for (int i = 0; i < size; i++)
        {
            values[i] = 1 + rand() % 1000;
            weights[i] = 1 + rand() % 100;
        }

        int capacity = (rand() % 50 + 20) * size / 100; // 20-70% of total weight

        cout << "Benchmarking Fractional Knapsack with " << size << " items..." << endl;
        cout << "Knapsack capacity: " << capacity << endl;

        auto startTime = high_resolution_clock::now();
        double result = greedy(values, weights, capacity);
        auto endTime = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(endTime - startTime);

        cout << "Execution time: " << duration.count() / 1000.0 << " milliseconds" << endl;
        cout << "Maximum value: " << fixed << setprecision(2) << result << endl;
    }
} // namespace FractionalKnapsack

/**
 * @namespace HuffmanCoding
 * @brief Implementation of the Huffman Coding algorithm
 *
 * Huffman coding is a lossless data compression algorithm that assigns variable-length
 * codes to input characters, with shorter codes for more frequent characters.
 */
namespace HuffmanCoding
{
    /**
     * @struct Node
     * @brief Node structure for Huffman tree
     */
    struct Node
    {
        char data;
        unsigned freq;
        Node *left, *right;

        Node(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}

        // Destructor to clean up the tree
        ~Node()
        {
            delete left;
            delete right;
        }
    };

    /**
     * @struct Compare
     * @brief Comparison function for priority queue
     */
    struct Compare
    {
        bool operator()(Node *l, Node *r)
        {
            return l->freq > r->freq;
        }
    };

    /**
     * @brief Helper function to print Huffman codes
     * @param root Root of Huffman tree
     * @param str Current code string
     * @param huffmanCode Map to store codes
     */
    void printCodes(Node *root, string str, unordered_map<char, string> &huffmanCode)
    {
        if (!root)
            return;

        // If this is a leaf node, store its code
        if (!root->left && !root->right)
        {
            huffmanCode[root->data] = str;
        }

        // Traverse left with code 0
        printCodes(root->left, str + "0", huffmanCode);
        // Traverse right with code 1
        printCodes(root->right, str + "1", huffmanCode);
    }

    /**
     * @brief Build Huffman codes for given text
     * @param text Input text
     * @return Map of characters to their Huffman codes
     * @time_complexity O(n log n) where n is the number of unique characters
     * @space_complexity O(n) for storing the Huffman tree and codes
     */
    unordered_map<char, string> buildHuffmanCodes(const string &text)
    {
        // Count frequency of characters
        unordered_map<char, unsigned> freq;
        for (char c : text)
        {
            freq[c]++;
        }

        // Create a priority queue (min-heap)
        priority_queue<Node *, vector<Node *>, Compare> pq;

        // Create a leaf node for each character and add to priority queue
        for (auto pair : freq)
        {
            pq.push(new Node(pair.first, pair.second));
        }

        // Build Huffman Tree
        while (pq.size() > 1)
        {
            // Extract two nodes with lowest frequency
            Node *left = pq.top();
            pq.pop();
            Node *right = pq.top();
            pq.pop();

            // Create a new internal node with these two nodes as children
            // and with frequency equal to sum of the two nodes' frequencies
            Node *top = new Node('$', left->freq + right->freq);
            top->left = left;
            top->right = right;

            // Add this node to the priority queue
            pq.push(top);
        }

        // The remaining node is the root of Huffman Tree
        Node *root = pq.top();

        // Print Huffman codes
        unordered_map<char, string> huffmanCode;
        printCodes(root, "", huffmanCode);

        // Clean up the tree (except root, which we'll use for decoding)
        // root will be cleaned up in the demo function

        return huffmanCode;
    }

    /**
     * @brief Encode text using Huffman codes
     * @param text Input text
     * @param huffmanCode Map of characters to their Huffman codes
     * @return Encoded text
     */
    string encode(const string &text, const unordered_map<char, string> &huffmanCode)
    {
        string encodedText = "";
        for (char c : text)
        {
            encodedText += huffmanCode.at(c);
        }
        return encodedText;
    }

    /**
     * @brief Decode Huffman-encoded text
     * @param encodedText Encoded text
     * @param root Root of Huffman tree
     * @return Decoded text
     */
    string decode(const string &encodedText, Node *root)
    {
        string decodedText = "";
        Node *current = root;

        for (char bit : encodedText)
        {
            // Traverse the Huffman tree according to the bits
            if (bit == '0')
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }

            // If leaf node, add character and reset to root
            if (!current->left && !current->right)
            {
                decodedText += current->data;
                current = root;
            }
        }

        return decodedText;
    }

    /**
     * @brief Demonstration of the Huffman Coding algorithm
     */
    void runDemo()
    {
        printHeader("HUFFMAN CODING");

        cout << "Huffman coding is a lossless data compression algorithm that assigns\n";
        cout << "variable-length codes to input characters, with shorter codes for more frequent characters.\n\n";

        // Example 1: Simple text
        string text = "AAAABBBCCD";

        cout << "Example 1: Simple text\n";
        cout << "Text: " << text << endl;

        unordered_map<char, string> huffmanCode = buildHuffmanCodes(text);

        cout << "\nHuffman Codes:\n";
        for (auto pair : huffmanCode)
        {
            cout << pair.first << ": " << pair.second << endl;
        }

        string encodedText = encode(text, huffmanCode);
        cout << "\nEncoded text: " << encodedText << endl;

        // Re-generate the Huffman tree for decoding
        priority_queue<Node *, vector<Node *>, Compare> pq;
        unordered_map<char, unsigned> freq;
        for (char c : text)
        {
            freq[c]++;
        }
        for (auto pair : freq)
        {
            pq.push(new Node(pair.first, pair.second));
        }
        while (pq.size() > 1)
        {
            Node *left = pq.top();
            pq.pop();
            Node *right = pq.top();
            pq.pop();
            Node *top = new Node('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
            pq.push(top);
        }
        Node *root = pq.top();

        string decodedText = decode(encodedText, root);
        cout << "Decoded text: " << decodedText << endl;

        cout << "\nCompression ratio: " << fixed << setprecision(2)
             << (double)(text.length() * 8) / encodedText.length() << endl;

        // Example 2: More complex text
        delete root; // Clean up previous example

        text = "The quick brown fox jumps over the lazy dog";

        cout << "\nExample 2: More complex text\n";
        cout << "Text: " << text << endl;

        huffmanCode = buildHuffmanCodes(text);

        cout << "\nHuffman Codes:\n";
        for (auto pair : huffmanCode)
        {
            if (pair.first == ' ')
            {
                cout << "SPACE: " << pair.second << endl;
            }
            else
            {
                cout << pair.first << ": " << pair.second << endl;
            }
        }

        encodedText = encode(text, huffmanCode);
        cout << "\nEncoded text: " << encodedText << endl;

        // Re-generate the Huffman tree for decoding
        pq = priority_queue<Node *, vector<Node *>, Compare>();
        freq.clear();
        for (char c : text)
        {
            freq[c]++;
        }
        for (auto pair : freq)
        {
            pq.push(new Node(pair.first, pair.second));
        }
        while (pq.size() > 1)
        {
            Node *left = pq.top();
            pq.pop();
            Node *right = pq.top();
            pq.pop();
            Node *top = new Node('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
            pq.push(top);
        }
        root = pq.top();

        decodedText = decode(encodedText, root);
        cout << "Decoded text: " << decodedText << endl;

        double originalSize = text.length() * 8; // 8 bits per ASCII character
        double compressedSize = encodedText.length();
        double compressionRatio = originalSize / compressedSize;

        cout << "\nOriginal size: " << originalSize << " bits" << endl;
        cout << "Compressed size: " << compressedSize << " bits" << endl;
        cout << "Compression ratio: " << fixed << setprecision(2) << compressionRatio << endl;

        // Real-world application
        delete root; // Clean up previous example

        cout << "\nReal-world Application: Text Compression\n";
        cout << "Huffman coding is used in many compression algorithms, such as GZIP and JPEG.\n";

        string repeatedText = "";
        for (int i = 0; i < 10; i++)
        {
            repeatedText += "This is a sample text with many repeated words. ";
        }

        cout << "\nCompressing a text with many repetitions:\n";
        cout << "Original text length: " << repeatedText.length() << " characters" << endl;

        huffmanCode = buildHuffmanCodes(repeatedText);
        encodedText = encode(repeatedText, huffmanCode);

        originalSize = repeatedText.length() * 8;
        compressedSize = encodedText.length();
        compressionRatio = originalSize / compressedSize;

        cout << "Original size: " << originalSize << " bits" << endl;
        cout << "Compressed size: " << compressedSize << " bits" << endl;
        cout << "Compression ratio: " << fixed << setprecision(2) << compressionRatio << endl;

        // Clean up the last Huffman tree
        delete pq.top();
    }

    /**
     * @brief Benchmark the Huffman Coding algorithm
     * @param size Length of text to generate
     */
    void runBenchmark(int size)
    {
        printHeader("HUFFMAN CODING BENCHMARK");

        // Generate random text
        string text = "";

        // Seed for reproducibility
        srand(42);

        // Add some bias to make compression more effective
        const string charset = "AAAABBBBCCCCDDEEEEFFFFGGGHHIIIIJJKKLLMMMNNOOOOOO";

        for (int i = 0; i < size; i++)
        {
            text += charset[rand() % charset.length()];
        }

        cout << "Benchmarking Huffman Coding with " << size << " characters of text..." << endl;

        auto startTime = high_resolution_clock::now();
        unordered_map<char, string> huffmanCode = buildHuffmanCodes(text);
        string encodedText = encode(text, huffmanCode);
        auto endTime = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(endTime - startTime);

        double originalSize = text.length() * 8; // 8 bits per ASCII character
        double compressedSize = encodedText.length();
        double compressionRatio = originalSize / compressedSize;

        cout << "Execution time: " << duration.count() / 1000.0 << " milliseconds" << endl;
        cout << "Original size: " << originalSize << " bits" << endl;
        cout << "Compressed size: " << compressedSize << " bits" << endl;
        cout << "Compression ratio: " << fixed << setprecision(2) << compressionRatio << endl;

        // Clean up the Huffman tree by regenerating it (to get the root) and then deleting it
        priority_queue<Node *, vector<Node *>, Compare> pq;
        unordered_map<char, unsigned> freq;
        for (char c : text)
        {
            freq[c]++;
        }
        for (auto pair : freq)
        {
            pq.push(new Node(pair.first, pair.second));
        }
        while (pq.size() > 1)
        {
            Node *left = pq.top();
            pq.pop();
            Node *right = pq.top();
            pq.pop();
            Node *top = new Node('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
            pq.push(top);
        }
        Node *root = pq.top();
        delete root;
    }
} // namespace HuffmanCoding

/**
 * @namespace CoinChange
 * @brief Implementation of the Coin Change Problem
 *
 * Given a set of coin denominations and an amount, find the minimum
 * number of coins needed to make up that amount.
 */
namespace CoinChange
{
    /**
     * @brief Greedy approach to the coin change problem
     * @param denominations Vector of coin denominations
     * @param amount Target amount
     * @return Vector of coins used
     * @time_complexity O(n log n + amount) where n is the number of denominations
     * @space_complexity O(n) for storing result
     * @note This algorithm is not guaranteed to be optimal for all coin systems
     */
    vector<int> greedyApproach(const vector<int> &denominations, int amount)
    {
        vector<int> sortedDenominations = denominations;
        sort(sortedDenominations.rbegin(), sortedDenominations.rend()); // Sort in descending order

        vector<int> result;
        int remaining = amount;

        for (int coin : sortedDenominations)
        {
            while (remaining >= coin)
            {
                result.push_back(coin);
                remaining -= coin;
            }
        }

        // Check if we were able to make exact change
        return (remaining == 0) ? result : vector<int>();
    }

    /**
     * @brief Dynamic programming approach to the coin change problem
     * @param denominations Vector of coin denominations
     * @param amount Target amount
     * @return Vector of coins used
     * @time_complexity O(amount * n) where n is the number of denominations
     * @space_complexity O(amount) for dp array
     */
    vector<int> dynamicProgrammingApproach(const vector<int> &denominations, int amount)
    {
        // DP array to store minimum number of coins for each amount
        vector<int> dp(amount + 1, amount + 1); // Initialize with a value larger than possible
        // DP array to store the last coin used
        vector<int> lastCoin(amount + 1, -1);

        dp[0] = 0; // Base case: 0 coins needed to make amount 0

        for (int coin : denominations)
        {
            for (int i = coin; i <= amount; i++)
            {
                if (dp[i - coin] + 1 < dp[i])
                {
                    dp[i] = dp[i - coin] + 1;
                    lastCoin[i] = coin;
                }
            }
        }

        // If no solution exists
        if (dp[amount] > amount)
        {
            return vector<int>();
        }

        // Reconstruct the solution
        vector<int> result;
        int remaining = amount;
        while (remaining > 0)
        {
            result.push_back(lastCoin[remaining]);
            remaining -= lastCoin[remaining];
        }

        return result;
    }

    /**
     * @brief Compare greedy and dynamic programming approaches
     */
    void compareApproaches()
    {
        printHeader("COIN CHANGE: GREEDY VS DYNAMIC PROGRAMMING");

        // Coin systems where greedy works
        cout << "1. US Coin System (where greedy approach works)\n";
        vector<int> usDenominations = {1, 5, 10, 25};
        int amount = 63;

        cout << "Amount: " << amount << " cents\n";
        cout << "Denominations: ";
        for (int coin : usDenominations)
        {
            cout << coin << " ";
        }
        cout << endl;

        cout << "\nGreedy approach: ";
        auto startTime = high_resolution_clock::now();
        vector<int> greedyResult = greedyApproach(usDenominations, amount);
        auto endTime = high_resolution_clock::now();
        auto greedyTime = duration_cast<microseconds>(endTime - startTime);

        if (greedyResult.empty())
        {
            cout << "No solution found" << endl;
        }
        else
        {
            cout << greedyResult.size() << " coins: ";
            for (int coin : greedyResult)
            {
                cout << coin << " ";
            }
            cout << endl;
        }

        cout << "Dynamic programming approach: ";
        startTime = high_resolution_clock::now();
        vector<int> dpResult = dynamicProgrammingApproach(usDenominations, amount);
        endTime = high_resolution_clock::now();
        auto dpTime = duration_cast<microseconds>(endTime - startTime);

        if (dpResult.empty())
        {
            cout << "No solution found" << endl;
        }
        else
        {
            cout << dpResult.size() << " coins: ";
            for (int coin : dpResult)
            {
                cout << coin << " ";
            }
            cout << endl;
        }

        cout << "\nExecution times:\n";
        cout << "Greedy: " << greedyTime.count() << " microseconds\n";
        cout << "DP: " << dpTime.count() << " microseconds\n";

        // Coin system where greedy fails
        cout << "\n2. Non-canonical Coin System (where greedy approach fails)\n";
        vector<int> nonCanonicalDenominations = {1, 3, 4};
        amount = 6;

        cout << "Amount: " << amount << " cents\n";
        cout << "Denominations: ";
        for (int coin : nonCanonicalDenominations)
        {
            cout << coin << " ";
        }
        cout << endl;

        cout << "\nGreedy approach: ";
        startTime = high_resolution_clock::now();
        greedyResult = greedyApproach(nonCanonicalDenominations, amount);
        endTime = high_resolution_clock::now();
        greedyTime = duration_cast<microseconds>(endTime - startTime);

        if (greedyResult.empty())
        {
            cout << "No solution found" << endl;
        }
        else
        {
            cout << greedyResult.size() << " coins: ";
            for (int coin : greedyResult)
            {
                cout << coin << " ";
            }
            cout << endl;
        }

        cout << "Dynamic programming approach: ";
        startTime = high_resolution_clock::now();
        dpResult = dynamicProgrammingApproach(nonCanonicalDenominations, amount);
        endTime = high_resolution_clock::now();
        dpTime = duration_cast<microseconds>(endTime - startTime);

        if (dpResult.empty())
        {
            cout << "No solution found" << endl;
        }
        else
        {
            cout << dpResult.size() << " coins: ";
            for (int coin : dpResult)
            {
                cout << coin << " ";
            }
            cout << endl;
        }

        cout << "\nExecution times:\n";
        cout << "Greedy: " << greedyTime.count() << " microseconds\n";
        cout << "DP: " << dpTime.count() << " microseconds\n";

        cout << "\nConclusion: The greedy approach works for canonical coin systems\n";
        cout << "(such as the US currency) but fails for non-canonical systems.\n";
        cout << "Dynamic programming, while slower, always produces the optimal solution.\n";
    }

    /**
     * @brief Demonstration of the Coin Change algorithms
     */
    void runDemo()
    {
        printHeader("COIN CHANGE PROBLEM");

        cout << "Given a set of coin denominations and an amount,\n";
        cout << "find the minimum number of coins needed to make up that amount.\n\n";

        compareApproaches();

        // Real-world application
        cout << "\nReal-world Application: ATM Cash Dispensing\n";
        cout << "An ATM needs to dispense the requested amount using the minimum number of bills.\n";

        vector<int> atmDenominations = {1, 5, 10, 20, 50, 100};
        int withdrawalAmount = 178;

        cout << "\nWithdrawal amount: $" << withdrawalAmount << endl;
        cout << "Available bill denominations: ";
        for (int bill : atmDenominations)
        {
            cout << "$" << bill << " ";
        }
        cout << endl;

        vector<int> dispensedBills = greedyApproach(atmDenominations, withdrawalAmount);

        cout << "\nDispensed bills: ";
        if (dispensedBills.empty())
        {
            cout << "Unable to dispense exact amount" << endl;
        }
        else
        {
            unordered_map<int, int> billCount;
            for (int bill : dispensedBills)
            {
                billCount[bill]++;
            }

            for (auto [bill, count] : billCount)
            {
                cout << count << " x $" << bill << ", ";
            }
            cout << "\nTotal bills: " << dispensedBills.size() << endl;
        }
    }
} // namespace CoinChange

/**
 * @namespace JobScheduling
 * @brief Implementation of the Job Scheduling with Deadlines Problem
 *
 * Given a set of jobs with profits and deadlines, schedule the jobs
 * to maximize profit, assuming only one job can be scheduled at a time.
 */
namespace JobScheduling
{
    /**
     * @struct Job
     * @brief Represents a job with ID, deadline, and profit
     */
    struct Job
    {
        int id;
        int deadline;
        int profit;

        Job(int id, int deadline, int profit) : id(id), deadline(deadline), profit(profit) {}

        // Overload operator for sorting by profit
        bool operator<(const Job &other) const
        {
            return profit < other.profit;
        }
    };

    /**
     * @brief Greedy algorithm for job scheduling with deadlines
     * @param jobs Vector of jobs
     * @param maxDeadline Maximum deadline among all jobs
     * @return Vector of scheduled job IDs
     * @time_complexity O(n log n + n * maxDeadline) where n is the number of jobs
     * @space_complexity O(maxDeadline) for storing scheduled slots
     */
    vector<int> scheduleJobs(vector<Job> &jobs, int maxDeadline)
    {
        // Sort jobs by profit in descending order
        sort(jobs.rbegin(), jobs.rend());

        // Initialize result array and slot array
        vector<int> result;
        vector<bool> slot(maxDeadline, false); // slot[i] = false means slot i is free

        // Iterate through all jobs
        for (const auto &job : jobs)
        {
            // Find a free slot for this job
            for (int i = min(maxDeadline - 1, job.deadline - 1); i >= 0; i--)
            {
                // If free slot found, add job to result and mark slot as occupied
                if (!slot[i])
                {
                    result.push_back(job.id);
                    slot[i] = true;
                    break;
                }
            }
        }

        return result;
    }

    /**
     * @brief Demonstration of the Job Scheduling algorithm
     */
    void runDemo()
    {
        printHeader("JOB SCHEDULING WITH DEADLINES");

        cout << "Given a set of jobs with profits and deadlines, schedule the jobs\n";
        cout << "to maximize profit, assuming only one job can be scheduled at a time.\n\n";

        // Example 1: Standard case
        vector<Job> jobs = {
            Job(1, 4, 20),
            Job(2, 1, 10),
            Job(3, 1, 40),
            Job(4, 1, 30)};

        int maxDeadline = 0;
        for (const auto &job : jobs)
        {
            maxDeadline = max(maxDeadline, job.deadline);
        }

        cout << "Example 1: Standard case\n";
        cout << "Jobs:\n";
        for (const auto &job : jobs)
        {
            cout << "Job " << job.id << ": Deadline = " << job.deadline << ", Profit = " << job.profit << endl;
        }

        vector<int> scheduled = scheduleJobs(jobs, maxDeadline);

        cout << "\nScheduled jobs: ";
        for (int id : scheduled)
        {
            cout << id << " ";
        }
        cout << endl;

        int totalProfit = 0;
        for (int id : scheduled)
        {
            for (const auto &job : jobs)
            {
                if (job.id == id)
                {
                    totalProfit += job.profit;
                    break;
                }
            }
        }
        cout << "Total profit: " << totalProfit << endl;

        // Example 2: More jobs
        jobs = {
            Job(1, 2, 100),
            Job(2, 1, 19),
            Job(3, 2, 27),
            Job(4, 1, 25),
            Job(5, 3, 15)};

        maxDeadline = 0;
        for (const auto &job : jobs)
        {
            maxDeadline = max(maxDeadline, job.deadline);
        }

        cout << "\nExample 2: More complex case\n";
        cout << "Jobs:\n";
        for (const auto &job : jobs)
        {
            cout << "Job " << job.id << ": Deadline = " << job.deadline << ", Profit = " << job.profit << endl;
        }

        scheduled = scheduleJobs(jobs, maxDeadline);

        cout << "\nScheduled jobs: ";
        for (int id : scheduled)
        {
            cout << id << " ";
        }
        cout << endl;

        totalProfit = 0;
        for (int id : scheduled)
        {
            for (const auto &job : jobs)
            {
                if (job.id == id)
                {
                    totalProfit += job.profit;
                    break;
                }
            }
        }
        cout << "Total profit: " << totalProfit << endl;

        // Real-world application
        cout << "\nReal-world Application: Freelance Job Selection\n";
        cout << "A freelancer needs to select which jobs to take from various clients,\n";
        cout << "each with their own deadline and payment.\n";

        vector<Job> freelanceJobs = {
            Job(1, 7, 200), // Project 1, due in 7 days, pays $200
            Job(2, 3, 180), // Project 2, due in 3 days, pays $180
            Job(3, 1, 120), // Project 3, due in 1 day, pays $120
            Job(4, 2, 100), // Project 4, due in 2 days, pays $100
            Job(5, 5, 250), // Project 5, due in 5 days, pays $250
            Job(6, 4, 150)  // Project 6, due in 4 days, pays $150
        };

        maxDeadline = 0;
        for (const auto &job : freelanceJobs)
        {
            maxDeadline = max(maxDeadline, job.deadline);
        }

        cout << "\nFreelance projects:\n";
        for (const auto &job : freelanceJobs)
        {
            cout << "Project " << job.id << ": Due in " << job.deadline << " days, Pays $" << job.profit << endl;
        }

        scheduled = scheduleJobs(freelanceJobs, maxDeadline);

        cout << "\nOptimal project selection: ";
        for (int id : scheduled)
        {
            cout << "Project " << id << ", ";
        }
        cout << endl;

        totalProfit = 0;
        for (int id : scheduled)
        {
            for (const auto &job : freelanceJobs)
            {
                if (job.id == id)
                {
                    totalProfit += job.profit;
                    break;
                }
            }
        }
        cout << "Total earnings: $" << totalProfit << endl;
    }
} // namespace JobScheduling

/**
 * @namespace MST
 * @brief Implementation of Minimum Spanning Tree algorithms
 *
 * Minimum Spanning Tree (MST) is a subset of the edges of a connected,
 * edge-weighted undirected graph that connects all the vertices together,
 * without any cycles and with the minimum possible total edge weight.
 */
namespace MST
{
    /**
     * @struct Edge
     * @brief Represents an edge in a graph
     */
    struct Edge
    {
        int src, dest, weight;

        Edge(int src, int dest, int weight) : src(src), dest(dest), weight(weight) {}

        // Overload operator for sorting by weight
        bool operator<(const Edge &other) const
        {
            return weight < other.weight;
        }
    };

    /**
     * @struct DisjointSet
     * @brief Data structure for disjoint-set operations
     */
    struct DisjointSet
    {
        vector<int> parent, rank;

        DisjointSet(int n)
        {
            parent.resize(n);
            rank.resize(n, 0);

            // Initialize parent array (each element is its own parent)
            for (int i = 0; i < n; i++)
            {
                parent[i] = i;
            }
        }

        // Find the parent of a vertex (with path compression)
        int find(int x)
        {
            if (parent[x] != x)
            {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        }

        // Union by rank
        void unionSets(int x, int y)
        {
            int rootX = find(x);
            int rootY = find(y);

            if (rootX == rootY)
                return;

            if (rank[rootX] < rank[rootY])
            {
                parent[rootX] = rootY;
            }
            else if (rank[rootX] > rank[rootY])
            {
                parent[rootY] = rootX;
            }
            else
            {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    };

    /**
     * @brief Kruskal's algorithm for finding MST
     * @param edges Vector of edges
     * @param V Number of vertices
     * @return Vector of edges in the MST
     * @time_complexity O(E log E) where E is the number of edges
     * @space_complexity O(E + V) for storing edges and disjoint set
     */
    vector<Edge> kruskalMST(vector<Edge> &edges, int V)
    {
        vector<Edge> result;

        // Sort edges by weight
        sort(edges.begin(), edges.end());

        // Create disjoint set
        DisjointSet ds(V);

        // Process edges in ascending order of weight
        for (const auto &edge : edges)
        {
            int rootSrc = ds.find(edge.src);
            int rootDest = ds.find(edge.dest);

            // If including this edge doesn't form a cycle, include it
            if (rootSrc != rootDest)
            {
                result.push_back(edge);
                ds.unionSets(rootSrc, rootDest);
            }
        }

        return result;
    }

    /**
     * @brief Demonstration of the MST algorithms
     */
    void runDemo()
    {
        printHeader("MINIMUM SPANNING TREE (KRUSKAL'S ALGORITHM)");

        cout << "A Minimum Spanning Tree (MST) is a subset of the edges of a connected,\n";
        cout << "edge-weighted undirected graph that connects all the vertices together,\n";
        cout << "without any cycles and with the minimum possible total edge weight.\n\n";

        // Example 1: Standard case
        int V = 4; // Number of vertices
        vector<Edge> edges = {
            Edge(0, 1, 10),
            Edge(0, 2, 6),
            Edge(0, 3, 5),
            Edge(1, 3, 15),
            Edge(2, 3, 4)};

        cout << "Example 1: Standard case\n";
        cout << "Graph with " << V << " vertices and " << edges.size() << " edges:\n";
        for (const auto &edge : edges)
        {
            cout << edge.src << " -- " << edge.dest << " with weight " << edge.weight << endl;
        }

        vector<Edge> mst = kruskalMST(edges, V);

        cout << "\nEdges in MST:\n";
        int totalWeight = 0;
        for (const auto &edge : mst)
        {
            cout << edge.src << " -- " << edge.dest << " with weight " << edge.weight << endl;
            totalWeight += edge.weight;
        }
        cout << "Total weight of MST: " << totalWeight << endl;

        // Example 2: More complex graph
        V = 6;
        edges = {
            Edge(0, 1, 4),
            Edge(0, 2, 3),
            Edge(1, 2, 1),
            Edge(1, 3, 2),
            Edge(2, 3, 4),
            Edge(2, 4, 5),
            Edge(3, 4, 7),
            Edge(3, 5, 6),
            Edge(4, 5, 3)};

        cout << "\nExample 2: More complex case\n";
        cout << "Graph with " << V << " vertices and " << edges.size() << " edges:\n";
        for (const auto &edge : edges)
        {
            cout << edge.src << " -- " << edge.dest << " with weight " << edge.weight << endl;
        }

        mst = kruskalMST(edges, V);

        cout << "\nEdges in MST:\n";
        totalWeight = 0;
        for (const auto &edge : mst)
        {
            cout << edge.src << " -- " << edge.dest << " with weight " << edge.weight << endl;
            totalWeight += edge.weight;
        }
        cout << "Total weight of MST: " << totalWeight << endl;

        // Real-world application
        cout << "\nReal-world Application: Network Design\n";
        cout << "A company wants to connect its offices with fiber optic cables,\n";
        cout << "minimizing the total length of cable needed.\n";

        vector<string> officeNames = {"Headquarters", "Research", "Sales", "Marketing", "Development"};
        V = officeNames.size();
        edges = {
            Edge(0, 1, 5), // HQ to Research: 5 km
            Edge(0, 2, 3), // HQ to Sales: 3 km
            Edge(0, 3, 4), // HQ to Marketing: 4 km
            Edge(0, 4, 2), // HQ to Development: 2 km
            Edge(1, 2, 6), // Research to Sales: 6 km
            Edge(1, 3, 2), // Research to Marketing: 2 km
            Edge(1, 4, 3), // Research to Development: 3 km
            Edge(2, 3, 1), // Sales to Marketing: 1 km
            Edge(2, 4, 3), // Sales to Development: 3 km
            Edge(3, 4, 4)  // Marketing to Development: 4 km
        };

        cout << "\nOffices to connect:\n";
        for (int i = 0; i < V; i++)
        {
            cout << i << ": " << officeNames[i] << endl;
        }

        cout << "\nPossible cable routes:\n";
        for (const auto &edge : edges)
        {
            cout << officeNames[edge.src] << " to " << officeNames[edge.dest]
                 << ": " << edge.weight << " km" << endl;
        }

        mst = kruskalMST(edges, V);

        cout << "\nOptimal network layout:\n";
        totalWeight = 0;
        for (const auto &edge : mst)
        {
            cout << officeNames[edge.src] << " to " << officeNames[edge.dest]
                 << ": " << edge.weight << " km" << endl;
            totalWeight += edge.weight;
        }
        cout << "Total cable length: " << totalWeight << " km" << endl;
    }
} // namespace MST