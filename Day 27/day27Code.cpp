#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <memory>
#include <iomanip>
#include <cctype>

/**
 * @brief Comprehensive Trie Data Structure Implementation for #DSAin45 Day 27
 *
 * This file provides implementations of:
 * - Basic Trie with map-based children
 * - Memory-optimized Trie with array-based children
 * - Compressed Trie (Radix Tree)
 * - Ternary Search Trie
 * - Applications: Autocomplete, Spell checking, Word games
 * - Performance benchmarks
 *
 * @author #DSAin45 Course
 * @date Day 27 of 45-Day DSA Challenge
 */

/****************************************************************************************
 * STANDARD TRIE IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Standard Trie implementation using an unordered_map for children nodes
 */
class Trie
{
private:
    /**
     * @brief Node structure for the Trie
     */
    struct TrieNode
    {
        bool isEndOfWord;                              // Flag to mark end of a word
        std::unordered_map<char, TrieNode *> children; // Children nodes
        int wordCount;                                 // Count of words ending at this node

        /**
         * @brief Constructor
         */
        TrieNode() : isEndOfWord(false), wordCount(0) {}

        /**
         * @brief Destructor - Clean up all children nodes
         */
        ~TrieNode()
        {
            for (auto &pair : children)
            {
                delete pair.second;
            }
        }
    };

    TrieNode *root; // Root node of the trie
    int wordCount;  // Total words in the trie

    /**
     * @brief Helper method to find a node corresponding to a prefix
     *
     * @param prefix The prefix to search for
     * @return TrieNode* Pointer to the node at the end of the prefix, or nullptr if not found
     */
    TrieNode *findNode(const std::string &prefix) const
    {
        TrieNode *current = root;

        for (char c : prefix)
        {
            // Check if this character exists in the current node's children
            auto it = current->children.find(c);
            if (it == current->children.end())
            {
                return nullptr; // Prefix not found
            }
            current = it->second;
        }

        return current;
    }

    /**
     * @brief Recursive helper to collect all words with a given prefix
     *
     * @param node Current node in the trie
     * @param prefix Current prefix up to this node
     * @param result Vector to collect words
     * @param limit Maximum number of results to collect (0 for unlimited)
     */
    void collectWords(TrieNode *node, const std::string &prefix, std::vector<std::string> &result, int limit = 0) const
    {
        if (!node)
            return;

        // If we've reached a word, add it to the result
        if (node->isEndOfWord)
        {
            result.push_back(prefix);

            // If we've reached the limit, stop collecting
            if (limit > 0 && result.size() >= limit)
            {
                return;
            }
        }

        // Recursively visit all children
        for (const auto &pair : node->children)
        {
            if (limit > 0 && result.size() >= limit)
            {
                break; // Reached the limit
            }

            char c = pair.first;
            TrieNode *child = pair.second;
            collectWords(child, prefix + c, result, limit);
        }
    }

    /**
     * @brief Recursive helper method for removing a word
     *
     * @param node Current node in trie
     * @param word Word to remove
     * @param depth Current depth in the trie
     * @return true if the node should be deleted
     */
    bool removeHelper(TrieNode *node, const std::string &word, int depth)
    {
        // Base case: we've reached the end of the word
        if (depth == word.size())
        {
            // If this node is not the end of a word, word doesn't exist in trie
            if (!node->isEndOfWord)
            {
                return false;
            }

            // Mark as not end of word
            node->isEndOfWord = false;
            node->wordCount--;

            // Return true if this node has no children (can be deleted)
            return node->children.empty();
        }

        // Get the next character
        char current = word[depth];

        // If this character doesn't exist in the trie, word doesn't exist
        auto it = node->children.find(current);
        if (it == node->children.end())
        {
            return false;
        }

        // Recursively check if we should delete the child
        bool shouldDeleteChild = removeHelper(it->second, word, depth + 1);

        // If we should delete the child
        if (shouldDeleteChild)
        {
            delete it->second;
            node->children.erase(it);

            // Return true if this node can be deleted
            return node->children.empty() && !node->isEndOfWord;
        }

        return false;
    }

    /**
     * @brief Count nodes in the trie
     *
     * @param node Current node
     * @return int Number of nodes in subtree
     */
    int countNodes(TrieNode *node) const
    {
        if (!node)
            return 0;

        int count = 1; // Count this node

        // Count all children
        for (const auto &pair : node->children)
        {
            count += countNodes(pair.second);
        }

        return count;
    }

public:
    /**
     * @brief Constructor
     */
    Trie() : wordCount(0)
    {
        root = new TrieNode();
    }

    /**
     * @brief Destructor
     */
    ~Trie()
    {
        delete root;
    }

    /**
     * @brief Insert a word into the trie
     *
     * @param word Word to insert
     */
    void insert(const std::string &word)
    {
        TrieNode *current = root;

        for (char c : word)
        {
            // If this character doesn't exist, create a new node
            if (current->children.find(c) == current->children.end())
            {
                current->children[c] = new TrieNode();
            }

            // Move to the next node
            current = current->children[c];
        }

        // If this wasn't already the end of a word, increase word count
        if (!current->isEndOfWord)
        {
            wordCount++;
        }

        // Mark as end of word and increment count for this node
        current->isEndOfWord = true;
        current->wordCount++;
    }

    /**
     * @brief Search for a word in the trie
     *
     * @param word Word to search for
     * @return true if word exists in trie
     */
    bool search(const std::string &word) const
    {
        TrieNode *node = findNode(word);
        return node != nullptr && node->isEndOfWord;
    }

    /**
     * @brief Check if any word in trie starts with the given prefix
     *
     * @param prefix Prefix to check
     * @return true if prefix exists
     */
    bool startsWith(const std::string &prefix) const
    {
        return findNode(prefix) != nullptr;
    }

    /**
     * @brief Find all words with the given prefix
     *
     * @param prefix Prefix to search for
     * @param limit Maximum number of results (0 for all matches)
     * @return std::vector<std::string> Vector of matching words
     */
    std::vector<std::string> findWordsWithPrefix(const std::string &prefix, int limit = 0) const
    {
        std::vector<std::string> result;
        TrieNode *prefixNode = findNode(prefix);

        if (prefixNode)
        {
            collectWords(prefixNode, prefix, result, limit);
        }

        return result;
    }

    /**
     * @brief Remove a word from the trie
     *
     * @param word Word to remove
     * @return true if word was removed
     */
    bool remove(const std::string &word)
    {
        if (word.empty())
            return false;

        bool result = removeHelper(root, word, 0);
        if (result)
        {
            wordCount--;
        }
        return result;
    }

    /**
     * @brief Get count of words in the trie
     *
     * @return int Number of words
     */
    int size() const
    {
        return wordCount;
    }

    /**
     * @brief Get count of nodes in the trie
     *
     * @return int Number of nodes
     */
    int nodeCount() const
    {
        return countNodes(root);
    }

    /**
     * @brief Check if trie is empty
     *
     * @return true if no words in trie
     */
    bool empty() const
    {
        return wordCount == 0;
    }

    /**
     * @brief Clear all words from the trie
     */
    void clear()
    {
        delete root;
        root = new TrieNode();
        wordCount = 0;
    }

    /**
     * @brief Find the longest common prefix of all words in trie
     *
     * @return std::string Longest common prefix
     */
    std::string longestCommonPrefix() const
    {
        if (empty())
        {
            return "";
        }

        std::string prefix = "";
        TrieNode *current = root;

        // Traverse down the trie until we find a branch or end of word
        while (current && current->children.size() == 1 && !current->isEndOfWord)
        {
            auto it = current->children.begin();
            prefix += it->first;
            current = it->second;
        }

        return prefix;
    }

    /**
     * @brief Find the longest prefix of the given word that is a word in the trie
     *
     * @param word Word to check
     * @return std::string Longest prefix that is a word
     */
    std::string findLongestPrefix(const std::string &word) const
    {
        TrieNode *current = root;
        std::string prefix = "";
        std::string lastMatchedPrefix = "";

        for (char c : word)
        {
            // Check if this character exists
            auto it = current->children.find(c);
            if (it == current->children.end())
            {
                break;
            }

            prefix += c;
            current = it->second;

            // If this is the end of a word, update the last matched prefix
            if (current->isEndOfWord)
            {
                lastMatchedPrefix = prefix;
            }
        }

        return lastMatchedPrefix;
    }

    /**
     * @brief Build a new trie from a vector of words
     *
     * @param words Vector of words to add
     */
    void build(const std::vector<std::string> &words)
    {
        clear();
        for (const auto &word : words)
        {
            insert(word);
        }
    }

    /**
     * @brief Get all words in the trie
     *
     * @return std::vector<std::string> All words
     */
    std::vector<std::string> getAllWords() const
    {
        std::vector<std::string> result;
        collectWords(root, "", result);
        return result;
    }
};

/****************************************************************************************
 * ARRAY-BASED TRIE (MEMORY OPTIMIZED FOR ENGLISH ALPHABET)
 ****************************************************************************************/

/**
 * @brief Array-based Trie implementation for lowercase English letters
 * This is more memory efficient for a known alphabet size
 */
