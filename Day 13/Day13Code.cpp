// Day 13: Sets & Maps in C++ STL #DSAin45
// Comprehensive implementation and demonstrations of Set and Map containers

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include <random>
#include <functional>
#include <fstream>
#include <sstream>
#include <cstdlib>

// STL containers we'll be exploring
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

// For multicontainers
#include <utility> // for std::pair

// For custom comparators and functors
#include <queue>

// ===== HELPER FUNCTIONS =====

// Helper for timing operations
template <typename Func>
long long measureExecutionTime(Func func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// ===== SET DEMONSTRATIONS =====

void setBasics()
{
    std::cout << "\n===== SET BASICS =====" << std::endl;

    // Create an ordered set
    std::set<int> orderedSet;

    // Insert elements
    orderedSet.insert(30);
    orderedSet.insert(10);
    orderedSet.insert(50);
    orderedSet.insert(20);
    orderedSet.insert(40);
    orderedSet.insert(10); // Duplicate - will be ignored

    // Print the set (will be in ascending order)
    std::cout << "Ordered set elements: ";
    for (int num : orderedSet)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Find an element
    auto it = orderedSet.find(30);
    if (it != orderedSet.end())
    {
        std::cout << "Found " << *it << " in the set" << std::endl;
    }

    // Check if element exists
    if (orderedSet.count(10) > 0)
    {
        std::cout << "10 exists in the set" << std::endl;
    }

    // Lower and upper bounds
    auto lower = orderedSet.lower_bound(25);
    auto upper = orderedSet.upper_bound(25);

    std::cout << "Lower bound of 25: " << *lower << std::endl;
    std::cout << "Upper bound of 25: " << *upper << std::endl;

    // Size and other operations
    std::cout << "Set size: " << orderedSet.size() << std::endl;
    std::cout << "Is empty: " << (orderedSet.empty() ? "Yes" : "No") << std::endl;

    // Erase elements
    orderedSet.erase(30);
    std::cout << "After erasing 30: ";
    for (int num : orderedSet)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Clear the set
    orderedSet.clear();
    std::cout << "After clearing, size: " << orderedSet.size() << std::endl;
}

void unorderedSetBasics()
{
    std::cout << "\n===== UNORDERED SET BASICS =====" << std::endl;

    // Create an unordered set
    std::unordered_set<int> hashSet;

    // Insert elements
    hashSet.insert(30);
    hashSet.insert(10);
    hashSet.insert(50);
    hashSet.insert(20);
    hashSet.insert(40);
    hashSet.insert(10); // Duplicate - will be ignored

    // Print the set (order not guaranteed)
    std::cout << "Unordered set elements: ";
    for (int num : hashSet)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Find an element
    auto it = hashSet.find(30);
    if (it != hashSet.end())
    {
        std::cout << "Found " << *it << " in the set" << std::endl;
    }

    // Check if element exists
    if (hashSet.count(10) > 0)
    {
        std::cout << "10 exists in the set" << std::endl;
    }

    // Size and other operations
    std::cout << "Set size: " << hashSet.size() << std::endl;
    std::cout << "Is empty: " << (hashSet.empty() ? "Yes" : "No") << std::endl;

    // Hash table specific operations
    std::cout << "Load factor: " << hashSet.load_factor() << std::endl;
    std::cout << "Bucket count: " << hashSet.bucket_count() << std::endl;

    // Erase elements
    hashSet.erase(30);
    std::cout << "After erasing 30: ";
    for (int num : hashSet)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Clear the set
    hashSet.clear();
    std::cout << "After clearing, size: " << hashSet.size() << std::endl;
}

void setOperations()
{
    std::cout << "\n===== SET OPERATIONS =====" << std::endl;

    // Create two sets
    std::set<int> set1 = {1, 2, 3, 4, 5};
    std::set<int> set2 = {4, 5, 6, 7, 8};

    // Print initial sets
    std::cout << "Set 1: ";
    for (int num : set1)
        std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Set 2: ";
    for (int num : set2)
        std::cout << num << " ";
    std::cout << std::endl;

    // Union
    std::vector<int> unionResult;
    std::set_union(set1.begin(), set1.end(),
                   set2.begin(), set2.end(),
                   std::back_inserter(unionResult));

    std::cout << "Union: ";
    for (int num : unionResult)
        std::cout << num << " ";
    std::cout << std::endl;

    // Intersection
    std::vector<int> intersectionResult;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::back_inserter(intersectionResult));

    std::cout << "Intersection: ";
    for (int num : intersectionResult)
        std::cout << num << " ";
    std::cout << std::endl;

    // Difference (set1 - set2)
    std::vector<int> differenceResult;
    std::set_difference(set1.begin(), set1.end(),
                        set2.begin(), set2.end(),
                        std::back_inserter(differenceResult));

    std::cout << "Difference (set1 - set2): ";
    for (int num : differenceResult)
        std::cout << num << " ";
    std::cout << std::endl;

    // Symmetric difference
    std::vector<int> symmetricDiffResult;
    std::set_symmetric_difference(set1.begin(), set1.end(),
                                  set2.begin(), set2.end(),
                                  std::back_inserter(symmetricDiffResult));

    std::cout << "Symmetric difference: ";
    for (int num : symmetricDiffResult)
        std::cout << num << " ";
    std::cout << std::endl;
}

