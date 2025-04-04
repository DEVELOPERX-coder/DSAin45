// STL Containers in C++ - Day 5 of #DSAin45
// Comprehensive examples of STL container usage

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
#include <chrono>  // For timing operations
#include <iomanip> // For output formatting

using namespace std;
using namespace std::chrono;

// ======== 1. VECTOR BASICS AND OPERATIONS ========

void vectorBasics()
{
    cout << "\n===== VECTOR BASICS =====\n";

    // Different ways to create and initialize vectors

    // Empty vector
    vector<int> vec1;
    cout << "Empty vector size: " << vec1.size() << endl;

    // Vector with initial size
    vector<int> vec2(5); // Creates a vector with 5 elements, all initialized to 0
    cout << "Vector with 5 default elements: ";
    for (int n : vec2)
        cout << n << " ";
    cout << endl;

    // Vector with initial size and value
    vector<int> vec3(5, 10); // Creates a vector with 5 elements, all set to 10
    cout << "Vector with 5 elements set to 10: ";
    for (int n : vec3)
        cout << n << " ";
    cout << endl;

    // Vector with initializer list
    vector<int> vec4 = {10, 20, 30, 40, 50};
    cout << "Vector with initializer list: ";
    for (int n : vec4)
        cout << n << " ";
    cout << endl;

    // Vector from another vector
    vector<int> vec5(vec4.begin(), vec4.end());
    cout << "Vector copied from vec4: ";
    for (int n : vec5)
        cout << n << " ";
    cout << endl;

    // 2D vector (vector of vectors)
    vector<vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    cout << "2D vector (matrix):" << endl;
    for (const auto &row : matrix)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }
}

void vectorOperations()
{
    cout << "\n===== VECTOR OPERATIONS =====\n";

    vector<int> vec = {10, 20, 30, 40, 50};

    // Basic properties
    cout << "Size: " << vec.size() << endl;
    cout << "Capacity: " << vec.capacity() << endl;
    cout << "Is empty: " << (vec.empty() ? "Yes" : "No") << endl;

    // Accessing elements
    cout << "First element: " << vec.front() << endl;
    cout << "Last element: " << vec.back() << endl;
    cout << "Element at index 2: " << vec[2] << endl;         // No bounds checking
    cout << "Element at index 2 (at): " << vec.at(2) << endl; // With bounds checking

    // Adding elements
    vec.push_back(60); // Add to end
    cout << "After push_back(60): ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // Inserting elements at specific position
    vec.insert(vec.begin() + 3, 35); // Insert 35 at index 3
    cout << "After insert(35) at index 3: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // Removing elements
    vec.pop_back(); // Remove from end
    cout << "After pop_back(): ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    vec.erase(vec.begin() + 3); // Remove element at index 3
    cout << "After erase() at index 3: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // Resizing
    vec.resize(10); // Increase size to 10, new elements initialized to 0
    cout << "After resize(10): ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    vec.resize(3); // Decrease size to 3, extra elements are removed
    cout << "After resize(3): ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // Clearing all elements
    vec.clear();
    cout << "After clear(), size: " << vec.size() << endl;
}

void vectorMemoryManagement()
{
    cout << "\n===== VECTOR MEMORY MANAGEMENT =====\n";

    vector<int> vec;

    cout << "Initial size: " << vec.size() << endl;
    cout << "Initial capacity: " << vec.capacity() << endl;

    // Adding elements one by one to observe capacity changes
    cout << "\nAdding elements one by one:" << endl;
    for (int i = 1; i <= 20; i++)
    {
        vec.push_back(i);
        cout << "After adding " << i << ": size=" << vec.size()
             << ", capacity=" << vec.capacity() << endl;
    }

    // Using reserve for efficiency
    vector<int> vec2;
    vec2.reserve(20); // Reserve space for 20 elements

    cout << "\nAfter reserve(20): size=" << vec2.size()
         << ", capacity=" << vec2.capacity() << endl;

    for (int i = 1; i <= 20; i++)
    {
        vec2.push_back(i);
    }

    cout << "After adding 20 elements: size=" << vec2.size()
         << ", capacity=" << vec2.capacity() << endl;

    // Shrink to fit
    vec2.shrink_to_fit();
    cout << "After shrink_to_fit(): size=" << vec2.size()
         << ", capacity=" << vec2.capacity() << endl;

    // Performance comparison
    const int num_elements = 100000;

    // Without reserve
    auto start = high_resolution_clock::now();
    vector<int> v1;
    for (int i = 0; i < num_elements; i++)
    {
        v1.push_back(i);
    }
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end - start);

    // With reserve
    start = high_resolution_clock::now();
    vector<int> v2;
    v2.reserve(num_elements);
    for (int i = 0; i < num_elements; i++)
    {
        v2.push_back(i);
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end - start);

    cout << "\nPerformance comparison for adding " << num_elements << " elements:" << endl;
    cout << "Without reserve: " << duration1.count() << " microseconds" << endl;
    cout << "With reserve: " << duration2.count() << " microseconds" << endl;
    cout << "Speed improvement: " << fixed << setprecision(2)
         << (float)duration1.count() / duration2.count() << "x" << endl;
}

