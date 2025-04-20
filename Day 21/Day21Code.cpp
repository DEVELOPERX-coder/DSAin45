#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <cmath>
#include <chrono>
#include <string>
#include <iomanip>
#include <random>

/**
 * @file Week3ChallengeAndRecap.cpp
 * @brief Comprehensive implementation of algorithms from Week 3, integrated challenges, and performance analysis
 *
 * This file demonstrates combined applications of:
 * - Recursion Fundamentals (Day 15)
 * - Searching Algorithms (Day 16)
 * - Basic Sorting Algorithms (Day 17)
 * - Efficient Sorting Algorithms (Day 18)
 * - Divide and Conquer (Day 19)
 * - Two-Pointer Technique (Day 20)
 *
 * It includes integrated challenge problems, performance optimizations,
 * and algorithm selection examples.
 *
 * @author #DSAin45 Series
 * @date Day 21
 */

// ============================================================================
// Helper Functions for Performance Measurement
// ============================================================================

/**
 * Measure the execution time of a function
 *
 * @param func The function to measure
 * @param args The arguments to pass to the function
 * @return The execution time in microseconds
 */
template <typename Func, typename... Args>
long long measureExecutionTime(Func func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

/**
 * Generate a random vector of integers
 *
 * @param size The size of the vector
 * @param min The minimum value
 * @param max The maximum value
 * @return A vector of random integers
 */
std::vector<int> generateRandomVector(int size, int min, int max)
{
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    for (int i = 0; i < size; i++)
    {
        vec[i] = distrib(gen);
    }

    return vec;
}

/**
 * Print a vector of integers
 *
 * @param vec The vector to print
 * @param label An optional label to print before the vector
 */
void printVector(const std::vector<int> &vec, const std::string &label = "")
{
    if (!label.empty())
    {
        std::cout << label << ": ";
    }

    std::cout << "[";
    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i];
        if (i < vec.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

/**
 * Print a 2D vector of integers
 *
 * @param vec The 2D vector to print
 * @param label An optional label to print before the vector
 */
void print2DVector(const std::vector<std::vector<int>> &vec, const std::string &label = "")
{
    if (!label.empty())
    {
        std::cout << label << ": " << std::endl;
    }

    std::cout << "[" << std::endl;
    for (const auto &row : vec)
    {
        std::cout << "  [";
        for (size_t i = 0; i < row.size(); i++)
        {
            std::cout << row[i];
            if (i < row.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

// ============================================================================
// Recursion (Day 15) + Binary Search (Day 16) Combined
// ============================================================================

/**
 * Find a peak element in an array (an element greater than both neighbors)
 * Uses both recursive approach and binary search principles
 *
 * @param nums The input array
 * @param left The left boundary of the search space
 * @param right The right boundary of the search space
 * @return The index of a peak element
 */
int findPeakElement(const std::vector<int> &nums, int left, int right)
{
    // Base case
    if (left == right)
    {
        return left;
    }

    int mid = left + (right - left) / 2;

    // If mid is a peak element
    if ((mid == 0 || nums[mid] > nums[mid - 1]) &&
        (mid == nums.size() - 1 || nums[mid] > nums[mid + 1]))
    {
        return mid;
    }

    // If mid is in an ascending sequence, peak is in the right half
    if (mid < nums.size() - 1 && nums[mid] < nums[mid + 1])
    {
        return findPeakElement(nums, mid + 1, right);
    }

    // Otherwise, peak is in the left half
    return findPeakElement(nums, left, mid);
}

/**
 * Wrapper function for findPeakElement
 *
 * @param nums The input array
 * @return The index of a peak element
 */
int findPeakElementWrapper(const std::vector<int> &nums)
{
    return findPeakElement(nums, 0, nums.size() - 1);
}

// ============================================================================
// Efficient Sorting (Day 18) + Two-Pointer (Day 20) Combined
// ============================================================================

/**
 * Merge Intervals Problem
 * Given a collection of intervals, merge all overlapping intervals.
 * Uses sorting and two-pointer technique.
 *
 * @param intervals The collection of intervals
 * @return The merged intervals
 *
 * Time Complexity: O(n log n) due to sorting
 * Space Complexity: O(1) excluding the output
 */
std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> &intervals)
{
    if (intervals.empty())
    {
        return {};
    }

    // Sort intervals by start time (using lambda function)
    std::sort(intervals.begin(), intervals.end(),
              [](const std::vector<int> &a, const std::vector<int> &b)
              {
                  return a[0] < b[0];
              });

    std::vector<std::vector<int>> result;
    result.push_back(intervals[0]);

    // Iterate through intervals, merging as needed
    for (size_t i = 1; i < intervals.size(); i++)
    {
        // Get reference to the last interval in our result
        std::vector<int> &lastInterval = result.back();

        // If current interval overlaps with the last interval
        if (intervals[i][0] <= lastInterval[1])
        {
            // Merge by updating the end time
            lastInterval[1] = std::max(lastInterval[1], intervals[i][1]);
        }
        else
        {
            // No overlap, add the current interval to result
            result.push_back(intervals[i]);
        }
    }

    return result;
}

// ============================================================================
// Quick Select (Divide & Conquer Day 19) Implementation
// ============================================================================

/**
 * Quick Select: Find the kth smallest element in an unsorted array
 * Uses the partition scheme from Quick Sort
 *
 * @param nums The input array
 * @param k The k value (1-based)
 * @return The kth smallest element
 *
 * Time Complexity: O(n) average case, O(n²) worst case
 * Space Complexity: O(log n) for the recursive call stack
 */
int quickSelect(std::vector<int> &nums, int k)
{
    // Convert to 0-indexed
    k = k - 1;

    std::function<int(int, int)> findKthSmallest = [&](int left, int right)
    {
        // Base case: if the list contains only one element
        if (left == right)
        {
            return nums[left];
        }

        // Select pivot (using random pivot to avoid worst case)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(left, right);
        int pivotIndex = distrib(gen);

        // Move pivot to the rightmost position
        std::swap(nums[pivotIndex], nums[right]);

        // Partition the array
        int storeIndex = left;
        for (int i = left; i < right; i++)
        {
            if (nums[i] < nums[right])
            {
                std::swap(nums[i], nums[storeIndex]);
                storeIndex++;
            }
        }

        // Move pivot to its final sorted position
        std::swap(nums[storeIndex], nums[right]);

        // If the pivot is the kth element, return it
        if (storeIndex == k)
        {
            return nums[storeIndex];
        }
        // If k is smaller, look in the left subarray
        else if (storeIndex > k)
        {
            return findKthSmallest(left, storeIndex - 1);
        }
        // If k is larger, look in the right subarray
        else
        {
            return findKthSmallest(storeIndex + 1, right);
        }
    };

    return findKthSmallest(0, nums.size() - 1);
}

// ============================================================================
// Sorting Algorithm Implementations from Week 3
// ============================================================================

namespace SortingAlgorithms
{

    // Day 17: Basic Sorting Algorithms

    /**
     * Bubble Sort
     * Repeatedly step through the list, comparing and swapping adjacent elements
     *
     * @param arr The array to sort
     *
     * Time Complexity: O(n²)
     * Space Complexity: O(1)
     */
    void bubbleSort(std::vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            bool swapped = false;

            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }

            // If no swaps were made in this pass, the array is already sorted
            if (!swapped)
            {
                break;
            }
        }
    }

    /**
     * Selection Sort
     * Repeatedly find the minimum element and put it at the beginning
     *
     * @param arr The array to sort
     *
     * Time Complexity: O(n²)
     * Space Complexity: O(1)
     */
    void selectionSort(std::vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            int minIndex = i;

            // Find the minimum element in the unsorted part
            for (int j = i + 1; j < n; j++)
            {
                if (arr[j] < arr[minIndex])
                {
                    minIndex = j;
                }
            }

            // Swap the found minimum element with the element at index i
            if (minIndex != i)
            {
                std::swap(arr[i], arr[minIndex]);
            }
        }
    }

    /**
     * Insertion Sort
     * Build the sorted array one element at a time
     *
     * @param arr The array to sort
     *
     * Time Complexity: O(n²)
     * Space Complexity: O(1)
     */
    void insertionSort(std::vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 1; i < n; i++)
        {
            int key = arr[i];
            int j = i - 1;

            // Move elements that are greater than key to one position ahead
            while (j >= 0 && arr[j] > key)
            {
                arr[j + 1] = arr[j];
                j--;
            }

            arr[j + 1] = key;
        }
    }

    // Day 18: Efficient Sorting Algorithms

    /**
     * Merge Sort
     * Divide the array into halves, sort each half, then merge them
     *
     * @param arr The array to sort
     *
     * Time Complexity: O(n log n)
     * Space Complexity: O(n)
     */
    void mergeSort(std::vector<int> &arr)
    {
        std::function<void(int, int)> mergeSortRecursive = [&](int left, int right)
        {
            if (left >= right)
            {
                return;
            }

            int mid = left + (right - left) / 2;

            // Sort first and second halves
            mergeSortRecursive(left, mid);
            mergeSortRecursive(mid + 1, right);

            // Merge the sorted halves
            std::vector<int> temp(right - left + 1);
            int i = left;
            int j = mid + 1;
            int k = 0;

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

            while (i <= mid)
            {
                temp[k++] = arr[i++];
            }

            while (j <= right)
            {
                temp[k++] = arr[j++];
            }

            // Copy back to the original array
            for (i = 0; i < k; i++)
            {
                arr[left + i] = temp[i];
            }
        };

        mergeSortRecursive(0, arr.size() - 1);
    }

    /**
     * Quick Sort
     * Choose a pivot, partition the array, then recursively sort the subarrays
     *
     * @param arr The array to sort
     *
     * Time Complexity: O(n log n) average, O(n²) worst case
     * Space Complexity: O(log n) for the recursive call stack
     */
    void quickSort(std::vector<int> &arr)
    {
        std::function<void(int, int)> quickSortRecursive = [&](int low, int high)
        {
            if (low < high)
            {
                // Choose a pivot (using the rightmost element for simplicity)
                int pivot = arr[high];

                // Partition the array
                int i = low - 1;

                for (int j = low; j < high; j++)
                {
                    if (arr[j] <= pivot)
                    {
                        i++;
                        std::swap(arr[i], arr[j]);
                    }
                }

                // Place the pivot in its correct position
                std::swap(arr[i + 1], arr[high]);
                int pivotIndex = i + 1;

                // Recursively sort the subarrays
                quickSortRecursive(low, pivotIndex - 1);
                quickSortRecursive(pivotIndex + 1, high);
            }
        };

        quickSortRecursive(0, arr.size() - 1);
    }

    /**
     * Quick Sort with Random Pivot
     * Similar to Quick Sort but uses a random pivot to avoid worst-case scenarios
     *
     * @param arr The array to sort
     *
     * Time Complexity: O(n log n) average, O(n²) worst case (but less likely)
     * Space Complexity: O(log n) for the recursive call stack
     */
    void quickSortRandomPivot(std::vector<int> &arr)
    {
        std::function<void(int, int)> quickSortRecursive = [&](int low, int high)
        {
            if (low < high)
            {
                // Choose a random pivot
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(low, high);
                int pivotIndex = distrib(gen);

                // Move pivot to the end for the standard partition scheme
                std::swap(arr[pivotIndex], arr[high]);

                // Partition the array
                int pivot = arr[high];
                int i = low - 1;

                for (int j = low; j < high; j++)
                {
                    if (arr[j] <= pivot)
                    {
                        i++;
                        std::swap(arr[i], arr[j]);
                    }
                }

                // Place pivot in its correct position
                std::swap(arr[i + 1], arr[high]);
                pivotIndex = i + 1;

                // Recursively sort the subarrays
                quickSortRecursive(low, pivotIndex - 1);
                quickSortRecursive(pivotIndex + 1, high);
            }
        };

        quickSortRecursive(0, arr.size() - 1);
    }

} // namespace SortingAlgorithms

// ============================================================================
// Searching Algorithm Implementations from Week 3
// ============================================================================

namespace SearchingAlgorithms
{

    /**
     * Linear Search
     * Sequentially check each element of the list until a match is found
     *
     * @param arr The array to search in
     * @param target The element to search for
     * @return The index of the target element, or -1 if not found
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    int linearSearch(const std::vector<int> &arr, int target)
    {
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i] == target)
            {
                return i;
            }
        }

        return -1;
    }

    /**
     * Binary Search (Recursive)
     * Search a sorted array by repeatedly dividing the search interval in half
     *
     * @param arr The sorted array to search in
     * @param target The element to search for
     * @param left The left boundary of the search space
     * @param right The right boundary of the search space
     * @return The index of the target element, or -1 if not found
     *
     * Time Complexity: O(log n)
     * Space Complexity: O(log n) for the recursive call stack
     */
    int binarySearchRecursive(const std::vector<int> &arr, int target, int left, int right)
    {
        if (left > right)
        {
            return -1;
        }

        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] > target)
        {
            return binarySearchRecursive(arr, target, left, mid - 1);
        }
        else
        {
            return binarySearchRecursive(arr, target, mid + 1, right);
        }
    }

    /**
     * Binary Search (Iterative)
     * Search a sorted array by repeatedly dividing the search interval in half
     *
     * @param arr The sorted array to search in
     * @param target The element to search for
     * @return The index of the target element, or -1 if not found
     *
     * Time Complexity: O(log n)
     * Space Complexity: O(1)
     */
    int binarySearchIterative(const std::vector<int> &arr, int target)
    {
        int left = 0;
        int right = arr.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (arr[mid] == target)
            {
                return mid;
            }
            else if (arr[mid] > target)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        return -1;
    }

    /**
     * Binary Search for the first occurrence of an element
     *
     * @param arr The sorted array to search in
     * @param target The element to search for
     * @return The index of the first occurrence of the target, or -1 if not found
     *
     * Time Complexity: O(log n)
     * Space Complexity: O(1)
     */
    int binarySearchFirstOccurrence(const std::vector<int> &arr, int target)
    {
        int left = 0;
        int right = arr.size() - 1;
        int result = -1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (arr[mid] == target)
            {
                result = mid;
                right = mid - 1; // Continue searching in the left half
            }
            else if (arr[mid] > target)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        return result;
    }

    /**
     * Binary Search for the last occurrence of an element
     *
     * @param arr The sorted array to search in
     * @param target The element to search for
     * @return The index of the last occurrence of the target, or -1 if not found
     *
     * Time Complexity: O(log n)
     * Space Complexity: O(1)
     */
    int binarySearchLastOccurrence(const std::vector<int> &arr, int target)
    {
        int left = 0;
        int right = arr.size() - 1;
        int result = -1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (arr[mid] == target)
            {
                result = mid;
                left = mid + 1; // Continue searching in the right half
            }
            else if (arr[mid] > target)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        return result;
    }

} // namespace SearchingAlgorithms

