// Day 16: Searching Algorithms #DSAin45
// Comprehensive implementations of various searching algorithms

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// ===== UTILITY FUNCTIONS =====

// Helper function to measure execution time
template <typename Func, typename... Args>
auto measureExecutionTime(Func func, Args &&...args)
{
auto start = std::chrono::high_resolution_clock::now();
auto result = func(std::forward<Args>(args)...);
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    return std::make_pair(result, duration.count());

}

// Overload for void functions
template <typename Func, typename... Args>
auto measureExecutionTimeVoid(Func func, Args &&...args)
{
auto start = std::chrono::high_resolution_clock::now();
func(std::forward<Args>(args)...);
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    return duration.count();

}

// Generate a random vector of the specified size
std::vector<int> generateRandomVector(int size, int min = 0, int max = 1000)
{
std::vector<int> vec(size);
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(min, max);

    for (int i = 0; i < size; i++)
    {
        vec[i] = dis(gen);
    }

    return vec;

}

// Generate a sorted vector of the specified size
std::vector<int> generateSortedVector(int size, int min = 0, int max = 1000)
{
auto vec = generateRandomVector(size, min, max);
std::sort(vec.begin(), vec.end());
return vec;
}

// Generate a vector with duplicates
std::vector<int> generateVectorWithDuplicates(int size, int numUniqueValues)
{
std::vector<int> vec(size);
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, numUniqueValues - 1);

    for (int i = 0; i < size; i++)
    {
        vec[i] = dis(gen);
    }

    return vec;

}

// Print the vector
void printVector(const std::vector<int> &vec, std::string name = "Vector", int maxDisplay = 20)
{
std::cout << name << " (size=" << vec.size() << "): [";
int showElements = std::min(maxDisplay, static_cast<int>(vec.size()));

    for (int i = 0; i < showElements; i++)
    {
        std::cout << vec[i];
        if (i < showElements - 1)
            std::cout << ", ";
    }

    if (vec.size() > maxDisplay)
    {
        std::cout << ", ... (" << vec.size() - maxDisplay << " more)";
    }

    std::cout << "]" << std::endl;

}

// ===== LINEAR SEARCH IMPLEMENTATIONS =====

// Simple linear search
int linearSearch(const std::vector<int> &arr, int target)
{
for (int i = 0; i < arr.size(); i++)
{
if (arr[i] == target)
{
return i; // Found the target at index i
}
}
return -1; // Target not found
}

// Optimized linear search with early termination for sorted arrays
int orderedLinearSearch(const std::vector<int> &arr, int target)
{
for (int i = 0; i < arr.size(); i++)
{
if (arr[i] == target)
{
return i; // Found the target
}
if (arr[i] > target)
{
return -1; // Target cannot be in the array
}
}
return -1; // Target not found
}

// Sentinel linear search (adds the target at the end to eliminate boundary check)
int sentinelLinearSearch(std::vector<int> arr, int target)
{
int n = arr.size();
if (n == 0) return -1;

    // Save the last element and place target at the end
    int lastElement = arr[n - 1];
    arr[n - 1] = target;

    int i = 0;
    // No need to check array bounds
    while (arr[i] != target)
    {
        i++;
    }

    // Restore the last element
    arr[n - 1] = lastElement;

    // Check if we found the target or reached the sentinel
    if (i < n - 1 || arr[n - 1] == target)
    {
        return i;
    }
    return -1; // Target not found

}

// Two-way linear search (start from both ends)
int twoWayLinearSearch(const std::vector<int> &arr, int target)
{
int left = 0;
int right = arr.size() - 1;

    while (left <= right)
    {
        // Check the left element
        if (arr[left] == target)
        {
            return left;
        }

        // Check the right element
        if (arr[right] == target)
        {
            return right;
        }

        left++;
        right--;
    }

    return -1; // Target not found

}

// ===== LINKED LIST SEARCH IMPLEMENTATIONS =====

// Definition for singly-linked list
struct ListNode
{
int val;
ListNode \*next;
ListNode(int x) : val(x), next(nullptr) {}
};

// Generate a linked list from a vector
ListNode \*createLinkedList(const std::vector<int> &nums)
{
if (nums.empty())
return nullptr;

    ListNode *head = new ListNode(nums[0]);
    ListNode *current = head;

    for (int i = 1; i < nums.size(); i++)
    {
        current->next = new ListNode(nums[i]);
        current = current->next;
    }

    return head;

}