// ======== 2. OTHER STL CONTAINERS ========

void listExample()
{
    cout << "\n===== LIST EXAMPLE =====\n";

    // std::list is a doubly-linked list
    list<int> myList = {10, 20, 30, 40, 50};

    cout << "Initial list: ";
    for (int n : myList)
        cout << n << " ";
    cout << endl;

    // Insert at the beginning (O(1))
    myList.push_front(5);

    // Insert at the end (O(1))
    myList.push_back(55);

    cout << "After push_front and push_back: ";
    for (int n : myList)
        cout << n << " ";
    cout << endl;

    // Insert in the middle
    auto it = myList.begin();
    advance(it, 3); // Move iterator to the 4th element
    myList.insert(it, 25);

    cout << "After inserting 25 at position 3: ";
    for (int n : myList)
        cout << n << " ";
    cout << endl;

    // Remove a specific value
    myList.remove(30);

    cout << "After removing 30: ";
    for (int n : myList)
        cout << n << " ";
    cout << endl;

    // Sort the list
    myList.sort();

    cout << "After sorting: ";
    for (int n : myList)
        cout << n << " ";
    cout << endl;

    // Reverse the list
    myList.reverse();

    cout << "After reversing: ";
    for (int n : myList)
        cout << n << " ";
    cout << endl;
}

void dequeExample()
{
    cout << "\n===== DEQUE EXAMPLE =====\n";

    // std::deque is a double-ended queue
    deque<int> myDeque = {10, 20, 30, 40, 50};

    cout << "Initial deque: ";
    for (int n : myDeque)
        cout << n << " ";
    cout << endl;

    // Insert at the beginning (O(1))
    myDeque.push_front(5);

    // Insert at the end (O(1))
    myDeque.push_back(55);

    cout << "After push_front and push_back: ";
    for (int n : myDeque)
        cout << n << " ";
    cout << endl;

    // Access elements (O(1))
    cout << "First element: " << myDeque.front() << endl;
    cout << "Last element: " << myDeque.back() << endl;
    cout << "Element at index 3: " << myDeque[3] << endl;

    // Remove from the beginning and end
    myDeque.pop_front();
    myDeque.pop_back();

    cout << "After pop_front and pop_back: ";
    for (int n : myDeque)
        cout << n << " ";
    cout << endl;

    // Insert in the middle
    myDeque.insert(myDeque.begin() + 2, 25);

    cout << "After inserting 25 at position 2: ";
    for (int n : myDeque)
        cout << n << " ";
    cout << endl;
}

void setExample()
{
    cout << "\n===== SET EXAMPLE =====\n";

    // std::set - Stores unique elements in sorted order
    set<int> mySet = {50, 30, 10, 40, 20, 10}; // Note: 10 is duplicated

    cout << "Set contents (automatically sorted, duplicates removed): ";
    for (int n : mySet)
        cout << n << " ";
    cout << endl;

    // Insert elements
    mySet.insert(15);
    mySet.insert(60);

    cout << "After insertions: ";
    for (int n : mySet)
        cout << n << " ";
    cout << endl;

    // Check if element exists
    if (mySet.find(15) != mySet.end())
    {
        cout << "15 is in the set" << endl;
    }

    if (mySet.find(100) == mySet.end())
    {
        cout << "100 is not in the set" << endl;
    }

    // Count occurrences (always 0 or 1 for set)
    cout << "Count of 15: " << mySet.count(15) << endl;
    cout << "Count of 100: " << mySet.count(100) << endl;

    // Erase elements
    mySet.erase(30);

    cout << "After erasing 30: ";
    for (int n : mySet)
        cout << n << " ";
    cout << endl;

    // Lower and upper bound
    auto lower = mySet.lower_bound(20); // Iterator to first element >= 20
    auto upper = mySet.upper_bound(40); // Iterator to first element > 40

    cout << "Elements from 20 to 40: ";
    for (auto it = lower; it != upper; ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}

void mapExample()
{
    cout << "\n===== MAP EXAMPLE =====\n";

    // std::map - Key-value pairs, sorted by key
    map<string, int> ages = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"Dave", 40}};

    cout << "Map contents (sorted by key):" << endl;
    for (const auto &pair : ages)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Access or insert with []
    ages["Eve"] = 28;
    ages["Bob"] = 26; // Update existing value

    cout << "\nAfter modifications:" << endl;
    for (const auto &pair : ages)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Check if key exists
    if (ages.find("Alice") != ages.end())
    {
        cout << "\nAlice's age is " << ages["Alice"] << endl;
    }

    // Access with at() (throws exception if key doesn't exist)
    try
    {
        cout << "Bob's age with at(): " << ages.at("Bob") << endl;
        cout << "Frank's age with at(): " << ages.at("Frank") << endl; // Will throw
    }
    catch (const out_of_range &e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    // Insert with insert()
    ages.insert({"Frank", 45});

    // Erase by key
    ages.erase("Dave");

    cout << "\nFinal map contents:" << endl;
    for (const auto &pair : ages)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}