class ArrayTrie
{
private:
    /**
     * @brief Node structure for the Array-based Trie
     */
    struct TrieNode
    {
        bool isEndOfWord;       // Flag to mark end of a word
        TrieNode *children[26]; // Array of children (one for each lowercase letter)
        int wordCount;          // Count of words ending at this node

        /**
         * @brief Constructor - Initialize all children to nullptr
         */
        TrieNode() : isEndOfWord(false), wordCount(0)
        {
            for (int i = 0; i < 26; i++)
            {
                children[i] = nullptr;
            }
        }

        /**
         * @brief Destructor - Clean up all children nodes
         */
        ~TrieNode()
        {
            for (int i = 0; i < 26; i++)
            {
                delete children[i];
            }
        }
    };

    TrieNode *root; // Root node of the trie
    int wordCount;  // Total words in the trie

    /**
     * @brief Convert a character to its index (0-25 for a-z)
     *
     * @param c Character to convert
     * @return int Index for the character
     */
    int charToIndex(char c) const
    {
        return c - 'a';
    }

    /**
     * @brief Convert an index (0-25) to its corresponding character (a-z)
     *
     * @param index Index to convert
     * @return char Corresponding character
     */
    char indexToChar(int index) const
    {
        return 'a' + index;
    }

    /**
     * @brief Helper method to find a node corresponding to a prefix
     *
     * @param prefix The prefix to search for
     * @return TrieNode* Pointer to the node at the end of the prefix, or nullptr if not found
     */
    TrieNode *findNode(const std::string &prefix) const
    {
        TrieNode *current = root;

        for (char c : prefix)
        {
            int index = charToIndex(c);

            // Check if valid index and if child exists
            if (index < 0 || index >= 26 || !current->children[index])
            {
                return nullptr; // Prefix not found
            }

            current = current->children[index];
        }

        return current;
    }

    /**
     * @brief Recursive helper to collect all words with a given prefix
     *
     * @param node Current node in the trie
     * @param prefix Current prefix up to this node
     * @param result Vector to collect words
     * @param limit Maximum number of results to collect (0 for unlimited)
     */
    void collectWords(TrieNode *node, const std::string &prefix, std::vector<std::string> &result, int limit = 0) const
    {
        if (!node)
            return;

        // If we've reached a word, add it to the result
        if (node->isEndOfWord)
        {
            result.push_back(prefix);

            // If we've reached the limit, stop collecting
            if (limit > 0 && result.size() >= limit)
            {
                return;
            }
        }

        // Recursively visit all children
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i])
            {
                if (limit > 0 && result.size() >= limit)
                {
                    break; // Reached the limit
                }

                char c = indexToChar(i);
                collectWords(node->children[i], prefix + c, result, limit);
            }
        }
    }

    /**
     * @brief Recursive helper method for removing a word
     *
     * @param node Current node in trie
     * @param word Word to remove
     * @param depth Current depth in the trie
     * @return true if the node should be deleted
     */
    bool removeHelper(TrieNode *node, const std::string &word, int depth)
    {
        // Base case: we've reached the end of the word
        if (depth == word.size())
        {
            // If this node is not the end of a word, word doesn't exist in trie
            if (!node->isEndOfWord)
            {
                return false;
            }

            // Mark as not end of word
            node->isEndOfWord = false;
            node->wordCount--;

            // Check if node has any children
            for (int i = 0; i < 26; i++)
            {
                if (node->children[i])
                {
                    return false; // Node has children, can't delete
                }
            }

            return true; // No children, can delete
        }

        // Get the next character
        int index = charToIndex(word[depth]);

        // If this character doesn't exist in the trie, word doesn't exist
        if (index < 0 || index >= 26 || !node->children[index])
        {
            return false;
        }

        // Recursively check if we should delete the child
        bool shouldDeleteChild = removeHelper(node->children[index], word, depth + 1);

        // If we should delete the child
        if (shouldDeleteChild)
        {
            delete node->children[index];
            node->children[index] = nullptr;

            // Check if node has any other children
            for (int i = 0; i < 26; i++)
            {
                if (node->children[i])
                {
                    return false; // Node has other children, can't delete
                }
            }

            // No children and not end of word, can delete
            return !node->isEndOfWord;
        }

        return false;
    }

    /**
     * @brief Count nodes in the trie
     *
     * @param node Current node
     * @return int Number of nodes in subtree
     */
    int countNodes(TrieNode *node) const
    {
        if (!node)
            return 0;

        int count = 1; // Count this node

        // Count all children
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i])
            {
                count += countNodes(node->children[i]);
            }
        }

        return count;
    }

public:
    /**
     * @brief Constructor
     */
    ArrayTrie() : wordCount(0)
    {
        root = new TrieNode();
    }

    /**
     * @brief Destructor
     */
    ~ArrayTrie()
    {
        delete root;
    }

    /**
     * @brief Insert a word into the trie
     *
     * @param word Word to insert
     * @return bool True if word was inserted, false if invalid characters
     */
    bool insert(const std::string &word)
    {
        TrieNode *current = root;

        for (char c : word)
        {
            int index = charToIndex(c);

            // Check if valid lowercase letter
            if (index < 0 || index >= 26)
            {
                return false; // Invalid character
            }

            // If this character doesn't exist, create a new node
            if (!current->children[index])
            {
                current->children[index] = new TrieNode();
            }

            // Move to the next node
            current = current->children[index];
        }

        // If this wasn't already the end of a word, increase word count
        if (!current->isEndOfWord)
        {
            wordCount++;
        }

        // Mark as end of word and increment count for this node
        current->isEndOfWord = true;
        current->wordCount++;

        return true;
    }

    /**
     * @brief Search for a word in the trie
     *
     * @param word Word to search for
     * @return true if word exists in trie
     */
    bool search(const std::string &word) const
    {
        TrieNode *node = findNode(word);
        return node != nullptr && node->isEndOfWord;
    }

    /**
     * @brief Check if any word in trie starts with the given prefix
     *
     * @param prefix Prefix to check
     * @return true if prefix exists
     */
    bool startsWith(const std::string &prefix) const
    {
        return findNode(prefix) != nullptr;
    }

    /**
     * @brief Find all words with the given prefix
     *
     * @param prefix Prefix to search for
     * @param limit Maximum number of results (0 for all matches)
     * @return std::vector<std::string> Vector of matching words
     */
    std::vector<std::string> findWordsWithPrefix(const std::string &prefix, int limit = 0) const
    {
        std::vector<std::string> result;
        TrieNode *prefixNode = findNode(prefix);

        if (prefixNode)
        {
            collectWords(prefixNode, prefix, result, limit);
        }

        return result;
    }

    /**
     * @brief Remove a word from the trie
     *
     * @param word Word to remove
     * @return true if word was removed
     */
    bool remove(const std::string &word)
    {
        if (word.empty())
            return false;

        bool result = removeHelper(root, word, 0);
        if (result)
        {
            wordCount--;
        }
        return result;
    }

    /**
     * @brief Get count of words in the trie
     *
     * @return int Number of words
     */
    int size() const
    {
        return wordCount;
    }

    /**
     * @brief Get count of nodes in the trie
     *
     * @return int Number of nodes
     */
    int nodeCount() const
    {
        return countNodes(root);
    }

    /**
     * @brief Check if trie is empty
     *
     * @return true if no words in trie
     */
    bool empty() const
    {
        return wordCount == 0;
    }

    /**
     * @brief Clear all words from the trie
     */
    void clear()
    {
        delete root;
        root = new TrieNode();
        wordCount = 0;
    }

    /**
     * @brief Build a new trie from a vector of words
     *
     * @param words Vector of words to add
     * @return int Number of words successfully added
     */
    int build(const std::vector<std::string> &words)
    {
        clear();
        int added = 0;
        for (const auto &word : words)
        {
            if (insert(word))
            {
                added++;
            }
        }
        return added;
    }

    /**
     * @brief Get all words in the trie
     *
     * @return std::vector<std::string> All words
     */
    std::vector<std::string> getAllWords() const
    {
        std::vector<std::string> result;
        collectWords(root, "", result);
        return result;
    }
};

/****************************************************************************************
 * COMPRESSED TRIE (RADIX TREE) IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Compressed Trie (Radix Tree) implementation
 * Merges nodes that have only one child to save space
 */
class CompressedTrie
{
private:
    /**
     * @brief Node structure for the Compressed Trie
     */
    struct TrieNode
    {
        bool isEndOfWord;                                     // Flag to mark end of a word
        std::unordered_map<std::string, TrieNode *> children; // Children nodes with edge labels
        int wordCount;                                        // Count of words ending at this node

        /**
         * @brief Constructor
         */
        TrieNode() : isEndOfWord(false), wordCount(0) {}

        /**
         * @brief Destructor - Clean up all children nodes
         */
        ~TrieNode()
        {
            for (auto &pair : children)
            {
                delete pair.second;
            }
        }
    };

    TrieNode *root; // Root node of the trie
    int wordCount;  // Total words in the trie

