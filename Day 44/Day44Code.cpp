/*
 * InterviewPatterns.cpp
 * Day 44 of #DSAin45 - Comprehensive Interview Pattern Examples
 *
 * This file demonstrates common DSA interview patterns with examples
 * and includes explanations of approaches that would be appropriate
 * to verbalize during an interview.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <deque>
#include <list>
#include <sstream>
#include <bitset>
#include <functional>
#include <numeric>

using namespace std;

// Basic data structures
struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Utility function to print a vector
template <typename T>
void printVector(const vector<T> &vec)
{
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++)
    {
        cout << vec[i];
        if (i < vec.size() - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}

// Utility function to print a 2D vector
template <typename T>
void print2DVector(const vector<vector<T>> &vec)
{
    cout << "[" << endl;
    for (const auto &row : vec)
    {
        cout << "  [";
        for (size_t i = 0; i < row.size(); i++)
        {
            cout << row[i];
            if (i < row.size() - 1)
                cout << ", ";
        }
        cout << "]" << endl;
    }
    cout << "]" << endl;
}

//==============================================================================
// PATTERN 1: Sliding Window
//==============================================================================
namespace SlidingWindow
{

    // Example 1: Maximum Sum Subarray of Size K
    /*
     * Interview Talking Points:
     * - This is a fixed-size sliding window problem
     * - We can use a sliding window to avoid recalculating the sum for overlapping subarrays
     * - Time complexity: O(n), Space complexity: O(1)
     * - Edge cases: array size < k
     */
    int maxSubArraySum(const vector<int> &nums, int k)
    {
        // Explain: First, I'll check for edge cases
        if (nums.size() < k)
        {
            return -1; // Not enough elements
        }

        // Explain: Initialize the window by calculating sum of first k elements
        int windowSum = 0;
        for (int i = 0; i < k; i++)
        {
            windowSum += nums[i];
        }

        int maxSum = windowSum;

        // Explain: Slide the window by adding next element and removing first element
        for (int i = k; i < nums.size(); i++)
        {
            windowSum = windowSum - nums[i - k] + nums[i];
            maxSum = max(maxSum, windowSum);
        }

        return maxSum;
    }

    // Example 2: Longest Substring Without Repeating Characters
    /*
     * Interview Talking Points:
     * - This is a variable-size sliding window problem
     * - We need to track character positions to handle duplicates
     * - Time complexity: O(n), Space complexity: O(min(m,n)) where m is alphabet size
     * - Edge cases: empty string
     */
    int lengthOfLongestSubstring(string s)
    {
        // Explain: I'll use a sliding window with a hashmap to track character positions
        unordered_map<char, int> charIndexMap;
        int maxLength = 0;
        int windowStart = 0;

        // Explain: Iterate through the string, extending the window when possible
        for (int windowEnd = 0; windowEnd < s.length(); windowEnd++)
        {
            char currentChar = s[windowEnd];

            // Explain: If we find a duplicate character, shrink the window
            if (charIndexMap.find(currentChar) != charIndexMap.end())
            {
                // Explain: Move window start to position after the first occurrence of the duplicate
                windowStart = max(windowStart, charIndexMap[currentChar] + 1);
            }

            // Explain: Update character position and max length
            charIndexMap[currentChar] = windowEnd;
            maxLength = max(maxLength, windowEnd - windowStart + 1);
        }

        return maxLength;
    }

    // Example 3: Minimum Size Subarray Sum
    /*
     * Interview Talking Points:
     * - This is a variable-size sliding window with a target sum
     * - We expand window until sum >= target, then shrink from left
     * - Time complexity: O(n), Space complexity: O(1)
     * - Edge cases: no subarray sums to target
     */
    int minSubArrayLen(int target, vector<int> &nums)
    {
        // Explain: I'll use a variable-size sliding window
        int windowSum = 0;
        int minLength = INT_MAX;
        int windowStart = 0;

        // Explain: Expand window with windowEnd pointer
        for (int windowEnd = 0; windowEnd < nums.size(); windowEnd++)
        {
            windowSum += nums[windowEnd];

            // Explain: Shrink window from the left while sum >= target
            while (windowSum >= target)
            {
                minLength = min(minLength, windowEnd - windowStart + 1);
                windowSum -= nums[windowStart];
                windowStart++;
            }
        }

        // Explain: Check if we found any valid subarray
        return (minLength != INT_MAX) ? minLength : 0;
    }

    // Demo of sliding window patterns
    void runSlidingWindowDemo()
    {
        cout << "\n=== SLIDING WINDOW PATTERN ===" << endl;

        // Example 1: Maximum Sum Subarray of Size K
        vector<int> nums1 = {2, 1, 5, 1, 3, 2};
        int k = 3;
        cout << "Array: ";
        printVector(nums1);
        cout << "Maximum sum of subarray of size " << k << ": " << maxSubArraySum(nums1, k) << endl;

        // Example 2: Longest Substring Without Repeating Characters
        string s = "abcabcbb";
        cout << "String: " << s << endl;
        cout << "Length of longest substring without repeating characters: " << lengthOfLongestSubstring(s) << endl;

        // Example 3: Minimum Size Subarray Sum
        vector<int> nums2 = {2, 3, 1, 2, 4, 3};
        int target = 7;
        cout << "Array: ";
        printVector(nums2);
        cout << "Minimum size subarray with sum >= " << target << ": " << minSubArrayLen(target, nums2) << endl;
    }
}

//==============================================================================
// PATTERN 2: Two Pointers
//==============================================================================
namespace TwoPointers
{

    // Example 1: Two Sum (input array is sorted)
    /*
     * Interview Talking Points:
     * - Using two pointers from opposite ends is efficient for sorted arrays
     * - Time complexity: O(n), Space complexity: O(1)
     * - Edge cases: empty array, no solution
     */
    vector<int> twoSum(vector<int> &numbers, int target)
    {
        // Explain: Using two pointers approach since array is sorted
        int left = 0;
        int right = numbers.size() - 1;

        while (left < right)
        {
            int currentSum = numbers[left] + numbers[right];

            if (currentSum == target)
            {
                // Explain: Return 1-indexed positions as per problem requirement
                return {left + 1, right + 1};
            }
            else if (currentSum < target)
            {
                // Explain: Need a larger sum, so move left pointer right
                left++;
            }
            else
            {
                // Explain: Need a smaller sum, so move right pointer left
                right--;
            }
        }

        // Explain: If no solution found, return empty array (problem states solution always exists)
        return {};
    }

    // Example 2: Remove Duplicates from Sorted Array
    /*
     * Interview Talking Points:
     * - Two pointers allow us to track unique elements while traversing
     * - Time complexity: O(n), Space complexity: O(1)
     * - Edge cases: empty array, all duplicates
     */
    int removeDuplicates(vector<int> &nums)
    {
        // Explain: Edge case check
        if (nums.empty())
        {
            return 0;
        }

        // Explain: Slow pointer keeps track of the position for unique elements
        int slow = 0;

        // Explain: Fast pointer scans through the array
        for (int fast = 1; fast < nums.size(); fast++)
        {
            // Explain: When we find a new unique element...
            if (nums[fast] != nums[slow])
            {
                // Explain: ...move slow pointer and update its value
                slow++;
                nums[slow] = nums[fast];
            }
        }

        // Explain: Return length of unique elements (slow + 1)
        return slow + 1;
    }