void unordered_mapExample()
{
    cout << "\n===== UNORDERED_MAP EXAMPLE =====\n";

    // std::unordered_map - Key-value pairs, not sorted, hash-based lookup
    unordered_map<string, int> ages = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"Dave", 40}};

    cout << "Unordered map contents (not necessarily sorted):" << endl;
    for (const auto &pair : ages)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Operations are similar to map
    ages["Eve"] = 28;
    ages.erase("Dave");

    cout << "\nAfter modifications:" << endl;
    for (const auto &pair : ages)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Hash-specific operations
    cout << "\nBucket count: " << ages.bucket_count() << endl;
    cout << "Load factor: " << ages.load_factor() << endl;

    // Performance comparison: map vs unordered_map
    const int num_elements = 100000;

    // Fill both maps with the same data
    map<int, int> orderedMap;
    unordered_map<int, int> unorderedMap;

    for (int i = 0; i < num_elements; i++)
    {
        orderedMap[i] = i;
        unorderedMap[i] = i;
    }

    // Test lookup performance
    int lookups = 10000;

    // map (tree-based)
    auto start = high_resolution_clock::now();
    for (int i = 0; i < lookups; i++)
    {
        int key = rand() % num_elements;
        int value = orderedMap[key];
    }
    auto end = high_resolution_clock::now();
    auto map_duration = duration_cast<microseconds>(end - start);

    // unordered_map (hash-based)
    start = high_resolution_clock::now();
    for (int i = 0; i < lookups; i++)
    {
        int key = rand() % num_elements;
        int value = unorderedMap[key];
    }
    end = high_resolution_clock::now();
    auto unordered_map_duration = duration_cast<microseconds>(end - start);

    cout << "\nPerformance comparison for " << lookups << " random lookups:" << endl;
    cout << "map (O(log n)): " << map_duration.count() << " microseconds" << endl;
    cout << "unordered_map (O(1) avg): " << unordered_map_duration.count() << " microseconds" << endl;
    cout << "Speed improvement: " << fixed << setprecision(2)
         << (float)map_duration.count() / unordered_map_duration.count() << "x" << endl;
}

void stackExample()
{
    cout << "\n===== STACK EXAMPLE =====\n";

    // std::stack - LIFO data structure
    stack<int> myStack;

    // Push elements
    myStack.push(10);
    myStack.push(20);
    myStack.push(30);

    cout << "Stack size: " << myStack.size() << endl;
    cout << "Top element: " << myStack.top() << endl;

    // Pop elements
    cout << "\nPopping elements:" << endl;
    while (!myStack.empty())
    {
        cout << myStack.top() << endl;
        myStack.pop();
    }

    // Classic stack application: Checking balanced parentheses
    string expressions[] = {
        "((()))",
        "({[]})",
        "(()",
        ")("};

    for (const string &expr : expressions)
    {
        stack<char> parenStack;
        bool balanced = true;

        for (char c : expr)
        {
            if (c == '(' || c == '{' || c == '[')
            {
                parenStack.push(c);
            }
            else if (c == ')' || c == '}' || c == ']')
            {
                if (parenStack.empty())
                {
                    balanced = false;
                    break;
                }

                char top = parenStack.top();
                if ((c == ')' && top == '(') ||
                    (c == '}' && top == '{') ||
                    (c == ']' && top == '['))
                {
                    parenStack.pop();
                }
                else
                {
                    balanced = false;
                    break;
                }
            }
        }

        if (!parenStack.empty())
        {
            balanced = false;
        }

        cout << "Expression \"" << expr << "\" is "
             << (balanced ? "balanced" : "not balanced") << endl;
    }
}

void queueExample()
{
    cout << "\n===== QUEUE EXAMPLE =====\n";

    // std::queue - FIFO data structure
    queue<int> myQueue;

    // Push elements (enqueue)
    myQueue.push(10);
    myQueue.push(20);
    myQueue.push(30);

    cout << "Queue size: " << myQueue.size() << endl;
    cout << "Front element: " << myQueue.front() << endl;
    cout << "Back element: " << myQueue.back() << endl;

    // Pop elements (dequeue)
    cout << "\nDequeuing elements:" << endl;
    while (!myQueue.empty())
    {
        cout << myQueue.front() << endl;
        myQueue.pop();
    }
}

