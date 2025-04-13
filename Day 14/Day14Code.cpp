// Day 14: Weekly Challenge & Recap #DSAin45
// Implementations of the three challenge problems with extensive testing

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <list>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <chrono>
#include <functional>
#include <optional>
#include <limits>
#include <iomanip>
#include <random>
#include <memory>

// ===== CHALLENGE #1: DOCUMENT WORD FREQUENCY ANALYZER =====

class DocumentAnalyzer
{
private:
    std::vector<std::string> words;                                  // All words in order
    std::vector<std::string> sentences;                              // All sentences
    std::unordered_map<std::string, int> wordFrequency;              // Word frequency count
    std::unordered_map<std::string, std::vector<int>> wordPositions; // Positions of each word
    std::unordered_map<std::string, std::set<int>> wordToSentences;  // Which sentences contain each word

    // Helper function to convert to lowercase and remove punctuation
    std::string normalize(const std::string &word)
    {
        std::string result;
        for (char c : word)
        {
            if (std::isalpha(c))
            {
                result += std::tolower(c);
            }
        }
        return result;
    }

    // Split document into words and sentences
    void processDocument(const std::string &document)
    {
        // Split into sentences
        size_t start = 0;
        size_t end = document.find_first_of(".!?");

        while (end != std::string::npos)
        {
            sentences.push_back(document.substr(start, end - start + 1));
            start = end + 1;

            // Skip whitespace
            while (start < document.length() && std::isspace(document[start]))
            {
                start++;
            }

            end = document.find_first_of(".!?", start);
        }

        // Process remaining text if any
        if (start < document.length())
        {
            sentences.push_back(document.substr(start));
        }

        // Process each sentence
        for (int sentenceIdx = 0; sentenceIdx < sentences.size(); sentenceIdx++)
        {
            std::istringstream iss(sentences[sentenceIdx]);
            std::string word;

            while (iss >> word)
            {
                std::string normalizedWord = normalize(word);
                if (!normalizedWord.empty())
                {
                    words.push_back(normalizedWord);
                    wordFrequency[normalizedWord]++;
                    wordPositions[normalizedWord].push_back(words.size() - 1);
                    wordToSentences[normalizedWord].insert(sentenceIdx);
                }
            }
        }
    }

public:
    DocumentAnalyzer(const std::string &document)
    {
        processDocument(document);
    }

