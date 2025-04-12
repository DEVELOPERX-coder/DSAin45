# 🗃️ DAY 13: Sets & Maps in C++ STL - The Plug-and-Play Powerhouses #DSAin45

Welcome to Day 13! Over the last two days, we've explored hash tables from first principles, building our own implementations and understanding their inner workings. Today, we're diving into the battle-tested associative containers provided by the C++ Standard Template Library (STL) that let us leverage all that power right out of the box.

_Why reinvent the wheel when the STL provides high-performance Formula 1 tires ready for the race? Today we'll compare ordered and unordered variants and see when to use each for optimal performance._

## 🧰 C++ STL Associative Containers Overview

The C++ STL provides eight associative containers that fall into two main categories:

### 1. Ordered Associative Containers (Tree-Based)

- **`std::set`**: Collection of unique keys, sorted by keys
- **`std::map`**: Collection of key-value pairs, sorted by keys, keys are unique
- **`std::multiset`**: Collection of keys, sorted by keys, permits duplicates
- **`std::multimap`**: Collection of key-value pairs, sorted by keys, permits duplicate keys

### 2. Unordered Associative Containers (Hash-Based)

- **`std::unordered_set`**: Collection of unique keys, hashed by keys
- **`std::unordered_map`**: Collection of key-value pairs, hashed by keys, keys are unique
- **`std::unordered_multiset`**: Collection of keys, hashed by keys, permits duplicates
- **`std::unordered_multimap`**: Collection of key-value pairs, hashed by keys, permits duplicate keys

> **🧠 Fun Fact**: The ordered associative containers were part of the original STL design by Alexander Stepanov in the early 1990s, while the unordered variants were added later in C++11 (2011) to provide hash-based alternatives.

## 🔍 Unordered Containers (Hash Tables)

Unordered containers are implemented using hash tables, similar to what we've built in the last two days. They provide average O(1) operations but may degrade to O(n) in worst-case scenarios (when all elements hash to the same bucket).

### `std::unordered_set`

A collection of unique keys, hashed by keys:

```cpp
#include <iostream>
#include <unordered_set>
#include <string>

int main() {
    // Create an unordered_set of strings
    std::unordered_set<std::string> colors = {"red", "green", "blue"};

    // Insert elements
    colors.insert("yellow");
    colors.insert("purple");

    // Check if element exists
    if (colors.find("green") != colors.end()) {
        std::cout << "Found green!" << std::endl;
    }

    // Attempt to insert duplicate (will be ignored)
    auto [iter, inserted] = colors.insert("red");
    std::cout << "Inserted red: " << (inserted ? "yes" : "no") << std::endl;

    // Remove an element
    colors.erase("blue");

    // Iterate through all elements (unordered)
    std::cout << "Colors: ";
    for (const auto& color : colors) {
        std::cout << color << " ";
    }
    std::cout << std::endl;

    // Size operations
    std::cout << "Size: " << colors.size() << std::endl;
    std::cout << "Empty? " << (colors.empty() ? "yes" : "no") << std::endl;

    // Hash table specific operations
    std::cout << "Bucket count: " << colors.bucket_count() << std::endl;
    std::cout << "Load factor: " << colors.load_factor() << std::endl;
    std::cout << "Max load factor: " << colors.max_load_factor() << std::endl;

    return 0;
}
```

_An unordered_set is like a drawer where you toss items in without a specific order—quick to put in and find, but if you want them sorted, you'll need a different solution._

### `std::unordered_map`

A collection of key-value pairs, hashed by keys, where keys are unique:

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // Create an unordered_map of string keys to int values
    std::unordered_map<std::string, int> ages = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35}
    };

    // Access with operator[] (creates entry if key doesn't exist)
    ages["David"] = 40;

    // Access with at() (throws exception if key doesn't exist)
    try {
        std::cout << "Charlie's age: " << ages.at("Charlie") << std::endl;
        std::cout << "Eve's age: " << ages.at("Eve") << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // Check if key exists
    if (ages.contains("Bob")) {  // C++20 feature
        std::cout << "Bob is in the map" << std::endl;
    }

    // Pre-C++20 way to check if key exists
    if (ages.find("Bob") != ages.end()) {
        std::cout << "Bob is in the map (older check style)" << std::endl;
    }

    // Iterate through key-value pairs (unordered)
    std::cout << "Ages: ";
    for (const auto& [name, age] : ages) {  // C++17 structured binding
        std::cout << name << "=" << age << " ";
    }
    std::cout << std::endl;

    // Erase an element
    ages.erase("Bob");

    // Update an existing value
    ages["Alice"] = 26;

    // Size and bucket information
    std::cout << "Size: " << ages.size() << std::endl;
    std::cout << "Bucket count: " << ages.bucket_count() << std::endl;

    return 0;
}
```

_An unordered_map is like a phonebook where entries are randomly distributed across pages for quick lookup by name—not alphabetically ordered, but lightning fast to find someone._

### Multi-Variants: `std::unordered_multiset` and `std::unordered_multimap`

These allow duplicate keys:

```cpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>