    // Example 3: Container With Most Water
    /*
     * Interview Talking Points:
     * - Two pointers help find the maximum area efficiently
     * - We move the pointer with the smaller height to maximize potential area
     * - Time complexity: O(n), Space complexity: O(1)
     * - Edge cases: less than 2 heights
     */
    int maxArea(vector<int> &height)
    {
        // Explain: Using two pointers to find maximum area
        int left = 0;
        int right = height.size() - 1;
        int maxWater = 0;

        while (left < right)
        {
            // Explain: Calculate current area
            int h = min(height[left], height[right]);
            int w = right - left;
            int area = h * w;

            // Explain: Update maximum area
            maxWater = max(maxWater, area);

            // Explain: Move the pointer with smaller height to potentially find larger area
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

    // Example 4: 3Sum
    /*
     * Interview Talking Points:
     * - We can use sorting and two pointers to find triplets
     * - Need to handle duplicates carefully
     * - Time complexity: O(n²), Space complexity: O(1) excluding output
     * - Edge cases: less than 3 elements, all zeroes, all duplicates
     */
    vector<vector<int>> threeSum(vector<int> &nums)
    {
        // Explain: Sort the array to use two pointers and handle duplicates
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;

        // Explain: Iterate through the array, fixing the first element
        for (int i = 0; i < (int)nums.size() - 2; i++)
        {
            // Explain: Skip duplicates for first element
            if (i > 0 && nums[i] == nums[i - 1])
            {
                continue;
            }

            // Explain: Use two pointers for the remaining elements
            int left = i + 1;
            int right = nums.size() - 1;
            int target = -nums[i]; // We want the sum to be 0

            while (left < right)
            {
                int sum = nums[left] + nums[right];

                if (sum == target)
                {
                    // Explain: Found a triplet
                    result.push_back({nums[i], nums[left], nums[right]});

                    // Explain: Skip duplicates for second element
                    while (left < right && nums[left] == nums[left + 1])
                    {
                        left++;
                    }

                    // Explain: Skip duplicates for third element
                    while (left < right && nums[right] == nums[right - 1])
                    {
                        right--;
                    }

                    // Explain: Move both pointers
                    left++;
                    right--;
                }
                else if (sum < target)
                {
                    // Explain: Need a larger sum, move left pointer
                    left++;
                }
                else
                {
                    // Explain: Need a smaller sum, move right pointer
                    right--;
                }
            }
        }

        return result;
    }

    // Demo of two pointers patterns
    void runTwoPointersDemo()
    {
        cout << "\n=== TWO POINTERS PATTERN ===" << endl;

        // Example 1: Two Sum Sorted
        vector<int> numbers = {2, 7, 11, 15};
        int target = 9;
        cout << "Array: ";
        printVector(numbers);
        cout << "Two sum with target " << target << ": ";
        printVector(twoSum(numbers, target));

        // Example 2: Remove Duplicates
        vector<int> nums = {1, 1, 2, 2, 3, 4, 5, 5};
        cout << "Array with duplicates: ";
        printVector(nums);
        int newLength = removeDuplicates(nums);
        cout << "After removing duplicates: ";
        printVector(vector<int>(nums.begin(), nums.begin() + newLength));

        // Example 3: Container With Most Water
        vector<int> height = {1, 8, 6, 2, 5, 4, 8, 3, 7};
        cout << "Heights: ";
        printVector(height);
        cout << "Maximum water container area: " << maxArea(height) << endl;

        // Example 4: 3Sum
        vector<int> nums2 = {-1, 0, 1, 2, -1, -4};
        cout << "Array for 3Sum: ";
        printVector(nums2);
        cout << "Triplets summing to 0: " << endl;
        print2DVector(threeSum(nums2));
    }
}

//==============================================================================
// PATTERN 3: Fast & Slow Pointers
//==============================================================================
namespace FastSlowPointers
{

    // Example 1: Linked List Cycle Detection
    /*
     * Interview Talking Points:
     * - Fast and slow pointers (tortoise and hare) can detect cycles
     * - Time complexity: O(n), Space complexity: O(1)
     * - Edge cases: empty list, single node
     */
    bool hasCycle(ListNode *head)
    {
        // Explain: Edge case check
        if (!head || !head->next)
        {
            return false;
        }

        // Explain: Initialize slow and fast pointers
        ListNode *slow = head;
        ListNode *fast = head;

        // Explain: Move slow by 1 step and fast by 2 steps
        while (fast && fast->next)
        {
            slow = slow->next;       // Move one step
            fast = fast->next->next; // Move two steps

            // Explain: If they meet, there's a cycle
            if (slow == fast)
            {
                return true;
            }
        }

        // Explain: If fast reaches the end, there's no cycle
        return false;
    }

    // Example 2: Find Cycle Start
    /*
     * Interview Talking Points:
     * - After detecting cycle, reset one pointer to head and move both at same speed
     * - Time complexity: O(n), Space complexity: O(1)
     * - Mathematical proof that they meet at cycle start
     */
    ListNode *detectCycle(ListNode *head)
    {
        // Explain: Edge case check
        if (!head || !head->next)
        {
            return nullptr;
        }

        // Explain: First detect if there's a cycle using two pointers
        ListNode *slow = head;
        ListNode *fast = head;
        bool hasCycle = false;

        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast)
            {
                hasCycle = true;
                break;
            }
        }

        // Explain: If no cycle is found, return null
        if (!hasCycle)
        {
            return nullptr;
        }

        // Explain: To find the start of the cycle, reset one pointer to head
        slow = head;
        while (slow != fast)
        {
            slow = slow->next;
            fast = fast->next;
        }

        // Explain: When they meet again, it's at the start of the cycle
        return slow;
    }

    // Example 3: Find Middle of Linked List
    /*
     * Interview Talking Points:
     * - Fast pointer moves twice as fast as slow
     * - When fast reaches end, slow is at middle
     * - Time complexity: O(n), Space complexity: O(1)
     * - Handle both even and odd length lists
     */
    ListNode *middleNode(ListNode *head)
    {
        // Explain: Edge case check
        if (!head || !head->next)
        {
            return head;
        }

        // Explain: Initialize slow and fast pointers
        ListNode *slow = head;
        ListNode *fast = head;

        // Explain: While fast can move forward...
        while (fast && fast->next)
        {
            slow = slow->next;       // Move one step
            fast = fast->next->next; // Move two steps
        }

        // Explain: When fast reaches end, slow is at middle
        return slow;
    }

    // Example 4: Check if a number is a palindrome
    /*
     * Interview Talking Points:
     * - Fast/slow approach can find middle of linked structure
     * - Can similarly work with digits of a number
     * - Time complexity: O(log n), Space complexity: O(1)
     * - Edge cases: negative numbers, single digit
     */
    bool isPalindrome(int x)
    {
        // Explain: Edge cases - negative numbers are not palindromes
        if (x < 0)
        {
            return false;
        }

        // Explain: Single digit is always a palindrome
        if (x < 10)
        {
            return true;
        }

        // Explain: If number ends with 0, it's a palindrome only if it's 0
        if (x % 10 == 0 && x != 0)
        {
            return false;
        }

        // Explain: Reverse half the number and compare
        int reversed = 0;
        while (x > reversed)
        {
            reversed = reversed * 10 + x % 10;
            x /= 10;
        }

        // Explain: For even digits, compare x == reversed
        // For odd digits, compare x == reversed/10 (to ignore middle digit)
        return x == reversed || x == reversed / 10;
    }

