# 🗺️ DAY 13: Sets & Maps in C++ STL - Power Tools for Efficient Data Management #DSAin45

Welcome to Day 13 of our DSA journey! Yesterday, we explored the inner workings of hash tables by building advanced implementations from scratch. Today, we'll harness the power of C++'s Standard Template Library (STL) to work with robust, pre-built container types that leverage these concepts.

_Think of yesterday as learning how to build an engine, and today as learning how to drive a high-performance car that has that engine under the hood._

## 🎯 Ordered vs Unordered: The Two Container Families

C++ STL provides us with two flavors of associative containers:

### 🌲 Ordered Containers (Tree-based)

- **Implementation**: Self-balancing binary search trees (typically Red-Black Trees)
- **Iteration Order**: Elements are always traversed in sorted order
- **Time Complexity**: O(log n) for insertions, deletions, and lookups
- **Examples**: `std::set`, `std::map`, `std::multiset`, `std::multimap`

### 🔍 Unordered Containers (Hash-based)

- **Implementation**: Hash tables (similar to what we built yesterday)
- **Iteration Order**: No guaranteed order (depends on hash function and bucket distribution)
- **Time Complexity**: O(1) average case, O(n) worst case for operations
- **Examples**: `std::unordered_set`, `std::unordered_map`, `std::unordered_multiset`, `std::unordered_multimap`

_Order vs. Chaos: Ordered containers are like a meticulously organized library where every book is perfectly arranged by topic and author. Unordered containers are like a magical library where books teleport instantly to your hand when you ask for them, but they're stored in seemingly random piles._

## 📚 Sets in C++ STL

Sets store unique elements with no duplicates allowed. They're perfect for membership testing and maintaining a collection of distinct items.

### 🌟 std::set - The Ordered Set

```cpp
#include <iostream>
#include <set>

int main() {
    // Create a set of integers
    std::set<int> orderedSet;

    // Insert elements
    orderedSet.insert(30);
    orderedSet.insert(10);
    orderedSet.insert(50);
    orderedSet.insert(20);
    orderedSet.insert(40);
    orderedSet.insert(10); // Duplicate - will be ignored

    // Print all elements (will be in ascending order)
    std::cout << "Ordered set elements: ";
    for (int num : orderedSet) {
        std::cout << num << " ";
    }
    // Output: 10 20 30 40 50 (sorted automatically)

    // Check if an element exists
    if (orderedSet.find(30) != orderedSet.end()) {
        std::cout << "\nFound 30 in the set";
    }

    // Get set size
    std::cout << "\nSize: " << orderedSet.size(); // 5

    // Erase an element
    orderedSet.erase(30);

    // Check lower and upper bounds
    auto lower = orderedSet.lower_bound(25); // Iterator to 30
    auto upper = orderedSet.upper_bound(25); // Iterator to 30

    return 0;
}
```

#### Key Operations and Their Complexity

| Operation       | Time Complexity | Description                              |
| --------------- | --------------- | ---------------------------------------- |
| `insert()`      | O(log n)        | Inserts an element                       |
| `find()`        | O(log n)        | Searches for an element                  |
| `erase()`       | O(log n)        | Removes an element                       |
| `count()`       | O(log n)        | Returns 1 if element exists, 0 otherwise |
| `lower_bound()` | O(log n)        | Returns iterator to first element >= key |
| `upper_bound()` | O(log n)        | Returns iterator to first element > key  |
| `size()`        | O(1)            | Returns number of elements               |
| `empty()`       | O(1)            | Checks if set is empty                   |
| `clear()`       | O(n)            | Removes all elements                     |

_A `std::set` is like a bouncer at an exclusive club who always keeps the guests in alphabetical order and never lets in someone who's already inside._

### 🔥 std::unordered_set - The Hash-Based Set

```cpp
#include <iostream>
#include <unordered_set>

int main() {
    // Create an unordered set of integers
    std::unordered_set<int> hashSet;

    // Insert elements
    hashSet.insert(30);
    hashSet.insert(10);
    hashSet.insert(50);
    hashSet.insert(20);
    hashSet.insert(40);
    hashSet.insert(10); // Duplicate - will be ignored

    // Print all elements (order not guaranteed)
    std::cout << "Unordered set elements: ";
    for (int num : hashSet) {
        std::cout << num << " ";
    }
    // Output might be: 40 20 50 30 10 (or any other order)

    // Check if an element exists
    if (hashSet.find(30) != hashSet.end()) {
        std::cout << "\nFound 30 in the set";
    }

    // Get set size
    std::cout << "\nSize: " << hashSet.size(); // 5

    // Erase an element
    hashSet.erase(30);

    // Get load factor
    std::cout << "\nLoad factor: " << hashSet.load_factor();

    // Get/set number of buckets
    std::cout << "\nBucket count: " << hashSet.bucket_count();
    hashSet.rehash(20); // Set to at least 20 buckets

    return 0;
}
```

#### Key Operations and Their Complexity