void priorityQueueExample()
{
    cout << "\n===== PRIORITY QUEUE EXAMPLE =====\n";

    // std::priority_queue - Heap-based priority queue (max heap by default)
    priority_queue<int> maxHeap;

    // Insert elements
    maxHeap.push(30);
    maxHeap.push(100);
    maxHeap.push(50);
    maxHeap.push(10);

    cout << "Max heap - elements in descending order:" << endl;
    while (!maxHeap.empty())
    {
        cout << maxHeap.top() << endl; // Get highest priority element
        maxHeap.pop();
    }

    // Min heap (using greater comparator)
    priority_queue<int, vector<int>, greater<int>> minHeap;

    minHeap.push(30);
    minHeap.push(100);
    minHeap.push(50);
    minHeap.push(10);

    cout << "\nMin heap - elements in ascending order:" << endl;
    while (!minHeap.empty())
    {
        cout << minHeap.top() << endl; // Get lowest priority element
        minHeap.pop();
    }

    // Custom comparator with a class
    struct Person
    {
        string name;
        int age;

        Person(string n, int a) : name(n), age(a) {}
    };

    // Custom comparator - compare by age
    struct CompareAge
    {
        bool operator()(const Person &a, const Person &b)
        {
            return a.age > b.age; // Min heap by age (younger people first)
        }
    };

    priority_queue<Person, vector<Person>, CompareAge> personQueue;

    personQueue.push(Person("Alice", 30));
    personQueue.push(Person("Bob", 25));
    personQueue.push(Person("Charlie", 35));
    personQueue.push(Person("Dave", 20));

    cout << "\nPriority queue with custom comparator - people by age (youngest first):" << endl;
    while (!personQueue.empty())
    {
        Person p = personQueue.top();
        cout << p.name << ": " << p.age << endl;
        personQueue.pop();
    }
}

// ======== 3. STL ALGORITHMS ========

void algorithmExamples()
{
    cout << "\n===== STL ALGORITHM EXAMPLES =====\n";

    vector<int> vec = {5, 9, 1, 3, 7, 4, 8, 2, 6};

    cout << "Original vector: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // find - Locate element
    auto it = find(vec.begin(), vec.end(), 7);
    if (it != vec.end())
    {
        cout << "Found 7 at position: " << (it - vec.begin()) << endl;
    }

    // find_if - Find element satisfying predicate
    auto it2 = find_if(vec.begin(), vec.end(), [](int x)
                       { return x > 5; });
    if (it2 != vec.end())
    {
        cout << "First element > 5: " << *it2 << " at position: " << (it2 - vec.begin()) << endl;
    }

    // sort - Sort elements
    sort(vec.begin(), vec.end());
    cout << "After sorting: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // binary_search - Check if element exists (works on sorted ranges)
    bool has7 = binary_search(vec.begin(), vec.end(), 7);
    cout << "Vector contains 7: " << (has7 ? "Yes" : "No") << endl;

    // lower_bound - First element >= value
    auto lower = lower_bound(vec.begin(), vec.end(), 5);
    cout << "lower_bound(5): " << *lower << " at position: " << (lower - vec.begin()) << endl;

    // upper_bound - First element > value
    auto upper = upper_bound(vec.begin(), vec.end(), 5);
    cout << "upper_bound(5): " << *upper << " at position: " << (upper - vec.begin()) << endl;

    // reverse - Reverse elements
    reverse(vec.begin(), vec.end());
    cout << "After reversing: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // max_element - Find maximum element
    auto max_it = max_element(vec.begin(), vec.end());
    cout << "Maximum element: " << *max_it << endl;

    // min_element - Find minimum element
    auto min_it = min_element(vec.begin(), vec.end());
    cout << "Minimum element: " << *min_it << endl;

    // count, count_if - Count elements
    int count3 = count(vec.begin(), vec.end(), 3);
    int countGt5 = count_if(vec.begin(), vec.end(), [](int x)
                            { return x > 5; });
    cout << "Count of 3: " << count3 << endl;
    cout << "Count of elements > 5: " << countGt5 << endl;

    // transform - Apply function to range
    vector<int> squared(vec.size());
    transform(vec.begin(), vec.end(), squared.begin(), [](int x)
              { return x * x; });
    cout << "Squared: ";
    for (int n : squared)
        cout << n << " ";
    cout << endl;

    // for_each - Apply function to all elements (without creating new container)
    cout << "Printing with for_each: ";
    for_each(vec.begin(), vec.end(), [](int x)
             { cout << x << " "; });
    cout << endl;
}

// ======== 4. PITFALLS AND BEST PRACTICES ========