// Print a linked list
void printLinkedList(ListNode *head, std::string name = "Linked List", int maxDisplay = 20)
{
std::cout << name << ": ";
int count = 0;
ListNode *current = head;

    while (current && count < maxDisplay)
    {
        std::cout << current->val;
        current = current->next;
        if (current && count < maxDisplay - 1)
            std::cout << " -> ";
        count++;
    }

    if (current)
    {
        std::cout << " -> ... (more elements)";
    }

    std::cout << std::endl;

}

// Clean up a linked list
void deleteLinkedList(ListNode *head)
{
while (head)
{
ListNode *temp = head;
head = head->next;
delete temp;
}
}

// Linear search in a linked list
ListNode *linearSearchLinkedList(ListNode *head, int target)
{
ListNode \*current = head;
while (current)
{
if (current->val == target)
{
return current; // Found the target
}
current = current->next;
}
return nullptr; // Target not found
}

// Find the index of a value in a linked list
int findIndexInLinkedList(ListNode *head, int target)
{
ListNode *current = head;
int index = 0;

    while (current)
    {
        if (current->val == target)
        {
            return index; // Found the target at this index
        }
        current = current->next;
        index++;
    }

    return -1; // Target not found

}

// ===== BINARY SEARCH IMPLEMENTATIONS =====

// Iterative binary search
int binarySearch(const std::vector<int> &arr, int target)
{
int left = 0;
int right = arr.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2; // Avoid potential overflow

        // Check if target is present at mid
        if (arr[mid] == target)
        {
            return mid;
        }

        // If target is greater, ignore left half
        if (arr[mid] < target)
        {
            left = mid + 1;
        }
        // If target is smaller, ignore right half
        else
        {
            right = mid - 1;
        }
    }

    // Element not present
    return -1;

}

// Recursive binary search
int binarySearchRecursive(const std::vector<int> &arr, int target, int left, int right)
{
// Base case: element not found
if (left > right)
{
return -1;
}

    int mid = left + (right - left) / 2;

    // If the element is present at the middle
    if (arr[mid] == target)
    {
        return mid;
    }

    // If element is smaller than mid, search in the left subarray
    if (arr[mid] > target)
    {
        return binarySearchRecursive(arr, target, left, mid - 1);
    }

    // Else search in the right subarray
    return binarySearchRecursive(arr, target, mid + 1, right);

}

// Wrapper for recursive binary search
int binarySearchRecursive(const std::vector<int> &arr, int target)
{
return binarySearchRecursive(arr, target, 0, arr.size() - 1);
}

// Binary search to find the insertion position
int binarySearchInsertPosition(const std::vector<int> &arr, int target)
{
int left = 0;
int right = arr.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
        {
            return mid; // Exact match found
        }

        if (arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    // At this point, left > right
    // 'left' is the position where the element should be inserted
    return left;

}

// Binary search to find first occurrence of target
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
            result = mid; // Save this result
            right = mid - 1; // Continue searching on the left side
        }
        else if (arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return result;

}

// Binary search to find last occurrence of target
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
            result = mid; // Save this result
            left = mid + 1; // Continue searching on the right side
        }
        else if (arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return result;

}

// Find both first and last occurrence (range) of target
std::pair<int, int> binarySearchRange(const std::vector<int> &arr, int target)
{
int first = binarySearchFirstOccurrence(arr, target);
int last = binarySearchLastOccurrence(arr, target);
return {first, last};
}

// ===== JUMP SEARCH IMPLEMENTATIONS =====

// Jump search (jumping ahead by sqrt(n) steps)
int jumpSearch(const std::vector<int> &arr, int target)
{
int n = arr.size();
if (n == 0) return -1;

    // Finding the optimal jump size: sqrt(n)
    int step = static_cast<int>(sqrt(n));

    // Finding the block where the element might be present
    int prev = 0;
    while (prev < n && arr[std::min(step, n) - 1] < target)
    {
        prev = step;
        step += static_cast<int>(sqrt(n));
        if (prev >= n)
        {
            return -1; // Element not present
        }
    }

    // Linear search within the identified block
    while (prev < n && arr[prev] < target)
    {
        prev++;
        if (prev == std::min(step, n))
        {
            return -1; // Element not present
        }
    }

    // If element is found
    if (prev < n && arr[prev] == target)
    {
        return prev;
    }

    return -1; // Element not present

}

// Jump search with customizable step size
int jumpSearchCustomStep(const std::vector<int> &arr, int target, int step)
{
int n = arr.size();
if (n == 0) return -1;

    // Finding the block where the element might be present
    int prev = 0;
    while (prev < n && arr[std::min(step, n) - 1] < target)
    {
        prev = step;
        step += step; // Double the step each time
        if (prev >= n)
        {
            return -1; // Element not present
        }
    }

    // Linear search within the identified block
    while (prev < n && arr[prev] < target)
    {
        prev++;
        if (prev == std::min(step, n))
        {
            return -1; // Element not present
        }
    }

    // If element is found
    if (prev < n && arr[prev] == target)
    {
        return prev;
    }

    return -1; // Element not present

}