    /**
     * @brief Helper method to find the longest common prefix of two strings
     *
     * @param s1 First string
     * @param s2 Second string
     * @return std::string Longest common prefix
     */
    std::string longestCommonPrefix(const std::string &s1, const std::string &s2) const
    {
        int minLength = std::min(s1.length(), s2.length());
        for (int i = 0; i < minLength; i++)
        {
            if (s1[i] != s2[i])
            {
                return s1.substr(0, i);
            }
        }
        return s1.substr(0, minLength);
    }

    /**
     * @brief Helper method to find a node and the remaining suffix for a given word
     *
     * @param word Word to search for
     * @param remainingSuffix Output parameter for the remaining suffix
     * @return std::pair<TrieNode*, std::string> Node and edge where search ended
     */
    std::pair<TrieNode *, std::string> findNodeAndEdge(const std::string &word) const
    {
        TrieNode *current = root;
        std::string remainingWord = word;

        while (!remainingWord.empty())
        {
            bool foundEdge = false;

            for (const auto &pair : current->children)
            {
                const std::string &edge = pair.first;

                // If the remaining word starts with this edge
                if (remainingWord.find(edge) == 0)
                {
                    // Move to the next node and remove the edge from remaining word
                    current = pair.second;
                    remainingWord = remainingWord.substr(edge.length());
                    foundEdge = true;
                    break;
                }

                // If the edge starts with the remaining word (partial match)
                std::string commonPrefix = longestCommonPrefix(edge, remainingWord);
                if (!commonPrefix.empty())
                {
                    // Return the current node and the edge that partially matches
                    return {current, edge};
                }
            }

            // If no edge was found that matches the remaining word
            if (!foundEdge)
            {
                return {current, ""};
            }
        }

        // Found exact match
        return {current, ""};
    }

    /**
     * @brief Recursive helper to collect all words with a given prefix
     *
     * @param node Current node in the trie
     * @param prefix Current prefix up to this node
     * @param result Vector to collect words
     * @param limit Maximum number of results to collect (0 for unlimited)
     */
    void collectWords(TrieNode *node, const std::string &prefix, std::vector<std::string> &result, int limit = 0) const
    {
        if (!node)
            return;

        // If we've reached a word, add it to the result
        if (node->isEndOfWord)
        {
            result.push_back(prefix);

            // If we've reached the limit, stop collecting
            if (limit > 0 && result.size() >= limit)
            {
                return;
            }
        }

        // Recursively visit all children
        for (const auto &pair : node->children)
        {
            if (limit > 0 && result.size() >= limit)
            {
                break; // Reached the limit
            }

            const std::string &edge = pair.first;
            TrieNode *child = pair.second;
            collectWords(child, prefix + edge, result, limit);
        }
    }

    /**
     * @brief Count nodes in the trie
     *
     * @param node Current node
     * @return int Number of nodes in subtree
     */
    int countNodes(TrieNode *node) const
    {
        if (!node)
            return 0;

        int count = 1; // Count this node

        // Count all children
        for (const auto &pair : node->children)
        {
            count += countNodes(pair.second);
        }

        return count;
    }

public:
    /**
     * @brief Constructor
     */
    CompressedTrie() : wordCount(0)
    {
        root = new TrieNode();
    }

    /**
     * @brief Destructor
     */
    ~CompressedTrie()
    {
        delete root;
    }

    /**
     * @brief Insert a word into the trie
     *
     * @param word Word to insert
     */
    void insert(const std::string &word)
    {
        if (word.empty())
            return;

        // Find the node where we should insert the word
        auto [current, existingEdge] = findNodeAndEdge(word);

        // If we found an exact node for the word
        if (existingEdge.empty())
        {
            // Check if we need to handle a remaining suffix
            std::string remainingSuffix = "";

            // Find where the traversal ended to calculate remaining suffix
            std::string traversedPrefix = "";
            TrieNode *traverseNode = root;

            while (traverseNode != current && traversedPrefix.length() < word.length())
            {
                bool foundEdge = false;

                for (const auto &pair : traverseNode->children)
                {
                    const std::string &edge = pair.first;

                    if (word.substr(traversedPrefix.length()).find(edge) == 0)
                    {
                        traverseNode = pair.second;
                        traversedPrefix += edge;
                        foundEdge = true;
                        break;
                    }
                }

                if (!foundEdge)
                    break;
            }

            // If we didn't traverse the entire word, the rest is the remaining suffix
            if (traversedPrefix.length() < word.length())
            {
                remainingSuffix = word.substr(traversedPrefix.length());
            }

            // If there's a remaining suffix, add a new node
            if (!remainingSuffix.empty())
            {
                current->children[remainingSuffix] = new TrieNode();
                current = current->children[remainingSuffix];
            }

            // If this wasn't already the end of a word, increase word count
            if (!current->isEndOfWord)
            {
                wordCount++;
            }

            // Mark as end of word and increment count for this node
            current->isEndOfWord = true;
            current->wordCount++;
            return;
        }

        // Partial match with an existing edge
        std::string prefix = word;
        std::string commonPrefix = longestCommonPrefix(existingEdge, prefix);

        // Original edge without the common prefix
        std::string existingSuffix = existingEdge.substr(commonPrefix.length());

        // Remaining part of the word to insert
        std::string newSuffix = prefix.substr(commonPrefix.length());

        // Get the existing child node
        TrieNode *existingChild = current->children[existingEdge];

        // Remove the original edge
        current->children.erase(existingEdge);

        // Create a new interim node for the common prefix
        TrieNode *newNode = new TrieNode();
        current->children[commonPrefix] = newNode;

        // Add the existing suffix back to the tree
        if (!existingSuffix.empty())
        {
            newNode->children[existingSuffix] = existingChild;
        }
        else
        {
            // If there's no existing suffix, the existing child becomes the new node
            newNode->isEndOfWord = existingChild->isEndOfWord;
            newNode->wordCount = existingChild->wordCount;

            // Move all children from existingChild to newNode
            for (auto &pair : existingChild->children)
            {
                newNode->children[pair.first] = pair.second;
            }

            // Clear the existing child to avoid double deletion
            existingChild->children.clear();
            delete existingChild;
        }

        // Add the new suffix if necessary
        if (!newSuffix.empty())
        {
            newNode->children[newSuffix] = new TrieNode();
            newNode->children[newSuffix]->isEndOfWord = true;
            newNode->children[newSuffix]->wordCount = 1;
        }
        else
        {
            // If there's no new suffix, mark the new node as the end of a word
            newNode->isEndOfWord = true;
            newNode->wordCount++;
        }

        wordCount++; // Increment total word count
    }

    /**
     * @brief Search for a word in the trie
     *
     * @param word Word to search for
     * @return true if word exists in trie
     */
    bool search(const std::string &word) const
    {
        TrieNode *current = root;
        std::string remainingWord = word;

        while (!remainingWord.empty())
        {
            bool foundEdge = false;

            for (const auto &pair : current->children)
            {
                const std::string &edge = pair.first;

                // If the remaining word starts with this edge
                if (remainingWord.find(edge) == 0)
                {
                    // Move to the next node and remove the edge from remaining word
                    current = pair.second;
                    remainingWord = remainingWord.substr(edge.length());
                    foundEdge = true;
                    break;
                }
            }

            // If no edge was found that matches the remaining word
            if (!foundEdge)
            {
                return false;
            }
        }

        // Found exact match, check if it's the end of a word
        return current->isEndOfWord;
    }

    /**
     * @brief Check if any word in trie starts with the given prefix
     *
     * @param prefix Prefix to check
     * @return true if prefix exists
     */
    bool startsWith(const std::string &prefix) const
    {
        TrieNode *current = root;
        std::string remainingPrefix = prefix;

        while (!remainingPrefix.empty())
        {
            bool foundEdge = false;

            for (const auto &pair : current->children)
            {
                const std::string &edge = pair.first;

                // If edge is contained in the remaining prefix
                if (remainingPrefix.find(edge) == 0)
                {
                    // Move to the next node and remove the edge from remaining prefix
                    current = pair.second;
                    remainingPrefix = remainingPrefix.substr(edge.length());
                    foundEdge = true;
                    break;
                }

                // If prefix is contained in the edge
                if (edge.find(remainingPrefix) == 0)
                {
                    // Prefix is a partial match, so it exists in the trie
                    return true;
                }
            }

            // If no edge was found that matches the remaining prefix
            if (!foundEdge)
            {
                return false;
            }
        }

        // If we've processed the entire prefix, it exists in the trie
        return true;
    }