    // Create a linked list with a cycle for demonstration
    ListNode *createCyclicLinkedList()
    {
        ListNode *head = new ListNode(1);
        ListNode *node2 = new ListNode(2);
        ListNode *node3 = new ListNode(3);
        ListNode *node4 = new ListNode(4);
        ListNode *node5 = new ListNode(5);

        head->next = node2;
        node2->next = node3;
        node3->next = node4;
        node4->next = node5;
        node5->next = node3; // Create cycle pointing to node3

        return head;
    }

    // Create a linked list for demonstration
    ListNode *createLinkedList(const vector<int> &values)
    {
        if (values.empty())
        {
            return nullptr;
        }

        ListNode *head = new ListNode(values[0]);
        ListNode *current = head;

        for (size_t i = 1; i < values.size(); i++)
        {
            current->next = new ListNode(values[i]);
            current = current->next;
        }

        return head;
    }

    // Print a linked list (for non-cyclic lists)
    void printLinkedList(ListNode *head)
    {
        cout << "[";
        while (head)
        {
            cout << head->val;
            if (head->next)
            {
                cout << " -> ";
            }
            head = head->next;
        }
        cout << "]" << endl;
    }

    // Clean up linked list memory
    void cleanupLinkedList(ListNode *head, bool hasCycle = false)
    {
        if (!head || hasCycle)
        {
            return;
        }

        ListNode *current = head;
        while (current)
        {
            ListNode *next = current->next;
            delete current;
            current = next;
        }
    }

    // Demo of fast and slow pointers patterns
    void runFastSlowPointersDemo()
    {
        cout << "\n=== FAST & SLOW POINTERS PATTERN ===" << endl;

        // Example 1 & 2: Linked List Cycle Detection
        cout << "Cyclic linked list:" << endl;
        cout << "[1 -> 2 -> 3 -> 4 -> 5 -> (back to 3)]" << endl;

        ListNode *cyclicList = createCyclicLinkedList();
        cout << "Has cycle: " << (hasCycle(cyclicList) ? "Yes" : "No") << endl;

        ListNode *cycleStart = detectCycle(cyclicList);
        cout << "Cycle starts at node with value: " << (cycleStart ? to_string(cycleStart->val) : "N/A") << endl;

        // Example 3: Find Middle of Linked List
        vector<int> values = {1, 2, 3, 4, 5};
        ListNode *list = createLinkedList(values);
        cout << "Linked list: ";
        printLinkedList(list);

        ListNode *middle = middleNode(list);
        cout << "Middle node: " << middle->val << endl;

        // Example 4: Palindrome Number
        int num1 = 12321;
        int num2 = 12345;
        cout << num1 << " is palindrome: " << (isPalindrome(num1) ? "Yes" : "No") << endl;
        cout << num2 << " is palindrome: " << (isPalindrome(num2) ? "Yes" : "No") << endl;

        // Clean up memory (don't cleanup cyclic list with simple traversal)
        cleanupLinkedList(list);
    }
}

//==============================================================================
// PATTERN 4: Binary Search
//==============================================================================
namespace BinarySearch
{

    // Example 1: Basic Binary Search
    /*
     * Interview Talking Points:
     * - Works on sorted arrays
     * - Time complexity: O(log n), Space complexity: O(1)
     * - Edge cases: empty array, target not found
     * - Careful with integer overflow in mid calculation
     */
    int binarySearch(vector<int> &nums, int target)
    {
        // Explain: Initialize left and right pointers
        int left = 0;
        int right = nums.size() - 1;

        // Explain: Continue search while valid range exists
        while (left <= right)
        {
            // Explain: Calculate middle index (avoiding overflow)
            int mid = left + (right - left) / 2;

            // Explain: Found target
            if (nums[mid] == target)
            {
                return mid;
            }
            // Explain: If middle value is less than target, search right half
            else if (nums[mid] < target)
            {
                left = mid + 1;
            }
            // Explain: If middle value is greater than target, search left half
            else
            {
                right = mid - 1;
            }
        }

        // Explain: Target not found
        return -1;
    }

    // Example 2: Search in Rotated Sorted Array
    /*
     * Interview Talking Points:
     * - Modified binary search for rotated sorted array
     * - Check which half is normally ordered and use that for decision
     * - Time complexity: O(log n), Space complexity: O(1)
     * - Edge cases: no rotation, empty array
     */
    int searchRotated(vector<int> &nums, int target)
    {
        // Explain: Initialize left and right pointers
        int left = 0;
        int right = nums.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            // Explain: Check if middle element is the target
            if (nums[mid] == target)
            {
                return mid;
            }

            // Explain: Check if the left half is sorted
            if (nums[left] <= nums[mid])
            {
                // Explain: Check if target is in the left sorted half
                if (nums[left] <= target && target < nums[mid])
                {
                    right = mid - 1;
                }
                else
                {
                    left = mid + 1;
                }
            }
            // Explain: Otherwise, the right half is sorted
            else
            {
                // Explain: Check if target is in the right sorted half
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

        // Explain: Target not found
        return -1;
    }

    // Example 3: First Bad Version
    /*
     * Interview Talking Points:
     * - Binary search to find first occurrence
     * - Keep track of potential answers while narrowing search
     * - Time complexity: O(log n), Space complexity: O(1)
     * - Edge case: all versions are bad
     */
    // Mock API for first bad version problem
    bool isBadVersion(int version, int firstBad)
    {
        return version >= firstBad;
    }

    int firstBadVersion(int n, int firstBad)
    {
        // Explain: Initialize left and right pointers
        int left = 1;
        int right = n;

        while (left < right)
        {
            // Explain: Calculate middle (avoiding overflow)
            int mid = left + (right - left) / 2;

            // Explain: If middle version is bad, look in left half
            if (isBadVersion(mid, firstBad))
            {
                right = mid;
            }
            // Explain: If middle version is good, look in right half
            else
            {
                left = mid + 1;
            }
        }

        // Explain: left is now the first bad version
        return left;
    }

    // Example 4: Find Peak Element
    /*
     * Interview Talking Points:
     * - Binary search, moving toward higher values
     * - Valid because there's always a peak (nums[-1] = nums[n] = -∞)
     * - Time complexity: O(log n), Space complexity: O(1)
     * - Edge cases: single element array
     */
    int findPeakElement(vector<int> &nums)
    {
        // Explain: Initialize left and right pointers
        int left = 0;
        int right = nums.size() - 1;

        while (left < right)
        {
            int mid = left + (right - left) / 2;

            // Explain: If mid element is greater than next, peak is in left half
            if (nums[mid] > nums[mid + 1])
            {
                right = mid;
            }
            // Explain: Otherwise, peak is in right half
            else
            {
                left = mid + 1;
            }
        }

        // Explain: left is now at a peak element
        return left;
    }

    // Demo of binary search patterns
    void runBinarySearchDemo()
    {
        cout << "\n=== BINARY SEARCH PATTERN ===" << endl;

        // Example 1: Basic Binary Search
        vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int target = 6;
        cout << "Array: ";
        printVector(nums);
        cout << "Index of " << target << ": " << binarySearch(nums, target) << endl;

        // Example 2: Search in Rotated Sorted Array
        vector<int> rotated = {4, 5, 6, 7, 0, 1, 2};
        int rotatedTarget = 0;
        cout << "Rotated array: ";
        printVector(rotated);
        cout << "Index of " << rotatedTarget << " in rotated array: " << searchRotated(rotated, rotatedTarget) << endl;

        // Example 3: First Bad Version
        int n = 10;
        int firstBad = 7;
        cout << "Total versions: " << n << ", First bad version: " << firstBad << endl;
        cout << "Found first bad version: " << firstBadVersion(n, firstBad) << endl;

        // Example 4: Find Peak Element
        vector<int> peakArray = {1, 2, 3, 1};
        cout << "Array: ";
        printVector(peakArray);
        cout << "Peak element index: " << findPeakElement(peakArray) << endl;
    }
}

//==============================================================================
// PATTERN 5: Tree Traversal
//==============================================================================
namespace TreeTraversal
{