// ===== INTERPOLATION SEARCH IMPLEMENTATIONS =====

// Interpolation search
int interpolationSearch(const std::vector<int> &arr, int target)
{
int left = 0;
int right = arr.size() - 1;

    while (left <= right && target >= arr[left] && target <= arr[right])
    {
        // Avoid division by zero
        if (left == right)
        {
            if (arr[left] == target) return left;
            return -1;
        }

        // Estimate the position using interpolation formula
        int pos = left + ((target - arr[left]) * (right - left)) /
                           (arr[right] - arr[left]);

        // Check if the estimate is out of bounds
        if (pos < left) pos = left;
        if (pos > right) pos = right;

        if (arr[pos] == target)
        {
            return pos;
        }

        if (arr[pos] < target)
        {
            left = pos + 1;
        }
        else
        {
            right = pos - 1;
        }
    }

    return -1; // Element not present

}

// Faster interpolation search with bounds checking
int interpolationSearchFast(const std::vector<int> &arr, int target)
{
int low = 0;
int high = arr.size() - 1;

    // Check edge cases first
    if (arr.empty() || target < arr[low] || target > arr[high])
        return -1;

    while (low <= high && target >= arr[low] && target <= arr[high])
    {
        if (low == high)
        {
            if (arr[low] == target) return low;
            return -1;
        }

        // Avoid division by zero
        if (arr[high] == arr[low])
        {
            if (arr[low] == target) return low;
            return -1;
        }

        // Calculate the position
        int pos = low + ((target - arr[low]) * (high - low)) / (arr[high] - arr[low]);

        // Ensure pos is within bounds
        if (pos < low) pos = low;
        if (pos > high) pos = high;

        if (arr[pos] == target)
            return pos;

        if (arr[pos] < target)
            low = pos + 1;
        else
            high = pos - 1;
    }

    return -1;

}

// ===== EXPONENTIAL SEARCH IMPLEMENTATIONS =====

// Exponential search
int exponentialSearch(const std::vector<int> &arr, int target)
{
int n = arr.size();
if (n == 0) return -1;

    // If target is the first element
    if (arr[0] == target)
    {
        return 0;
    }

    // Find the range for binary search
    int i = 1;
    while (i < n && arr[i] <= target)
    {
        i *= 2; // Double i each time
    }

    // Apply binary search in the found range
    int left = i / 2;
    int right = std::min(i, n - 1);

    return binarySearchRecursive(arr, target, left, right);

}

// ===== BINARY SEARCH TREE IMPLEMENTATIONS =====

// Binary Search Tree Node
struct TreeNode
{
int val;
TreeNode *left;
TreeNode *right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

};

// Insert a value into a BST
TreeNode* insertIntoBST(TreeNode* root, int val)
{
// Base case: empty tree or reached a leaf node
if (root == nullptr)
{
return new TreeNode(val);
}

    // Recursive insertion
    if (val < root->val)
    {
        root->left = insertIntoBST(root->left, val);
    }
    else if (val > root->val)
    {
        root->right = insertIntoBST(root->right, val);
    }
    // In case of duplicates, you can decide what to do
    // Here we ignore duplicates

    return root;

}

// Create a BST from a vector
TreeNode\* createBST(const std::vector<int>& nums)
{
if (nums.empty())
return nullptr;

    TreeNode* root = nullptr;
    for (int num : nums)
    {
        root = insertIntoBST(root, num);
    }

    return root;

}

// Search in a BST - recursive
TreeNode* searchBST(TreeNode* root, int target)
{
// Base cases: root is null or the target is found
if (root == nullptr || root->val == target)
{
return root;
}

    // If target is less than root's value, search in left subtree
    if (target < root->val)
    {
        return searchBST(root->left, target);
    }

    // If target is greater than root's value, search in right subtree
    return searchBST(root->right, target);

}

// Search in a BST - iterative
TreeNode* searchBSTIterative(TreeNode* root, int target)
{
while (root != nullptr && root->val != target)
{
if (target < root->val)
{
root = root->left;
}
else
{
root = root->right;
}
}
return root;
}

// In-order traversal of BST (results in sorted order)
void inorderTraversal(TreeNode\* root, std::vector<int>& result)
{
if (root == nullptr)
return;

    inorderTraversal(root->left, result);
    result.push_back(root->val);
    inorderTraversal(root->right, result);

}