    /**
     * @brief Find all words with the given prefix
     *
     * @param prefix Prefix to search for
     * @param limit Maximum number of results (0 for all matches)
     * @return std::vector<std::string> Vector of matching words
     */
    std::vector<std::string> findWordsWithPrefix(const std::string &prefix, int limit = 0) const
    {
        std::vector<std::string> result;

        TrieNode *current = root;
        std::string remainingPrefix = prefix;
        std::string matchedPrefix = "";

        // Find the node corresponding to the prefix
        while (!remainingPrefix.empty())
        {
            bool foundEdge = false;

            for (const auto &pair : current->children)
            {
                const std::string &edge = pair.first;

                // Case 1: Edge is a prefix of what we're looking for
                if (remainingPrefix.find(edge) == 0)
                {
                    matchedPrefix += edge;
                    remainingPrefix = remainingPrefix.substr(edge.length());
                    current = pair.second;
                    foundEdge = true;
                    break;
                }

                // Case 2: What we're looking for is a prefix of the edge
                if (edge.find(remainingPrefix) == 0)
                {
                    // This is a partial match, so we need to collect words from this subtree
                    matchedPrefix += remainingPrefix;
                    remainingPrefix = "";

                    // Only include words that start with the full edge
                    if (edge == remainingPrefix)
                    {
                        collectWords(pair.second, matchedPrefix, result, limit);
                    }
                    else
                    {
                        // Include words from this subtree starting with the matched prefix + rest of edge
                        collectWords(pair.second, matchedPrefix + edge.substr(remainingPrefix.length()), result, limit);
                    }

                    foundEdge = true;
                    break;
                }

                // Case 3: Partial overlap
                std::string common = longestCommonPrefix(edge, remainingPrefix);
                if (!common.empty() && common.length() < edge.length() && common.length() < remainingPrefix.length())
                {
                    // No matches possible in this trie
                    return result;
                }
            }

            // If no matching edge was found
            if (!foundEdge)
            {
                return result;
            }
        }

        // If we've found the exact prefix, collect all words from this node
        collectWords(current, matchedPrefix, result, limit);

        return result;
    }

    /**
     * @brief Get count of words in the trie
     *
     * @return int Number of words
     */
    int size() const
    {
        return wordCount;
    }

    /**
     * @brief Get count of nodes in the trie
     *
     * @return int Number of nodes
     */
    int nodeCount() const
    {
        return countNodes(root);
    }

    /**
     * @brief Check if trie is empty
     *
     * @return true if no words in trie
     */
    bool empty() const
    {
        return wordCount == 0;
    }

    /**
     * @brief Clear all words from the trie
     */
    void clear()
    {
        delete root;
        root = new TrieNode();
        wordCount = 0;
    }

    /**
     * @brief Build a new trie from a vector of words
     *
     * @param words Vector of words to add
     */
    void build(const std::vector<std::string> &words)
    {
        clear();
        for (const auto &word : words)
        {
            insert(word);
        }
    }

    /**
     * @brief Get all words in the trie
     *
     * @return std::vector<std::string> All words
     */
    std::vector<std::string> getAllWords() const
    {
        std::vector<std::string> result;
        collectWords(root, "", result);
        return result;
    }
};

/****************************************************************************************
 * TERNARY SEARCH TRIE IMPLEMENTATION
 ****************************************************************************************/

/**
 * @brief Ternary Search Trie implementation
 * A hybrid between binary search trees and tries
 */
class TernarySearchTrie
{
private:
    /**
     * @brief Node structure for the Ternary Search Trie
     */
    struct TSTNode
    {
        char character;   // Character stored in this node
        bool isEndOfWord; // Flag to mark end of a word
        TSTNode *left;    // Left child (less than current character)
        TSTNode *middle;  // Middle child (next character in the sequence)
        TSTNode *right;   // Right child (greater than current character)
        int wordCount;    // Count of words ending at this node

        /**
         * @brief Constructor
         *
         * @param c Character to store
         */
        TSTNode(char c) : character(c), isEndOfWord(false), left(nullptr), middle(nullptr), right(nullptr), wordCount(0) {}

        /**
         * @brief Destructor - Clean up all children nodes
         */
        ~TSTNode()
        {
            delete left;
            delete middle;
            delete right;
        }
    };

    TSTNode *root; // Root node of the trie
    int wordCount; // Total words in the trie

    /**
     * @brief Helper method to insert a word into the trie
     *
     * @param node Current node (or nullptr for a new node)
     * @param word Word to insert
     * @param pos Current position in the word
     * @return TSTNode* Updated node
     */
    TSTNode *insertHelper(TSTNode *node, const std::string &word, int pos)
    {
        // Get the current character
        char c = word[pos];

        // Create a new node if necessary
        if (!node)
        {
            node = new TSTNode(c);
        }

        // Navigate based on the current character
        if (c < node->character)
        {
            node->left = insertHelper(node->left, word, pos);
        }
        else if (c > node->character)
        {
            node->right = insertHelper(node->right, word, pos);
        }
        else
        {
            // Character matches
            if (pos < word.length() - 1)
            {
                // More characters in the word, move to the next
                node->middle = insertHelper(node->middle, word, pos + 1);
            }
            else
            {
                // End of the word
                if (!node->isEndOfWord)
                {
                    wordCount++;
                }
                node->isEndOfWord = true;
                node->wordCount++;
            }
        }

        return node;
    }

    /**
     * @brief Helper method to search for a word in the trie
     *
     * @param node Current node
     * @param word Word to search for
     * @param pos Current position in the word
     * @return TSTNode* Node at the end of the word, or nullptr if not found
     */
    TSTNode *searchHelper(TSTNode *node, const std::string &word, int pos) const
    {
        if (!node)
            return nullptr;

        // Get the current character
        char c = word[pos];

        // Navigate based on the current character
        if (c < node->character)
        {
            return searchHelper(node->left, word, pos);
        }
        else if (c > node->character)
        {
            return searchHelper(node->right, word, pos);
        }
        else
        {
            // Character matches
            if (pos == word.length() - 1)
            {
                // End of the word
                return node;
            }

            // More characters in the word, move to the next
            return searchHelper(node->middle, word, pos + 1);
        }
    }

    /**
     * @brief Recursive helper to collect all words with a given prefix
     *
     * @param node Current node
     * @param prefix Current prefix
     * @param result Vector to collect words
     * @param limit Maximum number of results to collect (0 for unlimited)
     */
    void collectWords(TSTNode *node, std::string &prefix, std::vector<std::string> &result, int limit = 0) const
    {
        if (!node)
            return;

        // Traverse left subtree
        collectWords(node->left, prefix, result, limit);
        if (limit > 0 && result.size() >= limit)
            return;

        // Add current character to prefix
        prefix.push_back(node->character);

        // If this is the end of a word, add it to the result
        if (node->isEndOfWord)
        {
            result.push_back(prefix);
        }

        // Traverse middle subtree (next character in sequence)
        if (limit > 0 && result.size() >= limit)
        {
            prefix.pop_back(); // Remove the character before returning
            return;
        }
        collectWords(node->middle, prefix, result, limit);

        // Remove current character from prefix
        prefix.pop_back();

        // Traverse right subtree
        if (limit > 0 && result.size() >= limit)
            return;
        collectWords(node->right, prefix, result, limit);
    }

    /**
     * @brief Helper method to find node at the end of a prefix
     *
     * @param node Current node
     * @param prefix Prefix to search for
     * @param pos Current position in the prefix
     * @return TSTNode* Node at the end of the prefix, or nullptr if not found
     */
    TSTNode *findPrefixNode(TSTNode *node, const std::string &prefix, int pos) const
    {
        if (!node)
            return nullptr;

        // Get the current character
        char c = prefix[pos];

        // Navigate based on the current character
        if (c < node->character)
        {
            return findPrefixNode(node->left, prefix, pos);
        }
        else if (c > node->character)
        {
            return findPrefixNode(node->right, prefix, pos);
        }
        else
        {
            // Character matches
            if (pos == prefix.length() - 1)
            {
                // End of the prefix
                return node->middle;
            }

            // More characters in the prefix, move to the next
            return findPrefixNode(node->middle, prefix, pos + 1);
        }
    }

    /**
     * @brief Count nodes in the trie
     *
     * @param node Current node
     * @return int Number of nodes in subtree
     */
    int countNodes(TSTNode *node) const
    {
        if (!node)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->middle) + countNodes(node->right);
    }