    // Example 1: Binary Tree Inorder Traversal
    /*
     * Interview Talking Points:
     * - Inorder traversal visits left-root-right
     * - Both recursive and iterative implementations
     * - Time complexity: O(n), Space complexity: O(h) where h is tree height
     * - Edge cases: empty tree
     */
    vector<int> inorderTraversal(TreeNode *root)
    {
        vector<int> result;
        inorderHelper(root, result);
        return result;
    }

    void inorderHelper(TreeNode *root, vector<int> &result)
    {
        // Explain: Base case - empty tree
        if (!root)
        {
            return;
        }

        // Explain: Inorder traversal: left, root, right
        inorderHelper(root->left, result);
        result.push_back(root->val);
        inorderHelper(root->right, result);
    }

    // Iterative inorder traversal
    vector<int> inorderTraversalIterative(TreeNode *root)
    {
        vector<int> result;
        stack<TreeNode *> stack;
        TreeNode *current = root;

        // Explain: Process tree iteratively
        while (current || !stack.empty())
        {
            // Explain: Go as far left as possible
            while (current)
            {
                stack.push(current);
                current = current->left;
            }

            // Explain: Process current node
            current = stack.top();
            stack.pop();
            result.push_back(current->val);

            // Explain: Move to right subtree
            current = current->right;
        }

        return result;
    }

    // Example 2: Binary Tree Level Order Traversal
    /*
     * Interview Talking Points:
     * - BFS approach using a queue
     * - Process level by level
     * - Time complexity: O(n), Space complexity: O(n)
     * - Edge cases: empty tree
     */
    vector<vector<int>> levelOrder(TreeNode *root)
    {
        vector<vector<int>> result;

        // Explain: Handle empty tree
        if (!root)
        {
            return result;
        }

        // Explain: Use queue for BFS
        queue<TreeNode *> q;
        q.push(root);

        while (!q.empty())
        {
            // Explain: Process current level
            int levelSize = q.size();
            vector<int> currentLevel;

            for (int i = 0; i < levelSize; i++)
            {
                TreeNode *node = q.front();
                q.pop();

                currentLevel.push_back(node->val);

                // Explain: Add children to the queue
                if (node->left)
                {
                    q.push(node->left);
                }

                if (node->right)
                {
                    q.push(node->right);
                }
            }

            result.push_back(currentLevel);
        }

        return result;
    }

    // Example 3: Maximum Depth of Binary Tree
    /*
     * Interview Talking Points:
     * - Classic recursive problem
     * - Depth is 1 + max of left and right subtree depths
     * - Time complexity: O(n), Space complexity: O(h) for recursion stack
     * - Edge cases: empty tree
     */
    int maxDepth(TreeNode *root)
    {
        // Explain: Base case - empty tree has depth 0
        if (!root)
        {
            return 0;
        }

        // Explain: Recursively find depth of left and right subtrees
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);

        // Explain: Depth is 1 + max of left and right depths
        return 1 + max(leftDepth, rightDepth);
    }

    // Example 4: Path Sum
    /*
     * Interview Talking Points:
     * - DFS to check if path with target sum exists
     * - Subtract node values from target as we go down
     * - Time complexity: O(n), Space complexity: O(h)
     * - Edge cases: empty tree
     */
    bool hasPathSum(TreeNode *root, int targetSum)
    {
        // Explain: Base case - empty tree
        if (!root)
        {
            return false;
        }

        // Explain: Subtract current node's value from target
        targetSum -= root->val;

        // Explain: Check if this is a leaf node and sum is 0
        if (!root->left && !root->right)
        {
            return targetSum == 0;
        }

        // Explain: Check left and right subtrees
        return hasPathSum(root->left, targetSum) || hasPathSum(root->right, targetSum);
    }

    // Create a binary tree for demonstration
    TreeNode *createBinaryTree()
    {
        TreeNode *root = new TreeNode(1);
        root->left = new TreeNode(2);
        root->right = new TreeNode(3);
        root->left->left = new TreeNode(4);
        root->left->right = new TreeNode(5);
        root->right->left = new TreeNode(6);
        root->right->right = new TreeNode(7);
        return root;
    }

    // Clean up binary tree memory
    void cleanupTree(TreeNode *root)
    {
        if (!root)
        {
            return;
        }

        cleanupTree(root->left);
        cleanupTree(root->right);
        delete root;
    }

    // Demo of tree traversal patterns
    void runTreeTraversalDemo()
    {
        cout << "\n=== TREE TRAVERSAL PATTERN ===" << endl;

        // Create a sample binary tree
        TreeNode *root = createBinaryTree();
        cout << "Binary Tree: 1-2-4-5-3-6-7" << endl;

        // Example 1: Inorder Traversal
        cout << "Inorder Traversal (recursive): ";
        printVector(inorderTraversal(root));

        cout << "Inorder Traversal (iterative): ";
        printVector(inorderTraversalIterative(root));

        // Example 2: Level Order Traversal
        cout << "Level Order Traversal:" << endl;
        print2DVector(levelOrder(root));

        // Example 3: Maximum Depth
        cout << "Maximum Depth: " << maxDepth(root) << endl;

        // Example 4: Path Sum
        int targetSum = 7; // Path 1-3-3 has sum 7
        cout << "Has path with sum " << targetSum << ": " << (hasPathSum(root, targetSum) ? "Yes" : "No") << endl;

        // Clean up memory
        cleanupTree(root);
    }
}

//==============================================================================
// PATTERN 6: Graph Traversal
//==============================================================================
namespace GraphTraversal
{