| Operation        | Average | Worst Case | Description                              |
| ---------------- | ------- | ---------- | ---------------------------------------- |
| `insert()`       | O(1)    | O(n)       | Inserts an element                       |
| `find()`         | O(1)    | O(n)       | Searches for an element                  |
| `erase()`        | O(1)    | O(n)       | Removes an element                       |
| `count()`        | O(1)    | O(n)       | Returns 1 if element exists, 0 otherwise |
| `size()`         | O(1)    | O(1)       | Returns number of elements               |
| `empty()`        | O(1)    | O(1)       | Checks if set is empty                   |
| `clear()`        | O(n)    | O(n)       | Removes all elements                     |
| `load_factor()`  | O(1)    | O(1)       | Returns average elements per bucket      |
| `bucket_count()` | O(1)    | O(1)       | Returns number of buckets                |
| `rehash()`       | O(n)    | O(n)       | Sets minimum bucket count                |

_An `unordered_set` is like a nightclub with teleportation pads—you can find anyone instantly, but when you ask them to line up for a photo, they appear in a seemingly random order._

### 🔄 Set Operations

Sets can be used to perform mathematical set operations:

```cpp
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

int main() {
    std::set<int> set1 = {1, 2, 3, 4, 5};
    std::set<int> set2 = {4, 5, 6, 7, 8};
    std::vector<int> result;

    // Union: all elements from both sets (duplicates included only once)
    std::set_union(set1.begin(), set1.end(),
                  set2.begin(), set2.end(),
                  std::back_inserter(result));
    // result = {1, 2, 3, 4, 5, 6, 7, 8}

    result.clear();

    // Intersection: elements that appear in both sets
    std::set_intersection(set1.begin(), set1.end(),
                         set2.begin(), set2.end(),
                         std::back_inserter(result));
    // result = {4, 5}

    result.clear();

    // Difference: elements in set1 but not in set2
    std::set_difference(set1.begin(), set1.end(),
                       set2.begin(), set2.end(),
                       std::back_inserter(result));
    // result = {1, 2, 3}

    result.clear();

    // Symmetric difference: elements in either set but not both
    std::set_symmetric_difference(set1.begin(), set1.end(),
                                 set2.begin(), set2.end(),
                                 std::back_inserter(result));
    // result = {1, 2, 3, 6, 7, 8}

    return 0;
}
```

## 🗺️ Maps in C++ STL

Maps are key-value associations that let you store and retrieve values based on unique keys.

### 🌟 std::map - The Ordered Map

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // Create a map with string keys and int values
    std::map<std::string, int> studentScores;

    // Insert elements - multiple ways
    studentScores["Alice"] = 95;
    studentScores.insert({"Bob", 89});
    studentScores.insert(std::make_pair("Charlie", 78));

    // Update a value
    studentScores["Bob"] = 91;

    // Access elements
    std::cout << "Alice's score: " << studentScores["Alice"] << std::endl;

    // Safely check if key exists before accessing
    if (studentScores.count("David") > 0) {
        std::cout << "David's score: " << studentScores["David"] << std::endl;
    } else {
        std::cout << "David not found" << std::endl;
    }

    // Safer access with .at() (throws exception if not found)
    try {
        std::cout << "Charlie's score: " << studentScores.at("Charlie") << std::endl;
        std::cout << "Eve's score: " << studentScores.at("Eve") << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // Iterate through the map (will be in sorted key order)
    std::cout << "All scores:" << std::endl;
    for (const auto& [name, score] : studentScores) {
        std::cout << name << ": " << score << std::endl;
    }

    // Erase an entry
    studentScores.erase("Bob");

    return 0;
}
```

#### Key Operations and Their Complexity

| Operation       | Time Complexity | Description                         |
| --------------- | --------------- | ----------------------------------- |
| `operator[]`    | O(log n)        | Access or insert by key             |
| `at()`          | O(log n)        | Access with bounds checking         |
| `insert()`      | O(log n)        | Insert a key-value pair             |
| `find()`        | O(log n)        | Find element by key                 |
| `erase()`       | O(log n)        | Remove element by key               |
| `count()`       | O(log n)        | Count occurrences of a key (0 or 1) |
| `lower_bound()` | O(log n)        | Iterator to first element >= key    |
| `upper_bound()` | O(log n)        | Iterator to first element > key     |
| `size()`        | O(1)            | Get number of elements              |
| `empty()`       | O(1)            | Check if empty                      |

_A `std::map` is like a dictionary that not only alphabetizes all words automatically but also teleports you directly to any definition when you speak the word._

### 🔥 std::unordered_map - The Hash-Based Map

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // Create an unordered map with string keys and int values
    std::unordered_map<std::string, int> menu;

    // Insert elements
    menu["Burger"] = 12;
    menu["Pizza"] = 15;
    menu["Salad"] = 8;
    menu["Pasta"] = 14;

    // Print all items (order not guaranteed)
    std::cout << "Menu items:" << std::endl;
    for (const auto& [item, price] : menu) {
        std::cout << item << ": $" << price << std::endl;
    }

    // Find an item
    auto it = menu.find("Pizza");
    if (it != menu.end()) {
        std::cout << "Pizza costs $" << it->second << std::endl;
    }

    // Check if item exists
    if (menu.count("Burger") > 0) {
        std::cout << "Burger is on the menu" << std::endl;
    }

    // Hash table specific operations
    std::cout << "Load factor: " << menu.load_factor() << std::endl;
    std::cout << "Bucket count: " << menu.bucket_count() << std::endl;

    // See which bucket contains a key
    std::cout << "'Pasta' is in bucket: " << menu.bucket("Pasta") << std::endl;

    // See how many elements are in a specific bucket
    size_t bucketIndex = menu.bucket("Pasta");
    std::cout << "Elements in bucket " << bucketIndex << ": "
              << menu.bucket_size(bucketIndex) << std::endl;

    return 0;
}
```