// ============================================================================
// Two-Pointer Techniques from Week 3 (Day 20)
// ============================================================================

namespace TwoPointerTechniques
{

    /**
     * Two Sum (for sorted arrays)
     * Find two numbers that add up to a target
     *
     * @param arr The sorted array
     * @param target The target sum
     * @return A pair of indices of the two numbers, or (-1, -1) if not found
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    std::pair<int, int> twoSum(const std::vector<int> &arr, int target)
    {
        int left = 0;
        int right = arr.size() - 1;

        while (left < right)
        {
            int sum = arr[left] + arr[right];

            if (sum == target)
            {
                return {left, right};
            }
            else if (sum < target)
            {
                left++;
            }
            else
            {
                right--;
            }
        }

        return {-1, -1};
    }

    /**
     * Remove Duplicates from Sorted Array
     * Remove duplicates in-place and return the new length
     *
     * @param arr The sorted array
     * @return The new length after removing duplicates
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    int removeDuplicates(std::vector<int> &arr)
    {
        if (arr.empty())
        {
            return 0;
        }

        int slow = 0;

        for (int fast = 1; fast < arr.size(); fast++)
        {
            if (arr[fast] != arr[slow])
            {
                slow++;
                arr[slow] = arr[fast];
            }
        }

        return slow + 1;
    }

    /**
     * Longest Substring Without Repeating Characters
     * Find the length of the longest substring without repeating characters
     *
     * @param s The input string
     * @return The length of the longest substring without repeating characters
     *
     * Time Complexity: O(n)
     * Space Complexity: O(min(m, n)) where m is the size of the character set
     */
    int lengthOfLongestSubstring(const std::string &s)
    {
        std::unordered_map<char, int> charIndex;
        int left = 0;
        int maxLength = 0;

        for (int right = 0; right < s.length(); right++)
        {
            // If the character is already in the current window
            if (charIndex.find(s[right]) != charIndex.end() && charIndex[s[right]] >= left)
            {
                left = charIndex[s[right]] + 1;
            }

            charIndex[s[right]] = right;
            maxLength = std::max(maxLength, right - left + 1);
        }

        return maxLength;
    }