    // Query 1: Find top N frequent words
    std::vector<std::pair<std::string, int>> topNFrequentWords(int n)
    {
        // First approach: Using min heap to keep track of top N words
        auto compare = [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
        {
            return a.second > b.second; // Min heap based on frequency
        };

        std::priority_queue<std::pair<std::string, int>,
                            std::vector<std::pair<std::string, int>>,
                            decltype(compare)>
            minHeap(compare);

        for (const auto &[word, freq] : wordFrequency)
        {
            minHeap.push({word, freq});
            if (minHeap.size() > n)
            {
                minHeap.pop(); // Remove lowest frequency
            }
        }

        // Convert heap to vector in descending order of frequency
        std::vector<std::pair<std::string, int>> result;
        while (!minHeap.empty())
        {
            result.push_back(minHeap.top());
            minHeap.pop();
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    // Alternate implementation using sort
    std::vector<std::pair<std::string, int>> topNFrequentWords_UsingSort(int n)
    {
        std::vector<std::pair<std::string, int>> pairs(wordFrequency.begin(), wordFrequency.end());

        std::sort(pairs.begin(), pairs.end(),
                  [](const auto &a, const auto &b)
                  {
                      if (a.second != b.second)
                      {
                          return a.second > b.second; // Sort by frequency (descending)
                      }
                      return a.first < b.first; // If same frequency, sort alphabetically
                  });

        if (pairs.size() > n)
        {
            pairs.resize(n);
        }

        return pairs;
    }

    // Query 2: Find words that appear exactly K times
    std::vector<std::string> wordsWithExactFrequency(int k)
    {
        std::vector<std::string> result;

        for (const auto &[word, freq] : wordFrequency)
        {
            if (freq == k)
            {
                result.push_back(word);
            }
        }

        // Sort result alphabetically for consistent output
        std::sort(result.begin(), result.end());
        return result;
    }

    // Query 3: Find minimum distance between two words
    int minimumDistance(const std::string &word1, const std::string &word2)
    {
        std::string norm1 = normalize(word1);
        std::string norm2 = normalize(word2);

        // Check if both words exist
        if (wordPositions.find(norm1) == wordPositions.end() ||
            wordPositions.find(norm2) == wordPositions.end())
        {
            return -1; // One or both words not found
        }

        const std::vector<int> &positions1 = wordPositions[norm1];
        const std::vector<int> &positions2 = wordPositions[norm2];

        // Find minimum distance using two-pointer technique
        int minDist = std::numeric_limits<int>::max();
        int i = 0, j = 0;

        while (i < positions1.size() && j < positions2.size())
        {
            int dist = std::abs(positions1[i] - positions2[j]);
            minDist = std::min(minDist, dist);

            if (positions1[i] < positions2[j])
            {
                i++;
            }
            else
            {
                j++;
            }
        }

        return minDist;
    }

    // Alternative implementation using nested loops (less efficient)
    int minimumDistance_Naive(const std::string &word1, const std::string &word2)
    {
        std::string norm1 = normalize(word1);
        std::string norm2 = normalize(word2);

        // Check if both words exist
        if (wordPositions.find(norm1) == wordPositions.end() ||
            wordPositions.find(norm2) == wordPositions.end())
        {
            return -1;
        }

        const std::vector<int> &positions1 = wordPositions[norm1];
        const std::vector<int> &positions2 = wordPositions[norm2];

        // Brute force comparison of all positions
        int minDist = std::numeric_limits<int>::max();

        for (int pos1 : positions1)
        {
            for (int pos2 : positions2)
            {
                int dist = std::abs(pos1 - pos2);
                minDist = std::min(minDist, dist);
            }
        }

        return minDist;
    }

    // Query 4: Search for sentences containing a word
    std::vector<std::string> searchSentences(const std::string &word)
    {
        std::string normWord = normalize(word);
        std::vector<std::string> result;

        if (wordToSentences.find(normWord) != wordToSentences.end())
        {
            for (int idx : wordToSentences[normWord])
            {
                result.push_back(sentences[idx]);
            }
        }

        return result;
    }

    // Get document statistics
    std::unordered_map<std::string, int> getStats() const
    {
        return {
            {"totalWords", static_cast<int>(words.size())},
            {"uniqueWords", static_cast<int>(wordFrequency.size())},
            {"sentences", static_cast<int>(sentences.size())}};
    }
};

// Test function for Document Analyzer
void testDocumentAnalyzer()
{
    std::cout << "\n===== CHALLENGE #1: DOCUMENT WORD FREQUENCY ANALYZER =====\n"
              << std::endl;

    // Test case 1: Basic functionality
    std::string document1 = "The quick brown fox jumps over the lazy dog. The dog was not very lazy. The fox was quick.";

    DocumentAnalyzer analyzer1(document1);

    // Test query 1: Top N frequent words
    std::cout << "Test 1: Top 3 frequent words:" << std::endl;
    auto topWords = analyzer1.topNFrequentWords(3);
    for (const auto &[word, freq] : topWords)
    {
        std::cout << "  " << word << ": " << freq << " times" << std::endl;
    }

    // Test query 2: Words with exact frequency
    std::cout << "\nTest 2: Words that appear exactly 2 times:" << std::endl;
    auto exactFreqWords = analyzer1.wordsWithExactFrequency(2);
    std::cout << "  ";
    for (const auto &word : exactFreqWords)
    {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    // Test query 3: Minimum distance
    std::cout << "\nTest 3: Minimum distance between words:" << std::endl;
    std::cout << "  'fox' and 'dog': " << analyzer1.minimumDistance("fox", "dog") << " words" << std::endl;
    std::cout << "  'the' and 'quick': " << analyzer1.minimumDistance("the", "quick") << " words" << std::endl;
    std::cout << "  'fox' and 'nonexistent': " << analyzer1.minimumDistance("fox", "nonexistent") << std::endl;

    // Compare with naive implementation
    std::cout << "\nTest 3b: Comparison with naive implementation:" << std::endl;
    std::cout << "  'fox' and 'dog' (efficient): " << analyzer1.minimumDistance("fox", "dog") << std::endl;
    std::cout << "  'fox' and 'dog' (naive): " << analyzer1.minimumDistance_Naive("fox", "dog") << std::endl;

    // Test query 4: Search sentences
    std::cout << "\nTest 4: Sentences containing 'lazy':" << std::endl;
    auto lazySentences = analyzer1.searchSentences("lazy");
    for (const auto &sentence : lazySentences)
    {
        std::cout << "  " << sentence << std::endl;
    }

    // Test case 2: Larger document with benchmark
    std::cout << "\nTest 5: Performance with larger document:" << std::endl;

    // Generate a larger document
    std::string largeDocument;
    for (int i = 0; i < 50; i++)
    {
        largeDocument += document1 + " ";
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    DocumentAnalyzer analyzer2(largeDocument);
    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "  Document processing time: " << duration.count() << "ms" << std::endl;

    // Get statistics
    auto stats = analyzer2.getStats();
    std::cout << "  Total words: " << stats["totalWords"] << std::endl;
    std::cout << "  Unique words: " << stats["uniqueWords"] << std::endl;
    std::cout << "  Sentences: " << stats["sentences"] << std::endl;

    // Benchmark queries
    startTime = std::chrono::high_resolution_clock::now();
    analyzer2.topNFrequentWords(10);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "  Top 10 words query time: " << duration.count() << "μs" << std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    analyzer2.minimumDistance("fox", "dog");
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "  Minimum distance query time: " << duration.count() << "μs" << std::endl;

    // Compare min heap vs sort performance for top N words
    std::cout << "\nTest 6: Performance comparison - Min Heap vs Sort:" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    analyzer2.topNFrequentWords(10);
    endTime = std::chrono::high_resolution_clock::now();
    auto heapDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::high_resolution_clock::now();
    analyzer2.topNFrequentWords_UsingSort(10);
    endTime = std::chrono::high_resolution_clock::now();
    auto sortDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    std::cout << "  Min heap implementation: " << heapDuration.count() << "μs" << std::endl;
    std::cout << "  Sort implementation: " << sortDuration.count() << "μs" << std::endl;
    std::cout << "  Ratio (sort/heap): "
              << std::fixed << std::setprecision(2)
              << (double)sortDuration.count() / heapDuration.count() << "x" << std::endl;
}

// ===== CHALLENGE #2: BALANCED PARENTHESES EXTENDED =====

class BracketAnalyzer
{
private:
    // Maps opening brackets to their corresponding closing brackets
    std::unordered_map<char, char> bracketPairs = {
        {'(', ')'},
        {'[', ']'},
        {'{', '}'}};

    // Maps closing brackets to their corresponding opening brackets
    std::unordered_map<char, char> reverseBracketPairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}};

    // Check if character is an opening bracket
    bool isOpeningBracket(char c) const
    {
        return bracketPairs.find(c) != bracketPairs.end();
    }

    // Check if character is a closing bracket
    bool isClosingBracket(char c) const
    {
        return reverseBracketPairs.find(c) != reverseBracketPairs.end();
    }

public:
    // Check if string has balanced brackets
    bool isBalanced(const std::string &str)
    {
        std::stack<std::pair<char, int>> bracketStack; // Stores bracket and its position

        for (int i = 0; i < str.length(); i++)
        {
            char c = str[i];

            if (isOpeningBracket(c))
            {
                bracketStack.push({c, i});
            }
            else if (isClosingBracket(c))
            {
                if (bracketStack.empty() ||
                    bracketPairs[bracketStack.top().first] != c)
                {
                    return false;
                }
                bracketStack.pop();
            }
        }

        return bracketStack.empty();
    }

    // Identify problematic brackets and return descriptive message
    std::string findProblem(const std::string &str)
    {
        std::stack<std::pair<char, int>> bracketStack; // Stores bracket and its position

        for (int i = 0; i < str.length(); i++)
        {
            char c = str[i];

            if (isOpeningBracket(c))
            {
                bracketStack.push({c, i});
            }
            else if (isClosingBracket(c))
            {
                if (bracketStack.empty())
                {
                    return "Unbalanced - extra closing bracket " + std::string(1, c) +
                           " at position " + std::to_string(i);
                }

                if (bracketPairs[bracketStack.top().first] != c)
                {
                    return "Unbalanced - mismatched brackets: " + std::string(1, c) +
                           " at position " + std::to_string(i) +
                           " should be " + std::string(1, bracketPairs[bracketStack.top().first]);
                }

                bracketStack.pop();
            }
        }

        if (!bracketStack.empty())
        {
            auto [bracket, pos] = bracketStack.top();
            return "Unbalanced - missing closing " + std::string(1, bracketPairs[bracket]) +
                   " at position " + std::to_string(pos);
        }

        return "Balanced";
    }

    // Generate corrections to make the string balanced
    std::string makeBalanced(const std::string &str)
    {
        std::string result = str;
        std::stack<std::pair<char, int>> bracketStack; // Stores bracket and its position
        std::vector<std::pair<int, char>> corrections; // Position and character to insert/replace

        // First pass: identify mismatches and missing brackets
        for (int i = 0; i < str.length(); i++)
        {
            char c = str[i];

            if (isOpeningBracket(c))
            {
                bracketStack.push({c, i});
            }
            else if (isClosingBracket(c))
            {
                if (bracketStack.empty())
                {
                    // Extra closing bracket - mark for removal
                    corrections.push_back({i, '\0'});
                }
                else if (bracketPairs[bracketStack.top().first] != c)
                {
                    // Mismatched bracket - mark for replacement
                    corrections.push_back({i, bracketPairs[bracketStack.top().first]});
                    bracketStack.pop();
                }
                else
                {
                    // Matching bracket
                    bracketStack.pop();
                }
            }
        }

        // Add missing closing brackets
        while (!bracketStack.empty())
        {
            auto [bracket, pos] = bracketStack.top();
            corrections.push_back({str.length(), bracketPairs[bracket]});
            bracketStack.pop();
        }

        // Apply corrections in reverse order to avoid position shifts
        std::sort(corrections.rbegin(), corrections.rend());

        for (const auto &[pos, c] : corrections)
        {
            if (c == '\0')
            {
                // Remove character
                result.erase(pos, 1);
            }
            else if (pos >= result.length())
            {
                // Append character
                result.push_back(c);
            }
            else
            {
                // Replace character
                result[pos] = c;
            }
        }

        return result;
    }

    // Complete missing brackets (like in code editors)
    std::string completeBrackets(const std::string &str)
    {
        std::string result = str;
        std::stack<char> bracketStack;

        // First pass: check existing brackets
        for (char c : str)
        {
            if (isOpeningBracket(c))
            {
                bracketStack.push(c);
            }
            else if (isClosingBracket(c))
            {
                if (!bracketStack.empty() && bracketPairs[bracketStack.top()] == c)
                {
                    bracketStack.pop();
                }
            }
        }

        // Add missing closing brackets in reverse order
        while (!bracketStack.empty())
        {
            result += bracketPairs[bracketStack.top()];
            bracketStack.pop();
        }

        return result;
    }

    // Get bracket statistics from a string
    std::unordered_map<std::string, int> getBracketStats(const std::string &str)
    {
        std::unordered_map<std::string, int> stats = {
            {"openParentheses", 0},
            {"closeParentheses", 0},
            {"openBraces", 0},
            {"closeBraces", 0},
            {"openBrackets", 0},
            {"closeBrackets", 0},
            {"mismatches", 0}};

        std::stack<char> bracketStack;

        for (char c : str)
        {
            if (c == '(')
                stats["openParentheses"]++;
            else if (c == ')')
                stats["closeParentheses"]++;
            else if (c == '{')
                stats["openBraces"]++;
            else if (c == '}')
                stats["closeBraces"]++;
            else if (c == '[')
                stats["openBrackets"]++;
            else if (c == ']')
                stats["closeBrackets"]++;

            if (isOpeningBracket(c))
            {
                bracketStack.push(c);
            }
            else if (isClosingBracket(c))
            {
                if (bracketStack.empty() || bracketPairs[bracketStack.top()] != c)
                {
                    stats["mismatches"]++;
                }
                else
                {
                    bracketStack.pop();
                }
            }
        }

        return stats;
    }
};

// Test function for Bracket Analyzer
void testBracketAnalyzer()
{
    std::cout << "\n===== CHALLENGE #2: BALANCED PARENTHESES EXTENDED =====\n"
              << std::endl;

    BracketAnalyzer analyzer;

    // Test case 1: Basic balance checking
    std::vector<std::string> testCases = {
        "({[test]})",      // Balanced
        "({[test])",       // Missing closing }
        "({[test])}",      // Mismatched brackets
        "({[test",         // Multiple missing closings
        "test])",          // Extra closing brackets
        "{[(])}",          // Interleaved brackets
        "",                // Empty string
        "no brackets here" // No brackets
    };

    std::cout << "Test 1: Basic bracket balance checking:" << std::endl;

    for (const auto &test : testCases)
    {
        std::cout << "  Input: \"" << test << "\"" << std::endl;
        std::cout << "    " << analyzer.findProblem(test) << std::endl;
    }

    // Test case 2: Making strings balanced
    std::cout << "\nTest 2: Converting unbalanced to balanced:" << std::endl;

    for (const auto &test : testCases)
    {
        if (!analyzer.isBalanced(test))
        {
            std::cout << "  Input: \"" << test << "\"" << std::endl;
            std::cout << "    Corrected: \"" << analyzer.makeBalanced(test) << "\"" << std::endl;
        }
    }

    // Test case 3: Bracket completion
    std::cout << "\nTest 3: Bracket completion (code editor style):" << std::endl;

    std::vector<std::string> completionCases = {
        "({[test",
        "({",
        "function() {",
        "if (x > 0) { while (true) {"};

    for (const auto &test : completionCases)
    {
        std::cout << "  Input: \"" << test << "\"" << std::endl;
        std::cout << "    Completion: \"" << analyzer.completeBrackets(test) << "\"" << std::endl;
    }

    // Test case 4: Performance with very nested brackets
    std::cout << "\nTest 4: Performance with deeply nested brackets:" << std::endl;

    // Generate deeply nested brackets
    std::string deeplyNested;
    for (int i = 0; i < 1000; i++)
    {
        deeplyNested += "(";
    }
    for (int i = 0; i < 1000; i++)
    {
        deeplyNested += ")";
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    bool isBalanced = analyzer.isBalanced(deeplyNested);
    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    std::cout << "  1000 nested brackets is " << (isBalanced ? "balanced" : "unbalanced") << std::endl;
    std::cout << "  Check time: " << duration.count() << "μs" << std::endl;

    // Test case 5: Benchmark different operations
    std::cout << "\nTest 5: Operation benchmark comparison:" << std::endl;

    // Generate mixed brackets string
    std::string mixedBrackets;
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<> dist(0, 5);

    std::vector<char> bracketChars = {'(', ')', '{', '}', '[', ']'};

    for (int i = 0; i < 10000; i++)
    {
        mixedBrackets += bracketChars[dist(gen)];
    }

    // Benchmark isBalanced
    startTime = std::chrono::high_resolution_clock::now();
    analyzer.isBalanced(mixedBrackets);
    endTime = std::chrono::high_resolution_clock::now();
    auto balanceDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    // Benchmark findProblem
    startTime = std::chrono::high_resolution_clock::now();
    analyzer.findProblem(mixedBrackets);
    endTime = std::chrono::high_resolution_clock::now();
    auto problemDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    // Benchmark makeBalanced
    startTime = std::chrono::high_resolution_clock::now();
    analyzer.makeBalanced(mixedBrackets);
    endTime = std::chrono::high_resolution_clock::now();
    auto balancingDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    std::cout << "  isBalanced: " << balanceDuration.count() << "μs" << std::endl;
    std::cout << "  findProblem: " << problemDuration.count() << "μs" << std::endl;
    std::cout << "  makeBalanced: " << balancingDuration.count() << "μs" << std::endl;

    // Get bracket statistics
    auto stats = analyzer.getBracketStats(mixedBrackets);
    std::cout << "\n  String statistics:" << std::endl;
    std::cout << "    Open parentheses: " << stats["openParentheses"] << std::endl;
    std::cout << "    Close parentheses: " << stats["closeParentheses"] << std::endl;
    std::cout << "    Open braces: " << stats["openBraces"] << std::endl;
    std::cout << "    Close braces: " << stats["closeBraces"] << std::endl;
    std::cout << "    Open brackets: " << stats["openBrackets"] << std::endl;
    std::cout << "    Close brackets: " << stats["closeBrackets"] << std::endl;
    std::cout << "    Mismatches: " << stats["mismatches"] << std::endl;
}

// ===== CHALLENGE #3: LRU CACHE WITH TIME-BASED EXPIRY =====

// TimeSimulator for testing - allows controlling time for cache expiry testing
class TimeSimulator
{
private:
    static std::chrono::steady_clock::time_point mockTime;
    static bool usingMockTime;

public:
    static void enableMockTime()
    {
        usingMockTime = true;
        mockTime = std::chrono::steady_clock::now();
    }

    static void disableMockTime()
    {
        usingMockTime = false;
    }

    static void advanceTime(int seconds)
    {
        if (usingMockTime)
        {
            mockTime += std::chrono::seconds(seconds);
        }
    }

    static std::chrono::steady_clock::time_point now()
    {
        if (usingMockTime)
        {
            return mockTime;
        }
        else
        {
            return std::chrono::steady_clock::now();
        }
    }

    static void reset()
    {
        mockTime = std::chrono::steady_clock::now();
    }
};

// Initialize static members
std::chrono::steady_clock::time_point TimeSimulator::mockTime = std::chrono::steady_clock::now();
bool TimeSimulator::usingMockTime = false;

template <typename K, typename V>
class EnhancedLRUCache
{
private:
    // Structure to store cache entry data
    struct CacheEntry
    {
        K key;
        V value;
        std::chrono::steady_clock::time_point expiry;

        CacheEntry(const K &k, const V &v, int ttlSeconds)
            : key(k), value(v)
        {
            if (ttlSeconds > 0)
            {
                expiry = TimeSimulator::now() +
                         std::chrono::seconds(ttlSeconds);
            }
            else
            {
                // No expiry time (effectively infinite TTL)
                expiry = std::chrono::steady_clock::time_point::max();
            }
        }

        bool isExpired() const
        {
            return TimeSimulator::now() > expiry;
        }
    };

    // Maximum capacity of the cache
    size_t capacity;

    // List to keep track of LRU order (most recently used at front)
    std::list<CacheEntry> cacheList;

    // Map for O(1) lookups: key -> iterator to list node
    std::unordered_map<K, typename std::list<CacheEntry>::iterator> cacheMap;

    // Statistics
    size_t hits = 0;
    size_t misses = 0;
    size_t evictions = 0;
    size_t expirations = 0;

    // Callback for eviction events
    std::function<void(const K &, const V &, const std::string &)> evictionCallback = nullptr;

    // Clean expired entries (called periodically or when needed)
    void cleanExpired()
    {
        auto it = cacheList.begin();
        while (it != cacheList.end())
        {
            if (it->isExpired())
            {
                K key = it->key;
                V value = it->value;

                if (evictionCallback)
                {
                    evictionCallback(key, value, "expired");
                }

                cacheMap.erase(key);
                it = cacheList.erase(it);
                expirations++;
            }
            else
            {
                ++it;
            }
        }
    }

public:
    EnhancedLRUCache(size_t cap) : capacity(cap) {}

    // Set eviction callback
    void setEvictionCallback(const std::function<void(const K &, const V &, const std::string &)> &callback)
    {
        evictionCallback = callback;
    }

    // Get value for a key if it exists and is not expired
    std::optional<V> get(const K &key)
    {
        cleanExpired(); // Clean expired entries first

        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
        {
            misses++;
            return std::nullopt; // Key not found
        }

        auto listIt = it->second;

        // Check if the entry has expired
        if (listIt->isExpired())
        {
            // Remove expired entry
            cacheMap.erase(key);
            cacheList.erase(listIt);
            misses++;
            expirations++;
            return std::nullopt;
        }

        // Move entry to front (mark as most recently used)
        cacheList.splice(cacheList.begin(), cacheList, listIt);
        hits++;
        return listIt->value;
    }

    // Set a key-value pair with a TTL (in seconds)
    void set(const K &key, const V &value, int ttlSeconds = 0)
    {
        cleanExpired(); // Clean expired entries first

        // Check if key already exists
        auto it = cacheMap.find(key);
        if (it != cacheMap.end())
        {
            // Update existing entry
            cacheList.erase(it->second);
            cacheMap.erase(it);
        }
        else if (cacheList.size() >= capacity)
        {
            // Cache is full, remove least recently used (back of list)
            const CacheEntry &entry = cacheList.back();

            if (evictionCallback)
            {
                evictionCallback(entry.key, entry.value, "capacity");
            }

            cacheMap.erase(entry.key);
            cacheList.pop_back();
            evictions++;
        }

        // Insert new entry at front
        cacheList.emplace_front(key, value, ttlSeconds);
        cacheMap[key] = cacheList.begin();
    }

    // Batch get operation
    std::unordered_map<K, V> batchGet(const std::vector<K> &keys)
    {
        std::unordered_map<K, V> result;

        for (const K &key : keys)
        {
            auto value = get(key);
            if (value.has_value())
            {
                result[key] = *value;
            }
        }

        return result;
    }

    // Batch set operation
    void batchSet(const std::unordered_map<K, std::pair<V, int>> &entries)
    {
        for (const auto &[key, valueTtlPair] : entries)
        {
            const auto &[value, ttl] = valueTtlPair;
            set(key, value, ttl);
        }
    }

    // Get cache statistics
    std::unordered_map<std::string, size_t> getStats() const
    {
        return {
            {"hits", hits},
            {"misses", misses},
            {"size", cacheList.size()},
            {"capacity", capacity},
            {"evictions", evictions},
            {"expirations", expirations}};
    }

    // Get time to live for a key (in seconds)
    std::optional<int> getTTL(const K &key)
    {
        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
        {
            return std::nullopt; // Key not found
        }

        auto listIt = it->second;

        // Check if the entry has expired
        if (listIt->isExpired())
        {
            return std::nullopt;
        }

        // Calculate remaining TTL
        auto now = TimeSimulator::now();
        auto expiry = listIt->expiry;

        if (expiry == std::chrono::steady_clock::time_point::max())
        {
            return std::numeric_limits<int>::max(); // Infinite TTL
        }

        auto remainingDuration = std::chrono::duration_cast<std::chrono::seconds>(expiry - now);
        return remainingDuration.count();
    }

    // Update TTL for an existing key
    bool updateTTL(const K &key, int ttlSeconds)
    {
        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
        {
            return false; // Key not found
        }

        auto listIt = it->second;

        // Check if the entry has expired
        if (listIt->isExpired())
        {
            // Remove expired entry
            cacheMap.erase(key);
            cacheList.erase(listIt);
            expirations++;
            return false;
        }

        // Update expiry time
        if (ttlSeconds > 0)
        {
            listIt->expiry = TimeSimulator::now() + std::chrono::seconds(ttlSeconds);
        }
        else
        {
            listIt->expiry = std::chrono::steady_clock::time_point::max();
        }

        return true;
    }

    // Clear the cache
    void clear()
    {
        cacheList.clear();
        cacheMap.clear();
    }

    // Remove a key from the cache
    bool remove(const K &key)
    {
        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
        {
            return false; // Key not found
        }

        // Remove the entry
        cacheList.erase(it->second);
        cacheMap.erase(it);
        return true;
    }

    // Get current size and capacity
    size_t size() const
    {
        return cacheList.size();
    }

    size_t getCapacity() const
    {
        return capacity;
    }

    // Change capacity (may trigger evictions)
    void resize(size_t newCapacity)
    {
        if (newCapacity < capacity)
        {
            // Need to evict items
            while (cacheList.size() > newCapacity)
            {
                const CacheEntry &entry = cacheList.back();

                if (evictionCallback)
                {
                    evictionCallback(entry.key, entry.value, "resize");
                }

                cacheMap.erase(entry.key);
                cacheList.pop_back();
                evictions++;
            }
        }

        capacity = newCapacity;
    }
};

// Test function for Enhanced LRU Cache
void testEnhancedLRUCache()
{
    std::cout << "\n===== CHALLENGE #3: LRU CACHE WITH TIME-BASED EXPIRY =====\n"
              << std::endl;

    // Enable time simulation for testing
    TimeSimulator::enableMockTime();

    // Test case 1: Basic functionality
    std::cout << "Test 1: Basic functionality:" << std::endl;

    EnhancedLRUCache<std::string, std::string> cache(3);

    // Set eviction callback
    cache.setEvictionCallback([](const std::string &key, const std::string &value, const std::string &reason)
                              { std::cout << "  Evicted: " << key << " -> " << value << " (reason: " << reason << ")" << std::endl; });

    // Initial operations
    cache.set("key1", "value1", 60); // TTL of 60 seconds
    cache.set("key2", "value2", 30); // TTL of 30 seconds

    // Get operations
    auto value1 = cache.get("key1");
    auto value3 = cache.get("key3"); // Key doesn't exist

    std::cout << "  key1: " << (value1 ? *value1 : "not found") << std::endl;
    std::cout << "  key3: " << (value3 ? *value3 : "not found") << std::endl;

    // Print initial stats
    auto stats = cache.getStats();
    std::cout << "  Stats: hits=" << stats["hits"]
              << ", misses=" << stats["misses"]
              << ", size=" << stats["size"]
              << ", evictions=" << stats["evictions"] << std::endl;

    // Test case 2: Time-based expiry
    std::cout << "\nTest 2: Time-based expiry:" << std::endl;

    // Advance time by 40 seconds (key2 should expire)
    TimeSimulator::advanceTime(40);

    auto value2 = cache.get("key2"); // Should be expired
    std::cout << "  key2 after 40s: " << (value2 ? *value2 : "not found (expired)") << std::endl;

    // Check key1 (still valid)
    value1 = cache.get("key1");
    std::cout << "  key1 after 40s: " << (value1 ? *value1 : "not found") << std::endl;

    // Check TTL
    auto ttl1 = cache.getTTL("key1");
    auto ttl2 = cache.getTTL("key2");

    std::cout << "  TTL for key1: " << (ttl1 ? std::to_string(*ttl1) + "s" : "not found") << std::endl;
    std::cout << "  TTL for key2: " << (ttl2 ? std::to_string(*ttl2) + "s" : "not found") << std::endl;

    // Update TTL
    bool updated = cache.updateTTL("key1", 100);
    ttl1 = cache.getTTL("key1");

    std::cout << "  Updated TTL for key1: " << (updated ? "yes" : "no")
              << ", new TTL: " << (ttl1 ? std::to_string(*ttl1) + "s" : "not found") << std::endl;

    // Test case 3: LRU eviction
    std::cout << "\nTest 3: LRU eviction:" << std::endl;

    // Add items to trigger eviction
    cache.set("key3", "value3"); // No TTL
    cache.set("key4", "value4"); // No TTL

    // key1 should still be in cache (most recently used due to TTL update)
    value1 = cache.get("key1");
    std::cout << "  key1 after adding more keys: " << (value1 ? *value1 : "not found") << std::endl;

    // Add one more to trigger eviction of least recently used item
    cache.set("key5", "value5");

    // Check which items are in cache
    std::cout << "  Items in cache:" << std::endl;
    for (const auto &key : {"key1", "key2", "key3", "key4", "key5"})
    {
        auto value = cache.get(key);
        std::cout << "    " << key << ": " << (value ? *value : "not found") << std::endl;
    }

    // Test case 4: Batch operations
    std::cout << "\nTest 4: Batch operations:" << std::endl;

    // Clear cache for clean test
    cache.clear();

    // Batch set
    std::unordered_map<std::string, std::pair<std::string, int>> batchItems = {
        {"batch1", {"value1", 100}},
        {"batch2", {"value2", 200}},
        {"batch3", {"value3", 300}}};

    cache.batchSet(batchItems);

    // Batch get
    auto batchResults = cache.batchGet({"batch1", "batch2", "batch3", "nonexistent"});

    std::cout << "  Batch get results:" << std::endl;
    for (const auto &[key, value] : batchResults)
    {
        std::cout << "    " << key << ": " << value << std::endl;
    }

    // Test case 5: Performance benchmark
    std::cout << "\nTest 5: Performance benchmark:" << std::endl;

    // Create larger cache
    EnhancedLRUCache<int, int> benchCache(10000);

    // Measure set performance
    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100000; i++)
    {
        benchCache.set(i, i * 10, 3600); // 1 hour TTL
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto setDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    // Measure get performance (mix of hits and misses)
    startTime = std::chrono::high_resolution_clock::now();

    for (int i = 90000; i < 110000; i++)
    {
        benchCache.get(i);
    }

    endTime = std::chrono::high_resolution_clock::now();
    auto getDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    // Print benchmark results
    std::cout << "  Set 100,000 items: " << setDuration.count() << "ms" << std::endl;
    std::cout << "  Get 20,000 items (mixed hits/misses): " << getDuration.count() << "ms" << std::endl;

    auto benchStats = benchCache.getStats();
    std::cout << "  Final stats: hits=" << benchStats["hits"]
              << ", misses=" << benchStats["misses"]
              << ", size=" << benchStats["size"]
              << ", capacity=" << benchStats["capacity"]
              << ", evictions=" << benchStats["evictions"] << std::endl;

    // Test case 6: Edge cases
    std::cout << "\nTest 6: Edge cases:" << std::endl;

    // Create empty cache
    EnhancedLRUCache<std::string, std::string> emptyCache(0);
    emptyCache.set("key", "value");
    auto emptyStats = emptyCache.getStats();

    std::cout << "  Empty cache set operation:" << std::endl;
    std::cout << "    Size: " << emptyStats["size"] << std::endl;
    std::cout << "    Capacity: " << emptyStats["capacity"] << std::endl;

    // Resize cache
    EnhancedLRUCache<std::string, std::string> resizeCache(5);
    for (int i = 0; i < 5; i++)
    {
        resizeCache.set("key" + std::to_string(i), "value" + std::to_string(i));
    }

    std::cout << "  Before resize - size: " << resizeCache.size() << std::endl;

    resizeCache.resize(3);

    std::cout << "  After resize - size: " << resizeCache.size() << std::endl;

    // Disable time simulation
    TimeSimulator::disableMockTime();
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "===================================================" << std::endl;
    std::cout << "DAY 14: WEEKLY CHALLENGE & RECAP SOLUTIONS #DSAin45" << std::endl;
    std::cout << "===================================================" << std::endl;

    // Run tests for each challenge
    testDocumentAnalyzer();
    testBracketAnalyzer();
    testEnhancedLRUCache();

    return 0;
}