void iteratorInvalidation()
{
    cout << "\n===== ITERATOR INVALIDATION EXAMPLE =====\n";

    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "Original vector: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // WRONG: Modifying container while iterating
    cout << "\nWRONG approach (will cause undefined behavior):" << endl;
    cout << "// for (auto it = vec.begin(); it != vec.end(); ++it) {" << endl;
    cout << "//     if (*it % 2 == 0) {" << endl;
    cout << "//         vec.erase(it);  // WRONG: Iterator is invalidated!" << endl;
    cout << "//     }" << endl;
    cout << "// }" << endl;

    // CORRECT: Use the returned iterator from erase()
    cout << "\nCORRECT approach:" << endl;
    for (auto it = vec.begin(); it != vec.end();)
    {
        if (*it % 2 == 0)
        {
            it = vec.erase(it); // erase() returns iterator to next element
        }
        else
        {
            ++it;
        }
    }

    cout << "After removing even numbers: ";
    for (int n : vec)
        cout << n << " ";
    cout << endl;

    // Alternative approach: using remove_if and erase
    vector<int> vec2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The erase-remove idiom
    vec2.erase(
        remove_if(vec2.begin(), vec2.end(), [](int x)
                  { return x % 2 == 0; }),
        vec2.end());

    cout << "Using erase-remove idiom: ";
    for (int n : vec2)
        cout << n << " ";
    cout << endl;
}

void inefficientVectorUsage()
{
    cout << "\n===== INEFFICIENT VECTOR USAGE =====\n";

    const int num_elements = 100000;

    // INEFFICIENT: Growing one by one without reserving
    auto start = high_resolution_clock::now();
    vector<int> vec1;
    for (int i = 0; i < num_elements; i++)
    {
        vec1.push_back(i);
    }
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end - start);

    // EFFICIENT: Reserve space upfront
    start = high_resolution_clock::now();
    vector<int> vec2;
    vec2.reserve(num_elements);
    for (int i = 0; i < num_elements; i++)
    {
        vec2.push_back(i);
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end - start);

    cout << "Adding " << num_elements << " elements:" << endl;
    cout << "Without reserve: " << duration1.count() << " microseconds" << endl;
    cout << "With reserve: " << duration2.count() << " microseconds" << endl;
    cout << "Speed improvement: " << fixed << setprecision(2)
         << (float)duration1.count() / duration2.count() << "x" << endl;

    // INEFFICIENT: String concatenation in a loop
    start = high_resolution_clock::now();
    string result1;
    for (int i = 0; i < 10000; i++)
    {
        result1 += to_string(i);
    }
    end = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(end - start);

    // EFFICIENT: Reserve space for string
    start = high_resolution_clock::now();
    string result2;
    result2.reserve(100000); // Approximate size
    for (int i = 0; i < 10000; i++)
    {
        result2 += to_string(i);
    }
    end = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(end - start);

    cout << "\nString concatenation 10000 times:" << endl;
    cout << "Without reserve: " << duration3.count() << " microseconds" << endl;
    cout << "With reserve: " << duration4.count() << " microseconds" << endl;
    cout << "Speed improvement: " << fixed << setprecision(2)
         << (float)duration3.count() / duration4.count() << "x" << endl;
}

void wrongContainerChoice()
{
    cout << "\n===== WRONG CONTAINER CHOICE =====\n";

    const int num_elements = 10000;
    const int num_operations = 1000;

    // Scenario 1: Frequent lookups

    // Wrong choice: vector for frequent lookups by value
    vector<int> vec;
    for (int i = 0; i < num_elements; i++)
    {
        vec.push_back(i);
    }

    auto start = high_resolution_clock::now();
    int count = 0;
    for (int i = 0; i < num_operations; i++)
    {
        int value_to_find = rand() % num_elements;
        if (find(vec.begin(), vec.end(), value_to_find) != vec.end())
        {
            count++;
        }
    }
    auto end = high_resolution_clock::now();
    auto vec_lookup_time = duration_cast<microseconds>(end - start);

    // Better choice: unordered_set for O(1) lookups
    unordered_set<int> uset;
    for (int i = 0; i < num_elements; i++)
    {
        uset.insert(i);
    }

    start = high_resolution_clock::now();
    count = 0;
    for (int i = 0; i < num_operations; i++)
    {
        int value_to_find = rand() % num_elements;
        if (uset.find(value_to_find) != uset.end())
        {
            count++;
        }
    }
    end = high_resolution_clock::now();
    auto uset_lookup_time = duration_cast<microseconds>(end - start);

    cout << "Random lookups - " << num_operations << " operations:" << endl;
    cout << "vector with find(): " << vec_lookup_time.count() << " microseconds" << endl;
    cout << "unordered_set with find(): " << uset_lookup_time.count() << " microseconds" << endl;
    cout << "Speed improvement: " << fixed << setprecision(2)
         << (float)vec_lookup_time.count() / uset_lookup_time.count() << "x" << endl;

    // Scenario 2: Insertions in the middle

    // Wrong choice: vector for frequent insertions in the middle
    start = high_resolution_clock::now();
    vector<int> vec2;
    for (int i = 0; i < num_elements / 10; i++)
    {
        vec2.insert(vec2.begin() + (vec2.size() / 2), i); // Insert in the middle
    }
    end = high_resolution_clock::now();
    auto vec_insert_time = duration_cast<microseconds>(end - start);

    // Better choice: list for O(1) insertions once position is found
    start = high_resolution_clock::now();
    list<int> lst;
    for (int i = 0; i < num_elements / 10; i++)
    {
        auto it = lst.begin();
        advance(it, lst.size() / 2); // O(n) to find middle, but this is unavoidable
        lst.insert(it, i);           // O(1) insertion
    }
    end = high_resolution_clock::now();
    auto list_insert_time = duration_cast<microseconds>(end - start);

    cout << "\nInsertions in the middle - " << (num_elements / 10) << " operations:" << endl;
    cout << "vector: " << vec_insert_time.count() << " microseconds" << endl;
    cout << "list: " << list_insert_time.count() << " microseconds" << endl;
    cout << "Speed improvement: " << fixed << setprecision(2)
         << (float)vec_insert_time.count() / list_insert_time.count() << "x" << endl;
}