// Delete a BST
void deleteBST(TreeNode\* root)
{
if (root == nullptr)
return;

    deleteBST(root->left);
    deleteBST(root->right);
    delete root;

}

// Check if a BST is valid
bool isValidBST(TreeNode* root, TreeNode* min = nullptr, TreeNode\* max = nullptr)
{
// Empty tree is a valid BST
if (root == nullptr)
return true;

    // Check if current node's value is within valid range
    if ((min != nullptr && root->val <= min->val) ||
        (max != nullptr && root->val >= max->val))
        return false;

    // Check left and right subtrees
    return isValidBST(root->left, min, root) &&
           isValidBST(root->right, root, max);

}

// ===== ADVANCED SEARCHING ALGORITHMS =====

// Binary search in a rotated sorted array
int searchInRotatedSortedArray(const std::vector<int>& nums, int target)
{
int left = 0;
int right = nums.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target)
        {
            return mid;
        }

        // Check if the left half is sorted
        if (nums[left] <= nums[mid])
        {
            // Check if target is in the left sorted portion
            if (nums[left] <= target && target < nums[mid])
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }
        // Right half is sorted
        else
        {
            // Check if target is in the right sorted portion
            if (nums[mid] < target && target <= nums[right])
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }

    return -1;

}

// Find peak element in an array
int findPeakElement(const std::vector<int>& nums)
{
int left = 0;
int right = nums.size() - 1;

    while (left < right)
    {
        int mid = left + (right - left) / 2;

        // If mid is greater than the next element, a peak exists in the left half
        if (nums[mid] > nums[mid + 1])
        {
            right = mid;
        }
        // Otherwise, peak exists in the right half
        else
        {
            left = mid + 1;
        }
    }

    // At this point, left == right, which is the peak
    return left;

}