    /**
     * Container With Most Water
     * Find two lines that together with the x-axis form a container that holds the most water
     *
     * @param height The heights of the lines
     * @return The maximum amount of water the container can hold
     *
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    int maxArea(const std::vector<int> &height)
    {
        int left = 0;
        int right = height.size() - 1;
        int maxWater = 0;

        while (left < right)
        {
            // Calculate current container area
            int width = right - left;
            int h = std::min(height[left], height[right]);
            int area = width * h;

            maxWater = std::max(maxWater, area);

            // Move the pointer at the shorter line
            if (height[left] < height[right])
            {
                left++;
            }
            else
            {
                right--;
            }
        }

        return maxWater;
    }

} // namespace TwoPointerTechniques

// ============================================================================
// Integrated Challenge 1: K Closest Points to Origin
// ============================================================================

/**
 * K Closest Points to Origin
 * Find the k points closest to the origin (0, 0)
 *
 * This solution uses:
 * - Quick select (Divide and Conquer)
 * - Mathematical distance calculation
 *
 * @param points The array of points
 * @param k The number of closest points to find
 * @return The k closest points
 *
 * Time Complexity: O(n) average case
 * Space Complexity: O(1) excluding the output
 */
std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>> &points, int k)
{
    // Quick select approach
    std::function<int(int, int)> squaredDistance = [&](int i)
    {
        return points[i][0] * points[i][0] + points[i][1] * points[i][1];
    };

    std::function<int(int, int)> partition = [&](int left, int right)
    {
        // Choose a pivot (using random pivot for better performance)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(left, right);
        int pivotIndex = distrib(gen);
        int pivotDist = squaredDistance(pivotIndex);

        // Move pivot to the end
        std::swap(points[pivotIndex], points[right]);

        // Partition around the pivot distance
        int storeIndex = left;
        for (int i = left; i < right; i++)
        {
            if (squaredDistance(i) < pivotDist)
            {
                std::swap(points[i], points[storeIndex]);
                storeIndex++;
            }
        }

        // Move pivot to its final place
        std::swap(points[storeIndex], points[right]);
        return storeIndex;
    };

    std::function<void(int, int, int)> quickSelect = [&](int left, int right, int k)
    {
        if (left >= right)
        {
            return;
        }

        int pivotIndex = partition(left, right);

        if (pivotIndex == k - 1)
        {
            return;
        }
        else if (pivotIndex < k - 1)
        {
            quickSelect(pivotIndex + 1, right, k);
        }
        else
        {
            quickSelect(left, pivotIndex - 1, k);
        }
    };

    // Find the k closest points
    quickSelect(0, points.size() - 1, k);

    // Return the first k points (now sorted by distance)
    return std::vector<std::vector<int>>(points.begin(), points.begin() + k);
}