// ======== 5. PRACTICE PROBLEM SOLUTION ========

char firstNonRepeatingChar(const string &s)
{
    // Using unordered_map to count occurrences
    unordered_map<char, int> charCount;

    // First pass: count occurrences
    for (char c : s)
    {
        charCount[c]++;
    }

    // Second pass: find first non-repeating character
    for (char c : s)
    {
        if (charCount[c] == 1)
        {
            return c;
        }
    }

    return '\0'; // If no such character exists
}

void practiceProblems()
{
    cout << "\n===== PRACTICE PROBLEM SOLUTION =====\n";

    // Test cases for first non-repeating character
    vector<string> testStrings = {
        "leetcode",
        "loveleetcode",
        "aabb"};

    for (const string &s : testStrings)
    {
        char result = firstNonRepeatingChar(s);
        cout << "First non-repeating character in \"" << s << "\": ";
        if (result != '\0')
        {
            cout << "'" << result << "'" << endl;
        }
        else
        {
            cout << "None found" << endl;
        }
    }
}

// ======== 6. ADVANCED STL CONTAINER USAGE ========

void containerCombinations()
{
    cout << "\n===== CONTAINER COMBINATIONS =====\n";

    // Example 1: Vector of pairs
    vector<pair<string, int>> studentScores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92},
        {"David", 78},
        {"Eve", 88}};

    // Sort by score (descending)
    sort(studentScores.begin(), studentScores.end(),
         [](const pair<string, int> &a, const pair<string, int> &b)
         {
             return a.second > b.second;
         });

    cout << "Student scores (sorted by score):" << endl;
    for (const auto &student : studentScores)
    {
        cout << student.first << ": " << student.second << endl;
    }

    // Example 2: Map of vectors (grouping)
    map<char, vector<string>> namesByInitial;

    // Add names to respective groups
    vector<string> names = {"Alice", "Bob", "Charlie", "David", "Eve",
                            "Andrew", "Beth", "Carol", "Derek", "Eliza"};

    for (const string &name : names)
    {
        char initial = name[0];
        namesByInitial[initial].push_back(name);
    }

    cout << "\nNames grouped by initial:" << endl;
    for (const auto &group : namesByInitial)
    {
        cout << group.first << ": ";
        for (const string &name : group.second)
        {
            cout << name << " ";
        }
        cout << endl;
    }

    // Example 3: Set of custom objects
    struct Person
    {
        string name;
        int age;

        Person(string n, int a) : name(n), age(a) {}

        // Required for set - define ordering
        bool operator<(const Person &other) const
        {
            if (name != other.name)
            {
                return name < other.name;
            }
            return age < other.age;
        }
    };

    set<Person> uniquePeople;
    uniquePeople.insert(Person("Alice", 30));
    uniquePeople.insert(Person("Bob", 25));
    uniquePeople.insert(Person("Alice", 35)); // Different age, so considered different
    uniquePeople.insert(Person("Alice", 30)); // Duplicate, will be ignored

    cout << "\nSet of unique people (ordered by name, then age):" << endl;
    for (const Person &person : uniquePeople)
    {
        cout << person.name << ": " << person.age << endl;
    }

    // Example 4: Nested maps
    map<string, map<string, double>> countryStateCities = {
        {"USA", {{"California", 39.5}, // Population in millions
                 {"Texas", 29.0},
                 {"Florida", 21.5}}},
        {"India", {{"Maharashtra", 114.0}, {"Uttar Pradesh", 200.0}, {"Bihar", 104.0}}}};

    cout << "\nNested map of countries, states, and populations:" << endl;
    for (const auto &country : countryStateCities)
    {
        cout << country.first << ":" << endl;
        for (const auto &state : country.second)
        {
            cout << "  " << state.first << ": "
                 << state.second << " million" << endl;
        }
    }
}