    // Example 1: Number of Islands (DFS)
    /*
     * Interview Talking Points:
     * - Classic DFS approach for connected components
     * - Mark visited islands to avoid revisiting
     * - Time complexity: O(m*n), Space complexity: O(m*n) for recursion stack
     * - Edge cases: empty grid
     */
    int numIslands(vector<vector<char>> &grid)
    {
        // Explain: Handle empty grid
        if (grid.empty() || grid[0].empty())
        {
            return 0;
        }

        int rows = grid.size();
        int cols = grid[0].size();
        int count = 0;

        // Explain: Iterate through each cell
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                // Explain: If we find land, explore the island
                if (grid[i][j] == '1')
                {
                    count++;
                    dfs(grid, i, j);
                }
            }
        }

        return count;
    }

    void dfs(vector<vector<char>> &grid, int r, int c)
    {
        int rows = grid.size();
        int cols = grid[0].size();

        // Explain: Check bounds and if this is land
        if (r < 0 || c < 0 || r >= rows || c >= cols || grid[r][c] != '1')
        {
            return;
        }

        // Explain: Mark as visited
        grid[r][c] = '0';

        // Explain: Explore all 4 directions
        dfs(grid, r + 1, c);
        dfs(grid, r - 1, c);
        dfs(grid, r, c + 1);
        dfs(grid, r, c - 1);
    }

    // Example 2: Course Schedule (Cycle Detection)
    /*
     * Interview Talking Points:
     * - Detect cycle in directed graph using DFS
     * - Use separate visited and in-progress markers
     * - Time complexity: O(V+E), Space complexity: O(V+E)
     * - Edge cases: no prerequisites, disconnected graph
     */
    bool canFinish(int numCourses, vector<vector<int>> &prerequisites)
    {
        // Explain: Build adjacency list
        vector<vector<int>> graph(numCourses);
        for (const auto &pre : prerequisites)
        {
            graph[pre[0]].push_back(pre[1]);
        }

        // Explain: 0 = unvisited, 1 = in-progress, 2 = finished
        vector<int> visited(numCourses, 0);

        // Explain: Check each course
        for (int i = 0; i < numCourses; i++)
        {
            if (visited[i] == 0 && hasCycle(graph, i, visited))
            {
                return false; // Cycle detected, cannot finish
            }
        }

        return true; // No cycle detected, can finish
    }

    bool hasCycle(vector<vector<int>> &graph, int node, vector<int> &visited)
    {
        // Explain: Mark node as in-progress
        visited[node] = 1;

        // Explain: Check all neighbors
        for (int neighbor : graph[node])
        {
            // Explain: If neighbor is in-progress, we found a cycle
            if (visited[neighbor] == 1)
            {
                return true;
            }

            // Explain: If neighbor is unvisited, check if it leads to a cycle
            if (visited[neighbor] == 0 && hasCycle(graph, neighbor, visited))
            {
                return true;
            }
        }

        // Explain: Mark node as finished
        visited[node] = 2;
        return false;
    }

    // Example 3: Word Ladder (BFS)
    /*
     * Interview Talking Points:
     * - BFS for shortest path in unweighted graph
     * - Character-by-character transformation
     * - Time complexity: O(n * m^2) where n is wordList size and m is word length
     * - Edge cases: endWord not in wordList, no transformation possible
     */
    int ladderLength(string beginWord, string endWord, vector<string> &wordList)
    {
        // Explain: Convert wordList to a set for O(1) lookup
        unordered_set<string> wordSet(wordList.begin(), wordList.end());

        // Explain: Check if endWord is in the list
        if (wordSet.find(endWord) == wordSet.end())
        {
            return 0;
        }

        // Explain: Use queue for BFS
        queue<string> q;
        q.push(beginWord);

        // Explain: Keep track of the transformations
        int transformations = 1;

        while (!q.empty())
        {
            int levelSize = q.size();

            // Explain: Process current level
            for (int i = 0; i < levelSize; i++)
            {
                string currentWord = q.front();
                q.pop();

                // Explain: Try changing each character
                for (int j = 0; j < currentWord.length(); j++)
                {
                    char originalChar = currentWord[j];

                    // Explain: Try all 26 letters
                    for (char c = 'a'; c <= 'z'; c++)
                    {
                        if (c == originalChar)
                        {
                            continue;
                        }

                        currentWord[j] = c;

                        // Explain: Check if we reached the target
                        if (currentWord == endWord)
                        {
                            return transformations + 1;
                        }

                        // Explain: Check if transformed word is in the list
                        if (wordSet.find(currentWord) != wordSet.end())
                        {
                            q.push(currentWord);
                            wordSet.erase(currentWord); // Mark as visited
                        }
                    }

                    // Explain: Restore the original character
                    currentWord[j] = originalChar;
                }
            }

            transformations++;
        }

        return 0; // No transformation sequence found
    }

    // Example 4: Clone Graph (DFS)
    /*
     * Interview Talking Points:
     * - Deep copy of a graph using DFS with a hash map
     * - Time complexity: O(V+E), Space complexity: O(V)
     * - Edge cases: empty graph, disconnected graph
     */
    // Node definition for graph
    class Node
    {
    public:
        int val;
        vector<Node *> neighbors;

        Node()
        {
            val = 0;
            neighbors = vector<Node *>();
        }

        Node(int _val)
        {
            val = _val;
            neighbors = vector<Node *>();
        }

        Node(int _val, vector<Node *> _neighbors)
        {
            val = _val;
            neighbors = _neighbors;
        }
    };

    Node *cloneGraph(Node *node)
    {
        // Explain: Handle empty graph
        if (!node)
        {
            return nullptr;
        }

        // Explain: Use hash map to track visited nodes
        unordered_map<Node *, Node *> visited;
        return cloneGraphDFS(node, visited);
    }

    Node *cloneGraphDFS(Node *node, unordered_map<Node *, Node *> &visited)
    {
        // Explain: Check if node already cloned
        if (visited.find(node) != visited.end())
        {
            return visited[node];
        }

        // Explain: Create a new node
        Node *clonedNode = new Node(node->val);
        visited[node] = clonedNode;

        // Explain: Clone all neighbors
        for (Node *neighbor : node->neighbors)
        {
            clonedNode->neighbors.push_back(cloneGraphDFS(neighbor, visited));
        }

        return clonedNode;
    }

    // Demo of graph traversal patterns
    void runGraphTraversalDemo()
    {
        cout << "\n=== GRAPH TRAVERSAL PATTERN ===" << endl;

        // Example 1: Number of Islands
        vector<vector<char>> grid = {
            {'1', '1', '0', '0', '0'},
            {'1', '1', '0', '0', '0'},
            {'0', '0', '1', '0', '0'},
            {'0', '0', '0', '1', '1'}};

        cout << "Grid:" << endl;
        for (const auto &row : grid)
        {
            cout << "  ";
            for (char c : row)
            {
                cout << c << " ";
            }
            cout << endl;
        }

        cout << "Number of islands: " << numIslands(grid) << endl;

        // Example 2: Course Schedule
        vector<vector<int>> prerequisites = {{1, 0}, {0, 2}, {2, 1}};
        int numCourses = 3;

        cout << "Prerequisites: ";
        for (const auto &pre : prerequisites)
        {
            cout << pre[0] << " depends on " << pre[1] << ", ";
        }
        cout << endl;

        cout << "Can finish all courses: " << (canFinish(numCourses, prerequisites) ? "Yes" : "No") << endl;

        // Example 3: Word Ladder
        string beginWord = "hit";
        string endWord = "cog";
        vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};

        cout << "Begin word: " << beginWord << ", End word: " << endWord << endl;
        cout << "Word list: ";
        printVector(wordList);

        cout << "Shortest transformation sequence length: " << ladderLength(beginWord, endWord, wordList) << endl;
    }
}