int main() {
    // Multiset example - allows duplicate keys
    std::unordered_multiset<std::string> tags = {"programming", "C++", "STL", "C++", "tutorial"};

    std::cout << "All tags: ";
    for (const auto& tag : tags) {
        std::cout << tag << " ";
    }
    std::cout << std::endl;

    // Count occurrences of a key
    std::cout << "Occurrences of 'C++': " << tags.count("C++") << std::endl;

    // Multimap example - allows duplicate keys
    std::unordered_multimap<std::string, std::string> courseInstructors;

    // Insert multiple teachers for the same course
    courseInstructors.insert({"Mathematics", "Dr. Smith"});
    courseInstructors.insert({"Computer Science", "Prof. Johnson"});
    courseInstructors.insert({"Mathematics", "Dr. Brown"});
    courseInstructors.insert({"Physics", "Dr. Lee"});
    courseInstructors.insert({"Computer Science", "Dr. Wilson"});

    // Finding all instructors for a course
    std::cout << "Mathematics instructors: ";
    auto range = courseInstructors.equal_range("Mathematics");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

_A multiset is like a bag that can hold multiple identical items, while a multimap is like a bulletin board where multiple notices can be pinned under the same category._

## 🌲 Ordered Containers (Tree-Based)

Ordered containers are implemented using balanced binary search trees (typically Red-Black Trees). They provide O(log n) operations but guarantee sorted traversal and bounded worst-case performance.

### `std::set`

A collection of unique keys, sorted by keys:

```cpp
#include <iostream>
#include <set>
#include <string>

int main() {
    // Create a set of strings (sorted lexicographically)
    std::set<std::string> colors = {"red", "green", "blue"};

    // Insert elements
    colors.insert("yellow");
    colors.insert("purple");

    // Check if element exists
    if (colors.find("green") != colors.end()) {
        std::cout << "Found green!" << std::endl;
    }

    // Attempt to insert duplicate (will be ignored)
    auto [iter, inserted] = colors.insert("red");
    std::cout << "Inserted red: " << (inserted ? "yes" : "no") << std::endl;

    // Remove an element
    colors.erase("blue");

    // Iterate through all elements (in sorted order)
    std::cout << "Colors: ";
    for (const auto& color : colors) {
        std::cout << color << " ";
    }
    std::cout << std::endl;

    // Lower bound and upper bound
    auto lb = colors.lower_bound("green");
    auto ub = colors.upper_bound("green");

    std::cout << "Lower bound of 'green': " << *lb << std::endl;
    std::cout << "Upper bound of 'green': " << *ub << std::endl;

    return 0;
}
```

_A set is like a perfectly organized bookshelf where each title appears exactly once and everything is arranged alphabetically—takes a bit longer to find things than a random pile, but allows for ordered operations like "find all books starting with 'A'"._

### `std::map`

A collection of key-value pairs, sorted by keys, where keys are unique:

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // Create a map of string keys to int values (sorted by keys)
    std::map<std::string, int> ages = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35}
    };

    // Access with operator[] (creates entry if key doesn't exist)
    ages["David"] = 40;

    // Access with at() (throws exception if key doesn't exist)
    try {
        std::cout << "Charlie's age: " << ages.at("Charlie") << std::endl;
        std::cout << "Eve's age: " << ages.at("Eve") << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // Iterate through key-value pairs (in sorted key order)
    std::cout << "Ages: ";
    for (const auto& [name, age] : ages) {  // C++17 structured binding
        std::cout << name << "=" << age << " ";
    }
    std::cout << std::endl;

    // Erase an element
    ages.erase("Bob");

    // Range-based queries
    std::cout << "People with names from A to C: ";
    for (auto it = ages.lower_bound("A"); it != ages.lower_bound("D"); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

_A map is like an alphabetically organized address book—takes a moment longer to find someone compared to a random notebook, but makes it easy to find "all names starting with M"._

### Multi-Variants: `std::multiset` and `std::multimap`

These allow duplicate keys and maintain sorted order:

```cpp
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    // Multiset example - allows duplicate keys and keeps them sorted
    std::multiset<int> scores = {85, 92, 78, 85, 90, 78, 95};

    std::cout << "Scores in ascending order: ";
    for (const auto& score : scores) {
        std::cout << score << " ";
    }
    std::cout << std::endl;

    // Count occurrences of a value
    std::cout << "Occurrences of 85: " << scores.count(85) << std::endl;

    // Multimap example - allows duplicate keys and keeps them sorted
    std::multimap<int, std::string> rankingsByScore;

    // Insert multiple students for the same score
    rankingsByScore.insert({95, "Alice"});
    rankingsByScore.insert({85, "Bob"});
    rankingsByScore.insert({95, "Charlie"});
    rankingsByScore.insert({78, "David"});
    rankingsByScore.insert({85, "Eve"});

    // Print in descending order (highest score first)
    std::cout << "Rankings (highest scores first):" << std::endl;
    for (auto it = rankingsByScore.rbegin(); it != rankingsByScore.rend(); ++it) {
        std::cout << "  Score " << it->first << ": " << it->second << std::endl;
    }

    // Find all students with a particular score
    std::cout << "Students with score 85: ";
    auto range = rankingsByScore.equal_range(85);
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

_A multiset is like a leaderboard that can show multiple players with the same score, all sorted by score, while a multimap is like a school grade book where multiple students can have the same grade, organized from lowest to highest._

## ⚡ Performance Comparison

Understanding the performance characteristics is crucial for choosing the right container:

| Operation | Ordered (set/map) | Unordered (unordered_set/unordered_map) |
| --------- | ----------------- | --------------------------------------- |
| Insert    | O(log n)          | O(1) average, O(n) worst case           |
| Delete    | O(log n)          | O(1) average, O(n) worst case           |
| Search    | O(log n)          | O(1) average, O(n) worst case           |
| Iteration | O(n) in order     | O(n), no guaranteed order               |
| Memory    | Lower             | Higher due to hash table overhead       |

### When to Use Ordered Containers

- When elements need to be kept in sorted order
- When you need efficient range queries (e.g., find all keys between X and Y)
- When you need guaranteed worst-case performance
- When the key type doesn't have a natural hash function
- When memory usage is a significant concern

### When to Use Unordered Containers

- When lookup speed is critical and you don't care about order
- When you have a good hash function for your key type
- When you're working with large datasets where O(log n) vs O(1) makes a meaningful difference
- When you need specialized hash table operations (like load factor control)

_Choosing between ordered and unordered containers is like deciding between a methodically organized filing cabinet vs. a hash-based retrieval system—the filing cabinet ensures everything is in order but takes longer to find things, while the hash system provides lightning-fast retrieval but at the cost of orderliness._

## 🛠️ Internal Implementation Details

Understanding the internals helps explain the performance characteristics:

### Ordered Containers

- Implemented using balanced binary search trees (typically Red-Black Trees)
- Each node contains a key (and value for maps)
- Trees self-balance to maintain O(log n) height
- Traversing in-order gives sorted elements
- Node-based structure with good memory locality for adjacent elements

### Unordered Containers

- Implemented using hash tables
- Array of buckets, each containing a linked list (or similar) of elements
- Hash function maps keys to bucket indices
- Load factor determines when to rehash (resize and redistribute)
- Default max load factor is typically around 1.0
- Uses much more memory than ordered containers due to buckets

## 🧩 Custom Comparators and Hash Functions

### Custom Comparators for Ordered Containers

When using custom types with ordered containers, you need to define how they should be compared:

```cpp
#include <iostream>
#include <set>
#include <map>
#include <string>

// Custom type
struct Person {
    std::string name;
    int age;

    // Constructor
    Person(std::string n, int a) : name(std::move(n)), age(a) {}

    // For demonstration, we'll make this work in both less-than and greater-than contexts
    bool operator<(const Person& other) const {
        // Primary: Compare by name
        if (name != other.name) {
            return name < other.name;
        }
        // Secondary: Compare by age
        return age < other.age;
    }
};

// Custom comparator for ordering by age
struct CompareByAge {
    bool operator()(const Person& a, const Person& b) const {
        // Primary: Compare by age
        if (a.age != b.age) {
            return a.age < b.age;
        }
        // Secondary: Compare by name
        return a.name < b.name;
    }
};

int main() {
    // Set ordered by the operator< (by name, then age)
    std::set<Person> peopleByName;
    peopleByName.insert(Person("Alice", 30));
    peopleByName.insert(Person("Bob", 25));
    peopleByName.insert(Person("Alice", 25));  // Will be separate from the other Alice

    std::cout << "People ordered by name, then age:" << std::endl;
    for (const auto& person : peopleByName) {
        std::cout << "  " << person.name << " (" << person.age << ")" << std::endl;
    }

    // Set ordered by the custom comparator (by age, then name)
    std::set<Person, CompareByAge> peopleByAge;
    peopleByAge.insert(Person("Alice", 30));
    peopleByAge.insert(Person("Bob", 25));
    peopleByAge.insert(Person("Charlie", 25));  // Same age as Bob, but different name

    std::cout << "\nPeople ordered by age, then name:" << std::endl;
    for (const auto& person : peopleByAge) {
        std::cout << "  " << person.name << " (" << person.age << ")" << std::endl;
    }

    return 0;
}
```

_Creating a custom comparator is like defining the sorting rules for a card game—you're telling the container "this is how I want my elements ranked."_

### Custom Hash Functions for Unordered Containers

When using custom types with unordered containers, you need to define how they should be hashed and compared for equality:

```cpp
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>

// Custom type
struct Point {
    int x, y;

    // Constructor
    Point(int x_, int y_) : x(x_), y(y_) {}

    // Equality operator (required for unordered containers)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Specialization of std::hash for Point
namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            // Combine the hash of x and y
            size_t h1 = std::hash<int>{}(p.x);
            size_t h2 = std::hash<int>{}(p.y);
            return h1 ^ (h2 << 1);  // Simple combining technique
        }
    };
}