void advancedVectorOperations()
{
    cout << "\n===== ADVANCED VECTOR OPERATIONS =====\n";

    vector<int> nums = {9, 4, 7, 2, 5, 10, 3, 8, 1, 6};
    cout << "Original vector: ";
    for (int n : nums)
        cout << n << " ";
    cout << endl;

    // partial_sort - Sort just a portion of the vector
    vector<int> partial = nums;
    partial_sort(partial.begin(), partial.begin() + 4, partial.end());
    cout << "After partial_sort (first 4 elements): ";
    for (int n : partial)
        cout << n << " ";
    cout << endl;

    // nth_element - Places the nth element in its sorted position
    vector<int> nth = nums;
    nth_element(nth.begin(), nth.begin() + nth.size() / 2, nth.end());
    cout << "After nth_element (median position): ";
    for (int n : nth)
        cout << n << " ";
    cout << endl;
    cout << "Median element: " << nth[nth.size() / 2] << endl;

    // partition - Separate elements satisfying a predicate
    vector<int> partitioned = nums;
    auto pivot = partition(partitioned.begin(), partitioned.end(),
                           [](int x)
                           { return x % 2 == 0; });
    cout << "After partition (evens first): ";
    for (int n : partitioned)
        cout << n << " ";
    cout << endl;

    // stable_partition - Like partition but preserves relative order
    vector<int> stable_part = nums;
    auto stable_pivot = stable_partition(stable_part.begin(), stable_part.end(),
                                         [](int x)
                                         { return x % 2 == 0; });
    cout << "After stable_partition (evens first, preserving order): ";
    for (int n : stable_part)
        cout << n << " ";
    cout << endl;

    // unique - Remove consecutive duplicates
    vector<int> with_dups = {1, 1, 2, 2, 3, 3, 3, 4, 5, 5};
    sort(with_dups.begin(), with_dups.end());
    auto last = unique(with_dups.begin(), with_dups.end());
    with_dups.erase(last, with_dups.end());
    cout << "After unique (removing duplicates): ";
    for (int n : with_dups)
        cout << n << " ";
    cout << endl;

    // rotate - Rotate elements
    vector<int> to_rotate = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    rotate(to_rotate.begin(), to_rotate.begin() + 3, to_rotate.end());
    cout << "After rotate (by 3 positions): ";
    for (int n : to_rotate)
        cout << n << " ";
    cout << endl;
}

void realWorldExamples()
{
    cout << "\n===== REAL WORLD EXAMPLES =====\n";

    // Example 1: Word frequency counter
    string text = "this is a sample text to count word frequency this is just a sample";
    istringstream iss(text);
    unordered_map<string, int> wordFreq;

    string word;
    while (iss >> word)
    {
        wordFreq[word]++;
    }

    cout << "Word frequency:" << endl;
    for (const auto &pair : wordFreq)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Example 2: Event scheduler using priority queue
    struct Event
    {
        string name;
        int priority;
        time_t time;

        Event(string n, int p, time_t t) : name(n), priority(p), time(t) {}

        // Higher priority events come first, or earlier events if priority is the same
        bool operator<(const Event &other) const
        {
            if (priority != other.priority)
            {
                return priority < other.priority; // Note: priority_queue is a max-heap
            }
            return time > other.time;
        }
    };

    priority_queue<Event> eventQueue;

    // Current time
    time_t now = time(nullptr);

    // Add events (with different priorities and times)
    eventQueue.push(Event("Send email", 1, now + 3600));
    eventQueue.push(Event("Critical system update", 10, now + 7200));
    eventQueue.push(Event("Database backup", 5, now + 1800));
    eventQueue.push(Event("Team meeting", 3, now + 900));
    eventQueue.push(Event("Emergency alert", 10, now + 300)); // Same priority but earlier

    cout << "\nEvent schedule (by priority, then time):" << endl;
    while (!eventQueue.empty())
    {
        Event e = eventQueue.top();
        eventQueue.pop();

        // Convert time to readable format
        char timeStr[26];
        ctime_r(&e.time, timeStr);
        timeStr[24] = '\0'; // Remove newline

        cout << "Priority " << e.priority << ": " << e.name
             << " at " << timeStr << endl;
    }

    // Example 3: Graph representation using adjacency list
    unordered_map<string, vector<pair<string, int>>> graph;

    // Add edges (source, destination, weight)
    graph["A"].push_back({"B", 5});
    graph["A"].push_back({"C", 3});
    graph["B"].push_back({"C", 2});
    graph["B"].push_back({"D", 4});
    graph["C"].push_back({"D", 1});
    graph["D"].push_back({"A", 8});

    cout << "\nGraph representation (adjacency list):" << endl;
    for (const auto &node : graph)
    {
        cout << node.first << " -> ";
        for (const auto &edge : node.second)
        {
            cout << "(" << edge.first << ", weight: " << edge.second << ") ";
        }
        cout << endl;
    }

    // Example 4: LRU Cache implementation
    class LRUCache
    {
    private:
        int capacity;
        list<pair<int, int>> cache;                             // (key, value) pairs
        unordered_map<int, list<pair<int, int>>::iterator> map; // key -> iterator

    public:
        LRUCache(int cap) : capacity(cap) {}

        int get(int key)
        {
            auto it = map.find(key);
            if (it == map.end())
            {
                return -1; // Not found
            }

            // Move to front (most recently used)
            cache.splice(cache.begin(), cache, it->second);
            return it->second->second; // Return value
        }

        void put(int key, int value)
        {
            auto it = map.find(key);

            // Key exists, update value and move to front
            if (it != map.end())
            {
                it->second->second = value;
                cache.splice(cache.begin(), cache, it->second);
                return;
            }

            // Cache is full, remove least recently used
            if (cache.size() == capacity)
            {
                int lru_key = cache.back().first;
                cache.pop_back();
                map.erase(lru_key);
            }

            // Add new key-value pair to front
            cache.push_front({key, value});
            map[key] = cache.begin();
        }

        void display()
        {
            cout << "LRU Cache (most recent first): ";
            for (const auto &pair : cache)
            {
                cout << "(" << pair.first << ":" << pair.second << ") ";
            }
            cout << endl;
        }
    };

    cout << "\nLRU Cache example:" << endl;
    LRUCache lruCache(3); // Capacity of 3

    lruCache.put(1, 10);
    lruCache.display();

    lruCache.put(2, 20);
    lruCache.display();

    lruCache.put(3, 30);
    lruCache.display();

    cout << "get(2): " << lruCache.get(2) << endl;
    lruCache.display(); // 2 should be most recent now

    lruCache.put(4, 40); // Should evict key 1
    lruCache.display();

    cout << "get(1): " << lruCache.get(1) << endl; // Should return -1 (not found)
}