public:
    /**
     * @brief Constructor
     */
    TernarySearchTrie() : root(nullptr), wordCount(0) {}

    /**
     * @brief Destructor
     */
    ~TernarySearchTrie()
    {
        delete root;
    }

    /**
     * @brief Insert a word into the trie
     *
     * @param word Word to insert
     */
    void insert(const std::string &word)
    {
        if (word.empty())
            return;
        root = insertHelper(root, word, 0);
    }

    /**
     * @brief Search for a word in the trie
     *
     * @param word Word to search for
     * @return true if word exists in trie
     */
    bool search(const std::string &word) const
    {
        if (word.empty())
            return false;
        TSTNode *node = searchHelper(root, word, 0);
        return node != nullptr && node->isEndOfWord;
    }

    /**
     * @brief Check if any word in trie starts with the given prefix
     *
     * @param prefix Prefix to check
     * @return true if prefix exists
     */
    bool startsWith(const std::string &prefix) const
    {
        if (prefix.empty())
            return false;
        TSTNode *lastNode = searchHelper(root, prefix, 0);
        return lastNode != nullptr;
    }

    /**
     * @brief Find all words with the given prefix
     *
     * @param prefix Prefix to search for
     * @param limit Maximum number of results (0 for all matches)
     * @return std::vector<std::string> Vector of matching words
     */
    std::vector<std::string> findWordsWithPrefix(const std::string &prefix, int limit = 0) const
    {
        std::vector<std::string> result;
        if (prefix.empty())
            return result;

        // Find the node at the end of the prefix
        TSTNode *prefixNode = searchHelper(root, prefix, 0);
        if (!prefixNode)
            return result;

        // If the prefix itself is a word, add it to the result
        if (prefixNode->isEndOfWord)
        {
            result.push_back(prefix);
            if (limit > 0 && result.size() >= limit)
                return result;
        }

        // Collect all words in the subtree
        std::string current = prefix;
        collectWords(prefixNode->middle, current, result, limit);

        return result;
    }

    /**
     * @brief Get count of words in the trie
     *
     * @return int Number of words
     */
    int size() const
    {
        return wordCount;
    }

    /**
     * @brief Get count of nodes in the trie
     *
     * @return int Number of nodes
     */
    int nodeCount() const
    {
        return countNodes(root);
    }

    /**
     * @brief Check if trie is empty
     *
     * @return true if no words in trie
     */
    bool empty() const
    {
        return wordCount == 0;
    }

    /**
     * @brief Clear all words from the trie
     */
    void clear()
    {
        delete root;
        root = nullptr;
        wordCount = 0;
    }

    /**
     * @brief Build a new trie from a vector of words
     *
     * @param words Vector of words to add
     */
    void build(const std::vector<std::string> &words)
    {
        clear();
        for (const auto &word : words)
        {
            insert(word);
        }
    }
};

/****************************************************************************************
 * APPLICATIONS OF TRIES
 ****************************************************************************************/

/**
 * @brief AutoComplete class - Suggests words as you type
 */
class AutoComplete
{
private:
    Trie trie;                           // Trie to store dictionary words
    std::vector<std::string> dictionary; // Full dictionary

public:
    /**
     * @brief Constructor - Initialize with a dictionary
     *
     * @param words Dictionary of words
     */
    AutoComplete(const std::vector<std::string> &words)
    {
        dictionary = words;
        trie.build(words);
    }

    /**
     * @brief Get suggestions for a prefix
     *
     * @param prefix Prefix to suggest completions for
     * @param limit Maximum number of suggestions (0 for all matches)
     * @return std::vector<std::string> Vector of suggestions
     */
    std::vector<std::string> getSuggestions(const std::string &prefix, int limit = 10) const
    {
        return trie.findWordsWithPrefix(prefix, limit);
    }

    /**
     * @brief Add a new word to the dictionary
     *
     * @param word Word to add
     */
    void addWord(const std::string &word)
    {
        if (!trie.search(word))
        {
            trie.insert(word);
            dictionary.push_back(word);
        }
    }

    /**
     * @brief Get the size of the dictionary
     *
     * @return int Number of words
     */
    int size() const
    {
        return trie.size();
    }

    /**
     * @brief Build a new dictionary
     *
     * @param words Vector of words
     */
    void build(const std::vector<std::string> &words)
    {
        dictionary = words;
        trie.build(words);
    }
};

/**
 * @brief SpellChecker class - Checks if words are spelled correctly
 */
class SpellChecker
{
private:
    Trie dictionary;                                  // Trie to store dictionary words
    std::unordered_map<std::string, int> frequencies; // Word frequencies for better suggestions

    /**
     * @brief Generate edit distance 1 variations of a word
     *
     * @param word Input word
     * @return std::vector<std::string> All words with edit distance 1
     */
    std::vector<std::string> getEditDistance1Words(const std::string &word) const
    {
        std::vector<std::string> result;

        // Deletions (remove one character)
        for (int i = 0; i < word.length(); i++)
        {
            result.push_back(word.substr(0, i) + word.substr(i + 1));
        }

        // Transpositions (swap adjacent characters)
        for (int i = 0; i < word.length() - 1; i++)
        {
            std::string s = word;
            std::swap(s[i], s[i + 1]);
            result.push_back(s);
        }

        // Replacements (change one character to another)
        for (int i = 0; i < word.length(); i++)
        {
            std::string s = word;
            for (char c = 'a'; c <= 'z'; c++)
            {
                if (c != word[i])
                {
                    s[i] = c;
                    result.push_back(s);
                }
            }
        }

        // Insertions (add one character)
        for (int i = 0; i <= word.length(); i++)
        {
            for (char c = 'a'; c <= 'z'; c++)
            {
                result.push_back(word.substr(0, i) + c + word.substr(i));
            }
        }

        return result;
    }

public:
    /**
     * @brief Constructor - Initialize with a dictionary
     *
     * @param words Dictionary of words
     * @param wordFrequencies Optional word frequencies
     */
    SpellChecker(const std::vector<std::string> &words,
                 const std::unordered_map<std::string, int> &wordFrequencies = {})
    {
        // Build the dictionary
        dictionary.build(words);

        // Store frequencies if provided
        frequencies = wordFrequencies;

        // If no frequencies provided, set default frequency of 1 for all words
        if (frequencies.empty())
        {
            for (const auto &word : words)
            {
                frequencies[word] = 1;
            }
        }
    }

    /**
     * @brief Check if a word is spelled correctly
     *
     * @param word Word to check
     * @return true if correctly spelled
     */
    bool isCorrectlySpelled(const std::string &word) const
    {
        return dictionary.search(word);
    }

    /**
     * @brief Suggest corrections for a misspelled word
     *
     * @param word Misspelled word
     * @param limit Maximum number of suggestions
     * @return std::vector<std::string> Suggested corrections
     */
    std::vector<std::string> suggestCorrections(const std::string &word, int limit = 5) const
    {
        // If the word is correct, no need for suggestions
        if (isCorrectlySpelled(word))
        {
            return {word};
        }

        // Get all possible words with edit distance 1
        std::vector<std::string> candidates = getEditDistance1Words(word);

        // Filter to only valid words and sort by frequency
        std::vector<std::pair<std::string, int>> validCandidates;
        for (const auto &candidate : candidates)
        {
            if (dictionary.search(candidate))
            {
                int freq = 1; // Default frequency
                auto it = frequencies.find(candidate);
                if (it != frequencies.end())
                {
                    freq = it->second;
                }
                validCandidates.push_back({candidate, freq});
            }
        }

        // Sort by frequency (descending)
        std::sort(validCandidates.begin(), validCandidates.end(),
                  [](const auto &a, const auto &b)
                  { return a.second > b.second; });

        // Extract just the words
        std::vector<std::string> result;
        for (const auto &pair : validCandidates)
        {
            result.push_back(pair.first);
            if (result.size() >= limit)
            {
                break;
            }
        }

        return result;
    }

    /**
     * @brief Add a new word to the dictionary
     *
     * @param word Word to add
     * @param frequency Optional frequency
     */
    void addWord(const std::string &word, int frequency = 1)
    {
        dictionary.insert(word);
        frequencies[word] = frequency;
    }

    /**
     * @brief Get the size of the dictionary
     *
     * @return int Number of words
     */
    int size() const
    {
        return dictionary.size();
    }
};

/**
 * @brief BoggleSolver class - Finds words in a Boggle board
 */
class BoggleSolver
{
private:
    Trie dictionary; // Trie to store dictionary words

    /**
     * @brief Recursive DFS to find words on the board
     *
     * @param board Boggle board
     * @param visited Visited cells
     * @param i Current row
     * @param j Current column
     * @param currentWord Current word being formed
     * @param result Set of found words
     */
    void dfs(const std::vector<std::vector<char>> &board,
             std::vector<std::vector<bool>> &visited,
             int i, int j,
             std::string &currentWord,
             std::unordered_set<std::string> &result) const
    {
        // Check bounds and if already visited
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || visited[i][j])
        {
            return;
        }

        // Add current character to the word
        currentWord.push_back(board[i][j]);

        // Check if the current word is a valid prefix
        if (!dictionary.startsWith(currentWord))
        {
            // If not a valid prefix, backtrack
            currentWord.pop_back();
            return;
        }

        // Mark as visited
        visited[i][j] = true;

        // Check if the current word is a valid word (minimum length 3)
        if (dictionary.search(currentWord) && currentWord.length() >= 3)
        {
            result.insert(currentWord);
        }

        // Explore all 8 directions
        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int k = 0; k < 8; k++)
        {
            dfs(board, visited, i + dx[k], j + dy[k], currentWord, result);
        }

        // Backtrack
        visited[i][j] = false;
        currentWord.pop_back();
    }

