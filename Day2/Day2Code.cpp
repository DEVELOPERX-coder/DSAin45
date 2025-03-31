// Time Complexity Analysis Examples - Day 2 of #DSAin45
// Let's analyze the time complexity of various code snippets!

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

// Example 1: O(1) - Constant Time
// Accessing an array element by index
int getElement(const vector<int> &arr, int index)
{
    // This operation takes the same amount of time regardless of array size
    // It's a direct memory access calculation: base_address + (index * element_size)
    return arr[index]; // O(1)
}
// Analysis: Time complexity is O(1) because array access is constant time

// Example 2: O(n) - Linear Time
// Finding the maximum element in an array
int findMax(const vector<int> &arr)
{
    if (arr.empty())
        return -1;

    int max_val = arr[0]; // O(1)

    // We have to check every element in the worst case
    for (int i = 1; i < arr.size(); i++)
    { // This loop runs n-1 times
        if (arr[i] > max_val)
        {                     // O(1) comparison
            max_val = arr[i]; // O(1) assignment
        }
    }

    return max_val; // O(1)
}
// Analysis: Time complexity is O(n) because we examine each element once

// Example 3: O(log n) - Logarithmic Time
// Binary search implementation
bool binarySearch(const vector<int> &sortedArr, int target)
{
    int left = 0;                     // O(1)
    int right = sortedArr.size() - 1; // O(1)

    // Each iteration eliminates half of the remaining elements
    while (left <= right)
    {                                        // This loop runs log(n) times
        int mid = left + (right - left) / 2; // O(1)

        if (sortedArr[mid] == target)
        {                // O(1)
            return true; // Found it!
        }

        if (sortedArr[mid] < target)
        {                   // O(1)
            left = mid + 1; // Move right, eliminating left half
        }
        else
        {
            right = mid - 1; // Move left, eliminating right half
        }
    }

    return false; // Not found
}
// Analysis: Time complexity is O(log n) because we eliminate half the search space in each step

// Example 4: O(n log n) - Linearithmic Time
// Implementing a merge sort (simplified version)
void mergeSort(vector<int> &arr, int left, int right)
{
    if (left >= right)
        return; // Base case: array of size 0 or 1

    int mid = left + (right - left) / 2;

    // Recursively sort both halves
    mergeSort(arr, left, mid);      // T(n/2)
    mergeSort(arr, mid + 1, right); // T(n/2)

    // Merge the sorted halves (this costs O(n))
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    // Merge two sorted arrays (this loop is O(n))
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }

    // Copy remaining elements
    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }

    while (j <= right)
    {
        temp[k++] = arr[j++];
    }

    // Copy back to original array
    for (int p = 0; p < k; p++)
    {
        arr[left + p] = temp[p];
    }
}
// Analysis: T(n) = 2T(n/2) + O(n)
// By the Master Theorem, this gives us O(n log n)

// Example 5: O(n²) - Quadratic Time
// Bubble sort implementation
void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    bool swapped;

    // This outer loop can run up to n times
    for (int i = 0; i < n - 1; i++)
    {
        swapped = false;

        // This inner loop runs n-i-1 times
        for (int j = 0; j < n - i - 1; j++)
        {
            // Compare adjacent elements
            if (arr[j] > arr[j + 1])
            {
                // Swap them if they're in the wrong order
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no swapping occurred in this pass, array is sorted
        if (!swapped)
            break;
    }
}
// Analysis: In the worst case, both loops run fully, giving us O(n²)

// Example 6: O(2^n) - Exponential Time
// Calculating Fibonacci numbers recursively (naive approach)
int fibonacci(int n)
{
    // Base cases
    if (n <= 1)
        return n;

    // Recursive case: we make TWO recursive calls for each non-base case
    return fibonacci(n - 1) + fibonacci(n - 2);
}
// Analysis: This is O(2^n) because each call branches into two more calls
// We're solving the same subproblems repeatedly (hence the exponential blowup)

// Example 7: O(n!) - Factorial Time
// Generate all permutations of an array (simplified)
void generatePermutations(vector<int> &arr, int start)
{
    if (start == arr.size() - 1)
    {
        // We've reached a complete permutation
        // In a real implementation, we would do something with this permutation
        return;
    }

    for (int i = start; i < arr.size(); i++)
    {
        // Swap to create a new permutation
        swap(arr[start], arr[i]);

        // Recursively generate permutations for the rest of the array
        generatePermutations(arr, start + 1);

        // Backtrack to restore the array
        swap(arr[start], arr[i]);
    }
}
// Analysis: For n elements, we have n choices for the first position,
// n-1 for the second, and so on, giving us n! permutations.

// Example from the post: Duplicate checking
// Time Complexity: O(n), Space Complexity: O(n)
bool containsDuplicate(vector<int> &nums)
{
    unordered_set<int> seen; // O(1) space initially
    for (int num : nums)
    { // Loop runs n times
        if (seen.count(num) > 0)
        {                // O(1) lookup in hash set (average case)
            return true; // Found a duplicate!
        }
        seen.insert(num); // O(1) insertion (average case)
    } // Space grows to O(n) in worst case
    return false; // No duplicates found
}
// Analysis: Time complexity is O(n) because we process each element once
// Space complexity is O(n) because in worst case we store all elements in the set

int main()
{
    // Example usage of our functions
    vector<int> arr = {64, 25, 12, 22, 11};

    cout << "Max element: " << findMax(arr) << endl;

    vector<int> sortedArr = {1, 2, 3, 5, 8, 13, 21};
    cout << "Binary search for 8: " << (binarySearch(sortedArr, 8) ? "Found" : "Not found") << endl;
    cout << "Binary search for 7: " << (binarySearch(sortedArr, 7) ? "Found" : "Not found") << endl;

    vector<int> unsortedArr = {38, 27, 43, 3, 9, 82, 10};
    mergeSort(unsortedArr, 0, unsortedArr.size() - 1);
    cout << "After merge sort: ";
    for (int num : unsortedArr)
        cout << num << " ";
    cout << endl;

    // Note: We don't actually run the exponential or factorial examples
    // as they would take too long for even moderate input sizes!

    return 0;
}