void multisetDemo()
{
    std::cout << "\n===== MULTISET DEMO =====" << std::endl;

    // Create a multiset
    std::multiset<int> numbers;

    // Insert elements (including duplicates)
    numbers.insert(10);
    numbers.insert(20);
    numbers.insert(10); // Allowed in multiset!
    numbers.insert(30);
    numbers.insert(20); // Allowed in multiset!

    std::cout << "Multiset elements: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Count occurrences
    std::cout << "Count of 10: " << numbers.count(10) << std::endl;
    std::cout << "Count of 20: " << numbers.count(20) << std::endl;
    std::cout << "Count of 30: " << numbers.count(30) << std::endl;

    // Erase one occurrence of an element
    auto it = numbers.find(10);
    if (it != numbers.end())
    {
        numbers.erase(it); // Erases only this occurrence
    }

    std::cout << "After erasing one occurrence of 10: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Erase all occurrences of an element
    numbers.erase(20); // Removes all 20s

    std::cout << "After erasing all occurrences of 20: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Equal range (all elements with a specific value)
    numbers.insert(10);
    numbers.insert(10);

    auto range = numbers.equal_range(10);
    std::cout << "Equal range of 10 contains: ";
    for (auto it = range.first; it != range.second; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void unorderedMultisetDemo()
{
    std::cout << "\n===== UNORDERED MULTISET DEMO =====" << std::endl;

    // Create an unordered multiset
    std::unordered_multiset<int> numbers;

    // Insert elements (including duplicates)
    numbers.insert(10);
    numbers.insert(20);
    numbers.insert(10); // Allowed in multiset!
    numbers.insert(30);
    numbers.insert(20); // Allowed in multiset!

    std::cout << "Unordered multiset elements: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Count occurrences
    std::cout << "Count of 10: " << numbers.count(10) << std::endl;
    std::cout << "Count of 20: " << numbers.count(20) << std::endl;
    std::cout << "Count of 30: " << numbers.count(30) << std::endl;

    // Hash-specific data
    std::cout << "Load factor: " << numbers.load_factor() << std::endl;
    std::cout << "Bucket count: " << numbers.bucket_count() << std::endl;

    // Equal range (all elements with a specific value)
    auto range = numbers.equal_range(10);
    std::cout << "Equal range of 10 contains: ";
    for (auto it = range.first; it != range.second; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// ===== MAP DEMONSTRATIONS =====

void mapBasics()
{
    std::cout << "\n===== MAP BASICS =====" << std::endl;

    // Create a map
    std::map<std::string, int> studentScores;

    // Insert elements
    studentScores["Alice"] = 95;
    studentScores.insert({"Bob", 89});
    studentScores.insert(std::make_pair("Charlie", 78));

    // Access elements
    std::cout << "Alice's score: " << studentScores["Alice"] << std::endl;

    // Update a value
    studentScores["Bob"] = 91;
    std::cout << "Bob's updated score: " << studentScores["Bob"] << std::endl;

    // Check if key exists before accessing
    if (studentScores.count("David") > 0)
    {
        std::cout << "David's score: " << studentScores["David"] << std::endl;
    }
    else
    {
        std::cout << "David not found in the map" << std::endl;
    }

    // Safe access with at() (throws exception if not found)
    try
    {
        std::cout << "Charlie's score: " << studentScores.at("Charlie") << std::endl;
        std::cout << "Eve's score: " << studentScores.at("Eve") << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Iterate through all entries
    std::cout << "All students:" << std::endl;
    for (const auto &[name, score] : studentScores)
    {
        std::cout << name << ": " << score << std::endl;
    }

    // Find specific entry
    auto it = studentScores.find("Bob");
    if (it != studentScores.end())
    {
        std::cout << "Found " << it->first << " with score " << it->second << std::endl;
    }

    // Erase an entry
    studentScores.erase("Bob");

    // Lower and upper bounds
    auto lower = studentScores.lower_bound("B");
    auto upper = studentScores.upper_bound("C");

    std::cout << "Lower bound of 'B': " << lower->first << std::endl;
    std::cout << "Upper bound of 'C': " << upper->first << std::endl;

    // Size
    std::cout << "Map size: " << studentScores.size() << std::endl;
}

void unorderedMapBasics()
{
    std::cout << "\n===== UNORDERED MAP BASICS =====" << std::endl;

    // Create an unordered map
    std::unordered_map<std::string, int> menu;

    // Insert elements
    menu["Burger"] = 12;
    menu.insert({"Pizza", 15});
    menu.insert(std::make_pair("Salad", 8));
    menu["Pasta"] = 14;

    // Access elements
    std::cout << "Burger price: $" << menu["Burger"] << std::endl;

    // Update a value
    menu["Pizza"] = 16;
    std::cout << "Updated pizza price: $" << menu["Pizza"] << std::endl;

    // Check if key exists
    if (menu.count("Sushi") > 0)
    {
        std::cout << "Sushi price: $" << menu["Sushi"] << std::endl;
    }
    else
    {
        std::cout << "Sushi not on the menu" << std::endl;
    }

    // Iterate through all entries (order not guaranteed)
    std::cout << "Full menu:" << std::endl;
    for (const auto &[item, price] : menu)
    {
        std::cout << item << ": $" << price << std::endl;
    }

    // Hash table specific operations
    std::cout << "Load factor: " << menu.load_factor() << std::endl;
    std::cout << "Bucket count: " << menu.bucket_count() << std::endl;

    // Find which bucket contains a key
    std::cout << "'Pasta' is in bucket: " << menu.bucket("Pasta") << std::endl;

    // Count elements in a specific bucket
    size_t bucketIndex = menu.bucket("Pasta");
    std::cout << "Elements in bucket " << bucketIndex << ": "
              << menu.bucket_size(bucketIndex) << std::endl;

    // Erase an item
    menu.erase("Salad");

    // Size
    std::cout << "Menu size after removing salad: " << menu.size() << std::endl;
}

void multimapDemo()
{
    std::cout << "\n===== MULTIMAP DEMO =====" << std::endl;

    // Create a multimap
    std::multimap<std::string, int> studentScores;

    // Insert elements (including duplicates with same key)
    studentScores.insert({"Alice", 95});
    studentScores.insert({"Bob", 89});
    studentScores.insert({"Alice", 91}); // Alice has multiple scores
    studentScores.insert({"Bob", 84});
    studentScores.insert({"Alice", 88});

    // Count entries with specific key
    std::cout << "Alice has " << studentScores.count("Alice") << " scores" << std::endl;

    // Cannot use operator[] with multimap because it's ambiguous
    // studentScores["Alice"] = 100; // This would not compile

    // Accessing all values for a key
    std::cout << "Alice's scores: ";
    auto range = studentScores.equal_range("Alice");
    for (auto it = range.first; it != range.second; ++it)
    {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    // Iterate through all entries (sorted by key)
    std::cout << "All scores:" << std::endl;
    for (const auto &[name, score] : studentScores)
    {
        std::cout << name << ": " << score << std::endl;
    }

    // Erase one entry
    auto it = studentScores.find("Bob");
    if (it != studentScores.end())
    {
        studentScores.erase(it); // Removes just one entry
    }

    // Erase all entries with a specific key
    studentScores.erase("Alice"); // Removes all Alice's scores

    std::cout << "After erasures, size: " << studentScores.size() << std::endl;
}

void unorderedMultimapDemo()
{
    std::cout << "\n===== UNORDERED MULTIMAP DEMO =====" << std::endl;

    // Create an unordered multimap
    std::unordered_multimap<std::string, int> courseEnrollments;

    // Insert elements (including duplicates with same key)
    courseEnrollments.insert({"CS101", 3001});
    courseEnrollments.insert({"MATH200", 2001});
    courseEnrollments.insert({"CS101", 3002});
    courseEnrollments.insert({"CS101", 3003});
    courseEnrollments.insert({"PHYS101", 4001});

    // Count entries with specific key
    std::cout << "CS101 has " << courseEnrollments.count("CS101") << " students" << std::endl;

    // Accessing all values for a key
    std::cout << "Students in CS101: ";
    auto range = courseEnrollments.equal_range("CS101");
    for (auto it = range.first; it != range.second; ++it)
    {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    // Iterate through all entries (order not guaranteed)
    std::cout << "All enrollments:" << std::endl;
    for (const auto &[course, studentId] : courseEnrollments)
    {
        std::cout << course << ": " << studentId << std::endl;
    }

    // Hash-specific operations
    std::cout << "Load factor: " << courseEnrollments.load_factor() << std::endl;
    std::cout << "Bucket count: " << courseEnrollments.bucket_count() << std::endl;
}

// ===== CUSTOM KEY TYPES =====

// Example for ordered containers (set, map)
struct Person
{
    std::string name;
    int age;

    // For std::set and std::map, define operator<
    bool operator<(const Person &other) const
    {
        // First compare by name
        if (name != other.name)
        {
            return name < other.name;
        }
        // If names are equal, compare by age
        return age < other.age;
    }
};

void orderedCustomKeyDemo()
{
    std::cout << "\n===== ORDERED CUSTOM KEY DEMO =====" << std::endl;

    // Use Person as key in a set
    std::set<Person> people;

    people.insert({"Alice", 30});
    people.insert({"Bob", 25});
    people.insert({"Alice", 35}); // Different age, so it's a different person

    std::cout << "People in the set:" << std::endl;
    for (const auto &person : people)
    {
        std::cout << person.name << ", " << person.age << std::endl;
    }

    // Use Person as key in a map
    std::map<Person, std::string> departments;

    departments[{"Alice", 30}] = "Engineering";
    departments[{"Bob", 25}] = "Marketing";
    departments[{"Alice", 35}] = "Management";

    std::cout << "\nDepartments:" << std::endl;
    for (const auto &[person, dept] : departments)
    {
        std::cout << person.name << ", " << person.age << ": " << dept << std::endl;
    }
}

// Example for unordered containers (unordered_set, unordered_map)
struct Employee
{
    std::string id;
    std::string name;

    // Need operator== for unordered containers
    bool operator==(const Employee &other) const
    {
        return id == other.id; // Only compare by ID
    }
};

// Custom hash function defined in the std namespace
namespace std
{
    template <>
    struct hash<Employee>
    {
        size_t operator()(const Employee &e) const
        {
            // Use the existing std::hash for string
            return hash<string>()(e.id);
        }
    };
}

void unorderedCustomKeyDemo()
{
    std::cout << "\n===== UNORDERED CUSTOM KEY DEMO =====" << std::endl;

    // Use Employee with unordered_set
    std::unordered_set<Employee> employees;

    employees.insert({"E123", "Alice"});
    employees.insert({"E456", "Bob"});
    employees.insert({"E789", "Charlie"});

    std::cout << "Employees in the set:" << std::endl;
    for (const auto &emp : employees)
    {
        std::cout << emp.id << ": " << emp.name << std::endl;
    }

    // Look up by ID
    Employee searchKey{"E456", ""}; // Name doesn't matter for comparison
    if (employees.find(searchKey) != employees.end())
    {
        std::cout << "Found employee E456" << std::endl;
    }

    // Use Employee as key in unordered_map
    std::unordered_map<Employee, double> salaries;

    salaries[{"E123", "Alice"}] = 75000.0;
    salaries[{"E456", "Bob"}] = 82000.0;
    salaries[{"E789", "Charlie"}] = 67000.0;

    std::cout << "\nSalaries:" << std::endl;
    for (const auto &[emp, salary] : salaries)
    {
        std::cout << emp.id << " (" << emp.name << "): $" << salary << std::endl;
    }
}

// Using functors for comparison and hashing
struct Person2
{
    std::string firstName;
    std::string lastName;
};

// Comparison functor for ordered map
struct PersonCompare
{
    bool operator()(const Person2 &a, const Person2 &b) const
    {
        // Compare by last name first, then first name
        if (a.lastName != b.lastName)
        {
            return a.lastName < b.lastName;
        }
        return a.firstName < b.firstName;
    }
};

// Hash functor for unordered map
struct PersonHash
{
    size_t operator()(const Person2 &p) const
    {
        // Combine hashes of first and last name
        size_t h1 = std::hash<std::string>{}(p.firstName);
        size_t h2 = std::hash<std::string>{}(p.lastName);
        return h1 ^ (h2 << 1); // Simple hash combine
    }
};

// Equality functor for unordered map
struct PersonEqual
{
    bool operator()(const Person2 &a, const Person2 &b) const
    {
        return (a.firstName == b.firstName && a.lastName == b.lastName);
    }
};

void functorDemo()
{
    std::cout << "\n===== FUNCTOR DEMO =====" << std::endl;

    // Using functors with ordered map
    std::map<Person2, int, PersonCompare> orderedPeople;

    orderedPeople[{"John", "Smith"}] = 25;
    orderedPeople[{"Jane", "Doe"}] = 30;
    orderedPeople[{"John", "Doe"}] = 28;

    std::cout << "Ordered by last name, then first name:" << std::endl;
    for (const auto &[person, age] : orderedPeople)
    {
        std::cout << person.lastName << ", " << person.firstName
                  << ": " << age << std::endl;
    }

    // Using functors with unordered map
    std::unordered_map<
        Person2,
        std::string,
        PersonHash,
        PersonEqual>
        unorderedPeople;

    unorderedPeople[{"Alice", "Johnson"}] = "Engineering";
    unorderedPeople[{"Bob", "Williams"}] = "Marketing";
    unorderedPeople[{"Carol", "Johnson"}] = "Finance";

    std::cout << "\nUnordered people:" << std::endl;
    for (const auto &[person, dept] : unorderedPeople)
    {
        std::cout << person.firstName << " " << person.lastName
                  << ": " << dept << std::endl;
    }
}

// ===== PERFORMANCE COMPARISONS =====

void performanceComparison()
{
    std::cout << "\n===== PERFORMANCE COMPARISON =====" << std::endl;

    const int DATA_SIZE = 100000;

    // Generate random data
    std::vector<int> data;
    data.reserve(DATA_SIZE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, DATA_SIZE * 10);

    for (int i = 0; i < DATA_SIZE; ++i)
    {
        data.push_back(dist(gen));
    }

    // Create lookup data
    std::vector<int> lookupData;
    lookupData.reserve(DATA_SIZE);

    for (int i = 0; i < DATA_SIZE / 2; ++i)
    {
        lookupData.push_back(data[i]); // Existing elements
    }

    for (int i = 0; i < DATA_SIZE / 2; ++i)
    {
        lookupData.push_back(dist(gen) + DATA_SIZE * 10); // Likely non-existing
    }

    std::shuffle(lookupData.begin(), lookupData.end(), gen);

    // Benchmark std::set
    std::set<int> orderedSet;

    long long setInsertTime = measureExecutionTime([&]()
                                                   {
        for (int value : data) {
            orderedSet.insert(value);
        } });

    long long setLookupTime = measureExecutionTime([&]()
                                                   {
        for (int value : lookupData) {
            auto it = orderedSet.find(value);
            // Do something with it to prevent optimization
            if (it != orderedSet.end()) {
                volatile int x = *it;
            }
        } });

    // Benchmark std::unordered_set
    std::unordered_set<int> hashSet;

    long long unorderedSetInsertTime = measureExecutionTime([&]()
                                                            {
        for (int value : data) {
            hashSet.insert(value);
        } });

    long long unorderedSetLookupTime = measureExecutionTime([&]()
                                                            {
        for (int value : lookupData) {
            auto it = hashSet.find(value);
            // Do something with it to prevent optimization
            if (it != hashSet.end()) {
                volatile int x = *it;
            }
        } });

    // Benchmark std::map
    std::map<int, int> orderedMap;

    long long mapInsertTime = measureExecutionTime([&]()
                                                   {
        for (int value : data) {
            orderedMap[value] = value;
        } });

    long long mapLookupTime = measureExecutionTime([&]()
                                                   {
        for (int value : lookupData) {
            auto it = orderedMap.find(value);
            // Do something with it to prevent optimization
            if (it != orderedMap.end()) {
                volatile int x = it->second;
            }
        } });

    // Benchmark std::unordered_map
    std::unordered_map<int, int> hashMap;

    long long unorderedMapInsertTime = measureExecutionTime([&]()
                                                            {
        for (int value : data) {
            hashMap[value] = value;
        } });

    long long unorderedMapLookupTime = measureExecutionTime([&]()
                                                            {
        for (int value : lookupData) {
            auto it = hashMap.find(value);
            // Do something with it to prevent optimization
            if (it != hashMap.end()) {
                volatile int x = it->second;
            }
        } });

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

    // Calculate speedup
    double setLookupSpeedup = static_cast<double>(setLookupTime) / unorderedSetLookupTime;
    double mapLookupSpeedup = static_cast<double>(mapLookupTime) / unorderedMapLookupTime;

    std::cout << "\nSpeedup factors:" << std::endl;
    std::cout << "unordered_set lookup is " << std::fixed << std::setprecision(2)
              << setLookupSpeedup << "x faster than set" << std::endl;
    std::cout << "unordered_map lookup is " << std::fixed << std::setprecision(2)
              << mapLookupSpeedup << "x faster than map" << std::endl;
}

// ===== PRACTICAL APPLICATIONS =====

// 1. Word Frequency Counter
void wordFrequencyCounter()
{
    std::cout << "\n===== WORD FREQUENCY COUNTER =====" << std::endl;

    // Sample text
    std::string text = "To be or not to be that is the question "
                       "Whether tis nobler in the mind to suffer "
                       "The slings and arrows of outrageous fortune "
                       "Or to take arms against a sea of troubles "
                       "And by opposing end them";

    // Convert to lowercase and count words
    std::unordered_map<std::string, int> wordFreq;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word)
    {
        // Remove punctuation and convert to lowercase
        word.erase(std::remove_if(word.begin(), word.end(),
                                  [](char c)
                                  { return !std::isalpha(c); }),
                   word.end());
        std::transform(word.begin(), word.end(), word.begin(),
                       [](char c)
                       { return std::tolower(c); });

        if (!word.empty())
        {
            wordFreq[word]++;
        }
    }

    // Transfer to map to get sorted output
    std::map<std::string, int> sortedFreq(wordFreq.begin(), wordFreq.end());

    std::cout << "Word frequencies (alphabetical order):" << std::endl;
    for (const auto &[word, count] : sortedFreq)
    {
        std::cout << std::left << std::setw(12) << word << ": " << count << std::endl;
    }

    // Sort by frequency (highest first)
    std::vector<std::pair<std::string, int>> freqVec(wordFreq.begin(), wordFreq.end());
    std::sort(freqVec.begin(), freqVec.end(),
              [](const auto &a, const auto &b)
              { return a.second > b.second; });

    std::cout << "\nWord frequencies (by frequency):" << std::endl;
    for (const auto &[word, count] : freqVec)
    {
        std::cout << std::left << std::setw(12) << word << ": " << count << std::endl;
    }
}

// 2. Symbol Table (Simple Compiler Implementation)
enum class SymbolType
{
    Variable,
    Function,
    Constant
};

struct Symbol
{
    std::string name;
    SymbolType type;
    std::string dataType;
    int scope;
    size_t address; // Memory location

    Symbol(const std::string &n, SymbolType t, const std::string &dt, int s, size_t addr)
        : name(n), type(t), dataType(dt), scope(s), address(addr) {}
};

class SymbolTable
{
private:
    std::unordered_map<std::string, std::vector<Symbol>> symbols;
    int currentScope = 0;
    size_t nextAddress = 0;

public:
    bool insert(const std::string &name, SymbolType type, const std::string &dataType)
    {
        // Check if symbol already exists in current scope
        auto it = symbols.find(name);
        if (it != symbols.end())
        {
            for (const auto &sym : it->second)
            {
                if (sym.scope == currentScope)
                {
                    return false; // Already defined in this scope
                }
            }
        }

        // Insert new symbol
        Symbol newSymbol(name, type, dataType, currentScope, nextAddress);
        symbols[name].push_back(newSymbol);

        // Update next available address (simplified)
        if (dataType == "int")
        {
            nextAddress += 4;
        }
        else if (dataType == "double")
        {
            nextAddress += 8;
        }
        else if (dataType == "char")
        {
            nextAddress += 1;
        }
        else
        {
            nextAddress += 8; // Default size
        }

        return true;
    }

    Symbol *lookup(const std::string &name)
    {
        auto it = symbols.find(name);
        if (it == symbols.end())
        {
            return nullptr; // Symbol not found
        }

        // Search from innermost scope outward
        for (int scope = currentScope; scope >= 0; --scope)
        {
            for (auto &sym : it->second)
            {
                if (sym.scope == scope)
                {
                    return &sym;
                }
            }
        }

        return nullptr; // Not found in any accessible scope
    }

    void enterScope()
    {
        currentScope++;
    }

    void exitScope()
    {
        if (currentScope > 0)
        {
            currentScope--;
        }
    }

    void printTable()
    {
        std::cout << "SYMBOL TABLE:" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        std::cout << std::left << std::setw(15) << "Name"
                  << std::setw(10) << "Type"
                  << std::setw(10) << "Data Type"
                  << std::setw(10) << "Scope"
                  << std::setw(15) << "Address" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        for (const auto &[name, symbolList] : symbols)
        {
            for (const auto &sym : symbolList)
            {
                std::string typeStr;
                switch (sym.type)
                {
                case SymbolType::Variable:
                    typeStr = "Variable";
                    break;
                case SymbolType::Function:
                    typeStr = "Function";
                    break;
                case SymbolType::Constant:
                    typeStr = "Constant";
                    break;
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

void symbolTableDemo()
{
    std::cout << "\n===== SYMBOL TABLE DEMO =====" << std::endl;

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
    Symbol *sym1 = table.lookup("x");
    if (sym1)
    {
        std::cout << "\nLooking up 'x' found: " << sym1->name
                  << " (dataType: " << sym1->dataType
                  << ", scope: " << sym1->scope << ")" << std::endl;
    }

    // Exit inner scope
    table.exitScope();

    Symbol *sym2 = table.lookup("x");
    if (sym2)
    {
        std::cout << "After exiting scope, 'x' is: " << sym2->name
                  << " (dataType: " << sym2->dataType
                  << ", scope: " << sym2->scope << ")" << std::endl;
    }
}

// 3. Graph Adjacency List
class Graph
{
private:
    // Adjacency list using unordered_map for fast node lookup
    // and set for fast edge queries
    std::unordered_map<int, std::set<int>> adjacencyList;

public:
    // Add an edge to the graph
    void addEdge(int from, int to, bool bidirectional = true)
    {
        adjacencyList[from].insert(to);
        if (bidirectional)
        {
            adjacencyList[to].insert(from);
        }
    }

    // Remove an edge
    void removeEdge(int from, int to, bool bidirectional = true)
    {
        if (adjacencyList.find(from) != adjacencyList.end())
        {
            adjacencyList[from].erase(to);
        }

        if (bidirectional && adjacencyList.find(to) != adjacencyList.end())
        {
            adjacencyList[to].erase(from);
        }
    }

    // Check if edge exists
    bool hasEdge(int from, int to) const
    {
        auto it = adjacencyList.find(from);
        if (it != adjacencyList.end())
        {
            return it->second.find(to) != it->second.end();
        }
        return false;
    }

    // Get all nodes
    std::vector<int> getNodes() const
    {
        std::vector<int> nodes;
        for (const auto &[node, _] : adjacencyList)
        {
            nodes.push_back(node);
        }
        return nodes;
    }

    // Get neighbors of a node
    std::vector<int> getNeighbors(int node) const
    {
        std::vector<int> neighbors;
        auto it = adjacencyList.find(node);
        if (it != adjacencyList.end())
        {
            neighbors.assign(it->second.begin(), it->second.end());
        }
        return neighbors;
    }

    // Print the graph
    void print() const
    {
        std::cout << "Graph Adjacency List:" << std::endl;
        for (const auto &[node, neighbors] : adjacencyList)
        {
            std::cout << node << " -> ";
            for (int neighbor : neighbors)
            {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

    // Breadth-first search
    std::vector<int> bfs(int start) const
    {
        std::vector<int> result;
        std::unordered_map<int, bool> visited;
        std::queue<int> queue;

        visited[start] = true;
        queue.push(start);

        while (!queue.empty())
        {
            int current = queue.front();
            queue.pop();
            result.push_back(current);

            auto it = adjacencyList.find(current);
            if (it != adjacencyList.end())
            {
                for (int neighbor : it->second)
                {
                    if (!visited[neighbor])
                    {
                        visited[neighbor] = true;
                        queue.push(neighbor);
                    }
                }
            }
        }

        return result;
    }
};

void graphDemo()
{
    std::cout << "\n===== GRAPH ADJACENCY LIST DEMO =====" << std::endl;

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
    for (int node : bfsResult)
    {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}

// 4. LRU Cache
template <typename K, typename V>
class LRUCache
{
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
    bool get(const K &key, V &value)
    {
        auto it = cache.find(key);
        if (it == cache.end())
        {
            return false; // Key not found
        }

        // Move to front of list (mark as most recently used)
        items.splice(items.begin(), items, it->second);
        value = it->second->second;
        return true;
    }

    // Insert or update key-value pair
    void put(const K &key, const V &value)
    {
        // Check if key already exists
        auto it = cache.find(key);
        if (it != cache.end())
        {
            // Update existing value
            it->second->second = value;
            // Move to front (most recently used)
            items.splice(items.begin(), items, it->second);
            return;
        }

        // If cache is full, remove least recently used item (back of list)
        if (items.size() == capacity)
        {
            auto last = items.back();
            cache.erase(last.first);
            items.pop_back();
        }

        // Insert new item at front
        items.push_front({key, value});
        cache[key] = items.begin();
    }

    size_t size() const
    {
        return items.size();
    }

    bool empty() const
    {
        return items.empty();
    }

    void clear()
    {
        items.clear();
        cache.clear();
    }

    // Print all items in order (most recently used first)
    void print() const
    {
        std::cout << "LRU Cache Contents (most recent first):" << std::endl;
        for (const auto &[key, value] : items)
        {
            std::cout << key << " -> " << value << std::endl;
        }
    }
};

void lruCacheDemo()
{
    std::cout << "\n===== LRU CACHE DEMO =====" << std::endl;

    // Create an LRU cache with capacity 3
    LRUCache<std::string, int> cache(3);

    // Add some items
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("c", 3);

    cache.print();

    // Access 'a' (makes it most recently used)
    int value;
    if (cache.get("a", value))
    {
        std::cout << "\nGot 'a': " << value << std::endl;
    }

    cache.print();

    // Add a new item, should evict 'b' (least recently used)
    cache.put("d", 4);

    std::cout << "\nAfter adding 'd' (should evict 'b'):" << std::endl;
    cache.print();

    // Try to access 'b' (should be gone)
    if (!cache.get("b", value))
    {
        std::cout << "\n'b' no longer in cache" << std::endl;
    }
}

// 5. Task Priority Queue with Custom Comparator
struct Task
{
    std::string name;
    int priority;
    std::string description;

    Task(const std::string &n, int p, const std::string &d)
        : name(n), priority(p), description(d) {}
};

// Custom comparator for tasks
struct TaskComparator
{
    bool operator()(const Task &a, const Task &b) const
    {
        // Higher priority first (descending order)
        if (a.priority != b.priority)
        {
            return a.priority > b.priority;
        }
        // If priorities are equal, sort alphabetically
        return a.name < b.name;
    }
};

void taskPriorityDemo()
{
    std::cout << "\n===== TASK PRIORITY QUEUE DEMO =====" << std::endl;

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
    std::cout << std::left << std::setw(5) << "Prio"
              << std::setw(20) << "Task"
              << "Description" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto &task : taskQueue)
    {
        std::cout << std::left << std::setw(5) << task.priority
                  << std::setw(20) << task.name
                  << task.description << std::endl;
    }

    // Alternative: using std::priority_queue
    std::cout << "\nUsing std::priority_queue:" << std::endl;
    std::priority_queue<
        Task,
        std::vector<Task>,
        std::function<bool(const Task &, const Task &)>>
        pq(
            [](const Task &a, const Task &b)
            {
                // Note: priority_queue uses reverse comparison logic
                if (a.priority != b.priority)
                {
                    return a.priority < b.priority; // Higher priority first
                }
                return a.name > b.name; // Alphabetical order
            });

    // Add tasks
    pq.push(Task("Write report", 3, "Complete quarterly report"));
    pq.push(Task("Fix bug", 5, "High priority bug in login system"));
    pq.push(Task("Update database", 2, "Apply schema changes"));
    pq.push(Task("Review PR", 4, "Review pull request from team"));
    pq.push(Task("Meeting", 3, "Weekly team meeting"));

    // Process tasks
    std::cout << std::left << std::setw(5) << "Prio"
              << std::setw(20) << "Task"
              << "Description" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    while (!pq.empty())
    {
        const Task &task = pq.top();
        std::cout << std::left << std::setw(5) << task.priority
                  << std::setw(20) << task.name
                  << task.description << std::endl;
        pq.pop();
    }
}

// ===== LEETCODE-STYLE PROBLEMS =====

// Two Sum problem (LeetCode #1)
std::vector<int> twoSum(const std::vector<int> &nums, int target)
{
    std::unordered_map<int, int> numMap; // value -> index

    for (int i = 0; i < nums.size(); i++)
    {
        int complement = target - nums[i];

        // Check if complement exists in the map
        auto it = numMap.find(complement);
        if (it != numMap.end())
        {
            return {it->second, i};
        }

        // Add current number to map
        numMap[nums[i]] = i;
    }

    return {}; // No solution found
}

// Contains Duplicate (LeetCode #217)
bool containsDuplicate(const std::vector<int> &nums)
{
    std::unordered_set<int> seen;

    for (int num : nums)
    {
        if (seen.count(num) > 0)
        {
            return true; // Found duplicate
        }
        seen.insert(num);
    }

    return false; // No duplicates found
}

// Valid Anagram (LeetCode #242)
bool isAnagram(const std::string &s, const std::string &t)
{
    if (s.length() != t.length())
    {
        return false;
    }

    std::unordered_map<char, int> charCount;

    // Count characters in first string
    for (char c : s)
    {
        charCount[c]++;
    }

    // Decrement counts for second string
    for (char c : t)
    {
        charCount[c]--;
        if (charCount[c] < 0)
        {
            return false; // More occurrences in t than in s
        }
    }

    // Check all counts are zero
    for (const auto &[_, count] : charCount)
    {
        if (count != 0)
        {
            return false;
        }
    }

    return true;
}

// Group Anagrams (LeetCode #49)
std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string> &strs)
{
    std::unordered_map<std::string, std::vector<std::string>> anagramGroups;

    for (const std::string &s : strs)
    {
        // Use sorted string as key
        std::string key = s;
        std::sort(key.begin(), key.end());

        anagramGroups[key].push_back(s);
    }

    // Convert map to vector of vectors
    std::vector<std::vector<std::string>> result;
    for (const auto &[_, group] : anagramGroups)
    {
        result.push_back(group);
    }

    return result;
}

void leetcodeProblemsDemo()
{
    std::cout << "\n===== LEETCODE PROBLEMS DEMO =====" << std::endl;

    // 1. Two Sum
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> twoSumResult = twoSum(nums, target);

    std::cout << "Two Sum: [";
    for (size_t i = 0; i < twoSumResult.size(); i++)
    {
        std::cout << twoSumResult[i];
        if (i < twoSumResult.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    // 2. Contains Duplicate
    std::vector<int> dupeNums1 = {1, 2, 3, 1};
    std::vector<int> dupeNums2 = {1, 2, 3, 4};

    std::cout << "Contains Duplicate Test 1: "
              << (containsDuplicate(dupeNums1) ? "Yes" : "No") << std::endl;
    std::cout << "Contains Duplicate Test 2: "
              << (containsDuplicate(dupeNums2) ? "Yes" : "No") << std::endl;

    // 3. Valid Anagram
    std::string s1 = "anagram";
    std::string t1 = "nagaram";
    std::string s2 = "rat";
    std::string t2 = "car";

    std::cout << "Is Anagram Test 1: "
              << (isAnagram(s1, t1) ? "Yes" : "No") << std::endl;
    std::cout << "Is Anagram Test 2: "
              << (isAnagram(s2, t2) ? "Yes" : "No") << std::endl;

    // 4. Group Anagrams
    std::vector<std::string> anagramStrs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    std::vector<std::vector<std::string>> groupedAnagrams = groupAnagrams(anagramStrs);

    std::cout << "Group Anagrams Result:" << std::endl;
    for (const auto &group : groupedAnagrams)
    {
        std::cout << "[";
        for (size_t i = 0; i < group.size(); i++)
        {
            std::cout << group[i];
            if (i < group.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
}

// ===== MAIN FUNCTION =====

int main()
{
    std::cout << "===================================================" << std::endl;
    std::cout << "DAY 13: SETS & MAPS IN C++ STL DEMONSTRATION #DSAin45" << std::endl;
    std::cout << "===================================================" << std::endl;

    // Basic demonstrations
    setBasics();
    unorderedSetBasics();
    setOperations();

    multisetDemo();
    unorderedMultisetDemo();

    mapBasics();
    unorderedMapBasics();
    multimapDemo();
    unorderedMultimapDemo();

    // Custom key types
    orderedCustomKeyDemo();
    unorderedCustomKeyDemo();
    functorDemo();

    // Performance comparison
    performanceComparison();

    // Practical applications
    wordFrequencyCounter();
    symbolTableDemo();
    graphDemo();
    lruCacheDemo();
    taskPriorityDemo();

    // LeetCode-style problems
    leetcodeProblemsDemo();

    std::cout << "\n===================================================" << std::endl;
    std::cout << "DEMONSTRATION COMPLETE" << std::endl;
    std::cout << "===================================================" << std::endl;

    return 0;
}