// ============================================================================
// Integrated Challenge 3: Longest Increasing Subsequence
// ============================================================================

/**
 * Longest Increasing Subsequence (Dynamic Programming approach)
 * Find the length of the longest strictly increasing subsequence
 *
 * @param nums The input array
 * @return The length of the longest increasing subsequence
 *
 * Time Complexity: O(n²)
 * Space Complexity: O(n)
 */
int lengthOfLIS_DP(const std::vector<int> &nums)
{
    if (nums.empty())
    {
        return 0;
    }

    int n = nums.size();
    // dp[i] = length of LIS ending at index i
    std::vector<int> dp(n, 1);

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (nums[i] > nums[j])
            {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
    }

    return *std::max_element(dp.begin(), dp.end());
}

/**
 * Longest Increasing Subsequence (Binary Search approach)
 * Find the length of the longest strictly increasing subsequence
 *
 * @param nums The input array
 * @return The length of the longest increasing subsequence
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */
int lengthOfLIS_BS(const std::vector<int> &nums)
{
    if (nums.empty())
    {
        return 0;
    }

    // tails[i] = smallest value that ends an increasing subsequence of length i+1
    std::vector<int> tails;

    for (int num : nums)
    {
        // Find the position where num should be placed
        auto it = std::lower_bound(tails.begin(), tails.end(), num);

        if (it == tails.end())
        {
            // num is larger than all elements in tails
            tails.push_back(num);
        }
        else
        {
            // Replace the smallest value that is >= num
            *it = num;
        }
    }

    return tails.size();
}