public:
    /**
     * @brief Constructor - Initialize with a dictionary
     *
     * @param words Dictionary of words
     */
    BoggleSolver(const std::vector<std::string> &words)
    {
        dictionary.build(words);
    }

    /**
     * @brief Find all valid words on the Boggle board
     *
     * @param board Boggle board (2D grid of characters)
     * @return std::vector<std::string> Valid words found on the board
     */
    std::vector<std::string> findWords(const std::vector<std::vector<char>> &board) const
    {
        if (board.empty() || board[0].empty())
        {
            return {};
        }

        int rows = board.size();
        int cols = board[0].size();

        // Initialize visited grid
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

        // Set to store unique found words
        std::unordered_set<std::string> foundWords;

        // Try starting from each cell
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                std::string currentWord = "";
                dfs(board, visited, i, j, currentWord, foundWords);
            }
        }

        // Convert set to vector
        std::vector<std::string> result(foundWords.begin(), foundWords.end());

        // Sort alphabetically
        std::sort(result.begin(), result.end());

        return result;
    }

    /**
     * @brief Check if a word can be formed on the board
     *
     * @param board Boggle board
     * @param word Word to check
     * @return true if word can be formed
     */
    bool canFormWord(const std::vector<std::vector<char>> &board, const std::string &word) const
    {
        if (board.empty() || board[0].empty() || word.empty())
        {
            return false;
        }

        // First check if word is in dictionary
        if (!dictionary.search(word))
        {
            return false;
        }

        int rows = board.size();
        int cols = board[0].size();

        // Initialize visited grid
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

        // Try starting from each cell
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (board[i][j] == word[0])
                {
                    if (dfsCheckWord(board, visited, i, j, word, 0))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

private:
    /**
     * @brief Helper function to check if a specific word can be formed
     *
     * @param board Boggle board
     * @param visited Visited cells
     * @param i Current row
     * @param j Current column
     * @param word Word to check
     * @param index Current index in the word
     * @return true if remaining word can be formed
     */
    bool dfsCheckWord(const std::vector<std::vector<char>> &board,
                      std::vector<std::vector<bool>> &visited,
                      int i, int j,
                      const std::string &word,
                      int index) const
    {
        // If we've matched the entire word
        if (index == word.length())
        {
            return true;
        }

        // Check bounds and if already visited or character doesn't match
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() ||
            visited[i][j] || board[i][j] != word[index])
        {
            return false;
        }

        // Mark as visited
        visited[i][j] = true;

        // Explore all 8 directions
        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int k = 0; k < 8; k++)
        {
            if (dfsCheckWord(board, visited, i + dx[k], j + dy[k], word, index + 1))
            {
                return true;
            }
        }

        // Backtrack
        visited[i][j] = false;
        return false;
    }

public:
    /**
     * @brief Add a new word to the dictionary
     *
     * @param word Word to add
     */
    void addWord(const std::string &word)
    {
        dictionary.insert(word);
    }

    /**
     * @brief Get the size of the dictionary
     *
     * @return int Number of words
     */
    int size() const
    {
        return dictionary.size();
    }
};

/**
 * @brief IPRouter class - Simulates IP routing using tries
 */
class IPRouter
{
private:
    struct RouteInfo
    {
        std::string interface;
        int priority;

        RouteInfo(const std::string &intf = "", int pri = 0)
            : interface(intf), priority(pri) {}
    };

    struct TrieNode
    {
        bool isEndOfPrefix;
        RouteInfo routeInfo;
        TrieNode *children[2]; // Binary trie (0 and 1)

        TrieNode() : isEndOfPrefix(false), children{nullptr, nullptr} {}

        ~TrieNode()
        {
            delete children[0];
            delete children[1];
        }
    };

    TrieNode *root;

    /**
     * @brief Convert an IP address to a binary string representation
     *
     * @param ip IP address (e.g., "192.168.1.1")
     * @return std::string Binary representation
     */
    std::string ipToBinary(const std::string &ip) const
    {
        std::stringstream ss(ip);
        std::string token;
        std::string result;

        while (std::getline(ss, token, '.'))
        {
            int octet = std::stoi(token);
            std::string binary;

            for (int i = 7; i >= 0; i--)
            {
                binary += ((octet >> i) & 1) ? '1' : '0';
            }

            result += binary;
        }

        return result;
    }

    /**
     * @brief Convert a CIDR notation to a binary prefix
     *
     * @param cidr CIDR notation (e.g., "192.168.1.0/24")
     * @return std::pair<std::string, int> Binary prefix and prefix length
     */
    std::pair<std::string, int> cidrToBinaryPrefix(const std::string &cidr) const
    {
        size_t slashPos = cidr.find('/');
        if (slashPos == std::string::npos)
        {
            return {ipToBinary(cidr), 32}; // Assume /32 if not specified
        }

        std::string ip = cidr.substr(0, slashPos);
        int prefixLength = std::stoi(cidr.substr(slashPos + 1));

        // Convert IP to binary
        std::string binary = ipToBinary(ip);

        // Truncate to prefix length
        return {binary.substr(0, prefixLength), prefixLength};
    }

public:
    /**
     * @brief Constructor
     */
    IPRouter()
    {
        root = new TrieNode();
    }

    /**
     * @brief Destructor
     */
    ~IPRouter()
    {
        delete root;
    }

    /**
     * @brief Add a route to the routing table
     *
     * @param cidr CIDR notation (e.g., "192.168.1.0/24")
     * @param interface Interface name (e.g., "eth0")
     * @param priority Route priority (higher is better)
     */
    void addRoute(const std::string &cidr, const std::string &interface, int priority = 0)
    {
        auto [binaryPrefix, prefixLength] = cidrToBinaryPrefix(cidr);

        TrieNode *current = root;

        for (int i = 0; i < prefixLength; i++)
        {
            int bit = binaryPrefix[i] - '0';

            if (!current->children[bit])
            {
                current->children[bit] = new TrieNode();
            }

            current = current->children[bit];
        }

        // Store route information
        current->isEndOfPrefix = true;

        // Only update if new route has higher priority
        if (current->routeInfo.interface.empty() || priority > current->routeInfo.priority)
        {
            current->routeInfo = RouteInfo(interface, priority);
        }
    }

    /**
     * @brief Find the best matching route for an IP address
     *
     * @param ip IP address (e.g., "192.168.1.1")
     * @return std::string Best matching interface or "No route" if not found
     */
    std::string findBestRoute(const std::string &ip) const
    {
        std::string binaryIP = ipToBinary(ip);

        TrieNode *current = root;
        TrieNode *lastMatchNode = nullptr;

        for (int i = 0; i < binaryIP.length() && current; i++)
        {
            int bit = binaryIP[i] - '0';

            if (current->isEndOfPrefix)
            {
                lastMatchNode = current;
            }

            current = current->children[bit];
        }

        // Check the last node
        if (current && current->isEndOfPrefix)
        {
            lastMatchNode = current;
        }

        if (lastMatchNode)
        {
            return lastMatchNode->routeInfo.interface;
        }

        return "No route";
    }

    /**
     * @brief Clear all routes
     */
    void clear()
    {
        delete root;
        root = new TrieNode();
    }
};

/****************************************************************************************
 * BENCHMARKING UTILITIES
 ****************************************************************************************/

/**
 * @brief Generate a random string of the given length
 *
 * @param length Length of string to generate
 * @return std::string Random string
 */
std::string generateRandomString(int length)
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; i++)
    {
        result += charset[distribution(generator)];
    }

    return result;
}

/**
 * @brief Generate a vector of random strings
 *
 * @param count Number of strings to generate
 * @param minLength Minimum string length
 * @param maxLength Maximum string length
 * @return std::vector<std::string> Vector of random strings
 */
std::vector<std::string> generateRandomStrings(int count, int minLength = 3, int maxLength = 10)
{
    std::vector<std::string> result;
    result.reserve(count);

    std::mt19937 lengthGenerator(std::random_device{}());
    std::uniform_int_distribution<int> lengthDistribution(minLength, maxLength);

    for (int i = 0; i < count; i++)
    {
        int length = lengthDistribution(lengthGenerator);
        result.push_back(generateRandomString(length));
    }

    return result;
}

/**
 * @brief Load words from a file
 *
 * @param filename File to load from
 * @return std::vector<std::string> Vector of words
 */
std::vector<std::string> loadWordsFromFile(const std::string &filename)
{
    std::vector<std::string> words;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return words;
    }

    std::string word;
    while (std::getline(file, word))
    {
        // Remove any trailing whitespace
        word.erase(word.find_last_not_of(" \n\r\t") + 1);

        // Skip empty lines
        if (!word.empty())
        {
            words.push_back(word);
        }
    }

    return words;
}

/**
 * @brief Benchmark trie implementations with various operations
 *
 * @param wordCount Number of words to use
 * @param iterations Number of iterations for each test
 */
