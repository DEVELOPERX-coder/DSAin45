#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <string>
#include <deque>
#include <queue>
#include <list>

/**
 * @file TwoPointerTechniques.cpp
 * @brief Comprehensive implementation of the Two-Pointer Technique in C++
 *
 * This file contains implementations of various two-pointer patterns:
 * 1. Opposite Direction (Converging) Pattern
 * 2. Same Direction (Fast & Slow) Pattern
 * 3. Sliding Window Pattern
 *
 * Each pattern includes multiple example problems with detailed explanations
 * and complexity analysis.
 *
 * @author #DSAin45 Series
 * @date Day 20
 */

// ============================================================================
// Helper Classes (for demonstration purposes)
// ============================================================================

/**
 * Definition for singly-linked list node.
 */
struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

/**
 * Utility function to print a vector of integers
 */
void printVector(const std::vector<int> &vec, const std::string &label = "")
{
    if (!label.empty())
    {
        std::cout << label << ": ";
    }

    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i)
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
 * Utility function to print a linked list
 */
void printLinkedList(ListNode *head, const std::string &label = "")
{
    if (!label.empty())
    {
        std::cout << label << ": ";
    }

    std::cout << "[";
    ListNode *current = head;
    while (current)
    {
        std::cout << current->val;
        if (current->next)
        {
            std::cout << " -> ";
        }
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

/**
 * Utility function to create a linked list from a vector
 */
ListNode *createLinkedList(const std::vector<int> &values)
{
    if (values.empty())
    {
        return nullptr;
    }

    ListNode *head = new ListNode(values[0]);
    ListNode *current = head;

    for (size_t i = 1; i < values.size(); ++i)
    {
        current->next = new ListNode(values[i]);
        current = current->next;
    }

    return head;
}

/**
 * Utility function to free memory allocated for a linked list
 */
void deleteLinkedList(ListNode *head)
{
    ListNode *current = head;
    while (current)
    {
        ListNode *next = current->next;
        delete current;
        current = next;
    }
}

/**
 * Utility function to measure execution time of a function
 */
template <typename Func, typename... Args>
long long measureExecutionTime(Func func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// ============================================================================
// 1. Opposite Direction (Converging) Pattern
// ============================================================================

namespace ConvergingPointers
{

    /**
     * Problem: Two Sum (Sorted Array)
     * Given a sorted array of integers, find two numbers that add up to a specific target.
     * Return the indices of the two numbers (1-indexed).
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    std::vector<int> twoSum(std::vector<int> &numbers, int target)
    {
        int left = 0;
        int right = numbers.size() - 1;

        while (left < right)
        {
            int sum = numbers[left] + numbers[right];

            if (sum == target)
            {
                // Return 1-indexed result
                return {left + 1, right + 1};
            }
            else if (sum < target)
            {
                // Sum is too small, need to increase it by moving left pointer right
                left++;
            }
            else
            {
                // Sum is too large, need to decrease it by moving right pointer left
                right--;
            }
        }

        // If no solution found
        return {-1, -1};
    }

    /**
     * Problem: Valid Palindrome
     * Given a string, determine if it is a palindrome, considering only alphanumeric
     * characters and ignoring case.
     *
     * Time Complexity: O(n) - Each character is processed at most once
     * Space Complexity: O(1) - Constant extra space
     */
    bool isPalindrome(std::string s)
    {
        int left = 0;
        int right = s.size() - 1;

        while (left < right)
        {
            // Skip non-alphanumeric characters from the left
            if (!std::isalnum(s[left]))
            {
                left++;
                continue;
            }

            // Skip non-alphanumeric characters from the right
            if (!std::isalnum(s[right]))
            {
                right--;
                continue;
            }

            // Compare characters ignoring case
            if (std::tolower(s[left]) != std::tolower(s[right]))
            {
                return false;
            }

            left++;
            right--;
        }

        return true;
    }

    /**
     * Problem: Container With Most Water
     * Given n non-negative integers representing heights of lines, find two lines
     * that together with the x-axis form a container that holds the most water.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    int maxArea(std::vector<int> &height)
    {
        int left = 0;
        int right = height.size() - 1;
        int maxWater = 0;

        while (left < right)
        {
            // Calculate current container area
            int width = right - left;
            int currentHeight = std::min(height[left], height[right]);
            int currentArea = width * currentHeight;

            // Update maximum area
            maxWater = std::max(maxWater, currentArea);

            // Move the pointer at the shorter line
            // (moving the pointer at the taller line would only decrease the area)
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

    /**
     * Problem: 3Sum
     * Given an array of integers, find all unique triplets that sum to zero.
     *
     * Time Complexity: O(n²) - One loop O(n) and two-pointer technique O(n)
     * Space Complexity: O(1) - Excluding output array
     */
    std::vector<std::vector<int>> threeSum(std::vector<int> &nums)
    {
        std::vector<std::vector<int>> result;

        // Sort the array first (required for the two-pointer approach)
        std::sort(nums.begin(), nums.end());

        for (int i = 0; i < static_cast<int>(nums.size()) - 2; i++)
        {
            // Skip duplicates for the first element
            if (i > 0 && nums[i] == nums[i - 1])
            {
                continue;
            }

            // Use two pointers to find pairs that sum to -nums[i]
            int left = i + 1;
            int right = nums.size() - 1;
            int target = -nums[i];

            while (left < right)
            {
                int sum = nums[left] + nums[right];

                if (sum < target)
                {
                    left++;
                }
                else if (sum > target)
                {
                    right--;
                }
                else
                {
                    // Found a triplet that sums to zero
                    result.push_back({nums[i], nums[left], nums[right]});

                    // Skip duplicates for the second element
                    while (left < right && nums[left] == nums[left + 1])
                    {
                        left++;
                    }

                    // Skip duplicates for the third element
                    while (left < right && nums[right] == nums[right - 1])
                    {
                        right--;
                    }

                    left++;
                    right--;
                }
            }
        }

        return result;
    }

    /**
     * Problem: Reverse Vowels of a String
     * Reverse only the vowels in a string.
     *
     * Time Complexity: O(n) - Each character is processed at most once
     * Space Complexity: O(1) - Constant extra space (not counting the input/output string)
     */
    std::string reverseVowels(std::string s)
    {
        int left = 0;
        int right = s.size() - 1;

        // Helper function to check if a character is a vowel
        auto isVowel = [](char c)
        {
            c = std::tolower(c);
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };

        while (left < right)
        {
            // Find vowel from the left
            while (left < right && !isVowel(s[left]))
            {
                left++;
            }

            // Find vowel from the right
            while (left < right && !isVowel(s[right]))
            {
                right--;
            }

            // Swap vowels
            if (left < right)
            {
                std::swap(s[left], s[right]);
                left++;
                right--;
            }
        }

        return s;
    }

} // namespace ConvergingPointers

// ============================================================================
// 2. Same Direction (Fast & Slow) Pattern
// ============================================================================

namespace FastSlowPointers
{

    /**
     * Problem: Remove Duplicates from Sorted Array
     * Given a sorted array, remove duplicates in-place and return the new length.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    int removeDuplicates(std::vector<int> &nums)
    {
        if (nums.empty())
        {
            return 0;
        }

        // Slow pointer points to the position where next unique element will be placed
        int slow = 0;

        // Fast pointer scans through the array finding unique elements
        for (int fast = 1; fast < static_cast<int>(nums.size()); fast++)
        {
            if (nums[fast] != nums[slow])
            {
                // Found a new unique element
                slow++;
                nums[slow] = nums[fast];
            }
            // If duplicate, fast continues advancing but slow stays put
        }

        // The length of the resulting array is (slow + 1)
        return slow + 1;
    }

    /**
     * Problem: Remove Element
     * Given an array and a value, remove all instances of that value in-place and return the new length.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    int removeElement(std::vector<int> &nums, int val)
    {
        int slow = 0;

        for (int fast = 0; fast < static_cast<int>(nums.size()); fast++)
        {
            if (nums[fast] != val)
            {
                nums[slow] = nums[fast];
                slow++;
            }
            // If the current element matches the value, fast advances but slow stays put
        }

        return slow;
    }

    /**
     * Problem: Move Zeroes
     * Move all zeroes to the end of the array while maintaining the relative order of non-zero elements.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    void moveZeroes(std::vector<int> &nums)
    {
        int slow = 0; // Points to the position where next non-zero element will be placed

        // First pass: move all non-zero elements to the front
        for (int fast = 0; fast < static_cast<int>(nums.size()); fast++)
        {
            if (nums[fast] != 0)
            {
                nums[slow] = nums[fast];
                slow++;
            }
        }

        // Second pass: fill the remaining positions with zeros
        for (int i = slow; i < static_cast<int>(nums.size()); i++)
        {
            nums[i] = 0;
        }
    }

    /**
     * Alternative implementation of moveZeroes with a single pass
     */
    void moveZeroesSinglePass(std::vector<int> &nums)
    {
        int slow = 0; // Points to the position where next non-zero element will be placed

        for (int fast = 0; fast < static_cast<int>(nums.size()); fast++)
        {
            if (nums[fast] != 0)
            {
                if (slow != fast)
                {
                    std::swap(nums[slow], nums[fast]);
                }
                slow++;
            }
        }
    }

    /**
     * Problem: Find the Duplicate Number
     * Given an array of integers containing n + 1 integers where each integer is in the range [1, n],
     * find the duplicate number (there is exactly one duplicate).
     *
     * This implementation uses Floyd's Cycle Finding Algorithm (Tortoise and Hare).
     *
     * Time Complexity: O(n) - The fast pointer advances twice as fast, finding the cycle in O(n) time
     * Space Complexity: O(1) - Constant extra space
     */
    int findDuplicate(std::vector<int> &nums)
    {
        // Phase 1: Find the intersection point inside the cycle
        int slow = nums[0];
        int fast = nums[0];

        do
        {
            slow = nums[slow];       // Move one step
            fast = nums[nums[fast]]; // Move two steps
        } while (slow != fast);

        // Phase 2: Find the entrance to the cycle
        slow = nums[0];
        while (slow != fast)
        {
            slow = nums[slow];
            fast = nums[fast];
        }

        return slow;
    }

    /**
     * Problem: Linked List Cycle
     * Determine if a linked list has a cycle.
     *
     * Time Complexity: O(n) - If there's a cycle, the fast pointer will eventually catch up to the slow pointer
     * Space Complexity: O(1) - Constant extra space
     */
    bool hasCycle(ListNode *head)
    {
        if (!head || !head->next)
        {
            return false;
        }

        ListNode *slow = head;
        ListNode *fast = head;

        while (fast && fast->next)
        {
            slow = slow->next;       // Move one step
            fast = fast->next->next; // Move two steps

            if (slow == fast)
            {
                return true; // Cycle detected
            }
        }

        return false; // No cycle
    }

    /**
     * Problem: Linked List Cycle II
     * Given a linked list, return the node where the cycle begins.
     *
     * Time Complexity: O(n) - Similar to Floyd's algorithm
     * Space Complexity: O(1) - Constant extra space
     */
    ListNode *detectCycle(ListNode *head)
    {
        if (!head || !head->next)
        {
            return nullptr;
        }

        // Phase 1: Find meeting point inside the cycle
        ListNode *slow = head;
        ListNode *fast = head;

        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast)
            {
                // Phase 2: Find the entrance to the cycle
                slow = head;
                while (slow != fast)
                {
                    slow = slow->next;
                    fast = fast->next;
                }
                return slow; // Cycle start found
            }
        }

        return nullptr; // No cycle
    }

    /**
     * Problem: Middle of the Linked List
     * Return the middle node of a linked list.
     *
     * Time Complexity: O(n) - Fast pointer reaches the end in n/2 iterations
     * Space Complexity: O(1) - Constant extra space
     */
    ListNode *middleNode(ListNode *head)
    {
        if (!head)
        {
            return nullptr;
        }

        ListNode *slow = head;
        ListNode *fast = head;

        while (fast && fast->next)
        {
            slow = slow->next;       // Move one step
            fast = fast->next->next; // Move two steps
        }

        return slow; // When fast reaches the end, slow is at the middle
    }

    /**
     * Problem: Happy Number
     * Determine if a number is "happy". A happy number is defined by:
     * 1. Replace the number by the sum of the squares of its digits
     * 2. Repeat until the number equals 1 (happy) or the process cycles (not happy)
     *
     * Time Complexity: O(log n) - Number of digits decreases logarithmically
     * Space Complexity: O(1) - Constant extra space
     */
    bool isHappy(int n)
    {
        auto getNext = [](int num)
        {
            int sum = 0;
            while (num > 0)
            {
                int digit = num % 10;
                sum += digit * digit;
                num /= 10;
            }
            return sum;
        };

        int slow = n;
        int fast = getNext(n);

        while (fast != 1 && slow != fast)
        {
            slow = getNext(slow);          // Move one step
            fast = getNext(getNext(fast)); // Move two steps
        }

        return fast == 1; // If we end up at 1, it's a happy number
    }

} // namespace FastSlowPointers

// ============================================================================
// 3. Sliding Window Pattern
// ============================================================================

namespace SlidingWindow
{

    /**
     * Problem: Maximum Subarray Sum
     * Find the contiguous subarray with the largest sum.
     *
     * This is Kadane's algorithm, which can be seen as a special case of sliding window.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    int maxSubArray(std::vector<int> &nums)
    {
        if (nums.empty())
        {
            return 0;
        }

        int currentSum = nums[0];
        int maxSum = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); i++)
        {
            // Either start a new subarray or extend the existing one
            currentSum = std::max(nums[i], currentSum + nums[i]);
            maxSum = std::max(maxSum, currentSum);
        }

        return maxSum;
    }

    /**
     * Problem: Minimum Size Subarray Sum
     * Find the minimum length contiguous subarray with a sum ≥ target.
     * Return 0 if no such subarray exists.
     *
     * Time Complexity: O(n) - Each element is processed at most twice (added and removed once)
     * Space Complexity: O(1) - Constant extra space
     */
    int minSubArrayLen(int target, std::vector<int> &nums)
    {
        int left = 0;
        int sum = 0;
        int minLength = INT_MAX;

        for (int right = 0; right < static_cast<int>(nums.size()); right++)
        {
            sum += nums[right]; // Expand the window

            // Shrink the window as small as possible while maintaining sum >= target
            while (sum >= target)
            {
                minLength = std::min(minLength, right - left + 1);
                sum -= nums[left];
                left++;
            }
        }

        return (minLength == INT_MAX) ? 0 : minLength;
    }

    /**
     * Problem: Longest Substring Without Repeating Characters
     * Find the length of the longest substring without repeating characters.
     *
     * Time Complexity: O(n) - Each character is processed at most twice
     * Space Complexity: O(min(m, n)) - Where m is the size of the character set
     */
    int lengthOfLongestSubstring(std::string s)
    {
        std::unordered_map<char, int> charIndex; // Maps character to its last seen index
        int left = 0;
        int maxLength = 0;

        for (int right = 0; right < static_cast<int>(s.size()); right++)
        {
            char currentChar = s[right];

            // If we've seen this character before and it's within our current window
            if (charIndex.find(currentChar) != charIndex.end() &&
                charIndex[currentChar] >= left)
            {
                // Move left pointer to the next position after the last occurrence
                left = charIndex[currentChar] + 1;
            }

            // Update the last seen index of the current character
            charIndex[currentChar] = right;

            // Update the maximum length
            maxLength = std::max(maxLength, right - left + 1);
        }

        return maxLength;
    }

    /**
     * Alternative implementation using a set instead of a map
     */
    int lengthOfLongestSubstringSet(std::string s)
    {
        std::unordered_set<char> charSet;
        int left = 0;
        int maxLength = 0;

        for (int right = 0; right < static_cast<int>(s.size()); right++)
        {
            // While we have a repeating character in our set
            while (charSet.count(s[right]))
            {
                charSet.erase(s[left]);
                left++;
            }

            charSet.insert(s[right]);
            maxLength = std::max(maxLength, right - left + 1);
        }

        return maxLength;
    }

    /**
     * Problem: Find All Anagrams in a String
     * Find all the start indices of anagrams of string p in string s.
     *
     * Time Complexity: O(n) - Where n is the length of string s
     * Space Complexity: O(1) - Constant space for the character frequency arrays
     */
    std::vector<int> findAnagrams(std::string s, std::string p)
    {
        std::vector<int> result;

        if (s.empty() || p.empty() || s.size() < p.size())
        {
            return result;
        }

        // Create frequency arrays for p and the current window in s
        std::vector<int> pCount(26, 0);
        std::vector<int> sCount(26, 0);

        // Count frequencies in p
        for (char c : p)
        {
            pCount[c - 'a']++;
        }

        // Initialize the first window
        for (int i = 0; i < static_cast<int>(p.size()); i++)
        {
            sCount[s[i] - 'a']++;
        }

        // Check if the first window is an anagram
        if (pCount == sCount)
        {
            result.push_back(0);
        }

        // Slide the window
        for (int i = p.size(); i < s.size(); i++)
        {
            // Add the next character to the window
            sCount[s[i] - 'a']++;

            // Remove the leftmost character from the window
            sCount[s[i - p.size()] - 'a']--;

            // Check if the current window is an anagram
            if (pCount == sCount)
            {
                result.push_back(i - p.size() + 1);
            }
        }

        return result;
    }

    /**
     * Problem: Longest Repeating Character Replacement
     * Find the length of the longest substring containing the same letter after
     * replacing at most k characters.
     *
     * Time Complexity: O(n) - Each character is processed once
     * Space Complexity: O(1) - Constant space for the frequency array
     */
    int characterReplacement(std::string s, int k)
    {
        std::vector<int> count(26, 0);
        int left = 0;
        int maxCount = 0; // Max frequency of any character in the current window
        int maxLength = 0;

        for (int right = 0; right < static_cast<int>(s.size()); right++)
        {
            count[s[right] - 'A']++;
            maxCount = std::max(maxCount, count[s[right] - 'A']);

            // If the number of characters to replace exceeds k, shrink the window
            if (right - left + 1 - maxCount > k)
            {
                count[s[left] - 'A']--;
                left++;
            }

            maxLength = std::max(maxLength, right - left + 1);
        }

        return maxLength;
    }

    /**
     * Problem: Permutation in String
     * Determine if one string is a permutation of a substring of another string.
     *
     * Time Complexity: O(n) - Where n is the length of string s2
     * Space Complexity: O(1) - Constant space for the frequency arrays
     */
    bool checkInclusion(std::string s1, std::string s2)
    {
        if (s1.size() > s2.size())
        {
            return false;
        }

        // Create frequency arrays for s1 and the current window in s2
        std::vector<int> s1Count(26, 0);
        std::vector<int> s2Count(26, 0);

        // Count frequencies in s1
        for (char c : s1)
        {
            s1Count[c - 'a']++;
        }

        // Initialize the first window
        for (int i = 0; i < static_cast<int>(s1.size()); i++)
        {
            s2Count[s2[i] - 'a']++;
        }

        // Check if the first window is a permutation
        if (s1Count == s2Count)
        {
            return true;
        }

        // Slide the window
        for (int i = s1.size(); i < s2.size(); i++)
        {
            // Add the next character to the window
            s2Count[s2[i] - 'a']++;

            // Remove the leftmost character from the window
            s2Count[s2[i - s1.size()] - 'a']--;

            // Check if the current window is a permutation
            if (s1Count == s2Count)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * Problem: Subarray Product Less Than K
     * Find the number of contiguous subarrays where the product of all the elements is less than k.
     *
     * Time Complexity: O(n) - Each element is processed at most twice
     * Space Complexity: O(1) - Constant extra space
     */
    int numSubarrayProductLessThanK(std::vector<int> &nums, int k)
    {
        if (k <= 1)
        {
            return 0; // No valid subarray since all elements are positive
        }

        int left = 0;
        int product = 1;
        int count = 0;

        for (int right = 0; right < static_cast<int>(nums.size()); right++)
        {
            product *= nums[right]; // Expand the window

            // Shrink the window until product < k
            while (product >= k)
            {
                product /= nums[left];
                left++;
            }

            // Add the number of valid subarrays ending at right
            count += right - left + 1;
        }

        return count;
    }

} // namespace SlidingWindow

// ============================================================================
// Advanced Two-Pointer Techniques
// ============================================================================

namespace AdvancedTwoPointer
{

    /**
     * Problem: Trapping Rain Water
     * Calculate how much rainwater can be trapped between buildings.
     *
     * This uses the two-pointer approach with additional tracking of left and right maximums.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    int trap(std::vector<int> &height)
    {
        if (height.empty())
        {
            return 0;
        }

        int left = 0;
        int right = height.size() - 1;
        int leftMax = 0;
        int rightMax = 0;
        int water = 0;

        while (left < right)
        {
            if (height[left] < height[right])
            {
                // The amount of water above the current position depends on the left max
                if (height[left] >= leftMax)
                {
                    leftMax = height[left];
                }
                else
                {
                    water += leftMax - height[left];
                }
                left++;
            }
            else
            {
                // The amount of water above the current position depends on the right max
                if (height[right] >= rightMax)
                {
                    rightMax = height[right];
                }
                else
                {
                    water += rightMax - height[right];
                }
                right--;
            }
        }

        return water;
    }

    /**
     * Problem: Minimum Window Substring
     * Find the minimum window in string s that contains all characters of string t.
     *
     * This is a more complex sliding window problem that uses a map to track frequencies.
     *
     * Time Complexity: O(n) - Where n is the length of string s
     * Space Complexity: O(m) - Where m is the number of unique characters in string t
     */
    std::string minWindow(std::string s, std::string t)
    {
        if (s.empty() || t.empty())
        {
            return "";
        }

        // Count frequencies of characters in t
        std::unordered_map<char, int> targetFreq;
        for (char c : t)
        {
            targetFreq[c]++;
        }

        int required = targetFreq.size(); // Number of unique characters in t
        int formed = 0;                   // Number of unique characters in current window that meet or exceed the required frequency

        std::unordered_map<char, int> windowFreq;
        int left = 0;
        int right = 0;

        // To store the minimum window info
        int minLen = INT_MAX;
        int minLeft = 0;

        while (right < static_cast<int>(s.size()))
        {
            // Add the current character to the window
            char c = s[right];
            windowFreq[c]++;

            // Check if this character's frequency meets or exceeds the requirement
            if (targetFreq.count(c) && windowFreq[c] == targetFreq[c])
            {
                formed++;
            }

            // Try to shrink the window from the left
            while (left <= right && formed == required)
            {
                c = s[left];

                // Update the minimum window
                if (right - left + 1 < minLen)
                {
                    minLen = right - left + 1;
                    minLeft = left;
                }

                // Remove the leftmost character from the window
                windowFreq[c]--;

                // Check if removing this character breaks a requirement
                if (targetFreq.count(c) && windowFreq[c] < targetFreq[c])
                {
                    formed--;
                }

                left++;
            }

            right++;
        }

        return minLen == INT_MAX ? "" : s.substr(minLeft, minLen);
    }

    /**
     * Problem: Sliding Window Maximum
     * Find the maximum element in each sliding window of size k.
     *
     * This uses a deque to efficiently track candidates for the maximum in each window.
     *
     * Time Complexity: O(n) - Each element is processed once
     * Space Complexity: O(k) - Maximum size of the deque
     */
    std::vector<int> maxSlidingWindow(std::vector<int> &nums, int k)
    {
        std::vector<int> result;
        std::deque<int> dq; // Store indices of potential maximum elements

        for (int i = 0; i < static_cast<int>(nums.size()); i++)
        {
            // Remove elements that are out of the current window
            while (!dq.empty() && dq.front() < i - k + 1)
            {
                dq.pop_front();
            }

            // Remove smaller elements from the back (they can't be the maximum)
            while (!dq.empty() && nums[dq.back()] < nums[i])
            {
                dq.pop_back();
            }

            // Add current element (its index) to the back
            dq.push_back(i);

            // If we have a full window, add the maximum to the result
            if (i >= k - 1)
            {
                result.push_back(nums[dq.front()]);
            }
        }

        return result;
    }

    /**
     * Problem: Sort Colors (Dutch National Flag Problem)
     * Sort an array containing only 0s, 1s, and 2s in-place.
     *
     * This is a three-pointer variant of the algorithm.
     *
     * Time Complexity: O(n) - Single pass through the array
     * Space Complexity: O(1) - Constant extra space
     */
    void sortColors(std::vector<int> &nums)
    {
        int low = 0;                // For 0s
        int mid = 0;                // For 1s
        int high = nums.size() - 1; // For 2s

        while (mid <= high)
        {
            if (nums[mid] == 0)
            {
                std::swap(nums[low], nums[mid]);
                low++;
                mid++;
            }
            else if (nums[mid] == 1)
            {
                mid++;
            }
            else
            { // nums[mid] == 2
                std::swap(nums[mid], nums[high]);
                high--;
                // Don't increment mid here, as we need to check the swapped element
            }
        }
    }

} // namespace AdvancedTwoPointer

// ============================================================================
// Benchmarking and Performance Comparison
// ============================================================================

/**
 * Compare the performance of the brute force and two-pointer approaches
 * for the Two Sum problem.
 */
void benchmarkTwoSum()
{
    std::cout << "===== Benchmarking Two Sum Implementations =====" << std::endl;

    // Create a sorted array for testing
    std::vector<int> nums;
    for (int i = 1; i <= 10000; i++)
    {
        nums.push_back(i);
    }

    int target = 10001; // Will make both algorithms traverse the full array

    // Two Sum - Brute Force O(n²)
    auto bruteForce = [&nums, target]()
    {
        for (size_t i = 0; i < nums.size(); i++)
        {
            for (size_t j = i + 1; j < nums.size(); j++)
            {
                if (nums[i] + nums[j] == target)
                {
                    return std::vector<int>{static_cast<int>(i) + 1, static_cast<int>(j) + 1};
                }
            }
        }
        return std::vector<int>{-1, -1};
    };

    // Two Sum - Two Pointer O(n)
    auto twoPointer = [&nums, target]()
    {
        return ConvergingPointers::twoSum(nums, target);
    };

    // Benchmark
    long long bruteForceTime = measureExecutionTime(bruteForce);
    long long twoPointerTime = measureExecutionTime(twoPointer);

    std::cout << "Brute Force Time: " << bruteForceTime << " microseconds" << std::endl;
    std::cout << "Two Pointer Time: " << twoPointerTime << " microseconds" << std::endl;
    std::cout << "Speedup: " << static_cast<double>(bruteForceTime) / twoPointerTime << "x" << std::endl;
    std::cout << std::endl;
}

/**
 * Compare the performance of the two implementations of moveZeroes
 */
void benchmarkMoveZeroes()
{
    std::cout << "===== Benchmarking Move Zeroes Implementations =====" << std::endl;

    // Create a test array
    std::vector<int> nums1(100000);
    std::vector<int> nums2(100000);

    // Fill with random data (about 50% zeros)
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 100000; i++)
    {
        int val = rand() % 2 == 0 ? 0 : rand() % 100;
        nums1[i] = val;
        nums2[i] = val;
    }

    // Two-pass implementation
    auto twoPass = [&nums1]()
    {
        FastSlowPointers::moveZeroes(nums1);
    };

    // Single-pass implementation
    auto singlePass = [&nums2]()
    {
        FastSlowPointers::moveZeroesSinglePass(nums2);
    };

    // Benchmark
    long long twoPassTime = measureExecutionTime(twoPass);
    long long singlePassTime = measureExecutionTime(singlePass);

    std::cout << "Two-Pass Time: " << twoPassTime << " microseconds" << std::endl;
    std::cout << "Single-Pass Time: " << singlePassTime << " microseconds" << std::endl;

    // Determine which is faster
    if (twoPassTime < singlePassTime)
    {
        std::cout << "Two-Pass is faster by " << static_cast<double>(singlePassTime) / twoPassTime << "x" << std::endl;
    }
    else
    {
        std::cout << "Single-Pass is faster by " << static_cast<double>(twoPassTime) / singlePassTime << "x" << std::endl;
    }
    std::cout << std::endl;
}

/**
 * Compare the performance of the map-based and set-based implementations
 * of lengthOfLongestSubstring
 */
void benchmarkLongestSubstring()
{
    std::cout << "===== Benchmarking Longest Substring Implementations =====" << std::endl;

    // Create a test string
    std::string s;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 100000; i++)
    {
        s += static_cast<char>('a' + rand() % 26);
    }

    // Map-based implementation
    auto mapBased = [&s]()
    {
        return SlidingWindow::lengthOfLongestSubstring(s);
    };

    // Set-based implementation
    auto setBased = [&s]()
    {
        return SlidingWindow::lengthOfLongestSubstringSet(s);
    };

    // Benchmark
    long long mapTime = measureExecutionTime(mapBased);
    long long setTime = measureExecutionTime(setBased);

    std::cout << "Map-Based Time: " << mapTime << " microseconds" << std::endl;
    std::cout << "Set-Based Time: " << setTime << " microseconds" << std::endl;

    // Determine which is faster
    if (mapTime < setTime)
    {
        std::cout << "Map-Based is faster by " << static_cast<double>(setTime) / mapTime << "x" << std::endl;
    }
    else
    {
        std::cout << "Set-Based is faster by " << static_cast<double>(mapTime) / setTime << "x" << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================================
// Main Function with Examples and Demonstrations
// ============================================================================

int main()
{
    std::cout << "=======================================================" << std::endl;
    std::cout << "           Two-Pointer Technique Demonstrations         " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << std::endl;

    // ----- Converging Pointers Examples ----- //
    std::cout << "===== Converging Pointers Examples =====" << std::endl;

    // Two Sum
    std::vector<int> numbers = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> twoSumResult = ConvergingPointers::twoSum(numbers, target);
    std::cout << "Two Sum Result: [" << twoSumResult[0] << ", " << twoSumResult[1] << "]" << std::endl;

    // Valid Palindrome
    std::string palindrome = "A man, a plan, a canal: Panama";
    std::cout << "Is \"" << palindrome << "\" a palindrome? "
              << (ConvergingPointers::isPalindrome(palindrome) ? "Yes" : "No") << std::endl;

    // Container With Most Water
    std::vector<int> heights = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    std::cout << "Maximum water container area: " << ConvergingPointers::maxArea(heights) << std::endl;

    // 3Sum
    std::vector<int> threeSumNums = {-1, 0, 1, 2, -1, -4};
    std::vector<std::vector<int>> threeSumResult = ConvergingPointers::threeSum(threeSumNums);
    std::cout << "3Sum Results:" << std::endl;
    for (const auto &triplet : threeSumResult)
    {
        std::cout << "  [" << triplet[0] << ", " << triplet[1] << ", " << triplet[2] << "]" << std::endl;
    }

    // Reverse Vowels
    std::string vowelString = "hello";
    std::string reversedVowels = ConvergingPointers::reverseVowels(vowelString);
    std::cout << "Reversed vowels in \"" << vowelString << "\": \"" << reversedVowels << "\"" << std::endl;

    std::cout << std::endl;

    // ----- Fast & Slow Pointers Examples ----- //
    std::cout << "===== Fast & Slow Pointers Examples =====" << std::endl;

    // Remove Duplicates
    std::vector<int> dupsArray = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    std::vector<int> dupsArrayCopy = dupsArray;
    int newLength = FastSlowPointers::removeDuplicates(dupsArray);
    std::cout << "Original array: ";
    printVector(dupsArrayCopy);
    std::cout << "After removing duplicates (length " << newLength << "): ";
    printVector(std::vector<int>(dupsArray.begin(), dupsArray.begin() + newLength));

    // Move Zeroes
    std::vector<int> zeroesArray = {0, 1, 0, 3, 12};
    std::vector<int> zeroesArrayCopy = zeroesArray;
    FastSlowPointers::moveZeroes(zeroesArray);
    std::cout << "Original array: ";
    printVector(zeroesArrayCopy);
    std::cout << "After moving zeroes: ";
    printVector(zeroesArray);

    // Find Duplicate
    std::vector<int> dupNums = {1, 3, 4, 2, 2};
    std::cout << "Duplicate number in " << "{1, 3, 4, 2, 2}" << ": "
              << FastSlowPointers::findDuplicate(dupNums) << std::endl;

    // Middle of Linked List
    std::vector<int> listValues = {1, 2, 3, 4, 5};
    ListNode *head = createLinkedList(listValues);
    ListNode *mid = FastSlowPointers::middleNode(head);
    std::cout << "Original linked list: ";
    printLinkedList(head);
    std::cout << "Middle node: " << mid->val << std::endl;

    // Happy Number
    int happyCandidate = 19;
    std::cout << "Is " << happyCandidate << " a happy number? "
              << (FastSlowPointers::isHappy(happyCandidate) ? "Yes" : "No") << std::endl;

    // Clean up the linked list
    deleteLinkedList(head);

    std::cout << std::endl;

    // ----- Sliding Window Examples ----- //
    std::cout << "===== Sliding Window Examples =====" << std::endl;

    // Maximum Subarray
    std::vector<int> subArray = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    std::cout << "Maximum subarray sum for ";
    printVector(subArray);
    std::cout << "is: " << SlidingWindow::maxSubArray(subArray) << std::endl;

    // Minimum Size Subarray Sum
    std::vector<int> subArraySum = {2, 3, 1, 2, 4, 3};
    int targetSum = 7;
    std::cout << "Minimum length subarray with sum >= " << targetSum << ": "
              << SlidingWindow::minSubArrayLen(targetSum, subArraySum) << std::endl;

    // Longest Substring Without Repeating Characters
    std::string subString = "abcabcbb";
    std::cout << "Longest substring without repeating characters in \"" << subString << "\": "
              << SlidingWindow::lengthOfLongestSubstring(subString) << std::endl;

    // Find All Anagrams
    std::string s = "cbaebabacd";
    std::string p = "abc";
    std::vector<int> anagramIndices = SlidingWindow::findAnagrams(s, p);
    std::cout << "Anagrams of \"" << p << "\" in \"" << s << "\" start at indices: ";
    printVector(anagramIndices);

    // Subarray Product Less Than K
    std::vector<int> productArray = {10, 5, 2, 6};
    int k = 100;
    std::cout << "Number of subarrays with product < " << k << ": "
              << SlidingWindow::numSubarrayProductLessThanK(productArray, k) << std::endl;

    std::cout << std::endl;

    // ----- Advanced Two-Pointer Examples ----- //
    std::cout << "===== Advanced Two-Pointer Examples =====" << std::endl;

    // Trapping Rain Water
    std::vector<int> waterHeights = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    std::cout << "Water trapped: " << AdvancedTwoPointer::trap(waterHeights) << " units" << std::endl;

    // Minimum Window Substring
    std::string s1 = "ADOBECODEBANC";
    std::string t = "ABC";
    std::string minWindowResult = AdvancedTwoPointer::minWindow(s1, t);
    std::cout << "Minimum window substring containing \"" << t << "\" in \"" << s1 << "\": \""
              << minWindowResult << "\"" << std::endl;

    // Sliding Window Maximum
    std::vector<int> slidingNums = {1, 3, -1, -3, 5, 3, 6, 7};
    int windowSize = 3;
    std::vector<int> maxWindows = AdvancedTwoPointer::maxSlidingWindow(slidingNums, windowSize);
    std::cout << "Maximum values in sliding windows of size " << windowSize << ": ";
    printVector(maxWindows);

    // Sort Colors
    std::vector<int> colors = {2, 0, 2, 1, 1, 0};
    std::vector<int> colorsCopy = colors;
    AdvancedTwoPointer::sortColors(colors);
    std::cout << "Original colors: ";
    printVector(colorsCopy);
    std::cout << "Sorted colors: ";
    printVector(colors);

    std::cout << std::endl;

    // ----- Performance Benchmarks ----- //
    benchmarkTwoSum();
    benchmarkMoveZeroes();
    benchmarkLongestSubstring();

    return 0;
}