//==============================================================================
// PATTERN 7: Dynamic Programming
//==============================================================================
namespace DynamicProgramming
{

    // Example 1: Fibonacci with Memoization
    /*
     * Interview Talking Points:
     * - Top-down DP with memoization
     * - Classic example to demonstrate overlapping subproblems
     * - Time complexity: O(n), Space complexity: O(n)
     * - Edge cases: n <= 1
     */
    int fibMemoized(int n, unordered_map<int, int> &memo)
    {
        // Explain: Base cases
        if (n <= 1)
        {
            return n;
        }

        // Explain: Check if we've already computed this value
        if (memo.find(n) != memo.end())
        {
            return memo[n];
        }

        // Explain: Compute and store the result
        memo[n] = fibMemoized(n - 1, memo) + fibMemoized(n - 2, memo);
        return memo[n];
    }

    int fib(int n)
    {
        // Explain: Initialize memo table
        unordered_map<int, int> memo;
        return fibMemoized(n, memo);
    }

    // Example 2: Climbing Stairs
    /*
     * Interview Talking Points:
     * - Bottom-up DP approach
     * - Similar to Fibonacci but with practical application
     * - Time complexity: O(n), Space complexity: O(1) optimized
     * - Edge cases: n <= 2
     */
    int climbStairs(int n)
    {
        // Explain: Handle edge cases
        if (n <= 2)
        {
            return n;
        }

        // Explain: Optimize to use only two variables instead of array
        int oneStepBefore = 2;  // Ways to climb 2 stairs
        int twoStepsBefore = 1; // Ways to climb 1 stair
        int allWays = 0;

        // Explain: Build up the solution
        for (int i = 3; i <= n; i++)
        {
            allWays = oneStepBefore + twoStepsBefore;
            twoStepsBefore = oneStepBefore;
            oneStepBefore = allWays;
        }

        return allWays;
    }

    // Example 3: Coin Change
    /*
     * Interview Talking Points:
     * - Classic DP problem (minimum coins for a sum)
     * - State: minimum coins needed for amount i
     * - Recurrence: dp[i] = min(dp[i], dp[i-coin] + 1)
     * - Time complexity: O(amount * coins), Space complexity: O(amount)
     * - Edge cases: amount = 0, no valid combination
     */
    int coinChange(vector<int> &coins, int amount)
    {
        // Explain: Initialize DP array with amount+1 (more than any possible value)
        vector<int> dp(amount + 1, amount + 1);

        // Explain: Base case - 0 coins needed for amount 0
        dp[0] = 0;

        // Explain: Build up the solution
        for (int coin : coins)
        {
            for (int i = coin; i <= amount; i++)
            {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }

        // Explain: Check if we have a valid solution
        return dp[amount] > amount ? -1 : dp[amount];
    }

    // Example 4: Longest Increasing Subsequence
    /*
     * Interview Talking Points:
     * - DP approach for subsequence problems
     * - State: length of LIS ending at index i
     * - Time complexity: O(n²), Space complexity: O(n)
     * - Binary search approach for O(n log n) also exists
     * - Edge cases: empty array
     */
    int lengthOfLIS(vector<int> &nums)
    {
        // Explain: Handle empty array
        if (nums.empty())
        {
            return 0;
        }

        int n = nums.size();

        // Explain: Initialize DP array where dp[i] represents length of LIS ending at i
        vector<int> dp(n, 1);

        // Explain: For each element, check all previous elements
        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                // Explain: If current number is greater, we can extend the sequence
                if (nums[i] > nums[j])
                {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
        }

        // Explain: Find the maximum length in the DP array
        return *max_element(dp.begin(), dp.end());
    }

    // Demo of dynamic programming patterns
    void runDynamicProgrammingDemo()
    {
        cout << "\n=== DYNAMIC PROGRAMMING PATTERN ===" << endl;

        // Example 1: Fibonacci
        int n = 10;
        cout << "Fibonacci of " << n << ": " << fib(n) << endl;

        // Example 2: Climbing Stairs
        int steps = 5;
        cout << "Ways to climb " << steps << " stairs: " << climbStairs(steps) << endl;

        // Example 3: Coin Change
        vector<int> coins = {1, 2, 5};
        int amount = 11;
        cout << "Coins: ";
        printVector(coins);
        cout << "Minimum coins to make " << amount << ": " << coinChange(coins, amount) << endl;

        // Example 4: Longest Increasing Subsequence
        vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
        cout << "Array: ";
        printVector(nums);
        cout << "Length of longest increasing subsequence: " << lengthOfLIS(nums) << endl;
    }
}

//==============================================================================
// PATTERN 8: Backtracking
//==============================================================================
namespace Backtracking
{

    // Example 1: Subsets
    /*
     * Interview Talking Points:
     * - Generate all subsets of a set (power set)
     * - Classic backtracking with inclusion/exclusion choices
     * - Time complexity: O(n * 2^n), Space complexity: O(n * 2^n)
     * - Edge cases: empty set
     */
    vector<vector<int>> subsets(vector<int> &nums)
    {
        vector<vector<int>> result;
        vector<int> current;

        // Explain: Start backtracking from index 0
        backtrack(nums, 0, current, result);
        return result;
    }

    void backtrack(vector<int> &nums, int start, vector<int> &current,
                   vector<vector<int>> &result)
    {
        // Explain: Add current subset to result
        result.push_back(current);

        // Explain: Try all possible next elements
        for (int i = start; i < nums.size(); i++)
        {
            // Explain: Include current element
            current.push_back(nums[i]);

            // Explain: Recur with next position
            backtrack(nums, i + 1, current, result);

            // Explain: Backtrack - remove the element
            current.pop_back();
        }
    }

    // Example 2: Permutations
    /*
     * Interview Talking Points:
     * - Generate all permutations of a set
     * - Backtracking with swapping or marking used elements
     * - Time complexity: O(n!), Space complexity: O(n * n!)
     * - Edge cases: empty set
     */
    vector<vector<int>> permute(vector<int> &nums)
    {
        vector<vector<int>> result;

        // Explain: Start backtracking from index 0
        permuteBacktrack(nums, 0, result);
        return result;
    }

    void permuteBacktrack(vector<int> &nums, int start, vector<vector<int>> &result)
    {
        // Explain: If we've placed all elements, add permutation to result
        if (start == nums.size())
        {
            result.push_back(nums);
            return;
        }

        // Explain: Try all possible elements for current position
        for (int i = start; i < nums.size(); i++)
        {
            // Explain: Swap to place element at current position
            swap(nums[start], nums[i]);

            // Explain: Recur with next position
            permuteBacktrack(nums, start + 1, result);

            // Explain: Backtrack - restore array
            swap(nums[start], nums[i]);
        }
    }