// ============================================================================
// Performance Optimization Case Studies
// ============================================================================

/**
 * Case Study 1: Count Frequency of Elements
 * Compare sorting approach vs. hash map approach
 */
void countFrequencyDemo()
{
    std::cout << "===== Count Frequency Case Study =====" << std::endl;

    // Generate test data
    std::vector<int> nums = generateRandomVector(10000, 1, 100);

    // Approach 1: Sort and Count
    auto sortAndCount = [](std::vector<int> nums)
    {
        std::sort(nums.begin(), nums.end());

        std::vector<std::pair<int, int>> result;

        for (int i = 0; i < nums.size(); i++)
        {
            int count = 1;
            while (i + 1 < nums.size() && nums[i] == nums[i + 1])
            {
                count++;
                i++;
            }
            result.push_back({nums[i], count});
        }

        return result;
    };

    // Approach 2: Hash Map
    auto hashMapCount = [](const std::vector<int> &nums)
    {
        std::unordered_map<int, int> freqMap;

        for (int num : nums)
        {
            freqMap[num]++;
        }

        std::vector<std::pair<int, int>> result;
        for (const auto &[num, count] : freqMap)
        {
            result.push_back({num, count});
        }

        return result;
    };

    // Benchmark
    long long sortTime = measureExecutionTime(sortAndCount, nums);
    long long hashTime = measureExecutionTime(hashMapCount, nums);

    std::cout << "Sort and Count: " << sortTime << " microseconds" << std::endl;
    std::cout << "Hash Map: " << hashTime << " microseconds" << std::endl;
    std::cout << "Speedup Factor: " << (double)sortTime / hashTime << "x" << std::endl;
    std::cout << std::endl;
}

