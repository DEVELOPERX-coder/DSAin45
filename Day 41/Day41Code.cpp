/*
 * StringAlgorithms.cpp
 * Day 41 of #DSAin45 - Comprehensive String Algorithms Implementation
 *
 * This file implements various string pattern matching algorithms with
 * benchmarks, comparisons, and real-world applications.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <sstream>

// Utility function to measure execution time
template <typename TimePoint>
std::chrono::microseconds getElapsedMicroseconds(TimePoint start, TimePoint end)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

// Utility function to generate random strings
std::string generateRandomString(int length, const std::string &charset = "abcdefghijklmnopqrstuvwxyz")
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);

    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i)
    {
        result += charset[dist(g)];
    }
    return result;
}

// Utility function to print matches
void printMatches(const std::vector<int> &matches, const std::string &algorithm, int limit = 10)
{
    std::cout << algorithm << " found " << matches.size() << " matches at positions: ";

    for (size_t i = 0; i < std::min(matches.size(), static_cast<size_t>(limit)); ++i)
    {
        std::cout << matches[i];
        if (i < std::min(matches.size(), static_cast<size_t>(limit)) - 1)
        {
            std::cout << ", ";
        }
    }

    if (matches.size() > limit)
    {
        std::cout << ", ...";
    }

    std::cout << std::endl;
}

// --------------------------------------------------------------------------
// Naive String Matching Algorithm
// --------------------------------------------------------------------------
namespace Naive
{
    std::vector<int> search(const std::string &text, const std::string &pattern)
    {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();

        // Pattern can't be found if it's longer than text
        if (m > n)
            return matches;

        // Check each potential starting position in the text
        for (int i = 0; i <= n - m; i++)
        {
            int j;
            // Check if pattern matches starting at position i
            for (j = 0; j < m; j++)
            {
                if (text[i + j] != pattern[j])
                {
                    break; // Mismatch found, stop checking
                }
            }

            // If we checked all characters in pattern, it's a match
            if (j == m)
            {
                matches.push_back(i);
            }
        }

        return matches;
    }
}

// --------------------------------------------------------------------------
// Knuth-Morris-Pratt (KMP) Algorithm
// --------------------------------------------------------------------------
namespace KMP
{
    // Build the LPS (Longest Prefix Suffix) array
    std::vector<int> computeLPSArray(const std::string &pattern)
    {
        int m = pattern.length();
        std::vector<int> lps(m, 0);

        // Length of the previous longest prefix suffix
        int len = 0;

        // LPS[0] is always 0
        lps[0] = 0;

        // Calculate LPS[i] for i = 1 to m-1
        int i = 1;
        while (i < m)
        {
            if (pattern[i] == pattern[len])
            {
                len++;
                lps[i] = len;
                i++;
            }
            else
            {
                if (len != 0)
                {
                    // Try finding a shorter prefix that is also a suffix
                    len = lps[len - 1];
                }
                else
                {
                    // No matching prefix found
                    lps[i] = 0;
                    i++;
                }
            }
        }

        return lps;
    }

    // KMP search algorithm
    std::vector<int> search(const std::string &text, const std::string &pattern)
    {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();

        // Edge cases
        if (m == 0)
            return matches;
        if (m > n)
            return matches;

        // Preprocess the pattern to build LPS array
        std::vector<int> lps = computeLPSArray(pattern);

        int i = 0; // Index for text
        int j = 0; // Index for pattern

        // Search text for pattern
        while (i < n)
        {
            // Current characters match
            if (pattern[j] == text[i])
            {
                i++;
                j++;
            }

            // Pattern completely matched
            if (j == m)
            {
                // Found pattern at index i - j
                matches.push_back(i - j);
                // Look for next match, using LPS to avoid rechecking characters
                j = lps[j - 1];
            }
            // Current characters don't match
            else if (i < n && pattern[j] != text[i])
            {
                // Use LPS value to shift pattern efficiently
                if (j != 0)
                {
                    j = lps[j - 1];
                }
                else
                {
                    i++;
                }
            }
        }

        return matches;
    }

    // Visualize the KMP algorithm for educational purposes
    void visualizeKMP(const std::string &text, const std::string &pattern)
    {
        std::vector<int> lps = computeLPSArray(pattern);

        std::cout << "KMP Visualization:\n";
        std::cout << "Pattern: " << pattern << "\n";
        std::cout << "LPS Array: [";
        for (size_t i = 0; i < lps.size(); i++)
        {
            std::cout << lps[i];
            if (i < lps.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]\n\n";

        int i = 0; // Index for text
        int j = 0; // Index for pattern
        int step = 1;

        while (i < text.length())
        {
            std::cout << "Step " << step << ":\n";

            // Print text with current position
            std::cout << "Text: " << text << "\n";
            std::cout << "      ";
            for (int k = 0; k < i; k++)
                std::cout << " ";
            std::cout << "^\n";

            // Print pattern alignment
            std::cout << "Pattern: ";
            for (int k = 0; k < i - j; k++)
                std::cout << " ";
            std::cout << pattern << "\n";

            if (j < pattern.length() && pattern[j] == text[i])
            {
                std::cout << "Match: text[" << i << "] = pattern[" << j << "] = '" << text[i] << "'\n";
                i++;
                j++;
            }
            else
            {
                std::cout << "Mismatch at text[" << i << "] = '" << text[i]
                          << "' and pattern[" << j << "] = '" << (j < pattern.length() ? std::string(1, pattern[j]) : "END") << "'\n";

                if (j != 0)
                {
                    int prev_j = j;
                    j = lps[j - 1];
                    std::cout << "Shift pattern using LPS[" << (prev_j - 1) << "] = " << j << "\n";
                }
                else
                {
                    std::cout << "j = 0, advancing text position\n";
                    i++;
                }
            }

            if (j == pattern.length())
            {
                std::cout << "Pattern found at index " << (i - j) << "\n";
                j = lps[j - 1];
            }

            std::cout << "\n";
            step++;
        }
    }
}

// --------------------------------------------------------------------------
// Rabin-Karp Algorithm
// --------------------------------------------------------------------------
namespace RabinKarp
{
    // d is the number of characters in the input alphabet
    const int d = 256;

    std::vector<int> search(const std::string &text, const std::string &pattern, int q = 101)
    {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();

        // Edge cases
        if (m > n)
            return matches;
        if (m == 0)
            return matches;

        int pattern_hash = 0; // Hash value for pattern
        int text_hash = 0;    // Hash value for current text window
        int h = 1;            // Value of d^(m-1) % q

        // Calculate the value of h = d^(m-1) % q
        for (int i = 0; i < m - 1; i++)
        {
            h = (h * d) % q;
        }

        // Calculate initial hash values for pattern and first text window
        for (int i = 0; i < m; i++)
        {
            pattern_hash = (d * pattern_hash + pattern[i]) % q;
            text_hash = (d * text_hash + text[i]) % q;
        }

        // Slide the pattern over text one by one
        for (int i = 0; i <= n - m; i++)
        {
            // Check if hash values match
            if (pattern_hash == text_hash)
            {
                // Verify character by character
                bool match = true;
                for (int j = 0; j < m; j++)
                {
                    if (text[i + j] != pattern[j])
                    {
                        match = false;
                        break;
                    }
                }

                if (match)
                {
                    matches.push_back(i);
                }
            }

            // Calculate hash value for next window by removing leading digit,
            // adding trailing digit
            if (i < n - m)
            {
                text_hash = (d * (text_hash - text[i] * h) + text[i + m]) % q;

                // Make sure hash value is positive
                if (text_hash < 0)
                {
                    text_hash += q;
                }
            }
        }

        return matches;
    }

    // Multiple pattern search using Rabin-Karp
    std::unordered_map<std::string, std::vector<int>> multiPatternSearch(
        const std::string &text, const std::vector<std::string> &patterns)
    {
        std::unordered_map<std::string, std::vector<int>> all_matches;

        // Check if all patterns have the same length
        bool same_length = true;
        if (!patterns.empty())
        {
            int pattern_length = patterns[0].length();
            for (const auto &pattern : patterns)
            {
                if (pattern.length() != pattern_length)
                {
                    same_length = false;
                    break;
                }
            }
        }

        if (same_length && !patterns.empty())
        {
            // Optimized version for patterns of the same length
            int m = patterns[0].length();
            int n = text.length();
            int q = 101; // A prime number for hash calculation

            // Calculate hash values for all patterns
            std::unordered_map<int, std::vector<std::string>> pattern_hashes;
            for (const auto &pattern : patterns)
            {
                int pattern_hash = 0;
                for (int i = 0; i < m; i++)
                {
                    pattern_hash = (d * pattern_hash + pattern[i]) % q;
                }
                pattern_hashes[pattern_hash].push_back(pattern);
            }

            // Initialize hash for first window of text
            int text_hash = 0;
            int h = 1;

            // Calculate h = d^(m-1) % q
            for (int i = 0; i < m - 1; i++)
            {
                h = (h * d) % q;
            }

            // Calculate hash for first window
            for (int i = 0; i < m; i++)
            {
                text_hash = (d * text_hash + text[i]) % q;
            }

            // Slide window over text
            for (int i = 0; i <= n - m; i++)
            {
                // Check if current hash matches any pattern hash
                if (pattern_hashes.find(text_hash) != pattern_hashes.end())
                {
                    // Check each pattern with this hash
                    for (const auto &pattern : pattern_hashes[text_hash])
                    {
                        bool match = true;
                        for (int j = 0; j < m; j++)
                        {
                            if (text[i + j] != pattern[j])
                            {
                                match = false;
                                break;
                            }
                        }

                        if (match)
                        {
                            all_matches[pattern].push_back(i);
                        }
                    }
                }

                // Calculate hash for next window
                if (i < n - m)
                {
                    text_hash = (d * (text_hash - text[i] * h) + text[i + m]) % q;
                    if (text_hash < 0)
                    {
                        text_hash += q;
                    }
                }
            }
        }
        else
        {
            // Fallback to individual searches for patterns of different lengths
            for (const auto &pattern : patterns)
            {
                all_matches[pattern] = search(text, pattern);
            }
        }

        return all_matches;
    }
}

// --------------------------------------------------------------------------
// Z Algorithm
// --------------------------------------------------------------------------
namespace ZAlgorithm
{
    // Constructs Z array
    std::vector<int> computeZArray(const std::string &s)
    {
        int n = s.length();
        std::vector<int> z(n, 0);

        // [L, R] form a window which matches with prefix of s
        int L = 0, R = 0;

        for (int i = 1; i < n; i++)
        {
            // If i > R, compute Z[i] naively
            if (i > R)
            {
                L = R = i;

                // R-L = 0 in the beginning, so it will start checking from 0'th index
                while (R < n && s[R - L] == s[R])
                {
                    R++;
                }

                z[i] = R - L;
                R--;
            }
            else
            {
                // i is within [L, R], we can use previously computed values
                int k = i - L;

                // If Z[k] < remaining window size, Z[i] = Z[k]
                if (z[k] < R - i + 1)
                {
                    z[i] = z[k];
                }
                else
                {
                    // Otherwise, we need to do more comparisons
                    L = i;
                    while (R < n && s[R - L] == s[R])
                    {
                        R++;
                    }

                    z[i] = R - L;
                    R--;
                }
            }
        }

        return z;
    }

    // Z algorithm for pattern matching
    std::vector<int> search(const std::string &text, const std::string &pattern)
    {
        std::vector<int> matches;

        // Edge cases
        if (pattern.empty())
            return matches;

        // Concatenate pattern and text with a character that won't appear in either
        std::string concat = pattern + "$" + text;
        std::vector<int> z = computeZArray(concat);

        // If Z value equals pattern length, we found a match
        for (int i = 0; i < concat.length(); i++)
        {
            if (z[i] == pattern.length())
            {
                int original_index = i - pattern.length() - 1;
                if (original_index >= 0)
                {
                    matches.push_back(original_index);
                }
            }
        }

        return matches;
    }
}

// --------------------------------------------------------------------------
// Boyer-Moore Algorithm
// --------------------------------------------------------------------------
namespace BoyerMoore
{
    // Preprocesses the bad character heuristic
    std::vector<int> buildBadCharTable(const std::string &pattern)
    {
        int m = pattern.length();
        std::vector<int> badChar(256, -1);

        // Store the last occurrence of each character in the pattern
        for (int i = 0; i < m; i++)
        {
            badChar[pattern[i]] = i;
        }

        return badChar;
    }

    // Preprocesses the good suffix heuristic
    std::vector<int> buildGoodSuffixTable(const std::string &pattern)
    {
        int m = pattern.length();
        std::vector<int> suffix(m, 0);
        std::vector<int> goodSuffix(m, 0);

        // Case 1: suffix[i] = the largest position j such that pattern[j-i...j-1] matches pattern[m-i...m-1]
        int j = 0;
        suffix[m - 1] = m;
        for (int i = m - 2; i >= 0; i--)
        {
            if (i > j && suffix[m - 1 - (j - i)] < i - j)
            {
                suffix[i] = suffix[m - 1 - (j - i)];
            }
            else
            {
                if (i < j)
                    j = i;
                while (j >= 0 && pattern[j] == pattern[m - 1 - (i - j)])
                {
                    j--;
                }
                suffix[i] = i - j;
            }
        }

        // Case 2: goodSuffix[i] = the shift distance based on the good suffix rule
        for (int i = 0; i < m; i++)
        {
            goodSuffix[i] = m;
        }

        j = 0;
        for (int i = m - 1; i >= 0; i--)
        {
            if (suffix[i] == i + 1)
            {
                while (j < m - 1 - i)
                {
                    if (goodSuffix[j] == m)
                    {
                        goodSuffix[j] = m - 1 - i;
                    }
                    j++;
                }
            }
        }

        for (int i = 0; i <= m - 2; i++)
        {
            goodSuffix[m - 1 - suffix[i]] = m - 1 - i;
        }

        return goodSuffix;
    }

    // Boyer-Moore search algorithm
    std::vector<int> search(const std::string &text, const std::string &pattern)
    {
        std::vector<int> matches;
        int n = text.length();
        int m = pattern.length();

        // Edge cases
        if (m > n)
            return matches;
        if (m == 0)
            return matches;

        // Preprocess pattern
        std::vector<int> badChar = buildBadCharTable(pattern);
        std::vector<int> goodSuffix = buildGoodSuffixTable(pattern);

        int s = 0; // The shift of the pattern with respect to text

        while (s <= n - m)
        {
            int j = m - 1;

            // Match pattern from right to left
            while (j >= 0 && pattern[j] == text[s + j])
            {
                j--;
            }

            // Pattern found
            if (j < 0)
            {
                matches.push_back(s);
                // Shift using the good suffix rule
                s += goodSuffix[0];
            }
            else
            {
                // Shift using the maximum of the bad character and good suffix rules
                s += std::max(j - badChar[text[s + j]], goodSuffix[j]);
            }
        }

        return matches;
    }
}

// --------------------------------------------------------------------------
// Aho-Corasick Algorithm (Multi-pattern matching)
// --------------------------------------------------------------------------
namespace AhoCorasick
{
    // Trie node structure
    struct TrieNode
    {
        std::unordered_map<char, TrieNode *> children;
        bool isEndOfWord;
        std::string word;
        TrieNode *fail;
        std::vector<TrieNode *> output;

        TrieNode() : isEndOfWord(false), fail(nullptr) {}
    };

    // Build trie structure with failure links
    TrieNode *buildTrie(const std::vector<std::string> &patterns)
    {
        TrieNode *root = new TrieNode();

        // Insert all patterns into the trie
        for (const auto &pattern : patterns)
        {
            TrieNode *node = root;
            for (char c : pattern)
            {
                if (node->children.find(c) == node->children.end())
                {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }
            node->isEndOfWord = true;
            node->word = pattern;
        }

        // Build failure function using BFS
        std::vector<TrieNode *> queue;

        // Initialize failure for depth 1 nodes
        for (auto &[c, child] : root->children)
        {
            child->fail = root;
            queue.push_back(child);
        }

        // Process the rest of the nodes level by level
        while (!queue.empty())
        {
            TrieNode *current = queue.front();
            queue.erase(queue.begin());

            for (auto &[c, child] : current->children)
            {
                queue.push_back(child);

                // Compute failure function
                TrieNode *failure = current->fail;
                while (failure != nullptr &&
                       failure->children.find(c) == failure->children.end())
                {
                    failure = failure->fail;
                }

                if (failure == nullptr)
                {
                    child->fail = root;
                }
                else
                {
                    child->fail = failure->children[c];
                }

                // Add output links
                if (child->fail->isEndOfWord)
                {
                    child->output.push_back(child->fail);
                }

                // Merge output links from failure node
                for (auto &output_node : child->fail->output)
                {
                    child->output.push_back(output_node);
                }
            }
        }

        return root;
    }

    // Free memory used by trie
    void destroyTrie(TrieNode *root)
    {
        if (root == nullptr)
            return;

        for (auto &[c, child] : root->children)
        {
            destroyTrie(child);
        }

        delete root;
    }

    // Aho-Corasick search
    std::unordered_map<std::string, std::vector<int>> search(
        const std::string &text, const std::vector<std::string> &patterns)
    {
        std::unordered_map<std::string, std::vector<int>> matches;

        // Initialize trie
        TrieNode *root = buildTrie(patterns);

        // Search text using the trie
        TrieNode *current = root;

        for (int i = 0; i < text.length(); i++)
        {
            char c = text[i];

            // If current character is not found, follow failure links
            while (current != root && current->children.find(c) == current->children.end())
            {
                current = current->fail;
            }

            // If current character is found in current node's children, move down
            if (current->children.find(c) != current->children.end())
            {
                current = current->children[c];
            }

            // Check if current node is an endpoint or has outputs
            if (current->isEndOfWord)
            {
                matches[current->word].push_back(i - current->word.length() + 1);
            }

            // Check outputs
            for (auto &output_node : current->output)
            {
                matches[output_node->word].push_back(i - output_node->word.length() + 1);
            }
        }

        // Clean up
        destroyTrie(root);

        return matches;
    }
}

// --------------------------------------------------------------------------
// Real-world applications
// --------------------------------------------------------------------------
namespace Applications
{
    // 1. DNA Sequence Analysis
    bool findDNAPattern(const std::string &dna, const std::string &pattern)
    {
        auto matches = KMP::search(dna, pattern);
        return !matches.empty();
    }

    // 2. Plagiarism Detection
    struct Document
    {
        std::string content;
        std::string title;
    };

    double calculateDocumentSimilarity(const Document &doc1, const Document &doc2)
    {
        // Break documents into chunks (k-grams)
        const int k = 5; // Size of each chunk
        std::unordered_map<std::string, int> chunks1, chunks2;

        // Extract k-grams from document 1
        for (int i = 0; i <= doc1.content.length() - k; i++)
        {
            std::string chunk = doc1.content.substr(i, k);
            chunks1[chunk]++;
        }

        // Extract k-grams from document 2
        for (int i = 0; i <= doc2.content.length() - k; i++)
        {
            std::string chunk = doc2.content.substr(i, k);
            chunks2[chunk]++;
        }

        // Count matching chunks (Jaccard similarity)
        int intersection = 0;
        int totalUnique = 0;

        for (const auto &[chunk, count] : chunks1)
        {
            if (chunks2.find(chunk) != chunks2.end())
            {
                intersection += std::min(count, chunks2[chunk]);
            }
            totalUnique += count;
        }

        for (const auto &[chunk, count] : chunks2)
        {
            totalUnique += count;
        }

        // Subtract the intersection to avoid double counting
        totalUnique -= intersection;

        // Return similarity score (0 to 1)
        return totalUnique > 0 ? static_cast<double>(intersection) / totalUnique : 0.0;
    }

    // 3. Search and Replace in Text Editor
    std::string searchAndReplace(const std::string &text,
                                 const std::string &pattern,
                                 const std::string &replacement)
    {
        // Find all occurrences using KMP
        std::vector<int> matches = KMP::search(text, pattern);

        // If no matches, return original text
        if (matches.empty())
        {
            return text;
        }

        // Create a new string with replacements
        std::string result;
        int lastPos = 0;

        for (int pos : matches)
        {
            // Add text up to the match
            result.append(text.substr(lastPos, pos - lastPos));
            // Add replacement
            result.append(replacement);
            // Update last position
            lastPos = pos + pattern.length();
        }

        // Add remaining text
        result.append(text.substr(lastPos));

        return result;
    }

    // 4. Spell Checker
    int editDistance(const std::string &s1, const std::string &s2)
    {
        int m = s1.length();
        int n = s2.length();

        // Create a DP table
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Initialize first row and column
        for (int i = 0; i <= m; i++)
        {
            dp[i][0] = i;
        }

        for (int j = 0; j <= n; j++)
        {
            dp[0][j] = j;
        }

        // Fill the DP table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = 1 + std::min({dp[i - 1][j],       // Deletion
                                             dp[i][j - 1],       // Insertion
                                             dp[i - 1][j - 1]}); // Substitution
                }
            }
        }

        return dp[m][n];
    }

    std::vector<std::string> findSimilarWords(const std::string &word,
                                              const std::vector<std::string> &dictionary,
                                              int maxDistance = 2)
    {
        std::vector<std::string> suggestions;

        for (const auto &dictWord : dictionary)
        {
            int distance = editDistance(word, dictWord);
            if (distance <= maxDistance)
            {
                suggestions.push_back(dictWord);
            }
        }

        return suggestions;
    }

    // 5. Data Compression (Simple LZ77-like approach)
    struct Token
    {
        bool isReference;
        int offset;
        int length;
        char nextChar;

        // Constructor for literal character
        Token(char c) : isReference(false), offset(0), length(0), nextChar(c) {}

        // Constructor for reference
        Token(int o, int l, char c) : isReference(true), offset(o), length(l), nextChar(c) {}
    };

    std::vector<Token> compressLZ(const std::string &text)
    {
        std::vector<Token> tokens;
        int pos = 0;

        while (pos < text.length())
        {
            // Search for the longest match in the look-back buffer
            int bestMatchOffset = 0;
            int bestMatchLength = 0;

            // Search in the look-back buffer (simplified: just look at all previous text)
            for (int i = 0; i < pos; i++)
            {
                int j = 0;
                while (pos + j < text.length() &&
                       i + j < pos &&
                       text[i + j] == text[pos + j])
                {
                    j++;
                }

                if (j > bestMatchLength)
                {
                    bestMatchLength = j;
                    bestMatchOffset = pos - i;
                }
            }

            if (bestMatchLength > 0)
            {
                // We found a match, create a reference token
                char nextChar = pos + bestMatchLength < text.length() ? text[pos + bestMatchLength] : '\0';
                tokens.push_back(Token(bestMatchOffset, bestMatchLength, nextChar));
                pos += bestMatchLength + 1;
            }
            else
            {
                // No match, create a literal token
                tokens.push_back(Token(text[pos]));
                pos++;
            }
        }

        return tokens;
    }

    std::string decompressLZ(const std::vector<Token> &tokens)
    {
        std::string result;

        for (const auto &token : tokens)
        {
            if (token.isReference)
            {
                // Copy from the already decompressed text
                int start = result.length() - token.offset;
                for (int i = 0; i < token.length; i++)
                {
                    result.push_back(result[start + i]);
                }
                if (token.nextChar != '\0')
                {
                    result.push_back(token.nextChar);
                }
            }
            else
            {
                // Add literal character
                result.push_back(token.nextChar);
            }
        }

        return result;
    }
}

// --------------------------------------------------------------------------
// Benchmark tests
// --------------------------------------------------------------------------
void runBenchmarks()
{
    std::cout << "\n=== STRING ALGORITHM BENCHMARKS ===\n";

    // Test with different text and pattern sizes
    std::vector<std::pair<int, int>> test_sizes = {
        {1000, 5},    // Small text, small pattern
        {10000, 10},  // Medium text, small pattern
        {100000, 20}, // Large text, medium pattern
        {1000000, 50} // Very large text, large pattern
    };

    for (const auto &[text_size, pattern_size] : test_sizes)
    {
        std::cout << "\nTest with text size: " << text_size
                  << ", pattern size: " << pattern_size << "\n";

        // Generate random text and pattern
        std::string text = generateRandomString(text_size);
        std::string pattern = text.substr(text_size / 2, pattern_size); // Guaranteed to have at least one match

        // Add a few more occurrences of the pattern
        for (int i = 0; i < 10; i++)
        {
            int pos = (text_size / 20) * i;
            if (pos + pattern_size <= text_size)
            {
                text.replace(pos, pattern_size, pattern);
            }
        }

        // Benchmark all algorithms
        std::vector<std::pair<std::string, std::function<std::vector<int>(const std::string &, const std::string &)>>> algorithms = {
            {"Naive", Naive::search},
            {"KMP", KMP::search},
            {"Rabin-Karp", RabinKarp::search},
            {"Z Algorithm", ZAlgorithm::search},
            {"Boyer-Moore", BoyerMoore::search}};

        for (const auto &[name, algorithm] : algorithms)
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<int> matches = algorithm(text, pattern);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << name << " found " << matches.size() << " matches in "
                      << elapsed.count() << " microseconds\n";
        }
    }

    // Test multi-pattern matching
    std::cout << "\n=== MULTI-PATTERN MATCHING BENCHMARK ===\n";

    std::string large_text = generateRandomString(100000);
    std::vector<std::string> patterns;

    // Generate 100 random patterns of length 10
    for (int i = 0; i < 100; i++)
    {
        patterns.push_back(generateRandomString(10));
    }

    // Insert some patterns into the text to ensure matches
    for (int i = 0; i < 20; i++)
    {
        int pos = (100000 / 30) * i;
        if (pos + 10 <= 100000)
        {
            large_text.replace(pos, 10, patterns[i % patterns.size()]);
        }
    }

    // Benchmark Rabin-Karp multi-pattern and Aho-Corasick
    std::cout << "Testing with 100 patterns of length 10 in text of length 100,000\n";

    auto start = std::chrono::high_resolution_clock::now();
    auto rk_matches = RabinKarp::multiPatternSearch(large_text, patterns);
    auto end = std::chrono::high_resolution_clock::now();

    int rk_total_matches = 0;
    for (const auto &[pattern, matches] : rk_matches)
    {
        rk_total_matches += matches.size();
    }

    auto rk_elapsed = getElapsedMicroseconds(start, end);
    std::cout << "Rabin-Karp multi-pattern found " << rk_total_matches << " matches in "
              << rk_elapsed.count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    auto ac_matches = AhoCorasick::search(large_text, patterns);
    end = std::chrono::high_resolution_clock::now();

    int ac_total_matches = 0;
    for (const auto &[pattern, matches] : ac_matches)
    {
        ac_total_matches += matches.size();
    }

    auto ac_elapsed = getElapsedMicroseconds(start, end);
    std::cout << "Aho-Corasick found " << ac_total_matches << " matches in "
              << ac_elapsed.count() << " microseconds\n";

    // Verify the results match
    assert(rk_total_matches == ac_total_matches);
}

// --------------------------------------------------------------------------
// Demo applications
// --------------------------------------------------------------------------
void runDemos()
{
    std::cout << "\n=== STRING ALGORITHM DEMOS ===\n";

    // Basic pattern matching demo
    {
        std::cout << "\n--- Basic Pattern Matching ---\n";

        std::string text = "ABABDABACDABABCABAB";
        std::string pattern = "ABABCABAB";

        std::cout << "Text: " << text << "\n";
        std::cout << "Pattern: " << pattern << "\n\n";

        auto naive_matches = Naive::search(text, pattern);
        auto kmp_matches = KMP::search(text, pattern);
        auto rk_matches = RabinKarp::search(text, pattern);
        auto z_matches = ZAlgorithm::search(text, pattern);
        auto bm_matches = BoyerMoore::search(text, pattern);

        printMatches(naive_matches, "Naive");
        printMatches(kmp_matches, "KMP");
        printMatches(rk_matches, "Rabin-Karp");
        printMatches(z_matches, "Z Algorithm");
        printMatches(bm_matches, "Boyer-Moore");

        // Visualize KMP for educational purposes
        std::cout << "\nKMP Algorithm Visualization:\n";
        KMP::visualizeKMP("ABABCABAB", "ABC");
    }

    // DNA sequence matching demo
    {
        std::cout << "\n--- DNA Sequence Matching ---\n";

        std::string dna = "ACGTGCACGTACGTACGTAGCTAGCTAGCTACGATCGATCGTACGTAGCTACGT";
        std::vector<std::string> genes = {"ACGT", "TAGC", "GATC", "NNNN"};

        std::cout << "DNA: " << dna << "\n";

        for (const auto &gene : genes)
        {
            bool found = Applications::findDNAPattern(dna, gene);
            std::cout << "Gene " << gene << ": " << (found ? "Found" : "Not found") << "\n";

            if (found)
            {
                auto matches = KMP::search(dna, gene);
                std::cout << "  Positions: ";
                for (size_t i = 0; i < std::min(matches.size(), static_cast<size_t>(5)); i++)
                {
                    std::cout << matches[i];
                    if (i < std::min(matches.size(), static_cast<size_t>(5)) - 1)
                    {
                        std::cout << ", ";
                    }
                }
                if (matches.size() > 5)
                {
                    std::cout << ", ...";
                }
                std::cout << "\n";
            }
        }
    }

    // Plagiarism detection demo
    {
        std::cout << "\n--- Plagiarism Detection ---\n";

        Applications::Document doc1 = {
            "The quick brown fox jumps over the lazy dog. The dog was not amused.",
            "Original Document"};

        Applications::Document doc2 = {
            "The quick brown fox jumps over the lazy canine. The dog was not happy.",
            "Possible Plagiarism"};

        Applications::Document doc3 = {
            "A fast auburn fox leaped across a drowsy hound. The hound was irritated.",
            "Similar Concept, Different Words"};

        Applications::Document doc4 = {
            "Completely different content with no similarity to other documents.",
            "Unrelated Document"};

        std::cout << "Document 1: \"" << doc1.content << "\"\n";
        std::cout << "Document 2: \"" << doc2.content << "\"\n";
        std::cout << "Document 3: \"" << doc3.content << "\"\n";
        std::cout << "Document 4: \"" << doc4.content << "\"\n\n";

        double sim1_2 = Applications::calculateDocumentSimilarity(doc1, doc2);
        double sim1_3 = Applications::calculateDocumentSimilarity(doc1, doc3);
        double sim1_4 = Applications::calculateDocumentSimilarity(doc1, doc4);

        std::cout << "Similarity 1-2: " << std::fixed << std::setprecision(2) << sim1_2 * 100 << "%\n";
        std::cout << "Similarity 1-3: " << std::fixed << std::setprecision(2) << sim1_3 * 100 << "%\n";
        std::cout << "Similarity 1-4: " << std::fixed << std::setprecision(2) << sim1_4 * 100 << "%\n";
    }

    // Search and replace demo
    {
        std::cout << "\n--- Search and Replace ---\n";

        std::string text = "The cat sat on the mat. The cat was happy. Another cat walked by.";
        std::string pattern = "cat";
        std::string replacement = "dog";

        std::cout << "Original: " << text << "\n";

        std::string replaced = Applications::searchAndReplace(text, pattern, replacement);

        std::cout << "Replaced: " << replaced << "\n";
    }

    // Spell checker demo
    {
        std::cout << "\n--- Spell Checker ---\n";

        std::vector<std::string> dictionary = {
            "apple", "banana", "cherry", "date", "elderberry",
            "fig", "grape", "honeydew", "kiwi", "lemon",
            "mango", "nectarine", "orange", "peach", "quince",
            "raspberry", "strawberry", "tangerine", "watermelon"};

        std::vector<std::string> misspelled = {
            "appel", "banan", "cheery", "dat", "eldrberry", "figg"};

        for (const auto &word : misspelled)
        {
            std::cout << "Suggestions for \"" << word << "\": ";

            auto suggestions = Applications::findSimilarWords(word, dictionary);

            if (suggestions.empty())
            {
                std::cout << "No suggestions found.";
            }
            else
            {
                for (size_t i = 0; i < suggestions.size(); i++)
                {
                    std::cout << suggestions[i];
                    if (i < suggestions.size() - 1)
                    {
                        std::cout << ", ";
                    }
                }
            }

            std::cout << "\n";
        }
    }

    // Data compression demo
    {
        std::cout << "\n--- Data Compression ---\n";

        std::string text = "ABABABCABABABABCABABABABCABABA";

        std::cout << "Original text (" << text.length() << " bytes): " << text << "\n";

        auto tokens = Applications::compressLZ(text);

        std::cout << "Compressed to " << tokens.size() << " tokens:\n";
        for (const auto &token : tokens)
        {
            if (token.isReference)
            {
                std::cout << "(" << token.offset << "," << token.length << ","
                          << (token.nextChar ? token.nextChar : '#') << ") ";
            }
            else
            {
                std::cout << "'" << token.nextChar << "' ";
            }
        }
        std::cout << "\n";

        std::string decompressed = Applications::decompressLZ(tokens);

        std::cout << "Decompressed: " << decompressed << "\n";
        std::cout << "Decompression correct: " << (text == decompressed ? "Yes" : "No") << "\n";
    }
}

// --------------------------------------------------------------------------
// Interview problems
// --------------------------------------------------------------------------
namespace InterviewProblems
{
    // Problem 1: Implement strStr() (LeetCode #28)
    int strStr(const std::string &haystack, const std::string &needle)
    {
        if (needle.empty())
            return 0;

        std::vector<int> matches = KMP::search(haystack, needle);
        return matches.empty() ? -1 : matches[0];
    }

    // Problem 2: Repeated String Match (LeetCode #686)
    int repeatedStringMatch(const std::string &a, const std::string &b)
    {
        if (b.empty())
            return 0;

        // Calculate minimum repetitions needed
        int repeats = ceil(static_cast<double>(b.length()) / a.length());

        // Create the repeated string
        std::string repeated = "";
        for (int i = 0; i < repeats; i++)
        {
            repeated += a;
        }

        // Check if b is a substring
        if (ZAlgorithm::search(repeated, b).size() > 0)
        {
            return repeats;
        }

        // Try one more repetition
        repeated += a;
        if (ZAlgorithm::search(repeated, b).size() > 0)
        {
            return repeats + 1;
        }

        return -1;
    }

    // Problem 3: Shortest Palindrome (LeetCode #214)
    std::string shortestPalindrome(const std::string &s)
    {
        if (s.empty())
            return "";

        // Create a new string with s and its reverse, separated by a special character
        std::string reversed = s;
        std::reverse(reversed.begin(), reversed.end());
        std::string combined = s + "#" + reversed;

        // Compute LPS array
        std::vector<int> lps = KMP::computeLPSArray(combined);

        // The last value in LPS array gives the length of the largest palindrome prefix
        int palindrome_length = lps.back();

        // Create the shortest palindrome
        std::string prefix = s.substr(palindrome_length);
        std::reverse(prefix.begin(), prefix.end());

        return prefix + s;
    }
}

// --------------------------------------------------------------------------
// Main function
// --------------------------------------------------------------------------
int main()
{
    std::cout << "===================================\n";
    std::cout << "Day 41: String Algorithms\n";
    std::cout << "===================================\n";

    // Run demonstrations
    runDemos();

    // Run benchmarks
    runBenchmarks();

    // Test interview problems
    std::cout << "\n=== INTERVIEW PROBLEMS ===\n";

    // Problem 1: Implement strStr()
    {
        std::cout << "\n--- Problem 1: Implement strStr() ---\n";

        std::string haystack = "hello";
        std::string needle = "ll";

        int result = InterviewProblems::strStr(haystack, needle);

        std::cout << "haystack: \"" << haystack << "\", needle: \"" << needle << "\"\n";
        std::cout << "Result: " << result << " (Expected: 2)\n";
    }

    // Problem 2: Repeated String Match
    {
        std::cout << "\n--- Problem 2: Repeated String Match ---\n";

        std::string a = "abcd";
        std::string b = "cdabcdab";

        int result = InterviewProblems::repeatedStringMatch(a, b);

        std::cout << "a: \"" << a << "\", b: \"" << b << "\"\n";
        std::cout << "Result: " << result << " (Expected: 3)\n";
    }

    // Problem 3: Shortest Palindrome
    {
        std::cout << "\n--- Problem 3: Shortest Palindrome ---\n";

        std::string s = "aacecaaa";

        std::string result = InterviewProblems::shortestPalindrome(s);

        std::cout << "s: \"" << s << "\"\n";
        std::cout << "Result: \"" << result << "\" (Expected: \"aaacecaaa\")\n";
    }

    std::cout << "\nAll demonstrations and benchmarks completed successfully.\n";
    return 0;
}