    // Example 3: N-Queens
    /*
     * Interview Talking Points:
     * - Classic backtracking problem
     * - Need to track row, column, and diagonals
     * - Time complexity: O(n!), Space complexity: O(n)
     * - Edge case: n = 1
     */
    vector<vector<string>> solveNQueens(int n)
    {
        vector<vector<string>> result;

        // Explain: Initialize empty board
        vector<string> board(n, string(n, '.'));

        // Explain: Start backtracking from first row
        solveNQueensBacktrack(board, 0, result);

        return result;
    }

    void solveNQueensBacktrack(vector<string> &board, int row, vector<vector<string>> &result)
    {
        // Explain: If we've placed queens in all rows, add solution
        if (row == board.size())
        {
            result.push_back(board);
            return;
        }

        int n = board.size();

        // Explain: Try placing queen in each column
        for (int col = 0; col < n; col++)
        {
            // Explain: Check if it's safe to place a queen
            if (isValid(board, row, col))
            {
                // Explain: Place queen
                board[row][col] = 'Q';

                // Explain: Recur with next row
                solveNQueensBacktrack(board, row + 1, result);

                // Explain: Backtrack - remove queen
                board[row][col] = '.';
            }
        }
    }

    bool isValid(vector<string> &board, int row, int col)
    {
        int n = board.size();

        // Explain: Check column
        for (int i = 0; i < row; i++)
        {
            if (board[i][col] == 'Q')
            {
                return false;
            }
        }

        // Explain: Check upper-left diagonal
        for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
        {
            if (board[i][j] == 'Q')
            {
                return false;
            }
        }

        // Explain: Check upper-right diagonal
        for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++)
        {
            if (board[i][j] == 'Q')
            {
                return false;
            }
        }

        return true;
    }

    // Example 4: Combination Sum
    /*
     * Interview Talking Points:
     * - Find all combinations that sum to a target
     * - Elements can be reused
     * - Time complexity: O(n^target), Space complexity: O(target)
     * - Edge cases: empty candidates, no valid combinations
     */
    vector<vector<int>> combinationSum(vector<int> &candidates, int target)
    {
        vector<vector<int>> result;
        vector<int> current;

        // Explain: Start backtracking from index 0
        combinationSumBacktrack(candidates, target, 0, current, result);

        return result;
    }

    void combinationSumBacktrack(vector<int> &candidates, int target, int start,
                                 vector<int> &current, vector<vector<int>> &result)
    {
        // Explain: If target is reached, add to result
        if (target == 0)
        {
            result.push_back(current);
            return;
        }

        // Explain: Try all candidates from start index
        for (int i = start; i < candidates.size(); i++)
        {
            // Explain: Skip if candidate is too large
            if (candidates[i] > target)
            {
                continue;
            }

            // Explain: Include current candidate
            current.push_back(candidates[i]);

            // Explain: Recur with reduced target and same start index (can reuse)
            combinationSumBacktrack(candidates, target - candidates[i], i, current, result);

            // Explain: Backtrack - remove candidate
            current.pop_back();
        }
    }

    // Demo of backtracking patterns
    void runBacktrackingDemo()
    {
        cout << "\n=== BACKTRACKING PATTERN ===" << endl;

        // Example 1: Subsets
        vector<int> nums1 = {1, 2, 3};
        cout << "Array: ";
        printVector(nums1);
        cout << "All subsets:" << endl;
        print2DVector(subsets(nums1));

        // Example 2: Permutations
        vector<int> nums2 = {1, 2, 3};
        cout << "Array: ";
        printVector(nums2);
        cout << "All permutations:" << endl;
        print2DVector(permute(nums2));

        // Example 3: N-Queens
        int n = 4;
        cout << "N-Queens for n = " << n << ":" << endl;
        vector<vector<string>> nQueensSolutions = solveNQueens(n);
        for (int i = 0; i < min(2, (int)nQueensSolutions.size()); i++)
        {
            cout << "Solution " << i + 1 << ":" << endl;
            for (const string &row : nQueensSolutions[i])
            {
                cout << "  " << row << endl;
            }
        }
        if (nQueensSolutions.size() > 2)
        {
            cout << "  ..." << endl;
        }

        // Example 4: Combination Sum
        vector<int> candidates = {2, 3, 6, 7};
        int target = 7;
        cout << "Candidates: ";
        printVector(candidates);
        cout << "Target: " << target << endl;
        cout << "Combinations that sum to target:" << endl;
        print2DVector(combinationSum(candidates, target));
    }
}

//==============================================================================
// Interview Time Management & Communication Strategies
//==============================================================================
namespace InterviewStrategies
{

    // Structured problem-solving example
    void demonstrateStructuredApproach()
    {
        // Problem: Find the first and last position of a target element in a sorted array
        cout << "\n=== STRUCTURED PROBLEM-SOLVING APPROACH ===" << endl;

        cout << "PROBLEM: Given a sorted array of integers and a target value, find the first and "
             << "last position of the target in the array. If the target is not found, return [-1, -1]." << endl;

        cout << "\nSTEP 1: UNDERSTAND THE PROBLEM" << endl;
        cout << "- Input: Sorted array of integers, target value" << endl;
        cout << "- Output: Array [firstPos, lastPos] of the target positions" << endl;
        cout << "- Edge cases: Empty array, target not found, single occurrence, multiple occurrences" << endl;
        cout << "- Clarifying question: Does the array contain duplicates? Yes, it can." << endl;

        cout << "\nSTEP 2: PLAN THE APPROACH" << endl;
        cout << "- Since the array is sorted, we can use binary search" << endl;
        cout << "- We'll need two binary searches:" << endl;
        cout << "  1. Find the first occurrence (leftmost)" << endl;
        cout << "  2. Find the last occurrence (rightmost)" << endl;
        cout << "- Time complexity will be O(log n)" << endl;

        cout << "\nSTEP 3: IMPLEMENTATION" << endl;
        cout << "```cpp\n"
             << "vector<int> searchRange(vector<int>& nums, int target) {\n"
             << "    vector<int> result = {-1, -1};\n"
             << "    if (nums.empty()) return result;\n\n"
             << "    // Find first occurrence\n"
             << "    int left = 0, right = nums.size() - 1;\n"
             << "    while (left < right) {\n"
             << "        int mid = left + (right - left) / 2;\n"
             << "        if (nums[mid] < target) left = mid + 1;\n"
             << "        else right = mid;\n"
             << "    }\n"
             << "    if (nums[left] != target) return result;\n"
             << "    result[0] = left;\n\n"
             << "    // Find last occurrence\n"
             << "    right = nums.size() - 1;\n"
             << "    while (left < right) {\n"
             << "        // +1 to handle adjacent elements\n"
             << "        int mid = left + (right - left + 1) / 2;\n"
             << "        if (nums[mid] > target) right = mid - 1;\n"
             << "        else left = mid;\n"
             << "    }\n"
             << "    result[1] = right;\n\n"
             << "    return result;\n"
             << "}\n"
             << "```" << endl;

        cout << "\nSTEP 4: TESTING" << endl;
        cout << "Test Case 1: nums = [5,7,7,8,8,10], target = 8" << endl;
        cout << "Expected Output: [3,4]" << endl;
        cout << "Trace:\n"
             << "  First occurrence:\n"
             << "    Initial: left=0, right=5\n"
             << "    mid=2, nums[mid]=7 < 8, left=mid+1=3\n"
             << "    left=3, right=5, mid=4, nums[mid]=8 == 8, right=mid=4\n"
             << "    left=3, right=4, mid=3, nums[mid]=8 == 8, right=mid=3\n"
             << "    left=3, right=3 (exit loop)\n"
             << "    nums[left]=8 == 8, result[0]=3\n"
             << "  Last occurrence:\n"
             << "    left=3, right=5, mid=4, nums[mid]=8 == 8, left=mid=4\n"
             << "    left=4, right=5, mid=5, nums[mid]=10 > 8, right=mid-1=4\n"
             << "    left=4, right=4 (exit loop)\n"
             << "    result[1]=4\n"
             << "  Return [3,4]" << endl;

        cout << "Test Case 2: nums = [5,7,7,8,8,10], target = 6" << endl;
        cout << "Expected Output: [-1,-1]" << endl;

        cout << "Test Case 3: nums = [], target = 0" << endl;
        cout << "Expected Output: [-1,-1]" << endl;

        cout << "\nSTEP 5: OPTIMIZATION" << endl;
        cout << "- Our solution is already optimal with O(log n) time complexity" << endl;
        cout << "- Space complexity is O(1) excluding the output array" << endl;
        cout << "- One potential optimization is to extract the binary search into a helper function to avoid code duplication" << endl;
    }