#### Key Operations and Their Complexity

| Operation        | Average | Worst Case | Description                         |
| ---------------- | ------- | ---------- | ----------------------------------- |
| `operator[]`     | O(1)    | O(n)       | Access or insert by key             |
| `at()`           | O(1)    | O(n)       | Access with bounds checking         |
| `insert()`       | O(1)    | O(n)       | Insert a key-value pair             |
| `find()`         | O(1)    | O(n)       | Find element by key                 |
| `erase()`        | O(1)    | O(n)       | Remove element by key               |
| `count()`        | O(1)    | O(n)       | Count occurrences of a key (0 or 1) |
| `size()`         | O(1)    | O(1)       | Get number of elements              |
| `bucket()`       | O(1)    | O(1)       | Get bucket for a key                |
| `load_factor()`  | O(1)    | O(1)       | Get average elements per bucket     |
| `bucket_count()` | O(1)    | O(1)       | Get number of buckets               |

_An `unordered_map` is like a restaurant with tables assigned by a host who uses your name to instantly calculate which table is yours—sometimes there are collisions and you need to sit at the next available table, but most of the time, you get seated immediately._

### 🧠 Choosing Between Map Types

```cpp
// Use std::map when:
std::map<int, std::string> orderedMap;
// 1. You need keys to be sorted
// 2. You need to find the closest key (lower/upper bound)
// 3. You need predictable iteration order
// 4. You're working with custom types and defining a comparison is easier than a hash

// Use std::unordered_map when:
std::unordered_map<int, std::string> hashMap;
// 1. You need the fastest possible lookups
// 2. You don't care about the order of elements
// 3. Your keys have a natural hash function
// 4. Memory overhead from the hash table is acceptable
```

## 🔄 Multi-Variants: Handling Duplicates

STL also provides containers that allow multiple elements with the same key:

### 🌴 std::multiset and std::multimap

```cpp
#include <iostream>
#include <map>
#include <set>

int main() {
    // Multiset allows duplicate elements
    std::multiset<int> numbers;
    numbers.insert(10);
    numbers.insert(20);
    numbers.insert(10); // Allowed!
    numbers.insert(30);
    numbers.insert(20); // Allowed!

    std::cout << "Multiset elements: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    // Output: 10 10 20 20 30 (sorted with duplicates)

    // Count occurrences
    std::cout << "\nCount of 10: " << numbers.count(10); // 2

    // Erase one occurrence
    auto it = numbers.find(10);
    if (it != numbers.end()) {
        numbers.erase(it); // Erases only one occurrence
    }

    // Erase all occurrences
    numbers.erase(20); // Removes all 20s

    // Multimap allows multiple values per key
    std::multimap<std::string, int> studentScores;
    studentScores.insert({"Alice", 95});
    studentScores.insert({"Bob", 89});
    studentScores.insert({"Alice", 91}); // Alice has multiple scores
    studentScores.insert({"Alice", 88});

    // Find all scores for a student
    std::cout << "\nAlice's scores: ";
    auto range = studentScores.equal_range("Alice");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << " ";
    }
    // Output: 88 91 95 (or similar, maintained in order)

    return 0;
}
```

### 🔍 std::unordered_multiset and std::unordered_multimap

```cpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>

int main() {
    // Unordered multiset allows duplicate elements with hash-based storage
    std::unordered_multiset<int> numbers;
    numbers.insert(10);
    numbers.insert(20);
    numbers.insert(10); // Allowed!
    numbers.insert(30);
    numbers.insert(20); // Allowed!

    std::cout << "Unordered multiset elements: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    // Output might be: 30 20 20 10 10 (or any other order)

    // Count occurrences
    std::cout << "\nCount of 10: " << numbers.count(10); // 2

    // Unordered multimap allows multiple values per key with hash-based storage
    std::unordered_multimap<std::string, int> courseEnrollments;
    courseEnrollments.insert({"CS101", 3001});
    courseEnrollments.insert({"MATH200", 2001});
    courseEnrollments.insert({"CS101", 3002});
    courseEnrollments.insert({"CS101", 3003});

    // Find all student IDs for a course
    std::cout << "\nStudents in CS101: ";
    auto range = courseEnrollments.equal_range("CS101");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << " ";
    }
    // Output may be: 3003 3002 3001 (or any order)

    return 0;
}
```

_Multi-containers are like parties where people with the same name are allowed in, but ordered ones make them stand in alphabetical order while unordered ones let them stand wherever they want._

## 🏗️ Custom Key Types and Hash Functions

To use your own types as keys, you need to properly define how they should be compared (for ordered containers) or hashed (for unordered containers).

### 🧩 Custom Type in Ordered Containers