int main() {
    // Create an unordered_set of Points
    std::unordered_set<Point> points;
    points.insert(Point(1, 2));
    points.insert(Point(3, 4));
    points.insert(Point(1, 2));  // Duplicate will be ignored

    std::cout << "Points in the set: " << points.size() << std::endl;

    // Create an unordered_map with Point keys
    std::unordered_map<Point, std::string> pointLabels;
    pointLabels[Point(1, 2)] = "Origin";
    pointLabels[Point(5, 5)] = "Top-Right";

    // Check if a point exists
    Point target(3, 4);
    if (points.find(target) != points.end()) {
        std::cout << "Found point (" << target.x << ", " << target.y << ")" << std::endl;
    }

    // Alternative way to provide hash and equality functions (without specializing std::hash)
    struct PointHash {
        size_t operator()(const Point& p) const {
            return p.x * 31 + p.y;
        }
    };

    struct PointEqual {
        bool operator()(const Point& a, const Point& b) const {
            return a.x == b.x && a.y == b.y;
        }
    };

    std::unordered_set<Point, PointHash, PointEqual> pointsAlt;
    pointsAlt.insert(Point(1, 2));
    pointsAlt.insert(Point(3, 4));

    return 0;
}
```

_Creating a custom hash function is like designing a unique fingerprinting system for your objects—it needs to produce distinct values for different objects while still being fast to compute._

## 🌐 Real-World Applications

### 1. Using `std::set` for Duplicate Removal

```cpp
#include <iostream>
#include <set>
#include <vector>
#include <string>