/**
 * Case Study 2: Two Sum Problem
 * Compare brute force, hash map, and two-pointer approaches
 */
void twoSumDemo()
{
    std::cout << "===== Two Sum Case Study =====" << std::endl;

    // Generate test data
    std::vector<int> nums = generateRandomVector(1000, 1, 1000);
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end());

    int target = 1000; // A target that likely has solutions

    // Approach 1: Brute Force O(n²)
    auto bruteForce = [](const std::vector<int> &nums, int target)
    {
        for (int i = 0; i < nums.size(); i++)
        {
            for (int j = i + 1; j < nums.size(); j++)
            {
                if (nums[i] + nums[j] == target)
                {
                    return std::pair<int, int>(i, j);
                }
            }
        }
        return std::pair<int, int>(-1, -1);
    };

    // Approach 2: Hash Map O(n)
    auto hashMap = [](const std::vector<int> &nums, int target)
    {
        std::unordered_map<int, int> numMap;

        for (int i = 0; i < nums.size(); i++)
        {
            int complement = target - nums[i];

            if (numMap.find(complement) != numMap.end())
            {
                return std::pair<int, int>(numMap[complement], i);
            }

            numMap[nums[i]] = i;
        }

        return std::pair<int, int>(-1, -1);
    };

    // Approach 3: Two Pointer (requires sorted array) O(n)
    auto twoPointer = [](const std::vector<int> &nums, int target)
    {
        return TwoPointerTechniques::twoSum(nums, target);
    };

    // Benchmark
    long long bruteTime = measureExecutionTime(bruteForce, nums, target);
    long long hashTime = measureExecutionTime(hashMap, nums, target);
    long long twoPointerTime = measureExecutionTime(twoPointer, sortedNums, target);

    std::cout << "Brute Force: " << bruteTime << " microseconds" << std::endl;
    std::cout << "Hash Map: " << hashTime << " microseconds" << std::endl;
    std::cout << "Two Pointer (sorted): " << twoPointerTime << " microseconds" << std::endl;
    std::cout << "Hash Map vs. Brute Force Speedup: " << (double)bruteTime / hashTime << "x" << std::endl;
    std::cout << std::endl;
}

/**
 * Case Study 3: Binary Search vs. Linear Search
 * Compare performance for searching in sorted and unsorted arrays
 */
void searchComparisonDemo()
{
    std::cout << "===== Search Algorithm Comparison =====" << std::endl;

    // Generate test data
    std::vector<int> unsortedNums = generateRandomVector(100000, 1, 1000000);
    std::vector<int> sortedNums = unsortedNums;
    std::sort(sortedNums.begin(), sortedNums.end());

    int target = unsortedNums[50000]; // A target that exists in the array

    // Benchmark on unsorted array
    long long linearUnsortedTime = measureExecutionTime(SearchingAlgorithms::linearSearch, unsortedNums, target);

    // Benchmark on sorted array
    long long linearSortedTime = measureExecutionTime(SearchingAlgorithms::linearSearch, sortedNums, target);
    long long binaryIterativeTime = measureExecutionTime(SearchingAlgorithms::binarySearchIterative, sortedNums, target);
    long long binaryRecursiveTime = measureExecutionTime(
        [&]()
        {
            return SearchingAlgorithms::binarySearchRecursive(sortedNums, target, 0, sortedNums.size() - 1);
        });

    std::cout << "Linear Search (Unsorted): " << linearUnsortedTime << " microseconds" << std::endl;
    std::cout << "Linear Search (Sorted): " << linearSortedTime << " microseconds" << std::endl;
    std::cout << "Binary Search (Iterative): " << binaryIterativeTime << " microseconds" << std::endl;
    std::cout << "Binary Search (Recursive): " << binaryRecursiveTime << " microseconds" << std::endl;

    std::cout << "Binary Iterative vs. Linear Speedup: "
              << (double)linearSortedTime / binaryIterativeTime << "x" << std::endl;
    std::cout << std::endl;
}