```cpp
#include <iostream>
#include <map>
#include <set>

// Custom type
struct Person {
    std::string name;
    int age;

    // For std::set and std::map, define operator<
    bool operator<(const Person& other) const {
        // First compare by name
        if (name != other.name) {
            return name < other.name;
        }
        // If names are equal, compare by age
        return age < other.age;
    }
};

int main() {
    // Use Person as key in a set
    std::set<Person> people;

    people.insert({"Alice", 30});
    people.insert({"Bob", 25});
    people.insert({"Alice", 35}); // Different age, so it's a different person

    std::cout << "People in the set:" << std::endl;
    for (const auto& person : people) {
        std::cout << person.name << ", " << person.age << std::endl;
    }

    // Use Person as key in a map
    std::map<Person, std::string> departments;

    departments[{"Alice", 30}] = "Engineering";
    departments[{"Bob", 25}] = "Marketing";
    departments[{"Alice", 35}] = "Management";

    std::cout << "\nDepartments:" << std::endl;
    for (const auto& [person, dept] : departments) {
        std::cout << person.name << ", " << person.age << ": " << dept << std::endl;
    }

    return 0;
}
```

### 🔐 Custom Type in Unordered Containers

```cpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>

// Custom type
struct Employee {
    std::string id;
    std::string name;

    // Need operator== for unordered containers
    bool operator==(const Employee& other) const {
        return id == other.id; // Only compare by ID
    }
};

// Custom hash function (can be defined outside the class)
namespace std {
    template <>
    struct hash<Employee> {
        size_t operator()(const Employee& e) const {
            // Use the existing std::hash for string
            return hash<string>()(e.id);
        }
    };
}

int main() {
    // Now we can use Employee with unordered containers
    std::unordered_set<Employee> employees;

    employees.insert({"E123", "Alice"});
    employees.insert({"E456", "Bob"});
    employees.insert({"E789", "Charlie"});

    std::cout << "Employees:" << std::endl;
    for (const auto& emp : employees) {
        std::cout << emp.id << ": " << emp.name << std::endl;
    }

    // Look up by ID
    Employee searchKey{"E456", ""}; // Name doesn't matter for comparison
    if (employees.find(searchKey) != employees.end()) {
        std::cout << "\nFound employee E456" << std::endl;
    }

    // Using as key in unordered_map
    std::unordered_map<Employee, double> salaries;

    salaries[{"E123", "Alice"}] = 75000.0;
    salaries[{"E456", "Bob"}] = 82000.0;
    salaries[{"E789", "Charlie"}] = 67000.0;

    std::cout << "\nSalaries:" << std::endl;
    for (const auto& [emp, salary] : salaries) {
        std::cout << emp.id << " (" << emp.name << "): $" << salary << std::endl;
    }

    return 0;
}
```

_Creating a custom hash function is like giving a unique ID badge to each of your employees so they can instantly access the right areas without needing a full identity check._

### 💡 Alternative: Using Functors for Comparison and Hashing

```cpp
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

struct Person {
    std::string firstName;
    std::string lastName;
};

// Comparison functor for ordered map
struct PersonCompare {
    bool operator()(const Person& a, const Person& b) const {
        // Compare by last name first, then first name
        if (a.lastName != b.lastName) {
            return a.lastName < b.lastName;
        }
        return a.firstName < b.firstName;
    }
};

// Hash functor for unordered map
struct PersonHash {
    size_t operator()(const Person& p) const {
        // Combine hashes of first and last name
        size_t h1 = std::hash<std::string>{}(p.firstName);
        size_t h2 = std::hash<std::string>{}(p.lastName);
        return h1 ^ (h2 << 1); // Simple hash combine
    }
};

// Equality functor for unordered map
struct PersonEqual {
    bool operator()(const Person& a, const Person& b) const {
        return (a.firstName == b.firstName && a.lastName == b.lastName);
    }
};

int main() {
    // Using functors with ordered map
    std::map<Person, int, PersonCompare> orderedPeople;

    orderedPeople[{"John", "Smith"}] = 25;
    orderedPeople[{"Jane", "Doe"}] = 30;
    orderedPeople[{"John", "Doe"}] = 28;

    std::cout << "Ordered by last name, then first name:" << std::endl;
    for (const auto& [person, age] : orderedPeople) {
        std::cout << person.lastName << ", " << person.firstName
                  << ": " << age << std::endl;
    }

    // Using functors with unordered map
    std::unordered_map<
        Person,
        std::string,
        PersonHash,
        PersonEqual
    > unorderedPeople;

    unorderedPeople[{"Alice", "Johnson"}] = "Engineering";
    unorderedPeople[{"Bob", "Williams"}] = "Marketing";
    unorderedPeople[{"Carol", "Johnson"}] = "Finance";

    std::cout << "\nUnordered people:" << std::endl;
    for (const auto& [person, dept] : unorderedPeople) {
        std::cout << person.firstName << " " << person.lastName
                  << ": " << dept << std::endl;
    }

    return 0;
}
```

## 🚀 Performance Comparison

Let's benchmark the different container types to see their relative performance:

```cpp
#include <iostream>
#include <chrono>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <random>

// Helper function for timing
template<typename Func>
long long measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    const int DATA_SIZE = 100000;

    // Generate random data
    std::vector<int> data;
    data.reserve(DATA_SIZE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, DATA_SIZE * 10);

    for (int i = 0; i < DATA_SIZE; ++i) {
        data.push_back(dist(gen));
    }

    // Also create a vector of elements to lookup (half existing, half not)
    std::vector<int> lookupData;
    lookupData.reserve(DATA_SIZE);

    for (int i = 0; i < DATA_SIZE / 2; ++i) {
        lookupData.push_back(data[i]); // Existing elements
    }

    for (int i = 0; i < DATA_SIZE / 2; ++i) {
        lookupData.push_back(dist(gen) + DATA_SIZE * 10); // Likely non-existing
    }

    std::shuffle(lookupData.begin(), lookupData.end(), gen);

    // Benchmark std::set
    std::set<int> orderedSet;

    long long setInsertTime = measureTime([&]() {
        for (int value : data) {
            orderedSet.insert(value);
        }
    });

    long long setLookupTime = measureTime([&]() {
        for (int value : lookupData) {
            auto it = orderedSet.find(value);
            // Do something with it to prevent optimization
            if (it != orderedSet.end()) {
                volatile int x = *it;
            }
        }
    });

    // Benchmark std::unordered_set
    std::unordered_set<int> hashSet;

    long long unorderedSetInsertTime = measureTime([&]() {
        for (int value : data) {
            hashSet.insert(value);
        }
    });

    long long unorderedSetLookupTime = measureTime([&]() {
        for (int value : lookupData) {
            auto it = hashSet.find(value);
            // Do something with it to prevent optimization
            if (it != hashSet.end()) {
                volatile int x = *it;
            }
        }
    });

    // Benchmark std::map
    std::map<int, int> orderedMap;

    long long mapInsertTime = measureTime([&]() {
        for (int value : data) {
            orderedMap[value] = value;
        }
    });

    long long mapLookupTime = measureTime([&]() {
        for (int value : lookupData) {
            auto it = orderedMap.find(value);
            // Do something with it to prevent optimization
            if (it != orderedMap.end()) {
                volatile int x = it->second;
            }
        }
    });

    // Benchmark std::unordered_map
    std::unordered_map<int, int> hashMap;

    long long unorderedMapInsertTime = measureTime([&]() {
        for (int value : data) {
            hashMap[value] = value;
        }
    });

    long long unorderedMapLookupTime = measureTime([&]() {
        for (int value : lookupData) {
            auto it = hashMap.find(value);
            // Do something with it to prevent optimization
            if (it != hashMap.end()) {
                volatile int x = it->second;
            }
        }
    });

    // Print results
    std::cout << "PERFORMANCE COMPARISON (" << DATA_SIZE << " elements)" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Container          | Insert Time (μs) | Lookup Time (μs)" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "std::set           | " << std::setw(16) << setInsertTime
              << " | " << std::setw(16) << setLookupTime << std::endl;
    std::cout << "std::unordered_set | " << std::setw(16) << unorderedSetInsertTime
              << " | " << std::setw(16) << unorderedSetLookupTime << std::endl;
    std::cout << "std::map           | " << std::setw(16) << mapInsertTime
              << " | " << std::setw(16) << mapLookupTime << std::endl;
    std::cout << "std::unordered_map | " << std::setw(16) << unorderedMapInsertTime
              << " | " << std::setw(16) << unorderedMapLookupTime << std::endl;

    return 0;
}
```

Typical results might look like:

```
PERFORMANCE COMPARISON (100000 elements)
--------------------------------------------------
Container          | Insert Time (μs) | Lookup Time (μs)
--------------------------------------------------
std::set           |           64232 |           38755
std::unordered_set |           29876 |            8632
std::map           |           72569 |           42198
std::unordered_map |           33241 |           10128
```

🏎️ **Key Takeaways from Performance Comparison:**

1. **Hash-based containers** (`unordered_set`/`unordered_map`) are significantly faster for lookups, often 3-5x faster than tree-based ones.
2. **Insertion is also faster** with hash-based containers, though the difference is smaller than with lookups.
3. **Tree-based containers** (`set`/`map`) have more overhead but provide ordering guarantees.
4. The **difference is more pronounced** as the container size increases.

_Ordered containers are like playing "Twenty Questions" to find your item (divide and conquer with logarithmic steps), while unordered containers are like having a magic locator spell that takes you right to your item in one step (most of the time)._

## 🛠️ Practical Applications

### 1. Word Frequency Counter

```cpp
#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

int main() {
    // Use unordered_map for faster counting
    std::unordered_map<std::string, int> wordFreq;

    // Sample text (or read from file)
    std::string text = "To be or not to be that is the question "
                       "Whether tis nobler in the mind to suffer "
                       "The slings and arrows of outrageous fortune "
                       "Or to take arms against a sea of troubles "
                       "And by opposing end them";

    // Convert to lowercase and count words
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        // Remove punctuation and convert to lowercase
        word.erase(std::remove_if(word.begin(), word.end(),
                   [](char c) { return !std::isalpha(c); }), word.end());
        std::transform(word.begin(), word.end(), word.begin(),
                      [](char c) { return std::tolower(c); });

        if (!word.empty()) {
            wordFreq[word]++;
        }
    }

    // Transfer to map to get sorted output
    std::map<std::string, int> sortedFreq(wordFreq.begin(), wordFreq.end());

    std::cout << "Word frequencies (alphabetical order):" << std::endl;
    for (const auto& [word, count] : sortedFreq) {
        std::cout << word << ": " << count << std::endl;
    }

    // Sort by frequency (highest first)
    std::vector<std::pair<std::string, int>> freqVec(wordFreq.begin(), wordFreq.end());
    std::sort(freqVec.begin(), freqVec.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::cout << "\nWord frequencies (by frequency):" << std::endl;
    for (const auto& [word, count] : freqVec) {
        std::cout << word << ": " << count << std::endl;
    }

    return 0;
}
```