    // Time management demonstration
    void demonstrateTimeManagement()
    {
        cout << "\n=== TIME MANAGEMENT DURING INTERVIEWS ===" << endl;

        cout << "45-Minute Interview Structure:" << endl;
        cout << "- 0-5 min: Understand problem, ask questions, discuss examples" << endl;
        cout << "- 5-10 min: Brainstorm approaches, outline solution plan" << endl;
        cout << "- 10-25 min: Implementation" << endl;
        cout << "- 25-35 min: Testing and debugging" << endl;
        cout << "- 35-40 min: Optimize (if needed)" << endl;
        cout << "- 40-45 min: Discuss complexity and alternative approaches" << endl;

        cout << "\nCommon Time-Wasters to Avoid:" << endl;
        cout << "- Jumping into code without a clear plan" << endl;
        cout << "- Getting stuck on a single approach for too long" << endl;
        cout << "- Debugging complex implementations silently" << endl;
        cout << "- Overexplaining simple concepts" << endl;
        cout << "- Perfectionism in writing code" << endl;

        cout << "\nTime-Saving Strategies:" << endl;
        cout << "- Start with a brute force solution, then optimize" << endl;
        cout << "- Use pseudocode to outline complex logic first" << endl;
        cout << "- Modularize your code for better readability" << endl;
        cout << "- Ask for hints if stuck for more than 5 minutes" << endl;
        cout << "- Skip trivial implementation details with a comment (with interviewer's agreement)" << endl;
    }

    // Communication strategies demonstration
    void demonstrateCommunication()
    {
        cout << "\n=== COMMUNICATION STRATEGIES ===" << endl;

        cout << "Thinking Aloud Effectively:" << endl;
        cout << "- Verbalize your thought process: \"I'm thinking of using a two-pointer approach because...\"" << endl;
        cout << "- Acknowledge trade-offs: \"This approach has O(n) time complexity, but requires O(n) extra space.\"" << endl;
        cout << "- Structure your thoughts: \"First, I'll do X. Then, I'll implement Y.\"" << endl;
        cout << "- Ask for feedback: \"Does this approach seem reasonable to you?\"" << endl;

        cout << "\nResponding to Hints:" << endl;
        cout << "- Listen carefully to what the interviewer is suggesting" << endl;
        cout << "- Don't dismiss hints without consideration" << endl;
        cout << "- Build on suggestions: \"That's a good point, I could use a hash map here instead.\"" << endl;
        cout << "- Thank the interviewer for helpful guidance" << endl;

        cout << "\nHandling Challenges:" << endl;
        cout << "- When stuck: \"I'm having trouble with this part. Let me take a step back and reconsider.\"" << endl;
        cout << "- When making a mistake: \"I see the issue here. Let me correct that.\"" << endl;
        cout << "- When you don't know: \"I'm not immediately familiar with that, but I'd approach it by...\"" << endl;
        cout << "- Time constraints: \"I'm aware we're short on time. Let me implement the most critical parts first.\"" << endl;
    }

    // Final interview checklist
    void demonstrateFinalChecklist()
    {
        cout << "\n=== FINAL INTERVIEW CHECKLIST ===" << endl;

        cout << "Before the Interview:" << endl;
        cout << "✅ Research the company and role" << endl;
        cout << "✅ Review your resume and projects" << endl;
        cout << "✅ Practice common problem patterns" << endl;
        cout << "✅ Prepare a few questions for the interviewer" << endl;
        cout << "✅ Test your technical setup (for remote interviews)" << endl;
        cout << "✅ Get a good night's sleep" << endl;

        cout << "\nDuring the Interview:" << endl;
        cout << "✅ Listen carefully to the problem" << endl;
        cout << "✅ Ask clarifying questions" << endl;
        cout << "✅ Discuss your approach before coding" << endl;
        cout << "✅ Think aloud as you solve the problem" << endl;
        cout << "✅ Test your solution" << endl;
        cout << "✅ Engage with the interviewer's feedback" << endl;

        cout << "\nAfter the Interview:" << endl;
        cout << "✅ Send a thank-you note" << endl;
        cout << "✅ Reflect on what went well and what didn't" << endl;
        cout << "✅ Note any new concepts or techniques you learned" << endl;
        cout << "✅ Follow up appropriately if you don't hear back" << endl;
    }

    // Run demonstration of interview strategies
    void runInterviewStrategiesDemo()
    {
        cout << "\n=== INTERVIEW PREPARATION STRATEGIES ===" << endl;

        demonstrateStructuredApproach();
        demonstrateTimeManagement();
        demonstrateCommunication();
        demonstrateFinalChecklist();

        cout << "\nCOMPLEXITY CHEAT SHEET:" << endl;
        cout << "Array Access: O(1)" << endl;
        cout << "Binary Search: O(log n)" << endl;
        cout << "Sorting: O(n log n)" << endl;
        cout << "Hash Table Lookup: O(1) average" << endl;
        cout << "BFS/DFS: O(V + E)" << endl;
        cout << "Dijkstra's Algorithm: O(E log V)" << endl;
    }
}

//==============================================================================
// Main function
//==============================================================================

int main()
{
    cout << "====================================================" << endl;
    cout << "Day 44: Interview Preparation Strategies - DSA in 45" << endl;
    cout << "====================================================" << endl;

    // Run demos for each pattern
    SlidingWindow::runSlidingWindowDemo();
    TwoPointers::runTwoPointersDemo();
    FastSlowPointers::runFastSlowPointersDemo();
    BinarySearch::runBinarySearchDemo();
    TreeTraversal::runTreeTraversalDemo();
    GraphTraversal::runGraphTraversalDemo();
    DynamicProgramming::runDynamicProgrammingDemo();
    Backtracking::runBacktrackingDemo();

    // Run demo for interview strategies
    InterviewStrategies::runInterviewStrategiesDemo();

    return 0;
}