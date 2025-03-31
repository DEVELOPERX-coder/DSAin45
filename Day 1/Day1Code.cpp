// First C++ Program for DSA Journey - DETAILED BREAKDOWN

// Include necessary libraries
#include <iostream> // For input/output operations
#include <vector>   // For our first data structure - dynamic array
#include <string>   // For string manipulation

int main()
{
    // ======== BASIC OUTPUT ========

    // Print welcome message
    // std::cout = "character output" - sends data to standard output (console)
    // std::endl = end line - adds newline character and flushes the output buffer
    std::cout << "Welcome to Day 1 of #DSAin45 Challenge!" << std::endl;

    // ======== VARIABLES ========

    // Basic variable declaration and initialization
    int number = 42;                           // Integer
    double pi = 3.14159;                       // Floating-point number
    std::string message = "DSA is important!"; // String
    bool isImportant = true;                   // Boolean
    char grade = 'A';                          // Character

    // ======== OUR FIRST DATA STRUCTURE: VECTOR ========

    // Vector = dynamic array (resizes automatically)
    // Angle brackets <type> specify what type of data it will store
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // ======== ACCESSING ELEMENTS ========

    // Access individual elements using index (zero-based)
    std::cout << "First element: " << numbers[0] << std::endl; // Outputs: 1
    std::cout << "Third element: " << numbers[2] << std::endl; // Outputs: 3

    // ======== VECTOR OPERATIONS ========

    // Add an element to the end
    numbers.push_back(6); // Vector now contains: {1, 2, 3, 4, 5, 6}

    // Get vector size
    std::cout << "Vector size: " << numbers.size() << std::endl; // Outputs: 6

    // ======== ITERATION ========

    // Method 1: Classic for loop
    std::cout << "Elements (classic loop): ";
    for (int i = 0; i < numbers.size(); i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Method 2: Range-based for loop (C++11 and later)
    // This is more readable and less error-prone
    std::cout << "Elements (range loop): ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // ======== SIMPLE ALGORITHM ========

    // Calculate sum using iteration
    int sum = 0;
    for (int num : numbers)
    {
        sum += num; // Shorthand for: sum = sum + num
    }
    std::cout << "Sum of all elements: " << sum << std::endl;

    // ======== CONDITIONAL STATEMENTS ========

    // If-else statement
    if (sum > 20)
    {
        std::cout << "That's a large sum!" << std::endl;
    }
    else
    {
        std::cout << "That's a small sum!" << std::endl;
    }

    // ======== TIME COMPLEXITY PREVIEW ========

    std::cout << "\n--- Time Complexity Preview ---" << std::endl;
    std::cout << "Accessing an element: O(1) - Constant time" << std::endl;
    std::cout << "Calculating sum: O(n) - Linear time" << std::endl;
    std::cout << "Adding element to end: O(1) amortized - Nearly constant time" << std::endl;

    // ======== NEXT STEPS ========

    std::cout << "\nComing up on Day 2: Big O notation and complexity analysis!" << std::endl;

    // Return 0 to indicate successful execution
    return 0;
}

/* COMPILE AND RUN:
   1. Save this as day1.cpp
   2. Compile: g++ -std=c++11 day1.cpp -o day1
   3. Run: ./day1 (Unix/Mac) or day1.exe (Windows)
*/