### 2. Symbol Table (Simple Compiler Implementation)

```cpp
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

enum class SymbolType {
    Variable,
    Function,
    Constant
};

struct Symbol {
    std::string name;
    SymbolType type;
    std::string dataType;
    int scope;
    size_t address; // Memory location

    Symbol(const std::string& n, SymbolType t, const std::string& dt, int s, size_t addr)
        : name(n), type(t), dataType(dt), scope(s), address(addr) {}
};

class SymbolTable {
private:
    std::unordered_map<std::string, std::vector<Symbol>> symbols;
    int currentScope = 0;
    size_t nextAddress = 0;

public:
    bool insert(const std::string& name, SymbolType type, const std::string& dataType) {
        // Check if symbol already exists in current scope
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            for (const auto& sym : it->second) {
                if (sym.scope == currentScope) {
                    return false; // Already defined in this scope
                }
            }
        }

        // Insert new symbol
        Symbol newSymbol(name, type, dataType, currentScope, nextAddress);
        symbols[name].push_back(newSymbol);

        // Update next available address (simplified)
        if (dataType == "int") {
            nextAddress += 4;
        } else if (dataType == "double") {
            nextAddress += 8;
        } else if (dataType == "char") {
            nextAddress += 1;
        } else {
            nextAddress += 8; // Default size
        }

        return true;
    }

    Symbol* lookup(const std::string& name) {
        auto it = symbols.find(name);
        if (it == symbols.end()) {
            return nullptr; // Symbol not found
        }

        // Search from innermost scope outward
        for (int scope = currentScope; scope >= 0; --scope) {
            for (auto& sym : it->second) {
                if (sym.scope == scope) {
                    return &sym;
                }
            }
        }

        return nullptr; // Not found in any accessible scope
    }

    void enterScope() {
        currentScope++;
    }

    void exitScope() {
        if (currentScope > 0) {
            currentScope--;
        }
    }

    void printTable() {
        std::cout << "SYMBOL TABLE:" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        std::cout << std::left << std::setw(15) << "Name"
                  << std::setw(10) << "Type"
                  << std::setw(10) << "Data Type"
                  << std::setw(10) << "Scope"
                  << std::setw(15) << "Address" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        for (const auto& [name, symbolList] : symbols) {
            for (const auto& sym : symbolList) {
                std::string typeStr;
                switch (sym.type) {
                    case SymbolType::Variable: typeStr = "Variable"; break;
                    case SymbolType::Function: typeStr = "Function"; break;
                    case SymbolType::Constant: typeStr = "Constant"; break;
                }

                std::cout << std::left << std::setw(15) << sym.name
                          << std::setw(10) << typeStr
                          << std::setw(10) << sym.dataType
                          << std::setw(10) << sym.scope
                          << std::setw(15) << "0x" << std::hex << sym.address << std::dec << std::endl;
            }
        }
    }
};

int main() {
    SymbolTable table;

    // Global scope
    table.insert("printf", SymbolType::Function, "int");
    table.insert("MAX_SIZE", SymbolType::Constant, "int");

    // Enter function scope
    table.enterScope();
    table.insert("x", SymbolType::Variable, "int");
    table.insert("y", SymbolType::Variable, "double");

    // Enter block scope
    table.enterScope();
    table.insert("z", SymbolType::Variable, "int");
    table.insert("x", SymbolType::Variable, "char"); // Shadows outer x

    // Print table
    table.printTable();

    // Lookup examples
    Symbol* sym1 = table.lookup("x");
    if (sym1) {
        std::cout << "\nLooking up 'x' found: " << sym1->name
                  << " (dataType: " << sym1->dataType
                  << ", scope: " << sym1->scope << ")" << std::endl;
    }

    // Exit inner scope
    table.exitScope();

    Symbol* sym2 = table.lookup("x");
    if (sym2) {
        std::cout << "After exiting scope, 'x' is: " << sym2->name
                  << " (dataType: " << sym2->dataType
                  << ", scope: " << sym2->scope << ")" << std::endl;
    }

    return 0;
}
```

### 3. Graph Adjacency List