/**
 * Case Study 4: Sorting Algorithm Comparison
 * Compare various sorting algorithms on different input types
 */
void sortingComparisonDemo()
{
    std::cout << "===== Sorting Algorithm Comparison =====" << std::endl;

    const int SIZE = 10000;

    // Generate different types of arrays
    std::vector<int> randomArray = generateRandomVector(SIZE, 1, 10000);

    std::vector<int> nearlySortedArray = generateRandomVector(SIZE, 1, 10000);
    std::sort(nearlySortedArray.begin(), nearlySortedArray.end());
    // Swap a few elements to make it nearly sorted
    for (int i = 0; i < SIZE / 100; i++)
    {
        int idx1 = rand() % SIZE;
        int idx2 = rand() % SIZE;
        std::swap(nearlySortedArray[idx1], nearlySortedArray[idx2]);
    }

    std::vector<int> reversedArray = generateRandomVector(SIZE, 1, 10000);
    std::sort(reversedArray.begin(), reversedArray.end(), std::greater<int>());

    // Clone arrays for each algorithm
    auto testSortingAlgorithm = [](
                                    const std::string &name,
                                    std::function<void(std::vector<int> &)> sortFunc,
                                    const std::vector<int> &randomArr,
                                    const std::vector<int> &nearlySortedArr,
                                    const std::vector<int> &reversedArr)
    {
        std::vector<int> randomCopy = randomArr;
        std::vector<int> nearlySortedCopy = nearlySortedArr;
        std::vector<int> reversedCopy = reversedArr;

        long long randomTime = measureExecutionTime(sortFunc, randomCopy);
        long long nearlySortedTime = measureExecutionTime(sortFunc, nearlySortedCopy);
        long long reversedTime = measureExecutionTime(sortFunc, reversedCopy);

        std::cout << name << ":" << std::endl;
        std::cout << "  Random: " << randomTime << " microseconds" << std::endl;
        std::cout << "  Nearly Sorted: " << nearlySortedTime << " microseconds" << std::endl;
        std::cout << "  Reversed: " << reversedTime << " microseconds" << std::endl;
        std::cout << std::endl;
    };

    // Test each sorting algorithm
    testSortingAlgorithm("Bubble Sort", SortingAlgorithms::bubbleSort,
                         randomArray, nearlySortedArray, reversedArray);

    testSortingAlgorithm("Selection Sort", SortingAlgorithms::selectionSort,
                         randomArray, nearlySortedArray, reversedArray);

    testSortingAlgorithm("Insertion Sort", SortingAlgorithms::insertionSort,
                         randomArray, nearlySortedArray, reversedArray);

    testSortingAlgorithm("Merge Sort", SortingAlgorithms::mergeSort,
                         randomArray, nearlySortedArray, reversedArray);

    testSortingAlgorithm("Quick Sort", SortingAlgorithms::quickSort,
                         randomArray, nearlySortedArray, reversedArray);

    testSortingAlgorithm("Quick Sort (Random Pivot)", SortingAlgorithms::quickSortRandomPivot,
                         randomArray, nearlySortedArray, reversedArray);

    // Also test std::sort for comparison
    testSortingAlgorithm("std::sort", [](std::vector<int> &arr)
                         { std::sort(arr.begin(), arr.end()); }, randomArray, nearlySortedArray, reversedArray);
}

/**
 * Case Study 5: Longest Increasing Subsequence Comparison
 * Compare dynamic programming vs. binary search approaches
 */
