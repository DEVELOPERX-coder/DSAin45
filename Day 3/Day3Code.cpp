// Array Techniques in C++ - Day 3 of #DSAin45
// Let's explore practical applications of array techniques!

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ======== ARRAY BASICS AND OPERATIONS ========

void arrayBasics()
{
    cout << "\n===== ARRAY BASICS =====\n";

    // Static array declaration and initialization
    int staticArray[5] = {10, 20, 30, 40, 50};

    // Accessing elements
    cout << "staticArray[2] = " << staticArray[2] << endl;

    // Modifying elements
    staticArray[1] = 25;
    cout << "After modification, staticArray[1] = " << staticArray[1] << endl;

    // Getting array size (for static arrays)
    int size = sizeof(staticArray) / sizeof(staticArray[0]);
    cout << "Size of static array: " << size << endl;

    // Static 2D array
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}};

    // Accessing 2D array elements
    cout << "matrix[1][2] = " << matrix[1][2] << endl;

    // Dynamic array (vector)
    vector<int> dynamicArray = {10, 20, 30, 40, 50};

    // Vector operations
    dynamicArray.push_back(60); // Add element to end
    cout << "After push_back, last element = " << dynamicArray.back() << endl;
    cout << "Vector size: " << dynamicArray.size() << endl;
    cout << "Vector capacity: " << dynamicArray.capacity() << endl;

    // Insert at specific position
    dynamicArray.insert(dynamicArray.begin() + 2, 25);
    cout << "After insertion at index 2: ";
    for (int num : dynamicArray)
    {
        cout << num << " ";
    }
    cout << endl;

    // Remove element
    dynamicArray.erase(dynamicArray.begin() + 3);
    cout << "After erasing element at index 3: ";
    for (int num : dynamicArray)
    {
        cout << num << " ";
    }
    cout << endl;

    // 2D vector
    vector<vector<int>> grid = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    cout << "grid[2][0] = " << grid[2][0] << endl;

    // Adding a row
    grid.push_back({10, 11, 12});
    cout << "Grid size after adding row: " << grid.size() << " x "
         << grid[0].size() << endl;
}

// ======== TWO-POINTER TECHNIQUE ========

// Example 1: Find a pair that sums to target in a sorted array
bool findPairWithSum(const vector<int> &arr, int target)
{
    int left = 0;
    int right = arr.size() - 1;

    while (left < right)
    {
        int currentSum = arr[left] + arr[right];

        if (currentSum == target)
        {
            cout << "Found pair: " << arr[left] << " + " << arr[right]
                 << " = " << target << endl;
            return true;
        }
        else if (currentSum < target)
        {
            left++; // Need a larger sum, move left pointer right
        }
        else
        {
            right--; // Need a smaller sum, move right pointer left
        }
    }

    cout << "No pair found that sums to " << target << endl;
    return false;
}

// Example 2: Remove duplicates from sorted array in-place
int removeDuplicates(vector<int> &nums)
{
    if (nums.empty())
        return 0;

    int writePointer = 1; // Start from the second element

    for (int readPointer = 1; readPointer < nums.size(); readPointer++)
    {
        // If current element is different from the previous
        if (nums[readPointer] != nums[readPointer - 1])
        {
            nums[writePointer] = nums[readPointer]; // Copy to write position
            writePointer++;
        }
        // If it's a duplicate, readPointer advances but writePointer doesn't
    }

    return writePointer; // New length of array without duplicates
}

// ======== SLIDING WINDOW TECHNIQUE ========

// Example 1: Find maximum sum subarray of size k
int maxSumSubarray(const vector<int> &arr, int k)
{
    if (arr.size() < k)
    {
        cout << "Array size is less than window size" << endl;
        return -1;
    }

    // Compute sum of first window
    int maxSum = 0;
    int windowSum = 0;

    for (int i = 0; i < k; i++)
    {
        windowSum += arr[i];
    }

    maxSum = windowSum;

    // Slide window and update maxSum
    for (int i = k; i < arr.size(); i++)
    {
        // Add next element and remove first element of previous window
        windowSum = windowSum - arr[i - k] + arr[i];
        maxSum = max(maxSum, windowSum);
    }

    return maxSum;
}