```cpp
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

// Graph using STL containers
class Graph {
private:
    // Adjacency list using unordered_map for fast node lookup
    // and set for fast edge queries
    std::unordered_map<int, std::set<int>> adjacencyList;

public:
    // Add an edge to the graph
    void addEdge(int from, int to, bool bidirectional = true) {
        adjacencyList[from].insert(to);
        if (bidirectional) {
            adjacencyList[to].insert(from);
        }
    }

    // Remove an edge
    void removeEdge(int from, int to, bool bidirectional = true) {
        if (adjacencyList.find(from) != adjacencyList.end()) {
            adjacencyList[from].erase(to);
        }

        if (bidirectional && adjacencyList.find(to) != adjacencyList.end()) {
            adjacencyList[to].erase(from);
        }
    }

    // Check if edge exists
    bool hasEdge(int from, int to) const {
        auto it = adjacencyList.find(from);
        if (it != adjacencyList.end()) {
            return it->second.find(to) != it->second.end();
        }
        return false;
    }

    // Get all nodes
    std::vector<int> getNodes() const {
        std::vector<int> nodes;
        for (const auto& [node, _] : adjacencyList) {
            nodes.push_back(node);
        }
        return nodes;
    }

    // Get neighbors of a node
    std::vector<int> getNeighbors(int node) const {
        std::vector<int> neighbors;
        auto it = adjacencyList.find(node);
        if (it != adjacencyList.end()) {
            neighbors.assign(it->second.begin(), it->second.end());
        }
        return neighbors;
    }

    // Print the graph
    void print() const {
        std::cout << "Graph Adjacency List:" << std::endl;
        for (const auto& [node, neighbors] : adjacencyList) {
            std::cout << node << " -> ";
            for (int neighbor : neighbors) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

    // Breadth-first search
    std::vector<int> bfs(int start) const {
        std::vector<int> result;
        std::unordered_map<int, bool> visited;
        std::queue<int> queue;

        visited[start] = true;
        queue.push(start);

        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();
            result.push_back(current);

            auto it = adjacencyList.find(current);
            if (it != adjacencyList.end()) {
                for (int neighbor : it->second) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        queue.push(neighbor);
                    }
                }
            }
        }

        return result;
    }
};

int main() {
    Graph g;

    // Add edges
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    // Print the graph
    g.print();

    // Check if edges exist
    std::cout << "\nEdge 1->3 exists: " << (g.hasEdge(1, 3) ? "Yes" : "No") << std::endl;
    std::cout << "Edge 2->4 exists: " << (g.hasEdge(2, 4) ? "Yes" : "No") << std::endl;

    // Remove an edge
    g.removeEdge(1, 4);
    std::cout << "\nAfter removing edge 1->4:" << std::endl;
    g.print();

    // BFS traversal
    std::cout << "\nBFS traversal starting from node 0:" << std::endl;
    std::vector<int> bfsResult = g.bfs(0);
    for (int node : bfsResult) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### 4. LRU Cache Implementation

```cpp
#include <iostream>
#include <unordered_map>
#include <list>

template<typename K, typename V>
class LRUCache {
private:
    // Maximum size of the cache
    size_t capacity;

    // List to maintain LRU order: most recently used at front
    std::list<std::pair<K, V>> items;

    // Map for O(1) lookup: key -> iterator to list node
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache;

public:
    LRUCache(size_t size) : capacity(size) {}

    // Get value associated with key and move to front (most recently used)
    bool get(const K& key, V& value) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return false; // Key not found
        }

        // Move to front of list (mark as most recently used)
        items.splice(items.begin(), items, it->second);
        value = it->second->second;
        return true;
    }

    // Insert or update key-value pair
    void put(const K& key, const V& value) {
        // Check if key already exists
        auto it = cache.find(key);
        if (it != cache.end()) {
            // Update existing value
            it->second->second = value;
            // Move to front (most recently used)
            items.splice(items.begin(), items, it->second);
            return;
        }

        // If cache is full, remove least recently used item (back of list)
        if (items.size() == capacity) {
            auto last = items.back();
            cache.erase(last.first);
            items.pop_back();
        }

        // Insert new item at front
        items.push_front({key, value});
        cache[key] = items.begin();
    }

    size_t size() const {
        return items.size();
    }

    bool empty() const {
        return items.empty();
    }

    void clear() {
        items.clear();
        cache.clear();
    }

    // Print all items in order (most recently used first)
    void print() const {
        std::cout << "LRU Cache Contents (most recent first):" << std::endl;
        for (const auto& [key, value] : items) {
            std::cout << key << " -> " << value << std::endl;
        }
    }
};

int main() {
    // Create an LRU cache with capacity 3
    LRUCache<std::string, int> cache(3);

    // Add some items
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("c", 3);

    cache.print();

    // Access 'a' (makes it most recently used)
    int value;
    if (cache.get("a", value)) {
        std::cout << "\nGot 'a': " << value << std::endl;
    }

    cache.print();

    // Add a new item, should evict 'b' (least recently used)
    cache.put("d", 4);

    std::cout << "\nAfter adding 'd' (should evict 'b'):" << std::endl;
    cache.print();

    // Try to access 'b' (should be gone)
    if (!cache.get("b", value)) {
        std::cout << "\n'b' no longer in cache" << std::endl;
    }

    return 0;
}
```

### 5. Using Custom Comparators for Priority

```cpp
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <vector>

// Task with priority
struct Task {
    std::string name;
    int priority;
    std::string description;

    Task(const std::string& n, int p, const std::string& d)
        : name(n), priority(p), description(d) {}
};

// Custom comparator for tasks
struct TaskComparator {
    bool operator()(const Task& a, const Task& b) const {
        // Higher priority first (descending order)
        if (a.priority != b.priority) {
            return a.priority > b.priority;
        }
        // If priorities are equal, sort alphabetically
        return a.name < b.name;
    }
};