void lisComparisonDemo()
{
    std::cout << "===== Longest Increasing Subsequence Comparison =====" << std::endl;

    // Generate test data of different sizes
    std::vector<int> sizes = {100, 1000, 10000};

    for (int size : sizes)
    {
        std::vector<int> nums = generateRandomVector(size, 1, 10000);

        // Clone for each approach
        std::vector<int> nums1 = nums;
        std::vector<int> nums2 = nums;

        // Benchmark
        long long dpTime = measureExecutionTime(lengthOfLIS_DP, nums1);
        long long bsTime = measureExecutionTime(lengthOfLIS_BS, nums2);

        std::cout << "Array Size: " << size << std::endl;
        std::cout << "  DP Approach: " << dpTime << " microseconds" << std::endl;
        std::cout << "  Binary Search Approach: " << bsTime << " microseconds" << std::endl;
        std::cout << "  Speedup: " << (double)dpTime / bsTime << "x" << std::endl;
        std::cout << std::endl;
    }
}

// ============================================================================
// Main Function
// ============================================================================

int main()
{
    std::cout << "=======================================================" << std::endl;
    std::cout << "       WEEK 3 CHALLENGE & RECAP - MASTERING BASIC      " << std::endl;
    std::cout << "                    ALGORITHMS                         " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << std::endl;

    // Demonstration of Integrated Challenge 1: K Closest Points to Origin
    std::cout << "===== Challenge 1: K Closest Points to Origin =====" << std::endl;
    std::vector<std::vector<int>> points = {{1, 3}, {-2, 2}, {5, 8}, {0, 1}, {-10, -5}};
    int k = 3;

    std::cout << "Input points:" << std::endl;
    print2DVector(points);

    std::vector<std::vector<int>> closestPoints = kClosest(points, k);

    std::cout << k << " closest points to origin:" << std::endl;
    print2DVector(closestPoints);
    std::cout << std::endl;

    // Demonstration of Integrated Challenge 2: Merge Intervals
    std::cout << "===== Challenge 2: Merge Intervals =====" << std::endl;
    std::vector<std::vector<int>> intervals = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};

    std::cout << "Input intervals:" << std::endl;
    print2DVector(intervals);

    std::vector<std::vector<int>> mergedIntervals = mergeIntervals(intervals);

    std::cout << "Merged intervals:" << std::endl;
    print2DVector(mergedIntervals);
    std::cout << std::endl;

    // Demonstration of Integrated Challenge 3: Longest Increasing Subsequence
    std::cout << "===== Challenge 3: Longest Increasing Subsequence =====" << std::endl;
    std::vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};

    std::cout << "Input array: ";
    printVector(nums);

    int lisLength1 = lengthOfLIS_DP(nums);
    int lisLength2 = lengthOfLIS_BS(nums);

    std::cout << "Length of LIS (DP approach): " << lisLength1 << std::endl;
    std::cout << "Length of LIS (Binary Search approach): " << lisLength2 << std::endl;
    std::cout << std::endl;

    // Demonstrate Recursive to Iterative Transformation
    std::cout << "===== Recursive to Iterative Transformation =====" << std::endl;
    std::vector<int> searchArray = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    int searchTarget = 13;

    std::cout << "Array: ";
    printVector(searchArray);
    std::cout << "Target: " << searchTarget << std::endl;

    int recursiveResult = SearchingAlgorithms::binarySearchRecursive(
        searchArray, searchTarget, 0, searchArray.size() - 1);
    int iterativeResult = SearchingAlgorithms::binarySearchIterative(searchArray, searchTarget);

    std::cout << "Binary Search (Recursive): Found at index " << recursiveResult << std::endl;
    std::cout << "Binary Search (Iterative): Found at index " << iterativeResult << std::endl;
    std::cout << std::endl;

    // Run Performance Optimization Case Studies
    countFrequencyDemo();
    twoSumDemo();
    searchComparisonDemo();
    sortingComparisonDemo();
    lisComparisonDemo();

    // Finding a peak element using recursion + binary search
    std::cout << "===== Finding a Peak Element =====" << std::endl;
    std::vector<int> peakArray = {1, 2, 3, 1};

    std::cout << "Array: ";
    printVector(peakArray);

    int peakIndex = findPeakElementWrapper(peakArray);

    std::cout << "Peak element found at index " << peakIndex
              << " with value " << peakArray[peakIndex] << std::endl;
    std::cout << std::endl;

    // Finding the kth smallest element using Quick Select
    std::cout << "===== Quick Select: Kth Smallest Element =====" << std::endl;
    std::vector<int> quickSelectArray = {3, 2, 1, 5, 6, 4};
    int kthSmallest = 3;

    std::cout << "Array: ";
    printVector(quickSelectArray);

    int kthElement = quickSelect(quickSelectArray, kthSmallest);

    std::cout << "The " << kthSmallest << "rd smallest element is: " << kthElement << std::endl;
    std::cout << std::endl;

    return 0;
}