std::vector<std::string> removeDuplicates(const std::vector<std::string>& input) {
    // Insert all elements into a set (which automatically discards duplicates)
    std::set<std::string> uniqueItems(input.begin(), input.end());

    // Convert back to vector (now in sorted order)
    return std::vector<std::string>(uniqueItems.begin(), uniqueItems.end());
}

int main() {
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Alice", "David", "Bob", "Eve"};

    std::vector<std::string> uniqueNames = removeDuplicates(names);

    std::cout << "Unique names (sorted): ";
    for (const auto& name : uniqueNames) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

_Using a set for duplicate removal is like pouring mixed nuts through a sieve where each type of nut can only pass through once, resulting in a sorted collection of unique items._

### 2. Using `std::map` for Word Frequency Counter

```cpp
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

std::map<std::string, int> countWordFrequency(const std::string& text) {
    std::map<std::string, int> frequencies;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        // Convert to lowercase and remove punctuation
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
            return !std::isalpha(c);
        }), word.end());

        if (!word.empty()) {
            frequencies[word]++;
        }
    }

    return frequencies;
}

int main() {
    std::string text = "To be, or not to be, that is the question: "
                      "Whether 'tis nobler in the mind to suffer "
                      "The slings and arrows of outrageous fortune, "
                      "Or to take arms against a sea of troubles "
                      "And by opposing end them.";

    std::map<std::string, int> wordFreq = countWordFrequency(text);

    // Print words in alphabetical order with their frequencies
    std::cout << "Word frequencies (alphabetical order):" << std::endl;
    for (const auto& [word, count] : wordFreq) {
        std::cout << "  " << word << ": " << count << std::endl;
    }

    // Find the most frequent word
    auto mostFrequent = std::max_element(wordFreq.begin(), wordFreq.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    std::cout << "\nMost frequent word: " << mostFrequent->first
              << " (appears " << mostFrequent->second << " times)" << std::endl;

    return 0;
}
```

_A word frequency counter using map is like an automatic tally sheet that not only counts each unique word but also keeps them in alphabetical order._

### 3. Using `std::unordered_map` for Database Indexing

```cpp
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>

struct Record {
    int id;
    std::string name;
    std::string email;

    Record(int i, std::string n, std::string e)
        : id(i), name(std::move(n)), email(std::move(e)) {}
};

class SimpleDatabase {
private:
    std::vector<Record> records;
    std::unordered_map<int, size_t> id_index;        // id -> position in records
    std::unordered_map<std::string, size_t> name_index; // name -> position in records

public:
    void addRecord(int id, const std::string& name, const std::string& email) {
        // Add to main storage
        records.emplace_back(id, name, email);
        size_t index = records.size() - 1;

        // Update indices
        id_index[id] = index;
        name_index[name] = index;
    }

    Record* findById(int id) {
        auto it = id_index.find(id);
        if (it != id_index.end()) {
            return &records[it->second];
        }
        return nullptr;
    }

    Record* findByName(const std::string& name) {
        auto it = name_index.find(name);
        if (it != name_index.end()) {
            return &records[it->second];
        }
        return nullptr;
    }

    // Linear search (without using indices)
    Record* findByIdLinear(int id) {
        for (auto& record : records) {
            if (record.id == id) {
                return &record;
            }
        }
        return nullptr;
    }
};

int main() {
    // Create a small database
    SimpleDatabase db;

    // Add some records
    for (int i = 0; i < 10000; i++) {
        db.addRecord(i, "Person" + std::to_string(i), "person" + std::to_string(i) + "@example.com");
    }

    // Measure lookup performance with index
    auto start = std::chrono::high_resolution_clock::now();
    Record* record = db.findById(5000);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Indexed lookup: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
              << " ns" << std::endl;

    // Measure lookup performance without index (linear search)
    start = std::chrono::high_resolution_clock::now();
    record = db.findByIdLinear(5000);
    end = std::chrono::high_resolution_clock::now();

    std::cout << "Linear lookup: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
              << " ns" << std::endl;

    return 0;
}
```

_Using unordered_map for database indexing is like having an instant lookup table for books in a library—instead of searching every shelf, you can go directly to the right spot._

### 4. Using `std::unordered_set` for Fast Lookup Tables

```cpp
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

class SpellChecker {
private:
    std::unordered_set<std::string> dictionary;

public:
    // Load words from a file
    bool loadDictionary(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            return false;
        }

        std::string word;
        while (file >> word) {
            // Convert to lowercase
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            dictionary.insert(word);
        }

        return true;
    }

    // Check if a word is spelled correctly
    bool isCorrectlySpelled(const std::string& word) const {
        std::string lowercase = word;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);
        return dictionary.find(lowercase) != dictionary.end();
    }

    // Get dictionary size
    size_t size() const {
        return dictionary.size();
    }

    // Suggest corrections for a misspelled word (very basic)
    std::vector<std::string> suggestCorrections(const std::string& word) const {
        std::vector<std::string> suggestions;

        // For this example, we'll just look for words with one character different
        // This is simplistic - real spell checkers use more sophisticated algorithms
        std::string candidate = word;

        // Try changing each character
        for (size_t i = 0; i < word.length(); i++) {
            char original = candidate[i];
            for (char c = 'a'; c <= 'z'; c++) {
                if (c == original) continue;

                candidate[i] = c;
                if (isCorrectlySpelled(candidate)) {
                    suggestions.push_back(candidate);
                    if (suggestions.size() >= 5) return suggestions; // Limit to 5 suggestions
                }
            }
            candidate[i] = original; // Restore original character
        }

        return suggestions;
    }
};

int main() {
    SpellChecker checker;

    // In a real application, you would load from a file
    // For this example, we'll add words directly
    std::vector<std::string> sampleWords = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon",
        "mango", "nectarine", "orange", "peach", "quince",
        "raspberry", "strawberry", "tangerine", "watermelon"
    };

    for (const auto& word : sampleWords) {
        checker.dictionary.insert(word);
    }

    // Check some words
    std::vector<std::string> testWords = {
        "apple", "aple", "banana", "benana", "strawberry", "strawbery"
    };

    for (const auto& word : testWords) {
        bool correct = checker.isCorrectlySpelled(word);
        std::cout << "\"" << word << "\" is " << (correct ? "correctly" : "incorrectly") << " spelled." << std::endl;

        if (!correct) {
            std::cout << "  Suggestions: ";
            auto suggestions = checker.suggestCorrections(word);
            for (const auto& suggestion : suggestions) {
                std::cout << suggestion << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
```

_Using unordered_set for a spell checker is like having a massive dictionary where you can instantly check if a word exists, rather than flipping through pages._

### 5. Using `std::multimap` for Event Scheduling

```cpp
#include <iostream>
#include <map>
#include <string>
#include <ctime>
#include <iomanip>

struct Event {
    std::string title;
    std::string location;

    Event(std::string t, std::string l) : title(std::move(t)), location(std::move(l)) {}
};

class Calendar {
private:
    std::multimap<std::time_t, Event> events;

public:
    // Add an event at a specific time
    void addEvent(const std::string& title, const std::string& location, std::time_t time) {
        events.emplace(time, Event(title, location));
    }

    // Get all events for a specific day
    std::vector<std::pair<std::time_t, Event>> getEventsForDay(std::time_t day) {
        std::vector<std::pair<std::time_t, Event>> dayEvents;

        // Convert day to start and end of day
        std::tm* tm_day = std::localtime(&day);
        tm_day->tm_hour = 0;
        tm_day->tm_min = 0;
        tm_day->tm_sec = 0;
        std::time_t start_of_day = std::mktime(tm_day);

        tm_day->tm_hour = 23;
        tm_day->tm_min = 59;
        tm_day->tm_sec = 59;
        std::time_t end_of_day = std::mktime(tm_day);

        // Find all events within the day
        auto start = events.lower_bound(start_of_day);
        auto end = events.upper_bound(end_of_day);

        for (auto it = start; it != end; ++it) {
            dayEvents.push_back(*it);
        }

        return dayEvents;
    }

    // Get upcoming events
    std::vector<std::pair<std::time_t, Event>> getUpcomingEvents(int count) {
        std::vector<std::pair<std::time_t, Event>> upcoming;

        std::time_t now = std::time(nullptr);
        auto start = events.lower_bound(now);

        int added = 0;
        for (auto it = start; it != events.end() && added < count; ++it, ++added) {
            upcoming.push_back(*it);
        }

        return upcoming;
    }

    // Print all events
    void printAllEvents() {
        for (const auto& [time, event] : events) {
            std::tm* tm_time = std::localtime(&time);
            std::cout << std::put_time(tm_time, "%Y-%m-%d %H:%M:%S") << " - "
                      << event.title << " at " << event.location << std::endl;
        }
    }
};

int main() {
    Calendar calendar;

    // Get current time
    std::time_t now = std::time(nullptr);

    // Add some events
    calendar.addEvent("Team Meeting", "Conference Room A", now + 3600);      // 1 hour from now
    calendar.addEvent("Lunch with Client", "Cafe Bella", now + 7200);        // 2 hours from now
    calendar.addEvent("Project Deadline", "Office", now + 86400);            // 1 day from now
    calendar.addEvent("Conference Call", "Virtual Meeting Room", now + 3600); // Another event at same time

    // Print all events (sorted by time)
    std::cout << "All scheduled events:" << std::endl;
    calendar.printAllEvents();

    // Get upcoming events
    std::cout << "\nNext 2 upcoming events:" << std::endl;
    auto upcoming = calendar.getUpcomingEvents(2);
    for (const auto& [time, event] : upcoming) {
        std::tm* tm_time = std::localtime(&time);
        std::cout << std::put_time(tm_time, "%H:%M:%S") << " - "
                  << event.title << " at " << event.location << std::endl;
    }

    return 0;
}
```

_Using multimap for event scheduling is like having a chronological agenda where multiple events can occur at the same time, all neatly arranged from earliest to latest._

## 🏁 Benchmark: Comparing Container Performance

```cpp
#include <iostream>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <string>
#include <iomanip>

// Helper function to get current time in milliseconds
auto now() {
    return std::chrono::high_resolution_clock::now();
}

// Template function to measure duration
template <typename Func>
long long measureDuration(Func func) {
    auto start = now();
    func();
    auto end = now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Generate random strings
std::string randomString(int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    std::string str;
    str.reserve(length);
    for (int i = 0; i < length; ++i) {
        str += alphanum[dis(gen)];
    }

    return str;
}

// Generate vector of random strings
std::vector<std::string> generateRandomStrings(int count, int length) {
    std::vector<std::string> result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        result.push_back(randomString(length));
    }
    return result;
}

// Benchmark for set vs unordered_set
void benchmarkSets(int count) {
    std::cout << "===== Benchmarking std::set vs std::unordered_set with " << count << " elements =====" << std::endl;

    // Generate random data
    std::vector<std::string> data = generateRandomStrings(count, 10);
    std::vector<std::string> searchData = data;

    // Shuffle searchData for random access patterns
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(searchData.begin(), searchData.end(), g);

    // Measure set insertion
    auto setInsertTime = measureDuration([&]() {
        std::set<std::string> s;
        for (const auto& str : data) {
            s.insert(str);
        }
    });

    // Measure unordered_set insertion
    auto unorderedSetInsertTime = measureDuration([&]() {
        std::unordered_set<std::string> s;
        for (const auto& str : data) {
            s.insert(str);
        }
    });

    // Prepare containers for search test
    std::set<std::string> s;
    std::unordered_set<std::string> us;
    for (const auto& str : data) {
        s.insert(str);
        us.insert(str);
    }

    // Measure set search
    auto setSearchTime = measureDuration([&]() {
        size_t found = 0;
        for (const auto& str : searchData) {
            if (s.find(str) != s.end()) {
                found++;
            }
        }
    });

    // Measure unordered_set search
    auto unorderedSetSearchTime = measureDuration([&]() {
        size_t found = 0;
        for (const auto& str : searchData) {
            if (us.find(str) != us.end()) {
                found++;
            }
        }
    });

    // Measure set iteration
    auto setIterateTime = measureDuration([&]() {
        size_t sum = 0;
        for (const auto& str : s) {
            sum += str.length();
        }
    });

    // Measure unordered_set iteration
    auto unorderedSetIterateTime = measureDuration([&]() {
        size_t sum = 0;
        for (const auto& str : us) {
            sum += str.length();
        }
    });

    // Print results
    std::cout << std::left << std::setw(20) << "Operation"
              << std::setw(15) << "std::set"
              << std::setw(20) << "std::unordered_set"
              << std::setw(15) << "Difference" << std::endl;

    std::cout << std::string(70, '-') << std::endl;

    std::cout << std::left << std::setw(20) << "Insert (µs)"
              << std::setw(15) << setInsertTime
              << std::setw(20) << unorderedSetInsertTime
              << std::setw(15) << (setInsertTime * 100.0 / unorderedSetInsertTime - 100) << "%" << std::endl;

    std::cout << std::left << std::setw(20) << "Search (µs)"
              << std::setw(15) << setSearchTime
              << std::setw(20) << unorderedSetSearchTime
              << std::setw(15) << (setSearchTime * 100.0 / unorderedSetSearchTime - 100) << "%" << std::endl;

    std::cout << std::left << std::setw(20) << "Iterate (µs)"
              << std::setw(15) << setIterateTime
              << std::setw(20) << unorderedSetIterateTime
              << std::setw(15) << (setIterateTime * 100.0 / unorderedSetIterateTime - 100) << "%" << std::endl;

    std::cout << std::endl;
}

// Benchmark for map vs unordered_map
void benchmarkMaps(int count) {
    std::cout << "===== Benchmarking std::map vs std::unordered_map with " << count << " elements =====" << std::endl;

    // Generate random data
    std::vector<std::string> keys = generateRandomStrings(count, 10);
    std::vector<int> values(count);
    for (int i = 0; i < count; ++i) {
        values[i] = i;
    }

    std::vector<std::string> searchKeys = keys;

    // Shuffle searchKeys for random access patterns
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(searchKeys.begin(), searchKeys.end(), g);

    // Measure map insertion
    auto mapInsertTime = measureDuration([&]() {
        std::map<std::string, int> m;
        for (int i = 0; i < count; ++i) {
            m[keys[i]] = values[i];
        }
    });

    // Measure unordered_map insertion
    auto unorderedMapInsertTime = measureDuration([&]() {
        std::unordered_map<std::string, int> m;
        for (int i = 0; i < count; ++i) {
            m[keys[i]] = values[i];
        }
    });

    // Prepare containers for search test
    std::map<std::string, int> m;
    std::unordered_map<std::string, int> um;
    for (int i = 0; i < count; ++i) {
        m[keys[i]] = values[i];
        um[keys[i]] = values[i];
    }

    // Measure map search
    auto mapSearchTime = measureDuration([&]() {
        int sum = 0;
        for (const auto& key : searchKeys) {
            auto it = m.find(key);
            if (it != m.end()) {
                sum += it->second;
            }
        }
    });

    // Measure unordered_map search
    auto unorderedMapSearchTime = measureDuration([&]() {
        int sum = 0;
        for (const auto& key : searchKeys) {
            auto it = um.find(key);
            if (it != um.end()) {
                sum += it->second;
            }
        }
    });

    // Measure map iteration
    auto mapIterateTime = measureDuration([&]() {
        int sum = 0;
        for (const auto& [key, value] : m) {
            sum += value;
        }
    });

    // Measure unordered_map iteration
    auto unorderedMapIterateTime = measureDuration([&]() {
        int sum = 0;
        for (const auto& [key, value] : um) {
            sum += value;
        }
    });

    // Print results
    std::cout << std::left << std::setw(20) << "Operation"
              << std::setw(15) << "std::map"
              << std::setw(20) << "std::unordered_map"
              << std::setw(15) << "Difference" << std::endl;

    std::cout << std::string(70, '-') << std::endl;

    std::cout << std::left << std::setw(20) << "Insert (µs)"
              << std::setw(15) << mapInsertTime
              << std::setw(20) << unorderedMapInsertTime
              << std::setw(15) << (mapInsertTime * 100.0 / unorderedMapInsertTime - 100) << "%" << std::endl;

    std::cout << std::left << std::setw(20) << "Search (µs)"
              << std::setw(15) << mapSearchTime
              << std::setw(20) << unorderedMapSearchTime
              << std::setw(15) << (mapSearchTime * 100.0 / unorderedMapSearchTime - 100) << "%" << std::endl;

    std::cout << std::left << std::setw(20) << "Iterate (µs)"
              << std::setw(15) << mapIterateTime
              << std::setw(20) << unorderedMapIterateTime
              << std::setw(15) << (mapIterateTime * 100.0 / unorderedMapIterateTime - 100) << "%" << std::endl;

    std::cout << std::endl;
}

int main() {
    // Benchmark with different dataset sizes
    benchmarkSets(1000);
    benchmarkSets(10000);
    benchmarkSets(100000);

    benchmarkMaps(1000);
    benchmarkMaps(10000);
    benchmarkMaps(100000);

    return 0;
}
```

_Running benchmarks on different containers is like having a drag race between different car models—comparing their speed under identical conditions to see which one performs best for specific tasks._

## 😂 Set & Map Jokes

- I tried to use an unordered_set to keep track of my socks, but somehow they still ended up forming pairs.

- Why are programmers so bad at relationships? They keep trying to use map instead of multimap, so they can't handle it when the same key has multiple values.

- I told my friend I'd send him a map from my hike, but he was disappointed when I sent him a std::map instead of a geographical one. I guess he didn't like that it only showed the relationship between points and not the actual terrain.

- set and unordered_set walk into a bar. The bartender says, "We don't serve your type here!" To which unordered_set responds, "That's fine, I wasn't planning to stay in order anyway."

- What do you call it when you accidentally pass a map to a function expecting a set? A key-tastrophe!

## 📚 LeetCode Practice Problems

To reinforce today's set & map concepts, try these LeetCode problems:

### Easy:

- #217: Contains Duplicate (Set)
- #1: Two Sum (Map)
- #242: Valid Anagram (Map)
- #349: Intersection of Two Arrays (Set)
- #205: Isomorphic Strings (Map)

### Medium:

- #49: Group Anagrams (Map)
- #380: Insert Delete GetRandom O(1) (Set/Map combination)
- #347: Top K Frequent Elements (Map)
- #187: Repeated DNA Sequences (Set)
- #138: Copy List with Random Pointer (Map)

### Hard:

- #336: Palindrome Pairs (Map)
- #30: Substring with Concatenation of All Words (Map)
- #76: Minimum Window Substring (Map)
- #432: All O`one Data Structure (Map)
- #895: Maximum Frequency Stack (Map)

_These LeetCode problems are like a treasure hunt where maps and sets are your compass and GPS—equipping you with the right tools to navigate complex algorithmic challenges._

## 🔮 Tomorrow's Preview:

Tomorrow we'll dive into recursion fundamentals—the programming technique where a function calls itself. We'll explore how to think recursively, visualize recursive calls, identify base cases, and master recursive patterns that appear in many algorithms.

_From the deterministic world of containers to the elegant world of recursion—we'll see how functions can reference themselves to solve complex problems through simple, self-similar steps._

#DSAin45 #Day13 #CPlusPlus #STL #Containers #Sets #Maps