int main() {
    // Using std::set with custom comparator as a priority queue
    std::set<Task, TaskComparator> taskQueue;

    // Add tasks
    taskQueue.insert(Task("Write report", 3, "Complete quarterly report"));
    taskQueue.insert(Task("Fix bug", 5, "High priority bug in login system"));
    taskQueue.insert(Task("Update database", 2, "Apply schema changes"));
    taskQueue.insert(Task("Review PR", 4, "Review pull request from team"));
    taskQueue.insert(Task("Meeting", 3, "Weekly team meeting"));

    // Process tasks in priority order
    std::cout << "Processing tasks by priority:" << std::endl;
    for (const auto& task : taskQueue) {
        std::cout << "Priority " << task.priority << ": "
                  << task.name << " - " << task.description << std::endl;
    }

    // Alternative: using std::priority_queue
    std::cout << "\nUsing std::priority_queue:" << std::endl;
    std::priority_queue<
        Task,
        std::vector<Task>,
        std::function<bool(const Task&, const Task&)>
    > pq(
        [](const Task& a, const Task& b) {
            // Note: priority_queue uses reverse comparison logic
            if (a.priority != b.priority) {
                return a.priority < b.priority; // Higher priority first
            }
            return a.name > b.name; // Alphabetical order
        }
    );

    // Add tasks
    pq.push(Task("Write report", 3, "Complete quarterly report"));
    pq.push(Task("Fix bug", 5, "High priority bug in login system"));
    pq.push(Task("Update database", 2, "Apply schema changes"));
    pq.push(Task("Review PR", 4, "Review pull request from team"));
    pq.push(Task("Meeting", 3, "Weekly team meeting"));

    // Process tasks
    while (!pq.empty()) {
        const Task& task = pq.top();
        std::cout << "Priority " << task.priority << ": "
                  << task.name << " - " << task.description << std::endl;
        pq.pop();
    }

    return 0;
}
```

## 🎭 Container Jokes & Analogies

1. **Ordered vs. Unordered**: An ordered map is like a librarian who meticulously alphabetizes every book. An unordered map is like a librarian who can teleport to any book instantly but refuses to organize them in any particular order.

2. **Hash Table Rehashing**: Unordered containers rehashing is like when you realize your closet is too small, so you buy a bigger one and spend the weekend reorganizing all your clothes.

3. **Set Union**: Performing a set union is like organizing a wedding reception where two families need to be seated, but you can only have one aunt Mildred even though both sides have one.

4. **Multiset**: A multiset is like a Pokemon card collection where you're specifically trying to get duplicates.

5. **Memory Usage**: An unordered_map's memory usage is like a taxi fare—usually reasonable, but can surge unexpectedly when more buckets are needed.

6. **Map vs. Unordered_map**: A map is like taking the stairs—reliable, predictable, but slower. An unordered_map is like taking the elevator—usually faster, but occasionally breaks down and needs maintenance (rehashing).

7. **Tree Balancing**: A balanced tree in an ordered container is like a carefully stacked Jenga tower that rearranges itself whenever it gets wobbly.

8. **Cache Miss**: Looking up a key that doesn't exist in a map is like asking for directions to a place that doesn't exist—you still have to wait for the person to think about it before telling you they can't help.

## 🧩 Practice Problems

### Easy:

1. **Two Sum** (LeetCode #1)

   - Given an array of integers and a target, find two numbers that add up to the target.
   - Perfect application for `unordered_map` to track values.

2. **Contains Duplicate** (LeetCode #217)

   - Determine if an array has any duplicate values.
   - Uses `unordered_set` for fast lookup.

3. **Valid Anagram** (LeetCode #242)
   - Check if two strings are anagrams of each other.
   - Can use `unordered_map` to count character frequencies.

### Medium:

1. **Group Anagrams** (LeetCode #49)

   - Group strings that are anagrams of each other.
   - Uses `unordered_map` with clever key creation.

2. **Top K Frequent Elements** (LeetCode #347)

   - Find the k most frequent elements in an array.
   - Combines `unordered_map` with `priority_queue` or `map`.

3. **LRU Cache** (LeetCode #146)
   - Implement a Least Recently Used (LRU) cache.
   - Similar to our example using `list` and `unordered_map`.

### Hard:

1. **Minimum Window Substring** (LeetCode #76)

   - Find the minimum substring that contains all characters from a target string.
   - Uses `unordered_map` for character frequency tracking.

2. **Longest Consecutive Sequence** (LeetCode #128)

   - Find the length of the longest consecutive sequence in an array.
   - Clever use of `unordered_set` for O(n) solution.

3. **All O(1) Data Structure** (LeetCode #432)
   - Design a data structure supporting all operations in O(1) time.
   - Combines multiple STL containers for efficient operations.

## 🔮 Tomorrow's Preview

Tomorrow, we'll wrap up our first three weeks with a Weekly Challenge and Recap (Day 14). We'll solve some integrated problems that combine all the data structures we've learned so far. We'll also review key performance characteristics and best practices for choosing the right container for different scenarios.

_It's like taking all the LEGO pieces we've been studying and building something awesome with them!_

#DSAin45 #Day13 #CPlusPlus #DataStructures #STL #Maps #Sets #HashTables