// Binary search on answer (finding square root as example)
int mySqrt(int x)
{
if (x <= 1) return x;

    int left = 1;
    int right = x / 2;
    int result = 0;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        long long square = static_cast<long long>(mid) * mid;

        if (square == x)
        {
            return mid;
        }

        if (square < x)
        {
            result = mid; // Potential answer
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return result;

}

// ===== PERFORMANCE TESTING FUNCTIONS =====

// Test and compare linear search algorithms
void testLinearSearchAlgorithms()
{
std::cout << "\n===== LINEAR SEARCH ALGORITHMS =====" << std::endl;

    // Generate test data
    std::vector<int> unsortedArray = generateRandomVector(10000);
    std::vector<int> sortedArray = generateSortedVector(10000);

    // Select targets from different positions
    int beginTarget = unsortedArray[10];                           // Near beginning
    int middleTarget = unsortedArray[unsortedArray.size() / 2];    // Middle
    int endTarget = unsortedArray[unsortedArray.size() - 10];      // Near end
    int notPresentTarget = 100000;                                // Not in array

    std::cout << "\nPerformance testing with array size = " << unsortedArray.size() << std::endl;

    // Test on unsorted array
    std::cout << "\nUnsorted Array Tests:" << std::endl;
    std::cout << std::left << std::setw(25) << "Algorithm"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Standard linear search
    {
        auto [beginResult, beginTime] = measureExecutionTime(linearSearch, unsortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(linearSearch, unsortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(linearSearch, unsortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(linearSearch, unsortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Linear Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Sentinel linear search
    {
        auto [beginResult, beginTime] = measureExecutionTime(sentinelLinearSearch, unsortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(sentinelLinearSearch, unsortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(sentinelLinearSearch, unsortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(sentinelLinearSearch, unsortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Sentinel Linear Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Two-way linear search
    {
        auto [beginResult, beginTime] = measureExecutionTime(twoWayLinearSearch, unsortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(twoWayLinearSearch, unsortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(twoWayLinearSearch, unsortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(twoWayLinearSearch, unsortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Two-Way Linear Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Test on sorted array
    std::cout << "\nSorted Array Tests:" << std::endl;
    beginTarget = sortedArray[10];
    middleTarget = sortedArray[sortedArray.size() / 2];
    endTarget = sortedArray[sortedArray.size() - 10];

    std::cout << std::left << std::setw(25) << "Algorithm"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Ordered linear search
    {
        auto [beginResult, beginTime] = measureExecutionTime(orderedLinearSearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(orderedLinearSearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(orderedLinearSearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(orderedLinearSearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Ordered Linear Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Linked list tests
    std::cout << "\nLinked List Search Test:" << std::endl;
    std::vector<int> listData = generateRandomVector(1000);
    ListNode* head = createLinkedList(listData);

    int listBeginTarget = listData[0];
    int listMiddleTarget = listData[listData.size() / 2];
    int listEndTarget = listData[listData.size() - 1];

    auto [beginNode, beginTime] = measureExecutionTime(linearSearchLinkedList, head, listBeginTarget);
    auto [middleNode, middleTime] = measureExecutionTime(linearSearchLinkedList, head, listMiddleTarget);
    auto [endNode, endTime] = measureExecutionTime(linearSearchLinkedList, head, listEndTarget);
    auto [notFoundNode, notFoundTime] = measureExecutionTime(linearSearchLinkedList, head, notPresentTarget);

    std::cout << std::left << std::setw(25) << "Linked List Search"
              << std::setw(15) << beginTime
              << std::setw(15) << middleTime
              << std::setw(15) << endTime
              << std::setw(15) << notFoundTime << std::endl;

    // Clean up linked list
    deleteLinkedList(head);

}

// Test and compare binary search algorithms
void testBinarySearchAlgorithms()
{
std::cout << "\n===== BINARY SEARCH ALGORITHMS =====" << std::endl;

    // Generate a sorted array
    std::vector<int> sortedArray = generateSortedVector(100000);

    // Select targets from different positions
    int beginTarget = sortedArray[10];                        // Near beginning
    int middleTarget = sortedArray[sortedArray.size() / 2];   // Middle
    int endTarget = sortedArray[sortedArray.size() - 10];     // Near end
    int notPresentTarget = sortedArray.back() + 1000;         // Not in array

    std::cout << "\nPerformance testing with sorted array size = " << sortedArray.size() << std::endl;
    std::cout << std::left << std::setw(25) << "Algorithm"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Linear search (for comparison)
    {
        auto [beginResult, beginTime] = measureExecutionTime(linearSearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(linearSearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(linearSearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(linearSearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Linear Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Ordered linear search
    {
        auto [beginResult, beginTime] = measureExecutionTime(orderedLinearSearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(orderedLinearSearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(orderedLinearSearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(orderedLinearSearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Ordered Linear Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Iterative binary search
    {
        auto [beginResult, beginTime] = measureExecutionTime(binarySearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(binarySearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(binarySearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(binarySearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Binary Search (Iter)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Recursive binary search
    {
        auto [beginResult, beginTime] = measureExecutionTime(binarySearchRecursive, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(binarySearchRecursive, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(binarySearchRecursive, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(binarySearchRecursive, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Binary Search (Rec)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Jump search with sqrt(n) step
    {
        auto [beginResult, beginTime] = measureExecutionTime(jumpSearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(jumpSearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(jumpSearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(jumpSearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Jump Search (sqrt)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Interpolation search
    {
        auto [beginResult, beginTime] = measureExecutionTime(interpolationSearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(interpolationSearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(interpolationSearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(interpolationSearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Interpolation Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Exponential search
    {
        auto [beginResult, beginTime] = measureExecutionTime(exponentialSearch, sortedArray, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(exponentialSearch, sortedArray, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(exponentialSearch, sortedArray, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(exponentialSearch, sortedArray, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Exponential Search"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

}

// Test binary search variations
void testBinarySearchVariations()
{
std::cout << "\n===== BINARY SEARCH VARIATIONS =====" << std::endl;

    // Generate a sorted array with duplicates
    std::vector<int> arr = generateVectorWithDuplicates(1000, 100);
    std::sort(arr.begin(), arr.end());

    // Print a sample of the array
    printVector(arr, "Sorted Array with Duplicates", 20);

    // Pick a value that appears multiple times
    int target = arr[arr.size() / 2];
    int count = std::count(arr.begin(), arr.end(), target);

    std::cout << "Target: " << target << " (appears " << count << " times)" << std::endl;

    // Find first occurrence
    auto [firstPos, firstTime] = measureExecutionTime(binarySearchFirstOccurrence, arr, target);
    std::cout << "First occurrence at index: " << firstPos << " (found in " << firstTime << " ns)" << std::endl;

    // Find last occurrence
    auto [lastPos, lastTime] = measureExecutionTime(binarySearchLastOccurrence, arr, target);
    std::cout << "Last occurrence at index: " << lastPos << " (found in " << lastTime << " ns)" << std::endl;

    // Find range (both first and last)
    auto [range, rangeTime] = measureExecutionTime(binarySearchRange, arr, target);
    std::cout << "Range: [" << range.first << ", " << range.second << "] (found in " << rangeTime << " ns)" << std::endl;

    // Verify results
    std::cout << "\nVerification:" << std::endl;
    std::cout << "Count of occurrences: " << (range.second - range.first + 1) << std::endl;
    assert(count == (range.second - range.first + 1));
    std::cout << "Verification successful!" << std::endl;

}

// Test interpolation search for different distributions
void testInterpolationSearch()
{
std::cout << "\n===== INTERPOLATION SEARCH TESTS =====" << std::endl;

    // 1. Uniformly distributed data (ideal case for interpolation search)
    std::vector<int> uniformData(10000);
    for (int i = 0; i < uniformData.size(); i++)
    {
        uniformData[i] = i * 10; // Values 0, 10, 20, ..., 99990
    }

    // 2. Non-uniformly distributed data (worst case)
    std::vector<int> exponentialData(10000);
    for (int i = 0; i < exponentialData.size(); i++)
    {
        exponentialData[i] = static_cast<int>(pow(1.1, i)); // Exponential growth
    }

    // 3. Regular sorted random data
    std::vector<int> randomData = generateSortedVector(10000);

    // Test targets (beginning, middle, end, not present)
    int uniformTarget1 = uniformData[10];
    int uniformTarget2 = uniformData[uniformData.size() / 2];
    int uniformTarget3 = uniformData[uniformData.size() - 10];
    int uniformNotPresent = -1;

    int exponentialTarget1 = exponentialData[10];
    int exponentialTarget2 = exponentialData[exponentialData.size() / 2];
    int exponentialTarget3 = exponentialData[exponentialData.size() - 10];
    int exponentialNotPresent = -1;

    int randomTarget1 = randomData[10];
    int randomTarget2 = randomData[randomData.size() / 2];
    int randomTarget3 = randomData[randomData.size() - 10];
    int randomNotPresent = randomData.back() + 1000;

    std::cout << "\n1. Uniform Distribution (values increase by 10):" << std::endl;
    std::cout << std::left << std::setw(25) << "Algorithm"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Binary search on uniform data
    {
        auto [result1, time1] = measureExecutionTime(binarySearch, uniformData, uniformTarget1);
        auto [result2, time2] = measureExecutionTime(binarySearch, uniformData, uniformTarget2);
        auto [result3, time3] = measureExecutionTime(binarySearch, uniformData, uniformTarget3);
        auto [result4, time4] = measureExecutionTime(binarySearch, uniformData, uniformNotPresent);

        std::cout << std::left << std::setw(25) << "Binary Search"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;
    }

    // Interpolation search on uniform data
    {
        auto [result1, time1] = measureExecutionTime(interpolationSearch, uniformData, uniformTarget1);
        auto [result2, time2] = measureExecutionTime(interpolationSearch, uniformData, uniformTarget2);
        auto [result3, time3] = measureExecutionTime(interpolationSearch, uniformData, uniformTarget3);
        auto [result4, time4] = measureExecutionTime(interpolationSearch, uniformData, uniformNotPresent);

        std::cout << std::left << std::setw(25) << "Interpolation Search"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;
    }

    std::cout << "\n2. Exponential Distribution (values grow exponentially):" << std::endl;
    std::cout << std::left << std::setw(25) << "Algorithm"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Binary search on exponential data
    {
        auto [result1, time1] = measureExecutionTime(binarySearch, exponentialData, exponentialTarget1);
        auto [result2, time2] = measureExecutionTime(binarySearch, exponentialData, exponentialTarget2);
        auto [result3, time3] = measureExecutionTime(binarySearch, exponentialData, exponentialTarget3);
        auto [result4, time4] = measureExecutionTime(binarySearch, exponentialData, exponentialNotPresent);

        std::cout << std::left << std::setw(25) << "Binary Search"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;
    }

    // Interpolation search on exponential data
    {
        auto [result1, time1] = measureExecutionTime(interpolationSearch, exponentialData, exponentialTarget1);
        auto [result2, time2] = measureExecutionTime(interpolationSearch, exponentialData, exponentialTarget2);
        auto [result3, time3] = measureExecutionTime(interpolationSearch, exponentialData, exponentialTarget3);
        auto [result4, time4] = measureExecutionTime(interpolationSearch, exponentialData, exponentialNotPresent);

        std::cout << std::left << std::setw(25) << "Interpolation Search"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;
    }

    std::cout << "\n3. Random Sorted Data:" << std::endl;
    std::cout << std::left << std::setw(25) << "Algorithm"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Binary search on random data
    {
        auto [result1, time1] = measureExecutionTime(binarySearch, randomData, randomTarget1);
        auto [result2, time2] = measureExecutionTime(binarySearch, randomData, randomTarget2);
        auto [result3, time3] = measureExecutionTime(binarySearch, randomData, randomTarget3);
        auto [result4, time4] = measureExecutionTime(binarySearch, randomData, randomNotPresent);

        std::cout << std::left << std::setw(25) << "Binary Search"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;
    }

    // Interpolation search on random data
    {
        auto [result1, time1] = measureExecutionTime(interpolationSearch, randomData, randomTarget1);
        auto [result2, time2] = measureExecutionTime(interpolationSearch, randomData, randomTarget2);
        auto [result3, time3] = measureExecutionTime(interpolationSearch, randomData, randomTarget3);
        auto [result4, time4] = measureExecutionTime(interpolationSearch, randomData, randomNotPresent);

        std::cout << std::left << std::setw(25) << "Interpolation Search"
                  << std::setw(15) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;
    }

}

// Test Binary Search Tree operations
void testBST()
{
std::cout << "\n===== BINARY SEARCH TREE TESTS =====" << std::endl;

    // Create a balanced BST
    std::vector<int> sortedData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    // To create a balanced BST, we'll insert the middle element first
    std::function<TreeNode*(const std::vector<int>&, int, int)> createBalancedBST =
        [&](const std::vector<int>& nums, int start, int end) -> TreeNode* {
            if (start > end) return nullptr;

            int mid = (start + end) / 2;
            TreeNode* root = new TreeNode(nums[mid]);

            root->left = createBalancedBST(nums, start, mid - 1);
            root->right = createBalancedBST(nums, mid + 1, end);

            return root;
        };

    TreeNode* balancedRoot = createBalancedBST(sortedData, 0, sortedData.size() - 1);

    // Create an unbalanced BST (essentially a linked list)
    TreeNode* unbalancedRoot = nullptr;
    for (int num : sortedData) {
        unbalancedRoot = insertIntoBST(unbalancedRoot, num);
    }

    // Verify trees are valid BSTs
    bool balancedIsValid = isValidBST(balancedRoot);
    bool unbalancedIsValid = isValidBST(unbalancedRoot);

    std::cout << "Balanced BST is valid: " << (balancedIsValid ? "Yes" : "No") << std::endl;
    std::cout << "Unbalanced BST is valid: " << (unbalancedIsValid ? "Yes" : "No") << std::endl;

    // Display tree structure via in-order traversal
    std::vector<int> balancedInorder;
    inorderTraversal(balancedRoot, balancedInorder);
    printVector(balancedInorder, "Balanced BST (in-order)");

    std::vector<int> unbalancedInorder;
    inorderTraversal(unbalancedRoot, unbalancedInorder);
    printVector(unbalancedInorder, "Unbalanced BST (in-order)");

    // Performance comparison for searching
    std::cout << "\nSearch Performance Comparison:" << std::endl;
    std::cout << std::left << std::setw(25) << "Tree Structure"
              << std::setw(15) << "Begin (ns)"
              << std::setw(15) << "Middle (ns)"
              << std::setw(15) << "End (ns)"
              << std::setw(15) << "Not Found (ns)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    // Targets to search for
    int beginTarget = sortedData.front();
    int middleTarget = sortedData[sortedData.size() / 2];
    int endTarget = sortedData.back();
    int notPresentTarget = 100;

    // Test on balanced BST with recursive search
    {
        auto [beginResult, beginTime] = measureExecutionTime(searchBST, balancedRoot, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(searchBST, balancedRoot, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(searchBST, balancedRoot, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(searchBST, balancedRoot, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Balanced BST (Rec)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Test on balanced BST with iterative search
    {
        auto [beginResult, beginTime] = measureExecutionTime(searchBSTIterative, balancedRoot, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(searchBSTIterative, balancedRoot, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(searchBSTIterative, balancedRoot, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(searchBSTIterative, balancedRoot, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Balanced BST (Iter)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Test on unbalanced BST with recursive search
    {
        auto [beginResult, beginTime] = measureExecutionTime(searchBST, unbalancedRoot, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(searchBST, unbalancedRoot, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(searchBST, unbalancedRoot, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(searchBST, unbalancedRoot, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Unbalanced BST (Rec)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Test on unbalanced BST with iterative search
    {
        auto [beginResult, beginTime] = measureExecutionTime(searchBSTIterative, unbalancedRoot, beginTarget);
        auto [middleResult, middleTime] = measureExecutionTime(searchBSTIterative, unbalancedRoot, middleTarget);
        auto [endResult, endTime] = measureExecutionTime(searchBSTIterative, unbalancedRoot, endTarget);
        auto [notFoundResult, notFoundTime] = measureExecutionTime(searchBSTIterative, unbalancedRoot, notPresentTarget);

        std::cout << std::left << std::setw(25) << "Unbalanced BST (Iter)"
                  << std::setw(15) << beginTime
                  << std::setw(15) << middleTime
                  << std::setw(15) << endTime
                  << std::setw(15) << notFoundTime << std::endl;
    }

    // Clean up
    deleteBST(balancedRoot);
    deleteBST(unbalancedRoot);

}

// Test advanced search algorithms
void testAdvancedSearchAlgorithms()
{
std::cout << "\n===== ADVANCED SEARCH ALGORITHMS =====" << std::endl;

    // 1. Search in Rotated Sorted Array
    std::cout << "\n1. Search in Rotated Sorted Array:" << std::endl;

    std::vector<int> rotatedArray = {4, 5, 6, 7, 0, 1, 2};
    printVector(rotatedArray, "Rotated Array");

    std::vector<int> targetsToSearch = {4, 0, 2, 3};
    for (int target : targetsToSearch)
    {
        auto [index, time] = measureExecutionTime(searchInRotatedSortedArray, rotatedArray, target);
        if (index != -1)
        {
            std::cout << "Found " << target << " at index " << index << " in " << time << " ns" << std::endl;
        }
        else
        {
            std::cout << target << " not found in the array (search took " << time << " ns)" << std::endl;
        }
    }

    // 2. Find Peak Element
    std::cout << "\n2. Find Peak Element:" << std::endl;

    std::vector<int> peakArray = {1, 3, 5, 7, 6, 4, 2};
    printVector(peakArray, "Peak Array");

    auto [peakIndex, peakTime] = measureExecutionTime(findPeakElement, peakArray);
    std::cout << "Peak element is " << peakArray[peakIndex] << " at index " << peakIndex << " (found in " << peakTime << " ns)" << std::endl;

    // 3. Binary Search on Answer (Square Root Example)
    std::cout << "\n3. Binary Search on Answer (Square Root):" << std::endl;

    std::vector<int> numbersToFindSqrt = {4, 8, 16, 50, 100, 10000};
    for (int num : numbersToFindSqrt)
    {
        auto [sqrt, time] = measureExecutionTime(mySqrt, num);
        std::cout << "Square root of " << num << " is " << sqrt << " (found in " << time << " ns)" << std::endl;
        // Verify
        std::cout << "  Verification: " << sqrt << "^2 = " << (long long)sqrt * sqrt << " and "
                  << (sqrt + 1) << "^2 = " << (long long)(sqrt + 1) * (sqrt + 1) << std::endl;
    }

}

// ===== MAIN FUNCTION =====

int main()
{
std::cout << "====================================================" << std::endl;
std::cout << "DAY 16: SEARCHING ALGORITHMS #DSAin45" << std::endl;
std::cout << "====================================================" << std::endl;

    // Demo of linear search
    std::vector<int> demoArray = {34, 12, 78, 56, 45, 23, 65, 87, 32, 11};
    std::cout << "\nDEMO: Linear Search" << std::endl;
    printVector(demoArray, "Demo Array");

    int target = 45;
    int linearResult = linearSearch(demoArray, target);
    std::cout << "Searching for " << target << " using linear search..." << std::endl;
    if (linearResult != -1)
    {
        std::cout << "Element found at index: " << linearResult << std::endl;
    }
    else
    {
        std::cout << "Element not found!" << std::endl;
    }

    // Demo of binary search
    std::vector<int> sortedDemoArray = {11, 12, 23, 32, 34, 45, 56, 65, 78, 87};
    std::cout << "\nDEMO: Binary Search" << std::endl;
    printVector(sortedDemoArray, "Sorted Demo Array");

    int binaryResult = binarySearch(sortedDemoArray, target);
    std::cout << "Searching for " << target << " using binary search..." << std::endl;
    if (binaryResult != -1)
    {
        std::cout << "Element found at index: " << binaryResult << std::endl;
    }
    else
    {
        std::cout << "Element not found!" << std::endl;
    }

    // Benchmark linear search algorithms
    testLinearSearchAlgorithms();

    // Benchmark binary search algorithms
    testBinarySearchAlgorithms();

    // Test binary search variations
    testBinarySearchVariations();

    // Test interpolation search with different distributions
    testInterpolationSearch();

    // Test Binary Search Tree operations
    testBST();

    // Test advanced search algorithms
    testAdvancedSearchAlgorithms();

    std::cout << "\n====================================================" << std::endl;
    std::cout << "End of Day 16: Searching Algorithms" << std::endl;
    std::cout << "====================================================" << std::endl;

    return 0;

}
