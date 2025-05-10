/*
 * AdvancedProblems.cpp
 * Day 42 of #DSAin45 - Comprehensive Implementation of Advanced Algorithm Problems
 *
 * This file demonstrates solutions to complex problems that integrate
 * multiple data structures and algorithms, with benchmarks and analysis.
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
#include <bitset>
#include <optional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>

// Utility function to measure execution time
template <typename TimePoint>
std::chrono::microseconds getElapsedMicroseconds(TimePoint start, TimePoint end)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

// Utility function for string hashing
size_t hashString(const std::string &s, int seed = 131)
{
    size_t hash = 0;
    for (char c : s)
    {
        hash = hash * seed + c;
    }
    return hash;
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

//==============================================================================
// Challenge 1: Efficient File Indexing System
//==============================================================================

namespace FileIndexing
{

    // Document representation
    struct Document
    {
        int id;
        std::string name;
        std::string content;

        Document(int i, std::string n, std::string c)
            : id(i), name(std::move(n)), content(std::move(c)) {}
    };

    // Term position in a document
    struct TermPosition
    {
        int docId;
        int position;

        TermPosition(int d, int p) : docId(d), position(p) {}
    };

    // Index entry for a term
    struct IndexEntry
    {
        std::unordered_map<int, std::vector<int>> positions; // docId -> [positions]
        int totalFrequency;

        IndexEntry() : totalFrequency(0) {}
    };

    // File Indexing System
    class InvertedIndex
    {
    private:
        // Main inverted index: term -> {docId -> [positions]}
        std::unordered_map<std::string, IndexEntry> index;

        // Document metadata
        std::unordered_map<int, Document *> documents;

        // Collection statistics
        int totalDocuments;
        int totalTerms;

        // Tokenize text into terms
        std::vector<std::string> tokenize(const std::string &text)
        {
            std::vector<std::string> tokens;
            std::string token;

            // Simple tokenization by splitting on whitespace and punctuation
            for (char c : text)
            {
                if (isalnum(c))
                {
                    token += tolower(c);
                }
                else if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
            }

            if (!token.empty())
            {
                tokens.push_back(token);
            }

            return tokens;
        }

    public:
        InvertedIndex() : totalDocuments(0), totalTerms(0) {}

        ~InvertedIndex()
        {
            for (auto &[id, doc] : documents)
            {
                delete doc;
            }
        }

        // Add a document to the index
        void addDocument(int id, const std::string &name, const std::string &content)
        {
            // Check if document already exists
            if (documents.find(id) != documents.end())
            {
                // Update existing document
                removeDocument(id);
            }

            // Create new document
            documents[id] = new Document(id, name, content);
            totalDocuments++;

            // Tokenize content
            std::vector<std::string> terms = tokenize(content);

            // Add terms to index
            for (size_t pos = 0; pos < terms.size(); pos++)
            {
                const std::string &term = terms[pos];

                // Update index for this term
                index[term].positions[id].push_back(pos);
                index[term].totalFrequency++;

                totalTerms++;
            }
        }

        // Remove a document from the index
        void removeDocument(int id)
        {
            if (documents.find(id) == documents.end())
            {
                return; // Document not found
            }

            Document *doc = documents[id];

            // Remove document from index
            std::vector<std::string> terms = tokenize(doc->content);
            for (size_t pos = 0; pos < terms.size(); pos++)
            {
                const std::string &term = terms[pos];

                // Update index for this term
                if (index.find(term) != index.end())
                {
                    auto &entry = index[term];

                    if (entry.positions.find(id) != entry.positions.end())
                    {
                        // Count the positions for this term in this document
                        int termCount = entry.positions[id].size();

                        // Update total frequency
                        entry.totalFrequency -= termCount;

                        // Remove document from posting list
                        entry.positions.erase(id);

                        // If no more documents contain this term, remove the term
                        if (entry.positions.empty())
                        {
                            index.erase(term);
                        }

                        totalTerms -= termCount;
                    }
                }
            }

            // Clean up document
            delete doc;
            documents.erase(id);
            totalDocuments--;
        }

        // Search for a query and return ranked results
        std::vector<std::pair<int, double>> search(const std::string &query)
        {
            std::vector<std::string> queryTerms = tokenize(query);

            // Documents matching at least one term
            std::unordered_map<int, double> scores;

            // Calculate TF-IDF scores
            for (const std::string &term : queryTerms)
            {
                if (index.find(term) != index.end())
                {
                    // Calculate IDF (Inverse Document Frequency)
                    int docFreq = index[term].positions.size();
                    double idf = log10((double)totalDocuments / docFreq);

                    // Calculate score contribution for each document
                    for (const auto &[docId, positions] : index[term].positions)
                    {
                        // Term frequency in this document
                        double tf = (double)positions.size() / documents[docId]->content.length();

                        // Accumulate TF-IDF score
                        scores[docId] += tf * idf;
                    }
                }
            }

            // Convert to sortable vector
            std::vector<std::pair<int, double>> results;
            for (const auto &[docId, score] : scores)
            {
                results.push_back({docId, score});
            }

            // Sort by descending score
            std::sort(results.begin(), results.end(),
                      [](const std::pair<int, double> &a, const std::pair<int, double> &b)
                      {
                          return a.second > b.second;
                      });

            return results;
        }

        // Get document by ID
        const Document *getDocument(int id) const
        {
            auto it = documents.find(id);
            if (it != documents.end())
            {
                return it->second;
            }
            return nullptr;
        }

        // Get statistics about the index
        void printStatistics() const
        {
            std::cout << "Index Statistics:\n";
            std::cout << "  Total Documents: " << totalDocuments << "\n";
            std::cout << "  Total Terms: " << totalTerms << "\n";
            std::cout << "  Unique Terms: " << index.size() << "\n";

            // Calculate average document length
            double avgDocLength = 0;
            for (const auto &[id, doc] : documents)
            {
                avgDocLength += doc->content.length();
            }
            if (totalDocuments > 0)
            {
                avgDocLength /= totalDocuments;
            }
            std::cout << "  Average Document Length: " << avgDocLength << " characters\n";
        }
    };

    // Extended inverted index with phrase queries and wildcard support
    class AdvancedInvertedIndex : public InvertedIndex
    {
    private:
        // Trie node for prefix search
        struct TrieNode
        {
            std::unordered_map<char, TrieNode *> children;
            bool isEndOfWord;
            std::string term;

            TrieNode() : isEndOfWord(false) {}

            ~TrieNode()
            {
                for (auto &[c, child] : children)
                {
                    delete child;
                }
            }
        };

        // Trie for prefix search
        TrieNode *trieRoot;

        // Insert a term into the trie
        void insertIntoTrie(const std::string &term)
        {
            TrieNode *node = trieRoot;

            for (char c : term)
            {
                if (node->children.find(c) == node->children.end())
                {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }

            node->isEndOfWord = true;
            node->term = term;
        }

        // Search the trie for terms with a given prefix
        void searchTrie(TrieNode *node, std::vector<std::string> &result)
        {
            if (node->isEndOfWord)
            {
                result.push_back(node->term);
            }

            for (auto &[c, child] : node->children)
            {
                searchTrie(child, result);
            }
        }

    public:
        AdvancedInvertedIndex() : trieRoot(new TrieNode()) {}

        ~AdvancedInvertedIndex()
        {
            delete trieRoot;
        }

        // Add a document to the index
        void addDocument(int id, const std::string &name, const std::string &content)
        {
            // Call base class implementation
            InvertedIndex::addDocument(id, name, content);

            // Add terms to trie
            std::vector<std::string> terms = tokenize(content);
            for (const std::string &term : terms)
            {
                insertIntoTrie(term);
            }
        }

        // Search for terms with a given prefix
        std::vector<std::string> prefixSearch(const std::string &prefix)
        {
            std::vector<std::string> result;
            TrieNode *node = trieRoot;

            // Navigate to the node corresponding to the prefix
            for (char c : prefix)
            {
                if (node->children.find(c) == node->children.end())
                {
                    return result; // Prefix not found
                }
                node = node->children[c];
            }

            // Collect all terms with this prefix
            searchTrie(node, result);
            return result;
        }

        // Search for a phrase (exact sequence of terms)
        std::vector<std::pair<int, std::vector<int>>> phraseSearch(const std::string &phrase)
        {
            std::vector<std::string> terms = tokenize(phrase);
            std::vector<std::pair<int, std::vector<int>>> results;

            if (terms.empty())
            {
                return results;
            }

            // Get documents containing the first term
            if (index.find(terms[0]) == index.end())
            {
                return results; // First term not found
            }

            const auto &firstTermDocs = index[terms[0]].positions;

            // Check each document that contains the first term
            for (const auto &[docId, positions] : firstTermDocs)
            {
                std::vector<int> phrasePositions;

                // For each position of the first term
                for (int pos : positions)
                {
                    bool phraseFound = true;

                    // Check if the subsequent terms appear at the expected positions
                    for (size_t i = 1; i < terms.size(); i++)
                    {
                        if (index.find(terms[i]) == index.end() ||
                            index[terms[i]].positions.find(docId) == index[terms[i]].positions.end())
                        {
                            phraseFound = false;
                            break;
                        }

                        const auto &termPositions = index[terms[i]].positions.at(docId);

                        // Check if this term appears at the expected position
                        if (std::find(termPositions.begin(), termPositions.end(), pos + i) == termPositions.end())
                        {
                            phraseFound = false;
                            break;
                        }
                    }

                    if (phraseFound)
                    {
                        phrasePositions.push_back(pos);
                    }
                }

                if (!phrasePositions.empty())
                {
                    results.push_back({docId, phrasePositions});
                }
            }

            return results;
        }
    };

    // Demonstration of file indexing and search
    void runFileIndexingDemo()
    {
        std::cout << "\n=== FILE INDEXING SYSTEM DEMO ===\n";

        // Create an index
        std::cout << "Creating inverted index...\n";
        InvertedIndex index;

        // Add sample documents
        index.addDocument(1, "Introduction to Algorithms",
                          "Algorithms are step-by-step procedures for calculations. "
                          "Algorithm design is a specific method to create a mathematical process "
                          "in solving problems. Algorithms are widely used throughout all areas "
                          "of IT.");

        index.addDocument(2, "Data Structures Explained",
                          "Data structures are specialized formats for organizing and storing data. "
                          "General data structure types include arrays, files, linked lists, stacks, "
                          "queues, trees, graphs and more. Data structures are used in almost every "
                          "program or software system.");

        index.addDocument(3, "The Art of Programming",
                          "Programming is the process of creating a set of instructions that tell "
                          "a computer how to perform a task. Programming can be done using a variety "
                          "of computer programming languages, such as JavaScript, Python, and C++.");

        index.addDocument(4, "Algorithms and Data Structures",
                          "Algorithms and data structures form the core of computer science. "
                          "An algorithm is a sequence of steps designed to accomplish a specific task. "
                          "Data structures are specialized formats used to organize and store data.");

        // Print index statistics
        index.printStatistics();

        // Perform searches
        std::vector<std::string> queries = {
            "algorithms",
            "data structures",
            "programming languages",
            "computer science",
            "nonexistent term"};

        std::cout << "\nPerforming searches:\n";
        for (const std::string &query : queries)
        {
            std::cout << "\nSearch query: \"" << query << "\"\n";

            auto start = std::chrono::high_resolution_clock::now();
            auto results = index.search(query);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << "Found " << results.size() << " matching documents in "
                      << elapsed.count() << " microseconds\n";

            // Print top results
            for (size_t i = 0; i < std::min(results.size(), size_t(3)); i++)
            {
                int docId = results[i].first;
                double score = results[i].second;
                const Document *doc = index.getDocument(docId);

                std::cout << "  " << i + 1 << ". " << doc->name
                          << " (Score: " << std::fixed << std::setprecision(4) << score << ")\n";

                // Print snippet
                std::string content = doc->content;
                if (content.length() > 100)
                {
                    content = content.substr(0, 97) + "...";
                }
                std::cout << "     " << content << "\n";
            }
        }

        // Advanced index demo with phrase search
        std::cout << "\n=== ADVANCED FILE INDEXING DEMO ===\n";

        AdvancedInvertedIndex advIndex;

        // Add the same documents
        advIndex.addDocument(1, "Introduction to Algorithms",
                             "Algorithms are step-by-step procedures for calculations. "
                             "Algorithm design is a specific method to create a mathematical process "
                             "in solving problems. Algorithms are widely used throughout all areas "
                             "of IT.");

        advIndex.addDocument(2, "Data Structures Explained",
                             "Data structures are specialized formats for organizing and storing data. "
                             "General data structure types include arrays, files, linked lists, stacks, "
                             "queues, trees, graphs and more. Data structures are used in almost every "
                             "program or software system.");

        advIndex.addDocument(3, "The Art of Programming",
                             "Programming is the process of creating a set of instructions that tell "
                             "a computer how to perform a task. Programming can be done using a variety "
                             "of computer programming languages, such as JavaScript, Python, and C++.");

        advIndex.addDocument(4, "Algorithms and Data Structures",
                             "Algorithms and data structures form the core of computer science. "
                             "An algorithm is a sequence of steps designed to accomplish a specific task. "
                             "Data structures are specialized formats used to organize and store data.");

        // Perform phrase searches
        std::vector<std::string> phraseQueries = {
            "data structures",
            "computer science",
            "programming languages"};

        std::cout << "\nPerforming phrase searches:\n";
        for (const std::string &query : phraseQueries)
        {
            std::cout << "\nPhrase query: \"" << query << "\"\n";

            auto start = std::chrono::high_resolution_clock::now();
            auto results = advIndex.phraseSearch(query);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << "Found " << results.size() << " exact matches in "
                      << elapsed.count() << " microseconds\n";

            // Print results
            for (const auto &[docId, positions] : results)
            {
                const Document *doc = advIndex.getDocument(docId);

                std::cout << "  Document: " << doc->name << "\n";
                std::cout << "  Positions: ";
                for (size_t i = 0; i < std::min(positions.size(), size_t(5)); i++)
                {
                    std::cout << positions[i];
                    if (i < std::min(positions.size(), size_t(5)) - 1)
                    {
                        std::cout << ", ";
                    }
                }
                if (positions.size() > 5)
                {
                    std::cout << ", ...";
                }
                std::cout << "\n";
            }
        }

        // Prefix search demo
        std::vector<std::string> prefixes = {
            "algo",
            "pro",
            "data"};

        std::cout << "\nPerforming prefix searches:\n";
        for (const std::string &prefix : prefixes)
        {
            std::cout << "\nPrefix query: \"" << prefix << "\"\n";

            auto start = std::chrono::high_resolution_clock::now();
            auto results = advIndex.prefixSearch(prefix);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed = getElapsedMicroseconds(start, end);

            std::cout << "Found " << results.size() << " matching terms in "
                      << elapsed.count() << " microseconds\n";

            // Print results
            std::cout << "  Terms: ";
            for (size_t i = 0; i < std::min(results.size(), size_t(10)); i++)
            {
                std::cout << results[i];
                if (i < std::min(results.size(), size_t(10)) - 1)
                {
                    std::cout << ", ";
                }
            }
            if (results.size() > 10)
            {
                std::cout << ", ...";
            }
            std::cout << "\n";
        }
    }

    // Benchmark indexing and search performance with varying collection sizes
    void runFileIndexingBenchmark()
    {
        std::cout << "\n=== FILE INDEXING PERFORMANCE BENCHMARK ===\n";

        // Generate random document collection
        std::cout << "Generating document collection...\n";

        // Different collection sizes
        std::vector<int> collectionSizes = {100, 1000, 10000};

        // Document length
        const int documentLength = 1000;

        for (int size : collectionSizes)
        {
            std::cout << "\nTesting with " << size << " documents of "
                      << documentLength << " characters each\n";

            std::vector<Document> documents;
            documents.reserve(size);

            for (int i = 0; i < size; i++)
            {
                std::string content = generateRandomString(documentLength);
                documents.emplace_back(i, "Document_" + std::to_string(i), content);
            }

            // Benchmark indexing
            std::cout << "Benchmarking indexing...\n";
            InvertedIndex index;

            auto startIndex = std::chrono::high_resolution_clock::now();

            for (const auto &doc : documents)
            {
                index.addDocument(doc.id, doc.name, doc.content);
            }

            auto endIndex = std::chrono::high_resolution_clock::now();
            auto indexTime = getElapsedMicroseconds(startIndex, endIndex);

            std::cout << "  Indexed " << size << " documents in "
                      << indexTime.count() << " microseconds "
                      << "(" << indexTime.count() / size << " μs per document)\n";

            // Benchmark search
            std::cout << "Benchmarking search...\n";

            // Create random search queries
            const int numQueries = 100;
            std::vector<std::string> queries;
            queries.reserve(numQueries);

            for (int i = 0; i < numQueries; i++)
            {
                int docIndex = rand() % size;
                const std::string &content = documents[docIndex].content;

                // Extract a random 3-word sequence from the document
                std::vector<std::string> terms;
                std::string term;
                int wordCount = 0;
                int startPos = rand() % (content.length() / 2);

                for (size_t j = startPos; j < content.length() && wordCount < 3; j++)
                {
                    if (isalnum(content[j]))
                    {
                        term += content[j];
                    }
                    else if (!term.empty())
                    {
                        terms.push_back(term);
                        term.clear();
                        wordCount++;
                    }
                }

                if (!term.empty() && wordCount < 3)
                {
                    terms.push_back(term);
                }

                // Combine terms into a query
                std::string query;
                for (const std::string &t : terms)
                {
                    if (!query.empty())
                    {
                        query += " ";
                    }
                    query += t;
                }

                if (!query.empty())
                {
                    queries.push_back(query);
                }
            }

            // Benchmark search performance
            auto startSearch = std::chrono::high_resolution_clock::now();

            int totalResults = 0;
            for (const std::string &query : queries)
            {
                auto results = index.search(query);
                totalResults += results.size();
            }

            auto endSearch = std::chrono::high_resolution_clock::now();
            auto searchTime = getElapsedMicroseconds(startSearch, endSearch);

            std::cout << "  Executed " << queries.size() << " queries in "
                      << searchTime.count() << " microseconds "
                      << "(" << searchTime.count() / queries.size() << " μs per query)\n";
            std::cout << "  Found a total of " << totalResults << " results\n";
        }
    }
}

//==============================================================================
// Challenge 2: Route Optimization with Constraints
//==============================================================================

namespace RouteOptimization
{

    // Location with time window constraints
    struct Location
    {
        int id;
        double x, y;
        int packageWeight;
        int earliestArrival; // Time window start
        int latestArrival;   // Time window end
        int serviceTime;     // Time required to serve this location

        Location(int id, double x, double y, int weight, int early, int late, int service)
            : id(id), x(x), y(y), packageWeight(weight),
              earliestArrival(early), latestArrival(late), serviceTime(service) {}
    };

    // Route representation
    struct Route
    {
        std::vector<int> locationOrder;
        double totalDistance;
        int totalTime;
        int totalLoad;
        bool feasible;

        Route() : totalDistance(0), totalTime(0), totalLoad(0), feasible(false) {}
    };

    // Calculate Euclidean distance between two locations
    double distance(const Location &a, const Location &b)
    {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    // Calculate total distance of a route
    double calculateTotalDistance(const std::vector<Location> &locations, const std::vector<int> &route)
    {
        double total = 0;
        int prevLoc = 0; // Start at depot (location 0)

        for (int locId : route)
        {
            total += distance(locations[prevLoc], locations[locId]);
            prevLoc = locId;
        }

        // Return to depot
        total += distance(locations[prevLoc], locations[0]);

        return total;
    }

    // Check if a route is feasible given time windows and capacity
    bool isFeasible(const std::vector<Location> &locations, const std::vector<int> &route,
                    int vehicleCapacity)
    {
        int currentTime = 0;
        int currentLoad = 0;
        int prevLocation = 0; // Start at depot

        for (int locId : route)
        {
            const Location &loc = locations[locId];

            // Add travel time (assume 1 time unit per distance unit)
            currentTime += static_cast<int>(distance(locations[prevLocation], loc));

            // Check if we arrive too late
            if (currentTime > loc.latestArrival)
            {
                return false;
            }

            // Wait if we arrive too early
            currentTime = std::max(currentTime, loc.earliestArrival);

            // Add package to vehicle
            currentLoad += loc.packageWeight;

            // Check capacity constraint
            if (currentLoad > vehicleCapacity)
            {
                return false;
            }

            // Service time at location
            currentTime += loc.serviceTime;
            prevLocation = locId;
        }

        // Check return to depot
        currentTime += static_cast<int>(distance(locations[prevLocation], locations[0]));

        // Assume depot has no time window for return
        return true;
    }

    // Evaluate a route's feasibility and metrics
    Route evaluateRoute(const std::vector<Location> &locations, const std::vector<int> &route,
                        int vehicleCapacity)
    {
        Route result;
        result.locationOrder = route;

        // Calculate distance
        result.totalDistance = calculateTotalDistance(locations, route);

        // Calculate time and check feasibility
        int currentTime = 0;
        int currentLoad = 0;
        int prevLocation = 0; // Start at depot

        for (int locId : route)
        {
            const Location &loc = locations[locId];

            // Add travel time
            int travelTime = static_cast<int>(distance(locations[prevLocation], loc));
            currentTime += travelTime;

            // Wait if we arrive too early
            int waitTime = std::max(0, loc.earliestArrival - currentTime);
            currentTime += waitTime;

            // Check if we arrive too late
            if (currentTime > loc.latestArrival)
            {
                result.feasible = false;
                return result;
            }

            // Add package to vehicle
            currentLoad += loc.packageWeight;

            // Check capacity constraint
            if (currentLoad > vehicleCapacity)
            {
                result.feasible = false;
                return result;
            }

            // Service time at location
            currentTime += loc.serviceTime;
            prevLocation = locId;
        }

        // Return to depot
        currentTime += static_cast<int>(distance(locations[prevLocation], locations[0]));

        result.totalTime = currentTime;
        result.totalLoad = currentLoad;
        result.feasible = true;

        return result;
    }

    // Greedy initial solution
    std::vector<int> createInitialSolution(const std::vector<Location> &locations, int vehicleCapacity)
    {
        std::vector<int> route;
        std::vector<bool> visited(locations.size(), false);

        // Start at depot (location 0)
        int currentLocation = 0;
        visited[0] = true; // Mark depot as visited

        // Visit locations until all are visited
        int locationsToVisit = locations.size() - 1; // Excluding depot
        int currentLoad = 0;

        while (locationsToVisit > 0)
        {
            int bestLocation = -1;
            double bestScore = std::numeric_limits<double>::max();

            // Find the nearest unvisited location
            for (size_t i = 1; i < locations.size(); i++)
            {
                if (!visited[i])
                {
                    // Check capacity constraint
                    if (currentLoad + locations[i].packageWeight <= vehicleCapacity)
                    {
                        double dist = distance(locations[currentLocation], locations[i]);

                        // Score considers distance and time window urgency
                        double timeWindowUrgency = 1.0 / (locations[i].latestArrival - locations[i].earliestArrival + 1);
                        double score = dist * (1 + timeWindowUrgency);

                        if (score < bestScore)
                        {
                            bestScore = score;
                            bestLocation = i;
                        }
                    }
                }
            }

            // If no location can be added, break
            if (bestLocation == -1)
            {
                break;
            }

            // Add the best location to the route
            route.push_back(bestLocation);
            visited[bestLocation] = true;
            currentLocation = bestLocation;
            currentLoad += locations[bestLocation].packageWeight;
            locationsToVisit--;
        }

        return route;
    }

    // Simulated Annealing optimization
    Route optimizeRoute(const std::vector<Location> &locations, int vehicleCapacity, int iterations = 10000)
    {
        int n = locations.size() - 1; // Excluding depot

        // Generate initial solution
        std::vector<int> currentRoute = createInitialSolution(locations, vehicleCapacity);

        // Evaluate initial solution
        Route best = evaluateRoute(locations, currentRoute, vehicleCapacity);

        // If initial solution is not feasible, try random solutions
        if (!best.feasible)
        {
            // Create multiple random solutions and choose the best
            for (int attempt = 0; attempt < 100; attempt++)
            {
                std::vector<int> randomRoute;
                for (int i = 1; i <= n; i++)
                {
                    randomRoute.push_back(i);
                }
                std::random_shuffle(randomRoute.begin(), randomRoute.end());

                Route randomSolution = evaluateRoute(locations, randomRoute, vehicleCapacity);
                if (randomSolution.feasible &&
                    (!best.feasible || randomSolution.totalDistance < best.totalDistance))
                {
                    best = randomSolution;
                }
            }

            // If still no feasible solution, use the initial greedy solution
            if (!best.feasible)
            {
                best = evaluateRoute(locations, currentRoute, vehicleCapacity);
            }
        }

        Route current = best;

        // Simulated annealing parameters
        double temperature = 1000.0;
        double coolingRate = 0.995;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int iter = 0; iter < iterations; iter++)
        {
            // Create a neighbor by applying a random move
            std::vector<int> newRoute = current.locationOrder;

            // Choose a random move type: swap, insert, or reverse
            int moveType = rand() % 3;

            if (moveType == 0 && newRoute.size() >= 2)
            {
                // Swap move: swap two locations
                int i = rand() % newRoute.size();
                int j = rand() % newRoute.size();
                std::swap(newRoute[i], newRoute[j]);
            }
            else if (moveType == 1 && newRoute.size() >= 2)
            {
                // Insert move: remove a location and insert it elsewhere
                int i = rand() % newRoute.size();
                int j = rand() % newRoute.size();

                if (i != j)
                {
                    int value = newRoute[i];
                    newRoute.erase(newRoute.begin() + i);

                    if (j > i)
                        j--; // Adjust for removal

                    newRoute.insert(newRoute.begin() + j, value);
                }
            }
            else if (moveType == 2 && newRoute.size() >= 2)
            {
                // Reverse move: reverse a segment of the route
                int i = rand() % newRoute.size();
                int j = rand() % newRoute.size();

                if (i > j)
                    std::swap(i, j);

                std::reverse(newRoute.begin() + i, newRoute.begin() + j + 1);
            }

            // Evaluate new route
            Route newSolution = evaluateRoute(locations, newRoute, vehicleCapacity);

            // Decide whether to accept new solution
            bool accept = false;

            if (newSolution.feasible && (!current.feasible || newSolution.totalDistance < current.totalDistance))
            {
                // Always accept better feasible solutions
                accept = true;
            }
            else if (newSolution.feasible == current.feasible)
            {
                // Accept worse solutions with probability based on temperature
                double acceptProbability = exp((current.totalDistance - newSolution.totalDistance) / temperature);
                if (dis(gen) < acceptProbability)
                {
                    accept = true;
                }
            }

            if (accept)
            {
                current = newSolution;

                // Update best solution
                if (current.feasible &&
                    (!best.feasible || current.totalDistance < best.totalDistance))
                {
                    best = current;
                }
            }

            // Cool down
            temperature *= coolingRate;
        }

        return best;
    }

    // Generate a random problem instance
    std::vector<Location> generateRandomProblem(int numLocations, double areaSize = 100.0,
                                                int maxWeight = 10, int timeWindowWidth = 30)
    {
        std::vector<Location> locations;
        locations.reserve(numLocations + 1); // +1 for depot

        // Random number generators
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> disPos(0.0, areaSize);
        std::uniform_int_distribution<> disWeight(1, maxWeight);
        std::uniform_int_distribution<> disServiceTime(5, 15);

        // Add depot at center
        locations.emplace_back(0, areaSize / 2, areaSize / 2, 0, 0, 480, 0); // 8-hour day (0-480 minutes)

        // Add other locations
        for (int i = 1; i <= numLocations; i++)
        {
            double x = disPos(gen);
            double y = disPos(gen);
            int weight = disWeight(gen);
            int serviceTime = disServiceTime(gen);

            // Generate time window
            int earliestTime = std::uniform_int_distribution<>(0, 480 - timeWindowWidth)(gen);
            int latestTime = earliestTime + timeWindowWidth;

            locations.emplace_back(i, x, y, weight, earliestTime, latestTime, serviceTime);
        }

        return locations;
    }

    // Visualize a route (text-based)
    void visualizeRoute(const std::vector<Location> &locations, const Route &route)
    {
        if (route.locationOrder.empty())
        {
            std::cout << "Empty route\n";
            return;
        }

        std::cout << "Route: 0"; // Start at depot
        for (int locId : route.locationOrder)
        {
            std::cout << " -> " << locId;
        }
        std::cout << " -> 0\n"; // Return to depot

        std::cout << "Total distance: " << std::fixed << std::setprecision(2)
                  << route.totalDistance << "\n";
        std::cout << "Total time: " << route.totalTime << " minutes\n";
        std::cout << "Total load: " << route.totalLoad << "\n";
        std::cout << "Feasible: " << (route.feasible ? "Yes" : "No") << "\n";

        // Print detailed schedule
        std::cout << "\nDetailed Schedule:\n";
        std::cout << "Loc\tArrive\tWait\tStart\tFinish\tTravel\n";

        int currentTime = 0;
        int prevLocation = 0; // Start at depot

        for (int locId : route.locationOrder)
        {
            const Location &loc = locations[locId];

            // Travel time
            int travelTime = static_cast<int>(distance(locations[prevLocation], loc));
            currentTime += travelTime;

            // Wait time
            int waitTime = std::max(0, loc.earliestArrival - currentTime);
            int arriveTime = currentTime;
            currentTime += waitTime;

            // Service time
            int startTime = currentTime;
            currentTime += loc.serviceTime;
            int finishTime = currentTime;

            std::cout << locId << "\t"
                      << arriveTime << "\t"
                      << waitTime << "\t"
                      << startTime << "\t"
                      << finishTime << "\t"
                      << travelTime << "\n";

            prevLocation = locId;
        }

        // Return to depot
        int finalTravelTime = static_cast<int>(distance(locations[prevLocation], locations[0]));
        currentTime += finalTravelTime;

        std::cout << "0\t"
                  << currentTime << "\t"
                  << "0\t"
                  << currentTime << "\t"
                  << currentTime << "\t"
                  << finalTravelTime << "\n";
    }

    // Run route optimization demo
    void runRouteOptimizationDemo()
    {
        std::cout << "\n=== ROUTE OPTIMIZATION DEMO ===\n";

        int numLocations = 20;
        int vehicleCapacity = 30;

        std::cout << "Generating random problem with " << numLocations << " locations...\n";
        std::vector<Location> locations = generateRandomProblem(numLocations);

        std::cout << "Locations generated. Vehicle capacity: " << vehicleCapacity << "\n\n";

        // Create initial solution
        std::cout << "Creating initial greedy solution...\n";
        std::vector<int> initialRoute = createInitialSolution(locations, vehicleCapacity);
        Route initialSolution = evaluateRoute(locations, initialRoute, vehicleCapacity);

        std::cout << "Initial solution:\n";
        visualizeRoute(locations, initialSolution);

        // Optimize with simulated annealing
        std::cout << "\nOptimizing with simulated annealing...\n";
        auto startTime = std::chrono::high_resolution_clock::now();
        Route optimizedSolution = optimizeRoute(locations, vehicleCapacity);
        auto endTime = std::chrono::high_resolution_clock::now();

        auto elapsed = getElapsedMicroseconds(startTime, endTime);

        std::cout << "Optimization completed in " << elapsed.count() / 1000.0
                  << " milliseconds\n\n";

        std::cout << "Optimized solution:\n";
        visualizeRoute(locations, optimizedSolution);

        // Calculate improvement
        double improvement = 0;
        if (initialSolution.feasible && optimizedSolution.feasible)
        {
            improvement = (initialSolution.totalDistance - optimizedSolution.totalDistance) /
                          initialSolution.totalDistance * 100;
        }
        else if (!initialSolution.feasible && optimizedSolution.feasible)
        {
            improvement = 100.0; // Found feasible solution
        }

        std::cout << "\nImprovement: " << std::fixed << std::setprecision(2)
                  << improvement << "%\n";
    }

    // Run route optimization benchmark with different problem sizes
    void runRouteOptimizationBenchmark()
    {
        std::cout << "\n=== ROUTE OPTIMIZATION BENCHMARK ===\n";

        std::vector<int> problemSizes = {10, 20, 50, 100};
        int vehicleCapacity = 50;
        int numIterations = 5000;

        for (int size : problemSizes)
        {
            std::cout << "\nProblem size: " << size << " locations\n";

            // Generate problem
            std::vector<Location> locations = generateRandomProblem(size);

            // Measure initial solution time
            auto startInitial = std::chrono::high_resolution_clock::now();
            std::vector<int> initialRoute = createInitialSolution(locations, vehicleCapacity);
            Route initialSolution = evaluateRoute(locations, initialRoute, vehicleCapacity);
            auto endInitial = std::chrono::high_resolution_clock::now();

            auto initialTime = getElapsedMicroseconds(startInitial, endInitial);

            std::cout << "Initial solution time: " << initialTime.count() / 1000.0
                      << " milliseconds\n";

            // Measure optimization time
            auto startOptimize = std::chrono::high_resolution_clock::now();
            Route optimizedSolution = optimizeRoute(locations, vehicleCapacity, numIterations);
            auto endOptimize = std::chrono::high_resolution_clock::now();

            auto optimizeTime = getElapsedMicroseconds(startOptimize, endOptimize);

            std::cout << "Optimization time: " << optimizeTime.count() / 1000.0
                      << " milliseconds\n";

            // Calculate improvement
            double improvement = 0;
            if (initialSolution.feasible && optimizedSolution.feasible)
            {
                improvement = (initialSolution.totalDistance - optimizedSolution.totalDistance) /
                              initialSolution.totalDistance * 100;
            }
            else if (!initialSolution.feasible && optimizedSolution.feasible)
            {
                improvement = 100.0; // Found feasible solution
            }

            std::cout << "Improvement: " << std::fixed << std::setprecision(2)
                      << improvement << "%\n";

            std::cout << "Initial solution - Distance: " << initialSolution.totalDistance
                      << ", Feasible: " << (initialSolution.feasible ? "Yes" : "No") << "\n";

            std::cout << "Optimized solution - Distance: " << optimizedSolution.totalDistance
                      << ", Feasible: " << (optimizedSolution.feasible ? "Yes" : "No") << "\n";
        }
    }
}

//==============================================================================
// Challenge 3: Real-Time Network Traffic Analysis
//==============================================================================

namespace NetworkTrafficAnalysis
{

    // Network packet representation
    struct NetworkPacket
    {
        std::string sourceIP;
        int sourcePort;
        std::string destIP;
        int destPort;
        int protocol;  // TCP=6, UDP=17, etc.
        int size;      // Packet size in bytes
        int timestamp; // Timestamp in seconds

        NetworkPacket(std::string src, int srcPort, std::string dst, int dstPort,
                      int proto, int sz, int time)
            : sourceIP(std::move(src)), sourcePort(srcPort), destIP(std::move(dst)),
              destPort(dstPort), protocol(proto), size(sz), timestamp(time) {}
    };

    // Count-Min Sketch for approximate frequency counting
    class CountMinSketch
    {
    private:
        std::vector<std::vector<int>> sketch;
        std::vector<int> hashParams;
        int width;
        int depth;

        // Hash function using multiplication method
        size_t hash(const std::string &item, int param) const
        {
            size_t result = 0;
            for (char c : item)
            {
                result = result * param + c;
            }
            return result % width;
        }

    public:
        CountMinSketch(int w, int d) : width(w), depth(d)
        {
            sketch.resize(depth, std::vector<int>(width, 0));
            hashParams.resize(depth);

            // Initialize hash function parameters
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 999983); // Large prime

            for (int i = 0; i < depth; i++)
            {
                hashParams[i] = dis(gen);
            }
        }

        // Increment the count for an item
        void increment(const std::string &item, int count = 1)
        {
            for (int i = 0; i < depth; i++)
            {
                size_t h = hash(item, hashParams[i]);
                sketch[i][h] += count;
            }
        }

        // Estimate the frequency of an item
        int estimate(const std::string &item) const
        {
            int minCount = std::numeric_limits<int>::max();

            for (int i = 0; i < depth; i++)
            {
                size_t h = hash(item, hashParams[i]);
                minCount = std::min(minCount, sketch[i][h]);
            }

            return minCount;
        }

        // Clear the sketch
        void clear()
        {
            for (auto &row : sketch)
            {
                std::fill(row.begin(), row.end(), 0);
            }
        }
    };

    // Bloom filter for approximate set membership
    class BloomFilter
    {
    private:
        std::vector<bool> filter;
        std::vector<int> hashParams;
        int size;
        int numHashes;

        // Hash function
        size_t hash(const std::string &item, int param) const
        {
            size_t result = 0;
            for (char c : item)
            {
                result = result * param + c;
            }
            return result % size;
        }

    public:
        BloomFilter(int s, int k) : size(s), numHashes(k)
        {
            filter.resize(size, false);
            hashParams.resize(numHashes);

            // Initialize hash function parameters
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 999983); // Large prime

            for (int i = 0; i < numHashes; i++)
            {
                hashParams[i] = dis(gen);
            }
        }

        // Add an item to the filter
        void add(const std::string &item)
        {
            for (int i = 0; i < numHashes; i++)
            {
                size_t h = hash(item, hashParams[i]);
                filter[h] = true;
            }
        }

        // Check if an item might be in the set
        bool mightContain(const std::string &item) const
        {
            for (int i = 0; i < numHashes; i++)
            {
                size_t h = hash(item, hashParams[i]);
                if (!filter[h])
                {
                    return false; // Definitely not in the set
                }
            }

            return true; // Might be in the set
        }

        // Clear the filter
        void clear()
        {
            std::fill(filter.begin(), filter.end(), false);
        }
    };

    // Network traffic analyzer
    class TrafficAnalyzer
    {
    private:
        // Count-Min Sketch for IP frequency counting
        CountMinSketch ipSketch;

        // Count-Min Sketch for connection frequency counting
        CountMinSketch connSketch;

        // Bloom filter for suspicious IPs
        BloomFilter suspiciousIPs;

        // Sliding window for recent packets
        std::deque<NetworkPacket> recentPackets;

        // Anomaly detection thresholds
        int packetRateThreshold;
        int connectionRateThreshold;
        int volumeThreshold;

        // Traffic statistics
        std::atomic<long> totalPackets;
        std::atomic<long> totalBytes;
        std::atomic<int> alertCount;

        // Time window in seconds
        int timeWindow;

        // Create connection string
        std::string getConnectionString(const NetworkPacket &packet) const
        {
            return packet.sourceIP + ":" + std::to_string(packet.sourcePort) + "->" +
                   packet.destIP + ":" + std::to_string(packet.destPort);
        }

        // Mutex for thread safety
        mutable std::shared_mutex mutex;

    public:
        TrafficAnalyzer(int sketchWidth = 1024, int sketchDepth = 5,
                        int bloomSize = 10000, int bloomHashes = 5,
                        int window = 60)
            : ipSketch(sketchWidth, sketchDepth),
              connSketch(sketchWidth, sketchDepth),
              suspiciousIPs(bloomSize, bloomHashes),
              packetRateThreshold(1000),
              connectionRateThreshold(100),
              volumeThreshold(1000000), // 1MB
              totalPackets(0),
              totalBytes(0),
              alertCount(0),
              timeWindow(window)
        {
        }

        // Process a network packet
        void processPacket(const NetworkPacket &packet)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            // Add to sliding window
            recentPackets.push_back(packet);

            // Remove old packets (maintain time window)
            while (!recentPackets.empty() &&
                   recentPackets.front().timestamp + timeWindow < packet.timestamp)
            {
                recentPackets.pop_front();
            }

            // Update statistics
            totalPackets++;
            totalBytes += packet.size;

            // Update IP frequencies
            ipSketch.increment(packet.sourceIP);
            ipSketch.increment(packet.destIP);

            // Update connection frequencies
            std::string connection = getConnectionString(packet);
            connSketch.increment(connection);

            // Check for high packet rate
            int srcIPFreq = ipSketch.estimate(packet.sourceIP);
            if (srcIPFreq > packetRateThreshold)
            {
                suspiciousIPs.add(packet.sourceIP);
                triggerAlert("High packet rate", packet);
            }

            // Check for high connection rate
            int connFreq = connSketch.estimate(connection);
            if (connFreq > connectionRateThreshold)
            {
                suspiciousIPs.add(packet.sourceIP);
                triggerAlert("High connection rate", packet);
            }

            // Check for high volume
            int volumeInWindow = calculateVolumeInWindow(packet.sourceIP);
            if (volumeInWindow > volumeThreshold)
            {
                suspiciousIPs.add(packet.sourceIP);
                triggerAlert("High traffic volume", packet);
            }
        }

        // Calculate total traffic volume from an IP in the current window
        int calculateVolumeInWindow(const std::string &ip) const
        {
            int volume = 0;

            for (const auto &packet : recentPackets)
            {
                if (packet.sourceIP == ip)
                {
                    volume += packet.size;
                }
            }

            return volume;
        }

        // Check if an IP is suspicious
        bool isSuspiciousIP(const std::string &ip) const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            return suspiciousIPs.mightContain(ip);
        }

        // Trigger an alert
        void triggerAlert(const std::string &reason, const NetworkPacket &packet)
        {
            alertCount++;

            std::cout << "ALERT #" << alertCount << ": " << reason << "\n";
            std::cout << "  Source IP: " << packet.sourceIP << ":" << packet.sourcePort << "\n";
            std::cout << "  Destination: " << packet.destIP << ":" << packet.destPort << "\n";
            std::cout << "  Protocol: " << packet.protocol << "\n";
            std::cout << "  Timestamp: " << packet.timestamp << "\n";
            std::cout << "  Packet Size: " << packet.size << " bytes\n";

            // Print estimated frequencies
            std::cout << "  Estimated packet count from this IP: "
                      << ipSketch.estimate(packet.sourceIP) << "\n";

            std::string connection = getConnectionString(packet);
            std::cout << "  Estimated connection count: "
                      << connSketch.estimate(connection) << "\n";

            std::cout << "  Traffic volume from this IP in window: "
                      << calculateVolumeInWindow(packet.sourceIP) << " bytes\n";
        }

        // Get traffic statistics
        void getStatistics() const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::cout << "Traffic Statistics:\n";
            std::cout << "  Total Packets: " << totalPackets << "\n";
            std::cout << "  Total Bytes: " << totalBytes << "\n";
            std::cout << "  Alert Count: " << alertCount << "\n";
            std::cout << "  Current Window Size: " << recentPackets.size() << " packets\n";

            // Calculate packets per second
            if (!recentPackets.empty())
            {
                int windowDuration = recentPackets.back().timestamp - recentPackets.front().timestamp;
                if (windowDuration > 0)
                {
                    double packetsPerSecond = static_cast<double>(recentPackets.size()) / windowDuration;
                    std::cout << "  Packet Rate: " << std::fixed << std::setprecision(2)
                              << packetsPerSecond << " packets/second\n";
                }
            }
        }

        // Clear all data structures
        void clear()
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            ipSketch.clear();
            connSketch.clear();
            suspiciousIPs.clear();
            recentPackets.clear();
            totalPackets = 0;
            totalBytes = 0;
            alertCount = 0;
        }
    };

    // Generate random IP address
    std::string generateRandomIP()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 254);

        return std::to_string(dis(gen)) + "." +
               std::to_string(dis(gen)) + "." +
               std::to_string(dis(gen)) + "." +
               std::to_string(dis(gen));
    }

    // Generate random port number
    int generateRandomPort()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1024, 65535);

        return dis(gen);
    }

    // Generate realistic network traffic
    std::vector<NetworkPacket> generateNetworkTraffic(int numPackets, int duration = 300)
    {
        std::vector<NetworkPacket> packets;
        packets.reserve(numPackets);

        std::random_device rd;
        std::mt19937 gen(rd());

        // Packet size distribution
        std::discrete_distribution<> packetSizeDist({1, 5, 10, 20, 10, 5, 1});
        std::vector<int> packetSizes = {64, 128, 256, 512, 1024, 1460, 8192};

        // Protocol distribution (TCP=6, UDP=17)
        std::discrete_distribution<> protocolDist({80, 20});
        std::vector<int> protocols = {6, 17};

        // Generate some "normal" IPs
        std::vector<std::string> normalIPs;
        for (int i = 0; i < 20; i++)
        {
            normalIPs.push_back(generateRandomIP());
        }

        // Generate some "server" IPs
        std::vector<std::string> serverIPs;
        for (int i = 0; i < 5; i++)
        {
            serverIPs.push_back(generateRandomIP());
        }

        // Generate some "attacker" IPs
        std::vector<std::string> attackerIPs;
        for (int i = 0; i < 3; i++)
        {
            attackerIPs.push_back(generateRandomIP());
        }

        // Timestamp distribution
        std::uniform_int_distribution<> timestampDist(0, duration);

        // Normal traffic
        for (int i = 0; i < numPackets * 0.8; i++)
        {
            std::string srcIP = normalIPs[std::uniform_int_distribution<>(0, normalIPs.size() - 1)(gen)];
            std::string dstIP = serverIPs[std::uniform_int_distribution<>(0, serverIPs.size() - 1)(gen)];

            int srcPort = generateRandomPort();
            int dstPort = std::uniform_int_distribution<>(0, 10)(gen) < 8 ? 80 : 443; // HTTP or HTTPS

            int protocol = protocols[protocolDist(gen)];
            int size = packetSizes[packetSizeDist(gen)];
            int timestamp = timestampDist(gen);

            packets.emplace_back(srcIP, srcPort, dstIP, dstPort, protocol, size, timestamp);
        }

        // Attack traffic - port scan
        for (int i = 0; i < numPackets * 0.1; i++)
        {
            std::string srcIP = attackerIPs[0]; // First attacker
            std::string dstIP = serverIPs[std::uniform_int_distribution<>(0, serverIPs.size() - 1)(gen)];

            int srcPort = generateRandomPort();
            int dstPort = i % 1000 + 1; // Sequential port scan

            int protocol = 6; // TCP
            int size = 64;    // Small packets
            int timestamp = timestampDist(gen);

            packets.emplace_back(srcIP, srcPort, dstIP, dstPort, protocol, size, timestamp);
        }

        // Attack traffic - DDoS
        for (int i = 0; i < numPackets * 0.1; i++)
        {
            std::string srcIP = attackerIPs[1]; // Second attacker
            std::string dstIP = serverIPs[0];   // Target the first server

            int srcPort = generateRandomPort();
            int dstPort = 80; // Attack web server

            int protocol = 17;                        // UDP
            int size = 1024;                          // Medium packets
            int timestamp = duration - 60 + (i % 60); // Last minute of capture

            packets.emplace_back(srcIP, srcPort, dstIP, dstPort, protocol, size, timestamp);
        }

        // Sort by timestamp
        std::sort(packets.begin(), packets.end(),
                  [](const NetworkPacket &a, const NetworkPacket &b)
                  {
                      return a.timestamp < b.timestamp;
                  });

        return packets;
    }

    // Run network traffic analysis demo
    void runNetworkAnalysisDemo()
    {
        std::cout << "\n=== NETWORK TRAFFIC ANALYSIS DEMO ===\n";

        // Initialize traffic analyzer
        TrafficAnalyzer analyzer(1024, 5, 10000, 5, 60);

        // Generate network traffic
        int numPackets = 10000;
        int duration = 300; // 5 minutes

        std::cout << "Generating " << numPackets << " packets of network traffic over "
                  << duration << " seconds...\n";

        std::vector<NetworkPacket> packets = generateNetworkTraffic(numPackets, duration);

        // Process packets
        std::cout << "Processing packets...\n\n";

        int processedCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();

        for (const auto &packet : packets)
        {
            analyzer.processPacket(packet);
            processedCount++;

            // Print progress every 1000 packets
            if (processedCount % 1000 == 0)
            {
                std::cout << "Processed " << processedCount << " packets...\n";
                analyzer.getStatistics();
                std::cout << "\n";
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsed = getElapsedMicroseconds(startTime, endTime);

        std::cout << "Processing completed in " << elapsed.count() / 1000.0
                  << " milliseconds\n";
        std::cout << "Average processing time: "
                  << elapsed.count() / static_cast<double>(numPackets)
                  << " microseconds per packet\n\n";

        // Final statistics
        analyzer.getStatistics();
    }

    // Run network analysis benchmark with different sketch sizes
    void runNetworkAnalysisBenchmark()
    {
        std::cout << "\n=== NETWORK ANALYSIS BENCHMARK ===\n";

        // Generate traffic once
        int numPackets = 100000;
        int duration = 300; // 5 minutes

        std::cout << "Generating " << numPackets << " packets for benchmarking...\n";
        std::vector<NetworkPacket> packets = generateNetworkTraffic(numPackets, duration);

        // Test different sketch sizes
        std::vector<int> sketchSizes = {256, 1024, 4096, 16384};

        for (int size : sketchSizes)
        {
            std::cout << "\nTesting with sketch size: " << size << "\n";

            // Initialize analyzer
            TrafficAnalyzer analyzer(size, 5, size * 10, 5, 60);

            // Measure processing time
            auto startTime = std::chrono::high_resolution_clock::now();

            for (const auto &packet : packets)
            {
                analyzer.processPacket(packet);
            }

            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsed = getElapsedMicroseconds(startTime, endTime);

            std::cout << "Processing time: " << elapsed.count() / 1000.0
                      << " milliseconds\n";
            std::cout << "Average processing time: "
                      << elapsed.count() / static_cast<double>(numPackets)
                      << " microseconds per packet\n";

            // Get statistics
            analyzer.getStatistics();
        }
    }
}

//==============================================================================
// Challenge 4: Distributed Cache with Smart Eviction
//==============================================================================

namespace DistributedCache
{

    // Cache entry
    struct CacheEntry
    {
        std::string key;
        std::string value;
        int frequency;
        int lastAccess;
        int size; // Size in bytes

        CacheEntry(std::string k, std::string v, int sz)
            : key(std::move(k)), value(std::move(v)), frequency(1),
              lastAccess(static_cast<int>(time(nullptr))), size(sz) {}
    };

    // LRU Cache implementation
    class LRUCache
    {
    private:
        // Cache capacity in bytes
        int capacity;

        // Current size in bytes
        int currentSize;

        // Cache entries
        std::unordered_map<std::string, CacheEntry> entries;

        // Access order (most recent at the back)
        std::list<std::string> accessOrder;

        // Key to iterator mapping for O(1) access order updates
        std::unordered_map<std::string, std::list<std::string>::iterator> keyIter;

        // Mutex for thread safety
        mutable std::shared_mutex mutex;

    public:
        explicit LRUCache(int cap) : capacity(cap), currentSize(0) {}

        // Get a value
        std::optional<std::string> get(const std::string &key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            auto it = entries.find(key);
            if (it == entries.end())
            {
                return std::nullopt; // Not found
            }

            // Update access order
            accessOrder.erase(keyIter[key]);
            accessOrder.push_back(key);
            keyIter[key] = std::prev(accessOrder.end());

            // Update statistics
            it->second.frequency++;
            it->second.lastAccess = static_cast<int>(time(nullptr));

            return it->second.value;
        }

        // Put a value
        void put(const std::string &key, const std::string &value)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            int size = value.size();

            // Check if key already exists
            auto it = entries.find(key);
            if (it != entries.end())
            {
                // Update existing entry
                int oldSize = it->second.size;
                it->second.value = value;
                it->second.size = size;
                it->second.frequency++;
                it->second.lastAccess = static_cast<int>(time(nullptr));

                // Update access order
                accessOrder.erase(keyIter[key]);
                accessOrder.push_back(key);
                keyIter[key] = std::prev(accessOrder.end());

                // Update current size
                currentSize = currentSize - oldSize + size;
                return;
            }

            // Evict entries if necessary
            while (currentSize + size > capacity && !accessOrder.empty())
            {
                std::string evictKey = accessOrder.front();
                int evictSize = entries[evictKey].size;

                entries.erase(evictKey);
                accessOrder.pop_front();
                keyIter.erase(evictKey);

                currentSize -= evictSize;
            }

            // Add new entry if it fits
            if (size <= capacity)
            {
                entries.emplace(key, CacheEntry(key, value, size));
                accessOrder.push_back(key);
                keyIter[key] = std::prev(accessOrder.end());
                currentSize += size;
            }
        }

        // Check if key exists
        bool contains(const std::string &key) const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            return entries.find(key) != entries.end();
        }

        // Get cache statistics
        void getStats() const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::cout << "LRU Cache Statistics:\n";
            std::cout << "  Entries: " << entries.size() << "\n";
            std::cout << "  Current Size: " << currentSize << " bytes\n";
            std::cout << "  Capacity: " << capacity << " bytes\n";
            std::cout << "  Utilization: " << (currentSize * 100.0 / capacity) << "%\n";
        }
    };

    // LFU Cache implementation
    class LFUCache
    {
    private:
        // Cache capacity in bytes
        int capacity;

        // Current size in bytes
        int currentSize;

        // Cache entries
        std::unordered_map<std::string, CacheEntry> entries;

        // Frequency map: frequency -> list of keys
        std::map<int, std::list<std::string>> freqMap;

        // Key to (frequency, iterator) mapping for O(1) frequency updates
        std::unordered_map<std::string, std::pair<int, std::list<std::string>::iterator>> keyInfo;

        // Mutex for thread safety
        mutable std::shared_mutex mutex;

    public:
        explicit LFUCache(int cap) : capacity(cap), currentSize(0) {}

        // Get a value
        std::optional<std::string> get(const std::string &key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            auto it = entries.find(key);
            if (it == entries.end())
            {
                return std::nullopt; // Not found
            }

            // Update frequency
            int oldFreq = it->second.frequency;
            int newFreq = oldFreq + 1;
            it->second.frequency = newFreq;
            it->second.lastAccess = static_cast<int>(time(nullptr));

            // Update frequency map
            auto keyInfoIt = keyInfo.find(key);
            freqMap[oldFreq].erase(keyInfoIt->second.second);
            if (freqMap[oldFreq].empty())
            {
                freqMap.erase(oldFreq);
            }

            freqMap[newFreq].push_back(key);
            keyInfoIt->second = {newFreq, std::prev(freqMap[newFreq].end())};

            return it->second.value;
        }

        // Put a value
        void put(const std::string &key, const std::string &value)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);

            int size = value.size();

            // Check if key already exists
            auto it = entries.find(key);
            if (it != entries.end())
            {
                // Update existing entry
                int oldSize = it->second.size;
                it->second.value = value;
                it->second.size = size;
                it->second.lastAccess = static_cast<int>(time(nullptr));

                // Update frequency
                int oldFreq = it->second.frequency;
                int newFreq = oldFreq + 1;
                it->second.frequency = newFreq;

                // Update frequency map
                auto keyInfoIt = keyInfo.find(key);
                freqMap[oldFreq].erase(keyInfoIt->second.second);
                if (freqMap[oldFreq].empty())
                {
                    freqMap.erase(oldFreq);
                }

                freqMap[newFreq].push_back(key);
                keyInfoIt->second = {newFreq, std::prev(freqMap[newFreq].end())};

                // Update current size
                currentSize = currentSize - oldSize + size;
                return;
            }

            // Evict entries if necessary
            while (currentSize + size > capacity && !freqMap.empty())
            {
                // Get the lowest frequency
                int lowestFreq = freqMap.begin()->first;

                // Evict the least recently used item with lowest frequency
                std::string evictKey = freqMap[lowestFreq].front();
                int evictSize = entries[evictKey].size;

                entries.erase(evictKey);
                freqMap[lowestFreq].pop_front();
                if (freqMap[lowestFreq].empty())
                {
                    freqMap.erase(lowestFreq);
                }
                keyInfo.erase(evictKey);

                currentSize -= evictSize;
            }

            // Add new entry if it fits
            if (size <= capacity)
            {
                entries.emplace(key, CacheEntry(key, value, size));
                freqMap[1].push_back(key);
                keyInfo[key] = {1, std::prev(freqMap[1].end())};
                currentSize += size;
            }
        }

        // Check if key exists
        bool contains(const std::string &key) const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            return entries.find(key) != entries.end();
        }

        // Get cache statistics
        void getStats() const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::cout << "LFU Cache Statistics:\n";
            std::cout << "  Entries: " << entries.size() << "\n";
            std::cout << "  Current Size: " << currentSize << " bytes\n";
            std::cout << "  Capacity: " << capacity << " bytes\n";
            std::cout << "  Utilization: " << (currentSize * 100.0 / capacity) << "%\n";
            std::cout << "  Frequency Levels: " << freqMap.size() << "\n";
        }
    };

    // Consistent hashing for node selection
    class ConsistentHash
    {
    private:
        // Virtual nodes mapped to positions on the ring
        std::map<size_t, int> ring;

        // Number of virtual nodes per real node
        int numReplicas;

        // Hash function
        size_t hash(const std::string &key) const
        {
            return hashString(key);
        }

    public:
        explicit ConsistentHash(int replicas) : numReplicas(replicas) {}

        // Add a node to the ring
        void addNode(int nodeId)
        {
            for (int i = 0; i < numReplicas; i++)
            {
                size_t hash = hashString(std::to_string(nodeId) + ":" + std::to_string(i));
                ring[hash] = nodeId;
            }
        }

        // Remove a node from the ring
        void removeNode(int nodeId)
        {
            for (int i = 0; i < numReplicas; i++)
            {
                size_t hash = hashString(std::to_string(nodeId) + ":" + std::to_string(i));
                ring.erase(hash);
            }
        }

        // Get the node responsible for a key
        int getNode(const std::string &key) const
        {
            if (ring.empty())
            {
                throw std::runtime_error("Ring is empty");
            }

            size_t keyHash = hash(key);
            auto it = ring.lower_bound(keyHash);

            if (it == ring.end())
            {
                // Wrap around to the first node
                return ring.begin()->second;
            }

            return it->second;
        }

        // Get all nodes
        std::vector<int> getAllNodes() const
        {
            std::vector<int> nodes;
            std::set<int> uniqueNodes;

            for (const auto &[hash, nodeId] : ring)
            {
                uniqueNodes.insert(nodeId);
            }

            nodes.insert(nodes.end(), uniqueNodes.begin(), uniqueNodes.end());
            return nodes;
        }
    };

    // Distributed cache implementation
    class DistributedCacheImpl
    {
    private:
        // Cache nodes (LRU or LFU)
        std::vector<std::unique_ptr<LRUCache>> lruNodes;
        std::vector<std::unique_ptr<LFUCache>> lfuNodes;

        // Consistent hashing ring
        ConsistentHash ring;

        // Cache policy (LRU or LFU)
        std::string policy;

        // Node capacity
        int nodeCapacity;

        // Mutex for thread safety during reconfiguration
        mutable std::mutex configMutex;

    public:
        DistributedCacheImpl(int numNodes, int nodeCap, std::string p = "LRU")
            : ring(ConsistentHash(100)), nodeCapacity(nodeCap), policy(std::move(p))
        {

            std::lock_guard<std::mutex> lock(configMutex);

            if (policy == "LRU")
            {
                lruNodes.reserve(numNodes);
                for (int i = 0; i < numNodes; i++)
                {
                    lruNodes.push_back(std::make_unique<LRUCache>(nodeCapacity));
                    ring.addNode(i);
                }
            }
            else if (policy == "LFU")
            {
                lfuNodes.reserve(numNodes);
                for (int i = 0; i < numNodes; i++)
                {
                    lfuNodes.push_back(std::make_unique<LFUCache>(nodeCapacity));
                    ring.addNode(i);
                }
            }
            else
            {
                throw std::invalid_argument("Invalid cache policy: " + policy);
            }
        }

        // Get value for a key
        std::optional<std::string> get(const std::string &key)
        {
            int nodeId = ring.getNode(key);

            if (policy == "LRU")
            {
                return lruNodes[nodeId]->get(key);
            }
            else
            { // LFU
                return lfuNodes[nodeId]->get(key);
            }
        }

        // Put a key-value pair
        void put(const std::string &key, const std::string &value)
        {
            int nodeId = ring.getNode(key);

            if (policy == "LRU")
            {
                lruNodes[nodeId]->put(key, value);
            }
            else
            { // LFU
                lfuNodes[nodeId]->put(key, value);
            }
        }

        // Check if key exists in cache
        bool contains(const std::string &key)
        {
            int nodeId = ring.getNode(key);

            if (policy == "LRU")
            {
                return lruNodes[nodeId]->contains(key);
            }
            else
            { // LFU
                return lfuNodes[nodeId]->contains(key);
            }
        }

        // Add a new node
        void addNode()
        {
            std::lock_guard<std::mutex> lock(configMutex);

            int newNodeId = 0;

            if (policy == "LRU")
            {
                newNodeId = lruNodes.size();
                lruNodes.push_back(std::make_unique<LRUCache>(nodeCapacity));
            }
            else
            { // LFU
                newNodeId = lfuNodes.size();
                lfuNodes.push_back(std::make_unique<LFUCache>(nodeCapacity));
            }

            ring.addNode(newNodeId);
        }

        // Remove a node
        void removeNode(int nodeId)
        {
            std::lock_guard<std::mutex> lock(configMutex);

            if (policy == "LRU")
            {
                if (nodeId < 0 || nodeId >= lruNodes.size())
                {
                    throw std::out_of_range("Invalid node ID");
                }
            }
            else
            { // LFU
                if (nodeId < 0 || nodeId >= lfuNodes.size())
                {
                    throw std::out_of_range("Invalid node ID");
                }
            }

            ring.removeNode(nodeId);
        }

        // Get cache statistics
        void getStats() const
        {
            std::cout << "Distributed Cache Statistics:\n";
            std::cout << "  Policy: " << policy << "\n";
            std::cout << "  Number of Nodes: " << (policy == "LRU" ? lruNodes.size() : lfuNodes.size()) << "\n";
            std::cout << "  Node Capacity: " << nodeCapacity << " bytes\n";

            // Get stats for each node
            if (policy == "LRU")
            {
                for (size_t i = 0; i < lruNodes.size(); i++)
                {
                    std::cout << "\nNode " << i << " Stats:\n";
                    lruNodes[i]->getStats();
                }
            }
            else
            { // LFU
                for (size_t i = 0; i < lfuNodes.size(); i++)
                {
                    std::cout << "\nNode " << i << " Stats:\n";
                    lfuNodes[i]->getStats();
                }
            }
        }
    };

    // Run distributed cache demo
    void runDistributedCacheDemo()
    {
        std::cout << "\n=== DISTRIBUTED CACHE DEMO ===\n";

        // Initialize cache
        int numNodes = 3;
        int nodeCapacity = 1024 * 1024; // 1 MB per node

        std::cout << "Initializing distributed cache with " << numNodes
                  << " nodes and " << (nodeCapacity / 1024) << " KB capacity per node...\n";

        DistributedCacheImpl cache(numNodes, nodeCapacity, "LRU");

        // Test basic operations
        std::cout << "\nTesting basic operations...\n";

        // Put some values
        cache.put("key1", "value1");
        cache.put("key2", "value2");
        cache.put("key3", "value3");

        // Get values
        std::cout << "key1: " << cache.get("key1").value_or("not found") << "\n";
        std::cout << "key2: " << cache.get("key2").value_or("not found") << "\n";
        std::cout << "key3: " << cache.get("key3").value_or("not found") << "\n";
        std::cout << "key4: " << cache.get("key4").value_or("not found") << "\n";

        // Test cache hit/miss
        std::cout << "\nTesting cache contains...\n";
        std::cout << "Contains key1: " << (cache.contains("key1") ? "Yes" : "No") << "\n";
        std::cout << "Contains key4: " << (cache.contains("key4") ? "Yes" : "No") << "\n";

        // Fill cache with data
        std::cout << "\nFilling cache with random data...\n";

        // Generate random key-value pairs
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> keySizeDist(10, 30);
        std::uniform_int_distribution<> valueSizeDist(100, 10000);

        for (int i = 0; i < 1000; i++)
        {
            std::string key = "key_" + std::to_string(i) + "_" + generateRandomString(keySizeDist(gen));
            std::string value = generateRandomString(valueSizeDist(gen));

            cache.put(key, value);
        }

        // Get statistics
        cache.getStats();

        // Test node addition
        std::cout << "\nAdding a new node...\n";
        cache.addNode();

        // Put and get more values
        for (int i = 1000; i < 2000; i++)
        {
            std::string key = "key_" + std::to_string(i) + "_" + generateRandomString(keySizeDist(gen));
            std::string value = generateRandomString(valueSizeDist(gen));

            cache.put(key, value);
        }

        // Get statistics again
        cache.getStats();

        // Test node removal
        std::cout << "\nRemoving a node...\n";
        cache.removeNode(1); // Remove the second node

        // Get statistics again
        cache.getStats();
    }

    // Run distributed cache benchmark
    void runDistributedCacheBenchmark()
    {
        std::cout << "\n=== DISTRIBUTED CACHE BENCHMARK ===\n";

        // Test different configurations
        std::vector<std::pair<std::string, int>> configs = {
            {"LRU", 3},
            {"LFU", 3},
            {"LRU", 5},
            {"LFU", 5}};

        int nodeCapacity = 1024 * 1024; // 1 MB per node
        int numOperations = 100000;

        // Generate random keys and values once
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> keySizeDist(10, 30);
        std::uniform_int_distribution<> valueSizeDist(100, 1000);

        std::vector<std::string> keys;
        std::vector<std::string> values;

        for (int i = 0; i < numOperations; i++)
        {
            keys.push_back("key_" + std::to_string(i) + "_" + generateRandomString(keySizeDist(gen)));
            values.push_back(generateRandomString(valueSizeDist(gen)));
        }

        for (const auto &[policy, numNodes] : configs)
        {
            std::cout << "\nTesting " << policy << " policy with " << numNodes << " nodes...\n";

            DistributedCacheImpl cache(numNodes, nodeCapacity, policy);

            // Measure put performance
            auto startPut = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < numOperations; i++)
            {
                cache.put(keys[i], values[i]);
            }

            auto endPut = std::chrono::high_resolution_clock::now();
            auto putTime = getElapsedMicroseconds(startPut, endPut);

            std::cout << "Put operations: " << numOperations << " in "
                      << putTime.count() / 1000.0 << " milliseconds "
                      << "(" << putTime.count() / numOperations << " μs per operation)\n";

            // Measure get performance (cache hits)
            auto startGetHit = std::chrono::high_resolution_clock::now();

            int hits = 0;
            for (int i = 0; i < numOperations; i++)
            {
                auto value = cache.get(keys[i]);
                if (value)
                    hits++;
            }

            auto endGetHit = std::chrono::high_resolution_clock::now();
            auto getHitTime = getElapsedMicroseconds(startGetHit, endGetHit);

            std::cout << "Get operations (cache hits): " << hits << " in "
                      << getHitTime.count() / 1000.0 << " milliseconds "
                      << "(" << getHitTime.count() / numOperations << " μs per operation)\n";

            // Measure get performance (cache misses)
            auto startGetMiss = std::chrono::high_resolution_clock::now();

            int misses = 0;
            for (int i = 0; i < numOperations; i++)
            {
                auto value = cache.get("missing_" + keys[i]);
                if (!value)
                    misses++;
            }

            auto endGetMiss = std::chrono::high_resolution_clock::now();
            auto getMissTime = getElapsedMicroseconds(startGetMiss, endGetMiss);

            std::cout << "Get operations (cache misses): " << misses << " in "
                      << getMissTime.count() / 1000.0 << " milliseconds "
                      << "(" << getMissTime.count() / numOperations << " μs per operation)\n";

            // Get cache statistics
            cache.getStats();
        }
    }
}

// Main function
int main()
{
    std::cout << "===================================\n";
    std::cout << "Day 42: Advanced Algorithm Problems\n";
    std::cout << "===================================\n";

    // Run demonstrations
    FileIndexing::runFileIndexingDemo();
    RouteOptimization::runRouteOptimizationDemo();
    NetworkTrafficAnalysis::runNetworkAnalysisDemo();
    DistributedCache::runDistributedCacheDemo();

    // Run benchmarks
    FileIndexing::runFileIndexingBenchmark();
    RouteOptimization::runRouteOptimizationBenchmark();
    NetworkTrafficAnalysis::runNetworkAnalysisBenchmark();
    DistributedCache::runDistributedCacheBenchmark();

    return 0;
}