// Example 2: Longest subarray with sum less than or equal to target
int longestSubarrayWithSum(const vector<int> &arr, int target)
{
    int maxLength = 0;
    int currentSum = 0;
    int left = 0;

    for (int right = 0; right < arr.size(); right++)
    {
        currentSum += arr[right]; // Expand window

        // Shrink window from left if sum exceeds target
        while (currentSum > target && left <= right)
        {
            currentSum -= arr[left];
            left++;
        }

        // Update maximum length if current window is valid
        maxLength = max(maxLength, right - left + 1);
    }

    return maxLength;
}

// ======== PREFIX SUM TECHNIQUE ========

// Build prefix sum array
vector<int> buildPrefixSum(const vector<int> &arr)
{
    vector<int> prefix(arr.size());
    prefix[0] = arr[0];

    for (int i = 1; i < arr.size(); i++)
    {
        prefix[i] = prefix[i - 1] + arr[i];
    }

    return prefix;
}

// Get sum of range [left, right] in O(1) time
int rangeSum(const vector<int> &prefix, int left, int right)
{
    if (left == 0)
    {
        return prefix[right];
    }
    else
    {
        return prefix[right] - prefix[left - 1];
    }
}

// ======== ARRAY ROTATION TECHNIQUE ========

// Rotate array to the right by k steps
// Solution for the practice problem in the post
void rotateArray(vector<int> &nums, int k)
{
    int n = nums.size();
    k = k % n; // Handle case where k > n

    if (k == 0)
        return; // No rotation needed

    // Solution 1: Using extra space O(n)
    vector<int> temp(n);
    for (int i = 0; i < n; i++)
    {
        temp[(i + k) % n] = nums[i];
    }
    nums = temp;
}

// More efficient solution: O(n) time and O(1) space
void rotateArrayInPlace(vector<int> &nums, int k)
{
    int n = nums.size();
    k = k % n; // Handle case where k > n

    if (k == 0)
        return; // No rotation needed

    // Reverse the entire array
    reverse(nums.begin(), nums.end());

    // Reverse the first k elements
    reverse(nums.begin(), nums.begin() + k);

    // Reverse the remaining n-k elements
    reverse(nums.begin() + k, nums.end());
}

// ======== MAIN FUNCTION WITH DEMONSTRATIONS ========

int main()
{
    // Array basics
    arrayBasics();

    cout << "\n===== TWO-POINTER TECHNIQUE =====\n";
    vector<int> sortedArray = {1, 4, 6, 8, 10, 15, 20};
    findPairWithSum(sortedArray, 18);
    findPairWithSum(sortedArray, 13);

    vector<int> duplicates = {1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5};
    int newLength = removeDuplicates(duplicates);
    cout << "After removing duplicates: ";
    for (int i = 0; i < newLength; i++)
    {
        cout << duplicates[i] << " ";
    }
    cout << endl;

    cout << "\n===== SLIDING WINDOW TECHNIQUE =====\n";
    vector<int> windowArray = {2, 5, 1, 8, 2, 9, 1};
    cout << "Maximum sum of subarray of size 3: "
         << maxSumSubarray(windowArray, 3) << endl;

    vector<int> subarraySum = {3, 1, 2, 7, 4, 2, 1, 1, 5};
    cout << "Longest subarray with sum <= 8: "
         << longestSubarrayWithSum(subarraySum, 8) << endl;

    cout << "\n===== PREFIX SUM TECHNIQUE =====\n";
    vector<int> nums = {4, 2, 3, 1, 5, 6, 7};
    vector<int> prefix = buildPrefixSum(nums);

    cout << "Prefix sum array: ";
    for (int num : prefix)
    {
        cout << num << " ";
    }
    cout << endl;

    cout << "Sum of range [2, 4]: " << rangeSum(prefix, 2, 4) << endl;
    cout << "Sum of range [0, 3]: " << rangeSum(prefix, 0, 3) << endl;
    cout << "Sum of range [1, 5]: " << rangeSum(prefix, 1, 5) << endl;

    cout << "\n===== ARRAY ROTATION =====\n";
    vector<int> rotateArray1 = {1, 2, 3, 4, 5, 6, 7};
    rotateArray(rotateArray1, 3);
    cout << "After rotation by 3 (using extra space): ";
    for (int num : rotateArray1)
    {
        cout << num << " ";
    }
    cout << endl;

    vector<int> rotateArray2 = {1, 2, 3, 4, 5, 6, 7};
    rotateArrayInPlace(rotateArray2, 3);
    cout << "After rotation by 3 (in-place): ";
    for (int num : rotateArray2)
    {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}