// ======== MAIN FUNCTION ========

int main()
{
    // Vector basics and operations
    vectorBasics();
    vectorOperations();
    vectorMemoryManagement();

    // Other STL containers
    listExample();
    dequeExample();
    setExample();
    mapExample();
    unordered_mapExample();
    stackExample();
    queueExample();
    priorityQueueExample();

    // STL algorithms
    algorithmExamples();

    // Pitfalls and best practices
    iteratorInvalidation();
    inefficientVectorUsage();
    wrongContainerChoice();

    // Practice problem solution
    practiceProblems();

    cout << "\n===== LEETCODE PRACTICE PROBLEMS =====\n";
    cout << "To further practice these concepts, try these LeetCode problems:\n";
    cout << "\nEasy:" << endl;
    cout << "- #1: Two Sum (Hash Map)" << endl;
    cout << "- #20: Valid Parentheses (Stack)" << endl;
    cout << "- #217: Contains Duplicate (Set/Hash Set)" << endl;
    cout << "- #706: Design HashMap (Implement your own)" << endl;

    cout << "\nMedium:" << endl;
    cout << "- #49: Group Anagrams (Hash Map)" << endl;
    cout << "- #380: Insert Delete GetRandom O(1) (Vector + Hash Map)" << endl;
    cout << "- #146: LRU Cache (List + Hash Map)" << endl;
    cout << "- #347: Top K Frequent Elements (Hash Map + Heap)" << endl;

    cout << "\nHard:" << endl;
    cout << "- #460: LFU Cache (Multiple Maps + Lists)" << endl;

    // Advanced container examples
    containerCombinations();
    advancedVectorOperations();
    realWorldExamples();

    // More LeetCode recommendations based on these advanced topics
    cout << "\n===== ADDITIONAL LEETCODE PRACTICE PROBLEMS =====\n";
    cout << "Based on the advanced topics we've covered:\n";

    cout << "\nMedium:" << endl;
    cout << "- #215: Kth Largest Element in an Array (nth_element/priority_queue)" << endl;
    cout << "- #75: Sort Colors (partition)" << endl;
    cout << "- #56: Merge Intervals (vector of pairs + sorting)" << endl;
    cout << "- #251: Flatten 2D Vector (vector of vectors)" << endl;

    cout << "\nHard:" << endl;
    cout << "- #295: Find Median from Data Stream (priority queue)" << endl;
    cout << "- #239: Sliding Window Maximum (deque)" << endl;
    cout << "- #23: Merge k Sorted Lists (priority queue)" << endl;
    cout << "- #336: Palindrome Pairs (hash map + string manipulation)" << endl;

    return 0;
}