void benchmarkTrieImplementations(int wordCount = 10000, int iterations = 5)
{
    std::cout << "===== Trie Implementation Benchmarks =====" << std::endl;
    std::cout << "Words: " << wordCount << ", Iterations: " << iterations << std::endl
              << std::endl;

    // Generate test data
    std::vector<std::string> words = generateRandomStrings(wordCount);

    // Make sure there are some common prefixes for realistic testing
    for (int i = 0; i < wordCount / 10; i++)
    {
        if (i < words.size() - 1)
        {
            words[i + wordCount / 10] = words[i].substr(0, words[i].length() / 2) +
                                        generateRandomString(3);
        }
    }

    // Standard Trie
    double stdTrieInsertTime = 0;
    double stdTrieSearchTime = 0;
    double stdTriePrefixTime = 0;
    int stdTrieNodeCount = 0;

    // Array Trie
    double arrayTrieInsertTime = 0;
    double arrayTrieSearchTime = 0;
    double arrayTriePrefixTime = 0;
    int arrayTrieNodeCount = 0;

    // Compressed Trie
    double compressedTrieInsertTime = 0;
    double compressedTrieSearchTime = 0;
    double compressedTriePrefixTime = 0;
    int compressedTrieNodeCount = 0;

    // Ternary Search Trie
    double tstInsertTime = 0;
    double tstSearchTime = 0;
    double tstPrefixTime = 0;
    int tstNodeCount = 0;

    for (int iter = 0; iter < iterations; iter++)
    {
        // Standard Trie Benchmarks
        {
            Trie trie;

            // Insert
            auto start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.insert(word);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            stdTrieInsertTime += duration.count();

            if (iter == 0)
            {
                stdTrieNodeCount = trie.nodeCount();
            }

            // Search
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.search(word);
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            stdTrieSearchTime += duration.count();

            // Prefix search (use first 3 chars as prefix)
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                if (word.length() >= 3)
                {
                    trie.findWordsWithPrefix(word.substr(0, 3), 10);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            stdTriePrefixTime += duration.count();
        }

        // Array Trie Benchmarks
        {
            ArrayTrie trie;

            // Insert
            auto start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.insert(word);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            arrayTrieInsertTime += duration.count();

            if (iter == 0)
            {
                arrayTrieNodeCount = trie.nodeCount();
            }

            // Search
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.search(word);
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            arrayTrieSearchTime += duration.count();

            // Prefix search (use first 3 chars as prefix)
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                if (word.length() >= 3)
                {
                    trie.findWordsWithPrefix(word.substr(0, 3), 10);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            arrayTriePrefixTime += duration.count();
        }

        // Compressed Trie Benchmarks
        {
            CompressedTrie trie;

            // Insert
            auto start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.insert(word);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            compressedTrieInsertTime += duration.count();

            if (iter == 0)
            {
                compressedTrieNodeCount = trie.nodeCount();
            }

            // Search
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.search(word);
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            compressedTrieSearchTime += duration.count();

            // Prefix search (use first 3 chars as prefix)
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                if (word.length() >= 3)
                {
                    trie.findWordsWithPrefix(word.substr(0, 3), 10);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            compressedTriePrefixTime += duration.count();
        }

        // Ternary Search Trie Benchmarks
        {
            TernarySearchTrie trie;

            // Insert
            auto start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.insert(word);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            tstInsertTime += duration.count();

            if (iter == 0)
            {
                tstNodeCount = trie.nodeCount();
            }

            // Search
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                trie.search(word);
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            tstSearchTime += duration.count();

            // Prefix search (use first 3 chars as prefix)
            start = std::chrono::high_resolution_clock::now();
            for (const auto &word : words)
            {
                if (word.length() >= 3)
                {
                    trie.findWordsWithPrefix(word.substr(0, 3), 10);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            tstPrefixTime += duration.count();
        }
    }

    // Calculate averages
    stdTrieInsertTime /= iterations;
    stdTrieSearchTime /= iterations;
    stdTriePrefixTime /= iterations;

    arrayTrieInsertTime /= iterations;
    arrayTrieSearchTime /= iterations;
    arrayTriePrefixTime /= iterations;

    compressedTrieInsertTime /= iterations;
    compressedTrieSearchTime /= iterations;
    compressedTriePrefixTime /= iterations;

    tstInsertTime /= iterations;
    tstSearchTime /= iterations;
    tstPrefixTime /= iterations;

    // Print results in a table
    std::cout << std::left << std::setw(20) << "Implementation"
              << std::right << std::setw(15) << "Insert (ms)"
              << std::setw(15) << "Search (ms)"
              << std::setw(15) << "Prefix (ms)"
              << std::setw(15) << "Nodes"
              << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    std::cout << std::left << std::setw(20) << "Standard Trie"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << stdTrieInsertTime
              << std::setw(15) << std::fixed << std::setprecision(2) << stdTrieSearchTime
              << std::setw(15) << std::fixed << std::setprecision(2) << stdTriePrefixTime
              << std::setw(15) << stdTrieNodeCount
              << std::endl;

    std::cout << std::left << std::setw(20) << "Array Trie"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << arrayTrieInsertTime
              << std::setw(15) << std::fixed << std::setprecision(2) << arrayTrieSearchTime
              << std::setw(15) << std::fixed << std::setprecision(2) << arrayTriePrefixTime
              << std::setw(15) << arrayTrieNodeCount
              << std::endl;

    std::cout << std::left << std::setw(20) << "Compressed Trie"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << compressedTrieInsertTime
              << std::setw(15) << std::fixed << std::setprecision(2) << compressedTrieSearchTime
              << std::setw(15) << std::fixed << std::setprecision(2) << compressedTriePrefixTime
              << std::setw(15) << compressedTrieNodeCount
              << std::endl;

    std::cout << std::left << std::setw(20) << "Ternary Search Trie"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << tstInsertTime
              << std::setw(15) << std::fixed << std::setprecision(2) << tstSearchTime
              << std::setw(15) << std::fixed << std::setprecision(2) << tstPrefixTime
              << std::setw(15) << tstNodeCount
              << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Benchmark autocomplete functionality
 *
 * @param wordCount Number of words in the dictionary
 * @param queryCount Number of queries to test
 */
void benchmarkAutocomplete(int wordCount = 10000, int queryCount = 1000)
{
    std::cout << "===== Autocomplete Benchmark =====" << std::endl;

    // Generate test data
    std::vector<std::string> dictionary = generateRandomStrings(wordCount, 4, 12);
    std::vector<std::string> queries = generateRandomStrings(queryCount, 2, 4);

    // Initialize autocomplete
    AutoComplete autocomplete(dictionary);

    // Time autocomplete operations
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &query : queries)
    {
        auto suggestions = autocomplete.getSuggestions(query, 10);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Dictionary size: " << wordCount << " words" << std::endl;
    std::cout << "Query count: " << queryCount << std::endl;
    std::cout << "Average time per query: " << std::fixed << std::setprecision(3)
              << duration.count() / queryCount << " ms" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Benchmark spell checker functionality
 *
 * @param wordCount Number of words in the dictionary
 * @param queryCount Number of queries to test
 */
void benchmarkSpellChecker(int wordCount = 10000, int queryCount = 1000)
{
    std::cout << "===== Spell Checker Benchmark =====" << std::endl;

    // Generate test data
    std::vector<std::string> dictionary = generateRandomStrings(wordCount, 4, 12);

    // Generate misspelled words by applying random edits
    std::vector<std::string> misspelledWords;
    misspelledWords.reserve(queryCount);

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> randomIndex(0, dictionary.size() - 1);
    std::uniform_int_distribution<int> randomEdit(0, 2);  // 0 = delete, 1 = insert, 2 = replace
    std::uniform_int_distribution<int> randomChar(0, 25); // a-z

    for (int i = 0; i < queryCount; i++)
    {
        std::string word = dictionary[randomIndex(gen)];
        if (word.length() < 2)
            continue;

        int position = std::uniform_int_distribution<int>(0, word.length() - 1)(gen);
        int edit = randomEdit(gen);

        if (edit == 0 && word.length() > 1)
        {
            // Delete a character
            word.erase(position, 1);
        }
        else if (edit == 1)
        {
            // Insert a character
            char c = 'a' + randomChar(gen);
            word.insert(position, 1, c);
        }
        else
        {
            // Replace a character
            char c = 'a' + randomChar(gen);
            word[position] = c;
        }

        misspelledWords.push_back(word);
    }

    // Initialize spell checker
    SpellChecker spellChecker(dictionary);

    // Time spell check operations
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &word : misspelledWords)
    {
        spellChecker.isCorrectlySpelled(word);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationCheck = end - start;

    // Time suggestion operations
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < std::min(100, (int)misspelledWords.size()); i++)
    {
        spellChecker.suggestCorrections(misspelledWords[i], 5);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationSuggest = end - start;

    std::cout << "Dictionary size: " << wordCount << " words" << std::endl;
    std::cout << "Check count: " << queryCount << std::endl;
    std::cout << "Suggestion count: " << std::min(100, (int)misspelledWords.size()) << std::endl;
    std::cout << "Average time per check: " << std::fixed << std::setprecision(3)
              << durationCheck.count() / queryCount << " ms" << std::endl;
    std::cout << "Average time per suggestion: " << std::fixed << std::setprecision(3)
              << durationSuggest.count() / std::min(100, (int)misspelledWords.size()) << " ms" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Benchmark boggle solver functionality
 *
 * @param dictionarySize Size of the dictionary
 * @param boardSize Size of the boggle board (boardSize x boardSize)
 */
void benchmarkBoggleSolver(int dictionarySize = 10000, int boardSize = 4)
{
    std::cout << "===== Boggle Solver Benchmark =====" << std::endl;

    // Generate a dictionary
    std::vector<std::string> dictionary = generateRandomStrings(dictionarySize, 3, 8);

    // Generate a random boggle board
    std::vector<std::vector<char>> board(boardSize, std::vector<char>(boardSize));
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> randomChar(0, 25); // a-z

    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = 'a' + randomChar(gen);
        }
    }

    // Initialize boggle solver
    BoggleSolver solver(dictionary);

    // Time the board solving
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> foundWords = solver.findWords(board);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Dictionary size: " << dictionarySize << " words" << std::endl;
    std::cout << "Board size: " << boardSize << "x" << boardSize << std::endl;
    std::cout << "Words found: " << foundWords.size() << std::endl;
    std::cout << "Time to solve board: " << std::fixed << std::setprecision(3)
              << duration.count() << " ms" << std::endl;
    std::cout << std::endl;
}

/****************************************************************************************
 * DEMO FUNCTIONS
 ****************************************************************************************/

/**
 * @brief Demonstrate basic trie operations
 */
void demoBasicTrie()
{
    std::cout << "===== Basic Trie Operations =====" << std::endl;

    Trie trie;

    // Insert some words
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    trie.insert("banana");
    trie.insert("ball");
    trie.insert("bat");

    std::cout << "Words in trie: " << trie.size() << std::endl;
    std::cout << "Nodes in trie: " << trie.nodeCount() << std::endl;

    // Search for words
    std::cout << "Search results:" << std::endl;
    std::cout << "  apple: " << (trie.search("apple") ? "Found" : "Not found") << std::endl;
    std::cout << "  app: " << (trie.search("app") ? "Found" : "Not found") << std::endl;
    std::cout << "  appl: " << (trie.search("appl") ? "Found" : "Not found") << std::endl;

    // Check prefixes
    std::cout << "Prefix checks:" << std::endl;
    std::cout << "  app: " << (trie.startsWith("app") ? "Valid prefix" : "Not a prefix") << std::endl;
    std::cout << "  ban: " << (trie.startsWith("ban") ? "Valid prefix" : "Not a prefix") << std::endl;
    std::cout << "  ca: " << (trie.startsWith("ca") ? "Valid prefix" : "Not a prefix") << std::endl;

    // Find words with prefix
    std::cout << "Words with prefix 'app':" << std::endl;
    std::vector<std::string> words = trie.findWordsWithPrefix("app");
    for (const auto &word : words)
    {
        std::cout << "  " << word << std::endl;
    }

    // Remove a word
    std::cout << "Removing 'app'..." << std::endl;
    trie.remove("app");

    std::cout << "Words in trie after removal: " << trie.size() << std::endl;
    std::cout << "Search for 'app': " << (trie.search("app") ? "Found" : "Not found") << std::endl;
    std::cout << "Search for 'apple': " << (trie.search("apple") ? "Found" : "Not found") << std::endl;

    std::cout << std::endl;
}

/**
 * @brief Demonstrate autocomplete functionality
 */
void demoAutocomplete()
{
    std::cout << "===== Autocomplete Demo =====" << std::endl;

    // Create a dictionary
    std::vector<std::string> dictionary = {
        "apple", "application", "apply", "appreciate", "approach",
        "banana", "band", "bandana", "bar", "bark", "bat", "batch",
        "computer", "computing", "compute", "companion", "company",
        "program", "programming", "programmer", "progress", "progressive"};

    // Initialize autocomplete
    AutoComplete autocomplete(dictionary);

    // Test some prefixes
    std::vector<std::string> prefixes = {"ap", "ba", "com", "pro"};

    for (const auto &prefix : prefixes)
    {
        std::cout << "Suggestions for '" << prefix << "':" << std::endl;

        std::vector<std::string> suggestions = autocomplete.getSuggestions(prefix);

        for (const auto &suggestion : suggestions)
        {
            std::cout << "  " << suggestion << std::endl;
        }

        std::cout << std::endl;
    }
}

/**
 * @brief Demonstrate spell checker functionality
 */
void demoSpellChecker()
{
    std::cout << "===== Spell Checker Demo =====" << std::endl;

    // Create a dictionary
    std::vector<std::string> dictionary = {
        "apple", "application", "apply", "appreciate", "approach",
        "banana", "band", "bandana", "bar", "bark", "bat", "batch",
        "computer", "computing", "compute", "companion", "company",
        "program", "programming", "programmer", "progress", "progressive"};

    // Initialize spell checker with word frequencies
    std::unordered_map<std::string, int> frequencies = {
        {"apple", 100}, {"application", 80}, {"apply", 90}, {"banana", 85}, {"band", 75}, {"computer", 95}, {"programming", 70}, {"program", 85}};

    SpellChecker spellChecker(dictionary, frequencies);

    // Test some correctly spelled words
    std::cout << "Checking correctly spelled words:" << std::endl;
    std::vector<std::string> correctWords = {"apple", "banana", "computer"};

    for (const auto &word : correctWords)
    {
        std::cout << "  " << word << ": " << (spellChecker.isCorrectlySpelled(word) ? "Correct" : "Incorrect") << std::endl;
    }

    // Test some misspelled words
    std::cout << "\nChecking misspelled words:" << std::endl;
    std::vector<std::string> misspelledWords = {"appel", "banan", "compter", "progrm"};

    for (const auto &word : misspelledWords)
    {
        std::cout << "  " << word << ": " << (spellChecker.isCorrectlySpelled(word) ? "Correct" : "Incorrect") << std::endl;

        std::cout << "    Suggestions: ";
        std::vector<std::string> suggestions = spellChecker.suggestCorrections(word);

        for (size_t i = 0; i < suggestions.size(); i++)
        {
            std::cout << suggestions[i];
            if (i < suggestions.size() - 1)
            {
                std::cout << ", ";
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Demonstrate boggle solver functionality
 */
void demoBoggleSolver()
{
    std::cout << "===== Boggle Solver Demo =====" << std::endl;

    // Create a dictionary
    std::vector<std::string> dictionary = {
        "ace", "ape", "cape", "clap", "clay", "gape", "grape",
        "lace", "lap", "lay", "pace", "pay", "rap", "ray", "tape", "trap"};

    // Initialize boggle solver
    BoggleSolver solver(dictionary);

    // Create a boggle board
    std::vector<std::vector<char>> board = {
        {'g', 'r', 'a', 'p'},
        {'c', 'a', 'p', 'e'},
        {'l', 'a', 'y', 't'},
        {'t', 'r', 'a', 'p'}};

    // Print the board
    std::cout << "Boggle Board:" << std::endl;
    for (const auto &row : board)
    {
        std::cout << "  ";
        for (char c : row)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    // Find all words on the board
    std::vector<std::string> foundWords = solver.findWords(board);

    // Print the results
    std::cout << "\nFound " << foundWords.size() << " words:" << std::endl;
    for (const auto &word : foundWords)
    {
        std::cout << "  " << word << std::endl;
    }

    // Check if specific words can be formed
    std::cout << "\nSpecific word checks:" << std::endl;
    std::vector<std::string> wordsToCheck = {"grape", "trap", "laptop", "pay", "cat"};

    for (const auto &word : wordsToCheck)
    {
        std::cout << "  " << word << ": " << (solver.canFormWord(board, word) ? "Can form" : "Cannot form") << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Demonstrate IP router functionality
 */
void demoIPRouter()
{
    std::cout << "===== IP Router Demo =====" << std::endl;

    IPRouter router;

    // Add some routes
    router.addRoute("192.168.0.0/16", "eth0", 10); // 192.168.x.x
    router.addRoute("10.0.0.0/8", "eth1", 20);     // 10.x.x.x
    router.addRoute("172.16.0.0/12", "eth2", 15);  // 172.16-31.x.x
    router.addRoute("192.168.1.0/24", "eth3", 30); // 192.168.1.x (higher priority)
    router.addRoute("0.0.0.0/0", "default", 0);    // Default route

    // Test some IP addresses
    std::vector<std::string> ips = {
        "192.168.1.10", // Should match 192.168.1.0/24 (eth3)
        "192.168.2.10", // Should match 192.168.0.0/16 (eth0)
        "10.1.2.3",     // Should match 10.0.0.0/8 (eth1)
        "172.18.5.6",   // Should match 172.16.0.0/12 (eth2)
        "8.8.8.8"       // Should match default route
    };

    for (const auto &ip : ips)
    {
        std::string interface = router.findBestRoute(ip);
        std::cout << "Best route for " << ip << ": " << interface << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Main function
 */
int main()
{
    std::cout << "======================================================" << std::endl;
    std::cout << "=== DAY 27: Trie Data Structure Implementations    ===" << std::endl;
    std::cout << "=== Part of #DSAin45 Course                        ===" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << std::endl;

    // Demo basic trie operations
    demoBasicTrie();

    // Demo trie applications
    demoAutocomplete();
    demoSpellChecker();
    demoBoggleSolver();
    demoIPRouter();

    // Run benchmarks (Comment these out if not needed)
    benchmarkTrieImplementations(5000, 3); // Reduced size for quicker execution
    benchmarkAutocomplete(5000, 500);
    benchmarkSpellChecker(5000, 500);
    benchmarkBoggleSolver(5000, 4);

    std::cout << "======================================================" << std::endl;
    std::cout << "=== End of DAY 27 Demonstrations                   ===" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}